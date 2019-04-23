:: george @ 2019/4/21

@echo off

:: prepare build path
if not exist "build" mkdir build
cd build

:: config project with VS2017
cmake -G "Visual Studio 15 Win64" ..

@echo on
