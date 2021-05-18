#include "include.h"
#include "GNames.h"
#include "Entitys.h"
#include "Aimbot.h"
#include "Settings.h"
#include "Proxy.h"
#include "SDK.h"
#include "driver.h"

/* Random */
static const char alphanum[] =
"0123456789"
"!@#$%^&*"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";
int stringLength = sizeof(alphanum) - 1;
/* Random */

extern void initD3D(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);    // create the Direct3D interface

	D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information

	std::memset(&d3dpp, 0, sizeof(d3dpp)); // clear out the struct for use
	d3dpp.Windowed = TRUE;    // program windowed, not fullscreen
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
	d3dpp.hDeviceWindow = hWnd;    // set the window to be used by Direct3D
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;     // set the back buffer format to 32-bit
	d3dpp.BackBufferWidth = Width;    // set the width of the buffer
	d3dpp.BackBufferHeight = Height;    // set the height of the buffer

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// create a device class using this information and the info from the d3dpp stuct
	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);
	D3DXCreateFontA(d3ddev, 25, 0, FW_HEAVY, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Malgun Gothic", &pFont);
	D3DXCreateLine(d3ddev, &d3dLine);
}

VOID Cleanup()
{
	d3dLine->Release();
	d3d->Release();
	d3ddev->Release();
}

void DrawLine(float x, float y, float xx, float yy, D3DCOLOR color, float ww)
{
	D3DXVECTOR2 dLine[2];

	d3dLine->SetWidth(ww);

	dLine[0].x = x;
	dLine[0].y = y;

	dLine[1].x = xx;
	dLine[1].y = yy;

	d3dLine->Draw(dLine, 2, color);
}

void Circle(int X, int Y, int radius, int numSides, int r, int g, int b, int a)
{
	D3DCOLOR Colors = D3DCOLOR_ARGB(a, r, g, b);

	float Step = (float)(PI * 2.0 / numSides);
	int Count = 0;
	for (float a = 0; a < PI * 2.0; a += Step)
	{
		float X1 = (float)(radius * cos(a) + X);
		float Y1 = (float)(radius * sin(a) + Y);
		float X2 = (float)(radius * cos(a + Step) + X);
		float Y2 = (float)(radius * sin(a + Step) + Y);
		if (Count != 0)
		{
			DrawLine(X1, Y1, X2, Y2, Colors, 1.5f);
		}
		Count += 2;
	}
}

int DrawStr(int x, int y, DWORD color, LPD3DXFONT g_pFont, const char* fmt, ...)
{
	char buf[1024] = { 0 };
	va_list va_alist;
	RECT FontPos = { x, y, x + 120, y + 16 };
	va_start(va_alist, fmt);
	vsprintf_s(buf, fmt, va_alist);
	va_end(va_alist);
	try {
		return g_pFont->DrawTextA(NULL, buf, -1, &FontPos, DT_NOCLIP, color);
	}
	catch (int e) {
		return 0;
	}
}

int DrawString(int x, int y, DWORD color, LPD3DXFONT g_pFont, const char* fmt, ...)
{
	DrawStr(x - 1, y, D3DCOLOR_ARGB(255, 1, 1, 1), g_pFont, fmt);
	DrawStr(x + 1, y, D3DCOLOR_ARGB(255, 1, 1, 1), g_pFont, fmt);
	DrawStr(x, y - 1, D3DCOLOR_ARGB(255, 1, 1, 1), g_pFont, fmt);
	DrawStr(x, y + 1, D3DCOLOR_ARGB(255, 1, 1, 1), g_pFont, fmt);
	return DrawStr(x, y, color, g_pFont, fmt);
}

float DistanceCross(float X, float Y)
{
	float ydist = (Y - (Height / 2));
	float xdist = (X - (Width / 2));
	float Hypotenuse = sqrt(pow(ydist, 2) + pow(xdist, 2));
	return Hypotenuse;
}

void DrawStr2(int x, int y, DWORD color, LPD3DXFONT g_pFont, const char* fmt, ...)
{
	char buf[1024] = { 0 };
	va_list va_alist;
	RECT FontPos = { x, y, x + 150, y + 20 };
	va_start(va_alist, fmt);
	vsprintf_s(buf, fmt, va_alist);
	va_end(va_alist);
	g_pFont->DrawTextA(NULL, buf, -1, &FontPos, DT_NOCLIP, color);
}

