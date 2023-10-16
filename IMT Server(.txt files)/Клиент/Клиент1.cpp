#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

using namespace std;

struct Person {
    string name;
    double height;
    double weight;
};

struct Request {
    int client_id;
    string name;
    double height;
    double weight;
};

void addPersonInFile(string fileName, Person p, int CLIENT_ID) {
    fstream f(fileName, ios::app);
    f << CLIENT_ID << ' ' << p.name << ' ' << p.height << ' ' << p.weight << ' ' << endl;
    f.close();
}

string getLastTaskFromFile(string fileName) {
    fstream f(fileName, ios::in);
    string task = "";
    string currentLine;
    while (getline(f, currentLine)) {
        task = currentLine;
    }
    return task;
}

string getClientMessage(string answerTask) {
    for (auto it = answerTask.begin(); it < answerTask.end() && *it!=' ';) {
        it = answerTask.erase(it);
    }
    answerTask.erase(answerTask.begin());
    string IMT = answerTask;
    return "Your IMT = " + IMT;
}

int getClientIdFromAnswer(string s) {
    string number = "";
    for (int i = 0; i < s.length() && s[i]!=' '; i++) {
        number += s[i];
    }
    return stod(number);
}

int main()
{
    int CLIENT_ID;
    cout << "Input CLIENT_ID\n";
    cin >> CLIENT_ID;
    while (true) {
        cout << "Input name, height, weight\n";
        Person p;
        cin >> p.name >> p.height >> p.weight;
        addPersonInFile("../../../text.txt", p, CLIENT_ID);
        Sleep(200); //дать серверу подумать
        string lastAnswer = getLastTaskFromFile("../../../answer.txt");
        while (lastAnswer == "" || getClientIdFromAnswer(lastAnswer) != CLIENT_ID) {
            Sleep(100);
            lastAnswer = getLastTaskFromFile("../../../answer.txt");
            cout << "No answer\n";  
        }
        string clientMessage = getClientMessage(lastAnswer);
        cout << clientMessage << endl << endl;
    }
}
