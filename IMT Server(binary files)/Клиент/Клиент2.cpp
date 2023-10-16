#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <stdexcept>

using namespace std;

const int NAME_SIZE = 25;

struct Request {
    int client_id;
    char name[NAME_SIZE];
    double height;
    double weight;
};

struct Answer {
    int client_id;
    double IMT;
};

void writeRequest(const string& fileName, Request& request) {
    fstream f(fileName, ios::binary | ios::app);
    if (!f) {
        throw runtime_error("Could not open file");
    }
    f.write(reinterpret_cast<const char*>(&request), sizeof(request));
    f.close();
}

Answer getLastAnswer(const string& fileName, long currentSize) {
    size_t size_of_answer = sizeof(Answer);
    long last_answer_position = currentSize - size_of_answer;

    fstream f(fileName,ios::in | ios::binary);
    if (!f) {
        throw runtime_error("Could not open file");
    }

    Answer lastAnswer;
    f.seekg(last_answer_position);
    f.read((char*)&lastAnswer, sizeof(Answer));
    f.close();
    return lastAnswer;
}

long getSizeOfFile(const string& fileName) {
    fstream f(fileName, ios::binary | ios::app);
    if (!f) {
        throw runtime_error("Could not open file");
    }
    f.seekg(0, ios::end);
    long previousSize = f.tellg();
    f.close();
    return previousSize;
}

int main()
{
    const string PATH_ANSWERS = "C:/Users/User/Desktop/Бинарные файлы/answers.bin";
    const string PATH_REQUESTS = "C:/Users/User/Desktop/Бинарные файлы/requests.bin";
    cout << "Input CLIENT_ID\n";
    int client_id;
    cin >> client_id;
    while (true) {
        Request request;
        cout << "\nInput name (or 0 if you want to exit)\n";
        string name;
        cin >> name;
        if (name == "0") {
            return 1;
        }

        for (int i = 0; i < NAME_SIZE; i++) {
            request.name[i] = '0';
        }
        for (int i = 0; i < name.length(); i++) {
            request.name[i] = name[i];
        }
        
        cout << "Input height and weight\n";
        cin >> request.height >> request.weight;
        request.client_id = client_id;

        long previousSizeOfAnswers = getSizeOfFile(PATH_ANSWERS);
        writeRequest(PATH_REQUESTS, request);
        Sleep(200);
        long currentSizeOfAnswers = getSizeOfFile(PATH_ANSWERS);
        Answer lastAnswer = getLastAnswer(PATH_ANSWERS, currentSizeOfAnswers);
        while (previousSizeOfAnswers>=currentSizeOfAnswers || lastAnswer.client_id != request.client_id) {
            cout << "No answer\n";
            Sleep(100);
            lastAnswer = getLastAnswer(PATH_ANSWERS, currentSizeOfAnswers);
            currentSizeOfAnswers = getSizeOfFile(PATH_ANSWERS);
        }
        cout << "Your IMT = " << lastAnswer.IMT << endl;
    }
}
