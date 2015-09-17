#ifndef CORREPOCOYO_H_
#define CORREPOCOYO_H_

#include <iostream>
#include <cassert>
using namespace std;

/* 
 * Se puede asumir que el tipo T tiene constructor por copia y operator==
 * No se puede asumir que el tipo T tenga operator=
 */
template<typename T>
class CorrePocoyo{

  public:

	/*
	 * Crea una nueva carrera
	 */	
	CorrePocoyo() : len(0), primero(NULL), ultimo(NULL), camara(NULL) {}
	
	/*
	 * Una vez copiada, ambas CorrePocoyos deben ser independientes, 
	 * es decir, cuando se borre una no debe borrar la otra.
	 */	
	CorrePocoyo(const CorrePocoyo<T>& carrera) : len(0), primero(NULL), ultimo(NULL), camara(NULL) {
		for( int i = 0; i < carrera.tamanio(); i++ ) {
			this->nuevoCorredor( carrera.dameCorredorEnPos(i) );
		}
	}
	
	/*
	 * Acordarse de liberar toda la memoria!
	 */	 
	~CorrePocoyo() {
		Nodo* temp;

		while(primero != NULL) {
			temp = primero;
			primero = primero->anterior;
			delete temp->corredor;
			delete temp;
		}
	}

	/*
	 * Agrega un nuevo corredor al CorrePocoyo. Lo agregará al final 
	 */
	void nuevoCorredor(const T& corredor) {
		Nodo* nuevo = new Nodo;
		nuevo -> corredor = new T(corredor); //asigno una posición de memoria al puntero definido sobre la estructura
		nuevo -> anterior = NULL;
		
		if(primero == NULL) {
			primero = nuevo;
			nuevo -> siguiente = NULL;
			ultimo = nuevo;
			camara = camara == NULL ? nuevo : camara;
		} else {
			nuevo -> siguiente = ultimo;
			ultimo -> anterior = nuevo;
			ultimo = nuevo;
		}

		len++;
	}
	
	/*
	 * Agrega un nuevo corredor al CorrePocoyo. El primer T es el corredor a agregar y lo hace delante del segundo
	 *
	 * PRE: Existe el segundo corredor 
	 */
	void nuevoCorredor(const T& corredorA, const T& corredorB) {
		Nodo* temp = primero;
		Nodo* nuevo = new Nodo;
		nuevo->corredor = new T(corredorA);

		//buscamos el corredorB (el primer nodo que coincida)
		while(temp != NULL && *temp->corredor != corredorB) {
			temp = temp->anterior;
		}

		//si se encontró alguno
		if(temp != NULL) {
			nuevo->siguiente = temp->siguiente;
			nuevo->anterior = temp;
			temp->siguiente = nuevo;

			if(nuevo->siguiente == NULL) {
				primero = nuevo;
			} else {
				nuevo->siguiente->anterior = nuevo;
			}
			
			len++;
		}
	}
	
	/*
	 * Elimina del pocoyo al que se envía
	 *
	 * PRE: Existe el corredor
	 */
	void seCansa(const T& corredor) {
		Nodo* temp = primero;

		//buscamos a corredor (primer nodo que coincida)
		while(temp != NULL && *temp->corredor != corredor) {
			temp = temp->anterior;
		}

		if(temp != NULL) {
			if(primero == temp) {
				primero = temp->anterior;
			}

			if(temp->anterior != NULL) {
				temp->anterior->siguiente = temp->siguiente;
			}

			if(temp->siguiente != NULL) {
				temp->siguiente->anterior = temp->anterior;   
			}

			//eliminamos los recursos
			delete temp->corredor;
			delete temp;
			len--;
		}
	}

	/*
	 * El corredor pasa al que está adelante 
	 *
	 * PRE: Existe un corredor delante de él y T es un corredor 
	 */
	void sobrepasar(const T& corredor) {
		Nodo* temp = primero;

		//buscamos a corredor (primer nodo que coincida)
		while(temp != NULL && *temp->corredor != corredor) {
			temp = temp->anterior;
		}
		if(temp != NULL && temp != primero) {
			if (camara == temp){
				filmarProxExitoso();
			}
			if (camara == temp -> siguiente){
				filmarProxPerdedor();
			}

			T* valorRef = temp->corredor;
			temp->corredor = temp->siguiente->corredor;
			temp->siguiente->corredor = valorRef;
		}

	}
	
