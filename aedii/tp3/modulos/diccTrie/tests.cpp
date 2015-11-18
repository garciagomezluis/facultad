// g++ -g tests.cpp -o tests
// valgrind --leak-check=full -v ./tests

#include <algorithm>
#include "mini_test.h"
#include "cartas_enlazadas.h"

using namespace std;

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

void MesaVacia() {
  CartasEnlazadas<int> mesa;
  ASSERT_EQ(mesa.esVacia(), true);
  ASSERT_EQ(mesa.tamanio(), 0);
  ASSERT_EQ(to_s(mesa), "[]");
}

void MesaAgregar() {
  CartasEnlazadas<int> mesa;
  mesa.agregarJugador(1);
  ASSERT_EQ(mesa.esVacia(), false);
  ASSERT_EQ(mesa.tamanio(), 1);

  ASSERT_EQ(remove_spaces(to_s(mesa)), remove_spaces("[(1, 0)*]"));
  mesa.agregarJugador(2);
ASSERT_EQ(mesa.esVacia(), false);
ASSERT_EQ(mesa.tamanio(), 2);
ASSERT_EQ(remove_spaces(to_s(mesa)), remove_spaces("[(1, 0)*, (2, 0)]"));
  mesa.agregarJugador(3);
  ASSERT_EQ(mesa.esVacia(), false);
  ASSERT_EQ(mesa.tamanio(), 3);
  ASSERT_EQ(remove_spaces(to_s(mesa)), remove_spaces("[(1, 0)*, (3, 0), (2, 0)]"));
}

void MesaSumarPuntos(){
  CartasEnlazadas<int> mesa;
  mesa.agregarJugador(1);
  mesa.sumarPuntosAlJugador(1, 2);
  ASSERT_EQ(remove_spaces(to_s(mesa)), remove_spaces("[(1, 2)*]"));
}

void MesaAdelantarMazoRojo() {
  CartasEnlazadas<int> mesa;
  mesa.agregarJugador(1);
  mesa.agregarJugador(2);
  mesa.agregarJugador(3);
  ASSERT_EQ(mesa.dameJugadorConMazoRojo(), 1);
  mesa.adelantarMazoRojo(1);
  ASSERT_EQ(mesa.dameJugadorConMazoRojo(), 3);
  ASSERT_EQ(remove_spaces(to_s(mesa)), remove_spaces("[(1, 0), (3, 0)*, (2, 0)]"));
  mesa.adelantarMazoRojo(1);
  ASSERT_EQ(mesa.dameJugadorConMazoRojo(), 2);
  ASSERT_EQ(remove_spaces(to_s(mesa)), remove_spaces("[(1, 0), (3, 0), (2, 0)*]"));
  mesa.adelantarMazoRojo(-1);
  ASSERT_EQ(mesa.dameJugadorConMazoRojo(), 3);
  ASSERT_EQ(remove_spaces(to_s(mesa)), remove_spaces("[(1, 0), (3, 0)*, (2, 0)]"));
  mesa.adelantarMazoRojo(-2);
  ASSERT_EQ(mesa.dameJugadorConMazoRojo(), 2);
  ASSERT_EQ(remove_spaces(to_s(mesa)), remove_spaces("[(1, 0), (3, 0), (2, 0)*]"));
}

void MesaAdelantarMazoAzul() {
  CartasEnlazadas<int> mesa;
  mesa.agregarJugador(1);
  mesa.agregarJugador(2);
  mesa.agregarJugador(3);
  ASSERT_EQ(mesa.dameJugadorConMazoAzul(), 1);
  ASSERT_EQ(remove_spaces(to_s(mesa)), remove_spaces("[(1, 0)*, (3, 0), (2, 0)]"));
  mesa.adelantarMazoAzul(1);
  ASSERT_EQ(mesa.dameJugadorConMazoAzul(), 3);
  ASSERT_EQ(remove_spaces(to_s(mesa)), remove_spaces("[(3, 0), (2, 0), (1, 0)*]"));
  mesa.adelantarMazoAzul(1);
  ASSERT_EQ(mesa.dameJugadorConMazoAzul(), 2);
  ASSERT_EQ(remove_spaces(to_s(mesa)), remove_spaces("[(2, 0), (1, 0)*, (3, 0)]"));
  mesa.adelantarMazoAzul(-1);
  ASSERT_EQ(mesa.dameJugadorConMazoAzul(), 3);
  ASSERT_EQ(remove_spaces(to_s(mesa)), remove_spaces("[(3, 0), (2, 0), (1, 0)*]"));
  mesa.adelantarMazoAzul(-2);
  ASSERT_EQ(mesa.dameJugadorConMazoAzul(), 2);
  ASSERT_EQ(remove_spaces(to_s(mesa)), remove_spaces("[(2, 0), (1, 0)*, (3, 0)]"));
}

void MesaDameJugador() {
  CartasEnlazadas<int> mesa;
  mesa.agregarJugador(1);
  mesa.agregarJugador(2);
  mesa.agregarJugador(3);
  mesa.agregarJugador(4);
  ASSERT_EQ(mesa.dameJugador(0), 1);
  ASSERT_EQ(mesa.dameJugador(1), 4);
  ASSERT_EQ(mesa.dameJugador(-1), 2);
  ASSERT_EQ(mesa.dameJugadorEnfrentado(), 3);
  mesa.adelantarMazoAzul(1);
  ASSERT_EQ(mesa.dameJugadorEnfrentado(), 3);

}

