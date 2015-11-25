#include <string>
#include <iostream>
#include "mini_test.h"
#include "diccTrie.h"

using namespace aed2;

void Inicializacion()
{

	DiccString<int>* dicc = new DiccString<int>();
	
	ASSERT_EQ(dicc->Significados().EsVacio(),true);
	ASSERT_EQ(dicc->Definido("S"),false);
	delete dicc;
    

}

void DefinirValores(){
	DiccString<int>* dicc = new DiccString<int>();
	
	ASSERT_EQ(dicc->Significados().EsVacio(),true)

	dicc->Definir("a",1);


	delete dicc;
}

int main(int argc, char **argv)
{
    RUN_TEST(Inicializacion);
    RUN_TEST(DefinirValores);
	
	/********************************************************************
	 * TODO: escribir casos de test exhaustivos para todas              *
	 * las funcionalidades de cada módulo.                              *
     * La interacción con el TAD principal se debe hacer exclusivamente *
	 * a través de la interfaz del driver.                              *
	 ********************************************************************/

	return 0;
}