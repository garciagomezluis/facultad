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

/*void LeakTest(){
	DiccString<int>* dicc = new DiccString<int>();
	
	//ASSERT_EQ(dicc->Significados().EsVacio(),true)

	int* uno = new int();
	*uno = 1;
	dicc->Definir("a",*uno);
	//ASSERT_EQ(dicc->Significados().EsVacio(),false);
	delete uno;
	delete dicc;
}*/

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
	//cout<<dicc->Significado("a")<<endl;
	ASSERT_EQ(dicc->Significados().EsVacio(),false);
	//ASSERT_EQ(dicc->Significados().Pertenece(&uno),true);

	dicc->Definir("ca",dos);

	ASSERT_EQ(dicc->Definido("S"),false);
	ASSERT_EQ(dicc->Definido("a"),true);
	ASSERT_EQ(dicc->Definido("A"),true);
	ASSERT_EQ(dicc->Definido("ca"),true);
	ASSERT_EQ(dicc->Definido("Ca"),true);

	ASSERT_EQ(dicc->Definido("caaz"),false);
	dicc->Definir("caaz",tres);
	ASSERT_EQ(dicc->Definido("caaz"),true);

	//ASSERT_EQ(dicc->Significados().Pertenece(&dos),true);
	//ASSERT_EQ(dicc->Significados().Pertenece(&tres),true);
	


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
	

	//ASSERT_EQ(dicc->Significados().Pertenece(&uno),false);
	ASSERT_EQ(dicc->Definido("zzzza"),false);
	dicc->Definir("zzzza",uno);
	ASSERT_EQ(dicc->Definido("zzzza"),true);
	ASSERT_EQ(dicc->Significados().Cardinal()==1,true);

	dicc->Borrar("zzzza");
	ASSERT_EQ(dicc->Definido("zzzza"),false);
	//ASSERT_EQ(dicc->Significados().Pertenece(&uno),false);
	ASSERT_EQ(dicc->Significados().EsVacio(),true);
	
	delete dicc;
}

void LetsBreakYou(){
	int uno = 1;
	DiccString<int>* dicc = new DiccString<int>();

	dicc->Definir("Z",uno);
	ASSERT_EQ(dicc->Definido("z"),true);
	ASSERT_EQ(dicc->Significado("z"),1);

	dicc->Definir("Testeando",uno);
	int uno_ = dicc->Significado("Testeando");
	ASSERT_EQ(uno_,uno);

	dicc->Definir("Testeanda",uno);

	dicc->Borrar("Testeando");
	//ASSERT_EQ(dicc->Significados().Pertenece(&uno),true); bug detected
	ASSERT_EQ(dicc->Definido("Testeando"),false);
	ASSERT_EQ(dicc->Definido("Testeanda"),true);

	dicc->Definir("ProbandoConUnNombreRealmenteLargoQueVaAOcuparMuchoMemoria",uno);
	ASSERT_EQ(dicc->Definido("ProbandoConUnNombreRealmenteLargoQueVaAOcuparMuchoMemoria"),true);
	ASSERT_EQ(dicc->Significados().Cardinal(),3);	

	delete dicc;

}

void Varios(){
	int uno = 1;
	int dos = 2;
	int tres = 3;
	DiccString<int> d = DiccString<int>();
	d.Definir("lu",uno);
	d.Definir("luis",dos);
	d.Definir("A3",tres);
	ASSERT_EQ(d.Significados().Cardinal(),3);
}

int main(int argc, char **argv)
{
	
	RUN_TEST(Inicializacion);
	//RUN_TEST(LeakTest);
    RUN_TEST(DefinirValores);
	RUN_TEST(ObtenerSignificados);
	RUN_TEST(Eliminar);
	RUN_TEST(LetsBreakYou);
	RUN_TEST(Varios);
	return 0;
}