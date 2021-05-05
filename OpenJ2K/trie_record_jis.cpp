#include "trie_record_jis.h"
#include <brotli/decode.h>

trie_record_jis::trie_record_jis(const char* daPath)
{
	this->hFile = nullptr;
	this->hMap = nullptr;
	this->lpMapView = nullptr;
	this->p_sync_object = nullptr;

	this->cstring_da_file = new CStringA();
	this->cstring_inf_file = new CStringA();

	InitializeCriticalSection(&this->p_critical_section);

	CStringA da_path(daPath);
	auto path = da_path.Left(da_path.GetLength() - 3);
	
	CStringA inf_path(path + ".inf");

	HANDLE hFile = CreateFileA(inf_path, 0x80000000, 1u, 0, 3u, 0x80u, 0);
	this->hFile = hFile;

	if (hFile) {
		HANDLE hMap = CreateFileMappingA(hFile, 0, 2u, 0, 0, 0);
		this->hMap = hMap;
		if (hMap) {
			LPVOID lpMapView = MapViewOfFile(hMap, 4u, 0, 0, 0);
			this->lpMapView = lpMapView;

			*this->cstring_da_file = daPath;
			printf("test ok\n");
		}
	}
}

trie_record_jis::~trie_record_jis()
{
}
