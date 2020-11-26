#ifndef __SERVERSERIAL_H__
#define __SERVERSERIAL_H__

#include <FS.h>
#include <HTMLTemplateServer.h>

class ServerSerial : public HTMLTemplateServerDef {
public:
	virtual void sendLength(int length){
		Serial.printf("Total à envoyer = %u\r\n", length);
	}

	virtual void sendContent(const char *content) {
		Serial.printf(content); 
	};
	virtual void sendContent_P(PGM_P content) {
		Serial.printf_P(content);
	};
	virtual void sendContent_P(PGM_P content, uint32_t len){
		Serial.printf_P("%.*s", len, content);
	};
};

#endif  // __SERVERSERIAL_H__