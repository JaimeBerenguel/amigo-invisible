#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

class Participante{
	private:
		string nombre;
		string email;
		int idAmigoInvisible;
		vector<Participante> enemigos;
		
	public:
		Participante(){ this->nombre = ""; this->email = ""; this->idAmigoInvisible = -1; }
		Participante(string nombre,string email){ this->nombre = nombre; this->email = email; }
		Participante(string nombre,string email,vector<Participante> enemigos);
		
		string getNombre() const{ return nombre; }
		void setNombre(string nombre){ this->nombre = nombre; }
		
		string getEmail() const{ return email; }
		void setEmail(string email){ this->email = email; }
		
		vector<Participante> getEnemigos() const{ return enemigos; }
		void setEnemigos(vector<Participante> enemigos);

		int getIdAmigoInvisible() const{ return idAmigoInvisible;}
		void setIdAmigoInvisible(int idAmigoInvisible){ this->idAmigoInvisible = idAmigoInvisible; }
		
		void sendEmail(Participante p);
		
		Participante& operator=(const Participante &);
		bool operator==(const Participante &) const;
		bool operator!=(const Participante &) const;
		
		~Participante();
};
