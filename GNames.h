#pragma once
#include "include.h"
#include "SDK.h"

/* Gnames */
int PlayerMale, PlayerFemale, RegistedPlayer, RegistedPlayer2, AIPawnBase, BotPlayerFemale, BotPlayerMale, ak47, aug, awm, berylm762, fnfal, g36c, groza, hk416, kar98k, m16a4, m24, mini14, mk14, mk74, qbu, scar, sks, qbz;
/* Gnames */

vector<std::string> GNamesStr;

std::string GetNameFromIdUsingGName(UINT64 GN, int ID)
{
	UINT64 fNamePtr = drv.RPM<UINT64>(GN + ((int(ID / __OFFSET__ChunkSize)) * 8));
	UINT64 fName = drv.RPM<UINT64>(fNamePtr + ((int(ID % __OFFSET__ChunkSize)) * 8));
	char names_c[64];
	drv.RPM(fName + 0x10, names_c, sizeof(names_c));
	string names = string(names_c);
	if (names == "")
		names = "fail";
	return names;
}

string getName(int id) {
	string returnv = string(to_string(GNamesStr.size()) + " | " + to_string(id));
	if (GNamesStr.size() > id)
		returnv = GNamesStr[id];

	return returnv;
}

uint64_t FindGnames() {
	UINT64 gnames = 0;
	UINT64 tmp = drv.RPM<UINT64>(drv.GetGameModule() + __OFFSET__GNames);
	DWORD64 v11 = Decrypt(tmp);
	v11 = drv.RPM<UINT64>(v11);
	DWORD64 v12 = Decrypt(v11);
	v12 = drv.RPM<UINT64>(v12);
	DWORD64 v13 = Decrypt(v12);

	gnames = drv.RPM<UINT64>(v13);
	gnames = Decrypt(gnames);
	tmp = gnames;
	gnames = 0;
#pragma region NameDumper

	gnames = tmp;
	if (GetNameFromIdUsingGName(gnames, 1) == "ByteProperty" && GetNameFromIdUsingGName(gnames, 2) == "IntProperty" && GetNameFromIdUsingGName(gnames, 3) == "BoolProperty")
	{
#pragma region NameDumper
		int ia = 0;

		while (1)
		{
			string Gres = GetNameFromIdUsingGName(gnames, ia);
			if (Gres == "PlayerFemale_A_C")
			{
				PlayerFemale = ia;
			}
			if (Gres == "PlayerMale_A_C")
			{
				PlayerMale = ia;
			}
			if (Gres == "AIPawn_Base_Female_C") {
				BotPlayerFemale = ia;
				//file << Gres << " | " << ia << endl;
			}
			if (Gres == "AIPawn_Base_Male_C") {
				BotPlayerMale = ia;
				// file << Gres << " | " << ia << endl;
			}
			if (Gres == "WeapAK47_C")
			{
				ak47 = ia;
			}
			if (Gres == "WeapAUG_C")
			{
				aug = ia;
			}
			if (Gres == "WeapQBZ95_C")
			{
				qbz = ia;
			}
			if (Gres == "WeapKar98k_C")
			{
				kar98k = ia;
			}
			if (Gres == "WeapJuliesKar98k_C")
			{
				kar98k = ia;
			}
			if (Gres == "WeapM24_C")
			{
				m24 = ia;
			}
			if (Gres == "WeapSKS_C")
			{
				sks = ia;
			}
			if (Gres == "WeapFNFal_C")
			{
				fnfal = ia;
			}
			if (Gres == "WeapSCAR-L_C")
			{
				scar = ia;
			}
			if (Gres == "WeapGroza_C")
			{
				groza = ia;
			}
			if (Gres == "WeapHK416_C")
			{
				hk416 = ia;
			}
			if (Gres == "WeapDuncansHK416_C")
			{
				hk416 = ia;
			}
			if (Gres == "WeapM16A4_C")
			{
				m16a4 = ia;
			}
			if (Gres == "WeapAWM_C")
			{
				awm = ia;
			}
			if (Gres == "WeapMini14_C")
			{
				mini14 = ia;
			}
			if (Gres == "WeapMadsQBU88_C")
			{
				qbu = ia;
			}
			if (Gres == "WeapMk14_C")
			{
				mk14 = ia;
			}
			if (Gres == "WeapG36C_C")
			{
				g36c = ia;
			}
			if (Gres == "WeapBerylM762_C")
			{
				berylm762 = ia;
			}
			if (Gres == "WeapMk47Mutant_C")
			{
				mk74 = ia;

			}
			GNamesStr.push_back(Gres);

			if (ia > 350000)
				break;
			ia++;
		}
#pragma endregion
		UINT64 GNAMES_P = gnames;
		return gnames;
	}
	MessageBox(NULL, "Defined Error: G_N1", "ERROR", MB_ICONERROR);
	exit(1);
	return 0;
}