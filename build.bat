@echo off
cl dsmath.cpp -EHsc -nologo -O2 User32.lib
del *.obj >NUL 2>NUL
