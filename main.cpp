#include <iostream>              // для cout, cin
#include <winsock2.h>            // бібліотека сокетів для Windows
#include <conio.h>               // для _getch() — зчитує клавішу без Enter

#pragma comment(lib, "ws2_32.lib")  // кажемо лінкеру підключити сокет-бібліотеку

using namespace std;

#define IP_ADDRESS "127.0.0.1"   // IP сервака (localhost)
#define PORT 12345               // порт, такий самий має бути в серваку

// функція для очистки екрану (типу візуально перемістили смайлика)
void clearScreen() {
    system("cls");              // для Windows (на Mac було б "clear")
}

// функція відправляє координати на сервер
void sendCoords(SOCKET sock, int x, int y) {
    char msg[20];                              // буфер з текстом типу "012 004"
    sprintf_s(msg, "%03d %03d", x, y);         // записуємо у форматі з 0 попереду
    send(sock, msg, strlen(msg), 0);           // шлемо на сервер
}

int main() {
    WSADATA wsa;                               // для ініціалізації Winsock
    WSAStartup(MAKEWORD(2, 2), &wsa);          // запускаємо сокет-систему

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0); // створюємо TCP-сокет
    if (sock == INVALID_SOCKET) {
        cout << "Помилка створення сокета\n";
        return 1;
    }

    // налаштовуємо адресу сервера
    sockaddr_in addr;
    addr.sin_family = AF_INET;                          // IPv4
    addr.sin_port = htons(PORT);                        // порт
    addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);       // IP адреса

    // пробуємо з’єднатись із сервером
    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
        cout << "Не вдалося підключитись до сервера\n";
        closesocket(sock);
        return 1;
    }

    int x = 5, y = 5;          // стартові координати смайлика
    char ch;                   // клавіша яку натиснули

    while (true) {
        clearScreen();  // оновлюємо консоль (очищення перед новими координатами)

        // показуємо поточну позицію смайлика
        cout << "X = " << x << " | Y = " << y << endl;
        cout << "W A S D — керування, ESC — вихід\n";

        ch = _getch();  // зчитуємо клавішу без очікування Enter

        // якщо ESC — виходимо з циклу
        if (ch == 27) break;

        // керуємо положенням
        if (ch == 'a') x--;  // вліво
        if (ch == 'd') x++;  // вправо
        if (ch == 'w') y--;  // вгору
        if (ch == 's') y++;  // вниз

        // шлемо нові координати на сервер
        sendCoords(sock, x, y);
    }

    // закриваємо сокет і чистимо за собою
    closesocket(sock);
    WSACleanup();
    return 0;
}
