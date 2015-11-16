#ifndef CAMPUSSEGURO_H_
#define CAMPUSSEGURO_H_

#include <iostream>
#include <cassert>
#include <string>
#include "../../aed2/Dicc.h"
#include "../campus/campus.h"
#include "../../driver/Tipos.h"

using namespace std;

class CampusSeguro {

public:

	CampusSeguro( const CampusSeguro& otro ); // constructor por copia

	CampusSeguro( const Campus& c, const Dicc<Agente , Posicion >& d ); //comenzarRastrillaje

	void ingresaEstudiante( const Nombre& e , const Posicion& p);

	void ingresaHippie(const Nombre& h , const Posicion& p);

	void moverEstudiante(const Nombre& e , const Direccion& d );

	void moverHippie (const Nombre& h);

	void moverAgente(const Agente& a);

	const Campus& campus() const;

	const ItConj<Nombre>& estudiantes() const;

	const ItConj<Nombre>& hippies() const;

	const ItConj<Agente>& agentes() const;

	const Posicion& posicionEstudianteYHippie(const Nombre& n) const;

	const Posicion& posicionAgente(const Agente& a) const;

	Nat cantSanciones(const Agente& a) const;

	Nat cantHippiesAtrapados(const Agente& a) const;

	Nat cantHippies() const;

	Nat cantEstudiantes() const;

	const Agente& masVigilante() const;

	~CampusSeguro();
private:

/*campusseguro se representa con estr
donde estr es tupla ( 	masVigilante : agente , 
						hubieronSanciones : bool ,
						campus : campus ,
						conKSanciones : vector(agente) ,
						mismasSanciones : lista(conj(agente)) ,
						estudiantesAux : conj(nombre) ,
						hippiesAux : conj(nombre) ,
						agentesAux : conj(agente) ,
						matrizDeChabones : vector(vector(infoChabones)) ,
						estudiantes : diccString(nombre, infoEstudiante) ,
						hippies : diccString(nombre, infoHippie) ,
						agentes : diccRapido(agente, infoAgente) )
donde infoEstudiante es tupla ( posicion : posicion ,
								itaux : itConj(nombre) )
donde infoHippies es tupla ( 	posicion : posicion ,
								itaux : itConj(nombre) , 
								estudiantesMasCercanos : conj(posicion) )
donde infoAgente es tupla ( posAgente : posicion ,
							cantSanciones : nat ,
							cantHippiesAtrapados : nat ,
							hippiesMasCercanos : conj(posicion) ,
							mismasSanciones : itLista(conj(agente)) ,
							mismaSancion : itConj(agente) ,
							itAux : itConj(agente) )

donde infoChabones es tupla ( 	esHippieOEstudiante : bool ,
								esAgente : bool ,
								nombre : nombre ,
								agente : agente )*/

};

#endif
