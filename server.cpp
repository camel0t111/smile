#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define PORT 12345

void clearScreen() {
    system("cls");
}

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock == INVALID_SOCKET) {
        cout << "Ошибка при создании сокета\n";
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cout << "Ошибка при привязке\n";
        closesocket(serverSock);
        return 1;
    }

    listen(serverSock, 1);
    cout << "Ожидание клиента...\n";

    sockaddr_in clientAddr;
    int clientSize = sizeof(clientAddr);
    SOCKET clientSock = accept(serverSock, (sockaddr*)&clientAddr, &clientSize);

    if (clientSock == INVALID_SOCKET) {
        cout << "Ошибка при подключении клиента\n";
        closesocket(serverSock);
        return 1;
    }

    char buffer[20];
    int x = 0, y = 0;

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(clientSock, buffer, sizeof(buffer), 0);
        if (bytes <= 0) break;

        sscanf_s(buffer, "%d %d", &x, &y);
        clearScreen();
        cout << "Координаты: (" << x << ", " << y << ")\n";
    }

    closesocket(clientSock);
    closesocket(serverSock);
    WSACleanup();
    return 0;
}
