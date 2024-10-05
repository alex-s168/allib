#ifndef TCP_CLIENT_H 
#define TCP_CLIENT_H

#include <stdint.h>
#include <stddef.h>
#include "../kallok/kallok.h"

typedef struct TcpClient_Con TcpClient_Con;

static Ally TcpClient_getAlly(TcpClient_Con* con)
{
    extern size_t TcpClient_Con_allyOff;
    return *(Ally*)(((char*)con) + TcpClient_Con_allyOff);
}

TcpClient_Con * TcpClient_open(Ally ally, const char * ip, uint16_t port);
void TcpClient_send(TcpClient_Con * con, const uint8_t * buf, size_t len);
uint8_t * TcpClient_recv(TcpClient_Con * con, size_t * len_out);
void TcpClient_close(TcpClient_Con * con);

#endif 
