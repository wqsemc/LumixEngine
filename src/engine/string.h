#pragma once


#include "engine/lumix.h"


namespace Lumix
{


struct IAllocator;


LUMIX_ENGINE_API const char* stristr(const char* haystack, const char* needle);
LUMIX_ENGINE_API bool toCStringHex(u8 value, Span<char> output);
LUMIX_ENGINE_API bool toCStringPretty(i32 value, Span<char> output);
LUMIX_ENGINE_API bool toCStringPretty(u32 value, Span<char> output);
LUMIX_ENGINE_API bool toCStringPretty(u64 value, Span<char> output);
LUMIX_ENGINE_API bool toCString(i32 value, Span<char> output);
LUMIX_ENGINE_API bool toCString(i64 value, Span<char> output);
LUMIX_ENGINE_API bool toCString(u64 value, Span<char> output);
LUMIX_ENGINE_API bool toCString(u32 value, Span<char> output);
LUMIX_ENGINE_API bool toCString(float value, Span<char> output, int after_point);
LUMIX_ENGINE_API bool toCString(double value, Span<char> output, int after_point);
LUMIX_ENGINE_API const char* reverseFind(const char* begin_haystack, const char* end_haystack, char c);
LUMIX_ENGINE_API const char* fromCStringOctal(Span<const char> input, Ref<u32> value);
LUMIX_ENGINE_API const char* fromCString(Span<const char> input, Ref<i32> value);
LUMIX_ENGINE_API const char* fromCString(Span<const char> input, Ref<u64> value);
LUMIX_ENGINE_API const char* fromCString(Span<const char> input, Ref<i64> value);
LUMIX_ENGINE_API const char* fromCString(Span<const char> input, Ref<u32> value);
LUMIX_ENGINE_API const char* fromCString(Span<const char> input, Ref<u16> value);
LUMIX_ENGINE_API bool copyString(Span<char> output, const char* source);
LUMIX_ENGINE_API bool copyString(Span<char> output, Span<const char> source);
LUMIX_ENGINE_API bool copyNString(Span<char> output, const char* source, int N);
LUMIX_ENGINE_API bool catString(Span<char> output, const char* source);
LUMIX_ENGINE_API bool catNString(Span<char> output, const char* source, int N);
LUMIX_ENGINE_API bool makeLowercase(Span<char> output, const char* source);
LUMIX_ENGINE_API bool startsWith(const char* str, const char* prefix);
LUMIX_ENGINE_API int stringLength(const char* str);
LUMIX_ENGINE_API bool equalStrings(const char* lhs, const char* rhs);
LUMIX_ENGINE_API bool equalIStrings(const char* lhs, const char* rhs);
LUMIX_ENGINE_API int compareMemory(const void* lhs, const void* rhs, size_t size);
LUMIX_ENGINE_API int compareString(const char* lhs, const char* rhs);
LUMIX_ENGINE_API int compareStringN(const char* lhs, const char* rhs, int length);
LUMIX_ENGINE_API int compareIStringN(const char* lhs, const char* rhs, int length);
LUMIX_ENGINE_API void copyMemory(void* dest, const void* src, size_t count);
LUMIX_ENGINE_API void moveMemory(void* dest, const void* src, size_t count);
LUMIX_ENGINE_API void setMemory(void* ptr, u8 value, size_t num);
LUMIX_ENGINE_API const char* findSubstring(const char* str, const char* substr);
LUMIX_ENGINE_API bool endsWith(const char* str, const char* substr);


inline bool isLetter(char c)
{
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}


inline bool isNumeric(char c)
{
	return c >= '0' && c <= '9';
}


inline bool isUpperCase(char c)
{
	return c >= 'A' && c <= 'Z';
}


template <int SIZE> bool copyString(char(&destination)[SIZE], const char* source)
{
	return copyString(Span<char>(destination, SIZE), source);
}

template <int SIZE> bool catString(char(&destination)[SIZE], const char* source)
{
	return catString(Span<char>(destination, SIZE), source);
}


template <int SIZE> struct StaticString
{
	StaticString() { data[0] = '\0'; }

	explicit StaticString(const char* str) { copyString(Span(data), str); }

	template <typename... Args> StaticString(const char* str, Args... args)
	{
		copyString(data, str);
		int tmp[] = { (add(args), 0)... };
		(void)tmp;
	}

	template <int value_size> StaticString& operator<<(StaticString<value_size>& value)
	{
		add(value);
		return *this;
	}

	template <typename T> StaticString& operator<<(T value)
	{
		add(value);
		return *this;
	}

	template <int value_size> void add(StaticString<value_size>& value) { catString(data, value.data); }
	void add(const char* value) { catString(data, value); }
	void add(char* value) { catString(data, value); }
	void add(const Span<const char>& value) { catNString(Span(data), value.begin(), value.length()); }

	void operator=(const char* str) { copyString(data, str); }

	void add(char value)
	{
		char tmp[2] = {value, 0};
		catString(data, tmp);
	}

	void add(float value)
	{
		int len = stringLength(data);
		toCString(value, Span(data).fromLeft(len), 3);
	}

	template <typename T> void add(T value)
	{
		int len = stringLength(data);
		toCString(value, Span(data + len, SIZE - len));
	}

	bool operator<(const char* str) const {
		return compareString(data, str) < 0;
	}

	bool operator==(const char* str) const {
		return equalStrings(data, str);
	}

	bool operator!=(const char* str) const {
		return !equalStrings(data, str);
	}

	StaticString<SIZE> operator +(const char* rhs)
	{
		return StaticString<SIZE>(*this, rhs);
	}

	bool empty() const { return data[0] == '\0'; }

	operator const char*() const { return data; }
	char data[SIZE];
};


class LUMIX_ENGINE_API String
{
public:
	explicit String(IAllocator& allocator);
	String(const String& rhs, int start, i32 length);
	String(Span<const char> rhs, IAllocator& allocator);
	String(const String& rhs);
	String(String&& rhs);
	String(const char* rhs, IAllocator& allocator);
	~String();

	void resize(int size);
	char* getData() { return m_cstr; }
	char operator[](int index) const;
	void set(const char* rhs, int size);
	void operator=(const String& rhs);
	void operator=(const char* rhs);
	bool operator!=(const String& rhs) const;
	bool operator!=(const char* rhs) const;
	bool operator==(const String& rhs) const;
	bool operator==(const char* rhs) const;
	bool operator<(const String& rhs) const;
	bool operator>(const String& rhs) const;
	int length() const { return m_size; }
	const char* c_str() const { return m_cstr; }
	String substr(int start, int length) const;
	String& cat(Span<const char> value);
	String& cat(float value);
	String& cat(char* value);
	String& cat(const char* value);
	void insert(int position, const char* value);
	void eraseAt(int position);

	template <class V> String& cat(V value)
	{
		char tmp[30];
		toCString(value, Span<char>(tmp, 30));
		cat(tmp);
		return *this;
	}

	IAllocator& m_allocator;
private:
	i32 m_size;
	char* m_cstr;
};



} // namespace Lumix
