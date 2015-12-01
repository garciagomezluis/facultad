#include "campusSeguro.h"

/*CampusSeguro::CampusSeguro( const CampusSeguro& otro ){

	//agentes =  Dicc<Agente,infoAgente>(otro.agentes);
	//hippies =  Dicc(otro.hippies);
	//estudiantes =  Dicc(otro.estudiantes);
	matrizDeChabones = Vector<Vector <infoChabones> >(otro.matrizDeChabones);
	int pos = 0;
	while (pos < otro.matrizDeChabones.Longitud()) {
		matrizDeChabones[pos] = Vector<infoChabones>(otro.matrizDeChabones[pos]);
		pos++;
	}
	agentesAux =  Conj<Agente>(otro.agentesAux);
	hippiesAux =  Conj<Nombre>(otro.hippiesAux);
	estudiantesAux =  Conj<Nombre>(otro.estudiantesAux);
	mismasSanciones =  Lista <Conj <Agente> >();
	Lista <Conj <Agente> > :: const_Iterador it = otro.mismasSanciones.CrearIt();
	while (it.HaySiguiente()) {
		mismasSanciones.Agregar(Conj<Agente>(it.Siguiente()));
		it.Avanzar();
	}
	conKSanciones =  Vector<Agente>(otro.conKSanciones);
	campu = Campus(otro.campu.Columnas() , otro.campu.Filas());
	hubieronSanciones= otro.hubieronSanciones;	
	

} /// constructor por copia*/

CampusSeguro::CampusSeguro( const Campus& c, const Dicc<Agente , Posicion>& d ){

   	matrizDeChabones = Vector<  Vector<infoChabones> >();
	int i = 1;
	int j = 1;
	while( i < c.Filas()){
		while(j < c.Columnas()){
			infoChabones nuevo = infoChabones();
			matrizDeChabones[i][j] = nuevo;
			j++;
		}
		i++;
	}
	campu = Campus(c);
	estudiantesAux = Conj<Nombre>();
	hippiesAux = Conj<Nombre>();
	estudiante =  Dicc<Nombre,infoEstudiante>();
	hippie =  Dicc<Nombre,infoHippie>();
	agente = Dicc<Agente,infoAgente>();
	conKSanciones = Vector<Agente>();
	mismasSanciones = Lista< Conj<Agente> >();
	Lista< Conj<Agente> >::Iterador itMismasSanciones = mismasSanciones.CrearIt();
	agentesAux = Conj<Agente>(); 
	Dicc<Agente,Posicion>::const_Iterador it = d.CrearIt();
	while(it.HaySiguiente()){
		Conj<Agente>::Iterador itAgente = agentesAux.Agregar(it.SiguienteClave());
		Conj<Agente>::Iterador itSancion = itMismasSanciones.Siguiente().Agregar(it.SiguienteClave());
		
		infoAgente nuevo = infoAgente();
		
		nuevo.posAgente = it.SiguienteSignificado();
		nuevo.cantSanciones = 0;
		nuevo.cantHippiesAtrapados = 0;
		nuevo.hippiesMasCercanos =  Conj<Posicion>();
		nuevo.mismasSanciones = itMismasSanciones;
		nuevo.mismaSancion = itSancion;
		nuevo.itAux = itAgente; 
		
		agente.Definir(it.SiguienteClave(),nuevo);

		conKSanciones.AgregarAtras(it.SiguienteClave());

		it.Avanzar();

	}
	
	
	

} //comenzarRastrillaje


