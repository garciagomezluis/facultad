#include <string>
#include <iostream>
#include "../../mini_test.h"
#include "campusSeguro.h"

using namespace aed2;
using namespace std;

void Inicializacion()
{
	Nat filas = 3;
	Nat columnas = 3;
	Campus* campus = new Campus(filas, columnas);
	Dicc<Agente, Posicion> agentes;
	agentes.Definir(1,Posicion(0,0));
	agentes.Definir(2,Posicion(2,2));

	CampusSeguro* c = new CampusSeguro(campus,agentes);

	delete c;
	delete campus;
}

void IngresarEstudiantes(){
	Nat filas = 3;
	Nat columnas = 3;
	Campus* campus = new Campus(filas, columnas);
	Dicc<Agente, Posicion> agentes;
	agentes.Definir(1,Posicion(0,0));
	agentes.Definir(2,Posicion(2,2));

	CampusSeguro* c = new CampusSeguro(campus,agentes);

	c->IngresaEstudiante("lu", Posicion(0,2));
	c->IngresaEstudiante("luis", Posicion(1,2));
	c->IngresaEstudiante("julian", Posicion(1,0));
	
	delete c;
	delete campus;
}

void IngresarHippies(){
	Nat filas = 3;
	Nat columnas = 3;
	Campus* campus = new Campus(filas, columnas);
	Dicc<Agente, Posicion> agentes;
	agentes.Definir(1,Posicion(0,0));
	agentes.Definir(2,Posicion(2,2));

	CampusSeguro* c = new CampusSeguro(campus,agentes);
	//cout<<"1"<<endl;
	//c->IngresaHippie("lu", Posicion(0,2));
	//cout<<"2"<<endl;
	c->IngresaHippie("luis", Posicion(2,0));
	cout<<"CAMBIO DE PERSONA"<<endl;
	c->IngresaHippie("julian", Posicion(1,0));
	cout<<"4"<<endl;

	delete c;
	delete campus;
}

int main(int argc, char **argv)
{

	RUN_TEST(Inicializacion);
	RUN_TEST(IngresarEstudiantes);
	RUN_TEST(IngresarHippies);
	
	return 0;
}