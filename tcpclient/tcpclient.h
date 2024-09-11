#ifndef TCP_CLIENT_H 
#define TCP_CLIENT_H

#include <stdint.h>
#include <stddef.h>

typedef struct TcpClient_Con TcpClient_Con;

TcpClient_Con * TcpClient_open(const char * ip, uint16_t port);
void TcpClient_send(TcpClient_Con * con, const uint8_t * buf, size_t len);
uint8_t * TcpClient_recv(TcpClient_Con * con, size_t * len_out);
void TcpClient_close(TcpClient_Con * con);

#endif 
