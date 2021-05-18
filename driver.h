#pragma once
#include <Windows.h>
#include <winioctl.h>

//driver r/w struct
typedef struct _IOBUFFER
{
	unsigned long long UserBufferAdress;
	unsigned long long GameAddressOffset;
	unsigned long long Size;
	unsigned long      UserPID;
	unsigned long      GamePID;
} IOBUFFER, * PIOBUFFER;


#define IOCTL_IO_READ		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x8001, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IOCTL_IO_WRITE		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x8012, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IOCTL_IO_PID		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x8023, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IOCTL_IO_MOUDLE		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x8034, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IOCTL_IO_IMAGESIZE  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x8045, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)


class driver
{
public:
	driver();
	~driver();

	unsigned long long GetGameModule();
	unsigned long GetGamePid();
	unsigned long long GetImageSize();

	template <typename T> inline T RPM(unsigned long long Addr);
	template <typename T> inline T RPM(unsigned long long Addr, unsigned long Size);
	template <typename T> inline bool RPM(unsigned long long Addr, T OuterBuffer, unsigned long Size);
	template <typename T> inline bool WPM(unsigned long long Addr, T value);
	template <typename T> inline bool WPM(unsigned long long Addr, T value, unsigned long Size);

protected:
	void* hDriver;

};

template<typename T>
inline T driver::RPM(unsigned long long Addr)
{
	T readBuffer = 0;

	unsigned long Bytes;
	_IOBUFFER ReadRequest
	{
		(unsigned long long) & readBuffer,
		(unsigned long long)Addr,
		sizeof(T),
		(unsigned long)GetCurrentProcessId(),
		0
	};
	DeviceIoControl(
		hDriver,
		IOCTL_IO_READ,
		&ReadRequest,
		sizeof(ReadRequest),
		&ReadRequest,
		sizeof(ReadRequest),
		&Bytes,
		0);
	return (T)readBuffer;
}

template<typename T>
inline T driver::RPM(unsigned long long Addr, unsigned long Size)
{
	T readBuffer = {};

	unsigned long Bytes;
	_IOBUFFER ReadRequest
	{
		(unsigned long long) & readBuffer,
		(unsigned long long)Addr,
		Size,
		(unsigned long)GetCurrentProcessId(),
		0
	};
	DeviceIoControl(
		hDriver,
		IOCTL_IO_READ,
		&ReadRequest,
		sizeof(ReadRequest),
		&ReadRequest,
		sizeof(ReadRequest),
		&Bytes,
		0);
	return (T)readBuffer;
}

template<typename T>
inline bool driver::RPM(unsigned long long Addr, T OuterBuffer, unsigned long Size)
{
	unsigned long Bytes;
	_IOBUFFER ReadRequest
	{
		(unsigned long long)OuterBuffer,
		(unsigned long long)Addr,
		Size,
		(unsigned long)GetCurrentProcessId(),
		0
	};
	return DeviceIoControl(
		hDriver,
		IOCTL_IO_READ,
		&ReadRequest,
		sizeof(ReadRequest),
		&ReadRequest,
		sizeof(ReadRequest),
		&Bytes,
		0);
}

template<typename T>
inline bool driver::WPM(unsigned long long Addr, T value)
{
	T writeBuffer = value;
	unsigned long Bytes;
	_IOBUFFER  WriteRequest
	{
		(unsigned long long) & writeBuffer,
		(unsigned long long)Addr,
		sizeof(T),
		(unsigned long)GetCurrentProcessId(),
		0
	};
	return DeviceIoControl(
		hDriver,
		IOCTL_IO_WRITE,
		&WriteRequest,
		sizeof(WriteRequest),
		&WriteRequest,
		sizeof(WriteRequest),
		&Bytes,
		0);
}

template<typename T>
inline bool driver::WPM(unsigned long long Addr, T value, unsigned long Size)
{
	unsigned long Bytes;
	_IOBUFFER  WriteRequest
	{
		(unsigned long long)value,
		(unsigned long long)Addr,
		Size,
		(unsigned long)GetCurrentProcessId(),
		0
	};

	return DeviceIoControl(
		hDriver,
		IOCTL_IO_WRITE,
		&WriteRequest,
		sizeof(WriteRequest),
		&WriteRequest,
		sizeof(WriteRequest),
		&Bytes,
		0);
}

extern driver drv;