#ifndef TP_TIPOSBASICOS_H_INCLUDED
#define TP_TIPOSBASICOS_H_INCLUDED

#include<iostream>
#include<string>
#include<cassert>

#ifndef NULL
	#define NULL 0
#endif
using namespace aed2;
namespace tp
{
    typedef unsigned int Nat;
    typedef std::string String;
    typedef String Nombre;
    typedef Nat Agente;

    struct infoEntorno{
        Nat Estudiantes;
        Nat Hippies;
        Nat Seguridad;
        Nat Objetos;
    };
    
    /*Definimos el tipo Posicion con sus operaciones básicas*/
    struct Posicion {
        Nat x;
        Nat y;

        Posicion() {}

        Posicion(const Posicion& otro) {
            x = otro.x;
            y = otro.y;
        }

        Posicion(Nat _x, Nat _y) : x(_x), y(_y) {}

        bool operator==(const Posicion& otro) const;

        bool operator!=(const Posicion& otro) const;
    };
    
    std::ostream& operator<<(std::ostream& os, const Posicion& p);

    typedef enum { Izquierda, Derecha, Arriba, Abajo } Direccion;

   
}

#endif //TP_TIPOSBASICOS_H_INCLUDED
