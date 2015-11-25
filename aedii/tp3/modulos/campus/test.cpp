// g++ -g tests.cpp -o tests
// valgrind --leak-check=full -v ./tests

#include <algorithm>
#include "../../mini_test.h"
#include "campus.h"

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

void campus_constructor() {
	Campus c (10, 15);
	//sólo revisa que algunas posiciones aleatorias de la matriz hayan iniciado vacias
	Posicion p1 (7, 7);
	ASSERT_EQ(c.EsOcupada(p1), false);
	Posicion p2 (7, 8);
	ASSERT_EQ(c.EsOcupada(p2), false);
	Posicion p3 (0, 0);
	ASSERT_EQ(c.EsOcupada(p3), false);
	Posicion p4 (9, 14);
	ASSERT_EQ(c.EsOcupada(p4), false);
}

void campus_agregarObstaculo() {
	Campus c (10, 15);
	//sólo revisa que una vez agregado el obstáculo esa posición ya no se encuentre vacia
	Posicion p1 (7, 7);
	c.AgregarObstaculo(p1);
	ASSERT_EQ(c.EsOcupada(p1), true);
	Posicion p2 (7, 8);
	c.AgregarObstaculo(p2);
	ASSERT_EQ(c.EsOcupada(p2), true);
	Posicion p3 (0, 0);
	c.AgregarObstaculo(p3);
	ASSERT_EQ(c.EsOcupada(p3), true);
	Posicion p4 (9, 14);
	c.AgregarObstaculo(p4);
	ASSERT_EQ(c.EsOcupada(p4), true);
}

void campus_filas() {
	Campus c (10, 15);
	ASSERT_EQ(c.Filas(), 15);
}

void campus_columnas() {
	Campus c (10, 15);
	ASSERT_EQ(c.Columnas(), 10);
}

void campus_esOcupada() {
	Campus c (10, 15);
	Posicion p1 (7, 7);
	ASSERT_EQ(c.EsOcupada(p1), false);
	Posicion p2 (7, 8);
	ASSERT_EQ(c.EsOcupada(p2), false);
	Posicion p3 (0, 0);
	ASSERT_EQ(c.EsOcupada(p3), false);
	Posicion p4 (9, 14);
	ASSERT_EQ(c.EsOcupada(p4), false);
}

void campus_posValida() {
	Campus c(10, 15);
	Posicion p1 (7, 7);
	ASSERT_EQ(c.PosValida(p1), true);
	Posicion p2 (7, 8);
	ASSERT_EQ(c.PosValida(p2), true);
	Posicion p3 (0, 0);
	ASSERT_EQ(c.PosValida(p3), true);
	Posicion p4 (9, 14);
	ASSERT_EQ(c.PosValida(p4), true);
	Posicion p5 (30, 30);
	ASSERT_EQ(c.PosValida(p5), false);
	Posicion p6 (10, 15);
	ASSERT_EQ(c.PosValida(p6), false);
	Posicion p7 (7, 15);
	ASSERT_EQ(c.PosValida(p7), false);
	Posicion p8 (15, 7);
	ASSERT_EQ(c.PosValida(p8), false);
}

void campus_esIngreso() {
	Campus c(10, 15);
	Posicion p1 (0, 0);
	ASSERT_EQ(c.EsIngreso(p1), true);
	Posicion p2 (5, 0);
	ASSERT_EQ(c.EsIngreso(p2), true);
	Posicion p3 (9, 0);
	ASSERT_EQ(c.EsIngreso(p3), true);
	Posicion p4 (9, 14);
	ASSERT_EQ(c.EsIngreso(p4), true);
	Posicion p5 (9, 1);
	ASSERT_EQ(c.EsIngreso(p5), false);
	Posicion p6 (5, 14);
	ASSERT_EQ(c.EsIngreso(p6), true);
	Posicion p7 (9, 14);
	ASSERT_EQ(c.EsIngreso(p7), true);
	Posicion p8 (9, 13);
	ASSERT_EQ(c.EsIngreso(p8), false);
	Posicion p9 (7, 14);
	ASSERT_EQ(c.EsIngreso(p9), true);
	Posicion p10 (7, 7);
	ASSERT_EQ(c.EsIngreso(p10), false);
	Posicion p11 (0, 7);
	ASSERT_EQ(c.EsIngreso(p11), false);
	Posicion p12 (4, 7);
	ASSERT_EQ(c.EsIngreso(p12), false);
}

