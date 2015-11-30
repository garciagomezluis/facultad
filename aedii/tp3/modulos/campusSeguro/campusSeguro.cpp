#include "campusSeguro.h"

/*
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
	conKSanciones = new Vector(otro.conKSanciones);
	campus = new Campus(otro.campus);
	hubieronSanciones= otro.hubieronSanciones;	

} // constructor por copia
*/

CampusSeguro::CampusSeguro( const Campus& c, const Dicc<Agente , Posicion >& d ){

	matrizDeChabones = Vector<Vector<infoChabones>>();
	Nat i = 1;
	Nat j =1;
	while(i< c.Filas()){
		while(j < campus.Columnas()){
			matrizDeChabones[i].Agregar(j, infoChabones());
			j++;
		}
		i++;
	}	
	estudiantesAux = Conj<Nombre>();
	hippiesAux = Conj<Nombre>();
	estudiantes = DiccString<infoEstudiante>();
	hippies = DiccString<infoHippie>();
	agentes = DiccRapido<Agente,infoAgente>();
	conKSanciones = Vector<Agente>();
	mismasSanciones = Lista<Conj<Agente>>();
	Lista<Conj<Agente> > :: Iterador itMismasSanciones = mismasSanciones.CrearIt();
	agentesAux = Conj<Agente>();
	Dicc<Agente,Posicion>:: const_Iterador it = d.CrearIt();
	while(it.HaySiguiente()){
		Conj<Agente>::Iterador itAgente = agentesAux.Agregar(it.SiguienteClave());
		Conj<Agente>::Iterador itSancion = itMismasSanciones.Siguiente().Agregar(it.SiguienteClave());
		
		infoAgente nuevo;
		
		nuevo.posAgente = it.SiguienteSignificado();
		nuevo.cantSanciones = 0;
		nuevo.cantHippiesAtrapados = 0;
		nuevo.hippiesMasCercanos = Conj<Posicion>();
		nuevo.mismasSanciones = itMismasSanciones;
		nuevo.mismaSancion = itSancion;
		nuevo.itAux = itAgente; 
		
		agentes.Definir(it.SiguienteClave(),nuevo);

		conKSanciones.AgregarAtras(it.SiguienteClave());

		it.Avanzar();

	}

} //comenzarRastrillaje


void CampusSeguro::IngresaEstudiante( const Nombre& e , const Posicion& p){

	matrizDeChabones[p.x][p.y].esHippieOEstudiante = true;
	matrizDeChabones[p.x][p.y].nombre = e;
	if(CantPersonasAlrededor(campus.Vecinos(p)).Hippies > 2 ){
		infoHippie nuevo = infoHippie();
		nuevo.posicion = p;
		nuevo.itAux = hippiesAux.Agregar(e);
		hippies.Definir(e,nuevo);
		ModificarVecinos(p, campus.Vecinos(p));
		if(TotalOcupados(CantPersonasAlrededor(campus.Vecinos(p))) == campus.Vecinos(p).Cardinal() && CantPersonasAlrededor(campus.Vecinos(p)).Seguridad == 1){
			
			SumarHippieAAgente(campus.Vecinos(p));
			hippies.Significado(e).itAux.EliminarSiguiente();
			hippies.Borrar(e);
			matrizDeChabones[p.x][p.y].esHippieOEstudiante = false;	
		}
	}else if(TotalOcupados(CantPersonasAlrededor(campus.Vecinos(p))) == campus.Vecinos(p).Cardinal() && CantPersonasAlrededor(campus.Vecinos(p)).Seguridad == 1){

		infoEstudiante nuevo = infoEstudiante();
		nuevo.posicion = p;
		nuevo.itAux = estudiantesAux.Agregar(e);
		estudiantes.Definir(e,nuevo);
		SumarSancion(campus.Vecinos(p));
		ModificarVecinos(p, campus.Vecinos(p));
	}

}

