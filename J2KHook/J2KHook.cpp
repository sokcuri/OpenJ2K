#include <stdio.h>
#include <windows.h>
#include "subhook.h"

using cstring_t = void*;
subhook_t foo_hook;

HMODULE module_mfc;

LPVOID __0CString__QAE_XZ				= (LPVOID)0x101F2A4A; // CString::CString(void *this)
LPVOID __0CString__QAE_PBD_Z			= (LPVOID)0x101F2A68; // CString::CString(void *this, const char *)
LPVOID __1CString__QAE_XZ				= (LPVOID)0x101F2A50; // CString::~CString(void *this)
LPVOID _Left_CString__QBE_AV1_H_Z		= (LPVOID)0x101F2B04; // CString::Left(void *this, void **, int)
LPVOID __H_YG_AVCString__ABV0_PBD_Z		= (LPVOID)0x101F2B5E; // operator+(CString const &,char const *)

cstring_t t = nullptr;

bool foo(int x) {
	return true;
}

int CString_Constructor(void* _this) {
	int result = 0;
	__asm {
		mov ecx, _this
		call __0CString__QAE_XZ
		mov result, eax
	}
	return result;
}


int CString_Constructor(void* _this, const char *_sz) {
	int result = 0;
	__asm {
		push _sz
		mov ecx, _this
		call __0CString__QAE_PBD_Z
		mov result, eax
	}
	return result;
}

int CString_Destructor(void* _this) {
	int result = 0;
	__asm {
		mov ecx, _this
		call __1CString__QAE_XZ
		mov result, eax
	}
	return result;
}

int CString_Left(void* _this, void* _new, int _count) {
	int result = 0;

	__asm {
		push _count
		push _new
		mov ecx, _this
		call _Left_CString__QBE_AV1_H_Z
		mov result, eax
	}
	return result;
}

int CString_GetLength(void* _this) {
	return *(int*)((char*)_this - 8);
}

int Operator_CStringConstRef_CharConstPtr(void* _new, void* _left, const char *_right) {
	int result = 0;

	__asm {
		push _right
		push _left
		push _new
		call __H_YG_AVCString__ABV0_PBD_Z

		mov result, eax
	}
	return result;
}

int fc2(void* _this, void *a2) {

	void* v17 = nullptr;
	void* v171 = nullptr;
	void* v18 = nullptr;
	void* lpFileName = nullptr;

	CString_Constructor((LPVOID*)((DWORD *)_this + 8));
	CString_Constructor((LPVOID*)((DWORD *)_this + 12));
	CString_Constructor((LPVOID*)((DWORD *)_this + 60));

	CString_Constructor(&v17);
	CString_Destructor(&v17);
	CString_Constructor(&v17, "testasdf");
	Operator_CStringConstRef_CharConstPtr(&v171, &v17, "asdf.inf");
	CString_Destructor(&v17);

	CString_Left(&a2, &v18, CString_GetLength(a2) - 3);
	Operator_CStringConstRef_CharConstPtr(&lpFileName, &v18, ".inf");
	CString_Destructor(&v18);

	HANDLE hFile = CreateFileA((char*)lpFileName, 0x80000000, 1u, 0, 3u, 0x80u, 0);
	*(DWORD*)((DWORD *)_this + 76) = (DWORD)hFile;

	if (hFile) {
		HANDLE hMap = CreateFileMappingA(hFile, 0, 2u, 0, 0, 0);
		*(DWORD*)((DWORD *)_this + 80) = (DWORD)hMap;
		if (hMap) {
			LPVOID lpMapView = MapViewOfFile(hMap, 4u, 0, 0, 0);
			*(DWORD*)((DWORD*)_this + 84) = (DWORD)lpMapView;
		}

	}
	
	CString_Destructor((LPVOID*)((DWORD*)_this + 8));
	CString_Destructor((LPVOID*)((DWORD*)_this + 12));
	CString_Destructor((LPVOID*)((DWORD*)_this + 60));
	return 0;
}

__declspec(naked) int __fastcall fc() {

	__asm {
		push ecx

		push [esp + 0x04 + 0x04]
		push ecx
		call fc2
		add esp, 0x08

		mov ecx, [esp]
		push [esp + 0x04 + 0x04]
		mov eax, 0x101bba70
		call eax

		pop eax
		retn 4
	}
}

void my_foo(int x) {
	/* Remove the hook so that you can call the original function. */
	subhook_remove(foo_hook);

	printf("foo(%d) called\n", x);
	foo(x);

	/* Install the hook back to intercept further calls. */
	subhook_install(foo_hook);
}

int main() {
	const auto &module_j2k = LoadLibrary(L"J2KEngine.dlx");
	if (module_j2k == 0) return 1;

	const auto& trj_ctor = GetProcAddress(module_j2k, "??0trie_record_jis@@QAE@VCString@@@Z");
	if (trj_ctor == 0) return 2;

	auto trj_ctor_hook = subhook_new((void*)trj_ctor, (void*)fc, (subhook_flags_t)0);
	subhook_install(trj_ctor_hook);

	const auto& fnInit = (bool(__stdcall*)(const char *, const char *))GetProcAddress(module_j2k, "J2K_InitializeEx");
	if (fnInit == 0) return 3;

	if (!fnInit("CSUSER123455", "Dat\\")) return 4;

	const auto& fnTrans = (const char*(__stdcall*)(int, const char*))GetProcAddress(module_j2k, "J2K_TranslateMMNT");
	printf("%s\n", fnTrans(0, "?맯궢궫귞긚깋귽?궬궯궫뙊"));


	/* Create a hook that will redirect all foo() calls to to my_foo(). */
	foo_hook = subhook_new((void*)foo, (void*)my_foo, (subhook_flags_t)0);

	/* Install it. */
		subhook_install(foo_hook);

	foo(123);

	/* Remove the hook and free memory when you're done. */
	subhook_remove(foo_hook);
	subhook_free(foo_hook);

	return 0;
}