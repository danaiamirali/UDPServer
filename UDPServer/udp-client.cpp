/*
 * Copyright 2017 - FDE Co., <rabiei@fdeshragh.com>
 * This file is part of libfde_network.
 *
 * libfde_network is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libfde_network is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libfde_network.  If not, see <http://www.gnu.org/licenses/>.
*/


//open a UDP socket to send something to something
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "tests.h"
#ifdef __linux__
#include <unistd.h>
#endif // __linux__
#include "common.h"
#include <cpnet-network.h>


#ifdef _WIN32
DWORD WINAPI tst_udp_client(LPVOID)
#else
int main(int argc, char* argv[])
#endif
{
    /* Give time to server to bind (or else will be deadlock) */
#ifdef _WIN32
    Sleep(2000);
#else
    sleep(2);
#endif
    srand((unsigned int)time(NULL));
    /* Initialize networking API */
    cpnet_init();
    /* Start a client */
    socket_t socket = cpnet_socket(SOCK_DGRAM);

    uint16_t port = UDP_PORT;
    const char* ipaddress = "127.0.0.1";

    if (cpnet_connect(socket, ipaddress, port) != 0)
    {
        fprintf(stderr, "Unable to connect: %s", cpnet_last_error());
        exit(-1);
    }

    std::cout << "=> Connected to port " << port << " successfully." << std::endl;
    std::cout << "=> Type # to terminate connection." << std::endl;

    const int bufferSize = 1024;
    char buffer[bufferSize];
    bool isExit = false;

    do
    {
        std::string buf;
        std::cout << "\nClient: ";
        std::getline(std::cin, buf);

        std::strcpy(buffer, buf.c_str());

        ssize_t io_size = cpnet_write_packet(socket, buffer, bufferSize, "127.0.0.1", UDP_PORT);
        if (io_size < 0)
        {
            fprintf(stderr, "Unable to write: %s", cpnet_last_error());
            exit(-1);
        }
        if (*buffer == '#')
        {
            *buffer = 42;
            isExit = true;
        }

        std::cout << "Server: ";
        io_size = cpnet_read(socket, buffer, 1024);
        if (io_size <= 0)
        {
            fprintf(stderr, "Unable to read: %s", cpnet_last_error());
            exit(-1);
        }
        std::cout << buffer << " ";
        if (*buffer == '#')
        {
            *buffer = 42;
            isExit = true;
        }
    } while (!isExit);

    std::cout << "\nConnection terminated." << std::endl;

    cpnet_close(socket);

    /* Finalize networking API */
    cpnet_clean();
    return 0;
}

