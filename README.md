# Crash Reporter

Example of how crash reporting is achieved for applications such as games where throwing and catching errors is not feasable. The code here is to demonstrate the technique and should not be used for comercial products.

## Build
To use the batch script make sure LLVM is installed in Windows, this is easily achived by using the Microsoft package manager winget. Once installed run `build.bat`

## Usage
run.exe binary.exe arg1 arg2 arg3

where `run.exe` is the crash reporter, `binary.exe` is the executable you are wrapping around and `arg1 arg2 arg3` are commandline arguments you want to forward to `binary.exe`