#include <HTMLTemplate.h>
#include <HTMLTemplateServer.h>
#include <TagWallet.h>

void TemplateDef::send(HTMLTemplateServerDef *server) {
	TagCache cache;
	size_t total = strlen_P(_pattern);
	for (uint16_t i = 0; i<_tagCount; i++){
		char tempName[_tags[i].nameLen+1];
		strncpy_P(tempName, &_pattern[_tags[i].tagPos+_tags[i].namePos], _tags[i].nameLen);
		tempName[_tags[i].nameLen] = '\0';
		if (server->exist(tempName)){
			total-=_tags[i].tagLen;
			total+= server->cacheTag(&cache,tempName);
		}
	}
	printf("Total à envoyer = %u\r\n", total);

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
		//  else
		//  	printf("Tag '%s' at %u not found\r\n", t.name, t.pos);
	}
	if ((strlen_P(_pattern) - offset)>0)
		server->sendContent_P(&_pattern[offset], (uint32_t)(strlen_P(_pattern)-offset));
	printf("\r\n");
	// printf("%.3s\r\n",&_pattern[3]);
};
