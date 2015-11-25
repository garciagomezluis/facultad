#ifndef CAMPUS_H_
#define CAMPUS_H_
#include "../../aed2/includes.h"

using namespace std;
using namespace aed2;

class Campus {

	public:

		Campus(const Nat columnas, const Nat filas);

		void AgregarObstaculo(const Posicion&);

		~Campus();

		Nat Filas() const;

		Nat Columnas() const;

		bool EsOcupada(const Posicion&) const;

		bool PosValida(const Posicion&) const;

		bool EsIngreso(const Posicion&) const;

		bool EsIngresoSuperior(const Posicion&) const;

		bool EsIngresoInferior(const Posicion&) const;

		Conj<Posicion>* Vecinos(const Posicion&) const;

		Nat Distancia(const Posicion&, const Posicion&) const;

		const Posicion* ProxPosicion(const Posicion&, const Direccion&) const;

		const Conj<Posicion>* IngresosMasCercanos(const Posicion&) const;

	private:

		Nat filas;
		Nat columnas;
		Arreglo<Arreglo<bool> >* matriz;

		Nat RestaPositiva(const Nat, const Nat) const;
};

#endif //CAMPUS_H_