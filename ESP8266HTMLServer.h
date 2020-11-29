#ifndef __ESP8266HTMLSERVER_H__
#define __ESP8266HTMLSERVER_H__

#include <ESP8266WebServer.h>
#include <HTMLTemplate.h>
#include <TagWallet.h>

class ESP8266HTMLServer : public ESP8266WebServer, public TagWallet {
public:
	ESP8266HTMLServer(int port = 80) : ESP8266WebServer(port){};
	void sendLength(size_t length) {
		String header;
		_prepareHeader(header, 200, "text/html", length);
		_currentClient.write((const uint8_t *)header.c_str(), header.length());
	};
};

#endif  // __ESP8266HTMLSERVER_H__