void DrawBox(float x, float y, float width, float height, D3DCOLOR color, float ww)
{
	D3DXVECTOR2 points[5];
	points[0] = D3DXVECTOR2(x, y);
	points[1] = D3DXVECTOR2(x + width, y);
	points[2] = D3DXVECTOR2(x + width, y + height);
	points[3] = D3DXVECTOR2(x, y + height);
	points[4] = D3DXVECTOR2(x, y);
	d3dLine->SetWidth(ww);
	d3dLine->Draw(points, 5, color);
}

void FillRGB(float x, float y, float w, float h, int r, int g, int b, int a)
{
	D3DXVECTOR2 vLine[2];

	d3dLine->SetWidth(w);

	vLine[0].x = x + w / 2;
	vLine[0].y = y;
	vLine[1].x = x + w / 2;
	vLine[1].y = y + h;

	d3dLine->Begin();
	d3dLine->Draw(vLine, 2, D3DCOLOR_RGBA(r, g, b, a));
	d3dLine->End();
}

void DrawBar(int x, int y, int value)
{
	float l, r, g;

	g = value * 2.55;
	r = 255 - g;
	l = value / 3;
	FillRGB(x - (l / 2) - 1, y - 1, l + 2 + 13, 5, 0, 0, 0, 255);
	FillRGB(x - (l / 2), y, l + 13, 3, r, g, 0, 255);
}

class EntityData
{
public:
	uint64_t pAActor;
	uint64_t MeshComponent;
	uint64_t RootComponent;

	bool IsMyTeam;
};

std::vector<EntityData> HongEntities{};

void UpdateHongEntities()
{
	std::vector<EntityData> CurrrentHongEntities{};

	for (int i = 0; i < ActorCount; ++i)
	{
		auto Entity = drv.RPM<DWORD_PTR>(Actor + (i * 0x8));
		DWORD_PTR ActorID = Decrypts::DecryptCIndex(drv.RPM<int>(Entity + __OFFSET__ActorID));
		if (ActorID == PlayerFemale || ActorID == PlayerMale || ActorID == BotPlayerFemale || ActorID == BotPlayerMale)
		{
			EntityData currentEntity;
			currentEntity.pAActor = Entity;
			currentEntity.MeshComponent = drv.RPM<DWORD_PTR>(Entity + __OFFSET__ActorMesh);
			currentEntity.RootComponent = Decrypt(drv.RPM<DWORD_PTR>(Entity + __OFFSET__RootComponent));
			currentEntity.IsMyTeam = drv.RPM<uint8_t>(LocalPlayerPawn + __OFFSET__ActorTeam) == drv.RPM<uint8_t>(Entity + __OFFSET__ActorTeam);

			CurrrentHongEntities.push_back(currentEntity);
		}
	}

	HongEntities = CurrrentHongEntities;
}