void CampusSeguro::IngresaHippie(const Nombre& h , const Posicion& p){

	infoHippie nuevo = infoHippie();
	nuevo.posicion = p;
	nuevo.itAux = hippiesAux.Agregar(h);
	hippies.Definir(h,nuevo);
	matrizDeChabones[p.x][p.y].esHippieOEstudiante = true;
	matrizDeChabones[p.x][p.y].nombre = h;
	
	if(TotalOcupados(CantPersonasAlrededor(campus.Vecinos(p))) < campus.Vecinos(p).Cardinal()){
		ModificarVecinos(p,campus.Vecinos(p));
	}else if(CantPersonasAlrededor(campus.Vecinos(p)).Estudiantes + CantPersonasAlrededor(campus.Vecinos(p)).Objetos == campus.Vecinos(p).Cardinal() ){
			hippies.Significado(h).itAux.EliminarSiguiente();
			hippies.Borrar(h);
			infoEstudiante nuevo = infoEstudiante();
			nuevo.posicion = p;
			nuevo.itAux = estudiantesAux.Agregar(h);
			estudiantes.Definir(h,nuevo);
			ModificarVecinos(p,campus.Vecinos(p))
	      }else if(CantPersonasAlrededor(campus.Vecinos(p)).Seguridad + CantPersonasAlrededor(campus.Vecinos(p)).Objetos == campus.Vecinos(p).Cardinal() ){
			sumarHippieAAgente(campus.Vecinos(p));
			hippies.Significado(h).itAux.EliminarSiguiente();
			hippies.Borrar(h);
			matrizDeChabones[p.x][p.y].esHippieOEstudiante = true;		
		}else{
		     	ModificarVecinos(p,campus.Vecinos(p))
                     }

}

void CampusSeguro::MoverEstudiante(const Nombre& e , const Direccion& d ){

	Posicion viejaPos = estudiantes.Significado(e).posicion,
	Posicion nuevaPos = proxPosicion(viejaPos,d);
	matrizDeChabones[viejaPos.x][viejaPos.y].esHippieOEstudiante = false;
	estudiantes.Significado(e).posicion = nuevaPos;	
	matrizDeChabones[nuevaPos.x][nuevaPos.y].esHippieOEstudiante = true;
	matrizDeChabones[nuevaPos.x][nuevaPos.y].nombre = e;
	if(cantPersonasAlrededor(*(campus.Vecinos(nuevaPos))).H >=2){
		estudiantes.Significado(e).itAux.EliminarSiguiente();
		estudiantes.Borrar(e);
		infoHippie nuevo = new infoHippie();
		nuevo.posicion = nuevaPos;
		nuevo.itAux = hippiesAux.AgregarAtras(e);
		ModificarVecinos(nuevaPos,*(campus.Vecinos(nuevaPos)));	
	}else{
		ModificarVecinos(nuevaPos,*(campus.Vecinos(nuevaPos)));
	     }
}
//revisar
void CampusSeguro::MoverHippie (const Nombre& h){

	infoHippie datosHippie = hippies.Significado(h);
	Posicion p = datosHippie.posicion;
	Conj<infoEstudiante> datosEstudiante = estudiantes.Significado; // no devería tomar las claves?
	Conj<infoEstudiante> :: iterador itDatosEstudiante = datosEstudiante.CrearIt();
	Conj<Posicion> posicionesEstudiantes = Conj<Posicion>();
	while(itDatosEstudiante.haySiguiente()){
		posicionesEstudiantes.Agregar(itDatosEstudiante.Siguiente());
	}
	Conj<Posicion> estudiantesMasCercanos = posicionesMasCercanas(p,posicionesEstudiantes);
	datosHippies.estudidantesMasCercanos = estudiantesMasCercanos;
	Posicion proximaPosicion = ingresosMasCercanos(p);
	if(estudiantesMasCercanos.Cardinal() > 0){
		proximaPosicion = damePos(p,estudiantesMasCercanos.CrearIt().Siguiente());
	}
	if(!EstaOcupada(proximaPosicion)){
		matrizDeChabones[proximaPosicion.x][proximaPosicion.y].esHippieOEstudiante = true;
		matrizDeChabones[proximaPosicion.x][proximaPosicion.y].nombre = h;
		matrizDeChabones[p.x][p.y].esHippieOEstudiante = false;
		datosHippie.posicion = proximaPosicion;
		ModificarVecinos(proximaPosicion,*(campus.Vecinos(proximaPosicion)))
	}
}

