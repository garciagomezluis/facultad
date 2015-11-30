// g++ -g tests.cpp -o tests
// valgrind --leak-check=full -v ./tests

#include <algorithm>
#include "../../mini_test.h"
#include "diccRapido.h"

using namespace std;
using namespace aed2;

template<typename T>
string to_s(const T& m) {
 	ostringstream os;
	os << m;
	return os.str();
}

string remove_spaces(const string& s) {
  string out(s);
  out.erase(remove(out.begin(), out.end(), ' '), out.end());
  out.erase(remove(out.begin(), out.end(), '\n'), out.end());
  return out;
}

void diccRapido_constructor() {
	DiccRapido<int, int> d;
}

void diccRapido_definir() {
	DiccRapido<int, int> d;
	d.Definir(1, 1);
	ASSERT_EQ(d.Total(), 1);
	ASSERT_EQ(remove_spaces(to_s(d.Claves())), remove_spaces(to_s("{ 1 }")));
	ASSERT_EQ(d.Colisiones(), 0);
	ASSERT_EQ(d.FactorDeCarga() == (float)0.1, true);
	d.Definir(2, 1);
	ASSERT_EQ(d.Total(), 2);
	ASSERT_EQ(remove_spaces(to_s(d.Claves())), remove_spaces(to_s("{ 2, 1 }")));
	ASSERT_EQ(d.Colisiones(), 0);
	ASSERT_EQ(d.FactorDeCarga() == (float)0.2, true);
	d.Definir(1, 3);
	ASSERT_EQ(d.Total(), 2);
	ASSERT_EQ(remove_spaces(to_s(d.Claves())), remove_spaces(to_s("{ 2, 1 }")));
	ASSERT_EQ(d.Colisiones(), 1);
	ASSERT_EQ(d.FactorDeCarga() == (float)0.2, true);
	d.Definir(102, 5);
	ASSERT_EQ(d.Total(), 3);
	ASSERT_EQ(remove_spaces(to_s(d.Claves())), remove_spaces(to_s("{ 102, 2, 1 }")));
	ASSERT_EQ(d.Colisiones(), 2);
	ASSERT_EQ(d.FactorDeCarga() == (float)0.3, true);
	//A partir de acá se hace una redimension de la tabla y baja el factor de carga
	d.Definir(103, 3);
	ASSERT_EQ(d.Total(), 4);
	ASSERT_EQ(remove_spaces(to_s(d.Claves())), remove_spaces(to_s("{ 103, 102, 2, 1 }")));
	ASSERT_EQ(d.Colisiones(), 2);
	ASSERT_EQ(d.FactorDeCarga() == (float)0.2, true);

	cout << d;
}

int main() {
	//test interfáz pública de campus
	RUN_TEST(diccRapido_constructor);
	RUN_TEST(diccRapido_definir);
	RUN_TEST(diccRapido_definido);
	//RUN_TEST(diccRapido_significado);
	//RUN_TEST(diccRapido_total);
	//RUN_TEST(diccRapido_claves);
	//RUN_TEST(diccRapido_colisiones);

	return 0;
}