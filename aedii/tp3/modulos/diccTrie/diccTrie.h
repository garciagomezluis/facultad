#ifndef AED2_DICC_TRIE_
#define	AED2_DICC_TRIE_

#include <ostream>
#include <string>
#include "../../aed2/Vector.h"
#include "../../aed2/Conj.h"
#include "../../aed2/TiposBasicos.h"
using namespace std;

namespace aed2{

template<typename T>
class DiccString
{
	public: 
	DiccString();
	~DiccString();
	void Definir(const string s, /*const*/ T& significado); //Creo que no es const porque se guarda en la estructura interna como referencia no constante.
	bool Definido(const string s) const;
	T& Significado(const string s);
	Conj<T*> Significados();
	void Borrar(const string s);

	private:
		int LONG_ALPH;
		struct valores{
			/*valores(){
				significado = NULL;
				vec = new Vector<valores>();
			}*/
			T* significado;
			Vector<valores>* vec;
			typename Conj<T*>::Iterador it; //TODO. REVISAR POR QUE SE NECESITA TYPENAME ADELANTE
			/*~valores(){
				delete significado;
				//delete this->vec;
			}*/
		};
		
		Vector<valores>* vec;
		Conj<T*>* significados;
	/*PRE. EsVacio(vec)*/	
	void DefinirVec(Vector<valores>& vec);
	void EliminarVec(Vector<valores>* vec);
	int ord(const char c) const;


};

template<typename T>
DiccString<T>::DiccString(){
	this->vec = new Vector<valores>();
	this->significados = new Conj<T*>();
	LONG_ALPH = 26; //TODO. Ver por que no puedo definir como const.
}


template<typename T>
DiccString<T>::~DiccString(){
	cout<<"destructor"<<endl;
	//EliminarVec(vec);

	delete this->vec;
	delete this->significados;
}

template<typename T>
Conj<T*> DiccString<T>::Significados(){
	return *this->significados;
}

template<typename T>
void DiccString<T>::Definir(const string s, /*const*/ T& significado){
	if(vec->EsVacio())
		DefinirVec(*vec);
	Vector<valores>* aux = vec;
	int i = 1;
	while(i < s.length()){
		aux = (*aux)[ord(s[i])].vec;
		if(aux->EsVacio())
			DefinirVec(*aux);
		i++;
	}

	(*aux)[ord(s[i-1])].significado = &significado;
	typename Conj<T*>::Iterador it = significados->Agregar(&significado);
	(*aux)[ord(s[i-1])].it = it;
	//delete aux;
}

template<typename T>
void DiccString<T>::DefinirVec(Vector<valores>& vec){
	int i = 0;
	while(i < LONG_ALPH){
		//cout<<i<<endl;
		valores valoresVacios;
		valoresVacios.vec = new Vector<valores>();
		valoresVacios.significado = NULL;
		vec.AgregarAtras(valoresVacios);
		i++;
	}
}

template<typename T>
void DiccString<T>::EliminarVec(Vector<valores>* vector){
	if(vector->EsVacio()){
		cout<<"deleting vec"<<endl;
		delete vector;
		return;
	}
	int i = 0;
	while(i < LONG_ALPH){
		cout<<i<<endl;
		Vector<valores> v = *vector;
		valores aux = v[i];
		delete aux.significado;
		//delete aux.it;
		/*if(!aux.vec->EsVacio()){
			cout<<"Vector lleno. borrando"<<endl;
			EliminarVec(aux.vec);
		}*/
		EliminarVec(aux.vec);
		

		i++;
	}	
}

template<typename T>
bool DiccString<T>::Definido(const string s) const{
	if(vec->EsVacio()){
		return false;
	}
	Vector<valores> aux = *vec;
	int i = 1;
	while(i < s.length() && !aux.EsVacio()){
		aux = *aux[ord(s[i])].vec;
		i++;
	}
	if(!aux.EsVacio() && aux[ord(s[i-1])].significado!=NULL)
		return true;
	else
		return false;
}

template<typename T>
T& DiccString<T>::Significado(const string s){
	Vector<valores>* aux = vec;
	int i = 1;
	while(i < s.length()){
		
		aux = (*aux)[ord(s[i])].vec;
		i++;
	}
	
	return *(*aux)[ord(s[i-1])].significado;
}

template<typename T>
int DiccString<T>::ord(const char c) const{
	int result = -1;
	switch(c){
		case 'A':
		case 'a': result=0; break;
		case 'B':
		case 'b': result=1; break;
		case 'C':
		case 'c': result=2; break;
		case 'D':
		case 'd': result=3; break;
		case 'E':
		case 'e': result=4; break;
		case 'F':
		case 'f': result=5; break;
		case 'G':
		case 'g': result=6; break;
		case 'H':
		case 'h': result=7; break;
		case 'I':
		case 'i': result=8; break;
		case 'J':
		case 'j': result=9; break;
		case 'K':
		case 'k': result=10; break;
		case 'L':
		case 'l': result=11; break;
		case 'M':
		case 'm': result=12; break;
		case 'N':
		case 'n': result=13; break;
		case 'O':
		case 'o': result=14; break;
		case 'P':
		case 'p': result=15; break;
		case 'Q':
		case 'q': result=16; break;
		case 'R':
		case 'r': result=17; break;
		case 'S':
		case 's': result=18; break;
		case 'T':
		case 't': result=19; break;
		case 'U':
		case 'u': result=20; break;
		case 'V':
		case 'v': result=21; break;
		case 'W':
		case 'w': result=22; break;
		case 'X':
		case 'x': result=23; break;
		case 'Y':
		case 'y': result=24; break;
		case 'Z':
		case 'z': result=25; break;
		default: result=26; break;
	}
	return result;
}


};

#endif	//AED2_DICC_TRIE_