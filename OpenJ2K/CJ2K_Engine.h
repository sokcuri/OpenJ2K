#pragma once
#include <atlstr.h>
#include "trie_record_jis.h"

#pragma pack(4)
class CJ2K_Engine
{
	int a;
	int b;
	int c;

public:
	bool J2K_Initialize(const char* a1);
	bool J2K_InitializeEx(const char* a1, const char* a2);

private:
	/* 0x0044 */ bool csuser_validate_pass;
	/* 0x00FC */ trie_record_jis* p_trie_record_jis;
};
#pragma pack()
