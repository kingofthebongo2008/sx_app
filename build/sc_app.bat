@echo off

echo ^<?xml version="1.0" encoding="utf-8"?^> > sx_app_h.msbuild
echo ^<Project DefaultTargets="Build" ToolsVersion="15.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003"^> >> sx_app_h.msbuild

ucdev_build_file_generator_r.exe --input ..\src\ --mode h >> sx_app_h.msbuild
ucdev_build_file_generator_r.exe --input ..\3rdparty\dxsdk\include\ --mode h >> sx_app_h.msbuild

echo ^</Project^> >> sx_app_h.msbuild

echo ^<?xml version="1.0" encoding="utf-8"?^> > sx_app_cpp.msbuild
echo ^<Project DefaultTargets="Build" ToolsVersion="15.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003"^> >> sx_app_cpp.msbuild

ucdev_build_file_generator_r.exe --input ..\src\ --mode cpp >> sx_app_cpp.msbuild

echo ^</Project^> >> sx_app_cpp.msbuild


echo ^<?xml version="1.0" encoding="utf-8"?^> > sx_app.vcxproj.filters
echo ^<Project DefaultTargets="Build" ToolsVersion="15.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003"^> >> sx_app.vcxproj.filters

ucdev_build_file_generator_r.exe --type filters --input ..\src\ --mode h >> sx_app.vcxproj.filters
ucdev_build_file_generator_r.exe --type filters --input ..\3rdparty\dxsdk\include\ --mode h >> sx_app.vcxproj.filters
ucdev_build_file_generator_r.exe --type filters --input ..\src\ --mode cpp >> sx_app.vcxproj.filters

echo ^</Project^> >> sx_app.vcxproj.filters





