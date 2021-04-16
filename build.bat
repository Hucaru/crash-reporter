@echo off
rm binary.exe
rm run.exe

@echo on
clang binary.c -lUser32 -o binary.exe
clang run.c -o run.exe -lUser32 -lShell32