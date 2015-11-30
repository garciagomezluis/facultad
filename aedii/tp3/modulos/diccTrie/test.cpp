#include <string>
#include <iostream>
#include "mini_test.h"
#include "diccTrie.h"

using namespace aed2;

void Inicializacion()
{

	DiccString<int>* dicc = new DiccString<int>();
	
	//ASSERT_EQ(dicc->Significados().EsVacio(),true);
	ASSERT_EQ(dicc->Definido("S"),false);
	delete dicc;
    

}

void LeakTest(){
	DiccString<int>* dicc = new DiccString<int>();
	
	//ASSERT_EQ(dicc->Significados().EsVacio(),true)

	int* uno = new int();
	*uno = 1;
	dicc->Definir("a",*uno);
	//ASSERT_EQ(dicc->Significados().EsVacio(),false);
	delete uno;
	delete dicc;
}

void DefinirValores(){
	DiccString<int>* dicc = new DiccString<int>();
	int uno = 1;
	int dos = 2;
	int tres=3;

	ASSERT_EQ(dicc->Significados().EsVacio(),true);
	ASSERT_EQ(dicc->Significados().Pertenece(&uno),false);
	ASSERT_EQ(dicc->Significados().Pertenece(&dos),false);
	ASSERT_EQ(dicc->Significados().Pertenece(&tres),false);
	dicc->Definir("a",uno);
	ASSERT_EQ(dicc->Significados().Pertenece(&uno),true);
	dicc->Definir("ca",dos);
	ASSERT_EQ(dicc->Definido("S"),false);
	ASSERT_EQ(dicc->Definido("a"),true);
	ASSERT_EQ(dicc->Definido("A"),true);
	ASSERT_EQ(dicc->Definido("ca"),true);

	ASSERT_EQ(dicc->Definido("caaz"),false);
	dicc->Definir("caaz",tres);
	ASSERT_EQ(dicc->Definido("caaz"),true);

	ASSERT_EQ(dicc->Significados().Pertenece(&dos),true);
	ASSERT_EQ(dicc->Significados().Pertenece(&tres),true);
	ASSERT_EQ(dicc->Significados().EsVacio(),false);


	delete dicc;
}

void ObtenerSignificados(){
	DiccString<int>* dicc = new DiccString<int>();

	int uno = 1;
	int tres=3;

	ASSERT_EQ(dicc->Definido("caaz"),false);
	dicc->Definir("caaz",tres);
	dicc->Definir("c",uno);
	ASSERT_EQ(dicc->Definido("caaz"),true);
	ASSERT_EQ(dicc->Definido("caa"),false);
	ASSERT_EQ(dicc->Definido("caab"),false);

	ASSERT_EQ(dicc->Significado("c"),uno);
	ASSERT_EQ(dicc->Significado("caaz"),tres);

	

	delete dicc;
}

void Eliminar(){
	DiccString<int>* dicc = new DiccString<int>();

	int uno = 1;
	

	ASSERT_EQ(dicc->Significados().Pertenece(&uno),false);
	ASSERT_EQ(dicc->Definido("zzzza"),false);
	dicc->Definir("zzzza",uno);
	ASSERT_EQ(dicc->Definido("zzzza"),true);
	ASSERT_EQ(dicc->Significados().Pertenece(&uno),true);

	dicc->Borrar("zzzza");
	ASSERT_EQ(dicc->Definido("zzzza"),false);
	ASSERT_EQ(dicc->Significados().Pertenece(&uno),false);

	
	delete dicc;
}

int main(int argc, char **argv)
{

	RUN_TEST(Inicializacion);
	//RUN_TEST(LeakTest);
    RUN_TEST(DefinirValores);
	RUN_TEST(ObtenerSignificados);
	RUN_TEST(Eliminar);

	return 0;
}