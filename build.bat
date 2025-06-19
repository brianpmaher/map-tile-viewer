@echo off

set GAME_NAME=game.exe
set MAIN_SOURCES=src\main.c

set RAYLIB_SRC=deps\raylib\src
set TARGET_PLATFORM=x64

where cl >nul 2>nul
if %ERRORLEVEL% == 0 goto READ_ARGS
echo Setting up the msvc build environment, this could take some time but the next builds should be faster
call vcvarsall.bat %TARGET_PLATFORM% > nul 2>&1

:READ_ARGS
setlocal EnableDelayedExpansion
setlocal EnableExtensions

:ARG_LOOP
set ARG=%1
if "!ARG!" == "" ( goto BUILD )
if not "x!ARG!" == "x!ARG:h=!" (
  goto HELP
)
if not "x!ARG!" == "x!ARG:d=!" (
  set BUILD_DEBUG=1
)
if not "x!ARG!" == "x!ARG:u=!" (
  set UPX_IT=1
)
if not "x!ARG!" == "x!ARG:r=!" (
  set RUN_AFTER_BUILD=1
)
if not "x!ARG!" == "x!ARG:c=!" (
  set BUILD_ALL=1
)
if not "x!ARG!" == "x!ARG:qq=!" (
  set QUIET=1
  set REALLY_QUIET=1
) else if not "x!ARG!" == "x!ARG:q=!" (
  if defined QUIET (
    set REALLY_QUIET=1
  ) else (
    set QUIET=1
  )
)
if not "x!ARG!" == "x!ARG:v=!" (
  set VERBOSE=1
)
if not "x!ARG!" == "x!ARG:t=!" (
  set TIME_BUILD=1
)
if not "%1" == "" (
  shift /1
  goto ARG_LOOP
)


:HELP
echo Usage: build.bat [-hdurcqqv]
echo  -h  Show this information
echo  -d  Faster builds that have debug symbols, and enable warnings
echo  -u  Run upx* on the executable after compilation (before -r)
echo  -r  Run the executable after compilation
echo  -c  Remove the build\{debug,release} directory, ie. full recompile
echo  -q  Suppress this script's informational prints
echo  -qq Suppress all prints, complete silence
echo  -v  Print all cl.exe verbose information
echo  -t  Time the build and print timing information
exit /B


:BUILD

:: Build timing util if it hasn't been built
if not exist build\ctime.exe (
  echo COMPILE-INFO: Building ctime util.
  if defined REALLY_QUIET (
    cl /Fe: build\ctime.exe -O2 utils\ctime.c /link winmm.lib > NUL 2>&1
  ) else (
    cl /Fe: build\ctime.exe -O2 utils\ctime.c /link winmm.lib
  )
  del ctime.obj
)

:: Record the start time
if defined TIME_BUILD (
  set TIME_FILE=build_times_release.ctm
  if defined BUILD_DEBUG (
    set TIME_FILE=build_times_debug.ctm
  )
  build\ctime.exe -begin !TIME_FILE!
)

:: Directories
set "ROOT_DIR=%CD%"
set "MAIN_SOURCES=!ROOT_DIR!\!MAIN_SOURCES!"
set "RAYLIB_SRC=!ROOT_DIR!\!RAYLIB_SRC!"

:: Flags
set OUTPUT_FLAG=/Fe: "!GAME_NAME!"
set COMPILATION_FLAGS=/std:c11 /O2 /GL /favor:blend /utf-8 /validate-charset /EHsc
set WARNING_FLAGS=/W4 /sdl
set SUBSYSTEM_FLAGS=/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup
set LINK_FLAGS=/link /STACK:1024000000 /LTCG kernel32.lib user32.lib shell32.lib winmm.lib gdi32.lib opengl32.lib
set OUTPUT_DIR=build\release
:: Debug changes to flags
if defined BUILD_DEBUG (
  set OUTPUT_FLAG=/Fe: "!GAME_NAME!"
  set COMPILATION_FLAGS=/std:c11 /Od /Zi /utf-8 /validate-charset /EHsc /DDEBUG
  set WARNING_FLAGS=/W4 /WX /sdl
  set SUBSYSTEM_FLAGS=
  set LINK_FLAGS=/link /STACK:1024000000 /DEBUG:FULL kernel32.lib user32.lib shell32.lib winmm.lib gdi32.lib opengl32.lib
  set OUTPUT_DIR=build\debug
)
if not defined VERBOSE (
  set VERBOSITY_FLAG=/nologo
)

