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
	Campus campus(filas,columnas);
	Dicc<Agente, Posicion> agentes;
	agentes.Definir(1,Posicion(0,0));
	agentes.Definir(2,Posicion(2,2));

	CampusSeguro* c = new CampusSeguro(campus,agentes);
	
	ASSERT_EQ(true,true);
	
    
	delete c;
}



int main(int argc, char **argv)
{

	RUN_TEST(Inicializacion);
	
	return 0;
}