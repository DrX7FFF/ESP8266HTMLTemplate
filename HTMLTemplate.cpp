#include <HTMLTemplate.h>
#include <HTMLTemplateServer.h>
#include <TagWallet.h>

void HTMLTemplateDef::send(HTMLTemplateServerDef *server) {
	TagCache cache;
	size_t total = strlen_P(_pattern);
	for (HTMLTemplateTag &t : _tags)
		if (server->exist(t.name)){
			total-=t.len;
			total+= server->cacheTag(&cache,t.name);
		}
	printf("Total à envoyer = %u\r\n", total);
	cache.dump();
/*
	uint32_t offset = 0;
	for (HTMLTemplateTag &t : _tags) {
		// HTMLTagValue * tag = server->getTagValue(std::string(t.name));
		if (server->exist(t.name)){
			server->sendContent_P(&_pattern[offset], (uint32_t)(t.pos-offset));
			// server->sendTag(std::string(t.name));
			// server->sendContent(server->getValue(std::string(t.name)));
			offset=t.pos + t.len;
		}
		 else
		 	printf("Tag '%s' at %u not found\r\n", t.name, t.pos);
	}
	if ((strlen_P(_pattern) - offset)>0)
		server->sendContent_P(&_pattern[offset], (uint32_t)(strlen_P(_pattern)-offset));
	printf("\r\n");
	// printf("%.3s\r\n",&_pattern[3]);
	*/
};
