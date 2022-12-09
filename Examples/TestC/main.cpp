#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Base {
public:
	virtual string getValueStr() = 0;
};

template <class T>
class BaseObject : public Base {
public:
	template <typename T>
	BaseObject(T value) : _value(value){};
	virtual string getValueStr() {
		ostringstream sstr;
		sstr << _value;
		return sstr.str();
	};

protected:
	T _value;
};

// class BaseObjectInt : public BaseObject<int> {
// public:
// 	BaseObjectInt(int value) : BaseObject<int>(value){};
// };

// class BaseObjectString: public BaseObject<string>{
// public:
// 	BaseObjectString(string value) : BaseObject<string>(value) {};
// };

//typedef std::function<void(void)> THandlerFunction; //Pour ESP
typedef string (*HandlerFunction)();
class BaseObjectFct : public BaseObject<HandlerFunction> {
public:
	BaseObjectFct(HandlerFunction value) : BaseObject<HandlerFunction>(value){};
	virtual string getValueStr() { return _value(); };
};

class Wallet {
public:
	~Wallet() {
		for (const auto& kv : objectMap)
			delete kv.second;
		objectMap.clear();
	};
	template <typename T>
	void setValue(string name, T value) { objectMap[name] = new BaseObject<T>(value); };
	// void setValue(string name, int value) { objectMap[name] = new BaseObjectInt(value); };
	// void setValue(string name, string value) {objectMap[name] = new BaseObjectString(value);};
	void setValue(string name, HandlerFunction value) { objectMap[name] = new BaseObjectFct(value); };
	string getValueStr(const string& name) { return objectMap[name]->getValueStr(); };
	void dump() {
		for (const auto& kv : objectMap)
			printf("[%s]=%s\r\n", kv.first.c_str(), kv.second->getValueStr().c_str());
	};

protected:
	map<string, Base*> objectMap;
};

string wifiSSID() {
	return "Mackenzie";
}

int main() {
	Wallet* myW = new Wallet();
	int val = 10;

	myW->setValue("toto", val);
	myW->setValue("titi", "String Value");
	myW->setValue("wifi", wifiSSID);
	myW->dump();

	cout<<myW->getValueStr("toto")<<"\n";
	cout<<myW->getValueStr("titi")<<"\n";
	cout<<myW->getValueStr("wifi")<<"\n";

	delete myW;
}