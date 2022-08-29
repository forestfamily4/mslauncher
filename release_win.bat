windeployqt mslauncher.exe
windeployqt msconnect.exe

rd /s /q iconengines
rd /s /q imageformats
rd /s /q translations
del /q D3Dcompiler_47.dll
del /q opengl32sw.dll

@PAUSE