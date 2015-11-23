#include campusSeguro.h


CampusSeguro::CampusSeguro( const CampusSeguro& otro ){

	agentes = new diccRapido(otro.agentes);
	hippies = new DiccString(otro.hippies);
	estudiantes = new DiccString(otro.estudiantes);
	matrizDeChabones = new Vector();
	int pos = 0;
	while (pos < otro.matrizDeChabones.longitud) {
		matrizDeChabones[pos] = new Vector(otro.matrizDeChabones[pos]);
		pos++;
	}
	agentesAux = new Conj(otro.agentesAux);
	hippiesAux = new Conj(otro.hippiesAux);
	estudiantesAux = new Conj(otro.estudiantesAux);
	mismasSanciones = new Lista();
	Lista <Conj<Agente>> :: Iterador it = otro.mismasSanciones.crearIt();
	while (it.HaySiguiente()) {
		mismasSanciones.AgregarAtras(new Conj(it.Siguiente()));
		it.Avanzar();
	}
	conKsanciones = new Vector(otro.conKsanciones);
	campus = new Campus(otro.campus);
	hubieronSanciones= otro.hubieronSanciones;	

} // constructor por copia

CampusSeguro::CampusSeguro( const Campus& c, const Dicc<Agente , Posicion >& d ){

	matrizDeChabones = new Vector<Vector<infoChabones>>();
	Nat i = 1;
	Nat j =1;
	while(i< c.filas()){
		while(j < campus.columnas()){
			Agregar(matrizDeChabones[i][j],false,false,"",0);
			j++;
		}
		i++;
	}	
	estudiantesAux = new Conj<Nombre>();
	hippiesAux = new Conj<Nombre>();
	estudiantes = new DiccString<infoEstudiante>();
	hippies = new DiccString<infoHippie>();
	agentes = new DiccRapido<Agente,infoAgente>();
	conKsanciones = new Vector<Agente>();
	mismasSanciones = new Lista<Conj<Agente>>();
	Lista<Conj<Agente>> :: iterador itMismasSanciones = mismasSanciones.crearIt();
	agentesAux = new Conj<Agente>();
	Dicc<Agente,Posicion>:: iterador it = d.crearIt();
	while(it.haySiguiente()){
		Conj<Agente>::iterador itAgente = agentesAux.Agregar(it.siguienteClave());
		Conj<Agente>::iterador itSancion = itMismasSanciones.siguiente().Agregar(it.siguienteClave());
		
		infoAgente nuevo = new infoAgente();
		
		nuevo.posAgente = it.siguienteSignificado();
		nuevo.cantSanciones = 0;
		nuevo.cantHippiesAtrapados = 0;
		nuevo.hippiesMasCercanos = new Conj<Posicion>();
		nuevo.mismasSanciones = itMismasSanciones;
		nuevo.mismaSancion = itSancion;
		nuevo.itAux = itAgente; 
		
		agente.definir(it.siguienteClave(),nuevo);

		conKsanciones.agregarAtras(it.siguienteClave());

		it.Avanzar();

	}

} //comenzarRastrillaje

void CampusSeguro::ingresaEstudiante( const Nombre& e , const Posicion& p){

	matrizDeChabones[p.x][p.y].esHippieOEstudiante = true;
	matrizDeChabones[p.x][p.y].nombre = e;
	if(cantPersonasAlrrededor(*(campus.vecinos(p))).H > 2 ){
		infoHippie nuevo = new infoHippie();
		nuevo.posicion = p;
		nuevo.itAux = hippiesAux.AgregarAtras(e);
		hippies.Definir(e,nuevo);
		modificarVecinos(p,*(campus.vecinos(p)));
		if(totalOcupados(cantPersonasArededor(*(campus.vecinos(p)))) == longitud(*(campus.vecinos(p))) && cantPersonasArededor(*(campus.vecinos(p))).S == 1){
			
			SumarHippieAAgente(*(campus.vecinos(p))));
			hippies.Significado(e).itAux.EliminarSiguiente();
			hippies.Eliminar(e);
			matrizDeChabones[p.x][p.y].esHippieOEstudiante = false;	
		}
	}else if(totalOcupados(cantPersonasArededor(*(campus.vecinos(p)))) == longitud(*(campus.vecinos(p))) && cantPersonasArededor(*(campus.vecinos(p))).S == 1){

		infoEstudiante nuevo = new infoinfoEstudiante();
		nuevo.posicion = p;
		nuevo.itAux = estudiantesAux.AgregarAtras(e);
		estudiantes.Definir(e,nuevo);
		SumarSancion(*(campus.vecinos(p)));
		modificarVecinos(p,*(campus.vecinos(p)))
	}

}

void CampusSeguro::ingresaHippie(const Nombre& h , const Posicion& p){

	infoHippie nuevo = new infoHippie();
	nuevo.posicion = p;
	nuevo.itAux = hippiesAux.AgregarAtras(h);
	hippies.Definir(h,nuevo);
	matrizDeChabones[p.x][p.y].esHippieOEstudiante = true;
	matrizDeChabones[p.x][p.y].nombre = h;
	
	if(totalOcupados(cantPersonasAlrededor(*(campus.vecinos(p)))) < longitud(*(campus.vecinos(p))))){
		modificarVecinos(p,*(campus.vecinos(p))));
	}else if(cantPersonasAlrededor(*(campus.vecinos(p))).E + cantPersonasAlrededor(*(campus.vecinos(p))).O == longitud(*(campus.vecinos(p)))){
			hippies.Significado(h).itAux.EliminarSiguiente;
			hippies.Borrar(h);
			infoEstudiante nuevo = new infoEstudiante();
			nuevo.posicion = p;
			nuevo.itAux = estudiantesAux.AgregarAtras(h);
			estudiantes.Definir(h,nuevo);
			modificarVecinos(p,*(campus.vecinos(p)))
	      }else if(cantPersonasAlrededor(*(campus.vecinos(p))).S + cantPersonasAlrededor(*(campus.vecinos(p))).O == longitud(*(campus.vecinos(p)))){
			SumarhippieAAgente(*(campus.vecinos(p)));
			hippies.Significado(h).itAux.EliminarSiguiente;
			hippies.Borrar(h);
			matrizDeChabones[p.x][p.y].esHippieOEstudiante = true;		
		}else{
		     	modificarVecinos(p,*(campus.vecinos(p)))
                     }

}

void CampusSeguro::moverEstudiante(const Nombre& e , const Direccion& d ){


}

void CampusSeguro::moverHippie (const Nombre& h){


}

void CampusSeguro::moverAgente(const Agente& a){


}

const Campus& CampusSeguro::campus() const{


}

const ItConj<Nombre>& CampusSeguro::estudiantes() const{


}

const ItConj<Nombre>& CampusSeguro::hippies() const{


}

const ItConj<Agente>& CampusSeguro::agentes() const{


}

const Posicion& CampusSeguro::posicionEstudianteYHippie(const Nombre& n) const{


}

const Posicion& CampusSeguro::posicionAgente(const Agente& a) const{


}

Nat CampusSeguro::cantSanciones(const Agente& a) const{


}

Nat CampusSeguro::cantHippiesAtrapados(const Agente& a) const{


}

Nat CampusSeguro::cantHippies() const{


}

Nat CampusSeguro::cantEstudiantes() const{


}

const Agente& CampusSeguro::masVigilante() const{


}


CampusSeguro::~CampusSeguro(){


}
