#include <iostream>
#include <string>
#include "Log.h"

using namespace std;

struct PlayerPublic {
	int x, y;
	int speed;

	void setX(int thisX) {
		this->x = thisX;
	}

	void setY(int thisY) {
		this->y = thisY;
	}

	void setSpeed(int thisSpeed) {
		this->speed = thisSpeed;
	}

	int getX() {
		return this->x;
	}

	int getY() {
		return this->y;
	}

	int getSpeed() {
		return this->speed;
	}

	void Move(int xa, int ya) {
		this->x += xa;
		this->y += ya;
	}
};

class Player {
	int x, y;
	int speed;

public:
	void setX(int thisX) {
		this->x = thisX;
	}

	void setY(int thisY) {
		this->y = thisY;
	}

	void setSpeed(int thisSpeed) {
		this->speed = thisSpeed;
	}

	int getX() {
		return this->x;
	}

	int getY() {
		return this->y;
	}

	int getSpeed() {
		return this->speed;
	}

	void Move(int xa, int ya) {
		this->x += xa;
		this->y += ya;
	}
};

void Increment(int& value) {
	value++;
}

int Multiply(int a, int b) {
	int result = a * b;
	Log(result);
	return result;
}

int main() {
	Player player1;
	player1.setSpeed(16);
	player1.setX(0);
	player1.setY(0);

	Log(player1.getSpeed());
	player1.Move(10, 20);
	Log(player1.getX());

	char a = 'a';
	int b = 2;
	float c = 3.00f;
	double d = 4.0;
	bool e = false;
	string f = "cat";

	int var = 8;
	void* ptr = &var;
	int* ptrTwo = &var;
	*ptrTwo = 10;
	Log(var);
	
	int& ref = var;
	ref = 15;
	Increment(var);
	Log(var);

	char* buffer = new char[8];
	memset(buffer, 0, 8);
	delete[] buffer;
	
	int x = 6;
	bool comparisonResult = x == 5;
	if (comparisonResult == true) {
		Log("Result was true!");
	}
	else {
		Log("Result was false.");
	}

	Log("Multiply");
	Multiply(5, 8);

	for (int i = 0; i <= 10; i++) {
		if (i == 0) {
			continue;
		}
		Log(i);
	}

	int i = 0;
	while (i <= 10) {
		if (i != 0) {
			Log(i);
		}
		i++;
	}
	std::cin.get();
}