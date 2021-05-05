#include <stdio.h>
#include <windows.h>
#include <shobjidl.h>
#include <atlbase.h>

#include <fstream>
#include <string>

#include <brotli/encode.h>
#include <brotli/decode.h>

#include <rapidcsv.h>

bool BrowseFile(TCHAR *Path)
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    TCHAR WorkingDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, WorkingDir);

    CComPtr<IShellItem> location;
    SHCreateItemFromParsingName(WorkingDir, nullptr, IID_PPV_ARGS(&location));

    BOOL Result = false;

    if (SUCCEEDED(hr))
    {
        CComPtr<IFileOpenDialog> pFileOpen = nullptr;
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
            IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            COMDLG_FILTERSPEC rgSpec[] =
            {
                {L"CSV Files (*.csv)", L"*.csv"},
                {L"All Files (*.*)", L"*.*"}
            };

            pFileOpen->SetFileTypes(ARRAYSIZE(rgSpec), rgSpec);
            pFileOpen->SetFolder(location);

            hr = pFileOpen->Show(NULL);

            if (SUCCEEDED(hr))
            {
                CComPtr<IShellItem> pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    if (SUCCEEDED(hr))
                    {
                        Result = true;
                        wcscpy_s(Path, MAX_PATH, pszFilePath);
                        CoTaskMemFree(pszFilePath);
                    }
                }
            }
        }
        CoUninitialize();
    }
    return Result;
}

int main()
{
    TCHAR Path[MAX_PATH];

    if (BrowseFile(reinterpret_cast<TCHAR*>(Path)))
    {
        std::stringstream buffer;
        std::streampos fsize = 0;
        auto file = std::ifstream(Path, std::ios::binary);

        fsize = file.tellg();
        file.seekg(0, std::ios::end);
        fsize = file.tellg() - fsize;
        file.seekg(0, std::ios::beg);

        char* buf = new char[fsize];
        file.rdbuf()->pubsetbuf(buf, fsize);

        char* buf2 = new char[fsize];
        buffer.rdbuf()->pubsetbuf(buf2, fsize);

        buffer << file.rdbuf();

        rapidcsv::Document doc(buffer);
        SetConsoleOutputCP(65001);

        for (size_t i = 0; i < doc.GetRowCount(); i++)
        {
            const auto row = doc.GetRow<std::string>(i);
            printf("- %d: %s %s %s %s %s %s\n",
                i, row[0].c_str(), row[1].c_str(), row[2].c_str(), row[3].c_str(), row[4].c_str(), row[5].c_str());
        }

        printf("end");
    }
}