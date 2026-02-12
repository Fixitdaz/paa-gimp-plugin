#pragma once

/*
 * author: fixitdaztv
 * date: 2026-02-12
 */

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>
#include <grad_aff/paa/paa.h>
#include <filesystem>
#include <vector>
#include <string>

#define PLUG_IN_BINARY "paa-gimp-plugin"
#define LOAD_PROC      "file-paa-load"
#define SAVE_PROC      "file-paa-save"

/* Define GObject type for GIMP 3.0 plugin architecture */
#define PAA_TYPE_PLUG_IN (paa_plug_in_get_type ())
G_DECLARE_FINAL_TYPE (PaaPlugIn, paa_plug_in, PAA, PLUG_IN, GimpPlugIn)

/* Logic prototypes */
static GimpValueArray* paa_load(GimpProcedure* procedure, GimpRunMode run_mode, GFile* file, const GimpValueArray* args, gpointer data);
static GimpValueArray* paa_save(GimpProcedure* procedure, GimpRunMode run_mode, GimpImage* image, gint n_drawables, GimpDrawable** drawables, GFile* file, const GimpValueArray* args, gpointer data);
static bool is_power_of_two(uint32_t x);