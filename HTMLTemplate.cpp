#include <HTMLTemplate.h>
#include <TagWallet.h>
#include <ESP8266HTMLServer.h>

void TemplateDef::sendWithServer(ESP8266HTMLServer *server) {
	TagCache cache;
	size_t length = strlen_P(_pattern);
	
	for (uint16_t i = 0; i<_tagCount; i++){
		char tempName[_tags[i].nameLen+1];
		strncpy_P(tempName, &_pattern[_tags[i].tagPos+_tags[i].namePos], _tags[i].nameLen);
		tempName[_tags[i].nameLen] = '\0';
		if (server->exist(tempName)){
			length-=_tags[i].tagLen;
			length+= server->cacheTag(&cache,tempName);
		}
		//  else
		//  	printf("Tag '%s' at %u not found\r\n", t.name, t.pos);
	}

	server->sendLength(length);

	uint32_t offset = 0;
	for (uint16_t i = 0; i<_tagCount; i++){
		char tempName[_tags[i].nameLen+1];
		strncpy_P(tempName, &_pattern[_tags[i].tagPos+_tags[i].namePos], _tags[i].nameLen);
		tempName[_tags[i].nameLen] = '\0';
		const char * tagValue = cache.getCache(tempName);
		if (tagValue){
			server->sendContent_P(&_pattern[offset], (uint32_t)(_tags[i].tagPos-offset));
			server->sendContent(tagValue);
			offset=_tags[i].tagPos + _tags[i].tagLen;
		}
	}
	if ((strlen_P(_pattern) - offset)>0)
		server->sendContent_P(&_pattern[offset], (uint32_t)(strlen_P(_pattern)-offset));
};
