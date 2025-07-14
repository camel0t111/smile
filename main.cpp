#include <iostream>
#include <winsock2.h>
#include <conio.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define IP_ADDRESS "127.0.0.1"
#define PORT 12345

void clearScreen() {
    system("cls");
}

void sendCoords(SOCKET sock, int x, int y) {
    char msg[20];
    sprintf_s(msg, "%03d %03d", x, y);
    send(sock, msg, strlen(msg), 0);
}

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        cout << "Ошибка при создании сокета\n";
        return 1;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);

    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
        cout << "Не удалось подключиться к серверу\n";
        closesocket(sock);
        return 1;
    }

    int x = 5, y = 5;
    char ch;

    while (true) {
        clearScreen();
        cout << "X = " << x << " | Y = " << y << endl;
        cout << "W A S D для перемещения, ESC — выход\n";

        ch = _getch();

        if (ch == 27) break;
        if (ch == 'a') x--;
        if (ch == 'd') x++;
        if (ch == 'w') y--;
        if (ch == 's') y++;

        sendCoords(sock, x, y);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
