#include <iostream>
#include "biginteger.h"
using namespace std;

int main() {
	BigInt a, b;
	while (1) {
		cout << "Please input a: ";
		cin >> a;
		cout << "Please input b: ";
		cin >> b;

		cout << "a + b = " << a + b << endl;
		cout << "a - b = " << a - b << endl;
		cout << "a * b = " << a * b << endl;
		cout << "a / b = " << a / b << endl;
		cout << "a % b = " << a % b << endl;
		cout << endl;
	}
	return 0;
}