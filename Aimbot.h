#pragma once
#include "include.h"

void AimBot_API(float X, float Y)
{
	float ScreenCenterX = (Width / 2);
	float ScreenCenterY = (Height / 2);
	float TargetX = 0;
	float TargetY = 0;

	if (X != 0)
	{
		if (X > ScreenCenterX)
		{
			TargetX = -(ScreenCenterX - X);
			TargetX /= AimSpeed;
			if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
		}

		if (X < ScreenCenterX)
		{
			TargetX = X - ScreenCenterX;
			TargetX /= AimSpeed;
			if (TargetX + ScreenCenterX < 0) TargetX = 0;
		}
	}

	if (Y != 0)
	{
		if (Y > ScreenCenterY)
		{
			TargetY = -(ScreenCenterY - Y);
			TargetY /= AimSpeed;
			if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
		}

		if (Y < ScreenCenterY)
		{
			TargetY = Y - ScreenCenterY;
			TargetY /= AimSpeed;
			if (TargetY + ScreenCenterY < 0) TargetY = 0;
		}
	}

	INPUT InputMouse = { 0 };
	InputMouse.type = INPUT_MOUSE;
	InputMouse.mi.dwFlags = MOUSEEVENTF_MOVE;
	if (abs((DWORD)TargetX * AimSpeed) > 1)
		InputMouse.mi.dx = (DWORD)TargetX;
	if (abs((DWORD)TargetY * AimSpeed) > 1)
		InputMouse.mi.dy = (DWORD)TargetY;

	SendInput(1, &InputMouse, sizeof(INPUT));

	return;
}