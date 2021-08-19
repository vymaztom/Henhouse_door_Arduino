#include <EEPROM.h>
#include <stdint.h>
#include <DHT.h>

#define DHTTYPE DHT22

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) < (Y) ? (Y) : (X))

#define SENSOR A3
#define BUT_UP  A2
#define BUT_DOWN A1


#define MOT_1 3
#define MOT_2 2
#define MOT_DIR 4

#define LED_PIN 5

#define FAN_1 6
#define FAN_2 7

#define DATA_1 8
#define DATA_2 9

#define END_2_UP 11
#define END_2_DOWN 10
#define END_1_UP 13
#define END_1_DOWN 12

const char text_0[] PROGMEM = ">";
const char text_1[] PROGMEM = "help                      -- print help menu\r\n";
const char text_2[] PROGMEM = "sensor                    -- return actual value of intensity\r\n";
const char text_3[] PROGMEM = "info                      -- print info about device\r\n";
const char text_4[] PROGMEM = "intensity UP <NUMBER>     -- set int number for intensity from range 0-1023\r\n";
const char text_5[] PROGMEM = "time <NUMBER>             -- set int number for time delay from range 0-1023\r\n";
const char text_6[] PROGMEM = "motor up                  -- move up\r\n";
const char text_7[] PROGMEM = "motor down                -- move down\r\n";
const char text_8[] PROGMEM = "motor end                 -- stop motor move\r\n";
const char text_9[] PROGMEM = "button null               -- set BUTTON to NONE\r\n";
const char text_10[] PROGMEM = "temperature on            -- set temperature sensor ON\r\n";
const char text_11[] PROGMEM = "temperature off           -- set temperature sensors OFF\r\n";
const char text_12[] PROGMEM = "fan <NUMBER> on           -- set fan by NUMBER on\r\n";
const char text_13[] PROGMEM = "fan <NUMBER> off          -- set fan by NUMBER off\r\n";
const char text_14[] PROGMEM = "led on                    -- set manualy LED on\r\n";
const char text_15[] PROGMEM = "led off                   -- set manualy LED off\r\n";
const char text_16[] PROGMEM = "humMax <NUMBER>           -- set max Humidity, NUMBER in range 0-100, if bigger FAN on\r\n";
const char text_17[] PROGMEM = "humStop <NUMBER>          -- set Humidity, NUMBER in range 0-100, when FAN become off\r\n";
const char text_18[] PROGMEM = "intensity DOWN <NUMBER>   -- set int number for intensity from range 0-1023\r\n";

const char OK[] PROGMEM = "OK\r\n";


enum STATE_ { UP, DOWN, GO_UP, GO_DOWN };
enum BUTTON_ { B_UP, B_DOWN, B_NONE};
STATE_ STATE = GO_UP;
BUTTON_ BUTTON = B_NONE;
uint8_t LED = 0;
uint16_t intensityUP = 0;
uint16_t intensityDOWN = 0;
uint16_t readTime = 0;
unsigned long _startMillis;
unsigned long _diffrentMillis;
uint8_t temperature = 0;
uint8_t fan_1_ = 0;
uint8_t fan_2_ = 0;
uint8_t LED_ = 0;
uint8_t MAXhum = 0;
uint8_t STOPhum = 0;


DHT dht1(DATA_1, DHTTYPE);
DHT dht2(DATA_2, DHTTYPE);

void setup(){
	delay(1000);
	Serial.begin(9600);
	Serial.println("Start");


	_startMillis = millis();

	/******************* SET PINS *************************/

	//OUTPUTs
	pinMode(MOT_1, OUTPUT);
	pinMode(MOT_2, OUTPUT);
	pinMode(MOT_DIR, OUTPUT);
	pinMode(LED_PIN, OUTPUT);
	pinMode(FAN_1, OUTPUT);
	pinMode(FAN_2, OUTPUT);

	// END INPUT
	pinMode(END_2_UP, INPUT);
	pinMode(END_2_DOWN, INPUT);
	pinMode(END_1_UP, INPUT);
	pinMode(END_1_DOWN, INPUT);

	// SET VALUE
	digitalWrite(MOT_1,0);
	digitalWrite(MOT_2,0);
	digitalWrite(LED_PIN,0);
	digitalWrite(FAN_1,0);
	digitalWrite(FAN_2,0);

	/************************ READ DATA FROM EEPROM ***************************/

	// read intensity
	intensityUP = EEPROM.read(0);
	intensityUP = intensityUP << 8;
	intensityUP = intensityUP + EEPROM.read(1);

	intensityDOWN = EEPROM.read(7);
	intensityDOWN = intensityDOWN << 8;
	intensityDOWN = intensityDOWN + EEPROM.read(8);

	// read time
	readTime = EEPROM.read(2);
	readTime = readTime << 8;
	readTime = readTime + EEPROM.read(3);
	_diffrentMillis = 1000 * 30 * readTime;

	// temperature
	temperature = (uint8_t)EEPROM.read(4);
	fan_1_ = (uint8_t)EEPROM.read(5);
	fan_2_ = (uint8_t)EEPROM.read(6);

	// max Humidity
	MAXhum = (uint8_t)EEPROM.read(9);
	STOPhum = (uint8_t)EEPROM.read(10);

	/*********************** Temperature **************************************/

	dht1.begin();
	dht2.begin();

}


