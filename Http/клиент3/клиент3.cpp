// клиент3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

// www-client.cpp
#include <string>
#define _WINSOCK_DEPRECATED_NO_WARNINGS  
// подавление предупреждений библиотеки winsock2
#include <winsock2.h>
#include <iostream>
#pragma comment (lib,"Ws2_32.lib")
#pragma warning(disable: 4996)  // подавление предупреждения 4996
using namespace std;
#define request "Get/ http/1.1\r\n host: localhost \r\n\r\n" //html запрос.
#define max_packet_size   65535
struct Person
{
    char name[25];
    double temp;
    int davl;
}B;
int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    WSADATA  ws;
    SOCKET   s;
    sockaddr_in adr;
    HOSTENT* hn;
    char    buff[max_packet_size];
    if (WSAStartup(0x0202, &ws) != 0) { return -1; }    // error
    // создаём сокет
    if (INVALID_SOCKET == (s = socket(AF_INET, SOCK_STREAM, 0)))
    {
        return -1;
    }
    // получаем адрес
    if (NULL == (hn = gethostbyname("localhost")))
    {
        return -1;
    }   // error
// заполняем  поля структуры adr для использование ее в connect
    adr.sin_family = AF_INET;
    ((unsigned long*)&adr.sin_addr)[0] =
        ((unsigned long**)hn->h_addr_list)[0][0];
    adr.sin_port = htons(8000);
    // устанавливаем соединение с сервером
    if (SOCKET_ERROR == connect(s, (sockaddr*)&adr, sizeof(adr)))
    {
        int res = WSAGetLastError(); return -1;
    } //error
// посылаем запрос серверу
    if (SOCKET_ERROR == send(s, (char*)&request, sizeof(request), 0))
    {
        int res = WSAGetLastError(); return -1;
    }   // error
// ждём ответа
    int len = 0;
    cout << "Введите запрос: Фамилия, температура, давление" << endl;
    cin >> B.name >> B.temp >> B.davl;
    getchar();
    send(s, (char*)&B, sizeof(B), 0);
    do
    {
        if (SOCKET_ERROR == (len = recv(s, (char*)&buff, max_packet_size, 0)))
        {
            int res = WSAGetLastError();     return -1;
        }
        for (int i = 0; i < len; i++)
            cout << buff[i];

    } while (len != 0); //получаем данные по частям, пока не len != 0.

    if (SOCKET_ERROR == closesocket(s))   // закрываем соединение 
    {
        return -1;
    }  // error
    cin.get();
    return 1;
}
