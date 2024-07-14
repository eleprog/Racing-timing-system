#include "dataBase.h"
#include "utils.h"

#define IR_GATE_PIN 13
#define LED_PIN 32

#define MIN_DETECT_TIME 75 // ms

volatile uint32_t detectTimestamp = 0;
uint32_t prevDetectTimestamp = 0;
volatile bool isCarDetect = false;

bool isDbInit = false;

String racingSessionPath = "/racing_sessions";

String racerName;
int maxTime = 300;  // seconds (default 300 sec)
int minTime = 0;    // seconds (default 0 sec)

void IRAM_ATTR carGateHandler() {
	if(digitalRead(IR_GATE_PIN)) {
		detectTimestamp = millis();
	}
	else {
        if(millis() - detectTimestamp > MIN_DETECT_TIME && detectTimestamp) {
            isCarDetect = true;
        }    
	}
}

void setup() {
    Serial.begin(115200);

    if(DB::init()){
        Serial.println("DB init success");
        isDbInit = true;
    }
    else{
        Serial.println("DB init failed");
        isDbInit = false;
    }
        
    int filesAmt = DB::getFilesAmt(racingSessionPath);
    racingSessionPath += "/session_" + String(filesAmt + 1) + ".csv";

    pinMode(IR_GATE_PIN, INPUT);
    attachInterrupt(IR_GATE_PIN, carGateHandler, CHANGE);
}

void loop() {
    // обработчик детекта машины
    if(isCarDetect) {
        uint32_t deltaTime = detectTimestamp - prevDetectTimestamp;

        if(isDbInit) {
            DB::Record record{racerName, deltaTime};
            if(!DB::addRecord(racingSessionPath, record)) {
                Serial.println("DB error");
                isDbInit = false;
            }
        }
        Serial.print("Name: "); Serial.print(racerName);
        Serial.print("\t Time: "); Serial.println(int2timestamp(deltaTime));

        prevDetectTimestamp = detectTimestamp;
        detectTimestamp = 0;
        isCarDetect = false;
    }

    // Приём данных из последовательного порта
    if (Serial.available()) {
        String str = Serial.readString();
        if(str.indexOf("name ") != -1) {
            racerName = str.substring(5);
            Serial.print("racer name: "); Serial.println(racerName);
        } else {
            Serial.println("unknown command");
        }
    }
    //digitalWrite(LED_PIN, !digitalRead(IR_GATE_PIN));
}
