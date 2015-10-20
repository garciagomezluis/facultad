#include "correpocoyo.h"
#include "iostream"

//Faltan terminar varias cosas y correr los test.

int main() {

	CorrePocoyo<int> carrera1;

	carrera1.nuevoCorredor(1);
	carrera1.nuevoCorredor(2);
	carrera1.nuevoCorredor(3);

	carrera1.nuevoCorredor(5,2);
	carrera1.nuevoCorredor(4,3);

	cout << carrera1 << endl;

/*
	CorrePocoyo<int> carrera2;

	carrera2.nuevoCorredor(1);
	if( carrera1 == carrera2 ) {
		cout << "son iguales" << endl;
	} else {
		cout << "no son iguales" << endl;
	}
*/


	return 0;

}