#ifndef CAMPUS_H_
#define CAMPUS_H_
#include "../../aed2/includes.h"

using namespace std;
using namespace aed2;
using namespace tp;

class Campus {

	public:

		Campus();

		Campus(const Campus&);

		Campus(const Nat, const Nat);

		void AgregarObstaculo(const tp::Posicion&);

		~Campus();

		Nat Filas() const;

		Nat Columnas() const;

		bool EsOcupada(const tp::Posicion&) const;

		bool PosValida(const tp::Posicion&) const;

		bool EsIngreso(const tp::Posicion&) const;

		bool EsIngresoSuperior(const tp::Posicion&) const;

		bool EsIngresoInferior(const tp::Posicion&) const;

		Conj<tp::Posicion> Vecinos(const tp::Posicion&) const;

		Nat Distancia(const tp::Posicion&, const tp::Posicion&) const;

		const tp::Posicion ProxPosicion(const tp::Posicion&, const tp::Direccion&) const;

		const Conj<tp::Posicion> IngresosMasCercanos(const tp::Posicion&) const;

	private:

		Nat filas;
		Nat columnas;
		Arreglo<Arreglo<bool> > matriz;

		Nat RestaPositiva(const Nat, const Nat) const;
};

#endif //CAMPUS_H_
