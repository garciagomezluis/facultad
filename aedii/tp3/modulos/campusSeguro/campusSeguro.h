#ifndef CAMPUSSEGURO_H_
#define CAMPUSSEGURO_H_


#include "../diccTrie/diccTrie.h"
#include "../diccRapido/diccRapido.h"
#include "../campus/campus.h"
#include "../../aed2/includes.h"

using namespace std;
using namespace aed2;
using namespace tp;

class CampusSeguro {

public:

	CampusSeguro( const Campus& c, const Dicc<Agente , tp::Posicion >& d );

	void IngresaEstudiante( const Nombre& e , const tp::Posicion& p);

	void IngresaHippie(const Nombre& h , const tp::Posicion& p);

	void MoverEstudiante(const Nombre& e , const tp::Direccion& d );

	void MoverHippie (const Nombre& h);

	void MoverAgente(const Agente& a);

	const Campus& DameCampus() const;

	const Conj<Nombre>::const_Iterador Estudiantes() const;

	const Conj<Nombre>::const_Iterador Hippies() const;

	const Conj<Agente>::const_Iterador Agentes() const;

	const tp::Posicion& PosicionEstudianteYHippie(const Nombre& n) ;

	const tp::Posicion& PosicionAgente(const Agente& a) ;

	Nat CantSanciones(const Agente& a) const;

	Nat CantHippiesAtrapados(const Agente& a) const;

	Nat CantHippies() const;

	Nat CantEstudiantes() const;

	const Agente& MasVigilante() const;

	const Conj<Agente> ConMismasSanciones(Agente a) const;

private:

	struct infoChabones {	
		infoChabones() {
			esHippieOEstudiante = false;
			esAgente = false;
			nombre = "";
			agente = 0;
		}

		bool esHippieOEstudiante;
		bool esAgente;
		Nombre nombre; 
		Agente agente;
	};

	struct infoAgente {	
		tp::Posicion posAgente;
		Nat cantSanciones ; 
		Nat cantHippiesAtrapados ; 
		Conj <tp::Posicion> hippiesMasCercanos ;
		Lista<Conj <Agente> > :: Iterador mismasSanciones ;
		Conj <Agente> :: Iterador mismaSancion ; 
		Conj <Agente> :: Iterador itAux;
	};

	struct infoHippie {	
		tp::Posicion posicion ;
		Conj <Nombre> :: Iterador itAux ; 
		Conj <tp::Posicion> estudiantesMasCercanos;
	};

	struct infoEstudiante {	
		tp::Posicion posicion;
		Conj <Nombre> :: Iterador itAux;
	};

	/*struct infoEntorno{
		Nat Estudiantes;
		Nat Hippies;
		Nat Seguridad;
		Nat Objetos;
	};*/

	Agente masVigilante;
	bool hubieronSanciones;
	Campus campus;
	Vector <Agente> conKSanciones;
	Lista <Conj <Agente> > mismasSanciones ;
	Conj <Nombre> estudiantesAux;
	Conj <Nombre> hippiesAux;
	Conj <Agente> agentesAux;
	Vector <Vector <infoChabones> > matrizDeChabones;
	DiccString <infoEstudiante> estudiantes;
	DiccString <infoHippie> hippies;
	DiccRapido <Agente , infoAgente> agentes;

	//funciones auxiliares;

	void Sancionar(Nat p, Nat cs);
	Conj<Agente> ConKSanciones(Nat k);
	void InsertarOrdenado(Vector<Nat>& v,const Nat& a);
	Conj<Agente> BusquedaRapida(Nat n, Vector<Agente> v);
	Conj<Agente> Buscar(Vector<Agente> v,Nat i,Nat s,Nat k);
	Conj<tp::Posicion> PosicionesMasCercanas(const tp::Posicion& p,Conj<tp::Posicion> posiciones);
	bool EstaOcupada(tp::Posicion p);
	void ModificarVecinos(const tp::Posicion& p, const Conj<tp::Posicion>& c);
	void ModificarAux(const Nombre& n1, const Nombre& n2);
	void CapturadoE(const tp::Posicion& p);
	void CapturadoH(const Nombre& n, const tp::Posicion& p);
	void CorregidoYcapturado(const Nombre& n, const tp::Posicion& p);
	void ConvertidoYCapturado(const Nombre& n, const tp::Posicion& p);
	void SumarSancion(const Conj<tp::Posicion>& c);
	void SumarHippieAAgente(const Conj<tp::Posicion>& c);
	infoEntorno CantPersonasAlrededor(const Conj<tp::Posicion>& c);
	Nat TotalOcupados(infoEntorno t);
	tp::Posicion DamePos(const tp::Posicion& p1, const tp::Posicion& p2);
}; 

#endif