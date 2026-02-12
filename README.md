
# This is a work in progress and is not fully working yet

 

# Gruppe Adler PAA GIMP Plugin

paa-gimp-plugin is a GIMP File Format Plugin for opening and saving Arma PAA images in GIMP. For more information on the Arma PAA file format check out the [public PAA file format specification](https://community.bistudio.com/wiki/PAA_File_Format).

# GIMP 3.0 Compatibility Update
**Author:** fixitdaztv  
**Date:** 2026-02-12

This version of the plugin is updated to support **GIMP 3.0.0 through 3.0.8+**. It migrates the codebase to the modern GObject-based API and utilizes GEGL buffers for pixel data handling. Legacy versions of GIMP (2.10 and older) are no longer supported by this specific branch.

# Installation

**Requires GIMP Version 3.0.0 or higher**

## Windows
* Download the latest `paa-gimp-plugin.exe`.
* Navigate to your GIMP 3.0 plug-ins directory: `C:\Users\<username>\AppData\Roaming\GIMP\3.0\plug-ins\`.
* Create a new folder inside named exactly `paa-gimp-plugin`.
* Move `paa-gimp-plugin.exe` into that new folder.
* **Note:** GIMP 3.0 requires the executable to reside in a folder of the same name to load correctly.

## Linux
* Create a directory at `~/.config/GIMP/3.0/plug-ins/paa-gimp-plugin/`.
* Place the compiled `paa-gimp-plugin` binary inside that directory.
* Ensure the file has execution permissions (`chmod +x paa-gimp-plugin`).

# Features
* Uses `Open` and `Open As` menu commands to read `*.paa` files.
* Uses `Export` and `Export As` menu commands to write `*.paa` files.
* Employs modern GEGL buffers for high-fidelity pixel data handling.

# Limitations
* Image dimensions must be a power of 2 (`2^n`) as required by the PAA specification.
* Currently supports DXT1 and DXT5 PAA formats.

# Dependencies
* libgimp-3.0
* gegl-0.4
* [grad_aff](https://github.com/gruppe-adler/grad_aff/tree/dev)

# Building
For Windows, using msys2/mingw-w64 is recommended.
* Install libgimp-3.0 and [grad_aff](https://github.com/gruppe-adler/grad_aff/tree/dev).
* Clone this repository.
* Run the following commands in the project directory:

```bash
mkdir build
cd build
cmake ..
make
make install
