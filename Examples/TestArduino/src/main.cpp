#include <Arduino.h>
#include <ESP8266WiFi.h>  //ESP8266 Core WiFi Library (you most likely already have this in your sketch)

uint32_t memGet() { return ESP.getFreeHeap(); }
uint32_t memPrint(const char* label, uint32_t old) {
	uint32_t n = ESP.getFreeHeap();
	printf("[%s] %u\r\n", label, old - n);
	return n;
}

#include <HTMLTemplate.h>
#include <HTMLTemplateServer.h>
#include <serverSerial.h>

static const char TEST[] PROGMEM = "Z_d_d @{a1}Mz3def _@toto__ @{a2}@{ddkkdkkkkdkkd}X6 _@{_@{}_@{Dernier}";

//using HTMLTemplate = TemplateDef<ServerSerial>;

ServerSerial* server;

String myfct() {
	printf("%s\r\n", WiFi.SSID().c_str());
	return WiFi.SSID();
}

void setup() {
	Serial.begin(115200);
	Serial.println("\r\nStarting");
	const char* ssid = "Mackenzie";
	const char* password = "E5F71122CD9474D4DA1746525E3EAE";
	WiFi.begin(ssid, password);
	uint8_t count = 0;
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		if (count++ > 40) {
			count = 0;
			Serial.println();
		}
		delay(100);
	}
	Serial.printf("\r\nConnected\r\n");
	server = new ServerSerial();

	// printf("Sizeof Template %u\r\n", sizeof(TemplateDef));
	printf("Sizeof TemplateTag %u\r\n", sizeof(TemplateTag));
	printf("Sizeof PGM_P %u\r\n", sizeof(PGM_P));
	
	uint32_t mm;
	mm = memGet();
	TemplateDef* pat = new TemplateDef(F("qsdf@{AA}q@{zzz}s")); //dfABC=@{abc}_____@toto__WIFI=@{wifi}____@{undef}___@{TagInt} ~~~~~ i=@{PtrInt}"));
	memPrint("Taille template 1", mm);
	delete pat;
	memPrint("Taille après template 1", mm);

	pat = new TemplateDef(F("qsdfqsdfABC=@{abc}_____@toto__WIFI=@{wifi}____@{undef}___@{TagInt} ~~~~~ i=@{PtrInt}"));
	// TemplateDef* pat = new TemplateDef(F("Z__ @{abc}Mz3def _@toto__ @{ghi}@{ddkkdkkkkdkkd}X6 _@{_@{}_@{Dernier}"));
	// TemplateDef* pat = new TemplateDef(F("Z__ {abc}Mz3def _@toto__ {ghi}ddkkdkkkkdkkd}X6 _@{_@{}_@{Dernier}"));
	// TemplateDef* pat = new TemplateDef(FPSTR(TEST));
	memPrint("Taille template", mm);
	printf("List les tags trouvé dans le Template :\r\n");
	server->setTemplate(pat);

	int i = 25;
	server->setTag("PtrInt", &i);
	const char* valeurABC = "ValeurABC";
	server->setTag("abc", valeurABC);
	server->setTag("TagInt", 10);
	server->setTag("unused", 23455);
	server->setTag("wifi", std::bind(&ESP8266WiFiSTAClass::SSID, WiFi));
	printf("Dictionnaire :\r\n");
	server->dump();
	printf("Construction page :\r\n");
	server->sendPage();
	i=50;
	server->sendPage();
	// WiFi.mode(WIFI_STA);
	// WiFi.disconnect();
	// int n = WiFi.scanNetworks();
	// printf("%u networks found\r\n",n);
	// for (int i = 0; i < n; i++) {
	// 	printf("%s\r\n",WiFi.SSID(i).c_str());
	// }
	// printf("%s\r\n", WiFi.SSID().c_str());
}

void loop() {
	// put your main code here, to run repeatedly:
}