#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <fstream>
#pragma comment(lib, "ws2_32.lib")
using namespace std;
int main() {
    SetConsoleOutputCP(1251);
    //клієнт
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5000);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cout << "Помилка з'єднання: " << WSAGetLastError() << endl;
        return 1;
    }
    ifstream file("text.rtf", ios::binary);
    if (!file) {
        cout << "Не вдалося відкрити файл\n";
        return 1;
    }
    /* Суцільно файл
       char buffer[4096]; // більший буфер

        while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
        int bytes = file.gcount();
        send(clientSocket, buffer, bytes, 0);
}
    */
    const int FRAGMENT_SIZE = 512;
    char buffer[FRAGMENT_SIZE];
    while (!file.eof()) {
        file.read(buffer, FRAGMENT_SIZE);
        int bytes = file.gcount();

        if (bytes > 0) {
            send(clientSocket, buffer, bytes, 0);
            cout << "Відправлено: " << bytes << " байт\n";
        }
    }
    cout << "Файл відправлено\n";
    file.close();
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
