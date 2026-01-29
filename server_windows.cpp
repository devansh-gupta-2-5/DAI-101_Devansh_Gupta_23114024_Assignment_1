// server_win.cpp
// Code for the Windows Lab PC
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32.lib") // Link with Winsock library

int main() {
    // 1. Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }

    int port = 8081; // We will run on port 8081
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_len = sizeof(client_addr);

    // 2. Create Socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        return 1;
    }

    // 3. Bind to ALL interfaces (0.0.0.0) so your Laptop can see it
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed. Error: " << WSAGetLastError() << "\n";
        return 1;
    }

    // 4. Listen
    listen(server_socket, 5);
    std::cout << ">>> Windows Server listening on IP 0.0.0.0 Port " << port << " <<<\n";

    while (true) {
        // 5. Accept Connection
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Accept failed.\n";
            continue;
        }

        // 6. Send Response (Matches your Linux format)
        std::string msg = "Hello from WINDOWS LAB PC on Port " + std::to_string(port);
        send(client_socket, msg.c_str(), msg.length(), 0);

        std::cout << "Handled request from Load Balancer.\n";
        closesocket(client_socket);
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}
