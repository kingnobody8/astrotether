#include "func.h"

namespace util
{
	bool BigEndianCheck(void)
	{
		union
		{
			u32 i;
			uchar c[4];
		} bint = { 0x01020304 };
		return bint.c[0] == 1;
	}

	/*void TypeCheck(void)
	{
		SDL_Log("Printing sizeof standard types.\n");
		SDL_Log("Sizeof - char: \t\t%d\n", sizeof(char));
		SDL_Log("Sizeof - short: \t\t%d\n", sizeof(short));
		SDL_Log("Sizeof - long: \t\t%d\n", sizeof(long));
		SDL_Log("Sizeof - long long: \t%d\n", sizeof(long long));
		SDL_Log("Sizeof - float: \t\t%d\n", sizeof(float));
		SDL_Log("Sizeof - double: \t\t%d\n", sizeof(double));
		SDL_Log("Sizeof - long double: %d\n", sizeof(long double));
		SDL_Log("Sizeof - int: \t\t%d\n\n", sizeof(int));

		SDL_Log("Printing sizeof custom types.\n");
		SDL_Log("Sizeof - uchar: \t\t%d\n", sizeof(uchar));
		SDL_Log("Sizeof - ushort: \t\t%d\n", sizeof(ushort));
		SDL_Log("Sizeof - ulong: \t\t%d\n", sizeof(ulong));
		SDL_Log("Sizeof - ulonglong: \t%d\n", sizeof(ulonglong));
		SDL_Log("Sizeof - uint: \t\t%d\n\n", sizeof(uint));

		if (BigEndianCheck())
			SDL_Log("Big Endian\n");
		else
			SDL_Log("Lil Endian\n");
	}*/
}
