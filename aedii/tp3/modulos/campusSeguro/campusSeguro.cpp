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
	
	if(TotalOcupados(CantPersonasAlrededor(campus.Vecinos(p))) < campus.Vecinos(p).Cardinal()) {
		ModificarVecinos(p,campus.Vecinos(p));
	} else if(CantPersonasAlrededor(campus.Vecinos(p)).Estudiantes + CantPersonasAlrededor(campus.Vecinos(p)).Objetos == campus.Vecinos(p).Cardinal() ) {
			hippies.Significado(h).itAux.EliminarSiguiente();
			hippies.Borrar(h);
			infoEstudiante nuevo = infoEstudiante();
			nuevo.posicion = p;
			nuevo.itAux = estudiantesAux.Agregar(h);
			estudiantes.Definir(h,nuevo);
			ModificarVecinos(p,campus.Vecinos(p));
	      } else if(CantPersonasAlrededor(campus.Vecinos(p)).Seguridad + CantPersonasAlrededor(campus.Vecinos(p)).Objetos == campus.Vecinos(p).Cardinal() ) {
					SumarHippieAAgente(campus.Vecinos(p));
					hippies.Significado(h).itAux.EliminarSiguiente();
					hippies.Borrar(h);
					matrizDeChabones[p.x][p.y].esHippieOEstudiante = true;		
				} else {
		     		ModificarVecinos(p,campus.Vecinos(p));
                }
}

void CampusSeguro::MoverEstudiante(const Nombre& e , const Direccion& d ){

	Posicion viejaPos(estudiantes.Significado(e).posicion);
	Posicion nuevaPos(campus.ProxPosicion(viejaPos,d));
	matrizDeChabones[viejaPos.x][viejaPos.y].esHippieOEstudiante = false;
	estudiantes.Significado(e).posicion = nuevaPos;	
	matrizDeChabones[nuevaPos.x][nuevaPos.y].esHippieOEstudiante = true;
	matrizDeChabones[nuevaPos.x][nuevaPos.y].nombre = e;
	if(CantPersonasAlrededor(campus.Vecinos(nuevaPos)).Hippies >=2){
		estudiantes.Significado(e).itAux.EliminarSiguiente();
		estudiantes.Borrar(e);
		infoHippie nuevo = infoHippie();
		nuevo.posicion = nuevaPos;
		nuevo.itAux = hippiesAux.Agregar(e);
		ModificarVecinos(nuevaPos, campus.Vecinos(nuevaPos));	
	}else{
		ModificarVecinos(nuevaPos, campus.Vecinos(nuevaPos));
	     }
}
//revisar
void CampusSeguro::MoverHippie (const Nombre& h){

	infoHippie datosHippie = hippies.Significado(h);
	Posicion p = datosHippie.posicion;
	Conj<infoEstudiante*> datosEstudiante = estudiantes.Significados();
	Conj<infoEstudiante*> :: Iterador itDatosEstudiante = datosEstudiante.CrearIt();
	Conj<Posicion> posicionesEstudiantes = Conj<Posicion>();
	while(itDatosEstudiante.HaySiguiente()){
		posicionesEstudiantes.Agregar(itDatosEstudiante.Siguiente()->posicion);
	}
	Conj<Posicion> estudiantesMasCercanos = PosicionesMasCercanas(p,posicionesEstudiantes);
	datosHippie.estudiantesMasCercanos = estudiantesMasCercanos;
	Posicion proximaPosicion = campus.IngresosMasCercanos(p).CrearIt().Siguiente(); //TODO. Revisar si esto está bien. Está mal diseñado, no coinciden los tipos. OJO. para que esto ande el conjunto debe ser no vacio
	if(estudiantesMasCercanos.Cardinal() > 0){
		proximaPosicion = DamePos(p,estudiantesMasCercanos.CrearIt().Siguiente());
	}
	if(!EstaOcupada(proximaPosicion)){
		matrizDeChabones[proximaPosicion.x][proximaPosicion.y].esHippieOEstudiante = true;
		matrizDeChabones[proximaPosicion.x][proximaPosicion.y].nombre = h;
		matrizDeChabones[p.x][p.y].esHippieOEstudiante = false;
		datosHippie.posicion = proximaPosicion;
		ModificarVecinos(proximaPosicion, campus.Vecinos(proximaPosicion));
	}
}

