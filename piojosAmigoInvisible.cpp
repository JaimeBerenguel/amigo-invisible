#include <iostream>
#include <vector>
#include <cstring>
#include <ctime>
#include "Participante.cpp"
using namespace std;

#define __PROBANDO__
const int SILVIA = 0, LUIS = 1, LEO = 2, OLEH = 3, NIETO = 4, RAYAN = 5, JAIME = 6, ARTURO = 7, QUINO = 8, ESTHER = 9;

void inicializaParticipantes(vector<Participante> &participantes);
void realizarListaAleatoria(vector<Participante> &participantes);
void anyadirEnemigos(vector<Participante> &participantes);
void sendEmails(vector<Participante> participantes);

int main(){
	cout << "************************************************************************************************" << endl;
	cout << "***** Bienvenidx al generador aleatorio de listas para el amigo invisible de los piojos :) *****" << endl;
	cout << "************************************************************************************************" << endl;
	cout << "************************************************************************************************" << endl;
	cout << "Procediendo a realizar la lista..." << endl;
	vector<Participante> participantes;
	inicializaParticipantes(participantes);
	realizarListaAleatoria(participantes);
	sendEmails(participantes);
	return 0;
}

// En esta función incluiremos en el vector participantes a todos los que participen en este sorteo.
void inicializaParticipantes(vector<Participante> &participantes){
	Participante silvia("Silvia","silvia.cgv0@gmail.com"); 			participantes.push_back(silvia);
	Participante luis("Luis","luis51100lvl@gmail.com");				participantes.push_back(luis);
	Participante leo("Leo","leopoldocadavid@gmail.com");			participantes.push_back(leo);
	Participante oleh("Oleh","olehmoroz00@gmail.com");				participantes.push_back(oleh);
	Participante nieto("Nieto","panilo2013@gmail.com");				participantes.push_back(nieto);
	Participante rayan("Rayan","rayanchelihi@gmail.com");			participantes.push_back(rayan);
	Participante jaime("Jaime","jaimeberenguel@gmail.com");			participantes.push_back(jaime);
	Participante arturo("Arturo","elreinodearturo@hotmail.com");	participantes.push_back(arturo);
	Participante quino("Quino","quino7373@gmail.com");				participantes.push_back(quino);
	Participante esther("Esther","123esther50@gmail.com");			participantes.push_back(esther);
	anyadirEnemigos(participantes);
}

// En esta función añadiremos combinaciones que no pueden darse debido a enemistad entre miembros del sorteo....
void anyadirEnemigos(vector<Participante> &participantes){
	// Oleh no puede quiere que le pueda tocar Nieto.
	vector<Participante> enemigos; enemigos.push_back(participantes[NIETO]);
	participantes[OLEH].setEnemigos(enemigos);
}

void realizarListaAleatoria(vector<Participante> &participantes){
	// Inicializamos semilla
	srand(time(NULL));
	vector<int> indexSeleccionados;

	// Recorremos el vector de participantes y a cada uno le asociamos otro participante al que tendrá que regalarle algo
	for(int i=0;i<participantes.size();++i){
		int nrand = -1;

		/*  Vamos a sacar numeros aleatorios considerandolos índices del vector participantes y siguiendo los siguientes criterios:
			1-. El índice no puede ser el mismo que el del participante
			2-. El índice no puede haber aparecido ya (A dos participante no les puede tocar regalarle algo a la misma persona)
			3-. El índice no puede corresponder (según el vector participantes) a un enemigo de dicho participante
		*/
		do{
			nrand = rand()%participantes.size();
		}while(nrand == i || find(indexSeleccionados.begin(),indexSeleccionados.end(),nrand) != indexSeleccionados.end() || 
				participantes[nrand].findParticipante(participantes[i].getEnemigos()));
		
		#ifdef __PROBANDO__
		cout << participantes[i].getNombre() << " " ;
		cout << participantes[nrand].getNombre() << endl;
		#endif

		participantes[i].setIdAmigoInvisible(nrand);
		indexSeleccionados.push_back(nrand);
	}
}

void sendEmails(vector<Participante> participantes){
	for(int i=0;i<participantes.size();++i){
		participantes[i].sendEmail(participantes[participantes[i].getIdAmigoInvisible()]);
	}
}