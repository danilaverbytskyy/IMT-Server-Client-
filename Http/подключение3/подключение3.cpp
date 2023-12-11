// подключение3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

// www-client
#include "winsock2.h"
#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS  
#pragma warning(disable: 4996)  
#pragma comment (lib, "Ws2_32.lib")
#define request "GET / HTTP/1.0 \r\nHost: json.org\r\n\r\n"
#define MAX_PACKET_SIZE 4096
using namespace std;

int main() {
    WSADATA ws;
    SOCKET s;   sockaddr_in adr;   hostent* hn;
    char buff[MAX_PACKET_SIZE]; // Init 
    if (WSAStartup(0x0101, &ws) != 0) { return -1; } // Error
    if (INVALID_SOCKET == (s = socket(AF_INET, SOCK_STREAM, 0)))
        // Создаём сокет
    {
        return -1;
    } // Error 
          // Получаем адрес 
    if (NULL == (hn = gethostbyname("json.org")))
    {
        return -1;
    }  // Error 
 // Заполняем структуру с адресом

    adr.sin_family = AF_INET;
    // adr.sin_addr.s_addr = *(DWORD* ) hn->h_addr_list[0];
    ((unsigned long*)&adr.sin_addr)[0] = ((unsigned long**)hn->h_addr_list)[0][0];
    adr.sin_port = htons(80);

    // Устанавливаем соединение с сервером
    if (SOCKET_ERROR == connect(s, (sockaddr*)&adr, sizeof(adr)))
    {
        return -1;
    }

    // Посылаем запрос
    if (SOCKET_ERROR == send(s, (char*)&request, sizeof(request), 0))
    {
        return -1;
    } // Error
    int len = 0; // ждём ответа
    do {
        if (SOCKET_ERROR ==
            (len = recv(s, (char*)&buff, MAX_PACKET_SIZE, 0)))
        {
            int res = WSAGetLastError(); return -1;
        }
        for (int i = 0; i < len; i++)
            std::cout << buff[i];
    } while (len != 0);
    //получаем данные по частям, пока len не 0

    // закрываем соединение 
    if (SOCKET_ERROR == closesocket(s))
    {
        return -1;
    }  // error       

    cin.get();
    return 1;
}
