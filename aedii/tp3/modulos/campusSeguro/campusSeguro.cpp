#include "campusSeguro.h"

CampusSeguro::CampusSeguro( Campus& c, const Dicc<Agente , Posicion >& d ){

	for(Nat i = 0; i < c.Filas(); i++) {
		matrizDeChabones.Agregar(i, Vector<infoChabones>());
		for (Nat j = 0; j < c.Columnas(); j++) {
			matrizDeChabones[i].Agregar(j, infoChabones());			
		}
	}

	campus = c;

	mismasSanciones.AgregarAtras(Conj<Agente>());
	Lista<Conj<Agente> > :: Iterador itMismasSanciones = mismasSanciones.CrearIt();
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
}


void CampusSeguro::IngresaEstudiante( const Nombre& e , const Posicion& p){
	assert(!estudiantes.Definido(e) && !hippies.Definido(e) && campus.EsIngreso(p) && !EstaOcupada(p));
	//REVISAR MUY BIEN INGRESAR ESTUDIANTE. POR EJ: EN QUE MOMENTO SE AGREGA EL ESTUDIANTE SI ES QUE INGRESA SIN PROBLEMAS?

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
	}else {
		infoEstudiante nuevo = infoEstudiante();
		nuevo.posicion = p;
		nuevo.itAux = estudiantesAux.Agregar(e);
		estudiantes.Definir(e,nuevo);
		if(TotalOcupados(CantPersonasAlrededor(campus.Vecinos(p))) == campus.Vecinos(p).Cardinal() && CantPersonasAlrededor(campus.Vecinos(p)).Seguridad == 1) {
			SumarSancion(campus.Vecinos(p));
			ModificarVecinos(p, campus.Vecinos(p));
		}
	}



}

void CampusSeguro::IngresaHippie(const Nombre& h , const Posicion& p){
	assert(!estudiantes.Definido(h) && !hippies.Definido(h) && campus.EsIngreso(p) && !EstaOcupada(p));
	//assert(!estudiantes.Definido(h));
	//assert(!hippies.Definido(h));
	//assert(campus.EsIngreso(p));
	//assert(!EstaOcupada(p));

	infoHippie nuevo = infoHippie();
	nuevo.posicion = p;
	nuevo.itAux = hippiesAux.Agregar(h);
	hippies.Definir(h,nuevo);
	matrizDeChabones[p.x][p.y].esHippieOEstudiante = true;
	matrizDeChabones[p.x][p.y].nombre = h;
	//cout<<"i0"<<endl;


	if(TotalOcupados(CantPersonasAlrededor(campus.Vecinos(p))) < campus.Vecinos(p).Cardinal()) {



		//cout<<"i1"<<endl;
		ModificarVecinos(p,campus.Vecinos(p));
		//cout<<"i2"<<endl;
	} else if(CantPersonasAlrededor(campus.Vecinos(p)).Estudiantes + CantPersonasAlrededor(campus.Vecinos(p)).Objetos == campus.Vecinos(p).Cardinal() ) {
		//cout<<"i3"<<endl;
			hippies.Significado(h).itAux.EliminarSiguiente();
			hippies.Borrar(h);
			infoEstudiante nuevo = infoEstudiante();
			nuevo.posicion = p;
			nuevo.itAux = estudiantesAux.Agregar(h);
			estudiantes.Definir(h,nuevo);
			ModificarVecinos(p,campus.Vecinos(p));
	      } else if(CantPersonasAlrededor(campus.Vecinos(p)).Seguridad + CantPersonasAlrededor(campus.Vecinos(p)).Objetos == campus.Vecinos(p).Cardinal() ) {
	      			//cout<<"i4"<<endl;
					SumarHippieAAgente(campus.Vecinos(p));
					hippies.Significado(h).itAux.EliminarSiguiente();
					hippies.Borrar(h);
					matrizDeChabones[p.x][p.y].esHippieOEstudiante = true;		
				} else {
					//cout<<"i5"<<endl;
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
			InsertarOrdenado(conKSanciones, it.Siguiente());
			it.Avanzar();
		}
	}
	return BusquedaRapida(k,conKSanciones);
}

