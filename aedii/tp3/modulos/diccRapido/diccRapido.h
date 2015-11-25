#ifndef DICCRAPIDO_H_
#define DICCRAPIDO_H_

#include "../../aed2/includes.h"

using namespace std;
using namespace aed2;

template<typename A, typename B>
class DiccRapido {

  public:

  	DiccRapido() {
  		dimension = 100;
      vector = Arreglo<Lista<Elemento> >(dimension);

      for(int i = 0; i < dimension; i++) {
        vector[i] = Lista<Elemento>();
      }
  	}

  	~DiccRapido();

  	void Definir(const A& a, const B& b) {
      if(a > dimension) {
        RedimencionarArreglo(a, b);
      }
      vector[FuncionHash(a, b)].AgregarAtras(b);
    }

  	bool EsDefinido(const A& a) const;

  	B& Significado(const A& a) const;

  private:

  	struct Elemento {
  		const A& clave;
  		const B& valor;

  		Elemento(const A& a, const B& b) : clave(a), valor(b) {}
  	};

  	Nat dimension;
  	Arreglo<Lista<Elemento> > vector;

    Nat FuncionHash(const A& a, const B& b) const {
      return a + b % vector.Tamanho();
    }

    void RedimencionarArreglo(const A& a, const B& b) {
      Arreglo<Lista<Elemento> > anterior(vector);

      vector.Redimensionar(0);
      vector.Redimensionar(anterior.Tamanho()*2);

      for(int i=0; i < anterior.Tamanho(); i++){
        if(anterior.Definido(i)){
          typename Lista<Elemento>::const_Iterador it = anterior[i].CrearIt();
          while(it.HaySiguiente()) {
            Definir(it.Siguiente().clave, it.Siguiente().valor);
            it.Avanzar();
          }
        }
      }
    }


};

#endif //DICCRAPIDO_H_