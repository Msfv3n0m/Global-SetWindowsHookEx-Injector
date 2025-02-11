﻿#include <Windows.h>
#include <string>
#include <imagehlp.h>
#pragma comment(lib, "Dbghelp.lib")
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include "MinHook/include/MinHook.h" //detour

typedef void(__stdcall *D3D11DRAWINDEXED) (ID3D11DeviceContext *pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
D3D11DRAWINDEXED orig_D3D11DrawIndexed = NULL;

extern "C" {
	void *mProcs[51] = { 0 };

	void CreateDirect3D11DeviceFromDXGIDevice_wrapper();
	void CreateDirect3D11SurfaceFromDXGISurface_wrapper();
	void D3D11CoreCreateDevice_wrapper();
	void D3D11CoreCreateLayeredDevice_wrapper();
	void D3D11CoreGetLayeredDeviceSize_wrapper();
	void D3D11CoreRegisterLayers_wrapper();
	void D3D11CreateDevice_wrapper();
	void D3D11CreateDeviceAndSwapChain_wrapper();
	void D3D11CreateDeviceForD3D12_wrapper();
	void D3D11On12CreateDevice_wrapper();
	void D3DKMTCloseAdapter_wrapper();
	void D3DKMTCreateAllocation_wrapper();
	void D3DKMTCreateContext_wrapper();
	void D3DKMTCreateDevice_wrapper();
	void D3DKMTCreateSynchronizationObject_wrapper();
	void D3DKMTDestroyAllocation_wrapper();
	void D3DKMTDestroyContext_wrapper();
	void D3DKMTDestroyDevice_wrapper();
	void D3DKMTDestroySynchronizationObject_wrapper();
	void D3DKMTEscape_wrapper();
	void D3DKMTGetContextSchedulingPriority_wrapper();
	void D3DKMTGetDeviceState_wrapper();
	void D3DKMTGetDisplayModeList_wrapper();
	void D3DKMTGetMultisampleMethodList_wrapper();
	void D3DKMTGetRuntimeData_wrapper();
	void D3DKMTGetSharedPrimaryHandle_wrapper();
	void D3DKMTLock_wrapper();
	void D3DKMTOpenAdapterFromHdc_wrapper();
	void D3DKMTOpenResource_wrapper();
	void D3DKMTPresent_wrapper();
	void D3DKMTQueryAdapterInfo_wrapper();
	void D3DKMTQueryAllocationResidency_wrapper();
	void D3DKMTQueryResourceInfo_wrapper();
	void D3DKMTRender_wrapper();
	void D3DKMTSetAllocationPriority_wrapper();
	void D3DKMTSetContextSchedulingPriority_wrapper();
	void D3DKMTSetDisplayMode_wrapper();
	void D3DKMTSetDisplayPrivateDriverFormat_wrapper();
	void D3DKMTSetGammaRamp_wrapper();
	void D3DKMTSetVidPnSourceOwner_wrapper();
	void D3DKMTSignalSynchronizationObject_wrapper();
	void D3DKMTUnlock_wrapper();
	void D3DKMTWaitForSynchronizationObject_wrapper();
	void D3DKMTWaitForVerticalBlankEvent_wrapper();
	void D3DPerformance_BeginEvent_wrapper();
	void D3DPerformance_EndEvent_wrapper();
	void D3DPerformance_GetStatus_wrapper();
	void D3DPerformance_SetMarker_wrapper();
	void EnableFeatureLevelUpgrade_wrapper();
	void OpenAdapter10_wrapper();
	void OpenAdapter10_2_wrapper();
}

HHOOK hKeyHook = 0;
void **vtable = NULL;
BOOL flag = FALSE;

//get dir
using namespace std;
#include <fstream>
char dlldir[320];
char *GetDirectoryFile(char *filename)
{
	static char path[320];
	strcpy_s(path, dlldir);
	strcat_s(path, filename);
	return path;
}

//log
void Log(const char *fmt, ...)
{
	if (!fmt)	return;

	char		text[4096];
	va_list		ap;
	va_start(ap, fmt);
	vsprintf_s(text, fmt, ap);
	va_end(ap);

	ofstream logfile(GetDirectoryFile((PCHAR)"log.txt"), ios::app);
	if (logfile.is_open() && text)	logfile << text << endl;
	logfile.close();
}

void __stdcall hook_D3D11DrawIndexed(ID3D11DeviceContext *pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
{
	if (GetAsyncKeyState(VK_F9) & 1)
	Log("hook_D3D11DrawIndexed");

	ID3D11Buffer *veBuffer;
	UINT veWidth;
	UINT Stride;
	UINT veBufferOffset;
	D3D11_BUFFER_DESC veDesc;

	ID3D11Buffer *inBuffer;
	UINT inWidth;
	DXGI_FORMAT inFormat;
	UINT inOffset;
	D3D11_BUFFER_DESC inDesc;


	pContext->IAGetVertexBuffers(0, 1, &veBuffer, &Stride, &veBufferOffset);
	if (veBuffer) {
		veBuffer->GetDesc(&veDesc);
		veWidth = veDesc.ByteWidth;
	}
	if (NULL != veBuffer) {
		veBuffer->Release();
		veBuffer = NULL;
	}

	pContext->IAGetIndexBuffer(&inBuffer, &inFormat, &inOffset);
	if (inBuffer) {
		inBuffer->GetDesc(&inDesc);
		inWidth = inDesc.ByteWidth;
	}
	if (NULL != inBuffer) {
		inBuffer->Release();
		inBuffer = NULL;
	}

	//quick test to make models disappear in ut4
	if (Stride == 28||Stride == 40 || Stride == 44)
		return;

	return orig_D3D11DrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
}

/*
void ThreadProc(void*)
//int ThreadProc()
{
	ID3D11Device *pDevice = NULL;
	ID3D11DeviceContext *pContext = NULL;


	if (FAILED(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &pDevice, NULL, &pContext))) {
		MessageBox(0, "D3D11CreateDevice failed", NULL, MB_OK);
	}

	vtable = *(void***)pContext;
	MH_STATUS st;
	st = MH_Initialize();
	if (MH_OK != st && MH_ERROR_ALREADY_INITIALIZED != st) {
		MessageBox(0, "MH_Initialize", NULL, MB_OK);
	}
	st = MH_CreateHook(vtable[12], hook_D3D11DrawIndexed, (void**)&orig_D3D11DrawIndexed);
	if (MH_OK != st && MH_ERROR_ALREADY_CREATED != st) {
		MessageBox(0, "MH_CreateHook", NULL, MB_OK);
	}
	st = MH_EnableHook(vtable[12]);
	if (MH_OK != st && MH_ERROR_ENABLED != st) {
		MessageBox(0, "MH_EnableHook", NULL, MB_OK);
	}

	pDevice->Release();
	pContext->Release();
}
*/

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	do
	{
		Sleep(100);
	} while (!GetModuleHandle(L"dxgi.dll"));
	Sleep(4000);

	/*
	IDXGIFactory1* pFactory;
	IDXGIAdapter1* pAdapter;
	if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(&pFactory)))) {
		return 1;
	}

	if (FAILED(pFactory->EnumAdapters1(0, &pAdapter))) {
		pFactory->Release();
		return 1;
	}
	*/

	ID3D11Device *pDevice = NULL;
	ID3D11DeviceContext *pContext = NULL;


	if (FAILED(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &pDevice, NULL, &pContext))) {
		MessageBox(0, L"D3D11CreateDevice failed", NULL, MB_OK);
		return 1;
	}

	vtable = *(void***)pContext;
	MH_STATUS st;
	st = MH_Initialize();
	if (MH_OK != st && MH_ERROR_ALREADY_INITIALIZED != st) {
		MessageBox(0, L"MH_Initialize", NULL, MB_OK);
		return 1;
	}
	st = MH_CreateHook(vtable[12], hook_D3D11DrawIndexed, (void**)&orig_D3D11DrawIndexed);
	if (MH_OK != st && MH_ERROR_ALREADY_CREATED != st) {
		MessageBox(0, L"MH_CreateHook", NULL, MB_OK);
		return 1;
	}
	st = MH_EnableHook(vtable[12]);
	if (MH_OK != st && MH_ERROR_ENABLED != st) {
		MessageBox(0, L"MH_EnableHook", NULL, MB_OK);
		return 1;
	}

	pDevice->Release();
	pContext->Release();

	return 0;
}