void CampusSeguro::MoverAgente(const Agente& a){
	infoAgente datosAgente = new agentes.Significado(a);
	Posicion p = datosAgentes.posicion;
	Conj<infoHippie>::iterador itDatosHippies = datosHippies.CrearIt();
	Conj<Posicion> posicionesHippies = new Conj<Posicion>();
	while(itDatosHippies.HaySiguiente()){
		posicionesHippies.Agregar(itDatosHippies.Siguiente().posicion);
		itDatosHippies.AvanzarIt();
	}
	Conj<Posicion> hippiesMasCercanos = personasMasCercanas(p,posicionesHippies);
	datosAgentes.hippiesMasCercanos = hippiesMasCercanos;
	Posicion proximaPosicion = campus.ingresosMasCercanos(p);
	if(hippiesMasCercanos.longitud() > 0){
		proximaPosicion = damePos(p,hippiesMascercanos.CrearIt().Siguiente());
	}
	if(!EstaOcupada(proximaPosicion)){
		matrizDeChabones[proximaPosicion.x][proximaPosicion.y].esAgente = true;
		matrizDeChabones[proximaPosicion.x][proximaPosicion.y].agente = a;
		datosHippie.posicion = proximaPosicion;
		ModificarVecinos(proximaPosicion,*(campus.Vecinos(proximaPosicion)));
	}

}

const Campus& CampusSeguro::DameCampus() const{
	return campus;

}

const ItConj<Nombre>& CampusSeguro::Estudiantes() const{
	return estudiantesAux.CrearIt();

}

const ItConj<Nombre>& CampusSeguro::Hippies() const{
	return hippiesAux.CrearIt();

}

const ItConj<Agente>& CampusSeguro::Agentes() const{
	return agentesAux.CrearIt();

}

const Posicion& CampusSeguro::PosicionEstudianteYHippie(const Nombre& n) const{
	if(hippies.Definido(n)){
		return hippies.Significado(n).posicion;
	}else{
		return estudiantes.Significado(n).posicion;
	}

}

const Posicion& CampusSeguro::PosicionAgente(const Agente& a) const{
	return agentes.Significado(a).posicion;

}

Nat CampusSeguro::CantSanciones(const Agente& a) const{
	return agentes.Significado(a).cantSanciones;

}

Nat CampusSeguro::CantHippiesAtrapados(const Agente& a) const{
	return agentes.Significado(a).cantHippiesAtrapados;

}

Nat CampusSeguro::CantHippies() const{
	return longitud(hippiesAux);

}

Nat CampusSeguro::CantEstudiantes() const{
	return longitus(estudiantesAux);

}

const Agente& CampusSeguro::MasVigilante() const{
	return masVigilante;

}


//funciones auxiliares;


void CampusSeguro::Sancionar(Nat p, Nat cs){

	infoAgente agente = agentes.Significado(cs);
	Lista<Conj<Agente>>::iterador it = agente.mismasSancione;
	Nat i = 0;
	while(i < cs){
		if(!it.HaySiguiente()){
			it = Conj().AgregarAtras(mismasSanciones);
		}else{
			it.Avanzar();
		}
		i++;
	}
	Conj<Agente>::iterador itMismasSanciones = it.Agregar(p);
	agente.mismaSancion.EliminarSiguiente();
	agente.cantSanciones + = cs;
	agente.mismasSanciones = it;
	hubieronSanciones = true ;

}

Conj<Agente> CampusSeguro::ConKSanciones(Nat k){
	Conj<Agente>::iterador it = agentesAux.CrearIt();
	if(hubieronSanciones){
		conKSanciones = Vector<Agente>();
		while(it.HaySiguiente()){
			insertarOrdenado(conKSanciones,it.Siguiente());
			it.Avanzar();
		}
	}
	return busquesaRapida(k,conKSanciones);
}

void CampusSeguro::InsertarOrdenado(Vector<Agente> v, const Agente& a){
	Nat i = 1;
	bool posicionEncontrada = false;
	while(i <= v.Longitud() && != posicionEncontrada){
		Nat sancionesAgente = agentes.Significado(v[i]).cantSanciones;
		if(agentes.Significado(a).cantSanciones <= sancionesAgente){
			v.Agregar(i,a);
			posicionEncontrada = true;
		}
		i++;
	}
	if(!=posicionEncontrada){
		v.AgregarAtras(a);
	}
}

Conj<Agente> CampusSeguro::BusquedaRapida(Nat n , Vector<Agente> v){
	buscar(v,0,v.Longitud()-1,n);
}