void CampusSeguro::ingresaEstudiante( const Nombre& e , const Posicion& p){

	matrizDeChabones[p.x][p.y].esHippieOEstudiante = true;
	matrizDeChabones[p.x][p.y].nombre = e;
	if(cantPersonasAlrededor(campu.Vecinos(p)).H > 2 ){
		infoHippie nuevo =  infoHippie();
		nuevo.posicion = p;
		nuevo.itAux = hippiesAux.Agregar(e);
		hippie.Definir(e,nuevo);
		modificarVecinos(p,campu.Vecinos(p));
		if(totalOcupados(cantPersonasAlrededor(campu.Vecinos(p))) == campu.Vecinos(p).Cardinal() && cantPersonasAlrededor(campu.Vecinos(p)).S == 1){
			
			sumarHippieAAgente(campu.Vecinos(p));
			hippie.Significado(e).itAux.EliminarSiguiente();
			hippie.Borrar(e);
			matrizDeChabones[p.x][p.y].esHippieOEstudiante = false;	
		}
	}else if(totalOcupados(cantPersonasAlrededor(campu.Vecinos(p))) == campu.Vecinos(p).Cardinal() && cantPersonasAlrededor(campu.Vecinos(p)).S == 1){

		infoEstudiante nuevo =  infoEstudiante();
		nuevo.posicion = p;
		nuevo.itAux = estudiantesAux.Agregar(e);
		estudiante.Definir(e,nuevo);
		sumarSancion(campu.Vecinos(p));
		modificarVecinos(p,campu.Vecinos(p));
	}

}

void CampusSeguro::ingresaHippie(const Nombre& h , const Posicion& p){
	
	infoHippie nuevo =  infoHippie();
	nuevo.posicion = p;
	nuevo.itAux = hippiesAux.Agregar(h);
	hippie.Definir(h,nuevo);
	matrizDeChabones[p.x][p.y].esHippieOEstudiante = true;
	matrizDeChabones[p.x][p.y].nombre = h;
	
	Tupla tup = cantPersonasAlrededor(campu.Vecinos(p));
	
	if(totalOcupados(cantPersonasAlrededor(campu.Vecinos(p))) < campu.Vecinos(p).Cardinal()){
		modificarVecinos(p,campu.Vecinos(p));
	}else if((tup.E + tup.O) == campu.Vecinos(p).Cardinal()){
			hippie.Significado(h).itAux.EliminarSiguiente();
			hippie.Borrar(h);
			infoEstudiante nuevo =  infoEstudiante();
			nuevo.posicion = p;
			nuevo.itAux = estudiantesAux.Agregar(h);
			estudiante.Definir(h,nuevo);
			modificarVecinos(p,campu.Vecinos(p));
	      }else if(tup.S + tup.O == campu.Vecinos(p).Cardinal()){
			sumarHippieAAgente(campu.Vecinos(p));
			hippie.Significado(h).itAux.EliminarSiguiente();
			hippie.Borrar(h);
			matrizDeChabones[p.x][p.y].esHippieOEstudiante = true;		
		}else{
		     	modificarVecinos(p,campu.Vecinos(p));
                     }

}

