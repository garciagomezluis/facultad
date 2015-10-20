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

void check_constructor_copia() {
    CartasEnlazadas<int> mesa;

    int j1 = 1, j2 = 2, j3 = 3, j4 = 4;
    mesa.agregarJugador(j1);
    mesa.agregarJugador(j2);
    mesa.agregarJugador(j3);
    mesa.agregarJugador(j4);

    mesa.sumarPuntosAlJugador(j1, 3);
    mesa.sumarPuntosAlJugador(j1, 3);
    mesa.sumarPuntosAlJugador(j2, 4);
    mesa.adelantarMazoAzul(1);

    CartasEnlazadas<int> mesa_(mesa);

    //cout << endl << mesa << endl << mesa_ << endl;

    ASSERT_EQ(mesa == mesa_, true);
}

void check_crear_mesa_vacia() {
  CartasEnlazadas<int> mesa;

  ASSERT_EQ(mesa.esVacia(), true);

  ASSERT_EQ(mesa.tamanio(), 0);
}

void check_agregar_jugador() {
    CartasEnlazadas<int> mesa;
    int j1 = 1, j2 = 2, j3 = 3;

    mesa.agregarJugador(j1);
    mesa.agregarJugador(j2);
    mesa.agregarJugador(j3);

    ASSERT_EQ(mesa.tamanio(), 3);
    ASSERT_EQ(mesa.dameJugadorConMazoRojo(), j1);
    ASSERT_EQ(mesa.dameJugadorConMazoAzul(), j1);
    ASSERT_EQ(mesa.ganador(), j1);
    ASSERT_EQ(mesa.puntosDelJugador(j1), 0);
    ASSERT_EQ(mesa.puntosDelJugador(j2), 0);
    ASSERT_EQ(mesa.puntosDelJugador(j3), 0);
    ASSERT_EQ(mesa.dameJugador(1), j3);
    ASSERT_EQ(mesa.dameJugador(2), j2);
}

void check_adelantar_mazo_rojo() {
    CartasEnlazadas<int> mesa;

    int j1 = 1, j2 = 2;
    mesa.agregarJugador(j1);
    mesa.agregarJugador(j2);
    mesa.adelantarMazoRojo(1);
    ASSERT_EQ(mesa.dameJugadorConMazoRojo(), j2);
    mesa.adelantarMazoRojo(-1);
    ASSERT_EQ(mesa.dameJugadorConMazoRojo(), j1);
    mesa.adelantarMazoRojo(2);
    ASSERT_EQ(mesa.dameJugadorConMazoRojo(), j1);
}

void check_adelantar_mazo_azul() {
    CartasEnlazadas<int> mesa;

    int j1 = 1, j2 = 2;
    mesa.agregarJugador(j1);
    mesa.agregarJugador(j2);
    mesa.adelantarMazoAzul(1);
    ASSERT_EQ(mesa.dameJugadorConMazoAzul(), j2);
    mesa.adelantarMazoAzul(-1);
    ASSERT_EQ(mesa.dameJugadorConMazoAzul(), j1);
    mesa.adelantarMazoAzul(2);
    ASSERT_EQ(mesa.dameJugadorConMazoAzul(), j1);
}

void check_dame_jugador() {
    CartasEnlazadas<int> mesa;

    int j1 = 1, j2 = 2, j3 = 3;
    mesa.agregarJugador(j1);
    mesa.agregarJugador(j2);
    mesa.agregarJugador(j3);
    ASSERT_EQ(mesa.dameJugador(1), j3);
    ASSERT_EQ(mesa.dameJugador(2), j2);
    ASSERT_EQ(mesa.dameJugador(-1), j2);
    ASSERT_EQ(mesa.dameJugador(-3), j1);
    ASSERT_EQ(mesa.dameJugador(0), j1);
}

void check_dame_jugador_enfrentado() {
    CartasEnlazadas<int> mesa;

    int j1 = 1, j2 = 2, j3 = 3, j4 = 4;
    mesa.agregarJugador(j1);
    mesa.agregarJugador(j2);
    mesa.agregarJugador(j3);
    mesa.agregarJugador(j4);

    ASSERT_EQ(mesa.dameJugadorEnfrentado(), j3);
}

