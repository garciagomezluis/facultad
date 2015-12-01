#ifndef CAMPUSSEGURO_H_
#define CAMPUSSEGURO_H_

//#include <iostream>
//#include <cassert>
//#include <string>
//#include "../../aed2/Dicc.h"
#include "../campus/campus.h"
//#include "../../aed2/Conj.h"
//#include "../../aed2/TiposBasicos.h"
//#include "../../aed2/Vector.h"
#include "../../aed2/includes.h"

using namespace std;

class CampusSeguro {


public:

	//CampusSeguro( const CampusSeguro& otro ); // constructor por copia

	CampusSeguro( const Campus& c, const Dicc<Agente , Posicion >& d ); //comenzarRastrillaje

	void ingresaEstudiante( const Nombre& e , const Posicion& p);

	void ingresaHippie(const Nombre& h , const Posicion& p);

	void moverEstudiante(const Nombre& e , const Direccion& d );

	void moverHippie (const Nombre& h);

	void moverAgente(const Agente& a);

	const Campus& campus() const;

	const Conj<Nombre>::Iterador& estudiantes() const;

	const Conj<Nombre>::Iterador& hippies() const;

	const Conj<Agente>::Iterador& agentes() const;

	const Posicion& posicionEstudianteYHippie(const Nombre& n) const;

	const Posicion& posicionAgente(const Agente& a) const;

	Nat cantSanciones(const Agente& a) const;

	Nat cantHippiesAtrapados(const Agente& a) const;

	Nat cantHippies() const;

	Nat cantEstudiantes() const;

	const Agente& masVigilante() const;

	~CampusSeguro();
private:
	

	
	struct infoChabones {	bool esHippieOEstudiante ;
							bool esAgente ; 
							Nombre nombre ; 
							Agente agente;
						//	infoChabones(): {};
							};
							
	struct infoAgente {	Posicion posAgente ; 
						Nat cantSanciones ;
						Nat cantHippiesAtrapados ; 
						Conj <Posicion> hippiesMasCercanos ;
						Lista< Conj<Agente> >::Iterador mismasSanciones ;
						Conj <Agente>::Iterador mismaSancion ; 
						Conj <Agente>::Iterador itAux;
						//infoAgente(): {};
						};
						
						
	struct infoHippie {	Posicion posicion ; 
						Conj <Nombre>::Iterador itAux ; 
						Conj <Posicion> estudiantesMasCercanos;
					//	infoHippie(): {};
						
						};
	struct infoEstudiante {	Posicion posicion ; 
							Conj <Nombre>::Iterador itAux; 
							
						//	infoEstudiante(): {};
							};

	struct Tupla {
			Nat H;
			Nat E;
			Nat S;
			Nat O;
			
			 Tupla(): H(0),E(0),S(0),O(0){}
			
			};


	Agente elmasVigilante;
	bool hubieronSanciones;
	Campus campu;
	Vector <Agente> conKSanciones;
	Lista <Conj <Agente> > mismasSanciones ;
	Conj <Nombre> estudiantesAux;
	Conj <Nombre> hippiesAux;
	Conj <Agente> agentesAux;
	Vector <Vector <infoChabones> > matrizDeChabones;
	Dicc <Nombre,infoEstudiante> estudiante ;
	Dicc <Nombre,infoHippie> hippie ;
	Dicc <Agente , infoAgente> agente;

	//funciones auxiliares;

	void sancionar( Nat p, Nat& cs);
	Conj<Agente> KSanciones(Nat i);
	void insertarOrdenado(const Vector<Agente>& v,const Agente& a);
	Conj<Agente> busquedaRapida(Nat n, Vector<Agente> v);
	Conj<Agente> buscar(Vector<Agente> v,Nat i,Nat s,Nat k);
	Conj<Posicion> posicionesMasCercanas(const Posicion& p,Conj<Posicion> posiciones);
	bool estaOcupada(Posicion p);
	void modificarVecinos(const Posicion& p, Conj<Posicion> c);
	void modificarAux(const Nombre& n1, const Nombre& n2);
	void capturadoE(const Posicion& p);
	void capturadoH(const Nombre& n, const Posicion& p);
	void corregidoYcapturado(const Nombre& n, const Posicion& p);
	void convertidoYcapturado(const Nombre& n, const Posicion& p);
	void sumarSancion(Conj<Posicion> c);
	void sumarHippieAAgente(Conj<Posicion> c);
	Tupla cantPersonasAlrededor(Conj<Posicion> c);
	Nat totalOcupados(Tupla t);
	Posicion damePos(const Posicion& p1, const Posicion& p2);
	Posicion proxPosicion(Posicion p, const Direccion& d);
}; 




#endif
