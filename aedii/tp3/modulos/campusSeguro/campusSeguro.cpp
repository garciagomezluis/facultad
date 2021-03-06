#include "campusSeguro.h"

CampusSeguro::~CampusSeguro() { }

CampusSeguro::CampusSeguro( Campus* c, const Dicc<Agente , Posicion >& d ){

	for(Nat i = 0; i < c->Filas(); i++) {
		matrizDeChabones.Agregar(i, Vector<infoChabones>());
		for (Nat j = 0; j < c->Columnas(); j++) {
			matrizDeChabones[i].Agregar(j, infoChabones());			
		}
	}

	campus = c;
	hubieronSanciones = false;
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
		matrizDeChabones[nuevo.posAgente.x][nuevo.posAgente.y].esAgente = true;
		matrizDeChabones[nuevo.posAgente.x][nuevo.posAgente.y].agente = it.SiguienteClave();
		masVigilante = it.SiguienteClave(); //EL mas vigilante es cualquiera al ppio.
		it.Avanzar();
	}
}


void CampusSeguro::IngresaEstudiante( const Nombre& e , const Posicion& p){
	assert(!estudiantes.Definido(e) && !hippies.Definido(e) && campus->EsIngreso(p) && !EstaOcupada(p));

	matrizDeChabones[p.x][p.y].esHippieOEstudiante = true;
	matrizDeChabones[p.x][p.y].nombre = e;

	if(CantPersonasAlrededor(campus->Vecinos(p)).Hippies > 2 ){
		infoHippie nuevo;
		nuevo.posicion = p;
		nuevo.itAux = hippiesAux.Agregar(e);
		hippies.Definir(e,nuevo);
		ModificarVecinos(p, campus->Vecinos(p));
		if(TotalOcupados(CantPersonasAlrededor(campus->Vecinos(p))) == campus->Vecinos(p).Cardinal() && CantPersonasAlrededor(campus->Vecinos(p)).Seguridad == 1){
			SumarHippieAAgente(campus->Vecinos(p));
			hippies.Significado(e).itAux.EliminarSiguiente();
			hippies.Borrar(e);
			matrizDeChabones[p.x][p.y].esHippieOEstudiante = false;	
		}
	}else {
		infoEstudiante nuevo;
		nuevo.posicion = p;
		nuevo.itAux = estudiantesAux.Agregar(e);
		estudiantes.Definir(e, nuevo);
		if(TotalOcupados(CantPersonasAlrededor(campus->Vecinos(p))) == campus->Vecinos(p).Cardinal() && CantPersonasAlrededor(campus->Vecinos(p)).Seguridad >= 1) {
			SumarSancion(campus->Vecinos(p));
			ModificarVecinos(p, campus->Vecinos(p));
		}
	}



}

void CampusSeguro::IngresaHippie(const Nombre& h , const Posicion& p){
	assert(!estudiantes.Definido(h) && !hippies.Definido(h) && campus->EsIngreso(p) && !EstaOcupada(p));
	
	infoHippie nuevo;
	nuevo.posicion = p;
	nuevo.itAux = hippiesAux.Agregar(h);
	hippies.Definir(h,nuevo);
	matrizDeChabones[p.x][p.y].esHippieOEstudiante = true;
	matrizDeChabones[p.x][p.y].nombre = h;

	if(TotalOcupados(CantPersonasAlrededor(campus->Vecinos(p))) < campus->Vecinos(p).Cardinal()) {
		ModificarVecinos(p,campus->Vecinos(p));
	} else if(CantPersonasAlrededor(campus->Vecinos(p)).Estudiantes + CantPersonasAlrededor(campus->Vecinos(p)).Objetos == campus->Vecinos(p).Cardinal() ) {
			hippies.Significado(h).itAux.EliminarSiguiente();
			hippies.Borrar(h);
			infoEstudiante nuevo;
			nuevo.posicion = p;
			nuevo.itAux = estudiantesAux.Agregar(h);
			estudiantes.Definir(h,nuevo);
			ModificarVecinos(p,campus->Vecinos(p));
	      } else if(CantPersonasAlrededor(campus->Vecinos(p)).Seguridad + CantPersonasAlrededor(campus->Vecinos(p)).Objetos == campus->Vecinos(p).Cardinal() ) {
					SumarHippieAAgente(campus->Vecinos(p));
					hippies.Significado(h).itAux.EliminarSiguiente();
					hippies.Borrar(h);
					matrizDeChabones[p.x][p.y].esHippieOEstudiante = false;		
				} else {
		     		ModificarVecinos(p,campus->Vecinos(p));
                }
}