void CampusSeguro::moverEstudiante(const Nombre& e , const Direccion& d ){

	Posicion viejaPos = estudiante.Significado(e).posicion;
	Posicion nuevaPos = proxPosicion(viejaPos,d);
	matrizDeChabones[viejaPos.x][viejaPos.y].esHippieOEstudiante = false;
	estudiante.Significado(e).posicion = nuevaPos;	
	matrizDeChabones[nuevaPos.x][nuevaPos.y].esHippieOEstudiante = true;
	matrizDeChabones[nuevaPos.x][nuevaPos.y].nombre = e;
	if(cantPersonasAlrededor(campu.Vecinos(nuevaPos)).H >=2){
		estudiante.Significado(e).itAux.EliminarSiguiente();
		estudiante.Borrar(e);
		infoHippie nuevo = infoHippie();
		nuevo.posicion = nuevaPos;
		nuevo.itAux = hippiesAux.Agregar(e);
		modificarVecinos(nuevaPos,campu.Vecinos(nuevaPos));	
	}else{
		modificarVecinos(nuevaPos,campu.Vecinos(nuevaPos));
	     }
}
//revisar
/*void CampusSeguro::moverHippie (const Nombre& h){

	infoHippie datosHippie = hippie.Significado(h);
	Posicion p = datosHippie.posicion;
	Conj<infoEstudiante> datosEstudiante = estudiante.Significados(estudiante); 
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
	if(!estaOcupada?(proximaPosicion)){
		matrizDeChabones[proximaPosicion.x][proximaPosicion.y].esHippieOEstudiante = true;
		matrizDeChabones[proximaPosicion.x][proximaPosicion.y].nombre = h;
		matrizDeChabones[p.x][p.y].esHippieOEstudiante = false;
		datosHippie.posicion = proximaPosicion;
		modificarVecinos(proximaPosicion,campus.Vecinos(proximaPosicion))
	}
}

void CampusSeguro::moverAgente(const Agente& a){
	infoAgente datosAgente = agente.Significado(a);
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
	if(hippiesMasCercanos.Cardinal() > 0){
		proximaPosicion = damePos(p,hippiesMascercanos.CrearIt().Siguiente());
	}
	if(!estaOcupada(proximaPosicion)){
		matrizDeChabones[proximaPosicion.x][proximaPosicion.y].esAgente = true;
		matrizDeChabones[proximaPosicion.x][proximaPosicion.y].agente = a;
		datosHippie.posicion = proximaPosicion;
		modificarVecinos(proximaPosicion,campus.Vecinos(proximaPosicion));
	}

}*/

const Campus& CampusSeguro::campus() const{
	return campu;

}

const Conj<Nombre>::Iterador& CampusSeguro::estudiantes() const{
	return estudiantesAux.const_Iterador();

}

const Conj<Nombre>::Iterador& CampusSeguro::hippies() const{
	return hippiesAux.CrearIt();

}

const Conj<Agente>::Iterador& CampusSeguro::agentes() const{
	return agentesAux

}

const Posicion& CampusSeguro::posicionEstudianteYHippie(const Nombre& n) const{
	if(hippie.Definido(n)){
		return hippie.Significado(n).posicion;
	}else{
		return estudiante.Significado(n).posicion;
	}

}

const Posicion& CampusSeguro::posicionAgente(const Agente& a) const{
	return agente.Significado(a).posAgente;

}

Nat CampusSeguro::cantSanciones(const Agente& a) const{
	return agente.Significado(a).cantSanciones;

}

Nat CampusSeguro::cantHippiesAtrapados(const Agente& a) const{
	return agente.Significado(a).cantHippiesAtrapados;

}

Nat CampusSeguro::cantHippies() const{
	return hippiesAux.Cardinal();

}

Nat CampusSeguro::cantEstudiantes() const{
	return estudiantesAux.Cardinal();

}

const Agente& CampusSeguro::masVigilante() const{
	return elmasVigilante;

}


//funciones auxiliares;

Posicion CampusSeguro::proxPosicion(Posicion p, const Direccion& d){}


void CampusSeguro::sancionar(const Nat& p, const Nat& cs){

	infoAgente ag = agente.Significado(cs);
	Lista<Conj<Agente> >::Iterador it = ag.mismasSanciones.CrearIt();
	Nat i = 0;
	while(i < cs){
		if( !it.HaySiguiente()){
			it = Conj<Lista <Conj <Agente> > >().Agregar(mismasSanciones);
		}else{
			it.Avanzar();
		}
		i++;
	}
	Conj<Agente>::Iterador itMismasSanciones = it.Agregar(p);
	ag.mismaSancion.EliminarSiguiente();
	ag.cantSanciones + = cs;
	ag.mismasSanciones = it;
	hubieronSanciones = true ;

}

Conj<Agente> CampusSeguro::KSanciones(Nat k){
	Conj<Agente>::Iterador it = agentesAux.CrearIt();
	if(hubieronSanciones){
		conKSanciones = Vector<Agente>();
		while(it.HaySiguiente()){
			insertarOrdenado(conKSanciones,it.Siguiente());
			it.Avanzar();
		}
	}
	return busquedaRapida(k,conKSanciones);
}

