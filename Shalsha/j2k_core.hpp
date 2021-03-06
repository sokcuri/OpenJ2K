#pragma once

#include "j2k_serializer.hpp"
#include "j2k_util.hpp"
#include "j2k_base_dict.hpp"
#include "j2k_data_struct.hpp"

#undef min
#undef max

#include <filesystem>

#include "../Shared/mio/mio.hpp"
#include "../Shared/tsl/htrie_map.h"
#include "../Shared/serializer.h"

#include <brotli/decode.h>

class j2k_core
{
public:
	j2k_core()
	{
		printf("J2KEngine initialized\n");
	}

	virtual ~j2k_core()
	{

	}

	bool Initialize(std::string signature, std::string data_folder)
	{
		if (!startsWith(data_folder, "\\"))
			data_folder += '\\';


		std::string htrie_map_name = data_folder + "htrie.data";
		std::string hdata_vec_name = data_folder + "hdata.data";
		std::string kuri_data_name = data_folder + "hdata.data";

		std::vector<std::unique_ptr<j2k_base_dict>> v;

		//std::vector<char> data;
		//zpp::serializer::memory_input_archive in(data);
		//std::basic_ifstream<char> fin(hdata_vec_name, std::ios::binary);

		//data = std::vector<char>((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
		//fin.close();

		//in(v);

		//vec = std::move(v);

    mio::shared_mmap_source shared_mmap1(kuri_data_name);
    std::error_code error;

    std::wstring temp_map_file = std::filesystem::temp_directory_path();
    temp_map_file += L"\\temp.dat";

    std::ofstream ofs(temp_map_file, std::ios::binary | std::ios::out);
    ofs.seekp((20 << 20) - 1);
    ofs.write("", 1);
    ofs.close();

    auto temp_mmap = std::make_unique<mio::basic_mmap_sink<char>>(temp_map_file);
    // auto mmap = mio::make_mmap_source("temp.dat", 0, (100 << 20) - 1, error);

    size_t data_length = (20 << 20) - 1;

    {
      if (BrotliDecoderDecompress(shared_mmap1.length(), (const uint8_t*)shared_mmap1.data(), &data_length, (uint8_t *)temp_mmap->data()) == 0)
      {
        wprintf(L"decode failed\n");
        return false;
      }
    }

    std::error_code error_code;
    temp_mmap->sync(error_code);
    data_mmap = std::move(temp_mmap);

    
		wprintf(L"hdata load complete\n");

		deserializer dserial(htrie_map_name);
		trie_map = tsl::htrie_map<char, std::int32_t>::deserialize(dserial);

		wprintf(L"Load Complete\n");

		return true;
	}

	std::vector<std::pair<int, int>> find_prefix(std::string u8str)
	{
		std::vector<std::pair<int, int>> prefix_vec;

		auto lookup_prefix = trie_map.lookup_prefix(u8str);
		for (auto& prefix : lookup_prefix)
		{
			auto lpfx = ToWideString(CP_UTF8, prefix.key().c_str());
			auto lppx = ToMultiByteString(932, lpfx.c_str());
			auto value = (int)*prefix;
			prefix_vec.push_back(std::make_pair(value, lppx.length()));
		}
		return prefix_vec;
	}

  std::uint32_t get_rec_size()
  {
    return *(int*)((char*)data_mmap->data() + 0x08);
  }

	std::unique_ptr<j2k_base_dict> get_rec(size_t id)
	{
    if (id > get_rec_size() || id < 0) return nullptr;

    j2k_data_struct *st = new j2k_data_struct();

    char* base = data_mmap->data();
    auto ptr = *(int *)(base + 0x08 + 0x04 * id);
    char* tbase = base + ptr;
    //auto ptr2 = data_mmap->data() + (int)ptr;

    /*auto z = data_mmap->data() + ptr;*/
    int nn = 0;

    memcpy(st, tbase, sizeof(j2k_data_struct));

    nn += sizeof(j2k_data_struct);
    std::string kor = std::string(tbase + nn);
    nn += st->m_WordSize + 1;
    std::string pumsa = std::string(tbase + nn);
    nn += st->m_PumsaSize + 1;
    std::string prop1 = std::string(tbase + nn);
    nn += st->m_PropertyTypeASize + 1;
    std::string prop2 = std::string(tbase + nn);
    nn += st->m_PropertyTypeBSize + 1;
    std::string dict = std::string(tbase + nn);

    return std::make_unique<j2k_base_dict>(id, kor, pumsa, prop1, prop2, dict, st->m_NextEntry, st->m_ExtraParam, 0);
	}

public:
	bool is_initialize = false;
	tsl::htrie_map<char, std::int32_t> trie_map;
	std::vector<std::unique_ptr<j2k_base_dict>> vec;
  std::unique_ptr<mio::basic_mmap_sink<char>> data_mmap;
  size_t data_length;
  size_t data_size;
};
