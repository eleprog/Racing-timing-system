#include "dataBase.h"
#include "utils.h"
#include <map>
#include <iterator>

#define IR_GATE_PIN 13
#define LED_PIN 32

#define MIN_DETECT_TIME 75 // ms

volatile uint32_t detectTimestamp = 0;
uint32_t prevDetectTimestamp = 0;
volatile bool isCarDetect = false;

std::map<String, uint32_t> bestTimeRacers;

bool isDbInit = false;

String racingSessionPath = "/racing_sessions";

String racerName = "";
int maxTime = 120;  // seconds (default 120 sec)
int minTime = 0;    // seconds (default 0 sec)

int strLen(String str) {
    size_t len = 0; 
    for (size_t i = 0; i < str.length(); i++)
        len += ((str[i] & 0xc0) != 0x80);
    return len;
}

inline void serialReadHandler() {
    String str = Serial.readString();
    if(str.indexOf("name") != -1) {
        racerName = str.substring(5);
        Serial.print("Racer name: "); Serial.println(racerName);

    }else if(str.indexOf("max ") != -1) {
        String maxStr = str.substring(4);
        maxTime = maxStr.toInt();
        Serial.print("max time: "); Serial.println(maxTime);

    }else if(str.indexOf("min ") != -1) {
        String minStr = str.substring(4);
        minTime = minStr.toInt();
        Serial.print("min time: "); Serial.println(minTime);

    }else if(str.indexOf("table") != -1) {
        int numRecords = bestTimeRacers.size();
        int sortIndex[numRecords];
        for(int i = 0; i < numRecords; i++) {
            sortIndex[i] = i;
        }

        for(int i = 0; i < numRecords; i++) {
            for (int j = 0; j < numRecords - 1; j++) {
                auto record1{bestTimeRacers.begin()};
                std::advance(record1, sortIndex[j]);
                auto record2{bestTimeRacers.begin()};
                std::advance(record2, sortIndex[j + 1]);

                if(record1->second > record2->second) {
                    int tmp = sortIndex[j + 1];
                    sortIndex[j + 1] = sortIndex[j];
                    sortIndex[j] = tmp;
                }
            }
        }
        Serial.println("\n+-----------------------+----------+");
        Serial.println("|         Имя           |   время  |");
        Serial.println("+=======================+==========+");
        for(int i = 0; i < numRecords; i++) {
            auto record{bestTimeRacers.begin()};
            std::advance(record, sortIndex[i]);

            if(record->first == "")
                continue;

            Serial.print("| ");
            Serial.print(record->first);
            if(strLen(record->first) < 24) Serial.print("\t");
            if(strLen(record->first) < 16) Serial.print("\t");
            if(strLen(record->first) < 8)  Serial.print("\t");
            Serial.print("| ");
            Serial.print(int2timestamp(record->second));
            Serial.println(" |");
        }
        Serial.println("+-----------------------+----------+\n");
            
    }else {
        Serial.println("unknown command");
    }
}

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
        if(minTime * 1000 < deltaTime && deltaTime < maxTime * 1000) {
            if(isDbInit) {
                DB::Record record{racerName, deltaTime};
                if(!DB::addRecord(racingSessionPath, record)) {
                    Serial.println("DB error");
                    isDbInit = false;
                }
            }
            Serial.print("Name: "); Serial.print(racerName);

            if(racerName == "") Serial.print("\t");
            if(strLen(racerName) < 8) Serial.print("\t");

            Serial.print("\tTime: "); Serial.print(int2timestamp(deltaTime));

            if(!bestTimeRacers.contains(racerName) || bestTimeRacers[racerName] > deltaTime) {
                if(racerName != "") {
                    bestTimeRacers[racerName] = deltaTime;
                    Serial.print("\tbest");
                }
            }
            Serial.println();

        } else {
            Serial.print("drop time: "); Serial.println(int2timestamp(deltaTime));
        }

        prevDetectTimestamp = detectTimestamp;
        detectTimestamp = 0;
        isCarDetect = false;
    }

    // Приём данных из последовательного порта
    if (Serial.available()) {
        serialReadHandler();
    }
    
    //digitalWrite(LED_PIN, !digitalRead(IR_GATE_PIN));
}
