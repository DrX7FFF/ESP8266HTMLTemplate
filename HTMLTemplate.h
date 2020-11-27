#ifndef __HTMLTEMPLATE_H__
#define __HTMLTEMPLATE_H__

#include <FS.h>
#include <regex.h>

#include <queue>

class ESP8266HTMLServer;
class TagBase;

struct TemplateTag {
	uint32_t tagPos;
	uint16_t tagLen;
	uint8_t namePos;
	uint8_t nameLen;
};

class TemplateDef {
public:
	TemplateDef(const __FlashStringHelper *pattern, const char *tagRegex = "@{([a-zA-Z0-9]+)}") : _pattern((PGM_P)pattern) {
		regex_t regexCompiled;
		int ret = regcomp(&regexCompiled, tagRegex, REG_EXTENDED);
		if (ret)
			printf("Error compile regex (%u)", ret);
		else {
			if (regexCompiled.re_nsub != 1)
				printf("One group and only one int regex string (group found=%u)\r\n", regexCompiled.re_nsub);
			else {
				std::queue<TemplateTag> fifoTag;
				regmatch_t matchs[2];
				String buffer(_pattern);
				uint32_t offset = 0;
				while (regexec(&regexCompiled, &buffer[offset], 2, matchs, 0) == 0) {
					fifoTag.push({offset + matchs[0].rm_so, (uint8_t)(matchs[0].rm_eo - matchs[0].rm_so), (uint8_t)(matchs[1].rm_so - matchs[0].rm_so), (uint8_t)(matchs[1].rm_eo - matchs[1].rm_so)});
					offset += matchs[0].rm_eo;
				}
				_tagCount = fifoTag.size();
				_tags = new TemplateTag[_tagCount];
				for (uint16_t i = 0; i < _tagCount; i++) {
					_tags[i] = fifoTag.front();
					fifoTag.pop();
				}
			}
		};
		regfree(&regexCompiled);
	};
	~TemplateDef() {
		free(_tags);
	};

	size_t getLenght(ESP8266HTMLServer *server);
	void sendWithServer(ESP8266HTMLServer *server);

private:
	PGM_P _pattern;
	TemplateTag *_tags;
	uint16_t _tagCount;
	TagBase **_tagTemp;
};

class Page {
public:
	Page(const __FlashStringHelper *pattern, const char *tagRegex = "@{([a-zA-Z0-9]+)}") {
		_templ = new TemplateDef(pattern);
	};
	void sendWithServer(ESP8266HTMLServer *server);

protected:
	TemplateDef *_templ;
};
#endif  // __HTMLTEMPLATE_H__