// Проект 4- Сервер.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <winsock2.h>  
#include <string>
#pragma comment (lib,"Ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS  
#pragma warning(disable: 4996)
using namespace std;
u_short MY_PORT = 666;  // Порт, который слушает сервер         
// макрос для печати количества активных  пользователей
#define PRINTNUSERS if (nclients)\
  cout << " user on-line " << nclients <<endl;\
  else cout << "No User on line\n";
/* прототип функции, обслуживающий  подключившихся клиентов  */
const string Bol = "Вы больны";
const string Zdor = "Вы здоровы";
DWORD WINAPI ConToClient(LPVOID client_socket);
// глобальная переменная – количество  активных пользователей
int nclients = 0;
struct Person
{
    char name[25];
    double temp;
    int davl;
} A;
int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    char buff[1024];
    cout << "TCP SERVER DEMO\n";
    // Шаг 1 - Инициализация Библиотеки Сокетов

    if (WSAStartup(0x0202, (WSADATA*)&buff[0]))
    {
        cout << "Error WSAStartup \n" << WSAGetLastError();
        return -1;
    } // Ошибка!

      // Шаг 2 - создание сокета
    SOCKET mysocket;
    if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {     // Ошибка!
        cout << "Error socket \n" << WSAGetLastError();
        WSACleanup();  // Деиницилизация библиотеки Winsock
        return -1;
    }

    // Шаг 3 связывание сокета с локальным адресом
    sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(MY_PORT);
    local_addr.sin_addr.s_addr = 0;
    // вызываем bind для связывания
    if (bind(mysocket, (sockaddr*)&local_addr, sizeof(local_addr)))
    {       // Ошибка
        cout << "Error bind \n" << WSAGetLastError();
        closesocket(mysocket);  // закрываем сокет!
        WSACleanup();
        return -1;
    }

    // Шаг 4 ожидание подключений
    // размер очереди – 0x100
    if (listen(mysocket, 0x100))
    {    // Ошибка
        cout << "Error listen: “<<WSAGetLastError()";
        closesocket(mysocket);
        WSACleanup();
        return -1;
    }
    cout << "Waiting connections\n";

    // Шаг 5 извлекаем сообщение из очереди
    SOCKET client_socket;    // сокет для клиента
    sockaddr_in client_addr;    // адрес клиента
    // функции accept необходимо передать размер структуры
    int client_addr_size = sizeof(client_addr);
    // цикл извлечения запросов на подключение из  очереди
    while ((client_socket = accept(mysocket, (sockaddr*)
        &client_addr, &client_addr_size)))
    {
        nclients++;   // увеличиваем счетчик  клиентов
        HOSTENT* hst;  // пытаемся получить имя хоста
        hst = gethostbyaddr((char*)&client_addr.sin_addr.s_addr, 4, AF_INET);
        cout << "+new connect!\n";    // вывод сведений о клиенте
        if (hst) cout << hst->h_name;    else cout << "";
        cout << inet_ntoa(client_addr.sin_addr);
        PRINTNUSERS           cout << endl;
        DWORD thID; // Вызов нового потока для клиента      
        CreateThread(NULL, NULL, ConToClient, &client_socket, NULL, &thID);
    }     return 0;
}
/* Эта функция создается в отдельном потоке и  обсуживает очередного подключившегося клиента независимо от остальных */
DWORD WINAPI ConToClient(LPVOID client_socket)
{
    SOCKET my_sock;   int len;
    my_sock = ((SOCKET*)client_socket)[0];
    char buff[1024];
    char sHELLO[] = "Hello, Doctor \r\n";
    send(my_sock, sHELLO, sizeof(sHELLO), 0);
    // отправляем клиенту приветствие
    // цикл эхо: прием строки и  возвращение ее клиенту
    while (SOCKET_ERROR != (len = recv(my_sock, (char*)&A, 1024, 0)))
    {
        cout << A.name << " " << A.temp << " " << A.davl << endl;
        if (37.0 <= A.temp || A.davl > 90)     send(my_sock, (char*)&Bol[0], Bol.size(), 0);
        else     send(my_sock, (char*)&Zdor[0], Zdor.size(), 0);
    }
    // произошел выход из цикла, соединение c клиентом разорвано
    nclients--;     // уменьшаем счетчик активных клиентов
    cout << "-disconnect\n";
    PRINTNUSERS
        closesocket(my_sock);     return 0;
}