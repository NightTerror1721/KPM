#pragma once

#include <iostream>
#include <filesystem>
#include <concepts>

namespace kpm
{
	class ByteOutputStream
	{
	private:
		std::ostream* _os = nullptr;
		bool _inner = false;

	public:
		ByteOutputStream() = default;
		ByteOutputStream(const ByteOutputStream&) = delete;
		ByteOutputStream& operator= (const ByteOutputStream&) = delete;

	public:
		ByteOutputStream(ByteOutputStream&& right) noexcept;
		ByteOutputStream& operator= (ByteOutputStream&& right) noexcept;
		~ByteOutputStream();

		ByteOutputStream(std::ostream& os);
		ByteOutputStream(const std::filesystem::path& path);

		void open(std::ostream& os);
		void open(const std::filesystem::path& path);

		void close();

	public:
		size_t write(const void* data, size_t size);

		void flush();

	public:
		inline bool opened() const { return _os; }

		template <typename _Ty> requires std::integral<_Ty> || std::floating_point<_Ty>
		inline size_t write(_Ty value) { return write(&value, sizeof(_Ty)); }

		template <typename _Ty>
		inline size_t write(const _Ty& data) { return write(std::addressof(value), sizeof(_Ty)); }

		template <typename _Ty> requires std::integral<_Ty> || std::floating_point<_Ty>
		inline ByteOutputStream& operator<< (_Ty right) { return write(right), *this; }

		template <typename _Ty>
		inline ByteOutputStream& operator<< (_Ty right) { return write(right), *this; }

		inline operator bool() const { return !(!(*_os)); }
		inline bool operator! () const { return !(*_os); }
	};



	class ByteInputStream
	{
	private:
		std::istream* _is;
		bool _inner;

	public:
		ByteInputStream() = default;
		ByteInputStream(const ByteInputStream&) = delete;
		ByteInputStream& operator= (const ByteInputStream&) = delete;

	public:
		ByteInputStream(ByteInputStream&& right) noexcept;
		ByteInputStream& operator= (ByteInputStream&& right) noexcept;
		~ByteInputStream();

		ByteInputStream(std::istream & is);
		ByteInputStream(const std::filesystem::path & path);

		void open(std::istream& os);
		void open(const std::filesystem::path & path);

		void close();

	public:
		size_t read(void* buffer, size_t size);

	public:
		inline bool eof() const { return _is->eof(); }

		template <typename _Ty> requires std::integral<_Ty> || std::floating_point<_Ty>
		inline bool read(_Ty& dst) { return read(&dst, sizeof(_Ty)) == sizeof(_Ty); }

		template <typename _Ty> requires std::integral<_Ty> || std::floating_point<_Ty>
		inline _Ty read() { _Ty value{}; return read(&value, sizeof(_Ty)), value; }

		template <typename _Ty>
		inline bool read(_Ty& dst) { return read(std::addressof(dst), sizeof(_Ty)) == sizeof(_Ty); }

		template <typename _Ty> requires std::default_initializable<_Ty> && std::move_constructible<_Ty>
		inline _Ty read() { _Ty data{}; return read(std::addressof(dst), sizeof(_Ty)), value; }

		template <typename _Ty> requires std::integral<_Ty> || std::floating_point<_Ty>
		inline ByteInputStream& operator>> (_Ty & right) { return read(right), *this; }

		template <typename _Ty>
		inline ByteInputStream& operator>> (_Ty& right) { return read(right), *this; }

		inline operator bool() const { return !(!(*_is)); }
		inline bool operator! () const { return !(*_is); }
	};
}
