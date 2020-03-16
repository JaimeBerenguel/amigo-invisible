#include "Participante.h"
#include <algorithm>
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
	if(nombre != p.getNombre() || email != p.getEmail() || enemigos.size() != p.getEnemigos().size()) 
		return false;
	
	for(int i=0;i<enemigos.size();++i)
		if(enemigos[i].getNombre() != p.enemigos[i].getNombre() || enemigos[i].getEmail() != p.enemigos[i].getEmail() ) return false;
		
	return true;
}

bool
Participante::operator!=(const Participante &p) const{
	return (*this == p) ? false:true;
}

Participante::~Participante(){
	this->nombre = "";
	this->email = "";
	this->enemigos.clear();
}


#define FROM    "jaimeberenguel@gmail.com"
#define TO      "berfibili@gmail.com"
//#define CC      "someother@gmail.com"

static const char *payload_text[] = {
  "Date: Mon, 16 Marx 2020 20:32:50 +1100\r\n",
  "To: " TO "\r\n",
  "From: " FROM "Jaime Berenguel Pérez\r\n",
//  "Cc: " CC "(Another example User)\r\n",
  "Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9458efd@"
  "rfcpedant.example.org>\r\n",
  "Subject: 4o dia de cuarentena\r\n",
  "\r\n", /* empty line to divide headers from body, see RFC5322 */
  "Mis cordiales saludos berfín, como usted sabrá me hallo en mi cuarto dia de cuarentena y es por ello que le escribo esta carta desde San Blas",
  " para que usted sepa que aunque nos separe un virus, no la olvido y aún recuerdo todos los momentos juntos que hemos pasado, principalmente porque no nos",
  " conocemos, F. Jaime Berenguel Pérez.\r\n",
  NULL
};

struct upload_status {
  int lines_read;
};

static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp)
{
  struct upload_status *upload_ctx = (struct upload_status *)userp;
  const char *data;

  if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
    return 0;
  }

  data = payload_text[upload_ctx->lines_read];

  if(data) {
    size_t len = strlen(data);
    memcpy(ptr, data, len);
    upload_ctx->lines_read++;

    return len;
  }

  return 0;
}
 
void
Participante::sendEmail(Participante p){
  CURL *curl;
  CURLcode res = CURLE_OK;
  struct curl_slist *recipients = NULL;
  struct upload_status upload_ctx;

  upload_ctx.lines_read = 0;

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");
    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
    curl_easy_setopt(curl, CURLOPT_USERNAME, "jaimeberenguel@gmail.com");
    curl_easy_setopt(curl, CURLOPT_PASSWORD, "jaimeberenguel1999");
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, FROM);

    recipients = curl_slist_append(recipients, TO);
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

   curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);

    res = curl_easy_perform(curl);

    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    curl_slist_free_all(recipients);

    curl_easy_cleanup(curl);
  }

  cout << "Mensaje enviado con exito" << endl;
}
