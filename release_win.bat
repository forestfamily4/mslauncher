mkdir build-release

cd  build-mslauncher*Release
copy mslauncher.exe ..\build-release
cd ..

cd msconnect/build-msconnect*Release
copy msconnect.exe ..\..\build-release
cd ..\..

cd build-release

windeployqt mslauncher.exe
windeployqt msconnect.exe

rd /s /q iconengines
rd /s /q imageformats
rd /s /q translations
del /q D3Dcompiler_47.dll
del /q opengl32sw.dll
del /q Qt6Svg.dll
@PAUSE