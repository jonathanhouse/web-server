#include "TcpConnection.h"
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <regex.h>

TcpConnection::TcpConnection(int port, std::string ip_address) : 
m_port(port), m_ip_address(ip_address), m_socket(), m_socket_address() {
    start_server();
    m_socket_address.sin_family = AF_INET;
    m_socket_address.sin_port = htons(m_port);
    m_socket_address.sin_addr.s_addr = INADDR_ANY;//inet_addr(m_ip_address.c_str()); // 
}

TcpConnection::~TcpConnection(){
    close_server();
}

int TcpConnection::start_server(){

    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(m_socket < 0){
        std::cout << "Failed to create socket." << std::endl;
        exit(1);
        return 1;
    }

    if(bind(m_socket, (sockaddr *)&m_socket_address, sizeof(m_socket_address)) < 0){
        std::cout << "Failed to bind socket to address." << std::endl;
        exit(1);
        return 1;
    } 

    return 0;
}

void TcpConnection::close_server(){
    close(m_socket);
    exit(0);
}

void TcpConnection::start_listen(){

    if(listen(m_socket, 10) < 0){
        std::cout << "Failed to listen." << std::endl;
        exit(1);
    }

    std::cout << "Listening on address: " << inet_ntoa(m_socket_address.sin_addr) << ", port: " << ntohs(m_socket_address.sin_port) << std::endl;

}

void* handle_client(void* client_socket){
    int client_fd = *((int *)client_fd);
    char* buffer = new char[BUFFER_SIZE];

    ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
    if(bytes_received > 0){

        regex_t regex;
        regcomp(&regex, "^GET /([^ ]*) HTTP/1", REG_EXTENDED); //fill regex obj with pattern for comparison
        regmatch_t matches[2];

        if(regexec(&regex, buffer, 2, matches, 0) == 0){

            // parses filename from HTTP request and decodes URL 
            buffer[matches[1].rm_eo] = '\0'; // rm_eo is the offset in the buffer to the end of the substring, at \0 so it's interpretted as string 
            const char *url_encoded_file_name = buffer + matches[1].rm_so; // points to the start of the substring match 
            std::string file_name = url_encoded_file_name;//url_decode(url_encoded_file_name);

            std::cout << "File name: " << file_name << std::endl; 
            // std::string file_ext;
            // file_ext = get_file_extension(file_name);

            // char* response = new char[BUFFER_SIZE * 2];
            // size_t response_len; 
            // build_response(file_name, file_ext, response, &response_len);

            // send(client_fd, response, response_len, 0);

            // delete[] response;
        }
        regfree(&regex);

    }
    close(client_fd);
    delete[] buffer;
    delete[] (int *)client_socket;
    return NULL;
}

void TcpConnection::accept_connections(){

    while(1){

        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        int* client_socket = new int;
        *client_socket = accept(m_socket, (sockaddr *)&client_addr, &client_addr_len);
        std::cout << "Accepted a connection." << std::endl;

        if(*client_socket < 0){
            std::cout << "Failed to accept socket from address: " << inet_ntoa(client_addr.sin_addr) << ", port: " << ntohs(client_addr.sin_port) << std::endl;
        }

        // std::thread thread_id(handle_client, (void *)client_socket);
        // thread_id.detach();
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *)client_socket);
        pthread_detach(thread_id);
    }
}

