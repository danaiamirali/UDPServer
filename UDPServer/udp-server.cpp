/*
 * Copyright 2017 - Soroush Rabiei <soroush@ametisco.ir>
 * This file is part of libcpnet.
 *
 * libcpnet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libcpnet is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libcpnet.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "tests.h"
#ifdef __linux__
#include <unistd.h>
#endif /* __linux__ */
#include "common.h"
#include <cpnet-network.h>

#ifdef _WIN32
DWORD WINAPI tst_udp_server(LPVOID)
#else
int main(int argc, char* argv[])
#endif
{
    /* Initialize networking API (if any needed) */
    cpnet_init();
    /* Start a server */
    socket_t socket = cpnet_socket(SOCK_DGRAM);
    /* Bind on port UCP_PORT */
    uint16_t port = UDP_PORT;
    if (cpnet_bind(socket, NULL, &port) != 0)
    {
        printf("System error description:\n%s\n", cpnet_last_error());
        return -1;
    }
    int backlog{};
    int clientCount = 0;

    std::cout << "=> Looking for clients..." << std::endl;

    cpnet_listen(socket, backlog);

    socket_t server = cpnet_accept(socket, NULL, &port);

    if (server < 0)
    {
        std::cout << "Error on accepting connection." << std::endl;
    }

    const int bufferSize = 1024;
    char buffer[bufferSize];
    bool isExit = false;
    char peer[46];
    uint16_t pport = 0;

    // Enter a loop
    while (true)
    {
        ZeroMemory(buffer, bufferSize);

        uint16_t pport = 0;
        ssize_t r = cpnet_read_packet(socket, buffer, 1024, peer, &pport);
        if (r < 0) {
            printf("Unable to read from UDP socket!");
            return -1;
        }
        if (r == 0) {
            printf("Remote client stopped\n");
            return 0;
        }
        cpnet_write_packet(socket, buffer, r, peer, pport);
        // std::cout << "\nServer received: " << buffer;
    }

    cpnet_close(socket);

    cpnet_clean();
    return 0;
}

