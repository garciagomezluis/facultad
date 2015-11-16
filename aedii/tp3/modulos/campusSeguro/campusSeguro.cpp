#include campusSeguro.h


CampusSeguro::CampusSeguro( const CampusSeguro& otro ){

	
	
} // constructor por copia

CampusSeguro::CampusSeguro( const Campus& c, const Dicc<Agente , Posicion >& d ){


} //comenzarRastrillaje

void CampusSeguro::ingresaEstudiante( const Nombre& e , const Posicion& p){


}

void CampusSeguro::ingresaHippie(const Nombre& h , const Posicion& p){


}

void CampusSeguro::moverEstudiante(const Nombre& e , const Direccion& d ){


}

void CampusSeguro::moverHippie (const Nombre& h){


}

void CampusSeguro::moverAgente(const Agente& a){


}

const Campus& CampusSeguro::campus() const{


}

const ItConj<Nombre>& CampusSeguro::estudiantes() const{


}

const ItConj<Nombre>& CampusSeguro::hippies() const{


}

const ItConj<Agente>& CampusSeguro::agentes() const{


}

const Posicion& CampusSeguro::posicionEstudianteYHippie(const Nombre& n) const{


}

const Posicion& CampusSeguro::posicionAgente(const Agente& a) const{


}

Nat CampusSeguro::cantSanciones(const Agente& a) const{


}

Nat CampusSeguro::cantHippiesAtrapados(const Agente& a) const{


}

Nat CampusSeguro::cantHippies() const{


}

Nat CampusSeguro::cantEstudiantes() const{


}

const Agente& CampusSeguro::masVigilante() const{


}


CampusSeguro::~CampusSeguro(){


}