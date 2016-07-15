#pragma once
#include "types.h"
#include <assert.h>
#include <climits>

namespace util
{
	template<typename type>
	struct BitFlag
	{
	private:
		type val;

	private:
		inline uchar GetMaxBit(void) const { return sizeof(type) * 8 - 1; }
	public:
		BitFlag(void)
			: val(null)
		{
		}
		BitFlag(const type& v)
			:val(v)
		{
		}

		/*BIT FUNCS USE 0, 1, 2, 3, ect*/

		// Turns a bit ON
		void BitOn(const uchar& bit)
		{
			assert(bit < this->GetMaxBit());
			val |= (1 << bit);
		}
		// Turns a bit OFF
		void BitOff(const uchar& bit)
		{
			assert(bit < this->GetMaxBit());
			val &= ~(1 << bit);
		}
		// Toggles a bit
		void BitTog(const uchar& bit)
		{
			assert(bit < this->GetMaxBit());
			val ^= (1 << bit);
		}
		// Sets the given bit to the boolen value
		void BitBool(const uchar& bit, const bool b)
		{
			assert(bit < this->GetMaxBit());
			if (b)
			{
				val |= (1 << bit);
			}
			else
			{
				val &= ~(1 << bit);
			}
		}
		//Returns the result of a bit
		bool Bit(const uchar& bit) const
		{
			assert(bit < this->GetMaxBit());
			return (val & (1 << bit)) != 0;
		}

		/*FLAG FUNCS USE 0x01, 0x02, 0x04, 0x08, ect*/

		//Turns on the bits corresponding to this flag(s)
		void FlagOn(const type& flag)
		{
			val |= flag;
		}
		//Turns off the bits corresponding to this flag(s)
		void FlagOff(const type& flag)
		{
			val &= ~flag;
		}
		//Toggles the bits corresonding to this flag(s)
		void FlagTog(const type& flag)
		{
			val ^= flag;
		}
		//Sets the given flag(s) to the boolean value
		void FlagBool(const type& flag, const bool b)
		{
			if (b)
			{
				val |= flag;
			}
			else
			{
				val &= ~flag;
			}
		}
		//Returns the result of the corresponding flag(s)
		bool Flag(const type& flag) const
		{
			const type tmp = val & flag;
			return tmp == flag;
		}

		// Zeros out the whole flag
		void ZeroOut(void)
		{
			val = 0;
		}
	};

	typedef BitFlag<u08> Flag08;
	typedef BitFlag<u16> Flag16;
	typedef BitFlag<u32> Flag32;
	typedef BitFlag<u64> Flag64;
}
