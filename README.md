# SetWindowsHookEx-Injector
SetWindowsHookEx Injector with d3d11 hook example

- hookloader.exe = injector
- test.dll = dll


1. Just add this to your dll:
extern "C" __declspec(dllexport) int NextHook(int code, WPARAM wParam, LPARAM lParam) {
	return CallNextHookEx(NULL, code, wParam, lParam);
}

2. Edit your dll name in hookloader/main.cpp:
HMODULE dll = LoadLibraryEx(L"test.dll", NULL, DONT_RESOLVE_DLL_REFERENCES); //<-- DLL name

3. Compile & done