void CampusSeguro::MoverEstudiante(const Nombre& e , const Direccion& d ){
	Posicion viejaPos(estudiantes.Significado(e).posicion);
	

	if((campus->EsIngresoSuperior(viejaPos) && d == Arriba) || (campus->EsIngresoInferior(viejaPos) && d == Abajo)){
		//Se va el estudiante
		estudiantes.Significado(e).itAux.EliminarSiguiente();
		estudiantes.Borrar(e);
	}
	else{
		Posicion nuevaPos(campus->ProxPosicion(viejaPos,d));	
		//cout<<"Es ingreso donde estaba parado? "<<campus->EsIngreso(estudiantes.Significado(e).posicion)<<endl;
		//cout<<"Intentando mover estudiante a"<<nuevaPos<<endl;
		//acá!! que pasa si el estudiante sale del tablero?? se rompe matrizDeChabones! mirar la pre de moverEstudiante en el diseño

		matrizDeChabones[viejaPos.x][viejaPos.y].esHippieOEstudiante = false;
		estudiantes.Significado(e).posicion = nuevaPos;
		matrizDeChabones[nuevaPos.x][nuevaPos.y].esHippieOEstudiante = true;
		matrizDeChabones[nuevaPos.x][nuevaPos.y].nombre = e;
		if(CantPersonasAlrededor(campus->Vecinos(nuevaPos)).Hippies >=2){
			estudiantes.Significado(e).itAux.EliminarSiguiente();
			estudiantes.Borrar(e);
			infoHippie nuevo = infoHippie();
			nuevo.posicion = nuevaPos;
			nuevo.itAux = hippiesAux.Agregar(e);
			ModificarVecinos(nuevaPos, campus->Vecinos(nuevaPos));	
		}else{
			ModificarVecinos(nuevaPos, campus->Vecinos(nuevaPos));
		     }
	 }
}

