#ifndef __SERVERSERIAL_H__
#define __SERVERSERIAL_H__

#include <FS.h>
#include <HTMLTemplateServer.h>

class ServerSerial : public HTMLTemplateServerDef {
public:
	virtual void sendContent(const char *content) { sendContent_P(content); }
	virtual void sendContent_P(PGM_P content) {
		printf(content);
	};
	virtual void sendContent_P(PGM_P content, uint32_t len){
		printf("%.*s", len, content);
	}
};

#endif  // __SERVERSERIAL_H__