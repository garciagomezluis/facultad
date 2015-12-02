#include "Driver.h"

namespace aed2 {

	Driver::Driver()  {

	}

	Driver::~Driver() {
		
	}

	// Generadores del Campus

	void Driver::crearCampus(Nat ancho, Nat alto) {
	    campus = Campus(ancho, alto);
	}

	void Driver::agregarObstaculo(aed2::Posicion p) {
	    campus.AgregarObstaculo(p);
	}

	// Observadores del Campus

	Nat Driver::filas() const {
	    return campus.Filas();
	}

	Nat Driver::columnas() const {
	    return campus.Columnas();
	}

	bool Driver::ocupada(aed2::Posicion p) const {
	    return campusSeguro.EsOcupada(p); //publicar en campusSeguro
	}

	// Generadores de CampusSeguro
	void  Driver::comenzarRastrillaje(const Dicc<Agente,aed2::Posicion>& d) {
	    campusSeguro = CampusSeguro(campus, d);
	}

	void Driver::ingresarEstudiante(Nombre n, aed2::Posicion p) {
	    campusSeguro.ingresaEstudiante(n, p);
	}

	void Driver::ingresarHippie(Nombre n, aed2::Posicion p) {
	    campusSeguro.ingresaHippie(n, p);
	}

	void Driver::moverEstudiante(Nombre n, Direccion d) {
	    campusSeguro.moverEstudiante(n, d);
	}

	void Driver::moverHippie(Nombre n) {
	    campusSeguro.moverHippie(n);
	}

	void Driver::moverAgente(Agente pl) {
	    campusSeguro.moverAgente(pl);
	}

	// Observadores de CampusSeguro

	Nombre Driver::iesimoEstudiante(Nat i) const {
	    Conj<Nombre>::Iterador it = campusSeguro.estudiantes();
	    while(i > 0) {
	    	it.Avanzar();
	    	i--;
	    }
	    return it.Siguiente();
	}

	Nombre Driver::iesimoHippie(Nat i) const {
	    Conj<Nombre>::Iterador it = campusSeguro.hippies();
	    while(i > 0) {
	    	it.Avanzar();
	    	i--;
	    }
	    return it.Siguiente();
	}

	Nat Driver::iesimoAgente(Nat i) const {
	    Conj<Nat>::Iterador it = campusSeguro.agentes();
	    while(i > 0) {
	    	it.Avanzar();
	    	i--;
	    }
	    return it.Siguiente();
	}

	Nat Driver::cantEstudiantes() const {
	    return campusSeguro.cantEstudiantes();
	}

	Nat Driver::cantHippies() const {
		return campusSeguro.cantHippies();
	}

	Nat Driver::cantAgentes() const {
	    Conj<Agente>::Iterador it = campusSeguro.agentes();
		Nat cantidad = 0;
		while(it.HaySiguiente()) {
			cantidad++;
			it.Avanzar();
		}
		return cantidad;
	}

	aed2::Posicion Driver::posEstudianteYHippie(Nombre n) const {
	    return campusSeguro.posicionEstudianteYHippie(n);
	}

	aed2::Posicion Driver::posAgente(Agente pl) const {
	    return campusSeguro.posicionAgente(pl);
	}

	Nat Driver::cantSanciones(Agente pl) const {
	    return campusSeguro.cantSanciones(pl);
	}

	Nat Driver::cantHippiesAtrapados(Agente pl) const {
	    return campusSeguro.cantHippiesAtrapados(pl);
	}

	// Otras operaciones de CampusSeguro

	Agente Driver::masVigilante() const {
	    return campusSeguro.masVigilante();
	}

	const Driver::Conj<Agente> conMismasSanciones(Agente a) const {
	    return campusSeguro.conMismasSanciones(a); //publicar en campusSeguro
	}

	const Driver::Conj<Agente> conKSanciones(Nat k) {
	    return campusSeguro.conKSanciones(k); //publicar en campusSeguro
	}

} // namespace aed2