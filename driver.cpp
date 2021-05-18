#include "driver.h"
#include <stdio.h>

driver::driver()
{
	hDriver = CreateFileA("\\\\.\\NMH1313", GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	if (hDriver == INVALID_HANDLE_VALUE)
	{
		printf("[!] Driver Install Failed\n");
	}
	else
	{
		printf("[!] Driver Install Successed\n");
	}
}


driver::~driver()
{
	if (hDriver != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hDriver);
		printf("[!] Driver UnInstall Successed\n");
	}
}

unsigned long long driver::GetGameModule()
{
	if (hDriver == INVALID_HANDLE_VALUE) return 0;
	unsigned long long readBuffer;
	unsigned long Bytes;
	_IOBUFFER ReadRequest
	{
		(unsigned long long) & readBuffer,
		(unsigned long long)0x0,
		sizeof(unsigned long long),
		(unsigned long)GetCurrentProcessId(),
		0
	};
	int stat = DeviceIoControl(
		hDriver,
		IOCTL_IO_MOUDLE,
		&ReadRequest,
		sizeof(ReadRequest),
		&ReadRequest,
		sizeof(ReadRequest),
		&Bytes,
		0);
	return stat ? readBuffer : 0;
}

unsigned long driver::GetGamePid()
{
	if (hDriver == INVALID_HANDLE_VALUE) return 0;
	unsigned long readBuffer;
	unsigned long Bytes;
	_IOBUFFER ReadRequest
	{
		(unsigned long long) & readBuffer,
		(unsigned long long)0x0,
		sizeof(unsigned long long),
		(unsigned long)GetCurrentProcessId(),
		0
	};
	int stat = DeviceIoControl(
		hDriver,
		IOCTL_IO_PID,
		&ReadRequest,
		sizeof(ReadRequest),
		&ReadRequest,
		sizeof(ReadRequest),
		&Bytes,
		0);
	return stat ? readBuffer : 0;
}

unsigned long long driver::GetImageSize()
{
	if (hDriver == INVALID_HANDLE_VALUE) return 0;
	unsigned long long readBuffer;
	unsigned long Bytes;
	_IOBUFFER ReadRequest
	{
		(unsigned long long) & readBuffer,
		(unsigned long long)0x0,
		sizeof(unsigned long long),
		(unsigned long)GetCurrentProcessId(),
		0
	};
	int stat = DeviceIoControl(
		hDriver,
		IOCTL_IO_IMAGESIZE,
		&ReadRequest,
		sizeof(ReadRequest),
		&ReadRequest,
		sizeof(ReadRequest),
		&Bytes,
		0);
	return stat ? readBuffer : 0;
}

driver drv;