# Gruppe Adler PAA GIMP Plugin (GIMP 3.0 Rewrite)

**DISCLAIMER: This is a work in progress. This software may still contain bugs, errors, or unfinished features. Use at your own risk.**

paa-gimp-plugin is a GIMP File Format Plugin for opening and saving Arma PAA images in GIMP. For more information on the Arma PAA file format, check the [public PAA file format specification](https://community.bistudio.com/wiki/PAA_File_Format).

## GIMP 3.0 Compatibility Update
* **Author:** fixitdaztv
* **Date:** 2026-02-12

This version supports **GIMP 3.0.0 through 3.0.8+**. It migrates the codebase to the modern GObject-based API and utilizes GEGL buffers for pixel data handling. Legacy versions (GIMP 2.10 and older) are no longer supported.

## Installation

**Requires GIMP Version 3.0.0 or higher**

### Windows
1. Navigate to: `C:\Users\<username>\AppData\Roaming\GIMP\3.0\plug-ins\`.
2. Create a folder named exactly `paa-gimp-plugin`.
3. Move `paa-gimp-plugin.exe` into that folder.
4. **Note:** GIMP 3.0 requires the executable to reside in a folder of the same name to load correctly.

## Features
* Uses `Open` and `Open As` menu commands to read `*.paa` files.
* Uses `Export` and `Export As` menu commands to write `*.paa` files.
* Employs modern GEGL buffers for high-fidelity pixel data handling.

## Limitations
* Image dimensions must be a power of 2 (`2^n`).
* Currently supports DXT1 and DXT5 PAA formats.

## Dependencies
* libgimp-3.0
* gegl-0.4
* [grad_aff](https://github.com/gruppe-adler/grad_aff/tree/dev)

## License
Distributed under the GNU General Public License v2.