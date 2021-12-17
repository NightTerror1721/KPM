#include "bytestream.h"
#include <fstream>
#include <exception>

#define check_initiated(_Stream) if(!(_Stream)) throw std::exception{ "unloaded stream" }

namespace kpm
{
	ByteOutputStream::ByteOutputStream(ByteOutputStream&& right) noexcept :
		_os{ right._os },
		_inner{ right._inner }
	{
		right._os = nullptr;
	}

	ByteOutputStream& ByteOutputStream::operator= (ByteOutputStream&& right) noexcept
	{
		this->~ByteOutputStream();
		_os = right._os;
		_inner = right._inner;
		right._os = nullptr;
		return *this;
	}

	ByteOutputStream::~ByteOutputStream()
	{
		if (_os && _inner)
			delete _os;
	}

	ByteOutputStream::ByteOutputStream(std::ostream& os) :
		_os{ std::addressof(os) },
		_inner{ false }
	{}

	ByteOutputStream::ByteOutputStream(const std::filesystem::path& path) :
		_os{ new std::ofstream{ path, std::ios::out | std::ios::binary } },
		_inner{ true }
	{}

	void ByteOutputStream::open(std::ostream& os)
	{
		close();
		_os = std::addressof(os);
		_inner = false;
	}

	void ByteOutputStream::open(const std::filesystem::path& path)
	{
		close();
		_os = new std::ofstream{ path, std::ios::out | std::ios::binary };
		_inner = true;
	}

	void ByteOutputStream::close()
	{
		if (_os && _inner)
			delete _os;
		_os = nullptr;
		_inner = false;
	}

	size_t ByteOutputStream::write(const void* data, size_t size)
	{
		check_initiated(_os);
		decltype(_os->tellp()) off = _os->tellp();
		_os->write(reinterpret_cast<const char*>(data), size);
		return static_cast<size_t>(_os->tellp() - off);
	}

	void ByteOutputStream::flush()
	{
		check_initiated(_os);
		_os->flush();
	}
}

namespace kpm
{
	ByteInputStream::ByteInputStream(ByteInputStream&& right) noexcept :
		_is{ right._is },
		_inner{ right._inner }
	{
		right._is = nullptr;
	}

	ByteInputStream& ByteInputStream::operator= (ByteInputStream&& right) noexcept
	{
		this->~ByteInputStream();
		_is = right._is;
		_inner = right._inner;
		right._is = nullptr;
		return *this;
	}

	ByteInputStream::~ByteInputStream()
	{
		if (_is && _inner)
			delete _is;
	}

	ByteInputStream::ByteInputStream(std::istream& is) :
		_is{ std::addressof(is) },
		_inner{ false }
	{}

	ByteInputStream::ByteInputStream(const std::filesystem::path& path) :
		_is{ new std::ifstream{ path, std::ios::in | std::ios::binary } },
		_inner{ true }
	{}

	void ByteInputStream::open(std::istream& os)
	{
		close();
		_is = std::addressof(os);
		_inner = false;
	}

	void ByteInputStream::open(const std::filesystem::path& path)
	{
		close();
		_is = new std::ifstream{ path, std::ios::in | std::ios::binary };
		_inner = true;
	}

	void ByteInputStream::close()
	{
		if (_is && _inner)
			delete _is;
		_is = nullptr;
		_inner = false;
	}

	size_t ByteInputStream::read(void* buffer, size_t size)
	{
		check_initiated(_is);
		_is->read(reinterpret_cast<char*>(buffer), size);
		decltype(_is->gcount()) cnt = _is->gcount();
		return cnt < 0 ? 0 : static_cast<size_t>(cnt);
	}
}
