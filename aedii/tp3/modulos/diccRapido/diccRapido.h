#ifndef DICCRAPIDO_H_
#define DICCRAPIDO_H_

#include "../../aed2/includes.h"

using namespace std;
using namespace aed2;
using namespace tp;
#define TAM_TABLA_INI 10
#define UMBRAL_FC   0.3

template<typename A, typename B>
class DiccRapido {

  public:

  	DiccRapido() {
      dimension = TAM_TABLA_INI;
      cantidad = 0;
      tabla = Arreglo<Lista<Item> >(dimension);

      for(Nat i = 0; i < dimension; i++) {
        tabla.Definir(i, Lista<Item>());
      }
    }

  	void Definir(const A& a, const B& b) {
      if(FactorDeCarga() > UMBRAL_FC) {
        RedimensionarTabla(2 * dimension);
      }

      typename Lista<Item>::Iterador it = tabla[FuncionDeHash(a)].CrearIt();

      while(it.HaySiguiente() && it.Siguiente().clave != a) {
        it.Avanzar();
      }

      cantidad += it.HaySiguiente() ? 0 : 1;
      
      if(it.HaySiguiente()) {
        it.EliminarSiguiente();
      }

      it.AgregarComoSiguiente(Item(a, b));
    }

  	bool Definido(const A& a) const {
      cout<<"Chequeo si estoy definido"<<endl;
      typename Lista<Item>::const_Iterador it = tabla[FuncionDeHash(a)].CrearIt();
      cout<<"Chequeo si estoy vivo"<<endl;
      bool res = false;
      while(it.HaySiguiente() && !res) {
        cout<<"Me itero"<<endl;
        res = it.Siguiente().clave == a;
        it.Avanzar();
      }
      cout<<"Me voy"<<endl;
      return res;
    }

  	const B& Significado(const A& a) const {
      cout<<"Que pasa!!"<<endl;
      assert(Definido(a));
      cout<<"Estoy definido"<<endl;
      typename Lista<Item>::const_Iterador it = tabla[FuncionDeHash(a)].CrearIt();
      while(it.HaySiguiente() && it.Siguiente().clave != a) {
        cout<<"Avanzo"<<endl;
        it.Avanzar();
      }
      cout<<"Voy a retornar"<<endl;
      return it.Siguiente().valor;
    }

    //Para tests ------------------------------------------------------------------------

    Nat Total() const {
      return cantidad;
    }

    Conj<A> Claves() const {
      Conj<A> res;
      for(Nat i = 0; i < dimension; i++) {
        typename Lista<Item>::const_Iterador it = tabla[i].CrearIt();
        while(it.HaySiguiente()) {
          res.Agregar(it.Siguiente().clave);
          it.Avanzar();
        }
      }

      return res;
    }
    
    Nat Colisiones() const {
      Nat ret = 0;
      for(Nat i=0; i < tabla.Tamanho(); i++){
          ret = ret + (tabla[i].Longitud() == 0 ? 0 : tabla[i].Longitud() - 1);
      }
      return ret;
    }

    float FactorDeCarga() const {
      return (float)cantidad / (float)tabla.Tamanho();
    }

    ostream& mostrar(ostream& os) const {

      os << "\n{\n";

      for(Nat i = 0; i < dimension; i++) {
        typename Lista<Item>::const_Iterador it = tabla[i].CrearIt();
        while(it.HaySiguiente()) {
          os << "\t" << it.Siguiente().clave << ": " << it.Siguiente().valor << "\n";
          it.Avanzar();
        }
      }

      os << "}\n";

      return os;
    }

  private:

  	struct Item {
  		A clave;
  		B valor;

  		Item(const A a, const B b) : clave(a), valor(b) {}
  	};

  	Nat dimension;
    Nat cantidad;
  	Arreglo<Lista<Item> > tabla;

    Nat FuncionDeHash(const A& a) const {
      return a % dimension;
    }

    void RedimensionarTabla(const Nat d) {
      Arreglo<Lista<Item> > tabla_vieja(tabla);
      cantidad = 0;
      tabla = Arreglo<Lista<Item> >(d);

      for(Nat i = 0; i < d; i++) {
        tabla.Definir(i, Lista<Item>());
      }

      for(Nat i=0; i < tabla_vieja.Tamanho(); i++){
        if(tabla_vieja.Definido(i)){
          for(typename Lista<Item>::const_Iterador it = tabla_vieja[i].CrearIt(); it.HaySiguiente(); it.Avanzar())
            Definir(it.Siguiente().clave, it.Siguiente().valor);
        }
      }
    }

};

template<class A, class B>
ostream& operator<<(ostream& out, const DiccRapido<A, B>& d) {
  return d.mostrar(out);
}

#endif //DICCRAPIDO_H_