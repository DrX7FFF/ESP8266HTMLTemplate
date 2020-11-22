#ifndef __HTMLTEMPLATESERVER_H__
#define __HTMLTEMPLATESERVER_H__

#include <FS.h>
#include <HTMLTemplate.h>

#include <map>

class HTMLTagValue {
public:
	virtual char *getValue() { return nullptr; };
};

class TagWallet {
public:
	void setTagValue(std::string name, HTMLTagValue *value) { _objectMap[name] = value; };
	HTMLTagValue *getTagValue(const std::string name) { return _objectMap[name]; };

protected:
	std::map<std::string, HTMLTagValue *> _objectMap;
};

class HTMLTemplateServerDef : public TagWallet {
public:
	virtual void sendContent_P(PGM_P content);
	virtual void sendContent(const char *content);
	virtual void sendContent_P(PGM_P content, uint32_t len);
	virtual void sendPage() { _templ->send(this); };
	void setTemplate(HTMLTemplateDef *templ) { _templ = templ; };

protected:
	HTMLTemplateDef *_templ;
};

class HTMLTagValueString : public HTMLTagValue {
public:
	HTMLTagValueString() : HTMLTagValue(), _value(nullptr){};
	HTMLTagValueString(char *value) : HTMLTagValue(), _value(value){};
	virtual char *getValue() { return (char *)_value; };
	virtual void setValue(char *value) {
		free(_value);
		_value = value;
	};

protected:
	char *_value;
};

#endif  // __HTMLTEMPLATESERVER_H__