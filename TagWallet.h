#ifndef __TAGWALLET_H__
#define __TAGWALLET_H__

#include <map>
//#include <sstream>
//#include <string.h>

struct nameCompare {bool operator()(char const *a, char const *b) { return strcmp(a, b) < 0; };};

template <class T>
class MapCharName {
public:
	~MapCharName() { clear(); };
	void clear() {
		for (const auto &kv : _tags) delete kv.second;
		_tags.clear();
	};
	bool exist(const char *name) { return (_tags.find(name) != _tags.end()); }

protected:
	std::map<const char *, T *, nameCompare> _tags;
};

typedef std::map<const char *, const char *>::iterator iteratorCharChar;
class TagCache : public MapCharName<const char> {
public:
	const char *getCache(const char *name) {
		iteratorCharChar i = _tags.find(name);
		return i != _tags.end() ? i->second : nullptr;
	};
	size_t setCache(const char *name, const char *value) {
		size_t res;
		std::pair<iteratorCharChar, bool> t = _tags.emplace(name, nullptr);
		if (!t.second)  // name doesnot exist
			res = strlen(t.first->second);
		else{
			res = strlen(value);
			t.first->second = new char[res + 1];
			strncpy((char *)t.first->second, value, res + 1);
		}
		return res;
	};
	void dump() {
		for (const auto &kv : _tags)
			printf("[%s] = %s\r\n", kv.first, kv.second);
	}
};

class TagBase {
public:
	virtual const char *getValueStr() = 0;
};

template <class T>
class TagAbstract : public TagBase {
public:
	TagAbstract(T value) : _value(value){};
	virtual const char *getValueStr() { return "(TagType undef)"; };  //TODO Debug
protected:
	T _value;
};

typedef std::function<String(void)> HandlerFunction;  //Pour ESP
//typedef string (*HandlerFunction)(); //Standar C
class TagFctPtr : public TagAbstract<HandlerFunction> {
public:
	TagFctPtr(HandlerFunction value) : TagAbstract<HandlerFunction>(value){};
	virtual const char *getValueStr() {
		// return std::bind(&ESP8266WiFiSTAClass::SSID, WiFi)().c_str();
		printf("%s\r\n", _value().c_str());
		return _value().c_str();
	};
};

class TagInt : public TagAbstract<int> {
public:
	TagInt(int value) : TagAbstract<int>(value) { printf("Construct Int\r\n"); };
	virtual const char *getValueStr() {
		String temp = String(_value);
		return temp.c_str();
	};

protected:
	// String temp;
};

class TagCharPtr : public TagAbstract<const char *> {
public:
	TagCharPtr(const char *value) : TagAbstract<const char *>(value) { printf("Construct Char *\r\n"); };
	virtual const char *getValueStr() { return _value; };
};

class TagString : public TagAbstract<String> {
public:
	TagString(String value) : TagAbstract<String>(value) { printf("Construct String\r\n"); };
	virtual const char *getValueStr() { return _value.c_str(); };
};

typedef std::map<const char *, TagBase *>::iterator iteratorCharTagBase;
class TagWallet : public MapCharName<TagBase> {
public:
	size_t cacheTag(TagCache* cacheObj, const char * name){
		if (!exist(name))
			return 0;
		const char * temp;
		
		return cacheObj->setCache(name,getTagStr(name));
	};

	const char *getTagStr(const char *name) {
		iteratorCharTagBase i = _tags.find(name);
		return i != _tags.end() ? i->second->getValueStr() : nullptr;
	};
	// template<typename T> void setTag(const char *name, T value) { _tags[name] = new TagAbstract<T>(value); };
	void setTag(const char *name, HandlerFunction value) {
		std::pair<iteratorCharTagBase, bool> res = _tags.emplace(name, nullptr);
		if (res.second)
			res.first->second = new TagFctPtr(value);
	};
	void setTag(const char *name, int value) {
		std::pair<iteratorCharTagBase, bool> res = _tags.emplace(name, nullptr);
		if (res.second)
			res.first->second = new TagInt(value);
	};
	void setTag(const char *name, const char *value) {
		std::pair<iteratorCharTagBase, bool> res = _tags.emplace(name, nullptr);
		if (res.second)
			res.first->second = new TagCharPtr(value);
	};
	void setTag(const char *name, String value) {
		std::pair<iteratorCharTagBase, bool> res = _tags.emplace(name, nullptr);
		if (res.second)
			res.first->second = new TagString(value);
	};
	void dump() {
		for (const auto &kv : _tags)
			printf("[%s] = %s\r\n", kv.first, kv.second->getValueStr());
	}
};

#endif  // __TAGWALLET_H__