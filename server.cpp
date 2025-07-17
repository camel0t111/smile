#include <iostream>              // для вводу/виводу
#include <winsock2.h>            // сокети Windows

#pragma comment(lib, "ws2_32.lib")  // лінкуємо сокет-бібліотеку

using namespace std;

#define PORT 12345               // той самий порт що й у клієнта

// очистка екрану для оновлення "виводу"
void clearScreen() {
    system("cls");              // знову ж таки, це працює тільки у Windows
}

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);      // ініціалізуємо WinSock

    SOCKET serverSock = socket(AF_INET, SOCK_STREAM, 0); // створюємо TCP сокет
    if (serverSock == INVALID_SOCKET) {
        cout << "Не вдалося створити сокет сервера\n";
        return 1;
    }

    // задаємо параметри сервера
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;      // приймаємо з будь-якої IP
    serverAddr.sin_port = htons(PORT);            // наш порт

    // прив'язуємо сокет до адреси
    if (bind(serverSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cout << "Помилка bind()\n";
        closesocket(serverSock);
        return 1;
    }

    // запускаємо прослуховування (1 підключення максимум)
    listen(serverSock, 1);
    cout << "Очікування підключення клієнта...\n";

    sockaddr_in clientAddr;
    int clientSize = sizeof(clientAddr);

    // приймаємо з’єднання
    SOCKET clientSock = accept(serverSock, (sockaddr*)&clientAddr, &clientSize);
    if (clientSock == INVALID_SOCKET) {
        cout << "Помилка при підключенні клієнта\n";
        closesocket(serverSock);
        return 1;
    }

    // буфер і координати
    char buffer[20];
    int x = 0, y = 0;

    while (true) {
        memset(buffer, 0, sizeof(buffer));  // чистимо буфер
        int bytes = recv(clientSock, buffer, sizeof(buffer), 0); // приймаємо
        if (bytes <= 0) break;              // якщо клієнт відвалився — вихід

        sscanf_s(buffer, "%d %d", &x, &y);  // розбираємо координати з строки

        clearScreen();  // чистимо екран

        // виводимо оновлену позицію
        cout << "Клієнт перемістив смайлик в: (" << x << ", " << y << ")\n";
    }

    // закриваємо сокети і очищаємо
    closesocket(clientSock);
    closesocket(serverSock);
    WSACleanup();
    return 0;
}
