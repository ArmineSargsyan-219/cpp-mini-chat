#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in server, client;
    int clientLen = sizeof(client);
    char buffer[1024];

    WSAStartup(MAKEWORD(2, 2), &wsa);
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    bind(serverSocket, (struct sockaddr*)&server, sizeof(server));
    listen(serverSocket, 3);

    std::cout << "Waiting for connection...\n";
    clientSocket = accept(serverSocket, (struct sockaddr*)&client, &clientLen);
    std::cout << "Client connected!\n";

    while (true) {
        ZeroMemory(buffer, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) break;

        std::string received(buffer);
        if (received.find("exit") != std::string::npos) {
            std::cout << "Client has left the chat." << std::endl;
            break;
        }

        std::cout << received << std::endl;

        std::string reply;
        std::cout << "You: ";
        std::getline(std::cin, reply);

         if (reply == "exit") {
            send(clientSocket, reply.c_str(), reply.size(), 0);
            std::cout << "You have left the chat." << std::endl;
            break;
        }

        send(clientSocket, reply.c_str(), reply.size(), 0);
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
