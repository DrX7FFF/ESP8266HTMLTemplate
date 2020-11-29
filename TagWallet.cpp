#include <ESP8266HTMLServer.h>
#include <HTMLTemplate.h>
#include <TagWallet.h>

void TagCharPtr::sendWithServer(ESP8266HTMLServer* server) {
	server->sendContent(_value);
};

void TagPROGMEM::sendWithServer(ESP8266HTMLServer* server) {
	server->sendContent_P(_value);
};

void TagFctPtr::sendWithServer(ESP8266HTMLServer* server) {
	server->sendContent(_value().c_str());
};

void TagStringPtr::sendWithServer(ESP8266HTMLServer* server) {
	server->sendContent_P((*_value).c_str());
};

void TagIntPtr::sendWithServer(ESP8266HTMLServer* server) {
	server->sendContent(_cache);
	delete _cache;
	_cache = nullptr;
};

size_t TagTemplate::getLenght(ESP8266HTMLServer* server) {
	return _value->getLenght(server);
};

void TagTemplate::sendWithServer(ESP8266HTMLServer* server) {
	_value->sendWithServer(server);
};