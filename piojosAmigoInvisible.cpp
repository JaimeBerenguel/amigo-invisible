#include <iostream>
#include <vector>
#include <cstring>
#include <ctime>
#include "Participante.cpp"
using namespace std;

const int SILVIA = 0, LUIS = 1, LEO = 2, OLEH = 3, NIETO = 4, RAYAN = 5, JAIME = 6, PASTOR = 7, MANU = 8, QUINO = 9, ESTHER = 10;

void inicializaParticipantes(vector<Participante> &participantes);
void realizarListaAleatoria(vector<Participante> participantes);
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

void inicializaParticipantes(vector<Participante> &participantes){
	Participante silvia("Silvia","jaimeberenguel@gmail.com"); 	participantes.push_back(silvia);
	Participante luis("Luis","jaimeberenguel@gmail.com");		participantes.push_back(luis);
	Participante leo("Leo","jaimeberenguel@gmail.com");			participantes.push_back(leo);
	Participante oleh("Oleh","jaimeberenguel@gmail.com");		participantes.push_back(oleh);
	Participante nieto("Nieto","jaimeberenguel@gmail.com");		participantes.push_back(nieto);
	Participante rayan("Rayan","jaimeberenguel@gmail.com");		participantes.push_back(rayan);
	Participante jaime("Jaime","jaimeberenguel@gmail.com");		participantes.push_back(jaime);
	Participante pastor("Pastor","jaimeberenguel@gmail.com");	participantes.push_back(pastor);
	Participante manu("Manu","jaimeberenguel@gmail.com");		participantes.push_back(manu);
	Participante quino("Quino","jaimeberenguel@gmail.com");		participantes.push_back(quino);
	Participante esther("Esther","jaimeberenguel@gmail.com");	participantes.push_back(esther);
	anyadirEnemigos(participantes);
}

// En esta función añadiremos combinaciones que no pueden darse debido a enemistad entre miembros del sorteo....
void anyadirEnemigos(vector<Participante> &participantes){
	vector<Participante> enemigos; enemigos.push_back(participantes[PASTOR]);
}

void realizarListaAleatoria(vector<Participante> participantes){
	// Inicializamos semilla
	srand(time(NULL));
	vector<int> indexSeleccionados;
	for(int i=0;i<participantes.size();++i){
		int nrand = -1;
		do{
			nrand = rand()%participantes.size();
		}while(nrand == i || find(indexSeleccionados.begin(),indexSeleccionados.end(),nrand) != indexSeleccionados.end());
		participantes[i].setIdAmigoInvisible(nrand);
		cout << participantes[i].getNombre() << " " ;
		cout << participantes[nrand].getNombre() << endl;
		indexSeleccionados.push_back(nrand);
	}
}

void sendEmails(vector<Participante> participantes){
	/*
	for(int i=0;i<participantes.size();++i){
		participantes[i].sendEmail(participantes[participantes[i].getIdAmigoInvisible()]);
	}
	*/
	participantes[0].sendEmail(participantes[0]);
}
