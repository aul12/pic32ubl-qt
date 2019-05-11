// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QDebug>
#include <QThread>
#include <QTime>
#include <QTimer>
#include <QFileDialog>
#include <QGenericArgument>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <set>
#include <iostream>

#define BYTE uint8_t
#define SHORT int16_t
#define USHORT uint16_t
#define WORD uint16_t
#define INT int32_t
#define LONG int32_t
#define UINT uint32_t
#define ULONG uint32_t
#define DWORD uint32_t
#define INT_PTR int64_t
#define LONG_PTR int64_t
#define LPARAM int64_t
#define UINT_PTR uint64_t
#define WPARAM_PTR uint64_t
#define LONGLONG int64_t
#define ULONGLONG uint64_t
#define QWORD uint64_t
#define BOOL bool
#define CHAR char
#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define VOID void
typedef void *LPVOID;
typedef void *PVOID;
typedef PVOID HANDLE;
typedef CHAR *LPSTR;
typedef wchar_t WCHAR;
typedef WCHAR *LPWSTR;
#ifdef UNICODE
 typedef LPWSTR LPTSTR;
#else
 typedef LPSTR LPTSTR;
#endif
typedef uint64_t WPARAM;
typedef int64_t LRESULT;
#define Sleep(x) usleep(x * 1000)

class Timer
{
public:
    static QTime timer;
    static bool initialized;
    static int elapsed()
    {
        if (!initialized)
        {
            timer.start();
            initialized = true;
        }
        return timer.elapsed();
    }
};

#define GetTickCount Timer::elapsed