const Conj<Agente> CampusSeguro::ConMismasSanciones(Agente a) const{
	infoAgente info = agentes.Significado(a);
	if(info.mismasSanciones.HaySiguiente())
		return info.mismasSanciones.Siguiente();
	else
		return Conj<Agente>();
}

void CampusSeguro::InsertarOrdenado(Vector<Nat>& v, const Nat& a){
	Nat i = 1;
	bool posicionEncontrada = false;
	while(i <= v.Longitud() && !posicionEncontrada){
		Nat sancionesAgente = agentes.Significado(v[i]).cantSanciones;
		if(agentes.Significado(a).cantSanciones <= sancionesAgente){
			v.Agregar(i,a);
			posicionEncontrada = true;
		}
		i++;
	}
	if(!posicionEncontrada){
		v.AgregarAtras(a);
	}
}

Conj<Agente> CampusSeguro::BusquedaRapida(Nat n , Vector<Agente> v){
	return Buscar(v,0,v.Longitud()-1,n);
}

Conj<Agente> CampusSeguro::Buscar(Vector<Agente> v, Nat i ,Nat s ,Nat k){
	if(v.Longitud() != 1){

		int intermedio = (int)((i+s)/2);
		if(agentes.Significado(v[intermedio]).cantSanciones < k){
			Buscar(v,i,intermedio,k);
		}else{
			Buscar(v,intermedio,s,k);
		}

	}

	return agentes.Significado(v[0]).cantSanciones == k ? ConMismasSanciones(v[0]) : Conj<Agente>();

}

Conj<Posicion> CampusSeguro::PosicionesMasCercanas(const Posicion& p, Conj<Posicion> c){
	Conj<Posicion> posicionesMasCercanas = Conj<Posicion>();
	Conj<Posicion>::Iterador itPosiciones = c.CrearIt();
	if(itPosiciones.HaySiguiente()){
		Nat distanciaMasCercana = campus.Distancia(itPosiciones.Siguiente(), p);
		while(itPosiciones.HaySiguiente()){
			Nat distancia = campus.Distancia(itPosiciones.Siguiente(), p);
			if(distancia < distanciaMasCercana){
				distanciaMasCercana = distancia;
				posicionesMasCercanas.Agregar(itPosiciones.Siguiente());
			}else if(distancia == distanciaMasCercana){
					posicionesMasCercanas.Agregar(itPosiciones.Siguiente());
				}
			itPosiciones.Avanzar();
		}
	}
	return posicionesMasCercanas;
}

/*bool CampusSeguro::EstaOcupada(Posicion p) {
	return campus.EsOcupada(p) || matrizDeChabones[p.x][p.y].esHippieOEstudiante || matrizDeChabones[p.x][p.y].esAgente;
}*/

void CampusSeguro::ModificarVecinos(const Posicion& p, const Conj<Posicion>& c){
	Conj<Posicion>::const_Iterador it = c.CrearIt();
	while(it.HaySiguiente()){
		//cout<<"v1"<<endl;
		if(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].esHippieOEstudiante){
			if(matrizDeChabones[p.x][p.y].esHippieOEstudiante){
				cout<<"Modificar Vecinos"<<endl;
				ModificarAux(matrizDeChabones[p.x][p.y].nombre,matrizDeChabones[it.Siguiente().x][it.Siguiente().y].nombre);
			}
			//it.Avanzar();
		}else if(matrizDeChabones[p.x][p.y].esAgente && hippies.Definido(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].nombre)) {
				//cout<<"v3"<<endl;
				CapturadoH(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].nombre, it.Siguiente());
				//it.Avanzar();
			}else{
				//cout<<"v4"<<endl;
				CapturadoE(it.Siguiente());
				//it.Avanzar();
			}
		it.Avanzar();
	}
}

