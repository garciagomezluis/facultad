#ifndef CAMPUSSEGURO_H_
#define CAMPUSSEGURO_H_


#include "../diccTrie/diccTrie.h"
#include "../diccRapido/diccRapido.h"
#include "../campus/campus.h"
//#include "../../driver/Tipos.h"
#include "../../aed2/includes.h"

using namespace std;
using namespace aed2;

class CampusSeguro {

public:

	//CampusSeguro( const CampusSeguro& otro ); // constructor por copia

	CampusSeguro( const Campus& c, const Dicc<Agente , Posicion >& d ); //comenzarRastrillaje

	void IngresaEstudiante( const Nombre& e , const Posicion& p);

	void IngresaHippie(const Nombre& h , const Posicion& p);

	void MoverEstudiante(const Nombre& e , const Direccion& d );

	void MoverHippie (const Nombre& h);

	void MoverAgente(const Agente& a);

	const Campus& DameCampus() const;

	const Conj<Nombre>::const_Iterador Estudiantes() const;

	const Conj<Nombre>::const_Iterador Hippies() const;

	const Conj<Agente>::const_Iterador Agentes() const;

	const Posicion& PosicionEstudianteYHippie(const Nombre& n) ;

	const Posicion& PosicionAgente(const Agente& a) ;

	Nat CantSanciones(const Agente& a) const;

	Nat CantHippiesAtrapados(const Agente& a) const;

	Nat CantHippies() const;

	Nat CantEstudiantes() const;

	const Agente& MasVigilante() const;

	const Conj<Agente> ConMismasSanciones(Agente a) const;

	~CampusSeguro();
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
	struct infoAgente {	Posicion posAgente;
						Nat cantSanciones ; 
						Nat cantHippiesAtrapados ; 
						Conj <Posicion> hippiesMasCercanos ;
						Lista<Conj <Agente> > :: Iterador mismasSanciones ;
						Conj <Agente> :: Iterador mismaSancion ; 
						Conj <Agente> :: Iterador itAux;
					};
	struct infoHippie {	Posicion posicion ;
						Conj <Nombre> :: Iterador itAux ; 
						Conj <Posicion> estudiantesMasCercanos;
						};
	struct infoEstudiante {	Posicion posicion;
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
	Conj<Posicion> PosicionesMasCercanas(const Posicion& p,Conj<Posicion> posiciones);
	bool EstaOcupada(Posicion p);
	void ModificarVecinos(const Posicion& p, const Conj<Posicion>& c);
	void ModificarAux(const Nombre& n1, const Nombre& n2);
	void CapturadoE(const Posicion& p);
	void CapturadoH(const Nombre& n, const Posicion& p);
	void CorregidoYcapturado(const Nombre& n, const Posicion& p);
	void ConvertidoYCapturado(const Nombre& n, const Posicion& p);
	void SumarSancion(const Conj<Posicion>& c);
	void SumarHippieAAgente(const Conj<Posicion>& c);
	infoEntorno CantPersonasAlrededor(Conj<Posicion> c);
	Nat TotalOcupados(infoEntorno t);
	Posicion DamePos(const Posicion& p1, const Posicion& p2);
}; 




#endif