void MesaConstructorPorCopia() {
  CartasEnlazadas<int> mesa;
  mesa.agregarJugador(1);
  mesa.sumarPuntosAlJugador(1, 2);
  mesa.agregarJugador(2);
  mesa.agregarJugador(3);
  mesa.adelantarMazoAzul(1);
  mesa.adelantarMazoRojo(-1);
  CartasEnlazadas<int> mesa2(mesa);
  ASSERT_EQ(to_s(mesa2), to_s(mesa));
}

void MesaEliminarJugador() {
  CartasEnlazadas<int> mesa;
  mesa.agregarJugador(1);
  mesa.agregarJugador(2);
  mesa.agregarJugador(3);
  mesa.agregarJugador(4);
  ASSERT_EQ(remove_spaces(to_s(mesa)), remove_spaces("[(1, 0)*, (4, 0), (3, 0), (2, 0)]"));
  mesa.eliminarJugador(3);
  ASSERT_EQ(remove_spaces(to_s(mesa)), remove_spaces("[(1, 0)*, (4, 0), (2, 0)]"));
  mesa.eliminarJugador(1);
  ASSERT_EQ(remove_spaces(to_s(mesa)), remove_spaces("[(4, 0)*, (2, 0)]"));
  mesa.agregarJugador(1);
  ASSERT_EQ(remove_spaces(to_s(mesa)), remove_spaces("[(4, 0)*, (1, 0), (2, 0)]"));
  mesa.adelantarMazoRojo(2);
  ASSERT_EQ(remove_spaces(to_s(mesa)), remove_spaces("[(4, 0), (1, 0), (2, 0)*]"));
  mesa.eliminarJugadorConMazoAzul();
  ASSERT_EQ(remove_spaces(to_s(mesa)), remove_spaces("[(1, 0), (2, 0)*]"));
}

void MesaGanador() {
  CartasEnlazadas<int> mesa;
  mesa.agregarJugador(1);
  mesa.agregarJugador(2);
  mesa.agregarJugador(3);
  mesa.agregarJugador(4);
  ASSERT_EQ(remove_spaces(to_s(mesa)), remove_spaces("[(1, 0)*, (4, 0), (3, 0), (2, 0)]"));
  mesa.sumarPuntosAlJugador(1, 5);
  ASSERT_EQ(mesa.puntosDelJugador(1), 5);
  mesa.sumarPuntosAlJugador(1, 5);
  ASSERT_EQ(remove_spaces(to_s(mesa)), remove_spaces("[(1, 10)*, (4, 0), (3, 0), (2, 0)]"));
  mesa.sumarPuntosAlJugador(2, 6);
  ASSERT_EQ(mesa.puntosDelJugador(2), 6);
  mesa.sumarPuntosAlJugador(2, 6);
  ASSERT_EQ(remove_spaces(to_s(mesa)), remove_spaces("[(1, 10)*, (4, 0), (3, 0), (2, 12)]"));
  mesa.sumarPuntosAlJugador(4, 7);
  ASSERT_EQ(mesa.puntosDelJugador(4), 7);
  mesa.sumarPuntosAlJugador(4, 7);
  ASSERT_EQ(remove_spaces(to_s(mesa)), remove_spaces("[(1, 10)*, (4, 14), (3, 0), (2, 12)]"));


}

void MesaIgualdad() {
	CartasEnlazadas<int> mesa1;
	CartasEnlazadas<int> mesa2;
	// comparacion de mesas vacias
  ASSERT(mesa1==mesa2);
	mesa1.agregarJugador(1);
	// comparacion de mesas distintas, una vacia, otra no
	ASSERT(!(mesa1==mesa2));
	ASSERT(!(mesa2==mesa1));
	// comparacion de mesas iguales con 1 elemento
	mesa2.agregarJugador(1);
	ASSERT(mesa2==mesa1);
	// comparacion de mesas distintas con 1 y 2 elemento
	mesa1.agregarJugador(2);
	ASSERT(!(mesa1==mesa2));
	ASSERT(!(mesa2==mesa1));
	mesa2.agregarJugador(2);
	ASSERT(mesa2==mesa1);
	// comparacion de mesas iguales con 2 elemento, diferente posiciones de mazo
	mesa2.adelantarMazoAzul(1);
	ASSERT(!(mesa1==mesa2));
	ASSERT(!(mesa2==mesa1));
	mesa2.adelantarMazoAzul(-1);
	ASSERT(mesa2==mesa1);
	mesa2.adelantarMazoRojo(1);
	ASSERT(!(mesa1==mesa2));
	ASSERT(!(mesa2==mesa1));
	mesa2.adelantarMazoRojo(1);
	ASSERT(mesa2==mesa1);
}

int main() {
  RUN_TEST(MesaVacia); //OK
  RUN_TEST(MesaAgregar); //OK
  RUN_TEST(MesaSumarPuntos); //OK
  RUN_TEST(MesaConstructorPorCopia); //OK
  RUN_TEST(MesaAdelantarMazoRojo); //OK
  RUN_TEST(MesaAdelantarMazoAzul); //OK
  RUN_TEST(MesaDameJugador); //OK
  RUN_TEST(MesaEliminarJugador); //OK
  RUN_TEST(MesaGanador); //OK
  RUN_TEST(MesaIgualdad);  //OK
  return 0;
}

