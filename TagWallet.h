#ifndef __TAGWALLET_H__
#define __TAGWALLET_H__
#include <FS.h>

#include <map>

class ESP8266HTMLServer;
class TemplateDef;

class TagBase {
public:
	virtual size_t getLenght(ESP8266HTMLServer *server) = 0;
	virtual void sendWithServer(ESP8266HTMLServer *server) = 0;
};

template <class T>
class TagAbstract : public TagBase {
public:
	TagAbstract(T value) : _value(value){};

protected:
	T _value;
};

typedef std::function<String(void)> HandlerFunction;  //Pour ESP //typedef string (*HandlerFunction)(); //Standar C
class TagFctPtr : public TagAbstract<HandlerFunction> {
public:
	TagFctPtr(HandlerFunction value) : TagAbstract<HandlerFunction>(value){};
	virtual size_t getLenght(ESP8266HTMLServer *server) { return strlen(_value().c_str()); };
	virtual void sendWithServer(ESP8266HTMLServer *server);
};

class TagIntPtr : public TagAbstract<int *> {
public:
	TagIntPtr(int *value) : TagAbstract<int *>(value){};
	virtual size_t getLenght(ESP8266HTMLServer *server) {
		if (_cache)
			delete _cache;
		_cache = strdup(String(*_value).c_str());
		return strlen(_cache);
	};
	virtual void sendWithServer(ESP8266HTMLServer *server);

protected:
	const char *_cache = nullptr;
};

class TagStringPtr : public TagAbstract<const String*> {
public:
	TagStringPtr(const String *value) : TagAbstract<const String*>(value){};
	virtual size_t getLenght(ESP8266HTMLServer *server) { return (*_value).length(); };
	virtual void sendWithServer(ESP8266HTMLServer *server);
};


class TagCharPtr : public TagAbstract<const char *> {
public:
	TagCharPtr(const char *value) : TagAbstract<const char *>(value){};
	virtual size_t getLenght(ESP8266HTMLServer *server) { return strlen(_value); };
	virtual void sendWithServer(ESP8266HTMLServer *server);
};

class TagPROGMEM : public TagAbstract<PGM_P> {
public:
	TagPROGMEM(PGM_P value) : TagAbstract<PGM_P>((PGM_P)value){};
	virtual size_t getLenght(ESP8266HTMLServer *server) { return strlen_P(_value); };
	virtual void sendWithServer(ESP8266HTMLServer *server);
};

class TagTemplate : public TagAbstract<TemplateDef *> {
public:
	TagTemplate(TemplateDef *value) : TagAbstract<TemplateDef *>(value){};
	virtual size_t getLenght(ESP8266HTMLServer *server);
	virtual void sendWithServer(ESP8266HTMLServer *server);
};

struct nameCompare {
	bool operator()(char const *a, char const *b) { return strcmp(a, b) < 0; };
};
typedef std::map<const char *, TagBase *>::iterator itTagWallet;
class TagWallet {
public:
	~TagWallet() { clear(); };
	void clear() {
		for (const auto &kv : _tags)
			delete kv.second;
		_tags.clear();
	};
	TagBase *find(const char *name) {
		itTagWallet it = _tags.find(name);
		return it != _tags.end() ? it->second : nullptr;
	};

	// Pointeur sur une fonction
	void setTag(const char *name, HandlerFunction value) {
		std::pair<itTagWallet, bool> res = _tags.emplace(name, nullptr);
		if (res.second)
			res.first->second = new TagFctPtr(value);
	};
	// Entier constant
	void setTag(const char *name, int value) {
		std::pair<itTagWallet, bool> res = _tags.emplace(name, nullptr);
		if (res.second)
			res.first->second = new TagCharPtr(strdup(String(value).c_str()));
	};
	// Char * constant
	void setTag(const char *name, const char *value) {
		std::pair<itTagWallet, bool> res = _tags.emplace(name, nullptr);
		if (res.second)
			res.first->second = new TagCharPtr(value);
	};
	// Sting constant
	void setTag(const char *name, const String value) {
		std::pair<itTagWallet, bool> res = _tags.emplace(name, nullptr);
		if (res.second)
			res.first->second = new TagCharPtr(strdup(value.c_str()));
	};
	// Sting pointeur
	void setTag(const char *name, const String* value) {
		std::pair<itTagWallet, bool> res = _tags.emplace(name, nullptr);
		if (res.second)
			res.first->second = new TagStringPtr(value);
	};
	// Pointeur sur un entier
	//TODO gestion cache
	void setTag(const char *name, int *value) {
		std::pair<itTagWallet, bool> res = _tags.emplace(name, nullptr);
		if (res.second)
			res.first->second = new TagIntPtr(value);
	};
	// PROGMEM
	void setTag(const char *name, const __FlashStringHelper *value) {
		std::pair<itTagWallet, bool> res = _tags.emplace(name, nullptr);
		if (res.second)
			res.first->second = new TagPROGMEM((PGM_P)value);
	};
	// Template
	void setTag(const char *name, TemplateDef *value) {
		std::pair<itTagWallet, bool> res = _tags.emplace(name, nullptr);
		if (res.second)
			res.first->second = new TagTemplate(value);
	};
protected:
	std::map<const char *, TagBase *, nameCompare> _tags;
};

#endif  // __TAGWALLET_H__