void campus_esIngresoSuperior() {
	Campus c(10, 15);
	Posicion p1 (0, 0);
	ASSERT_EQ(c.EsIngresoSuperior(p1), false);
	Posicion p2 (5, 0);
	ASSERT_EQ(c.EsIngresoSuperior(p2), false);
	Posicion p3 (9, 0);
	ASSERT_EQ(c.EsIngresoSuperior(p3), false);
	Posicion p4 (9, 14);
	ASSERT_EQ(c.EsIngresoSuperior(p4), true);
	Posicion p5 (9, 1);
	ASSERT_EQ(c.EsIngresoSuperior(p5), false);
	Posicion p6 (5, 14);
	ASSERT_EQ(c.EsIngresoSuperior(p6), true);
	Posicion p7 (9, 14);
	ASSERT_EQ(c.EsIngresoSuperior(p7), true);
	Posicion p8 (9, 13);
	ASSERT_EQ(c.EsIngresoSuperior(p8), false);
	Posicion p9 (7, 14);
	ASSERT_EQ(c.EsIngresoSuperior(p9), true);
	Posicion p10 (7, 7);
	ASSERT_EQ(c.EsIngresoSuperior(p10), false);
	Posicion p11 (0, 7);
	ASSERT_EQ(c.EsIngresoSuperior(p11), false);
	Posicion p12 (4, 7);
	ASSERT_EQ(c.EsIngresoSuperior(p12), false);
}

void campus_esIngresoInferior() {
	Campus c(10, 15);
	Posicion p1 (0, 0);
	ASSERT_EQ(c.EsIngresoInferior(p1), true);
	Posicion p2 (5, 0);
	ASSERT_EQ(c.EsIngresoInferior(p2), true);
	Posicion p3 (9, 0);
	ASSERT_EQ(c.EsIngresoInferior(p3), true);
	Posicion p4 (9, 14);
	ASSERT_EQ(c.EsIngresoInferior(p4), false);
	Posicion p5 (9, 1);
	ASSERT_EQ(c.EsIngresoInferior(p5), false);
	Posicion p6 (5, 14);
	ASSERT_EQ(c.EsIngresoInferior(p6), false);
	Posicion p7 (9, 14);
	ASSERT_EQ(c.EsIngresoInferior(p7), false);
	Posicion p8 (9, 13);
	ASSERT_EQ(c.EsIngresoInferior(p8), false);
	Posicion p9 (7, 14);
	ASSERT_EQ(c.EsIngresoInferior(p9), false);
	Posicion p10 (7, 7);
	ASSERT_EQ(c.EsIngresoInferior(p10), false);
	Posicion p11 (0, 7);
	ASSERT_EQ(c.EsIngresoInferior(p11), false);
	Posicion p12 (4, 7);
	ASSERT_EQ(c.EsIngresoInferior(p12), false);
}

void campus_vecinos() {
	Campus c(10, 15);
	Posicion p1 (0, 0);
	ASSERT_EQ(remove_spaces(to_s(c.Vecinos(p1))), remove_spaces("{ [1, 0], [0, 1] }"));
	Posicion p2 (9, 0);
	ASSERT_EQ(remove_spaces(to_s(c.Vecinos(p2))), remove_spaces("{ [9, 1], [8, 0] }"));
	Posicion p3 (0, 14);
	ASSERT_EQ(remove_spaces(to_s(c.Vecinos(p3))), remove_spaces("{ [1, 14], [0, 13] }"));
	Posicion p4 (9, 14);
	ASSERT_EQ(remove_spaces(to_s(c.Vecinos(p4))), remove_spaces("{ [9, 13], [8, 14] }"));
	Posicion p5 (7, 7);
	ASSERT_EQ(remove_spaces(to_s(c.Vecinos(p5))), remove_spaces("{ [8, 7], [7, 8], [7, 6], [6, 7] }"));
	Posicion p6 (3, 0);
	ASSERT_EQ(remove_spaces(to_s(c.Vecinos(p6))), remove_spaces("{ [4, 0], [3, 1], [2, 0] }"));
	Posicion p7 (3, 14);
	ASSERT_EQ(remove_spaces(to_s(c.Vecinos(p7))), remove_spaces("{ [4, 14], [3, 13], [2, 14] }"));
	Posicion p8 (0, 3);
	ASSERT_EQ(remove_spaces(to_s(c.Vecinos(p8))), remove_spaces("{ [1, 3], [0, 4], [0, 2] }"));
	Posicion p9 (9, 3);
	ASSERT_EQ(remove_spaces(to_s(c.Vecinos(p9))), remove_spaces("{ [9, 4], [9, 2], [8, 3] }"));
}

