#ifndef DICCRAPIDO_H_
#define DICCRAPIDO_H_

#include "../../aed2/includes.h"

using namespace std;
using namespace aed2;

template<typename A, typename B>
class DiccRapido {

  public:

  	DiccRapido();

  	~DiccRapido();

  	void Definir(const A& a, const B& b);

  	bool Definido(const A& a) const;

  	const B& Significado(const A& a) const;

    //Para tests

    Nat CantClaves() const;

    Conj<A> Claves() const;
    
    Nat colisiones() const;

    float FactorDeCarga() const;

  private:

    #define TAM_TABLA_INI 100
    #define UMBRAL_FC   0.75

  	struct Item {
  		const A& clave;
  		const B& valor;

  		Item(const A& a, const B& b) : clave(a), valor(b) {}
  	};

  	Nat dimension;
    Nat _cant_elems;
  	Arreglo<Lista<Item> > tabla;

    Nat FuncionHash(const A&) const;

    void RedimensionarTabla(const Nat);

};

DiccRapido::DiccRapido() {
  dimension = TAM_TABLA_INI;
  _cant_elems = 0;
  tabla = Arreglo<Lista<Elemento> >(dimension);

  for(int i = 0; i < dimension; i++) {
    tabla[i] = Lista<Elemento>();
  }
}

DiccRapido::~DiccRapido() {}

void DiccRapido::Definir(const A& a, const B& b) {

  if(FactorDeCarga() > UMBRAL_FC) {
    RedimensionarTabla(2 * dimension);
  }

  //TODO
    //incrementar _cant_elems!

}

bool DiccRapido::Definido(const A& a) const {
  Lista<Item> candidatos = tabla[funcionDeHash(a)];
  Lista<Item>::const_Iterador it = candidatos.CrearIt();
  bool res = false;
  while(it.HaySiguiente() && !res) {
    res = it.Siguiente().clave == a;
    it.Avanzar();
  }
  return res;
}

const B& DiccRapido::Significado(const A& a) const {
  assert(Definido(a));

  Lista<Item> candidatos = tabla[funcionDeHash(a)];
  Lista<Item>::const_Iterador it = candidatos.CrearIt();
  const B& res;
  while(it.HaySiguiente()) {
    if(it.Siguiente().clave == a) {
      res = it.Siguiente().valor;
    }
    it.Avanzar();
  }
  return res;
}

void DiccRapido::RedimensionarTabla(const Nat d) {
  Arreglo<Lista<Item> > tabla_vieja = tabla;
  tabla = Arreglo<Lista<Item> >(d);
  dimension = d;
  _cant_elems = 0;
  for(int i = 0; i < d; i++) {
    tabla[i] = Lista<Elemento>();
  }

  for(int i = 0; i < d; i++) {
    Lista<Item>::const_Iterador it = tabla_vieja[i].CrearIt();
    while(it.HaySiguiente()) {
      Definir(it.Siguiente().clave, it.Siguiente().valor);
      it.Avanzar();
    }
  }
}

void DiccRapido::funcionDeHash(const A& a) const {
  return a % dimension;
}

float DiccRapido::FactorDeCarga() const {
  return float(_cant_elems)/ tabla.Tamanho();
}

Nat DiccRapido::CantClaves() const {
  return _cant_elems;
}

Conj<A> DiccRapido::Claves() const {
  Conj<A> res;

  for(int i = 0; i < dimension; i++) {
    Lista<Item>::const_Iterador it = tabla[i].CrearIt();
    while(it.HaySiguiente()) {
      res.Agregar(it.Siguiente());
      it.Avanzar();
    }
  }

  return res;
}

Nat DiccRapido::colisiones() const {
  Nat ret = 0;
  for(Nat i=0; i < tabla.Tamanho(); i++){
      ret = ret + tabla[i].Longitud() - 1;
  }
  return ret;
}

#endif //DICCRAPIDO_H_