void CampusSeguro::MoverAgente(const Agente& a){
	infoAgente datosAgente = agentes.Significado(a);
	Posicion p(datosAgente.posAgente);
	Conj<infoHippie*> datosHippies = hippies.Significados();
	Conj<infoHippie*>::Iterador itDatosHippies = datosHippies.CrearIt();
	Conj<Posicion> posicionesHippies = Conj<Posicion>();
	while(itDatosHippies.HaySiguiente()){
		posicionesHippies.Agregar(itDatosHippies.Siguiente()->posicion);
		itDatosHippies.Avanzar();
	}
	Conj<Posicion> hippiesMasCercanos = PosicionesMasCercanas(p,posicionesHippies);
	datosAgente.hippiesMasCercanos = hippiesMasCercanos;
	Posicion proximaPosicion = campus.IngresosMasCercanos(p).CrearIt().Siguiente();//TODO. Revisar si esto está bien. Está mal diseñado, no coinciden los tipos. OJO. para que esto ande el conjunto debe ser no vacio
	if(hippiesMasCercanos.Cardinal() > 0){
		proximaPosicion = DamePos(p,hippiesMasCercanos.CrearIt().Siguiente());
	}
	if(!EstaOcupada(proximaPosicion)){
		matrizDeChabones[proximaPosicion.x][proximaPosicion.y].esAgente = true;
		matrizDeChabones[proximaPosicion.x][proximaPosicion.y].agente = a;
		//datosHippies.posicion = proximaPosicion; //TODO. No tiene sentido pedirle la posicion a un conjunto. ¿A que hippie hay que actualizarle la posicion?
		ModificarVecinos(proximaPosicion, campus.Vecinos(proximaPosicion));
	}

}

const Campus& CampusSeguro::DameCampus() const{
	return campus;

}

const Conj<Nombre>::const_Iterador CampusSeguro::Estudiantes() const{
	return estudiantesAux.CrearIt();

}

const Conj<Nombre>::const_Iterador CampusSeguro::Hippies() const{
	return hippiesAux.CrearIt();

}

const Conj<Agente>::const_Iterador CampusSeguro::Agentes() const{
	return agentesAux.CrearIt();

}

const Posicion& CampusSeguro::PosicionEstudianteYHippie(const Nombre& n) {
	if(hippies.Definido(n)){
		return hippies.Significado(n).posicion;
	}else{
		return estudiantes.Significado(n).posicion;
	}

}

const Posicion& CampusSeguro::PosicionAgente(const Agente& a) {
	return agentes.Significado(a).posAgente;

}

Nat CampusSeguro::CantSanciones(const Agente& a) const{
	return agentes.Significado(a).cantSanciones;

}

Nat CampusSeguro::CantHippiesAtrapados(const Agente& a) const{
	return agentes.Significado(a).cantHippiesAtrapados;

}

Nat CampusSeguro::CantHippies() const{
	return hippiesAux.Cardinal();

}

Nat CampusSeguro::CantEstudiantes() const{
	return estudiantesAux.Cardinal();

}

const Agente& CampusSeguro::MasVigilante() const{
	return masVigilante;

}


//funciones auxiliares;

void CampusSeguro::Sancionar(Nat p, Nat cs){

	infoAgente agente = agentes.Significado(cs);
	Lista<Conj<Agente>>::Iterador it = agente.mismasSanciones;
	Nat i = 0;
	while(i < cs){
		if(!it.HaySiguiente()){
			it = mismasSanciones.AgregarAtras(Conj<Agente>());
		}else{
			it.Avanzar();
		}
		i++;
	}
	Conj<Agente>::Iterador itMismasSanciones = it.Siguiente().Agregar(p);
	agente.mismaSancion.EliminarSiguiente();
	agente.cantSanciones = agente.cantSanciones + cs;
	agente.mismasSanciones = it;
	agente.mismaSancion = itMismasSanciones;
	hubieronSanciones = true ;

}

