#include <curl/curl.h>
#include <iostream>
#include <cstring>
using namespace std;

#define FROM    "piojos.invisibles@gmail.com"

struct upload_status {
    int lines_read;
};

// Datos que se enviarán en el email a falta de futura modificación segun el participante en cuestión, fecha... etc.
static const char *payload_text[] = {
    "Date: (FECHA)\r\n",                                                                                            // Línea 0
    "From: Piojos invisibles" FROM "\r\n",                                                                          // Línea 1
    "Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9458efd@"                                                            // Línea 1 aún
    "rfcpedant.example.org>\r\n",                                                                                   // Línea 2
    "Subject: PIOJOS INVISIBLES\r\n",                                                                               // Línea 3
    "\r\n",                                                                                                         // Línea 4
    "Hola (NOMBRE) si te ha llegado este mensaje, significa que participas en los piojos invisibles :)",            // Línea 5
    "\r\n",                                                                                                         // Línea 6
    "Bueno, lo primero que has de saber es quién te ha tocado y por ello el piojo afortunado ha sido (NOMBRE)",     // Línea 7
    "\r\n",                                                                                                         // Línea 8
    "Dicho eso, debes saber que este evento se producirá (FECHA POR DECIDIR)",                                      // Línea 9
    " bueno, siempre que Leo no llegue tarde xd",                                                                   // Línea 10
    "\r\n",                                                                                                         // Línea 11
    "Recuerda, el regalo debe costar al menos 5€",                                                                  // Línea 12
    "\r\n",                                                                                                         // Línea 13
    "Si tienes cualquier duda háblame y si no, también por favor, estoy solo :(",                                   // Línea 14
    "\r\n",                                                                                                         // Línea 15
    NULL
};

// Esta función sera llamada por el servidor STMP para leer la información del mensaje a enviar que será rellenada con userdata.
size_t read_callback(char *buffer, size_t size, size_t nitems, void *userdata)
{
    struct upload_status *upload_ctx = (struct upload_status *)userdata;
    const char *data;

    if((size == 0) || (nitems == 0) || ((size*nitems) < 1)) {
        return 0;
    }

    data = payload_text[upload_ctx->lines_read];

    if(data) {
        size_t len = strlen(data);
        memcpy(buffer, data, len);
        upload_ctx->lines_read++;

        return len;
    }

    return 0;
}