void check_eliminar_jugador() {
    CartasEnlazadas<int> mesa;

    int j1 = 1, j2 = 2, j3 = 3, j4 = 4, j5 = 5;
    mesa.agregarJugador(j1);
    mesa.agregarJugador(j2);
    mesa.agregarJugador(j3);
    mesa.agregarJugador(j4);

    mesa.eliminarJugador(j5);

    ASSERT_EQ(mesa.tamanio(), 4);
    ASSERT_EQ(mesa.existeJugador(j5), false);
    ASSERT_EQ(mesa.dameJugadorConMazoAzul(), j1);
    ASSERT_EQ(mesa.dameJugadorConMazoRojo(), j1);

    mesa.eliminarJugador(j1);

    ASSERT_EQ(mesa.tamanio(), 3);
    ASSERT_EQ(mesa.existeJugador(j1), false);
    ASSERT_EQ(mesa.dameJugadorConMazoAzul(), j4);
    ASSERT_EQ(mesa.dameJugadorConMazoRojo(), j4);

    mesa.eliminarJugador(j1);

    ASSERT_EQ(mesa.tamanio(), 3);
    ASSERT_EQ(mesa.existeJugador(j1), false);
    ASSERT_EQ(mesa.dameJugadorConMazoAzul(), j4);
    ASSERT_EQ(mesa.dameJugadorConMazoRojo(), j4);

    mesa.eliminarJugador(j3);

    ASSERT_EQ(mesa.tamanio(), 2);
    ASSERT_EQ(mesa.existeJugador(j3), false);
    ASSERT_EQ(mesa.dameJugadorConMazoAzul(), j4);
    ASSERT_EQ(mesa.dameJugadorConMazoRojo(), j4);

    mesa.eliminarJugador(j4);

    ASSERT_EQ(mesa.tamanio(), 1);
    ASSERT_EQ(mesa.existeJugador(j4), false);
    ASSERT_EQ(mesa.dameJugadorConMazoAzul(), j2);
    ASSERT_EQ(mesa.dameJugadorConMazoRojo(), j2);

    mesa.eliminarJugador(j2);

    ASSERT_EQ(mesa.tamanio(), 0);
    ASSERT_EQ(mesa.existeJugador(j2), false);

}

void check_sumar_puntos_al_jugador() {
    CartasEnlazadas<int> mesa;

    int j1 = 1, j2 = 2, j3 = 3, j4 = 4;
    mesa.agregarJugador(j1);
    mesa.agregarJugador(j2);
    mesa.agregarJugador(j3);
    mesa.agregarJugador(j4);

    mesa.sumarPuntosAlJugador(j1, 3);
    mesa.sumarPuntosAlJugador(j1, 3);

    ASSERT_EQ(mesa.ganador(), j1);

    mesa.sumarPuntosAlJugador(j2, 4);

    ASSERT_EQ(mesa.puntosDelJugador(j1), 6);
    ASSERT_EQ(mesa.puntosDelJugador(j2), 4);
    ASSERT_EQ(mesa.puntosDelJugador(j3), 0);
    ASSERT_EQ(mesa.ganador(), j1);

}

void check_equal() {
    CartasEnlazadas<int> mesa1;

    int j1 = 1, j2 = 2, j3 = 3, j4 = 4;
    mesa1.agregarJugador(j1);
    mesa1.agregarJugador(j2);
    mesa1.agregarJugador(j3);
    mesa1.agregarJugador(j4);

    mesa1.sumarPuntosAlJugador(j1, 3);
    mesa1.sumarPuntosAlJugador(j1, 3);

    CartasEnlazadas<int> mesa2;

    mesa2.agregarJugador(j1);
    mesa2.agregarJugador(j2);
    mesa2.agregarJugador(j3);
    mesa2.agregarJugador(j4);

    mesa2.sumarPuntosAlJugador(j1, 3);
    mesa2.sumarPuntosAlJugador(j1, 3);

    ASSERT_EQ(mesa1 == mesa2, true);

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
  RUN_TEST(MesaVacia);
  RUN_TEST(MesaAgregar);
  RUN_TEST(MesaSumarPuntos);
  RUN_TEST(MesaConstructorPorCopia);
  RUN_TEST(MesaAdelantarMazoRojo);
  RUN_TEST(MesaAdelantarMazoAzul);
  RUN_TEST(MesaDameJugador);
  RUN_TEST(MesaEliminarJugador);
  RUN_TEST(MesaGanador);
  RUN_TEST(MesaIgualdad);
  //mis test
  RUN_TEST(check_crear_mesa_vacia);
  RUN_TEST(check_agregar_jugador);
  RUN_TEST(check_adelantar_mazo_rojo);
  RUN_TEST(check_adelantar_mazo_azul);   
  RUN_TEST(check_dame_jugador);
  RUN_TEST(check_dame_jugador_enfrentado);
  RUN_TEST(check_eliminar_jugador);
  RUN_TEST(check_sumar_puntos_al_jugador);
  RUN_TEST(check_equal);
  RUN_TEST(check_constructor_copia);

  return 0;
}
