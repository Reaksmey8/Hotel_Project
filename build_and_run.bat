@echo off
echo ========================================
echo    Hotel Management System - Build
echo ========================================

cd /d C:\Users\ASUS\Desktop\HotelProject

echo [1/5] Cleaning build folder...
if exist build rmdir /s /q build
mkdir build
cd build

echo [2/5] Running CMake...
cmake .. -G "MinGW Makefiles" -DCMAKE_C_COMPILER="C:/mingw64/mingw64/bin/gcc.exe" -DCMAKE_CXX_COMPILER="C:/mingw64/mingw64/bin/g++.exe"

echo [3/5] Patching tabulate locale fix...
set FILE=C:\Users\ASUS\Desktop\HotelProject\build\_deps\tabulate-src\include\tabulate\termcolor.hpp
powershell -Command "(Get-Content '%FILE%') -replace 'std::locale::global\(std::locale\(""""\)\);', 'std::locale::global(std::locale(\"C\"));' | Set-Content '%FILE%'"
echo Patched!

echo [4/5] Building...
cmake --build .

echo [5/5] Running hotel.exe...
echo ========================================
hotel.exe

pause