HMODULE Kernel32Dll;
HANDLE NtCreateThreadEx(HANDLE InProcess, void* InRemoteThreadStart, void* InRemoteCallback)
{
	typedef DWORD WINAPI NtCreateThreadEx_PROC(PHANDLE ThreadHandle,ACCESS_MASK DesiredAccess,LPVOID ObjectAttributes,HANDLE ProcessHandle,LPTHREAD_START_ROUTINE lpStartAddress,
		LPVOID lpParameter,BOOL CreateSuspended,DWORD dwStackSize,LPVOID Unknown1,LPVOID Unknown2,LPVOID Unknown3
	);

	//for vista and later...
	NtCreateThreadEx_PROC* VistaCreateThread = (NtCreateThreadEx_PROC*)
		GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtCreateThreadEx");

	if (VistaCreateThread == NULL)
		return NULL;

	HANDLE			hRemoteThread = NULL;
	HRESULT			hRes = 0;

	if (!SUCCEEDED(hRes = VistaCreateThread(&hRemoteThread,
		0x1FFFFF, // all access
		0,
		InProcess,
		(LPTHREAD_START_ROUTINE)InRemoteThreadStart,
		InRemoteCallback,
		FALSE,
		0,
		0,
		0,
		0
	)))
		return NULL;

	return hRemoteThread;
}

