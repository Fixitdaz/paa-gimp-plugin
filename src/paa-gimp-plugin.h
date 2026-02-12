/*
 * author: fixitdaztv
 * date: 2026-02-12
 */

#pragma once

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>
#include <grad_aff/paa/paa.h>
#include <vector>
#include <string>
#include <cstdint>

#define PAA_TYPE_PLUG_IN (paa_plug_in_get_type())
G_DECLARE_FINAL_TYPE (PaaPlugIn, paa_plug_in, PAA, PLUG_IN, GimpPlugIn)

/* Logic Prototypes */
static GimpValueArray* paa_load(GimpProcedure* procedure, GimpRunMode run_mode, GFile* file, GimpMetadata* metadata, GimpMetadataLoadFlags* flags, GimpProcedureConfig* config, gpointer run_data);
static GimpValueArray* paa_save(GimpProcedure* procedure, GimpRunMode run_mode, GimpImage* image, GimpDrawable** drawables, GimpProcedureConfig* config, gpointer run_data);
static bool is_power_of_two(uint32_t x);