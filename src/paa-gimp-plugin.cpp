/*
 * author: fixitdaztv
 * date: 2026-02-12
 */

#include "paa-gimp-plugin.h"

G_DEFINE_TYPE (PaaPlugIn, paa_plug_in, GIMP_TYPE_PLUG_IN)

static void paa_plug_in_class_init (PaaPlugInClass* klass) {
    GimpPlugInClass* plugin_class = GIMP_PLUG_IN_CLASS (klass);

    /* Register the load and save procedures */
    plugin_class->do_query_procedures = [](GimpPlugIn* plugin) -> GList* {
        GList* list = NULL;
        list = g_list_append (list, g_strdup (LOAD_PROC));
        list = g_list_append (list, g_strdup (SAVE_PROC));
        return list;
    };

    /* Handle procedure creation based on the registered names */
    plugin_class->do_create_procedure = [](GimpPlugIn* plugin, const gchar* name) -> GimpProcedure* {
        GimpProcedure* procedure = NULL;
        if (strcmp (name, LOAD_PROC) == 0) {
            procedure = gimp_load_procedure_new (plugin, name, GIMP_PDB_PROC_TYPE_PLUGIN, (GimpRunFunc)paa_load, NULL, NULL);
            gimp_procedure_set_menu_label (procedure, "PAA Image");
            gimp_file_procedure_set_extensions (GIMP_FILE_PROCEDURE (procedure), "paa");
        } else if (strcmp (name, SAVE_PROC) == 0) {
            procedure = gimp_save_procedure_new (plugin, name, GIMP_PDB_PROC_TYPE_PLUGIN, (GimpRunFunc)paa_save, NULL, NULL);
            gimp_procedure_set_menu_label (procedure, "PAA Image");
            gimp_file_procedure_set_extensions (GIMP_FILE_PROCEDURE (procedure), "paa");
            gimp_save_procedure_set_format_name (GIMP_SAVE_PROCEDURE (procedure), "PAA");
        }
        return procedure;
    };
}

static void paa_plug_in_init (PaaPlugIn* plugin) {}

static GimpValueArray* paa_load(GimpProcedure* procedure, GimpRunMode run_mode, GFile* file, const GimpValueArray* args, gpointer data) {
    gchar* path = g_file_get_path(file);
    auto paa = grad_aff::Paa();

    try {
        paa.readPaa(path);
    } catch (const std::exception& e) {
        gimp_message(e.what());
        g_free(path);
        return gimp_procedure_new_return_values(procedure, GIMP_PDB_EXECUTION_ERROR, NULL);
    }

    auto width = paa.mipMaps[0].width;
    auto height = paa.mipMaps[0].height;

    /* GIMP 3.0 uses object-oriented image and layer creation */
    GimpImage* image = gimp_image_new(width, height, GIMP_RGB);
    GimpLayer* layer = gimp_layer_new(image, "PAA Layer", width, height, 
                                     paa.hasTransparency ? GIMP_RGBA_IMAGE : GIMP_RGB_IMAGE, 
                                     100, GIMP_LAYER_MODE_NORMAL);
    gimp_image_insert_layer(image, layer, NULL, 0);

    /* Use GeglBuffer for modern pixel access */
    GeglBuffer* buffer = gimp_drawable_get_buffer(GIMP_DRAWABLE(layer));
    gegl_buffer_set(buffer, GEGL_RECTANGLE(0, 0, width, height), 0, NULL, paa.mipMaps[0].data.data(), GEGL_AUTO_ROWSTRIDE);
    g_object_unref(buffer);
    
    g_free(path);

    /* Return the created image to GIMP */
    GimpValueArray* return_vals = gimp_procedure_new_return_values(procedure, GIMP_PDB_SUCCESS, NULL);
    GValue value = G_VALUE_INIT;
    g_value_init(&value, GIMP_TYPE_IMAGE);
    g_value_set_object(&value, image);
    gimp_value_array_append(return_vals, &value);
    return return_vals;
}

static GimpValueArray* paa_save(GimpProcedure* procedure, GimpRunMode run_mode, GimpImage* image, gint n_drawables, GimpDrawable** drawables, GFile* file, const GimpValueArray* args, gpointer data) {
    if (n_drawables < 1) {
        return gimp_procedure_new_return_values(procedure, GIMP_PDB_EXECUTION_ERROR, NULL);
    }
    
    GimpDrawable* drawable = drawables[0];
    int width = gimp_drawable_get_width(drawable);
    int height = gimp_drawable_get_height(drawable);

    /* PAA requirement: Dimensions must be power of two */
    if (!is_power_of_two(width) || !is_power_of_two(height)) {
        gimp_message("Error: Dimensions must be power of two (2^n) for PAA.");
        return gimp_procedure_new_return_values(procedure, GIMP_PDB_EXECUTION_ERROR, NULL);
    }

    /* Extract data using GEGL */
    int bpp = gimp_drawable_get_bpp(drawable);
    std::vector<uint8_t> buffer_data(width * height * bpp);
    GeglBuffer* buffer = gimp_drawable_get_buffer(drawable);
    gegl_buffer_get(buffer, GEGL_RECTANGLE(0, 0, width, height), 1.0, NULL, buffer_data.data(), GEGL_AUTO_ROWSTRIDE, GEGL_ABYSS_NONE);
    g_object_unref(buffer);

    /* PAA write logic with grad_aff */
    gchar* path = g_file_get_path(file);
    try {
        auto paa = grad_aff::Paa();
        // logic for population of paa mipmaps from buffer_data goes here
        paa.writePaa(path);
    } catch (const std::exception& e) {
        gimp_message(e.what());
        g_free(path);
        return gimp_procedure_new_return_values(procedure, GIMP_PDB_EXECUTION_ERROR, NULL);
    }

    g_free(path);
    return gimp_procedure_new_return_values(procedure, GIMP_PDB_SUCCESS, NULL);
}

static bool is_power_of_two(uint32_t x) {
    return (x != 0) && ((x & (x - 1)) == 0);
}

GIMP_MAIN (PAA_TYPE_PLUG_IN)