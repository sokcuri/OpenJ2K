#pragma once

#include <fstream>

struct serializer {
	std::ofstream m_ostream;

	serializer(std::string file_name)
	{
		m_ostream.exceptions(m_ostream.badbit | m_ostream.failbit);
		m_ostream.open(file_name, std::ios::binary | std::ios::out);
	}

	template<typename T>
	void operator()(const T& value)
	{
		m_ostream.write(reinterpret_cast<const char*>(&value), sizeof(T));
	}

	template<typename CharT>
	void operator()(const CharT* value, std::size_t value_size)
	{
		m_ostream.write(value, value_size);
	}
};

struct deserializer {
	std::ifstream m_istream;

	deserializer(std::string file_name) {
		m_istream.exceptions(m_istream.badbit | m_istream.failbit | m_istream.eofbit);
		m_istream.open(file_name, std::ios::binary | std::ios::in);
	}

	template<typename T>
	T operator()()
	{
		T value;
		m_istream.read(reinterpret_cast<char*>(&value), sizeof(T));

		return value;
	}

	template<typename CharT>
	void operator()(CharT* value_out, std::size_t value_size)
	{
		m_istream.read(value_out, value_size);
	}
};
