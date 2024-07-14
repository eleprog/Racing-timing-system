#include "dataBase.h"

bool DB::init() {
    if(!SD.begin()) {
        return 0;
    }
        
    return 1;
}

bool DB::addRecord(String path, Record rec) {

    File file = SD.open(path, FILE_APPEND);
    if (!file) {
        return 0;
    }

    String strRecord = rec.racerName + "," + int2timestamp(rec.time) + "," + String(rec.time);

    file.println(strRecord.c_str());
	file.close();

    return 1;
}

int DB::getFilesAmt(String path) {
    File root = SD.open(path);
    if (!root) {
        return 0;
    }

    int filesAmt = 0;
    while(root.openNextFile()) {
        filesAmt++;
    }

    return filesAmt;
}

