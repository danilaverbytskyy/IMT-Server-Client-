#include <iostream> 
#include <winsock2.h> 
#include <string> 
#include <windows.h> 
#include<conio.h> 
#pragma comment (lib, "Ws2_32.lib") 
#pragma warning(disable: 4996) // подавление предупреждения 4996 

using namespace std;

#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#define SRV_HOST "localhost"
#define SRV_PORT 1234
#define CLNT_PORT 1235
#define BUF_SIZE 64

struct Person {
    char name[25];
    double temp;
    int davl;
}B;

int main() {
    char buff[1024];
    if (WSAStartup(0x0202, (WSADATA*)&buff[0])) {
        cout << "Error WSAStartup \n" << WSAGetLastError(); // Ошибка! 
        return -1;
    }
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    SOCKET s;
    int from_len;
    char buf[BUF_SIZE] = { 0 };
    char a[BUF_SIZE] = { 0 };
    hostent* hp;//задание указателя 
    sockaddr_in clnt_sin, srv_sin;
    s = socket(AF_INET, SOCK_STREAM, 0);//сокет клиента 
    clnt_sin.sin_family = AF_INET;
    clnt_sin.sin_addr.s_addr = 0;
    clnt_sin.sin_port = htons(CLNT_PORT);//номер порта в сетевом формате для клиента 
    bind(s, (sockaddr*)&clnt_sin, sizeof(clnt_sin));//связывание сокета с адресом клиента. копирование инфо в структуру сокета.1)сокет 2)структура 3)длина 
    hp = gethostbyname(SRV_HOST);//получение ip адреса. Расположение указателя на адрес. На входе только доменные имена,не цифровые 
    srv_sin.sin_port = htons(SRV_PORT);//номер порта в сетевом формате для сервера 
    srv_sin.sin_family = AF_INET;
    ((unsigned long*)&srv_sin.sin_addr)[0] = ((unsigned long**)hp->h_addr_list)[0][0];
    connect(s, (sockaddr*)&srv_sin, sizeof(srv_sin));//соединениие с сервером. 
    string mst;
    do {
        cout << "Введите запрос: Фамилия, температура, давление" << endl;
        cin >> B.name >> B.temp >> B.davl;
        getchar();
        int msg_size = sizeof(B);
        send(s, (char*)&B, msg_size, 0);//отправка сообщения 
        from_len = recv(s, (char*)&a, BUF_SIZE, 0);//получуние сообщение от сервера 
        a[from_len] = 0;
        cout << a << endl;
        cout << "Нажмите: 1-для продолжения работы, 0-завершения  \n";
        getline(cin, mst);
        send(s, (char*)&mst[0], mst.size(), 0);
        cout << " \n";
    } while (mst != "0");
    cout << "exit to infinity" << endl;
    cin.get(); 
    closesocket(s);
    return 0;
}