void CampusSeguro::MoverHippie (const Nombre& h){

	infoHippie datosHippie = hippies.Significado(h);
	Posicion p = datosHippie.posicion;

	Conj<infoEstudiante*> datosEstudiante = estudiantes.Significados();
	Conj<infoEstudiante*> :: Iterador itDatosEstudiante = datosEstudiante.CrearIt();
	Conj<Posicion> posicionesEstudiantes = Conj<Posicion>();
	while(itDatosEstudiante.HaySiguiente()){
		posicionesEstudiantes.Agregar(itDatosEstudiante.Siguiente()->posicion);
		itDatosEstudiante.Avanzar();
	}
	Conj<Posicion> estudiantesMasCercanos = PosicionesMasCercanas(p,posicionesEstudiantes);
	datosHippie.estudiantesMasCercanos = estudiantesMasCercanos;
	
	Posicion proximaPosicion = DamePos(p,campus->IngresosMasCercanos(p).CrearIt().Siguiente());
	if(estudiantesMasCercanos.Cardinal() > 0){
		proximaPosicion = DamePos(p,estudiantesMasCercanos.CrearIt().Siguiente());
	}
	if(!EstaOcupada(proximaPosicion)){
		matrizDeChabones[proximaPosicion.x][proximaPosicion.y].esHippieOEstudiante = true;
		matrizDeChabones[proximaPosicion.x][proximaPosicion.y].nombre = h;
		matrizDeChabones[p.x][p.y].esHippieOEstudiante = false;
		datosHippie.posicion = proximaPosicion;
		//Redefino el hippie.
		//Entiendo que teoricamente no se deberia hacer esto, supuestamente se devuelve una Referencia a info hippie cuando se le pide el significado, pero no estaria funcionando bien. Hacer esto no es lindo pero no rompe complejdades tampoco
		hippies.Borrar(h); //Esta linea la agrego Julian oara que no haya perdida de memoria por la linea que agrego luis
		hippies.Definir(h, datosHippie); //esta linea la agregó luis (consultar con los muchachos si esta bien (rompe test_mover_hippie_a_estudiante sino))

		ModificarVecinos(proximaPosicion, campus->Vecinos(proximaPosicion));
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

	Posicion proximaPosicion = DamePos(p,campus->IngresosMasCercanos(p).CrearIt().Siguiente());
	if(hippiesMasCercanos.Cardinal() > 0){
		proximaPosicion = DamePos(p,hippiesMasCercanos.CrearIt().Siguiente());
	}
	if(!EstaOcupada(proximaPosicion)){
		matrizDeChabones[proximaPosicion.x][proximaPosicion.y].esAgente = true;
		matrizDeChabones[proximaPosicion.x][proximaPosicion.y].agente = a;

		datosAgente.posAgente = proximaPosicion; 
		agentes.Borrar(a);
		agentes.Definir(a,datosAgente);
		ModificarVecinos(proximaPosicion, campus->Vecinos(proximaPosicion));
	}
}

const Campus& CampusSeguro::DameCampus() const{
	return *campus;
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
	infoAgente agente = agentes.Significado(a);
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

void CampusSeguro::Sancionar(Nat p, Nat cs){

	infoAgente agente = agentes.Significado(p);
	Lista<Conj<Agente>>::Iterador it = agente.mismasSanciones;

	if(!it.HaySiguiente()) {
		it = mismasSanciones.AgregarAtras(Conj<Agente>());
	}else{
		it.Avanzar();
	}

	if(!it.HaySiguiente()) {
		it = mismasSanciones.AgregarAtras(Conj<Agente>());
	}

	Conj<Agente>::Iterador itMismasSanciones = it.Siguiente().Agregar(p);

	agente.mismaSancion.EliminarSiguiente();
	agente.cantSanciones = agente.cantSanciones + cs;
	agente.mismasSanciones = it;
	agente.mismaSancion = itMismasSanciones;
	agentes.Definir(p, agente);
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
	Nat i = 0;
	bool posicionEncontrada = false;
	while(i < v.Longitud() && !posicionEncontrada){
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

Conj<Agente> CampusSeguro::Buscar(Vector<Agente> v, Nat min ,Nat max ,Nat k){
	int intermedio = (int)((min+max)/2);

	if( k < agentes.Significado(v[0]).cantSanciones || k > agentes.Significado(v[v.Longitud() - 1]).cantSanciones ) return Conj<Agente>();

	if(agentes.Significado(v[intermedio]).cantSanciones == k) {
		return ConMismasSanciones(v[intermedio]);
	} else if (agentes.Significado(v[intermedio]).cantSanciones < k) {
		return Buscar(v,intermedio + 1,max,k);
	} else {
		return Buscar(v,min,intermedio - 1,k);
	}

}

Conj<Posicion> CampusSeguro::PosicionesMasCercanas(const Posicion& p, Conj<Posicion> c){
	Conj<Posicion> posicionesMasCercanas = Conj<Posicion>();
	Conj<Posicion>::Iterador itPosiciones = c.CrearIt();
	if(itPosiciones.HaySiguiente()){
		Nat distanciaMasCercana = campus->Distancia(itPosiciones.Siguiente(), p);
		while(itPosiciones.HaySiguiente()){
			Nat distancia = campus->Distancia(itPosiciones.Siguiente(), p);
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

void CampusSeguro::ModificarVecinos(const Posicion& p, const Conj<Posicion>& c){
	Conj<Posicion>::const_Iterador it = c.CrearIt();
	while(it.HaySiguiente()){

		if(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].esHippieOEstudiante){
			if(matrizDeChabones[p.x][p.y].esHippieOEstudiante){
				ModificarAux(matrizDeChabones[p.x][p.y].nombre,matrizDeChabones[it.Siguiente().x][it.Siguiente().y].nombre);
			}
		}else if(matrizDeChabones[p.x][p.y].esAgente) {
				if(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].esHippieOEstudiante && hippies.Definido(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].nombre))
				{
					CapturadoH(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].nombre, it.Siguiente());
				}
			}else{
				CapturadoE(it.Siguiente());
			}
		it.Avanzar();
	}
}

void CampusSeguro::ModificarAux(const Nombre& n1, const Nombre& n2){
	if(estudiantes.Definido(n2)){
		if(estudiantes.Definido(n1)){
			CapturadoE(estudiantes.Significado(n2).posicion);
		}else if(hippies.Definido(n1)){
				ConvertidoYCapturado(n2,estudiantes.Significado(n2).posicion);
			}else{
				CapturadoE(estudiantes.Significado(n2).posicion);
			}
	}else if(estudiantes.Definido(n1)){
			CorregidoYcapturado(n2,hippies.Significado(n2).posicion);
		}else{
			CapturadoH(n2,hippies.Significado(n2).posicion);
		}
}

void CampusSeguro::CapturadoE(const Posicion& p){
	if(TotalOcupados(CantPersonasAlrededor(campus->Vecinos(p))) == campus->Vecinos(p).Cardinal()){
		SumarSancion(campus->Vecinos(p));
	}
}

void CampusSeguro::CapturadoH(const Nombre& n,const Posicion& p){
	//TODO. REVISAR ESTA FUNCION CON SOFIA O ALGUIEN QUE LA ENTIENDA.

	if(TotalOcupados(CantPersonasAlrededor(campus->Vecinos(p))) == campus->Vecinos(p).Cardinal() 
		&& 
		CantPersonasAlrededor(campus->Vecinos(p)).Seguridad >= 1)
	{
		SumarHippieAAgente(campus->Vecinos(p));
		matrizDeChabones[p.x][p.y].esHippieOEstudiante = false;
		hippies.Significado(n).itAux.EliminarSiguiente();
		hippies.Borrar(n);
	}
}

void CampusSeguro::CorregidoYcapturado(const Nombre& n, const Posicion& p){
	if(CantPersonasAlrededor(campus->Vecinos(p)).Estudiantes + CantPersonasAlrededor(campus->Vecinos(p)).Objetos == campus->Vecinos(p).Cardinal()) {
		infoEstudiante nuevo;
		nuevo.posicion = p;
		nuevo.itAux = estudiantesAux.Agregar(n);
		hippies.Significado(n).itAux.EliminarSiguiente();
		hippies.Borrar(n);
		estudiantes.Definir(n,nuevo);
	} else {
		CapturadoH(n,p);
	}
}

void CampusSeguro::ConvertidoYCapturado(const Nombre& n, const Posicion& p){
	if(CantPersonasAlrededor(campus->Vecinos(p)).Hippies >=2){
		infoHippie nuevo;
		nuevo.posicion = Posicion(p);
		nuevo.itAux = hippiesAux.Agregar(n);
		ModificarVecinos(p,campus->Vecinos(p));
		if(TotalOcupados(CantPersonasAlrededor(campus->Vecinos(p))) == campus->Vecinos(p).Cardinal() && CantPersonasAlrededor(campus->Vecinos(p)).Seguridad == 1){
			SumarHippieAAgente(campus->Vecinos(p));
			hippies.Significado(n).itAux.EliminarSiguiente();
			hippies.Borrar(n);
			matrizDeChabones[p.x][p.y].esHippieOEstudiante = false;
		}
	}else if(TotalOcupados(CantPersonasAlrededor(campus->Vecinos(p))) == campus->Vecinos(p).Cardinal() && CantPersonasAlrededor(campus->Vecinos(p)).Seguridad == 1){
		SumarSancion(campus->Vecinos(p));
	}
}

void CampusSeguro::SumarSancion(const Conj<Posicion>& c){
	Conj<Posicion>::const_Iterador it = c.CrearIt();
	while(it.HaySiguiente()){
		if(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].esAgente){
			Sancionar(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].agente,1);
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
			agentes.Borrar(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].agente);
			agentes.Definir(matrizDeChabones[it.Siguiente().x][it.Siguiente().y].agente, agente);
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

		if(campus->EsOcupada(it.Siguiente())) {
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
	return campus->EsOcupada(p) || matrizDeChabones[p.x][p.y].esHippieOEstudiante || matrizDeChabones[p.x][p.y].esAgente;
}

Posicion CampusSeguro::DamePos(const Posicion& p1, const Posicion& p2){
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