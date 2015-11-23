#ifndef AED2_TIPOSBASICOS_H_INCLUDED
#define AED2_TIPOSBASICOS_H_INCLUDED

#include<iostream>
#include<string>
#include<cassert>

#ifndef NULL
	#define NULL 0
#endif

namespace aed2{
    typedef unsigned int Nat;
    typedef std::string String;
    
    /*Definimos el tipo Posicion con sus operaciones básicas*/
    struct Posicion {
        const Nat x;
        const Nat y;

        Posicion(const Nat _x, const Nat _y) : x(_x), y(_y) {}

        bool operator==(const Posicion& otro) const;

        bool operator!=(const Posicion& otro) const;
    };
    
    std::ostream& operator<<(std::ostream& os, const Posicion& p);

    typedef enum { Izquierda, Derecha, Arriba, Abajo } Direccion;
    /**/

    template<class T>
    std::ostream& Mostrar(std::ostream& os, const T& t, char izq, char der, char sep = ',') {
        typename T::const_Iterador it = t.CrearIt();
        os << izq;
        while(it.HaySiguiente()) {
            os << it.Siguiente();
            it.Avanzar();
            if(it.HaySiguiente()) os << sep;
        }
        return os << der;
    }
}

#endif //AED2_TIPOSBASICOS_H_INCLUDED