void CampusSeguro::ModificarAux(const Nombre& n1, const Nombre& n2){
	//cout<<"aux1"<<endl;
	if(estudiantes.Definido(n2)){
		if(estudiantes.Definido(n1)){
			//cout<<"aux2"<<endl;
			CapturadoE(estudiantes.Significado(n2).posicion); //Sofi asegura que n2 esta bien
		}else if(hippies.Definido(n1)){
				//cout<<"aux3"<<endl;
				ConvertidoYCapturado(n2,estudiantes.Significado(n2).posicion);
			}else{
				//cout<<"aux4"<<endl;
				CapturadoE(estudiantes.Significado(n2).posicion); //Sofi asegura que n2 esta bien
			}
	}else if(estudiantes.Definido(n1)){
			//cout<<"aux5"<<endl;
			CorregidoYcapturado(n2,hippies.Significado(n2).posicion);
		}else{
			cout<<"aux6"<<endl;
			CapturadoH(n2,hippies.Significado(n2).posicion);
		}
}

void CampusSeguro::CapturadoE(const Posicion& p){
	//cout<<"e0"<<endl;
	if(TotalOcupados(CantPersonasAlrededor(campus.Vecinos(p))) == campus.Vecinos(p).Cardinal()){
		//cout<<"e1"<<endl;
		SumarSancion(campus.Vecinos(p));
//		cout<<"e2"<<endl;
	}
}

void CampusSeguro::CapturadoH(const Nombre& n,const Posicion& p){
	//if(!estudiantes.Definido(n))
	//	cout<<"ESTUDIANTE NO DEFINIDO "<<n<<endl;
	//TODO. REVISAR ESTA FUNCION CON SOFIA O ALGUIEN QUE LA ENTIENDA.

	if(TotalOcupados(CantPersonasAlrededor(campus.Vecinos(p))) == campus.Vecinos(p).Cardinal() 
		&& 
		CantPersonasAlrededor(campus.Vecinos(p)).Seguridad >= 1)
	{
		SumarHippieAAgente(campus.Vecinos(p));
		matrizDeChabones[p.x][p.y].esHippieOEstudiante = false;
		hippies.Significado(n).itAux.EliminarSiguiente();
		hippies.Borrar(n);
	}
}

void CampusSeguro::CorregidoYcapturado(const Nombre& n, const Posicion& p){
	if(CantPersonasAlrededor(campus.Vecinos(p)).Estudiantes + CantPersonasAlrededor(campus.Vecinos(p)).Objetos == campus.Vecinos(p).Cardinal()) {
		hippies.Significado(n).itAux.EliminarSiguiente();
		hippies.Borrar(n);
		infoEstudiante nuevo;
		nuevo.posicion = Posicion(p);
		nuevo.itAux = estudiantesAux.Agregar(n);
		estudiantes.Definir(n,nuevo);
	} else {
		CapturadoH(n,p);
	}
}

void CampusSeguro::ConvertidoYCapturado(const Nombre& n, const Posicion& p){
	if(CantPersonasAlrededor(campus.Vecinos(p)).Hippies >=2){
		infoHippie nuevo;
		nuevo.posicion = Posicion(p);
		nuevo.itAux = hippiesAux.Agregar(n);
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


infoEntorno CampusSeguro::CantPersonasAlrededor(const Conj<Posicion>& c){
	Conj<Posicion>::const_Iterador it =c.CrearIt();
	infoEntorno res;
	res.Estudiantes = 0;
	res.Hippies = 0;
	res.Seguridad = 0;
	res.Objetos = 0;

	while(it.HaySiguiente()){

		if(campus.EsOcupada(it.Siguiente())) {
			res.Objetos++;
		}

		if(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].esHippieOEstudiante) {

			//es estudiante
			if(estudiantes.Definido(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].nombre)) {
				res.Estudiantes++;
			}

			//es hippie
			if(hippies.Definido(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].nombre)) {
				res.Hippies++;
			}

		}

		if(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].esAgente ) {
			res.Seguridad++;
		}

		it.Avanzar();
	}
	return res;
}

Nat CampusSeguro::TotalOcupados(infoEntorno t){
	return t.Objetos + t.Hippies + t.Estudiantes + t.Seguridad;
}

bool CampusSeguro::EstaOcupada(tp::Posicion p)  const{
	return campus.EsOcupada(p) || matrizDeChabones[p.x][p.y].esHippieOEstudiante || matrizDeChabones[p.x][p.y].esAgente;
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