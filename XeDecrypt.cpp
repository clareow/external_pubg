#include "XeDecrypt.h"
namespace Decrypts
{
	DWORD DecryptCIndex(DWORD value)
	{
		/*DWORD v10 = _rotr(value ^ 0x9199156E, 0x06);
		DWORD result = v10 ^ (v10 << 0x10) ^ 0xEC4708C3;
		return result;*/

		return __ROL4__(value ^ 0x66FF60D8, 3) ^ (__ROL4__(value ^ 0x66FF60D8, 3) << 16) ^ 0xD3579094;
	}
}