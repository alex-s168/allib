#ifndef _WIN32

#include "tcpclient.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>

struct TcpClient_Con {
    Ally ally;
    int sockfd;
    struct sockaddr_in servaddr;
};

size_t TcpClient_Con_allyOff = offsetof(TcpClient_Con, ally);

TcpClient_Con * TcpClient_open(Ally ally, const char * ip, uint16_t port)
{
    TcpClient_Con* con = yalloc(ally, sizeof(struct TcpClient_Con));
    if (con == NULL) return NULL;
    memset(con, 0, sizeof(struct TcpClient_Con));
    con->ally = ally;

    con->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (con->sockfd == -1) {
        yfree(ally, con, sizeof(struct TcpClient_Con));
        return NULL;
    }

    con->servaddr.sin_family = AF_INET;
    con->servaddr.sin_addr.s_addr = inet_addr(ip);
    con->servaddr.sin_port = htons(port);

    if ((bind(con->sockfd, (struct sockaddr *)&con->servaddr, sizeof(con->servaddr))))
    {
        close(con->sockfd);
        yfree(ally, con, sizeof(struct TcpClient_Con));
        return NULL;
    }

    return con;
}

void TcpClient_send(TcpClient_Con * con, const uint8_t * buf, size_t len)
{
    write(con->sockfd, buf, len);
}

uint8_t * TcpClient_recv(TcpClient_Con * con, size_t * len_out)
{
    size_t count;
    ioctl(con->sockfd, FIONREAD, &count);

    uint8_t* buf = yalloc(con->ally, count);
    if (buf == NULL) return NULL;
    read(con->sockfd, buf, count);

    if (len_out) *len_out = count;

    return buf;
}

void TcpClient_close(TcpClient_Con * con)
{
    close(con->sockfd);
    yfree(con->ally, con, sizeof(struct TcpClient_Con));
}

#endif // !defined(_WIN32)