void loop(){

	/************************ EXECUTE SERIAL **********************************/

	while(Serial.available()){
		String inputString = Serial.readString();
		printConsString(text_0);
		Serial.println(inputString);
		if(inputString.equals("help\n") || inputString.equals("help")){
			printConsString(text_1);
			printConsString(text_2);
			printConsString(text_3);
			printConsString(text_4);
			printConsString(text_18);
			printConsString(text_5);
			printConsString(text_6);
			printConsString(text_7);
			printConsString(text_8);
			printConsString(text_9);
			printConsString(text_10);
			printConsString(text_11);
			printConsString(text_12);
			printConsString(text_13);
			printConsString(text_14);
			printConsString(text_15);
			printConsString(text_16);
			printConsString(text_17);

		}else
		if(inputString.equals("sensor\n") || inputString.equals("sensor")){
			Serial.println(analogRead(SENSOR));
			printConsString(OK);
		}else
		if(inputString.equals("info\n") || inputString.equals("info")){
			Serial.print("Intensity now: ");
			Serial.println(analogRead(SENSOR));
			Serial.print("Intensity UP:  ");
			Serial.println(intensityUP);
			Serial.print("Intensity DOWN:");
			Serial.println(intensityDOWN);
			Serial.print("State:         ");
			switch (STATE) {
				case DOWN:
					Serial.println("DOWN");
					break;
				case UP:
					Serial.println("UP");
					break;
				case GO_UP:
					Serial.println("GO_UP");
					break;
				case GO_DOWN:
					Serial.println("GO_DOWN");
					break;
			}
			Serial.print("State BUTTON:  ");
			switch (BUTTON) {
				case B_UP:
					Serial.println("B_UP");
					break;
				case B_DOWN:
					Serial.println("B_DOWN");
					break;
				case B_NONE:
					Serial.println("B_NONE");
					break;
			}
			Serial.print("LED:           ");
			if(LED == 1){
				Serial.println("ON");
			}else{
				Serial.println("OFF");
			}
			Serial.print("Time set:      ");
			Serial.print(readTime);
			Serial.println(" half minits");
			if(temperature == 1){
				float hum1 = dht1.readHumidity();
				float temp1 = dht1.readTemperature();
				float hum2 = dht2.readHumidity();
				float temp2 = dht2.readTemperature();

				Serial.print("Temperature 1: ");
				Serial.print(temp1);
				Serial.println(" °C");
				Serial.print("Humidity    1: ");
				Serial.print(hum1);
				Serial.println(" %");
				Serial.print("Temperature 2: ");
				Serial.print(temp2);
				Serial.println(" °C");
				Serial.print("Humidity    2: ");
				Serial.print(hum2);
				Serial.println(" %");

				if(!(isnan(hum1) || isnan(temp1))){
					float T1 = temp1 - ((100-hum1)/5);
					Serial.print("Dew point 1:   ");
					Serial.print(T1);
					Serial.println(" °C");
				}
				if(!(isnan(hum2) || isnan(temp2))){
					float T2 = temp2 - ((100-hum2)/5);
					Serial.print("Dew point 2:   ");
					Serial.print(T2);
					Serial.println(" °C");
				}
				Serial.print("Humidity MAX:  ");
				Serial.print(MAXhum);
				Serial.println(" %");
				Serial.print("Humidity STOP: ");
				Serial.print(STOPhum);
				Serial.println(" %");
			}else{
				Serial.println("Temperature:   OFF");
			}
			if(fan_1_ == 1){
				Serial.println("Manul Fun 1:   ON");
			}else{
				Serial.println("Manul Fun 1:   OFF");
			}
			if(fan_2_ == 1){
				Serial.println("Manul Fun 2:   ON");
			}else{
				Serial.println("Manul Fun 2:   OFF");
			}
		}else
		if(inputString.substring(0,12) == "intensity UP"){
			int readValueOfIntensity = inputString.substring(13).toInt();
			if(readValueOfIntensity < 0){
				readValueOfIntensity = 0;
			}else if(readValueOfIntensity > 1023){
				readValueOfIntensity = 1023;
			}
			intensityUP = (uint16_t)readValueOfIntensity;
			EEPROM.write(0, intensityUP >> 8);
			EEPROM.write(1, intensityUP & 0xFF);
			printConsString(OK);
		}else
		if(inputString.substring(0,14) == "intensity DOWN"){
			int readValueOfIntensity = inputString.substring(15).toInt();
			if(readValueOfIntensity < 0){
				readValueOfIntensity = 0;
			}else if(readValueOfIntensity > 1023){
				readValueOfIntensity = 1023;
			}
			intensityDOWN = (uint16_t)readValueOfIntensity;
			EEPROM.write(7, intensityDOWN >> 8);
			EEPROM.write(8, intensityDOWN & 0xFF);
			printConsString(OK);
		}else
		if(inputString.substring(0,6) == "humMax"){
			int readValueHum = inputString.substring(7).toInt();
			if(readValueHum < 0){
				readValueHum = 0;
			}else if(readValueHum > 100){
				readValueHum = 100;
			}
			MAXhum = (uint16_t)readValueHum;
			EEPROM.write(9, MAXhum);
			printConsString(OK);
		}else
		if(inputString.substring(0,7) == "humStop"){
			int readValueHum = inputString.substring(8).toInt();
			if(readValueHum < 0){
				readValueHum = 0;
			}else if(readValueHum > 100){
				readValueHum = 100;
			}
			STOPhum = (uint16_t)readValueHum;
			EEPROM.write(10, STOPhum);
			printConsString(OK);
		}else
		if(inputString.substring(0,4) == "time"){
			int readValueOfTime = inputString.substring(5).toInt();
			if(readValueOfTime < 0){
				readValueOfTime = 0;
			}else if(readValueOfTime > 1023){
				readValueOfTime = 1023;
			}
			readTime = (uint16_t)readValueOfTime;
			EEPROM.write(2, readTime >> 8);
			EEPROM.write(3, readTime & 0xFF);
			_diffrentMillis = 1000 * 30 * readTime;
			printConsString(OK);
		}else
		if(inputString.equals("motor up\n") || inputString.equals("motor up")){
			if((STATE == GO_DOWN)||(STATE == DOWN)){
				BUTTON = B_UP;
				digitalWrite(MOT_1, 0);
				digitalWrite(MOT_2, 0);
				delay(500);
				printConsString(OK);
			}
			STATE = GO_UP;
		}else
		if(inputString.equals("motor down\n") || inputString.equals("motor down")){
			if((STATE == GO_UP)||(STATE == UP)){
				BUTTON = B_DOWN;
				digitalWrite(MOT_1, 0);
				digitalWrite(MOT_2, 0);
				delay(500);
				printConsString(OK);
			}
			STATE = GO_DOWN;
		}else
		if(inputString.equals("motor end\n") || inputString.equals("motor end")){
			digitalWrite(MOT_1, 0);
			digitalWrite(MOT_2, 0);
			digitalWrite(MOT_DIR, 0);
			if(STATE == GO_DOWN){
				STATE = DOWN;
			}else if(STATE == GO_UP){
				STATE = UP;
			}
			printConsString(OK);
		}else
		if(inputString.equals("button null\n") || inputString.equals("button null")){
			BUTTON = B_DOWN;
			printConsString(OK);
		}else
		if(inputString.equals("temperature on\n") || inputString.equals("temperature on")){
			temperature = 1;
			EEPROM.write(4, temperature);
			printConsString(OK);
		}else
		if(inputString.equals("temperature off\n") || inputString.equals("temperature off")){
			temperature = 0;
			EEPROM.write(4, temperature);
			printConsString(OK);
		}else
		if(inputString.equals("fan 1 on\n") || inputString.equals("fan 1 on")){
			fan_1_ = 1;
			EEPROM.write(5, fan_1_);
			digitalWrite(FAN_1, fan_1_);
			printConsString(OK);
		}else
		if(inputString.equals("fan 2 on\n") || inputString.equals("fan 2 on")){
			fan_2_ = 1;
			EEPROM.write(6, fan_2_);
			digitalWrite(FAN_2, fan_2_);
			printConsString(OK);
		}else
		if(inputString.equals("fan 1 off\n") || inputString.equals("fan 1 off")){
			fan_1_ = 0;
			EEPROM.write(5, fan_1_);
			digitalWrite(FAN_1, fan_1_);
			printConsString(OK);
		}else
		if(inputString.equals("fan 2 off\n") || inputString.equals("fan 2 off")){
			fan_2_ = 0;
			EEPROM.write(6, fan_2_);
			digitalWrite(FAN_2, fan_2_);
			printConsString(OK);
		}else
		if(inputString.equals("led on\n") || inputString.equals("led on")){
			LED_ = 1;
			printConsString(OK);
		}else
		if(inputString.equals("led off\n") || inputString.equals("led off")){
			LED_ = 0;
			printConsString(OK);
		}

	}


	/************************ EXECUTE INPUTS **********************************/

	// HANDLE BUTTON UP
	if(analogRead(BUT_UP) > 800){
		if((STATE == GO_DOWN)||(STATE == DOWN)){
			BUTTON = B_UP;
			LED = 0;
			digitalWrite(MOT_1, 0);
			digitalWrite(MOT_2, 0);
			delay(500);
		}
		STATE = GO_UP;
	}

	// HANDLE BUTTON DOWN
	if(analogRead(BUT_DOWN) > 800){
		if((STATE == GO_UP)||(STATE == UP)){
			BUTTON = B_DOWN;
			LED = 0;
			digitalWrite(MOT_1, 0);
			digitalWrite(MOT_2, 0);
			delay(500);
		}
		STATE = GO_DOWN;
	}


	if(temperature == 1){
		float hum1 = dht1.readHumidity();
 		float temp1 = dht1.readTemperature();
		float hum2 = dht2.readHumidity();
 		float temp2 = dht2.readTemperature();
		if((!(isnan(hum1) || isnan(temp1))) && (fan_1_ == 0)){
			if(MAXhum <= hum1){
				digitalWrite(FAN_1, 1);
			}
			if(STOPhum >= hum1){
				digitalWrite(FAN_1, 0);
			}
		}else{
			if(fan_1_ == 0){
				digitalWrite(FAN_1, 0);
			}
		}
		if((!(isnan(hum2) || isnan(temp2))) && (fan_2_ == 0)){
			if(MAXhum <= hum2){
				digitalWrite(FAN_2, 1);
			}
			if(STOPhum >= hum2){
				digitalWrite(FAN_2, 0);
			}
		}else{
			if(fan_2_ == 0){
				digitalWrite(FAN_2, 0);
			}
		}
	}else{
		digitalWrite(FAN_1, 0);
		digitalWrite(FAN_2, 0);
	}

	digitalWrite(LED_PIN, (LED || LED_));


	// GO_DOWN
	if((analogRead(SENSOR) <= intensityDOWN) && (LED == 0) && (BUTTON != B_UP) && (STATE != DOWN) && (STATE != GO_DOWN)){
		Serial.print("INNTERUP GO DOWN ");
		Serial.print(analogRead(SENSOR));
		Serial.print(" <= ");
		Serial.println(intensityDOWN);
		LED = 1;
		_startMillis = millis();
	}

	// GO_DOWN
	unsigned long diffrentMillis = millis() - _startMillis;
	if((diffrentMillis > _diffrentMillis) && (LED == 1)){
		LED = 0;
		BUTTON = B_NONE;
		STATE = GO_DOWN;
	}
	if((analogRead(SENSOR) <= intensityDOWN) && (BUTTON == B_DOWN) && (STATE == DOWN)){
		BUTTON = B_UP;
	}


	// GO_UP
	if((analogRead(SENSOR) > intensityUP) && (BUTTON != B_DOWN) && (STATE != UP) && (STATE != GO_UP)){
		Serial.print("INNTERUP GO UP ");
		Serial.print(analogRead(SENSOR));
		Serial.print(" <= ");
		Serial.println(intensityUP);
		BUTTON = B_NONE;
		STATE = GO_UP;
	}
	if((analogRead(SENSOR) > intensityUP) && (BUTTON == B_UP) && (STATE == UP)){
		BUTTON = B_DOWN;
	}

	/************************ EXECUTE SERIAL **********************************/
	//delay(2000);
	// MOVE ACTION
	switch (STATE) {
		case UP:
			if(digitalRead(END_1_UP))								{	digitalWrite(MOT_1, 0);		}
			if(digitalRead(END_2_UP))								{	digitalWrite(MOT_2, 0);		}
			break;
		case GO_UP:
			digitalWrite(MOT_DIR, 0);
			digitalWrite(MOT_1, 1);
			digitalWrite(MOT_2, 1);
			STATE = UP;
			break;
		case DOWN:
			if(digitalRead(END_1_DOWN))								{	digitalWrite(MOT_1, 0);				}
			if(digitalRead(END_2_DOWN))								{	digitalWrite(MOT_2, 0);				}
			if(digitalRead(END_1_DOWN) && digitalRead(END_2_DOWN))	{	digitalWrite(MOT_DIR, 0); LED = 0;  }
			break;
		case GO_DOWN:
			digitalWrite(MOT_DIR, 1);
			digitalWrite(MOT_1, 1);
			digitalWrite(MOT_2, 1);
			STATE = DOWN;
			break;
	}
}


void printConsString(const char* text_){
	uint8_t len = strlen_P(text_);
	char charText;

	for (uint8_t i = 0; i < len; i++){
		charText =  pgm_read_byte_near(text_ + i);
		Serial.print(charText);
	}
}
