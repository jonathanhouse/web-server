#ifndef TCPCONNECTION_HPP
#define TCPCONNECTION_HPP

#include <string>
#include <arpa/inet.h>
#define BUFFER_SIZE 10000

class TcpConnection {

public: 

TcpConnection(uint16_t port, std::string ip_address);
~TcpConnection();
void start_listen();
void accept_connections();

private: 

uint16_t m_port;
std::string m_ip_address;
int m_socket; 
struct sockaddr_in m_socket_address;

int start_server();
void close_server();

};


#endif 