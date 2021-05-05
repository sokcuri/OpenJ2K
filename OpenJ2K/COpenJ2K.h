#pragma once
#include <windows.h>
#include "CJ2K_Engine.h"

template<typename T>
class Singleton {
public:
    static T& instance();

    Singleton(const Singleton&) = delete;
    Singleton& operator= (const Singleton) = delete;

protected:
    Singleton() {}
};

#include <memory>
template<typename T>
T& Singleton<T>::instance()
{
    static const std::unique_ptr<T> instance{ new T{} };
    return *instance;
}

class COpenJ2K final : public Singleton<COpenJ2K>
{
public:
    COpenJ2K();
    virtual ~COpenJ2K();

	bool J2K_InitializeEx(const char* a1, const char* a2);
    CStringA* GetJ2KMainDir(CStringA* a1);

public:
    CStringA datPath;
    HMODULE hJ2K;
    bool bInit;
    CJ2K_Engine* pCJ2K_Engine;
};