void campus_distancia() {
	Campus c(10, 15);
	Posicion p1 (0, 0);
	Posicion p2 (2, 2);
	Posicion p3 (1, 3);
	Posicion p4 (9, 14);
	ASSERT_EQ(c.Distancia(p1, p2), 4);
	ASSERT_EQ(c.Distancia(p2, p3), 2);
	ASSERT_EQ(c.Distancia(p3, p4), 19);
	ASSERT_EQ(c.Distancia(p4, p1), 23);
	ASSERT_EQ(c.Distancia(p3, p1), 4);
}

void campus_proxPosicion() {
	Campus c(10, 15);

	Posicion p1(0, 0);
	ASSERT_EQ(remove_spaces(to_s(c.ProxPosicion(p1, Derecha))), remove_spaces("[1, 0]"));
	ASSERT_EQ(remove_spaces(to_s(c.ProxPosicion(p1, Arriba))), remove_spaces("[0, 1]"));

	
	Posicion p2(9, 0);
	ASSERT_EQ(remove_spaces(to_s(c.ProxPosicion(p2, Izquierda))), remove_spaces("[8, 0]"));
	ASSERT_EQ(remove_spaces(to_s(c.ProxPosicion(p2, Arriba))), remove_spaces("[9, 1]"));
	
	Posicion p3(0, 14);
	ASSERT_EQ(remove_spaces(to_s(c.ProxPosicion(p3, Derecha))), remove_spaces("[1, 14]"));
	ASSERT_EQ(remove_spaces(to_s(c.ProxPosicion(p3, Abajo))), remove_spaces("[0, 13]"));
	
	Posicion p4(9, 14);
	ASSERT_EQ(remove_spaces(to_s(c.ProxPosicion(p4, Izquierda))), remove_spaces("[8, 14]"));
	ASSERT_EQ(remove_spaces(to_s(c.ProxPosicion(p4, Abajo))), remove_spaces("[9, 13]"));
	
	Posicion p5(7, 7);
	ASSERT_EQ(remove_spaces(to_s(c.ProxPosicion(p5, Izquierda))), remove_spaces("[6, 7]"));
	ASSERT_EQ(remove_spaces(to_s(c.ProxPosicion(p5, Derecha))), remove_spaces("[8, 7]"));
	ASSERT_EQ(remove_spaces(to_s(c.ProxPosicion(p5, Arriba))), remove_spaces("[7, 8]"));
	ASSERT_EQ(remove_spaces(to_s(c.ProxPosicion(p5, Abajo))), remove_spaces("[7, 6]"));
}

void campus_ingresosMasCercanos() {
	Campus c(10, 15);
	Posicion p1 (0, 0);
	ASSERT_EQ(remove_spaces(to_s(c.IngresosMasCercanos(p1))), remove_spaces("{ [0, 0] }"));
	Posicion p2 (5, 5);
	ASSERT_EQ(remove_spaces(to_s(c.IngresosMasCercanos(p2))), remove_spaces("{ [5, 0] }"));
	Posicion p3 (8, 7);
	ASSERT_EQ(remove_spaces(to_s(c.IngresosMasCercanos(p3))), remove_spaces("{ [8, 14], [8, 0] }"));
	Posicion p4 (9, 14);
	ASSERT_EQ(remove_spaces(to_s(c.IngresosMasCercanos(p4))), remove_spaces("{ [9, 14] }"));
}

int main() {
	//test interfáz pública de campus
	RUN_TEST(campus_constructor);
	RUN_TEST(campus_agregarObstaculo);
	RUN_TEST(campus_filas);
	RUN_TEST(campus_columnas);
	RUN_TEST(campus_esOcupada);
	RUN_TEST(campus_posValida);
	RUN_TEST(campus_esIngreso);
	RUN_TEST(campus_esIngresoSuperior);
	RUN_TEST(campus_esIngresoInferior);
	RUN_TEST(campus_vecinos);
	RUN_TEST(campus_distancia);
	RUN_TEST(campus_proxPosicion);
	RUN_TEST(campus_ingresosMasCercanos);

	return 0;
}