Conj<Agente> CampusSeguro::Buscar(Vector<Agente> v, Nat i ,Nat s ,Nat k){
	if(v.Longitud() == 1){
		if(agentes.Significado(v[0]).cantSanciones == k){
			return conMismasSanciones(v[0]);
		}else{
			return Conj<Agente>();
		}
	}else {
		int intermedio = (int)((i+s)/2);
		if(agentes.Significado(v[intermedio]).cantSanciones < k){
			buscar(v,i,intermedio,k);
		}else{
			buscar(v,intermedio,s,k);
		}
	}
}

Conj<Posicion> CampusSeguro::PosicionesMasCercanas(const Posicion& p, Conj<Posicion> c){
	Conj<Posicion> posicionesMasCercanas = Conj<Posicion>();
	Conj<Posicion>::iterador itPosiciones = c.CrearIt();
	if(itPosiciones.HaySiguiente()){
		Nat distanciaMasCercana = itPosiciones.Siguiente();
		while(itPosiciones.HaySiguiente()){
			Nat distancia = distancia(itPosiciones.Siguiente(),p);
			if(distancia < distanciaMasCercana){
				distanciaMasCercana = distancia;
				posicionesCercanas.Agregar(it.Siguiente());
			}else if(distancia == distanciaMasCercana){
					posicionesMasCercanas.Agregar(it.Siguiente());
				}
			it.Avanzar();
		}
	}
}

bool CampusSeguro::EstaOcupada(Posicion p){
	return campus.EstaOcupada(p) || matrizDeChabones[p.x][p.y].esHippieOEstudiante || matrizDeChabones[p.x][p.y].esAgente;
}

void CampusSeguro::ModificarVecinos(const Posicion& p, const Conj<Posicion>& c){
	Conj<Posicion>::interador it = c.CrearIt();
	while( it.HaySiguiente()){
		if(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].esHippieOEstudiante){
			if(matrizDeChabones[p.x][p.y].esHippieOEstudiante){
				modificarAux(matrizDeChabones[p.x][p.y].nombre,matrizDeChabones[it.Siguiente().x][it.Siguiente().y].nombre);
			}
			it.Avanzar();
		}else if(matrizDeChabones[p.x][p.y].esAgente && hipppies.Definido(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].nombre)){
				capturadoH(it.Siguiente());
				it.Avanzar();
			}else{
				capturadoE(it.Siguiente());
				it.Avanzar();
			}
		it.Avanzar();
	}
}

void CampusSeguro::ModificarAux(const Nombre& n1, const Nombre& n2){
	if(estudiantes.Definido(n2)){
		if(estudiantes.Definido(n1)){
			capturadoE(*(campus.Vecinos(estudiantes.Significado(n2).posicion)));
		}else if(hippies.Definido(n1)){
				convertidoYCapturado(n2,estudiantes.Significado(n2).posicion);
			}else{
				capturadoE(*(campus.Vecinos(estudiantes.Significado(n2).posicion)));
			}
	}else if(estudiantes.Definido(n1)){
			corregidoYcapturado(n2,hippies.Significado(n2).posicion);
		}else{
			capturadoH(n2,hippies.Significado(n2).posicion);
		}
}

