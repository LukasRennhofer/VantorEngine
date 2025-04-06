@echo off
mkdir build
cd build
rmdir /s /q windows
mkdir windows

echo Running CMake...
cmake -G "Unix Makefiles" ../../

echo Building TestFramework...
make

echo Creating output directories...
mkdir \resources
mkdir \shaders
mkdir \lib

echo Copying Assets and Shaders...
xcopy /e /i /h ..\..\..\..\src\resources resources
xcopy /e /i /h ..\..\..\..\src\shaders shaders
xcopy /e /i /h ..\..\..\..\src\lib lib

echo Build complete!
start "" TestFramework.exe
pause