	/*
	 * Devuelve el corredor que está siendo filmado.
	 *
	 * PRE: Hay adeptos en la CorrePocoyo.
	 */
	const T& corredorFilmado() const {
		return *camara->corredor;
	}
	
    /*
	 * Devuelve el próximo elemento según el orden dado.
	 * Pasa a filmar al corredor de atrás 
	 *
	 * PRE: Hay corredores en la CorrePocoyo.
	 */
	void filmarProxPerdedor() {
		camara = camara->anterior != NULL ? camara->anterior : camara;
	}

	 /*
	 * Devuelve el próximo elemento según el orden dado.
	 * Pasa a filmar al corredor de adelante
	 *
	 * PRE: Hay corredore en la CorrePocoyo.
	 */
	void filmarProxExitoso() {
		camara = camara->siguiente != NULL ? camara->siguiente : camara;
	}
	
	/*
	 * Devuelve al Primero. 
	 *
	 * PRE: Hay elementos 
	 */
	const T& damePrimero() const {
		return *primero->corredor;
	}

	/*
	 * Devuelve la posición del elemento pasado por parámetro. 
	 *
	 * PRE: Existe ese corredor 
	 */
	int damePosicion(const T& corredor) const {
		Nodo* temp = primero;
		int posicion = 0;

		//mientras buscamos a corredor (el primer nodo que coincida), aumentamos posicion
		while(temp != NULL && *temp->corredor != corredor) {
			temp = temp->anterior;
			posicion++;
		}

		return temp == NULL ? -1 : posicion;
	}

	/*
	 * Devuelve el corredor de la posición. 
	 *
	 * PRE: Existe al menos esa cantidad de corredores en la carrera
	 */
	const T& dameCorredorEnPos(int posicion) const {
		Nodo* temp = primero;

		while( posicion-- > 0 ) {
			temp = temp->anterior;
		}

		return *temp->corredor;
	}

	/*
	 * Dice si la CorrePocoyo tiene o no corredores.
	 */
	bool esVacia() const {
		return len == 0;
	}

	/*
	 * Devuelve la cantidad de corredores de la CorrePocoyo.
	 */
	int tamanio() const {
		return len;
	}	

	/*
	 * Devuelve true si las CorrePocoyos son iguales.
	 */
	bool operator==(const CorrePocoyo<T>& otra) const {
		bool sonLosMismosCorredores = otra.tamanio() == this->tamanio();

		for(int i = 0; i < otra.tamanio() && sonLosMismosCorredores; i++) {
			sonLosMismosCorredores = this->dameCorredorEnPos(i) == otra.dameCorredorEnPos(i);
		}

		return 	sonLosMismosCorredores &&
				this->damePrimero() == otra.damePrimero() &&
				this->dameUltimo() == otra.dameUltimo() &&
				this->corredorFilmado() == otra.corredorFilmado();
	};
	
	/*
	 * Debe mostrar la carrera por el ostream (y retornar el mismo).
	 * CorrePocoyo vacio: []
	 */
	ostream& mostrarCorrePocoyo(ostream& s) const {
		Nodo* temp = primero;

		s << "[";

		while(temp != NULL) {
			s << *temp->corredor;
			if (temp->anterior != NULL) {
				s << ", ";
			}
			temp = temp->anterior;
		}

		s << "]";

		return s;
	}
	
	
  private:
	/*
	 * No se puede modificar esta funcion.
	 */
	CorrePocoyo<T>& operator=(const CorrePocoyo<T>& otra) {
		assert(false);
		return *this;
	}

	/*
	 * Devuelve al Primero. 
	 *
	 * PRE: Hay elementos 
	 */
	const T& dameUltimo() const {
		return *ultimo->corredor;
	}

	struct Nodo {
		T* corredor; //guardo una direccion de memoria porque no se puede asumir que T tiene el operador =, entonces las asignaciones se realizan directamente por memoria.
		Nodo* siguiente;
		Nodo* anterior;
	};

	int len;
	Nodo* primero;
	Nodo* ultimo;
	Nodo* camara;

};

template<class T>
ostream& operator<<(ostream& out, const CorrePocoyo<T>& a) {
	return a.mostrarCorrePocoyo(out);
}

#endif //CORREPOCOYO_H_