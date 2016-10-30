#pragma once
#include "helper/platform.h"
#include "helper/macro.h"
#include <fstream>
#include <assert.h>


#ifndef NULL
#define NULL 0
#endif

#ifndef null
#define null 0
#endif

//THE GOOGLES DO NOTHING!!!
#define STATIC
#define VIRTUAL
#define OUT
#define IN


#ifdef IS_x64
//Unsigned
typedef unsigned char		u08;
typedef unsigned short int	u16;
typedef unsigned int		u32;
typedef unsigned long long	u64;
//Signed
typedef signed char			s08;
typedef signed short int	s16;
typedef signed int			s32;
typedef signed long long	s64;
#else //assume 32 bit
//Unsigned
typedef unsigned char		u08;
typedef unsigned short int	u16;
typedef unsigned long int	u32;
typedef unsigned long long	u64;
//Signed
typedef signed char			s08;
typedef signed short int	s16;
typedef signed long int		s32;
typedef signed long long	s64;
#endif

//Explicit Unsigned Names
typedef u08					uchar;
typedef u16					ushort;
typedef u32					ulong;
typedef u64					ulonglong;
//Explicit Signed Names
typedef s08					schar;
typedef s16					sshort;
typedef s32					slong;
typedef s64					slonglong;

//Int
typedef unsigned int		uint;
typedef signed int			sint;

//Buffer
struct TBuffer
{
	void* buffer;
	ulonglong size;
	TBuffer() : buffer(null), size(0) {}
	TBuffer(void* const b, const slong& s) : buffer(b), size(s) {}
	~TBuffer()
	{
		clear();
	}

	bool LoadFromFile(const std::string szFilePath)
	{
		std::fstream stream;

		stream.open(szFilePath, std::ios_base::in | std::ios_base::binary | std::ios_base::ate);

		assert(stream.is_open());
		if (!stream.is_open())
			return false;

		size = stream.tellg();
		buffer = new uchar[size + 1];
		stream.seekg(0, stream.beg);
		stream.read(reinterpret_cast<char*>(buffer), size);
		reinterpret_cast<char*>(buffer)[size] = null;

		stream.close();
		return true;
	}

	void clear()
	{
		if (buffer)
		{
			delete []buffer;
			buffer = null;
			size = 0;
		}
	}
};
