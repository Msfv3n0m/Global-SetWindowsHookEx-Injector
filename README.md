# Global-SetWindowsHookEx-Injector
Injects as many processes as possible with d3d11 hook example
## Usage
- hookloader.exe = injector
- test.dll = dll


1. Just add this to your dll:
extern "C" __declspec(dllexport) int NextHook(int code, WPARAM wParam, LPARAM lParam) {
	return CallNextHookEx(NULL, code, wParam, lParam);
}

2. Edit your dll name in hookloader/main.cpp:
HMODULE dll = LoadLibraryEx(L"test.dll", NULL, DONT_RESOLVE_DLL_REFERENCES); //<-- DLL name

3. Compile & done

![](https://github.com/Msfv3n0m/Global-SetWindowsHookEx-Injector/blob/master/loaded_dll.PNG)
