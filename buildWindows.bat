@echo off
echo Cleaning old build...#
mkdir build
cd build
rmdir /s /q windows
mkdir windows

cd windows

echo Running CMake...
cmake -G "Unix Makefiles" ../../src/

echo Building CHIFEngine...
make

echo Creating output directories...
mkdir \resources
mkdir \shaders
mkdir \lib

echo Copying Assets and Shaders...
xcopy /e /i /h ..\..\src\resources resources
xcopy /e /i /h ..\..\src\shaders shaders
xcopy /e /i /h ..\..\src\lib lib

echo Build complete!
start "" CHIFEngine.exe
pause
