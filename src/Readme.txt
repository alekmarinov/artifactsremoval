Image artifacts removal system using Morphologicaly-Geometric approach

-- Quick Instructions --

Command Line:
GeomMorphApproach.exe <InputDirectory> <OutputDirectory> [IsProduceEvaluationImage]

Examples:

GeomMorphApproach.exe data .		  -- process data/*.jpg -> ./*.jpg
GeomMorphApproach.exe c:/input c:/output  -- process c:/input/*.jpg -> c:/output/*.jpg
GeomMorphApproach.exe data \temp          -- process data/*.jpg -> /temp/*.jpg
GeomMorphApproach.exe data \temp 1        -- process data/*.jpg -> /temp/*.jpg, /temp/*_eval.jpg 

-- How it works --

The program GeomMorphApproach.exe looks for jpeg/jpg images from InputDirectory, 
process them with Morphologicaly-Geometric algorithm and save the result in OutputDirectory.
The last optional parameter IsProduceEvaluationImage tells to program to save the 
evaluation image as result of the algorithm used for suppressing the input image

-- Libraries
ia (Image Analyser) is ANSI C system independent library containing instruments for operating on images.
luaia is an ia to Lua binding library which registers the exported functions in the Lua context.

-- Build instructions

Linux users can replace "dll" with "so" and remove ".exe"
1. Build ia.dll,ia.lib with C compiler using preprocessing directive HAVE_JPEGLIB and link to jpeg.lib
2. Build luaia.dll,luaia.lib with C compiler linked to ia.lib. Make sure that the luaia.dll exports 
the function luaopen_luaia
3. Compile lua.exe (version 5.1 or above) with C compiler or take precompiled binary from www.lua.org

C compiler above refers any known ANSI C compiler like: bcc, cl, gcc, lcc etc.

After these steps use the following syntax:
	lua.exe GeomMorphApproach.lua <InputDirectory> <OutputDirectory> [IsProduceEvaluationImage]

Building GeomMorphApproach.exe will need the LRun system which can be asked from alek@crazyland.com

-- Manifest --
data/                      directory with sample images
ia/                        source of image analyzer library
luaia/                     source of ia to lua binding library
GeomMorphApproach.exe      Program executable
GeomMorphApproach.lua      Program source
Readme.txt                 this file

-- References

Lua http://www.lua.org
Independent JPEG Group http://www.ijg.org/
LRun mailto:alek@crazyland.com
