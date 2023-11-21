#include <iostream>

void Log(const char* message);

int Multiply(int a, int b) {
	int result = a * b;
	return result;
}

int main() {
	Log("Multiply");
	std::cout << Multiply(5, 8) << std::endl;
	std::cin.get();
}