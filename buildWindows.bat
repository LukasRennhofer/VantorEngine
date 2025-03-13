@echo off
echo Cleaning old build...
cd build
rmdir /s /q windows
mkdir windows

cd windows

echo Running CMake...
cmake -G "Unix Makefiles" ../..

echo Building CHIFEngine...
make

echo Creating output directories...
mkdir \resources
mkdir \shaders
mkdir \libraries

echo Copying Assets and Shaders...
xcopy /e /i /h ..\..\src\resources resources
xcopy /e /i /h ..\..\src\shaders shaders
xcopy /e /i /h ..\..\src\libraries libraries

echo Build complete!
pause
