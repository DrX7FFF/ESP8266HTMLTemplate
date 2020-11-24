#ifndef __HTMLTEMPLATESERVER_H__
#define __HTMLTEMPLATESERVER_H__

#include <FS.h>
#include <HTMLTemplate.h>
#include <TagWallet.h>

class HTMLTemplateServerDef : public TagWallet {
public:
	void setTemplate(HTMLTemplateDef *templ) { _templ = templ; };
	virtual void sendPage() { _templ->send(this); };
	virtual void sendContent_P(PGM_P content);
	virtual void sendContent(const char *content);
	virtual void sendContent_P(PGM_P content, uint32_t len);
	virtual void sendContent(std::string content){
		sendContent(content.c_str());
	};
protected:
	HTMLTemplateDef *_templ;
};

#endif  // __HTMLTEMPLATESERVER_H__