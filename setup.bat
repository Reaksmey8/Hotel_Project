@echo off
title Hotel Management System - Setup
color 0A

echo.
echo  ========================================
echo    Hotel Management System - Setup
echo  ========================================
echo.

cd /d C:\Users\ASUS\Desktop\HotelProject

echo [1/4] Removing old build...
if exist build (
    attrib -r build\* /s /d
    rd /s /q build
)
mkdir build
cd build

echo [2/4] Configuring with CMake...
cmake .. -G "MinGW Makefiles" ^
    -DCMAKE_C_COMPILER="C:/mingw64/mingw64/bin/gcc.exe" ^
    -DCMAKE_CXX_COMPILER="C:/mingw64/mingw64/bin/g++.exe" ^
    -DCMAKE_MAKE_PROGRAM="C:/mingw64/mingw64/bin/mingw32-make.exe"

echo [3/4] Building...
cmake --build . 2>&1

echo [4/4] Copying DLL...
if exist _deps\xlnt-build\source\libxlnt.dll (
    copy /y "_deps\xlnt-build\source\libxlnt.dll" "." >nul
    echo DLL copied!
) else (
    echo DLL not found, searching...
    for /r . %%f in (libxlnt.dll) do copy /y "%%f" "." >nul
)

echo.
echo  ========================================
echo    Running Hotel Management System...
echo  ========================================
echo.
hotel.exe

pause