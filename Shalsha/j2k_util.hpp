#pragma once

#include <Windows.h>
#include <string_view>

static bool endsWith(std::string_view str, std::string_view suffix)
{
	return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}

static bool startsWith(std::string_view str, std::string_view prefix)
{
	return str.size() >= prefix.size() && 0 == str.compare(0, prefix.size(), prefix);
}

static std::wstring ToWideString(int charset, const char* str)
{
	int len = MultiByteToWideChar(charset, 0, (char*)str, -1, NULL, NULL);
	wchar_t* buff = new wchar_t[len + 1];
	MultiByteToWideChar(charset, 0, (char*)str, -1, (wchar_t*)buff, len);
	std::wstring result = buff;

	delete[] buff;
	return result;
}

static std::string ToMultiByteString(int charset, const wchar_t* str)
{
	int len = WideCharToMultiByte(charset, 0, (wchar_t*)str, -1, NULL, NULL, NULL, NULL);
	char* buff = new char[len + 1];
	WideCharToMultiByte(charset, 0, (wchar_t*)str, -1, (char*)buff, len, nullptr, nullptr);
	std::string result = buff;

	delete[] buff;
	return result;
}
