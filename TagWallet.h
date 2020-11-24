#ifndef __TAGWALLET_H__
#define __TAGWALLET_H__

#include <map>

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

typedef std::map<const char *, const char *>::iterator itTagCache;
class TagCache : public MapCharName<const char> {
public:
	const char *getCache(const char *name) {
		itTagCache i = _tags.find(name);
		return i != _tags.end() ? i->second : nullptr;
	};
	size_t copyCache(const char *name, const char *value) {
		size_t res;
		std::pair<itTagCache, bool> t = _tags.emplace(name, nullptr);
		if (!t.second)  // name doesnot exist
			res = strlen(t.first->second);
		else {
			res = strlen(value);
			t.first->second = new char[res + 1];
			strcpy((char *)t.first->second, value);
		}
		return res;
	};

	size_t setCache(const char *name, const char *value) {
		std::pair<itTagCache, bool> t = _tags.emplace(name, nullptr);
		if (t.second)  // name doesnot exist
			t.first->second = value;
		return strlen(t.first->second);
	}
	void dump() {
		for (const auto &kv : _tags)
			printf("[%s] = %s\r\n", kv.first, kv.second);
	}
};

class TagBase {
public:
	virtual const char *getValueStr() = 0;
	virtual const char *dupValue() = 0;
	virtual void printValue() = 0;
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
	virtual const char *getValueStr() { return _value().c_str(); };
	virtual const char *dupValue() { return strdup(_value().c_str()); }
	virtual void printValue() { printf("%s (%u)[FctPtr]", _value().c_str(), strlen(_value().c_str())); }
};

class TagInt : public TagAbstract<int> {
public:
	TagInt(int value) : TagAbstract<int>(value){};
	virtual const char *getValueStr() { return String(_value).c_str(); };
	virtual const char *dupValue() { return strdup(String(_value).c_str()); }
	virtual void printValue() {
		String temp = String(_value);
		printf("%s (%u)[Int]", temp.c_str(), strlen(temp.c_str()));
	}
};

class TagIntPtr : public TagAbstract<int*> {
public:
	TagIntPtr(int* value) : TagAbstract<int*>(value){};
	virtual const char *getValueStr() { return String(*_value).c_str(); };
	virtual const char *dupValue() { return strdup(String(*_value).c_str()); }
	virtual void printValue() {
		String temp = String(*_value);
		printf("%s (%u)[Int]", temp.c_str(), strlen(temp.c_str()));
	}
};

class TagCharPtr : public TagAbstract<const char *> {
public:
	TagCharPtr(const char *value) : TagAbstract<const char *>(value){};
	virtual const char *getValueStr() { return _value; };
	virtual const char *dupValue() { return strdup(_value); }
	virtual void printValue() { printf("%s (%u)[Char*]", _value, strlen(_value)); }
};

class TagString : public TagAbstract<String> {
public:
	TagString(String value) : TagAbstract<String>(value){};
	virtual const char *getValueStr() { return _value.c_str(); };
	virtual const char *dupValue() { return strdup(_value.c_str()); }
	virtual void printValue() { printf("%s (%u)[String]", _value.c_str(), strlen(_value.c_str())); }
};

typedef std::map<const char *, TagBase *>::iterator itTagWallet;
class TagWallet : public MapCharName<TagBase> {
public:
	size_t cacheTag(TagCache *cacheObj, const char *name) {
		const char *temp = cacheObj->getCache(name);
		if (temp)  //allready in cache
			return strlen(temp);

		itTagWallet i = _tags.find(name);
		if (i == _tags.end())  //Taginconnu
			return 0;

		return cacheObj->setCache(name, i->second->dupValue());
	};

	const char *getTagStr(const char *name) {
		itTagWallet i = _tags.find(name);
		return i != _tags.end() ? i->second->getValueStr() : nullptr;
	};
	// template<typename T> void setTag(const char *name, T value) { _tags[name] = new TagAbstract<T>(value); };
	void setTag(const char *name, HandlerFunction value) {
		std::pair<itTagWallet, bool> res = _tags.emplace(name, nullptr);
		if (res.second)
			res.first->second = new TagFctPtr(value);
	};
	void setTag(const char *name, int value) {
		std::pair<itTagWallet, bool> res = _tags.emplace(name, nullptr);
		if (res.second)
			res.first->second = new TagInt(value);
	};
	void setTag(const char *name, const char *value) {
		std::pair<itTagWallet, bool> res = _tags.emplace(name, nullptr);
		if (res.second)
			res.first->second = new TagCharPtr(value);
	};
	void setTag(const char *name, String value) {
		std::pair<itTagWallet, bool> res = _tags.emplace(name, nullptr);
		if (res.second)
			res.first->second = new TagString(value);
	};
	void setTag(const char *name, int* value) {
		std::pair<itTagWallet, bool> res = _tags.emplace(name, nullptr);
		if (res.second)
			res.first->second = new TagIntPtr(value);
	};
	void dump() {
		for (const auto &kv : _tags) {
			printf("[%s] = ", kv.first);
			kv.second->printValue();
			printf(" \r\n");
		}
	}
};

#endif  // __TAGWALLET_H__