:: Display what we're doing
if defined BUILD_DEBUG (
  if not defined QUIET echo COMPILE-INFO: Compiling in debug mode, flags: !COMPILATION_FLAGS! !WARNING_FLAGS!
) ELSE (
  if not defined QUIET echo COMPILE-INFO: Compiling in release mode, flags: !COMPILATION_FLAGS! /link /LTCG
)

:: Create the cache directory for raylib
set "CACHE_DIR=build\release\cache"
if defined BUILD_DEBUG (
  set "CACHE_DIR=build\debug\cache"
)

if defined BUILD_ALL (
  if exist !CACHE_DIR!\ (
    if not defined QUIET echo COMPILE-INFO: Found cached raylib, rebuilding.
    del /Q !CACHE_DIR!
    rmdir !CACHE_DIR!
  )
)

:: Build raylib if it hasn't been cached in CACHE_DIR
if not exist !CACHE_DIR!\ (
  mkdir !CACHE_DIR!
  cd !CACHE_DIR!
  :: raylib source folder
  set "RAYLIB_DEFINES=/D_DEFAULT_SOURCE /DPLATFORM_DESKTOP /DGRAPHICS_API_OPENGL_33"
  set RAYLIB_C_FILES="!RAYLIB_SRC!\rcore.c" "!RAYLIB_SRC!\rshapes.c" "!RAYLIB_SRC!\rtextures.c" "!RAYLIB_SRC!\rtext.c" "!RAYLIB_SRC!\rmodels.c" "!RAYLIB_SRC!\utils.c" "!RAYLIB_SRC!\raudio.c" "!RAYLIB_SRC!\rglfw.c"
  set RAYLIB_INCLUDE_FLAGS=/I"!RAYLIB_SRC!" /I"!RAYLIB_SRC!\external\glfw\include"

  if defined REALLY_QUIET (
    cl.exe /w /c !RAYLIB_DEFINES! !RAYLIB_INCLUDE_FLAGS! !COMPILATION_FLAGS! !RAYLIB_C_FILES! > NUL 2>&1 || exit /B
  ) else (
    cl.exe /w /c !VERBOSITY_FLAG! !RAYLIB_DEFINES! !RAYLIB_INCLUDE_FLAGS! !COMPILATION_FLAGS! !RAYLIB_C_FILES! || exit /B
  )
  if not defined QUIET echo COMPILE-INFO: raylib compiled into object files in: !CACHE_DIR!\

  :: Out of the cache directory
  cd !ROOT_DIR!
)

:: Move to the build directory
if not exist !OUTPUT_DIR! mkdir !OUTPUT_DIR!
cd !OUTPUT_DIR!

:: Build the actual game
if not defined QUIET echo COMPILE-INFO: Compiling game code.
set COMPILE_COMMAND=cl.exe !VERBOSITY_FLAG! !COMPILATION_FLAGS! !WARNING_FLAGS! /c /I"!RAYLIB_SRC!" !MAIN_SOURCES!
set LINK_COMMAND=cl.exe !VERBOSITY_FLAG! !OUTPUT_FLAG! "!ROOT_DIR!\!CACHE_DIR!\*.obj" *.obj !LINK_FLAGS! !SUBSYSTEM_FLAGS!
if defined REALLY_QUIET (
  !COMPILE_COMMAND! > NUL 2>&1 || exit /B
  !LINK_COMMAND! > NUL 2>&1 || exit /B
) ELSE (
  !COMPILE_COMMAND! || exit /B
  !LINK_COMMAND! || exit /B
)
del *.obj
if not defined QUIET echo COMPILE-INFO: Game compiled into an executable in: !OUTPUT_DIR!

:: Run upx
if defined UPX_IT (
  if not defined QUIET echo COMPILE-INFO: Packing !GAME_NAME! with upx.
  upx !GAME_NAME! > NUL 2>&1
)

:: Back to development directory
cd !ROOT_DIR!

:: Record the end time
if defined TIME_BUILD (
  build\ctime.exe -end !TIME_FILE!
)

:: Finally, run the produced executable
if defined RUN_AFTER_BUILD (
  if not defined QUIET echo COMPILE-INFO: Running.
  if defined REALLY_QUIET (
    !OUTPUT_DIR!\!GAME_NAME! > NUL 2>&1
  ) else (
    !OUTPUT_DIR!\!GAME_NAME!
  )
)

if not defined QUIET echo COMPILE-INFO: All done.
