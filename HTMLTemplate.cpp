#include <HTMLTemplate.h>
#include <HTMLTemplateServer.h>

void HTMLTemplateDef::send(HTMLTemplateServerDef *server) {
	uint32_t offset = 0;
	for (HTMLTemplateTag &t : _tags) {
		HTMLTagValue * tag = server->getTagValue(std::string(t.name));
		if (tag){
			server->sendContent_P(&_pattern[offset], (uint32_t)(t.pos-offset));
			server->sendContent(tag->getValue());
			offset=t.pos + t.len;
		}
		 else
		 	printf("Tag '%s' at %u not found\r\n", t.name, t.pos);
	}
	if ((strlen_P(_pattern) - offset)>0)
		server->sendContent_P(&_pattern[offset], (uint32_t)(strlen_P(_pattern)-offset));
	printf("\r\n");
	// printf("%.3s\r\n",&_pattern[3]);
};
