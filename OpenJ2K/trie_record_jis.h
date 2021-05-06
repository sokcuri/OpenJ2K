#pragma once
#include <atlstr.h>

class trie_record_jis
{
public:
	trie_record_jis(const char *daPath);
	int GetGobiInfValue(char ch);
	void SaveMatchRec(int a2, struct CTrieParameters* a3);
	bool FirstLoadDatabase();
	void* GetRecInfo(int a2, int a3);
	int GetWordInfo(const char* SearchText, struct CRecData** RecData, int ExtraParam);
	virtual ~trie_record_jis();

private:
	/* 0x000C */ CStringA* cstring_da_file;
	/* 0x003C */ CStringA* cstring_inf_file;
	/* 0x004C */ HANDLE hFile;
	/* 0x0050 */ HANDLE hMap;
	/* 0x0054 */ LPVOID lpMapView;
	/* 0x0094 */ void* p_sync_object;
	/* 0x009C */ CRITICAL_SECTION p_critical_section;

};

