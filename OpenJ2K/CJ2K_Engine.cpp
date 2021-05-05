#include "CJ2K_Engine.h"
#include "COpenJ2K.h"

bool CJ2K_Engine::J2K_Initialize(const char* a1)
{
    CStringA csUserTemp(a1);

    if (csUserTemp.GetLength() == 12)
    {
        if (csUserTemp.Left(6).Compare("CSUSER") == 0 ||
            csUserTemp.Left(6).Compare("OHUSER") == 0)
        {
            int n = 0;
            for (int i = 6; i < 11; ++i)
            {
                n = n + csUserTemp[i] - 48;
            }

            if (n % 10 == csUserTemp[11] - 48)
            {
                CStringA path;
                COpenJ2K::instance().GetJ2KMainDir(&path);

                auto daPath = path + "JisJK.da";
                p_trie_record_jis = new trie_record_jis(daPath);

                if (!p_trie_record_jis)
                {
                    this->csuser_validate_pass = false;
                    return false;
                }

                return this->csuser_validate_pass;
            }
        }
        return true;
    }

    return false;
}

bool CJ2K_Engine::J2K_InitializeEx(const char* a1, const char* a2)
{
    if (a2)
    {
        CStringA datTemp(a2);

        if (datTemp.Right(1).Compare("\\") != 0)
        {
            datTemp += '\\';
        }

        COpenJ2K::instance().datPath = datTemp;
    }
    return J2K_Initialize(a1);
}
