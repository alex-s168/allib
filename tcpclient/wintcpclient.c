#ifdef _WIN32

#include "tcpclient.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

struct TcpClient_Con {
    SOCKET ConnectSocket;
};


TcpClient_Con * TcpClient_open(const char * ip, uint16_t port) {
    TcpClient_Con * con = malloc(sizeof(TcpClient_Con));
    if (con == NULL) return NULL;

    con->ConnectSocket = INVALID_SOCKET;

    WSADATA wsaData;

    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        free(con);
        return NULL;
    }

    struct addrinfo hints;
    struct addrinfo *result; 

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    char portstr[32];
    sprintf(portstr, "%u", port);

    // Resolve the server address and port
    iResult = getaddrinfo(ip, portstr, &hints, &result);
    if (iResult != 0) {
        WSACleanup();
        free(con);
        return NULL;
    }

    // Attempt to connect to an address until one succeeds
    for(struct addrinfo *ptr = result; ptr; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        con->ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (con->ConnectSocket == INVALID_SOCKET) {
            WSACleanup();
            free(con);
            return NULL;;
        }

        // Connect to server.
        iResult = connect(con->ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(con->ConnectSocket);
            con->ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (con->ConnectSocket == INVALID_SOCKET) {
        WSACleanup();
        free(con);
        return NULL;
    }

    return con;
}

void TcpClient_send(TcpClient_Con * con, const uint8_t * buf, size_t len) {
    assert(con);

    send(con->ConnectSocket, (const char *) buf, (int) len, 0);
}

uint8_t * TcpClient_recv(TcpClient_Con * con, size_t * len_out) {
    assert(con);

    size_t buf_len = 0;
    uint8_t * buf = NULL;

    int result;

    // try read until buf enough 
    do {
        buf_len += 256;
        buf = realloc(buf, buf_len);

        result = recv(con->ConnectSocket, (char*) buf, buf_len, 0);
    } while (result == SOCKET_ERROR && WSAGetLastError() == WSAEMSGSIZE);

    if (result == SOCKET_ERROR) {
        if (buf) free(buf);
        return NULL;
    }

    buf_len -= 256;
    buf_len +=  result; 

    if (len_out) *len_out = buf_len;

    return buf;
}

void TcpClient_close(TcpClient_Con * con) {
    assert(con);
    shutdown(con->ConnectSocket, SD_SEND);
    closesocket(con->ConnectSocket);
    WSACleanup();
    free(con);
}

#endif 
