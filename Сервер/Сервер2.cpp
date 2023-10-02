#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

using namespace std;

struct Request {
	int client_id;
	string name;
	double height;
	double weight;
};

Request r;

string getLastTaskFromFile(string fileName) {
	fstream f(fileName, ios::in);
	string task = "";
	while (!f.eof()) {
		string x;
		getline(f, x);
		if (!f.eof()) {
			task = x;
		}
		//getline(f, task);
	}
	return task;
}

Request getRequest(string s) {
	string substr = "";
	Request r;
	int c = 0;
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == ' ') {
			c++;
			if (c == 1) {
				r.client_id = stod(substr);
			}
			else if (c == 2) {
				r.name = substr;
			}
			else if (c == 3) {
				r.height = stod(substr);
			}
			else if (c == 4) {
				r.weight = stod(substr);
				break;
			}
			i++;
			substr.clear();
		}
		substr += s[i];
	}
	substr.clear();
	return r;
}

double getIMT(double height, double weight) {
	return weight / (0.01 * height) / (0.01 * height);
}

void addAnswerToAnswers(string fileName, Request r) {
	fstream f(fileName, ios::app);
	double IMT = getIMT(r.height, r.weight);
	f << r.client_id << ' ' << IMT << endl;
}

int main()
{
	cout << "The server is working\n";
    fstream f("../text.txt");    
	f.seekg(0, ios::end);
	long previousSize = f.tellg();
	f.close();
	while (true)
	{
		Sleep(100);
		f.open("../text.txt");
		f.seekg(0, ios::end);
		long currentSize = f.tellg();
		f.close();
		if (currentSize > previousSize) {
			previousSize = currentSize;
			string lastText = getLastTaskFromFile("../text.txt");
			Request r = getRequest(lastText);
			addAnswerToAnswers("../answer.txt", r);
			cout << "The answer is sent to the client №" << r.client_id << endl;
			lastText.clear();
		}
		f.close();
	}
}