void render()
{
	Vector3 center = Vector3(Width / 1.5f, Height - 150, 0);

	float DonguId = -1;
	float Dongudistance = 99999;
	float DonguDist = 99999;

	cameracache = GetCameraCache();

	FOV = tanf(DEG2RAD(5.f) / 2) / tanf(DEG2RAD(cameracache.POV.FOV) / 2) * (Width);

	for (int i = 0; i < HongEntities.size(); ++i)
	{
		float Health = GetActorHealth(HongEntities[i].pAActor);
		float GroggyHealth = GetActorGroggyHealth(HongEntities[i].pAActor);

		bool IsVisible = drv.RPM<float>(HongEntities[i].MeshComponent + __OFFSET__LastRenderTime) + 0.04f >= drv.RPM<float>(HongEntities[i].MeshComponent + __OFFSET__LastRenderTime - 0x8);
		float distance = cameracache.POV.Location.Distance(drv.RPM<Vector3>(HongEntities[i].RootComponent + __OFFSET_ROOTCOMPONENT_LOCATION, sizeof(Vector3))) / 100.f;

		DrawString(Width / 2, 0, D3DCOLOR_ARGB(255, 255, 0, 0), pFont, ("관전자[" + to_string(SpectateCount) + "]").c_str());

		bool Top = GetForegroundWindow() == FindWindow("UnrealWindow", NULL);

		CURSORINFO ci = { sizeof(CURSORINFO) };
		GetCursorInfo(&ci);

		if (Top)
		{
			if (ci.flags == 0)
			{
				SetCursorPos(99999, 99999);

				HWND hwnd2 = GetForegroundWindow();
				HWND hwnd3 = GetWindow(hwnd2, GW_HWNDPREV);
				SetWindowPos(hWnd, hwnd3, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				UpdateWindow(hWnd);
			}
			else
			{
				if (GetAsyncKeyState(VK_TAB) & 0x1)
				{
					SetCursorPos(99999, 99999);
				}

				SetWindowPos(gWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				UpdateWindow(hWnd);
			}
		}
		if ((Health || GroggyHealth) && !HongEntities[i].IsMyTeam && distance < 600)
		{
			Vector3 bones = WorldToScreen(GetBoneWithRotation(HongEntities[i].MeshComponent, EBoneIndex::forehead), cameracache);
			Vector3 bones3 = WorldToScreen(GetBoneWithRotation(HongEntities[i].MeshComponent, EBoneIndex::Head), cameracache);
			Vector3 localhead = GetBoneWithRotation(LocalMesh, EBoneIndex::forehead);
			Vector3 bones2 = GetBoneWithRotation(HongEntities[i].MeshComponent, EBoneIndex::forehead);
			Vector3 HeadBox = Vector3(bones.x, bones.y, bones.z + 15);
			Vector3 rootw2s = WorldToScreen(GetBoneWithRotation(HongEntities[i].MeshComponent, EBoneIndex::root), cameracache);
			float Dist = DistanceCross(bones.x, bones.y);
			float distance = localhead.Distance(bones2) / 100.f;

			if (Settings::ESP)
				DrawSkeleton(HongEntities[i].MeshComponent, IsVisible);

			if (Settings::HPBar)
			{
				float Height1 = abs(HeadBox.y - rootw2s.y);
				float Width1 = Height1 * 0.65;
				float healthbar_mig = bones.y - bones3.y;
				if (Health >= 100)
					Health = 100;

				XorS(dst, "[%.fm]\n");

				char dist[64];
				sprintf_s(dist, dst.decrypt(), distance);
				DrawString(rootw2s.x - (Width1 / 2), rootw2s.y, D3DCOLOR_ARGB(255, 92, 209, 229), pFont, dist);
				DrawBar(rootw2s.x - (Width1 / 2), rootw2s.y, (int)Health);
			}


			if (GetAsyncKeyState(VK_RBUTTON) && Settings::aimbot)
			{
				if (IsVisible)
				{
					if ((Dist <= FOV) && distance <= 600 && Dist < DonguDist && distance < Dongudistance)
					{
						Dongudistance = distance;
						DonguDist = Dist;
						DonguId = i;
					}

					if (distance < 450 && Dist < FOV)
					{
						Vector3 Velocity;
						uint64_t VehicleComponent = (drv.RPM<uint64_t>(HongEntities[i].pAActor + __OFFSET__VehicleRiderComponent));

						uint64_t VehiclePawn = drv.RPM<uint64_t>(VehicleComponent + __OFFSET__LastVehiclePawn);

						Vector3 VehicleVelocity = drv.RPM<Vector3>(VehiclePawn + __OFFSET__ReplicatedMovement, sizeof(Vector3));

						int SeatIndex = drv.RPM<int>(VehicleComponent + __OFFSET__SEATINDEX);

						//Velocity = drv.RPM<Vector3>(HongEntities[i].RootComponent + __OFFSET__ActorVelocity, sizeof(Vector3));

						if (SeatIndex == -1)
						{
							Velocity = drv.RPM<Vector3>(HongEntities[i].RootComponent + __OFFSET__ActorVelocity, sizeof(Vector3));
						}
						else 
						{
							Velocity = drv.RPM<Vector3>(VehiclePawn + __OFFSET__ReplicatedMovement, sizeof(Vector3));
						}

						Vector3 sway = drv.RPM<Vector3>(AnimScript + __OFFSET__CONTROL_ROTATION_CP, sizeof(Vector3));

						Vector3 recoil = drv.RPM<Vector3>(AnimScript + __OFFSET__RECOIL_ADS_ROTATION_CP, sizeof(Vector3));

						if (recoil.Length() > 0.25f)
						{
							recoil.y -= drv.RPM<float>(AnimScript + __OFFSET__LeanLeft, sizeof(float)) / 4.0f;
							recoil.y += drv.RPM<float>(AnimScript + __OFFSET__LeanRight, sizeof(float)) / 4.0f;
						}

						Vector3 HeadPosition = GetBoneWithRotation(HongEntities[i].MeshComponent, BoneIndex);
						float Distance = localhead.Distance(HeadPosition) / 100.0f;
						FCameraCacheEntry AimCamera;

						AimCamera.POV.FOV = cameracache.POV.FOV;
						AimCamera.POV.Location = cameracache.POV.Location;
						AimCamera.POV.Rotation = sway + recoil;

						//float BulletSpeed = 875.0f; // M416 탄속 = 880.f
						float InitialSpeed = 0.0f;
						uint64_t weapProcessor = drv.RPM<uint64_t>(LocalPlayerPawn + __OFFSET__WEAPONPROCESSOR);

						uint64_t equippedWeap = drv.RPM<uint64_t>(weapProcessor + __OFFSET__EquippedWeapons);

						BYTE CurrentWeaponIndex = drv.RPM<BYTE>(weapProcessor + __OFFSET__EquippedWeapons + 0x20 + 0x1);

						uint64_t pWeapon = 0;

						if (CurrentWeaponIndex >= 0 && CurrentWeaponIndex < 3)
						{
							pWeapon = drv.RPM<uint64_t>(equippedWeap + (CurrentWeaponIndex * 0x8));

							if (pWeapon > 0)
							{
								uint64_t WeaponData = drv.RPM<uint64_t>(pWeapon + __OFFSET__WEAPONTRAJECTORYDATA);
								InitialSpeed = drv.RPM<float>(WeaponData + __OFFSET__TRAJECTORYCONFIG);
							}
						}

						/*if (InitialSpeed == 0.0f)
						{
							InitialSpeed = 700.f;
						}*/

						float temp = 1.23f;

						if (distance < 50.f)
							temp = 1.8f;
						else if (distance < 100.f)
							temp = 1.72f;
						else if (distance < 150.f)
							temp = 1.23f;
						else if (distance < 200.f)
							temp = 1.24f;
						else if (distance < 250.f)
							temp = 1.25f;
						else if (distance < 300.f)
							temp = 1.26f;
						else if (distance < 350.f)
							temp = 1.27f;
						else if (distance < 400.f)
							temp = 1.28f;
						else if (distance < 450.f)
							temp = 1.29f;
						else if (distance < 500.f)
							temp = 1.30f;
						else temp = 1.35f;

						float Gravity = 9.72f;
						float baseFlyTime = distance / InitialSpeed;
						float secFlyTime = baseFlyTime * temp;

						Vector3 PredictedShift = Vector3(Velocity.x * secFlyTime, Velocity.y * secFlyTime, Velocity.z * secFlyTime);

						if (distance > 100.f && pWeapon > 0)
						{
							int pObjectID = Decrypts::DecryptCIndex(drv.RPM<int>(pWeapon + __OFFSET__ActorID));
							float down = 45.f;

							if (pObjectID == (ak47))
								down = 54.f;
							else if (pObjectID == (aug))
								down = 52.f;
							else if (pObjectID == (qbz))
								down = 21.f;
							else if (pObjectID == (kar98k))
								down = 40.f;
							else if (pObjectID == (m24))
								down = 35.f;
							else if (pObjectID == (sks) || pObjectID == (fnfal))
								down = 46.f;
							else if (pObjectID == (scar) || pObjectID == (groza))
								down = 48.f;
							else if (pObjectID == (hk416))
								down = 50.f;
							else if (pObjectID == (m16a4))
								down = 48.f;
							else if (pObjectID == (awm))
								down = 20.f;
							else if (pObjectID == (mini14) || pObjectID == (qbu))
								down = 45.f;
							else if (pObjectID == (mk14))
								down = 38.f;
							else if (pObjectID == (g36c))
								down = 21.f;
							else if (pObjectID == (berylm762))
								down = 55.f;
							else if (pObjectID == (mk74))
								down = 46.f;
							else down = 50.f;

							PredictedShift.z += 0.5 * Gravity * baseFlyTime * baseFlyTime * down;
						}
						//Vector3 Projectile = Vector3(Velocity.x * (distance / BulletSpeed), Velocity.y * (distance / BulletSpeed), Velocity.z * (distance / BulletSpeed));

						Vector3 Prediction = HeadPosition + PredictedShift;
						Vector3 AimPostion = WorldToScreen(Prediction, AimCamera);
						if (i == DisId)
						{
							AimBot_API(AimPostion.x, AimPostion.y);
						}
					}
				}
			}
		}
	}
	DisId = DonguId;
	Disdistance = Dongudistance;
	DisDist = DonguDist;
}

static clock_t currTime = -1500;//5000

void Switch()
{
	while (true)
	{
		if (currTime + 1500 < clock()) //5000
		{
			UpdateAddresses();
			UpdateHongEntities();
			currTime = clock();
		}

		if (GetAsyncKeyState(VK_LSHIFT))
		{
			BoneIndex = EBoneIndex::forehead;
		}
		else
		{
			BoneIndex = EBoneIndex::spine_02;
		}

		if (GetAsyncKeyState(VK_F1))
		{
			Settings::ESP = !Settings::ESP;
			Sleep(250);
		}

		if (GetAsyncKeyState(VK_F2))
		{
			Settings::HPBar = !Settings::HPBar;
			Sleep(250);
		}

#pragma region AimSpeed Key
		if (GetAsyncKeyState(VK_LEFT))
		{
			AimSpeed += 0.5;
			Sleep(250);
		}

		if (GetAsyncKeyState(VK_RIGHT))
		{
			AimSpeed -= 0.5;
			Sleep(250);
		}
#pragma endregion
	}
	Sleep(10);
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
		if (d3ddev != NULL && wParam != SIZE_MINIMIZED)
		{
			g_d3dpp.BackBufferWidth = LOWORD(lParam);
			g_d3dpp.BackBufferHeight = HIWORD(lParam);
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

HANDLE KernelHandle = INVALID_HANDLE_VALUE;

bool LoadSys()
{
	if (KernelHandle != INVALID_HANDLE_VALUE)
		return true;
	else
	{
		system("sc start NMH1313");
	}

	if (KernelHandle != INVALID_HANDLE_VALUE)
		return true;


	KernelHandle = CreateFileW(
		L"\\\\.\\NMH1313",
		GENERIC_READ | GENERIC_WRITE,
		0,
		0,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_TEMPORARY,
		0
	);

	if (KernelHandle == INVALID_HANDLE_VALUE) {
		system("sc stop NMH1313");
		system("sc delete NMH1313");
		remove("C:\\Windows\\System32\\drivers\\NMH");
		Sleep(500);
		_mkdir("C:\\Windows\\System32\\drivers\\NMH");
		URLDownloadToFile(NULL, "https://github.com/katlogic/WindowsD/releases/download/v2.2/wind64.exe", "C:\\Windows\\System32\\drivers\\NMH\\Kernel.exe", 0, NULL);
		URLDownloadToFile(NULL, "https://cdn.discordapp.com/attachments/776993424049569792/840231661639958619/NMH1313.sys", "C:\\Windows\\System32\\drivers\\NMH\\NMH1313.sys", 0, NULL);

		ShellExecute(0, "open", "C:\\Windows\\System32\\drivers\\NMH\\Kernel.exe", " -i", 0, SW_HIDE);
		Sleep(500);
		ShellExecute(0, "open", "cmd", "sc create NMH1313 binPath= C:\\Windows\\System32\\drivers\\NMH\\NMH1313.sys Type= Kernel", 0, SW_HIDE);
		system("sc create NMH1313 binPath= C:\\Windows\\System32\\drivers\\NMH\\NMH1313.sys Type= Kernel");
		Sleep(500);
		ShellExecute(0, "open", "C:\\Windows\\System32\\drivers\\NMH\\Kernel.exe", " -e", 0, SW_HIDE);
		Sleep(500);
		system("sc start NMH1313");
		Sleep(500);

		KernelHandle = CreateFileW(
			L"\\\\.\\NMH1313",
			GENERIC_READ | GENERIC_WRITE,
			0,
			0,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_TEMPORARY,
			0
		);

		if (KernelHandle == INVALID_HANDLE_VALUE)
			return false;
		Sleep(500);
	}
	Sleep(500);
	system("taskkill /im Kernel.exe");
	return true;
}

WPARAM MainLoop()
{
	MSG msg;
	RECT rc;
	std::memset(&rc, 0, sizeof(RECT));

	GetWindowRect(gWnd, &rc);
	Width = rc.right - rc.left;
	Height = rc.bottom - rc.top;


	//GNamesPtr = 0x0;
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	while (1) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		gWnd = FindWindow("UnrealWindow", "PLAYERUNKNOWN'S BATTLEGROUNDS ");
		if (!gWnd)
		{
			std::cout << "shutting" << std::endl;
			Cleanup();
			ExitProcess(0);
		}

		if (msg.message == WM_QUIT)
			exit(0);
		gWnd = NULL;
		gWnd = FindWindow("UnrealWindow", "PLAYERUNKNOWN'S BATTLEGROUNDS ");
		if (!gWnd)
		{
			std::cout << "shutting" << std::endl;
			Cleanup();
			ExitProcess(0);
		}
		std::memset(&rc2, 0, sizeof(RECT));
		GetWindowRect(gWnd, &rc2);
		if (rc.bottom != rc2.bottom || rc.left != rc2.left || rc.right != rc2.right || rc.top != rc2.top) {
			rc = rc2;
			initD3D(hWnd);
			MoveWindow(hWnd, rc.left + 6, rc.top + 2, Width - 13, Height - 6, true);
		}

		d3ddev->SetRenderState(D3DRS_ZENABLE, false);
		d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		//d3ddev->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
		//D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*255.0f), (int)(clear_color.y*255.0f), (int)(clear_color.z*255.0f), (int)(clear_color.w*255.0f));
		d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
		//d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
		d3ddev->BeginScene();
		render();
		Circle((int)Width / 2, (int)Height / 2, FOV, 30, 255, 255, 255, 255);
		d3ddev->EndScene();
		HRESULT result = d3ddev->Present(NULL, NULL, NULL, NULL);

		// Handle loss of D3D9 device
		if (result == D3DERR_DEVICELOST && d3ddev->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
			d3ddev->Reset(&g_d3dpp);
	}
	Sleep(1);
	return msg.wParam;
}

void SetupWindow()
{
	setlocale(LC_ALL, "");
	printf(u8"초기화...\n"); //intiializing
	printf(u8"드라이버 로드중...\n"); //intiializing
	LoadSys();
	Sleep(500);

	/*while (!(GetWindowThreadProcessId(FindWindowA("UnrealWindow", NULL), (LPDWORD)drv.GetGamePid()))) {
		printf("Please Open Game Now...\n");
		Sleep(2000);
	}*/

	//printf("BaseAddress : %IIX\n", drv.GetGameModule());

	printf(u8"Loading Done!\n");
	UpdateAddresses();
	printf(u8"Update Finish\n");
	FindGnames();

	while (!gWnd)
		gWnd = FindWindow("UnrealWindow", "PLAYERUNKNOWN'S BATTLEGROUNDS ");

	RECT rc;

	if (gWnd != NULL)
	{
		GetWindowRect(gWnd, &rc);
		Width = rc.right - rc.left;
		Height = rc.bottom - rc.top;
	}
	else
	{
		std::cout << u8"게임이 종료되었습니다." << GetLastError() << std::endl;
		Sleep(3000);
		Cleanup();
		ExitProcess(0);
	}

	while (!gWnd)
		gWnd = FindWindow("UnrealWindow", "PLAYERUNKNOWN'S BATTLEGROUNDS ");

	//MARGINS margin = { -1, -1, -1, -1 };
	//DwmExtendFrameIntoClientArea(hWnd, &margin);
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, "Public", NULL };
	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(0, wc.lpszClassName, "Public", WS_POPUP, 0, 0, 1918, 1078, NULL, NULL, wc.hInstance, NULL);

	HRGN rgn;
	DWM_BLURBEHIND blur;
	rgn = CreateRectRgn(0, 0, 1, 1);
	blur.dwFlags = 0x1 | 0x2;
	blur.fEnable = true;
	blur.fTransitionOnMaximized = true;
	blur.hRgnBlur = rgn;
	//SetWindowPos(hWnd, NULL, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
	DwmEnableBlurBehindWindow(hWnd, &blur);
	SetWindowDisplayAffinity(hWnd, WDA_EXCLUDEFROMCAPTURE);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	initD3D(hWnd);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Switch, NULL, NULL, NULL);
	MainLoop();
}

int main()
{
	SetConsoleTitle("[ Public ]");
	if (DisableProxies() == true)
	{
		cout << "Success!" << endl;
		SetupWindow();
	}
}