#include <iostream> 
#include <winsock2.h> 
#include <windows.h> 
#include <string> 
#include<conio.h> 

#pragma comment (lib, "Ws2_32.lib") 

using namespace std;

#define SRV_PORT 1234 
#define BUF_SIZE 64 

const string ILL_MESSAGE = "Вы больны";
const string HEALTHY_MESSAGE = "Вы здоровы";

struct Person {
    char name[25]; 
    double temp; 
    int davl;
} A;

int main() {
    //устанавливает кодировку
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    char buff[1024];
    if (WSAStartup(0x0202, (WSADATA*)&buff[0])) //инициализация в библиотеке.1)версия в библиотеке 2)структура для записи 
    {
        cout << "Error WSAStartup \n" << WSAGetLastError();
        return -1;
    }
    SOCKET s, s_new;
    int from_len, a;
    char buf[BUF_SIZE] = { 0 };
    sockaddr_in sin, from_sin; //структуры 
    s = socket(AF_INET, SOCK_STREAM, 0);//создание слушащего сокета сервера.1)Домен 2)Потоковый режим 3)TCP 
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = 0; //адресс ip узла 
    sin.sin_port = htons(SRV_PORT);//номер порта в сетевом формате 
    bind(s, (sockaddr*)&sin, sizeof(sin));//связывание сокета и структуры 
    string message;
    listen(s, 3);//создание очереди сокетов 
    while (true) {//работа сервера с клиентом 
        from_len = sizeof(from_sin);
        s_new = accept(s, (sockaddr*)&from_sin, &from_len);//запрос на установку соединения 1)сокет прослушивающийся сервером 
        cout << "new connected client! \n";
        while (true) {
            //общение с отдельным клиентом    
            from_len = recv(s_new, (char*)&A, BUF_SIZE, 0);//прием сообщения 
            cout << A.name << " " << A.temp << " " << A.davl << endl;
            if (A.temp >= 37.0 || A.davl > 90) {
                send(s_new, (char*)&ILL_MESSAGE[0], ILL_MESSAGE.size(), 0);
            }
            else {
                send(s_new, (char*)&HEALTHY_MESSAGE[0], HEALTHY_MESSAGE.size(), 0);
            }
            from_len = recv(s_new, (char*)&buf, BUF_SIZE, 0);
            buf[from_len] = 0;
            message = (string)buf;
            if (message == "0") {
                break;
            }
        }
        cout << "client is lost";
        cout << " \n";
        closesocket(s_new);//закрытие сокета 
    }
    return 0;
}
