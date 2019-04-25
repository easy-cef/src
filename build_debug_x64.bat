:: george @ 2019/4/21

@echo off

:: build with Visual Studio
cd build
devenv cef.sln /build "Debug|x64"
cd ..

@echo on