unsigned char Backup[5], *TerminateThreadAddress;        /** Backup**/
/** Dummy Function **/
BOOL WINAPI MyTerminateThread(
	HANDLE hThread,
	DWORD dwExitCode)
{
	return true;
}

HINSTANCE hinstDLL;
void DoThings() {
	/*
	// Showing messagebox with program name and PID as injection proof
	TCHAR szExeFileName[MAX_PATH];
	GetModuleFileName(NULL, szExeFileName, MAX_PATH);
	std::wstring exeName = szExeFileName;
	int pos = exeName.find_last_of(L"\\");
	exeName = exeName.substr(pos + 1, exeName.length());
	std::wstring message = L"Injected in " + exeName + L" (PID " + std::to_wstring(GetCurrentProcessId()) + L")";
	MessageBox(NULL, message.c_str(), L"Success", MB_OK);
	*/

	//bs
	FreeLibrary(Kernel32Dll);
	Kernel32Dll = LoadLibraryA("Kernel32.dll");
	TerminateThreadAddress = (unsigned char*)GetProcAddress(Kernel32Dll, "TerminateThread");
	HANDLE HandleProzess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, false, GetCurrentProcessId());
	NtCreateThreadEx(HandleProzess, (void*)ThreadProc, (void*)hinstDLL);
	CloseHandle(HandleProzess);

	//CreateThread(0, 0, ThreadProc, NULL, 0, 0);
}

// For standalone mode (EXE, cmd line)
int main() {
	DoThings();
	return EXIT_SUCCESS;
}

// For standalone mode (EXE, GUI)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	DoThings();
	return EXIT_SUCCESS;
}

// For DLL mode
extern "C" __declspec(dllexport)
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(DoThings), NULL, 0, 0);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

// Exporting function usable with SetWindowsHookEx
extern "C" __declspec(dllexport) int NextHook(int code, WPARAM wParam, LPARAM lParam) {
	return CallNextHookEx(NULL, code, wParam, lParam);
}