Conj<Agente> CampusSeguro::ConKSanciones(Nat k){
	Conj<Agente>::Iterador it = agentesAux.CrearIt();
	if(hubieronSanciones){
		conKSanciones = Vector<Agente>();
		while(it.HaySiguiente()){
			insertarOrdenado(conKSanciones,it.Siguiente());
			it.Avanzar();
		}
	}
	return BusquedaRapida(k,conKSanciones);
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
			Buscar(v,i,intermedio,k);
		}else{
			Buscar(v,intermedio,s,k);
		}
	}
}

Conj<Posicion> CampusSeguro::PosicionesMasCercanas(const Posicion& p, Conj<Posicion> c){
	Conj<Posicion> posicionesMasCercanas = Conj<Posicion>();
	Conj<Posicion>::Iterador itPosiciones = c.CrearIt();
	if(itPosiciones.HaySiguiente()){
		Nat distanciaMasCercana = itPosiciones.Siguiente();
		while(itPosiciones.HaySiguiente()){
			Nat distancia = distancia(itPosiciones.Siguiente(),p);
			if(distancia < distanciaMasCercana){
				distanciaMasCercana = distancia;
				posicionesMasCercanas.Agregar(itPosiciones.Siguiente());
			}else if(distancia == distanciaMasCercana){
					posicionesMasCercanas.Agregar(itPosiciones.Siguiente());
				}
			itPosiciones.Avanzar();
		}
	}
}

bool CampusSeguro::EstaOcupada(Posicion p) {
	return campus.EstaOcupada(p) || matrizDeChabones[p.x][p.y].esHippieOEstudiante || matrizDeChabones[p.x][p.y].esAgente;
}

void CampusSeguro::ModificarVecinos(const Posicion& p, const Conj<Posicion>& c){
	Conj<Posicion>::Iterador it = c.CrearIt();
	while(it.HaySiguiente()){
		if(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].esHippieOEstudiante){
			if(matrizDeChabones[p.x][p.y].esHippieOEstudiante){
				modificarAux(matrizDeChabones[p.x][p.y].nombre,matrizDeChabones[it.Siguiente().x][it.Siguiente().y].nombre);
			}
			it.Avanzar();
		}else if(matrizDeChabones[p.x][p.y].esAgente && hipppies.Definido(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].nombre)){
				CapturadoH(it.Siguiente());
				it.Avanzar();
			}else{
				CapturadoE(it.Siguiente());
				it.Avanzar();
			}
		it.Avanzar();
	}
}

void CampusSeguro::ModificarAux(const Nombre& n1, const Nombre& n2){
	if(estudiantes.Definido(n2)){
		if(estudiantes.Definido(n1)){
			CapturadoE(campus.Vecinos(estudiantes.Significado(n2).posicion));
		}else if(hippies.Definido(n1)){
				ConvertidoYCapturado(n2,estudiantes.Significado(n2).posicion);
			}else{
				CapturadoE(campus.Vecinos(estudiantes.Significado(n2).posicion));
			}
	}else if(estudiantes.Definido(n1)){
			CorregidoYcapturado(n2,hippies.Significado(n2).posicion);
		}else{
			CapturadoH(n2,hippies.Significado(n2).posicion);
		}
}

void CampusSeguro::CapturadoE(const Posicion& p){
	if(TotalOcupados(CantPersonarAlrededor(campus.Vecinos(p))) == campus.Vecinos(p).Cardinal())
		SumarSancion(campus.Vecinos(p));
}

void CampusSeguro::CapturadoH(const Nombre& n,const Posicion& p){
	if(TotalOcupados(CantPersonasAlrededor(campus.Vecinos(estudiantes.Significado(p).posicion))) == campus.Vecinos(estudiantes.Significado(p).posicion).Cardinal() && CantPersonasAlrededor(campus.Vecinos(estudiantes.Significado(p).posicion)).Seguridad >= 1){
		SumarHippieAAgente(campus.Vecinos(estudiantes.Significado(p).posicion));
		matrizDeChabones[p.x][p.y].esHippieOEstudiante = false;
		hippies.Significado(n).itAux.EliminarSiguiente();
		hippies.Borrar(n);
	}
}

