#pragma once

#define _WINSOCKAPI_  /* stops windows.h including winsock.h */
#include <Windows.h>

int full_test();
DWORD WINAPI tst_tcp_client(LPVOID);
DWORD WINAPI tst_tcp_server(LPVOID);
DWORD WINAPI tst_udp_client(LPVOID);
DWORD WINAPI tst_udp_server(LPVOID);