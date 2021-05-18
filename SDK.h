#pragma once
#include "include.h"
#include "driver.h"
/* XeDecrypt */
#include "XeDecrypt.h"
/* XeDecrypt */
#include "Vector.h"
/* Aimbot */
int AimTarget = EBoneIndex::forehead;
float AimSpeed = 2.5;
int FOV;
int BoneIndex;
float DisId = -1;
float DisDist = 99999;
float Disdistance = 99999;
/* Aimbot */

/* D3D */
ID3DXLine* d3dLine;
LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddev;
LPD3DXFONT pFont;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};
RECT rc2;
/* D3D */

/* HWND */
HWND hWnd;
HWND gWnd;
/* HWND */

/* String */
std::string ClassName;
std::string WindowName;
/* String */

/* FCameraCacheEntry */
FCameraCacheEntry cameracache = { NULL };
/* FCameraCacheEntry */

DWORD_PTR pUWorld = 0;
DWORD_PTR pLocalPlayer = 0;
DWORD_PTR pPersistentLevel = 0;
DWORD_PTR pGameInstance = 0;
DWORD_PTR pLocalPlayerArray = 0;
DWORD_PTR pPlayerController = 0;
DWORD_PTR AnimScript = 0;

uint64_t ActorsArray;
uint64_t Actor;
int ActorCount;
int SpectateCount;
uint64_t LocalPlayerPawn;
uint64_t LocalMesh;

/* Bone */
std::list<int> upper_part = { EBoneIndex::neck_01, EBoneIndex::Head, EBoneIndex::forehead };
std::list<int> right_arm = { EBoneIndex::neck_01, EBoneIndex::upperarm_r, EBoneIndex::lowerarm_r, EBoneIndex::hand_r };
std::list<int> left_arm = { EBoneIndex::neck_01, EBoneIndex::upperarm_l, EBoneIndex::lowerarm_l, EBoneIndex::hand_l };
std::list<int> spine = { EBoneIndex::neck_01, EBoneIndex::spine_01, EBoneIndex::spine_02, EBoneIndex::pelvis };
std::list<int> lower_right = { EBoneIndex::pelvis, EBoneIndex::thigh_r, EBoneIndex::calf_r, EBoneIndex::foot_r };
std::list<int> lower_left = { EBoneIndex::pelvis, EBoneIndex::thigh_l, EBoneIndex::calf_l, EBoneIndex::foot_l };
std::list<std::list<int>> skeleton = { upper_part, right_arm, left_arm, spine, lower_right, lower_left };
/* Bone */

FTransform GetBoneIndex(DWORD_PTR mesh, int index)
{
	DWORD_PTR bonearray = drv.RPM<DWORD_PTR>(mesh + __OFFSET__BoneArray);

	return drv.RPM<FTransform>(bonearray + (index * 0x30), sizeof(FTransform));
}

Vector3 GetBoneWithRotation(DWORD_PTR mesh, int id)
{
	FTransform bone = GetBoneIndex(mesh, id);

	FTransform ComponentToWorld = drv.RPM<FTransform>(mesh + __OFFSET__ComponentToWorld, sizeof(FTransform));
	D3DMATRIX Matrix;
	Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());
	return Vector3(Matrix._41, Matrix._42, Matrix._43);
}

int getLastRenderTime(UINT64 actor)
{
	return drv.RPM<int>(drv.RPM<uint64_t>(actor + __OFFSET__ActorMesh) + __OFFSET__LastRenderTime);
}

FCameraCacheEntry GetCameraCache()
{
	FCameraCacheEntry cce;
	auto player_camera_manager = drv.RPM<DWORD_PTR>(pPlayerController + __OFFSET__CameraManager);
	cce.POV.Rotation = drv.RPM<Vector3>(player_camera_manager + __OFFSET__CameraCache_Rot, sizeof(Vector3));
	cce.POV.Location = drv.RPM<Vector3>(player_camera_manager + __OFFSET__CameraCache_Loc, sizeof(Vector3));
	cce.POV.FOV = drv.RPM<float>(player_camera_manager + __OFFSET__CameraCache_FOV, sizeof(float));
	return cce;
}

void DrawLine2(float x, float y, float xx, float yy, D3DCOLOR color, float ww)
{
	D3DXVECTOR2 dLine[2];

	d3dLine->SetWidth(ww);

	dLine[0].x = x;
	dLine[0].y = y;

	dLine[1].x = xx;
	dLine[1].y = yy;

	d3dLine->Draw(dLine, 2, color);
}