void CampusSeguro::insertarOrdenado(const Vector<Agente>& v, const Agente& a){
	Nat i = 1;
	bool posicionEncontrada = false;
	while(i <= v.Cardinal() && != posicionEncontrada){
		Nat sancionesAgente = agentes.Significado(v[i]).cantSanciones;
		if(agentes.Significado(a).cantSanciones <= sancionesAgente){
			v.Agregar(i,a);
			posicionEncontrada = true;
		}
		i++;
	}
	if(!=posicionEncontrada){
		v.Agregar(a);
	}
}

Conj<Agente> CampusSeguro::busquedaRapida(Nat n , Vector<Agente> v){
	buscar(v,0,v.Longitud()-1,n);
}

Conj<Agente> CampusSeguro::buscar(Vector<Agente> v, Nat i ,Nat s ,Nat k){
	if(v.Longitud() == 1){
		if(agente.Significado(v[0]).cantSanciones == k){
			return conMismasSanciones(v[0]);
		}else{
			return Conj<Agente>();
		}
	}else {
		int intermedio = (int)((i+s)/2);
		if(agente.Significado(v[intermedio]).cantSanciones < k){
			buscar(v,i,intermedio,k);
		}else{
			buscar(v,intermedio,s,k);
		}
	}
}

/*Conj<Posicion> CampusSeguro::posicionesMasCercanas(const Posicion& p, Conj<Posicion> c){
	Conj<Posicion> posMasCercanas = Conj<Posicion>();
	Conj<Posicion>::Iterador itPosiciones = c.CrearIt();
	if(itPosiciones.HaySiguiente()){
		int distanciaMasCercana = itPosiciones.Siguiente();
		while(itPosiciones.HaySiguiente()){
			Nat distancia = campu.Distancia(itPosiciones.Siguiente(),p);
			if(distancia < distanciaMasCercana){
				distanciaMasCercana = distancia;
				posMasCercanas.Agregar(it.Siguiente());
			}else if(distancia == distanciaMasCercana){
					posMasCercanas.Agregar(it.Siguiente());
				}
			it.Avanzar();
		}
	}
}*/

bool CampusSeguro::estaOcupada(Posicion p){
	return campu.EsOcupada(p) || matrizDeChabones[p.x][p.y].esHippieOEstudiante || matrizDeChabones[p.x][p.y].esAgente;
}

void CampusSeguro::modificarVecinos(const Posicion& p, Conj<Posicion> c){
	Conj<Posicion>::Interador it = c.CrearIt();
	while( it.HaySiguiente()){
		if(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].esHippieOEstudiante){
			if(matrizDeChabones[p.x][p.y].esHippieOEstudiante){
				modificarAux(matrizDeChabones[p.x][p.y].nombre,matrizDeChabones[it.Siguiente().x][it.Siguiente().y].nombre);
			}
			it.Avanzar();
		}else if(matrizDeChabones[p.x][p.y].esAgente && hippie.Definido(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].nombre)){
				capturadoH(it.Siguiente());
				it.Avanzar();
			}else{
				capturadoE(it.Siguiente());
				it.Avanzar();
			}
		it.Avanzar();
	}
}

void CampusSeguro::modificarAux(const Nombre& n1, const Nombre& n2){
	if(estudiante.Definido(n2)){
		if(estudiante.Definido(n1)){
			capturadoE(estudiante.Significado(n2).posicion);
		}else if(hippie.Definido(n1)){
				convertidoYCapturado(n2,estudiante.Significado(n2).posicion);
			}else{
				capturadoE(estudiante.Significado(n2).posicion);
			}
	}else if(estudiante.Definido(n1)){
			corregidoYcapturado(n2,hippie.Significado(n2).posicion);
		}else{
			capturadoH(n2,hippie.Significado(n2).posicion);
		}
}