void CampusSeguro::CapturadoE(const Posicion& p){
	if(totalOcupados(cantPersonarAlrededor(*(campus.Vecinos(p)))) == longitud(*(campus.Vecinos(p)))){
		sumarSancion(*(campus.Vecinos(p)));
}

void CampusSeguro::CapturadoH(const Nombre& n,const Posicion& p){
	if(totalOcupados(cantPersonarAlrededor(*(campus.Vecinos(estudiantes.Significado(p).posicion)))) == longitud(*(campus.Vecinos(estudiantes.Significado(p).posicion))) && cantPersonasAlrededor(*(campus.Vecinos(estudiantes.Significado(p).posicion))).S >= 1){
		sumarHippieAAgente(*(campus.Vecinos(estudiantes.Significado(p).posicion)));
		matrizDeChabones[p.x][p.y].esHippieOEstudiante = false;
		hippies.Significado(n).itAux.EliminarSiguiente();
		hippies.Borrar(n);
	}

}

void CampusSeguro::CorregidoYcapturado(const Nombre& n, const Posicion& p){
	if(cantPersonasAlrededor(*(campus.Vecinos(p))).E + cantPersonasAlrededor(*(campus.Vecinos(p)).O == longitud(*(campus.Vecinos(p)))){
		hippies.Significado(n).itAux.EliminarSiguiente();
		hippies.Borrar(n);
		infoEstudiantes nuevo = new infoEstudiantes();
		nuevo.posicion = p;
		nuevo.itAux = estudiantesAux.AgregarAtras(n);
		
		capturadoH(*(campus.Vecinos(p)));
	}
}

void CampusSeguro::ConvertidoYcapturado(const Nombre& n, const Posicion& p){
	if(cantPersonasAlrededor(*(campus.Vecinos(p))).H >=2){
		infoHippies nuevo = new infoHippies();
		nuevo.posicion = p;
		nuevo.itAux = hippiesAux.AgregarAtras();
		ModificarVecinos(p,*(campus.Vecinos(p)));
		if(totalOcupados(cantPersonasAlrededor(*(campus.Vecinos(p)))) == longitud(*(campus.Vecinos(p))) && cantPersonasAlrededor(*(campus.Vecinos(p))).S == 1){
			sumasHippieAAgente(*(campus.Vecinos(p)));
			hippies.Significado(n).itAux.EliminarSiguiente();
			hippies.Borrar(n);CampusSeguro
			matrizDeChabones[p.x][p.y].esHippieOEstudiante = false;
		}
	}else if(totalOcupados(cantPersonasAlrededor(*(campus.Vecinos(p)))) == longitud(*(campus.Vecinos(p))) && cantPersonasAlrededor(*(campus.Vecinos(p))).S == 1){
			sumarSancion(*(campus.Vecinos(p));
	}
}

void CampusSeguro::SumarSancion(const Conj<Posicion>& c){
	Conj<Posicion>::iterador it = c.CrearIt();
	int n = agentes.Significado(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].placa).cantSanciones;
	int m = matrizDeChabones[itSiguiente().x][itSiguiente().y].placa;
	while(it.HaySiguinte()){
		if(matrizDeChabones[in.Siguiente().x][it.Siguiente.y].esAgente){
			sancionar(m,1);
			it.Avanzar();
		}	
		it.Avanzar();
	}
}

void CampusSeguro::SumarHippieAAgente(const Conj<Posicion>& c){
	Conj<Posicion>::iterador it = c.CrearIt();
	while(it.HaySiguiente()){
		if(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].esAgente){
			infoAgente agente = agentes.Significado(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].placa);
			agente.cantHippiesAtrapados++;
			if(agente.cantHippiesAtrapados > agentes.Significado(masVigilante).cantHippiesAtrapados){
				masVigilante = matrizDeChabones[it.Siguiente().x][it.Siguiente().y].placa;
			}
		}
		it.Avanzar();
	}
}

/*tuple CampusSeguro::cantPersonasAlrededor(const Conj<Posicion>& c){
	Conj<Posicion>::iterador it =c.CrearIt();
	Nat e = 0;
	Nat h = 0;
	Nat s = 0;
	Nat o = 0;
	while(it.HaySiguiente()){
		if(campus[it.Siguiente().x][it.Siguiente().y]){
			if(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].esHippieOEstudiante){
				if(estudiantes.Definido(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].nombre)){
					e++;
					it.Avanzar();
				}
				if(hippies.Definido(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].nombre){
					h++;
					it.Avanzar();
				}
				if(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].agente){
					s++;
					it.Avanzar();
				}else{
					o++;
					it.Avanzar();
				}
			}
		}
		it.Avanzar();

	}
	tup = (s,e,h,o);
	E,S,H,O = tup;
	return tup;
}*/

Nat CampusSeguro::TotalOcupados(tuple t){
	return t.O+t.H+t.E+t.S;
}

Posicion CampusSeguro::DamePos(const Posicion& p1,const Posicion& p2){
	if(p1.x > p2.x){
		return <p1.x-1,p1.y>;
	}
	if(p1.x < p2.x){
		return <p1.x+1,p1.y>;
	}
	if(p1.y > p2.y){
		return <p1.x,p1.y-1>;
	}
	if(p1.y < p2.y){
		return <p1.x,p1.y+1>;
	}
}




CampusSeguro::~CampusSeguro(){
	
	conKSanciones.~Vector<Agente>();
	matrizDeChabones.~Vector<Vector<infoChabones>>();
	agentes.~DiccRapido<Agente,infoAgente>();
	estudiantes.~DiccString<infoEstudiante>();
	hippies.~DiccString<infoHippie>();

}
