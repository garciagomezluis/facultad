/*
*	675/13 - Luis García - garcia_luis_94@hotmail.com (probados con test de la cátedra y propios).
*/

#ifndef CARTAS_ENLAZADAS_H_
#define CARTAS_ENLAZADAS_H_

#include <iostream>
#include <cassert>
using namespace std;

template<typename T>
class CartasEnlazadas {

  public:

	CartasEnlazadas(): mazoRojo(NULL), mazoAzul(NULL), vencedor(NULL), longitud(0) { };

	CartasEnlazadas(const CartasEnlazadas<T>& juego): mazoRojo(NULL), mazoAzul(NULL), vencedor(NULL), longitud(0) {
		if(juego.longitud > 0) {
			for(int i = 0; i < juego.longitud; i ++) {
				Nodo* j = avanzarDesdeJugador(juego.mazoAzul, i);
				agregarJugador(*j->jugador);
				adelantarMazoAzul(1);
				sumarPuntosAlJugador(*j->jugador, j->puntaje);
			}
			adelantarMazoAzul(1);
			mazoRojo = buscarJugador( juego.dameJugadorConMazoRojo() );
			vencedor = buscarJugador( juego.ganador() );
		}
	}

	~CartasEnlazadas() {
		while(longitud != 0) {
			eliminarJugador(dameJugador(longitud-1));
		}
	}

	void agregarJugador(const T& jugador) {
		assert( !existeJugador(jugador) );
		Nodo* nodo = new Nodo();
		nodo->jugador = new T(jugador);

		if(longitud == 0) {
			nodo->siguiente = nodo;
			nodo->anterior = nodo;
			mazoAzul = nodo;
			mazoRojo = nodo;
			vencedor = nodo;
		} else {
			nodo->siguiente = mazoAzul->siguiente;
			nodo->anterior = mazoAzul;
			mazoAzul->siguiente = nodo;
			nodo->siguiente->anterior = nodo;
		}

		longitud++;
	}

	void adelantarMazoRojo(int n) {
		assert( longitud > 0 );
		mazoRojo = avanzarDesdeJugador(mazoRojo, n);
	}

	void adelantarMazoAzul(int n) {
		assert( longitud > 0 );
		mazoAzul = avanzarDesdeJugador(mazoAzul, n);
	}

	const T& dameJugadorConMazoRojo() const {
		assert( longitud > 0 );
		return *mazoRojo->jugador;
	}

	const T& dameJugadorConMazoAzul() const {
		assert( longitud > 0 );
		return *mazoAzul->jugador;
	}

	const T& dameJugador(int n) const {
		assert( longitud > 0 );
		return *avanzarDesdeJugador(mazoRojo, n)->jugador;
	}

	const T& dameJugadorEnfrentado() const {
		assert( longitud > 0 && longitud % 2 == 0 );
		return *avanzarDesdeJugador(mazoRojo, longitud / 2)->jugador;
	}

	void eliminarJugador(const T& jugador) {
		if(existeJugador(jugador)) {
			Nodo* nodo = buscarJugador(jugador);

			if(longitud > 1) {
				nodo->anterior->siguiente = nodo->siguiente;
				nodo->siguiente->anterior = nodo->anterior;
				mazoRojo = mazoRojo == nodo ? nodo->siguiente : mazoRojo;
				mazoAzul = mazoAzul == nodo ? nodo->siguiente : mazoAzul;
			} else {
				mazoRojo = NULL;
				mazoAzul = NULL;
				vencedor = NULL;
			}
			longitud--;
			delete nodo;
		}
	}

	void eliminarJugadorConMazoAzul() {
		assert( longitud > 0 );
		eliminarJugador(*mazoAzul->jugador);
	}

	bool existeJugador(const T& jugador) const {
		return longitud > 0 && *buscarJugador(jugador)->jugador == jugador;
	}

	void sumarPuntosAlJugador(const T& jugador, int p) {
		assert( existeJugador(jugador) );
		Nodo* jugador_ = buscarJugador(jugador);
		jugador_->puntaje += p;
		vencedor = jugador_->puntaje > vencedor->puntaje ? jugador_ : vencedor;
	}

	int puntosDelJugador(const T& jugador) const {
		assert( existeJugador(jugador) );
		return buscarJugador(jugador)->puntaje;
	}

	const T& ganador() const {
		assert( longitud > 0 );
		return *vencedor->jugador;
	}

	bool esVacia() const {
		return longitud == 0;
	}

	int tamanio() const {
		return longitud;
	}

	bool operator==(const CartasEnlazadas<T>& otro) const {
		bool sonIguales = 	tamanio() == otro.tamanio() &&
							esVacia() == otro.esVacia() && 
							(
								esVacia() ||
									(
										dameJugadorConMazoRojo() == otro.dameJugadorConMazoRojo() &&
										dameJugadorConMazoAzul() == otro.dameJugadorConMazoAzul() &&
										ganador() == otro.ganador()
									)
							);

		for(int i = 1; i <= otro.tamanio() && sonIguales; i++) {
			const T& j1 = dameJugador(i);
			const T& j2 = otro.dameJugador(i); 
			sonIguales = j1 == j2 && puntosDelJugador(j1) == puntosDelJugador(j2);
		}

		return sonIguales;
	}

	ostream& mostrarCartasEnlazadas(ostream& os) const {
		Nodo* nodo = mazoAzul;
		os << "[";

		if(nodo != NULL) {
			for(int i = 0; i < longitud - 1; i++) {
				os << "(" << *nodo->jugador << ", " << nodo->puntaje << ")";
				os << (nodo == mazoRojo ? "* " : "");
				os << (nodo == mazoAzul ? " " : "");
				os << ", ";
				nodo = nodo->siguiente;
			}
			os << "(" << *nodo->jugador << ", " << nodo->puntaje << ")";
			os << (nodo == mazoRojo ? "* " : "");
			os << (nodo == mazoAzul ? " " : "");
		}
		os << "]";
		return os;
	}
  private:

	CartasEnlazadas<T>& operator=(const CartasEnlazadas<T>& otra) {
		assert(false);
		return *this;
	}

    struct Nodo {
		Nodo* siguiente;
		Nodo* anterior;
		int puntaje;
		const T* jugador;

		Nodo() : siguiente(NULL), anterior(NULL), jugador(NULL), puntaje(0) { };

		~Nodo() {
			delete jugador;
		}

		bool operator==(const Nodo& otro) const {
			return *jugador == *otro->jugador;
		}
    };

	Nodo* avanzarDesdeJugador(Nodo* nodo_, int n) const {
		Nodo* nodo = nodo_;
		bool signo = n > 0;
		while(n != 0) {
			nodo = signo ? nodo->siguiente : nodo->anterior;
			n = signo ? n - 1 : n + 1;
		}
		return nodo;
	}

	Nodo* buscarJugador(const T& jugador) const {
		assert( longitud > 0 );
		Nodo* nodo = mazoAzul;
		do {
			nodo = nodo->siguiente;
		} while(nodo != mazoAzul && *nodo->jugador != jugador);
		return nodo;
	}

	Nodo* mazoRojo;
	Nodo* mazoAzul;
	Nodo* vencedor;
	int longitud;
};

template<class T>
ostream& operator<<(ostream& out, const CartasEnlazadas<T>& a) {
	return a.mostrarCartasEnlazadas(out);
}

#endif //CARTAS_ENLAZADAS_H_