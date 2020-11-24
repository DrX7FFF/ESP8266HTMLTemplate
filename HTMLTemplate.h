#ifndef __HTMLTEMPLATE_H__
#define __HTMLTEMPLATE_H__

#include <FS.h>
#include <regex.h>
#include <forward_list>

class HTMLTemplateServerDef;

struct HTMLTemplateTag {
	char *name;
	uint32_t pos;
	size_t len;
	HTMLTemplateTag(char *name, uint32_t pos, size_t len)
		: name(name), pos(pos), len(len){};
};

class HTMLTemplateDef {
public:
	HTMLTemplateDef(const __FlashStringHelper *pattern, const char *tagRegex = "@{([a-zA-Z0-9]+)}") : _pattern((PGM_P)pattern) {
		regex_t regexCompiled;
		int ret = regcomp(&regexCompiled, tagRegex, REG_EXTENDED);
		if (ret)
			printf("Error compile regex (%u)", ret);
		else {
			if (regexCompiled.re_nsub != 1)
				printf("One group and only one int regex string (group found=%u)\r\n", regexCompiled.re_nsub);
			else {
				regmatch_t matchs[2];
				String buffer(_pattern);
				uint32_t offset = 0;
				while (regexec(&regexCompiled, &buffer[offset], 2, matchs, 0) == 0) {
					_tags.push_front({strndup(&buffer[offset + matchs[1].rm_so], matchs[1].rm_eo - matchs[1].rm_so), offset + matchs[0].rm_so, (size_t)(matchs[0].rm_eo - matchs[0].rm_so)});
					offset += matchs[0].rm_eo;
				}
				_tags.reverse();
			}
		};
		regfree(&regexCompiled);
	};
	~HTMLTemplateDef() {
		for (HTMLTemplateTag &t : _tags)
			free(t.name);
		_tags.clear();
	};
	void dump(){
		for (HTMLTemplateTag &t : _tags)
			printf("{%s,%u,%u}\r\n", t.name, t.pos, t.len);
	}
	void send(HTMLTemplateServerDef *server);

private:
	PGM_P _pattern;
	std::forward_list<HTMLTemplateTag> _tags;
};

#endif  // __HTMLTEMPLATE_H__