void CampusSeguro::capturadoE(const Posicion& p){
	if(totalOcupados(cantPersonasAlrededor(campu.Vecinos(p)) == campu.Vecinos(p).Cardinal()){
		sumarSancion(campu.Vecinos(p));
}

void CampusSeguro::capturadoH(const Nombre& n,const Posicion& p){
	if(totalOcupados(cantPersonarAlrededor(campus.Vecinos(estudiantes.Significado(p).posicion))) == Cardinal(campus.Vecinos(estudiantes.Significado(p).posicion)) && cantPersonasAlrededor(campus.Vecinos(estudiantes.Significado(p).posicion)).S >= 1){
		sumarHippieAAgente(campus.Vecinos(estudiantes.Significado(p).posicion));
		matrizDeChabones[p.x][p.y].esHippieOEstudiante = false;
		hippies.Significado(n).itAux.EliminarSiguiente();
		hippies.Borrar(n);
	}

}

void CampusSeguro::corregidoYcapturado(const Nombre& n, const Posicion& p){
	if(cantPersonasAlrededor(campus.Vecinos(p)).E + cantPersonasAlrededor(campus.Vecinos(p)).O == campus.Vecinos(p).Cardinal)){
		hippies.Significado(n).itAux.EliminarSiguiente();
		hippies.Borrar(n);
		infoEstudiantes nuevo =  infoEstudiantes();
		nuevo.posicion = p;
		nuevo.itAux = estudiantesAux.Agregar(n);
		
		capturadoH(campus.Vecinos(p));
	}
}

void CampusSeguro::convertidoYcapturado(const Nombre& n, const Posicion& p){
	if(cantPersonasAlrededor(campus.Vecinos(p)).H >=2){
		infoHippies nuevo =  infoHippies();
		nuevo.posicion = p;
		nuevo.itAux = hippiesAux.Agregar();
		modificarVecinos(p,campus.Vecinos(p));
		if(totalOcupados(cantPersonasAlrededor(campus.Vecinos(p))) == campus.Vecinos(p).Cardinal() && cantPersonasAlrededor(campus.Vecinos(p)).S == 1){
			sumasHippieAAgente(campus.Vecinos(p));
			hippies.Significado(n).itAux.EliminarSiguiente();
			hippies.Borrar(n);
			matrizDeChabones[p.x][p.y].esHippieOEstudiante = false;
		}
	}else if(totalOcupados(cantPersonasAlrededor(campus.Vecinos(p))) == campus.Vecinos(p).Cardinal() && cantPersonasAlrededor(campus.Vecinos(p)).S == 1){
			sumarSancion(campus.Vecinos(p));
	}
}

void CampusSeguro::sumarSancion(Conj<Posicion> c){
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

void CampusSeguro::sumarHippieAAgente(Conj<Posicion> c){
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

Tupla CampusSeguro::cantPersonasAlrededor(Conj<Posicion> c){
	Tupla tup = tupla();
	Conj<Posicion>::iterador it = c.CrearIt();
	while(it.HaySiguiente()){
		if(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].esHIppieOEstudiante){
			if(estudiantes.Definido(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].nombre)){
				tup.E++;
				it.Avanzar();
				}
				if(hippies.Definido(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].nombre){
				tup.H++;
				it.Avanzar();
					}
			}else if(estudiantes.Definido(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].agente){
						tup.S++;
						it.Avanzar();
				}else{
					tup.O++;
					it.Avanzar();
					}
		it.Avanzar();
		
		}
		return tup;
		
}

Nat CampusSeguro::totalOcupados(tupla t){
	return t.O+t.H+t.E+t.S;
}

Posicion CampusSeguro::damePos(const Posicion& p1,const Posicion& p2){
	if(p1.x > p2.x){
		return Posicion(p1.x-1,p1.y);
	}
	if(p1.x < p2.x){
		return Posicion(p1.x+1,p1.y);
	}
	if(p1.y > p2.y){
		return Posicion(p1.x,p1.y-1);
	}
	if(p1.y < p2.y){
		return Posicion(p1.x,p1.y+1);
	}
}




//CampusSeguro::~CampusSeguro(){

//}
