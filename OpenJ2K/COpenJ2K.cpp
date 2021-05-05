#include "COpenJ2K.h"

COpenJ2K::COpenJ2K()
{
    hJ2K = LoadLibrary(L"J2KEngine.dlx");
}

COpenJ2K::~COpenJ2K()
{
    FreeLibrary(hJ2K);
}

bool COpenJ2K::J2K_InitializeEx(const char* a1, const char* a2)
{
    if (!bInit)
    {
        pCJ2K_Engine = new CJ2K_Engine();
        if (pCJ2K_Engine)
        {
            pCJ2K_Engine->J2K_InitializeEx(a1, a2);
            bInit = true;

            return true;
        }
        else
        {
            bInit = false;

            return false;
        }
    }
    return false;
}

CStringA* COpenJ2K::GetJ2KMainDir(CStringA* a1)
{
    char Buffer[MAX_PATH];
    if (datPath)
    {
        *a1 = datPath;
    }
    else
    {
        GetSystemDirectoryA(Buffer, 0x104u);
        CString temp(Buffer);
        *a1 = temp + "\\DAT\\";
    }
    return a1;
}
