#include "Driver.h"

namespace aed2 {

	Driver::Driver()  {

	}

	Driver::~Driver() {
		delete campus;
		delete campusSeguro;
	}

	// Generadores del Campus

	void Driver::crearCampus(Nat ancho, Nat alto) {
	    campus = new Campus(ancho, alto);
	}

	void Driver::agregarObstaculo(aed2::Posicion p) {
	    campus->AgregarObstaculo(ConversionPosicion(p));
	}

	// Observadores del Campus

	Nat Driver::filas() const {
	    return campus->Filas();
	}

	Nat Driver::columnas() const {
	    return campus->Columnas();
	}

	bool Driver::ocupada(aed2::Posicion p) const {
	    return campusSeguro->EstaOcupada(ConversionPosicion(p)); 
	}

	// Generadores de CampusSeguro
	void  Driver::comenzarRastrillaje(const Dicc<Agente,aed2::Posicion>& d) {
	    campusSeguro = new CampusSeguro(campus, ConversionDiccionario(d));
	}

	void Driver::ingresarEstudiante(Nombre n, aed2::Posicion p) {
	    campusSeguro->IngresaEstudiante(n, ConversionPosicion(p));
	}

	void Driver::ingresarHippie(Nombre n, aed2::Posicion p) {
	    campusSeguro->IngresaHippie(n, ConversionPosicion(p));
	}

	void Driver::moverEstudiante(Nombre n, aed2::Direccion d) {
	    campusSeguro->MoverEstudiante(n, ConversionDireccion(d));
	}

	void Driver::moverHippie(Nombre n) {
	    campusSeguro->MoverHippie(n);
	}

	void Driver::moverAgente(Agente pl) {
	    campusSeguro->MoverAgente(pl);
	}

	// Observadores de CampusSeguro

	Nombre Driver::iesimoEstudiante(Nat i) const {
	    Conj<Nombre>::const_Iterador it = campusSeguro->Estudiantes();
	    while(i > 0) {
	    	it.Avanzar();
	    	i--;
	    }
	    return it.Siguiente();
	}

	Nombre Driver::iesimoHippie(Nat i) const {
	    Conj<Nombre>::const_Iterador it = campusSeguro->Hippies();
	    while(i > 0) {
	    	it.Avanzar();
	    	i--;
	    }
	    return it.Siguiente();
	}

	Nat Driver::iesimoAgente(Nat i) const {
	    Conj<Nat>::const_Iterador it = campusSeguro->Agentes();
	    while(i > 0) {
	    	it.Avanzar();
	    	i--;
	    }
	    return it.Siguiente();
	}

	Nat Driver::cantEstudiantes() const {
	    return campusSeguro->CantEstudiantes();
	}

	Nat Driver::cantHippies() const {
		return campusSeguro->CantHippies();
	}

	Nat Driver::cantAgentes() const {
	    Conj<Agente>::const_Iterador it = campusSeguro->Agentes();
		Nat cantidad = 0;
		while(it.HaySiguiente()) {
			cantidad++;
			it.Avanzar();
		}
		return cantidad;
	}

	aed2::Posicion Driver::posEstudianteYHippie(Nombre n) const {
	    return ConversionPosicionVuelta(campusSeguro->PosicionEstudianteYHippie(n));
	}

	aed2::Posicion Driver::posAgente(Agente pl) const {
		   return ConversionPosicionVuelta(campusSeguro->PosicionAgente(pl));
	}

	Nat Driver::cantSanciones(Agente pl) const {
	    return campusSeguro->CantSanciones(pl);
	}

	Nat Driver::cantHippiesAtrapados(Agente pl) const {
	    return campusSeguro->CantHippiesAtrapados(pl);
	}

	// Otras operaciones de CampusSeguro

	Agente Driver::masVigilante() const {
	    return campusSeguro->MasVigilante();
	}

	const Conj<Agente> Driver::conMismasSanciones(Agente a) const {
	    return campusSeguro->ConMismasSanciones(a); 
	}

	const Conj<Agente> Driver::conKSanciones(Nat k) {
	    return campusSeguro->ConKSanciones(k); 
	}

	tp::Posicion Driver::ConversionPosicion(aed2::Posicion p) const{
		return tp::Posicion(p.x - 1, p.y - 1);
		
	}

	aed2::Posicion Driver::ConversionPosicionVuelta(tp::Posicion p) const{	
		cout<<"Convirtiendo posicion"<<endl;
		aed2::Posicion p_;
		p_.x = p.x + 1;
		p_.y = p.y + 1;
		return p_;
	}

	Dicc<Agente,tp::Posicion> Driver::ConversionDiccionario(Dicc<Agente, aed2::Posicion> dicc) const{
		
		Dicc<Agente,tp::Posicion> nuestro;

		Dicc<Agente, aed2::Posicion>::Iterador it = dicc.CrearIt();
		while(it.HaySiguiente()){
			aed2::Posicion p_ = it.SiguienteSignificado();
			nuestro.DefinirRapido(it.SiguienteClave(),ConversionPosicion(p_));
			it.Avanzar();
		}

		return nuestro;
	}

	tp::Direccion Driver::ConversionDireccion(aed2::Direccion d) const{
		tp::Direccion d_;

		if(d == izq) d_ = Izquierda;
		if(d == der) d_ = Derecha;
		if(d == abajo) d_ = Arriba; //mmmm ver aca, para ellos es al revés!
		if(d == arriba) d_ = Abajo;

		return d_;
	}

} // namespace aed2