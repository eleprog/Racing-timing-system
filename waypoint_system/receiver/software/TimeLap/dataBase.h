#include <SD.h>
#include <WString.h>

#include <utils.h>

namespace DB {
    typedef struct {
        String racerName;
        uint32_t time;
    } Record;

    bool init();
    bool addRecord(String path, Record rec);
    int getFilesAmt(String path);
}
