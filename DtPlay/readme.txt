To compile with Visual Studio 2015/2017/2019/2022:
- Download latest WinSDK installer: http://www.dektec.com/Products/SDK/WinSDK/Downloads/WinSDK.zip
- Run the WinSDK installer to install latest DTAPI libraries and drivers
- Open the project file and compile

To compile under linux:
- Copy DTAPI.h and DTAPI(64).o from the LinuxSDK to the IMPORT/DTAPI/ directory
- Run make, the executable will be generated in the current directory