void DrawSkeleton(DWORD_PTR mesh, bool IsVisible)
{
	Vector3 neckpos = GetBoneWithRotation(mesh, EBoneIndex::neck_01);
	Vector3 pelvispos = GetBoneWithRotation(mesh, EBoneIndex::pelvis);
	Vector3 previous(0, 0, 0);
	Vector3 current, p1, c1;
	for (auto a : skeleton)
	{
		previous = Vector3(0, 0, 0);
		for (int bone : a)
		{
			current = bone == EBoneIndex::neck_01 ? neckpos : (bone == EBoneIndex::pelvis ? pelvispos : GetBoneWithRotation(mesh, bone));
			if (previous.x == 0.f)
			{
				previous = current;
				continue;
			}
			p1 = WorldToScreen(previous, GetCameraCache());
			c1 = WorldToScreen(current, GetCameraCache());

			DrawLine2(p1.x, p1.y, c1.x, c1.y, IsVisible ? D3DCOLOR_RGBA(0, 255, 0, 255) : D3DCOLOR_RGBA(255, 0, 0, 255), 1.2f);

			previous = current;
		}
	}
}

DWORD_PTR DecAddr;

void initDecrypt()
{
	DWORD dwOld = NULL;
	VirtualProtect(fnShieldDecrypt, 0x200, PAGE_EXECUTE_READWRITE, &dwOld);
	DecAddr = drv.RPM<DWORD_PTR>(drv.GetGameModule() + __OFFSET__Decrypt, sizeof(DWORD_PTR));
	drv.RPM(DecAddr, &fnShieldDecrypt, 0x200);
}

DWORD64 Decrypt(__int64 value)
{
	DWORD64 fnShield = (DWORD64)fnShieldDecrypt + 0x7;
	DWORD64 Shield_start = *(PDWORD)(fnShieldDecrypt + 0x3) + DecAddr + 0x7;
	return Shield_Decrypt(0, value, fnShield, Shield_start);
}

void UpdateAddresses()
{
	initDecrypt();
	pUWorld = Decrypt(drv.RPM<DWORD_PTR>(drv.GetGameModule() + __OFFSET__UWorld));
	pPersistentLevel = Decrypt(drv.RPM<DWORD_PTR>(pUWorld + __OFFSET__Level));
	pLocalPlayer = drv.RPM<DWORD_PTR>(drv.GetGameModule() + __OFFSET__LocalPlayer);
	pPlayerController = Decrypt(drv.RPM<DWORD_PTR>(pLocalPlayer + __OFFSET__PlayerController));
	ActorsArray = Decrypt(drv.RPM<DWORD_PTR>(pPersistentLevel + __OFFSET__Actors));
	Actor = drv.RPM<UINT64>(ActorsArray + 0x0); // ActorPointer
	ActorCount = drv.RPM<int>(ActorsArray + 0x8); // ActorCount
	LocalPlayerPawn = Decrypt(drv.RPM<uint64_t>(pPlayerController + __OFFSET__Pawn));
	SpectateCount = drv.RPM<int>(LocalPlayerPawn + __OFFSET__SpectateCount);
	LocalMesh = drv.RPM<DWORD_PTR>(LocalPlayerPawn + __OFFSET__ActorMesh);
	AnimScript = drv.RPM<DWORD_PTR>(LocalMesh + __OFFSET__UANIMINSTANCE);

}

Vector3 GetLocalPlayerPos()
{
	return cameracache.POV.Location;
}

BoneArrayStruct GetBoneArray(DWORD_PTR mesh) {
	DWORD_PTR bonearray = drv.RPM<DWORD_PTR>(mesh + __OFFSET__BoneArray);
	return  drv.RPM<BoneArrayStruct>(bonearray, sizeof(BoneArrayStruct));
}

Vector3 GetBoneWithRotationEx(BoneArrayStruct& bas, int id, FTransform& ComponentToWorld) {

	FTransform bone = bas.BoneArray[id];
	D3DMATRIX Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());
	return Vector3(Matrix._41, Matrix._42, Matrix._43);
}

int GetEntityID(DWORD_PTR entity)
{
	return Decrypts::DecryptCIndex(drv.RPM<int>(entity + __OFFSET__ActorID));
}

float GetActorHealth(DWORD_PTR pAActor)
{
	return drv.RPM<float>(pAActor + __OFFSET__Health);
}

float GetActorGroggyHealth(DWORD_PTR pAActor)
{
	return drv.RPM<float>(pAActor + __OFFSET__GroggyHealth);
}

Vector3 GetActorPos(DWORD_PTR entity)
{
	Vector3 pos;

	auto rootcomp = Decrypt(drv.RPM<DWORD_PTR>(entity + __OFFSET__RootComponent));
	pos = drv.RPM<Vector3>(rootcomp + __OFFSET_ROOTCOMPONENT_LOCATION, sizeof(Vector3));

	return pos;
}