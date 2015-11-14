#ifndef AED2_DICC_TRIE_
#define	AED2_DICC_TRIE_

#include <ostream>
#include <string>
#include "../aed2/Vector.h"
#include "../aed2/Conj.h"

namespace aed2{

template<typename T>
class DiccString
{
	public: 
	DiccString();
	void Definir(const string s, const T significado);
	bool Definido(const string s) const;
	T& Significado(const string s);
	Conj<T*> Significados();
	void Borrar(const string s);

	private:
		struct valores{
			T* significado;
			Vector<valores> vector;
			Iterador<T*> it;
		};
		struct estr{
			Vector<valores> vector;
			Conj<T*> significados;
		};
};

//TODO. IMPLEMENTAR FUNCIONES AQUI.

};

#endif	//AED2_DICC_TRIE_