void CampusSeguro::CorregidoYcapturado(const Nombre& n, const Posicion& p){
	if(CantPersonasAlrededor(campus.Vecinos(p)).Estudiantes + CantPersonasAlrededor(campus.Vecinos(p)).Objetos == campus.Vecinos(p).Cardinal()) {
		hippies.Significado(n).itAux.EliminarSiguiente();
		hippies.Borrar(n);
		infoEstudiante nuevo();
		nuevo.posicion(p);
		nuevo.itAux = estudiantesAux.Agregar(n);
		
		CapturadoH(campus.Vecinos(p));
	}
}

void CampusSeguro::ConvertidoYcapturado(const Nombre& n, const Posicion& p){
	if(CantPersonasAlrededor(campus.Vecinos(p)).Hippies >=2){
		infoHippie nuevo();
		nuevo.posicion(p);
		nuevo.itAux = hippiesAux.Agregar();
		ModificarVecinos(p,campus.Vecinos(p));
		if(TotalOcupados(CantPersonasAlrededor(campus.Vecinos(p))) == campus.Vecinos(p).Cardinal() && CantPersonasAlrededor(campus.Vecinos(p)).Seguridad == 1){
			SumarHippieAAgente(campus.Vecinos(p));
			hippies.Significado(n).itAux.EliminarSiguiente();
			hippies.Borrar(n);
			matrizDeChabones[p.x][p.y].esHippieOEstudiante = false;
		}
	}else if(TotalOcupados(CantPersonasAlrededor(campus.Vecinos(p))) == campus.Vecinos(p).Cardinal() && CantPersonasAlrededor(campus.Vecinos(p)).Seguridad == 1){
			SumarSancion(campus.Vecinos(p));
	}
}

void CampusSeguro::SumarSancion(const Conj<Posicion>& c){
	Conj<Posicion>::const_Iterador it = c.CrearIt();
	//int n = agentes.Significado(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].placa).cantSanciones;
	int m = matrizDeChabones[it.Siguiente().x][it.Siguiente().y].agente;
	while(it.HaySiguiente()){
		if(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].esAgente){
			Sancionar(m,1);
			it.Avanzar();
		}	
		it.Avanzar();
	}
}

void CampusSeguro::SumarHippieAAgente(const Conj<Posicion>& c){
	Conj<Posicion>::const_Iterador it = c.CrearIt();
	while(it.HaySiguiente()){
		if(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].esAgente){
			infoAgente agente = agentes.Significado(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].agente);
			agente.cantHippiesAtrapados++;
			if(agente.cantHippiesAtrapados > agentes.Significado(masVigilante).cantHippiesAtrapados){
				masVigilante = matrizDeChabones[it.Siguiente().x][it.Siguiente().y].agente;
			}
		}
		it.Avanzar();
	}
}

/*tuple CampusSeguro::CantPersonasAlrededor(const Conj<Posicion>& c){
	Conj<Posicion>::Iterador it =c.CrearIt();
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

Nat CampusSeguro::TotalOcupados(infoEntorno t){
	return t.Objetos + t.Hippies + t.Estudiantes + t.Seguridad;
}

Posicion CampusSeguro::DamePos(const Posicion& p1,const Posicion& p2){
	Posicion p;

	if(p1.x > p2.x){
		p.x = p1.x - 1; p.y = p1.y;
	}
	if(p1.x < p2.x){
		p.x = p1.x + 1; p.y = p1.y;
	}
	if(p1.y > p2.y){
		p.x = p1.x; p.y = p1.y - 1;
	}
	if(p1.y < p2.y){
		p.x = p1.x; p.y = p1.y + 1;
	}

	return p;
}

CampusSeguro::~CampusSeguro(){
	
	conKSanciones.~Vector<Agente>();
	matrizDeChabones.~Vector<Vector<infoChabones>>();
	agentes.~DiccRapido<Agente,infoAgente>();
	estudiantes.~DiccString<infoEstudiante>();
	hippies.~DiccString<infoHippie>();

}
