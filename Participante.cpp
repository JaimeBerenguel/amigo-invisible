#include "Participante.h"
#include "Email.h"
#include <algorithm>
#include <time.h>
#include <curl/curl.h>
using namespace std;

Participante::Participante(string nombre, string email,vector<Participante> enemigos){
	this->nombre = nombre;
	this->email = email;
	setEnemigos(enemigos);
}

void
Participante::setEnemigos(vector<Participante> enemigos){
	this->enemigos.clear();
	
	for(int i=0;i<enemigos.size();++i){
		this->enemigos.push_back(enemigos[i]);
	}
}

bool
Participante::findParticipante(const vector<Participante> &v) const{
  // El email ha de ser único por cada participante así que...
  for(int i=0;i<v.size();++i){
    if(v[i].getEmail() == getEmail()) return true;
  }

  return false;
}

Participante&
Participante::operator=(const Participante &p){
	this->~Participante();
	
	this->nombre = p.getNombre();
	this->email = p.getEmail();
	this->setEnemigos(p.getEnemigos());
	
	return *this;
}

bool
Participante::operator==(const Participante &p) const{
	return (nombre != p.getNombre() || email != p.getEmail()) ? false:true;
}

bool
Participante::operator!=(const Participante &p) const{
	return ((*this) == p) ? false:true;
}

Participante::~Participante(){
	this->nombre = "";
	this->email = "";
	this->enemigos.clear();
}

string
Participante::getActualDate(){
  time_t rawtime;
  struct tm * timeinfo;
  char buffer [80];

  time (&rawtime);
  timeinfo = localtime (&rawtime);

  strftime (buffer,80,"Date: %a, %e %B %Y %H:%M:%S +1100\r\n",timeinfo);

  string result = buffer;
  return result;
}
 
void
Participante::sendEmail(Participante p){
  CURL *handler;
  CURLcode res = CURLE_OK;
  struct curl_slist *recipients = NULL;
  struct upload_status upload_ctx;

  upload_ctx.lines_read = 0;
  
  // Actualizamos los datos del Email de acuerdo al participante en cuestión...
  payload_text[0] = getActualDate().c_str();
  string destinatario = "To: " + getEmail() + "\r\n";

  // Actualizamos la línea 5 del Email para que añada el piojo (participante) al que se le envía el correo...
  string nombrePiojo = "Hola " + getNombre() + " si te ha llegado este mensaje, significa que participas en los piojos invisibles :)";
  payload_text[5] = nombrePiojo.c_str();

  // Actualizamos la línea 7 del Email para que añada el piojo (participante) elegido...
  string piojoElegido = "Bueno, lo primero que has de saber es quién te ha tocado y por ello el piojo afortunado ha sido ¡¡¡¡¡" + p.getNombre() + "!!!!!";
  payload_text[7] = piojoElegido.c_str();


  handler = curl_easy_init();
  if(handler) {
    curl_easy_setopt(handler, CURLOPT_URL, "smtp://smtp.gmail.com:587");
    curl_easy_setopt(handler, CURLOPT_USE_SSL, CURLUSESSL_ALL);
    curl_easy_setopt(handler, CURLOPT_USERNAME, FROM);
    curl_easy_setopt(handler, CURLOPT_PASSWORD, "D4J5D9S0CO5A4S3F9D6K0");

    // Le pasamos el correo al que se lo vamos a enviar
    recipients = curl_slist_append(recipients, getEmail().c_str());
    curl_easy_setopt(handler, CURLOPT_MAIL_RCPT, recipients);

    // Mientras queden datos por leer, se llamará a esta función.
    curl_easy_setopt(handler, CURLOPT_READFUNCTION, read_callback);

    // Con esta opcion le pasamos el argumento userdata = upload_ctx a la funcion read_callback...
    curl_easy_setopt(handler, CURLOPT_READDATA, &upload_ctx);

    curl_easy_setopt(handler, CURLOPT_UPLOAD, 1L);

    res = curl_easy_perform(handler);

    // Si algo ha salido mal...
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    else
      cout << "Mensaje enviado con exito" << endl;
    

    curl_slist_free_all(recipients);

    curl_easy_cleanup(handler);
  }
}
