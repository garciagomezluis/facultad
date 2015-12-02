#include "campus.h"

using namespace std;
using namespace aed2;

bool Posicion::operator==(const Posicion& otro) const {
	return x == otro.x && y == otro.y;
}

bool Posicion::operator!=(const Posicion& otro) const {
	return !(x == otro.x && y == otro.y);
}

ostream& aed2::operator<<(ostream& os, const Posicion& p) {
	os << "[" << p.x << ", " << p.y << "]";
	return os;
}

Campus::Campus() {}

Campus::Campus(const Campus& otro) {
	filas = otro.filas;
	columnas = otro.columnas;
	matriz = Arreglo<Arreglo<bool> >(otro.matriz);
}

Campus::Campus(const Nat columnas, const Nat filas) {
	assert(0 <= columnas && 0 <= filas);

	this->columnas = columnas;
	this->filas = filas;
	matriz = Arreglo<Arreglo<bool> >(columnas);
	for(int i = 0; i < columnas; i++) {
		matriz.Definir(i, Arreglo<bool>(filas));
		for(int j = 0; j < filas; j++) {
			matriz[i].Definir(j, false);
		}
	}
}

void Campus::AgregarObstaculo(const Posicion& p) {
	assert(PosValida(p) && !EsOcupada(p));

	matriz[p.x][p.y] = true;
}

Campus::~Campus() {}

Nat Campus::Filas() const {
	return filas;
}

Nat Campus::Columnas() const {
	return columnas;
}

bool Campus::EsOcupada(const Posicion& p) const {
	assert(PosValida(p));

	return matriz[p.x][p.y];
}

bool Campus::PosValida(const Posicion& p) const {
	return 0 <= p.y && 0 <= p.x && p.y < filas && p.x < columnas;
}

bool Campus::EsIngreso(const Posicion& p) const {
	assert(PosValida(p));

	return EsIngresoSuperior(p) || EsIngresoInferior(p);
}

bool Campus::EsIngresoSuperior(const Posicion& p) const {
	assert(PosValida(p));

	return p.y == filas - 1;
}

bool Campus::EsIngresoInferior(const Posicion& p) const {
	assert(PosValida(p));

	return p.y == 0;
}

Conj<Posicion> Campus::Vecinos(const Posicion& p) const {
	assert(PosValida(p));

	Conj<Posicion> res;

	if(p.x > 0) {
		res.Agregar(Posicion(p.x - 1, p.y));
	}

	if(p.y > 0) {
		res.Agregar(Posicion(p.x, p.y - 1));
	}

	if(p.y < filas - 1) {
		res.Agregar(Posicion(p.x, p.y + 1));
	}

	if(p.x < columnas - 1) {
		res.Agregar(Posicion(p.x + 1, p.y));
	}

	return res;
}

Nat Campus::Distancia(const Posicion& p1, const Posicion& p2) const {
	return RestaPositiva(p1.x, p2.x) + RestaPositiva(p1.y, p2.y);
}

Nat Campus::RestaPositiva(const Nat a, const Nat b) const {
	return a >= b ? a - b : b - a;
}

const Posicion Campus::ProxPosicion(const Posicion& p, const Direccion& d) const {
	assert(PosValida(p));

	Nat x = p.x;
	Nat y = p.y;

	if(d == Arriba) {
		y = y + 1;
	}

	if(d == Abajo) {
		y = y - 1;
	}

	if(d == Derecha) {
		x = x + 1;
	}

	if(d == Izquierda) {
		x = x - 1;
	}

	return Posicion(x,y);
}

const Conj<Posicion> Campus::IngresosMasCercanos(const Posicion& p) const {
	assert(PosValida(p));

	Conj<Posicion> res;
	Posicion p1(p.x, 0);
	Posicion p2(p.x, filas - 1);
	if( Distancia(p, p1) < Distancia(p, p2) ) {
		res.Agregar(p1);
	} else if( Distancia(p, p1) > Distancia(p, p2) ) {
		res.Agregar(p2);
	} else {
		res.Agregar(p1);
		res.Agregar(p2);
	}
	return res;
}