﻿#include <stdio.h>
#include <windows.h>
#include "subhook.h"

LPVOID __0CString__QAE_XZ					= (LPVOID)0x101F2A4A; // CString::CString(void *this)
LPVOID __0CString__QAE_PBD_Z				= (LPVOID)0x101F2A68; // CString::CString(void *this, const char *)
LPVOID __1CString__QAE_XZ					= (LPVOID)0x101F2A50; // CString::~CString(void *this)
LPVOID _Left_CString__QBE_AV1_H_Z			= (LPVOID)0x101F2B04; // CString::Left(void *this, void **, int)
LPVOID __H_YG_AVCString__ABV0_PBD_Z			= (LPVOID)0x101F2B5E; // operator+(CString const &,char const *)
LPVOID __2_YAPAXI_Z							= (LPVOID)0x101F2A74; // void *__cdecl operator new(unsigned int a1)
LPVOID _GetJ2KMainDir__YA_AVCString__XZ_0	= (LPVOID)0x101C5EA0; // CString *__cdecl GetJ2KMainDir(CString *a1)

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

void* OperatorNew(unsigned int _len) {
	void *addr = 0;
	
	__asm {
		push _len
		call __2_YAPAXI_Z
		add esp, 0x04
		mov addr, eax
	}

	return addr;
}

void* __cdecl GetJ2KMainDir(void* a1) {
	void* result = nullptr;

	__asm {
		push a1
		call _GetJ2KMainDir__YA_AVCString__XZ_0
		add esp, 0x04
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

// _this = trie_record_jis
// userdict_path = ***/UserDict.jk
int sub_10002FFE(void* _this, void* userdict_path) {
	int result = 0;
	
	__asm {
		mov ecx, _this
		push userdict_path
		mov eax, 0x10002FFE
		call eax
		mov result, eax
	}

	return result;
}

int trie_record_jis_Constructor(void* _this, void *a2) {

	void* v15 = nullptr;
	void* v17 = nullptr;
	void* v171 = nullptr;
	void* v18 = nullptr;
	void* v19 = nullptr;
	void* lpFileName = nullptr;

	CString_Constructor((LPVOID*)((DWORD *)&_this + 8));
	CString_Constructor((LPVOID*)((DWORD *)&_this + 12));
	CString_Constructor((LPVOID*)((DWORD *)&_this + 60));

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
			void* test = OperatorNew(0x18);

			if (test) {

				GetJ2KMainDir(&v19);
				Operator_CStringConstRef_CharConstPtr(&v15, &v19, "UserDict.jk");
				
				int ret = sub_10002FFE(_this, v15);
			}
		}

	}

	CString_Destructor((LPVOID*)((DWORD*)&_this + 8));
	CString_Destructor((LPVOID*)((DWORD*)&_this + 12));
	CString_Destructor((LPVOID*)((DWORD*)&_this + 60));

	return 0;
}

int main() {
	const auto &module_j2k = LoadLibrary(L"J2KEngine.dlx");
	if (module_j2k == 0) return 1;

	const auto& trj_ctor = GetProcAddress(module_j2k, "??0trie_record_jis@@QAE@VCString@@@Z");
	if (trj_ctor == 0) return 2;

	auto trj_ctor_hook = subhook_new((void*)trj_ctor, (void*)trie_record_jis_Constructor, (subhook_flags_t)0);
	subhook_install(trj_ctor_hook);

	const auto& fnInit = (bool(__stdcall*)(const char *, const char *))GetProcAddress(module_j2k, "J2K_InitializeEx");
	if (fnInit == 0) return 3;

	if (!fnInit("CSUSER123455", "Dat\\")) return 4;

	const auto& fnTrans = (const char*(__stdcall*)(int, const char*))GetProcAddress(module_j2k, "J2K_TranslateMMNT");
	printf("%s\n", fnTrans(0, "?맯궢궫귞긚깋귽?궬궯궫뙊"));

	subhook_remove(trj_ctor_hook);
	subhook_free(trj_ctor_hook);
	return 0;
}