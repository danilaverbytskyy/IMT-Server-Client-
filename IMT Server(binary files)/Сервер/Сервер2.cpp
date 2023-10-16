#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <stdexcept>

using namespace std;

struct Request {
	int client_id;
	char name[25];
	double height;
	double weight;
};

struct Answer {
	int client_id;
	double IMT;
};

double getIMT(double height, double weight) {
	return weight / (0.01 * height) / (0.01 * height);
}

void addAnswerToAnswers(string fileName, Request r) {
	fstream f(fileName, ios::binary | ios::app);
	if (!f) {
		throw runtime_error("Could not open file");
	}
	double IMT = getIMT(r.height, r.weight);
	Answer answer{ r.client_id, IMT };
	f.write(reinterpret_cast<const char*>(&answer), sizeof(answer));
	f.close();
}

Request getLastRequest(string filename, long currentSize) {
	size_t size_of_request = sizeof(Request);
	long last_request_position = currentSize - size_of_request;

	ifstream file(filename, ios::binary);
	if (!file) {
		throw runtime_error("Could not open file");
	}

	Request last_request;
	file.seekg(last_request_position);
	file.read((char*)&last_request, size_of_request);

	file.close();
	return last_request;
}

int main()
{
	const string PATH_ANSWERS = "C:/Users/User/Desktop/Бинарные файлы/answers.bin";
	const string PATH_REQUESTS = "C:/Users/User/Desktop/Бинарные файлы/requests.bin";
	cout << "The server is working\n";
	fstream f(PATH_REQUESTS, ios::binary | ios::app);
	if (!f) {
		throw runtime_error("Could not open file");
	}
	f.seekg(0, ios::end);
	long previousSize = f.tellg();
	f.close();
	while (true)
	{
		Sleep(100);
		f.open(PATH_REQUESTS, ios::binary | ios::app);
		if (!f) {
			throw runtime_error("Could not open file");
		}
		f.seekg(0, ios::end);
		long currentSize = f.tellg();
		if (currentSize > previousSize) {
			Request lastRequest = getLastRequest(PATH_REQUESTS, currentSize);
			cout << "Got request from client№" << lastRequest.client_id << endl;
			addAnswerToAnswers(PATH_ANSWERS, lastRequest);
			cout << "Sent answer to client№" << lastRequest.client_id << endl;
			previousSize = currentSize;
		}
		f.close();
	}
}
