#include "Participante.h"
#include <algorithm>
#include <curl/include/curl/curl.h>
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

#define FROM_ADDR    "<jaimeberenguel@gmail.com>"
#define TO_ADDR      "<jaimeberenguel@gmail.com>"
#define CC_ADDR      "<info@example.org>"
 
#define FROM_MAIL "Jaime Berenguel Perez " FROM_ADDR
#define TO_MAIL   "A Receiver " TO_ADDR
#define CC_MAIL   "John CC Smith " CC_ADDR
 
static const char *payload_text[] = {
  "Date: Mon, 29 Nov 2010 21:54:29 +1100\r\n",
  "To: " TO_MAIL "\r\n",
  "From: " FROM_MAIL "\r\n",
  "Cc: " CC_MAIL "\r\n",
  "Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9458efd@"
  "rfcpedant.example.org>\r\n",
  "Subject: ¡Piojo Invisible!\r\n",
  "\r\n", /* empty line to divide headers from body, see RFC5322 */ 
  "The body of the message starts here.\r\n",
  "\r\n",
  "It could be a lot of lines, could be MIME encoded, whatever.\r\n",
  "Check RFC5322.\r\n",
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
    /* This is the URL for your mailserver */ 
    curl_easy_setopt(curl, CURLOPT_URL, "smtp://mail.example.com");
 
    /* Note that this option isn't strictly required, omitting it will result
     * in libcurl sending the MAIL FROM command with empty sender data. All
     * autoresponses should have an empty reverse-path, and should be directed
     * to the address in the reverse-path which triggered them. Otherwise,
     * they could cause an endless loop. See RFC 5321 Section 4.5.5 for more
     * details.
     */ 
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, FROM_ADDR);
 
    /* Add two recipients, in this particular case they correspond to the
     * To: and Cc: addressees in the header, but they could be any kind of
     * recipient. */ 
    recipients = curl_slist_append(recipients, TO_ADDR);
    recipients = curl_slist_append(recipients, CC_ADDR);
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
 
    /* We're using a callback function to specify the payload (the headers and
     * body of the message). You could just use the CURLOPT_READDATA option to
     * specify a FILE pointer to read from. */ 
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
 
    /* Send the message */ 
    res = curl_easy_perform(curl);
 
    /* Check for errors */ 
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    /* Free the list of recipients */ 
    curl_slist_free_all(recipients);
 
    /* curl won't send the QUIT command until you call cleanup, so you should
     * be able to re-use this connection for additional messages (setting
     * CURLOPT_MAIL_FROM and CURLOPT_MAIL_RCPT as required, and calling
     * curl_easy_perform() again. It may not be a good idea to keep the
     * connection open for a very long time though (more than a few minutes
     * may result in the server timing out the connection), and you do want to
     * clean up in the end.
     */ 
    curl_easy_cleanup(curl);
  }
}
