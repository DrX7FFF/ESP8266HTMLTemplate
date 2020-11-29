#include <ESP8266HTMLServer.h>
#include <HTMLTemplate.h>
#include <TagWallet.h>

size_t TemplateDef::getLenght(ESP8266HTMLServer *server) {
	_tagTemp = new TagBase *[_tagCount];
	size_t length = strlen_P(_pattern);

	for (uint16_t i = 0; i < _tagCount; i++) {
		char tempName[_tags[i].nameLen + 1];
		strncpy_P(tempName, &_pattern[_tags[i].tagPos + _tags[i].namePos], _tags[i].nameLen);
		tempName[_tags[i].nameLen] = '\0';
		_tagTemp[i] = server->find(tempName);
		if (_tagTemp[i]) {
			length -= _tags[i].tagLen;
			length += _tagTemp[i]->getLenght(server);
		} else {
			DEBUGLOG("Tag @{%s} not found in wallet\r\n", tempName);
		}
	}
	return length;
}

void TemplateDef::sendWithServer(ESP8266HTMLServer *server) {
	uint32_t offset = 0;
	for (uint16_t i = 0; i < _tagCount; i++) {
		if (_tagTemp[i]) {
			server->sendContent_P(&_pattern[offset], (uint32_t)(_tags[i].tagPos - offset));
			_tagTemp[i]->sendWithServer(server);
			offset = _tags[i].tagPos + _tags[i].tagLen;
		}
	}
	if ((strlen_P(_pattern) - offset) > 0)
		server->sendContent_P(&_pattern[offset], (uint32_t)(strlen_P(_pattern) - offset));
	delete _tagTemp;
}

void Page::sendWithServer() {
	_server->sendLength(_templ->getLenght(_server));
	_templ->sendWithServer(_server);
}