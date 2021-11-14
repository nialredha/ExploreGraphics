@echo off

mkdir ..\build
pushd ..\build
cl -FC -Zi ..\code\win32_graphics.cpp ..\code\sd_sphere.cpp user32.lib gdi32.lib
popd
