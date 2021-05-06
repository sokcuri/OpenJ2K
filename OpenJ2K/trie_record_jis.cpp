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
			FirstLoadDatabase();
		}  
	}
}

int trie_record_jis::GetGobiInfValue(char ch)
{
	int result;

	switch (ch)
	{
	case 48:
		result = 97;
		break;
	case 49:
		result = 96;
		break;
	case 50:
		result = 95;
		break;
	case 51:
		result = 94;
		break;
	case 52:
		result = 93;
		break;
	case 53:
		result = 92;
		break;
	case 54:
		result = 91;
		break;
	case 55:
		result = 88;
		break;
	case 56:
		result = 90;
		break;
	case 57:
	case 58:
		result = 99;
		break;
	case 59:
	case 61:
		result = 98;
		break;
	case 60:
		result = 89;
		break;
	default:
		result = 100;
		break;
	}
	return result;
}

void trie_record_jis::SaveMatchRec(int a2, CTrieParameters* a3)
{
}

bool trie_record_jis::FirstLoadDatabase()
{
	return false;
}

void* trie_record_jis::GetRecInfo(int a2, int a3)
{
	return nullptr;
}

int trie_record_jis::GetWordInfo(const char* SearchText, CRecData** RecData, int ExtraParam)
{
	return 0;
}

trie_record_jis::~trie_record_jis()
{
}
