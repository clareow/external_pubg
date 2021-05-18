#pragma once
#include "include.h"
/* Memory Unit */
#include "Memory.h"
/* Memory Unit */
using namespace std;
extern "C" DWORD64 Shield_Decrypt(__int64 seed, __int64 value, DWORD64 fnShield, DWORD64 ShieldStart);
static BYTE fnShieldDecrypt[0x200];
namespace Decrypts
{
	DWORD DecryptCIndex(DWORD value);
}