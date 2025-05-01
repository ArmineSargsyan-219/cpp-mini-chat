#include <iostream>
#include <winsock2.h>
#include <fstream>


#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    char buffer[1024];
    std::string username;

    WSAStartup(MAKEWORD(2, 2), &wsa);
    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);

    connect(sock, (struct sockaddr*)&server, sizeof(server));

    std::cout << "Enter your username: ";
    std::getline(std::cin, username);

    std::ofstream logFile("chat_log_client.txt", std::ios::app);


    while (true) {
        std::string message;
        std::cout << username << ": ";
        std::getline(std::cin, message);

        if (message == "exit") {
          logFile << "[" << username << "] left the chat.\n";
          std::cout << "You have left the chat." << std::endl;
          break;
      }
      
      std::string fullMessage = "[" + username + "]: " + message;
      send(sock, fullMessage.c_str(), fullMessage.size(), 0);
      logFile << fullMessage << std::endl;
      

        ZeroMemory(buffer, sizeof(buffer));
        int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);
if (bytesReceived <= 0) {
    std::cout << "Disconnected from server." << std::endl;
    break;
}
std::cout << buffer << std::endl;
logFile << buffer << std::endl;
    }

    closesocket(sock);
    WSACleanup();
    logFile.close();

    return 0;
}
