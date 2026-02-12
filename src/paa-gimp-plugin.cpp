/*
 * author: fixitdaztv
 * date: 2026-02-12
 */

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>
#include <vector>
#include <string>
#include <cstdint>

/* * MANUAL OVERRIDE: 
 * We define the library structure here to bypass the header's DLL import logic.
 * We use __asm__ to tell the linker exactly which symbol to find in the .a file.
 */
namespace grad_aff {
    
    struct MipMap {
        uint16_t width;
        uint16_t height;
        uint32_t dataLength;
        std::vector<uint8_t> data;
    };

    class Paa {
    public:
        enum class TypeOfPaX : uint32_t { RGBA8888 = 0x44475854 };

        // 1. Map the Constructor
        Paa() __asm__("_ZN8grad_aff3PaaC1Ev");

        std::vector<MipMap> mipMaps;
        TypeOfPaX typeOfPax;
        bool hasTransparency = true;

        // 2. Map readPaa (Matches: std::string, bool)
        void readPaa(std::string path, bool peek = false) __asm__("_ZN8grad_aff3Paa7readPaaENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEb");

        // 3. Map writePaa
        void writePaa(std::string path, TypeOfPaX type = TypeOfPaX::RGBA8888) __asm__("_ZN8grad_aff3Paa8writePaaENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEENS0_9TypeOfPaXE");
    };
}

/* GIMP Plugin Logic starts here */

#define PAA_TYPE_PLUG_IN (paa_plug_in_get_type())
G_DECLARE_FINAL_TYPE (PaaPlugIn, paa_plug_in, PAA, PLUG_IN, GimpPlugIn)

struct _PaaPlugIn { GimpPlugIn parent_instance; };
G_DEFINE_TYPE (PaaPlugIn, paa_plug_in, GIMP_TYPE_PLUG_IN)

static void paa_plug_in_init (PaaPlugIn *plug_in) {}

static GList* paa_query_procedures (GimpPlugIn *plug_in) {
    GList *list = NULL;
    list = g_list_append (list, g_strdup ("file-paa-load"));
    list = g_list_append (list, g_strdup ("file-paa-save"));
    return list;
}

static GimpValueArray* paa_load (GimpProcedure *procedure, GimpRunMode run_mode, GFile *file, GimpMetadata *metadata, GimpMetadataLoadFlags *flags, GimpProcedureConfig *config, gpointer run_data) {
    gchar *path = g_file_get_path(file);
    grad_aff::Paa paa;
    
    try {
        // Matches the manual signature above
        paa.readPaa(std::string(path), false); 
    } catch (...) {
        g_free(path);
        return gimp_procedure_new_return_values (procedure, GIMP_PDB_EXECUTION_ERROR, NULL);
    }
    g_free(path);

    if (paa.mipMaps.empty()) return gimp_procedure_new_return_values (procedure, GIMP_PDB_EXECUTION_ERROR, NULL);

    int w = paa.mipMaps[0].width;
    int h = paa.mipMaps[0].height;
    GimpImage *image = gimp_image_new (w, h, GIMP_RGB);
    GimpLayer *layer = gimp_layer_new (image, "PAA Layer", w, h, paa.hasTransparency ? GIMP_RGBA_IMAGE : GIMP_RGB_IMAGE, 100.0, GIMP_LAYER_MODE_NORMAL);
    gimp_image_insert_layer (image, layer, NULL, 0);

    GeglBuffer *buffer = gimp_drawable_get_buffer (GIMP_DRAWABLE (layer));
    gegl_buffer_set (buffer, GEGL_RECTANGLE (0, 0, w, h), 0, paa.hasTransparency ? babl_format ("R'G'B'A u8") : babl_format ("R'G'B' u8"), paa.mipMaps[0].data.data(), GEGL_AUTO_ROWSTRIDE);
    g_object_unref (buffer);

    GimpValueArray *return_vals = gimp_procedure_new_return_values (procedure, GIMP_PDB_SUCCESS, NULL);
    GValue value = G_VALUE_INIT;
    g_value_init (&value, GIMP_TYPE_IMAGE);
    g_value_set_object (&value, image);
    gimp_value_array_append (return_vals, &value);
    
    return return_vals;
}

static bool is_power_of_two(uint32_t x) { return (x != 0) && ((x & (x - 1)) == 0); }

static GimpValueArray* paa_save (GimpProcedure *procedure, GimpRunMode run_mode, GimpImage *image, GimpDrawable **drawables, GimpProcedureConfig *config, gpointer run_data) {
    GimpDrawable *drawable = drawables[0];
    int w = gimp_drawable_get_width (drawable);
    int h = gimp_drawable_get_height (drawable);

    if (!is_power_of_two(w) || !is_power_of_two(h)) return gimp_procedure_new_return_values (procedure, GIMP_PDB_EXECUTION_ERROR, NULL);

    GFile *file = NULL;
    g_object_get (config, "file", &file, NULL);
    gchar *path = g_file_get_path(file);
    if (file) g_object_unref(file);

    GeglBuffer *buffer = gimp_drawable_get_buffer (drawable);
    std::vector<uint8_t> pixel_data(w * h * 4);
    gegl_buffer_get (buffer, GEGL_RECTANGLE (0, 0, w, h), 1.0, babl_format ("R'G'B'A u8"), pixel_data.data(), GEGL_AUTO_ROWSTRIDE, GEGL_ABYSS_NONE);
    g_object_unref (buffer);

    try {
        grad_aff::Paa paa;
        grad_aff::MipMap mip; 
        mip.width = (uint16_t)w; 
        mip.height = (uint16_t)h;
        mip.data = pixel_data; 
        mip.dataLength = (uint32_t)pixel_data.size();
        
        paa.mipMaps.push_back(mip);
        paa.typeOfPax = grad_aff::Paa::TypeOfPaX::RGBA8888;
        paa.writePaa(std::string(path));
    } catch (...) { g_free(path); return gimp_procedure_new_return_values (procedure, GIMP_PDB_EXECUTION_ERROR, NULL); }

    g_free(path);
    return gimp_procedure_new_return_values (procedure, GIMP_PDB_SUCCESS, NULL);
}

static GimpProcedure* paa_create_procedure (GimpPlugIn *plug_in, const gchar *name) {
    GimpProcedure *procedure = NULL;

    if (g_strcmp0 (name, "file-paa-load") == 0) {
        procedure = gimp_load_procedure_new (plug_in, name, GIMP_PDB_PROC_TYPE_PLUGIN, paa_load, NULL, NULL);
        gimp_procedure_set_menu_label (procedure, "PAA Image");
        gimp_file_procedure_set_extensions (GIMP_FILE_PROCEDURE (procedure), "paa");
        gimp_file_procedure_set_magics (GIMP_FILE_PROCEDURE (procedure), "0,string,PAX");
    }
    else if (g_strcmp0 (name, "file-paa-save") == 0) {
        procedure = gimp_image_procedure_new (plug_in, name, GIMP_PDB_PROC_TYPE_PLUGIN, paa_save, NULL, NULL);
        gimp_procedure_set_menu_label (procedure, "PAA Image");
        gimp_file_procedure_set_extensions (GIMP_FILE_PROCEDURE (procedure), "paa");
    }
    return procedure;
}

static void paa_plug_in_class_init (PaaPlugInClass *klass) {
    GimpPlugInClass *plug_in_class = GIMP_PLUG_IN_CLASS (klass);
    plug_in_class->query_procedures = paa_query_procedures;
    plug_in_class->create_procedure = paa_create_procedure;
}

GIMP_MAIN (PAA_TYPE_PLUG_IN)