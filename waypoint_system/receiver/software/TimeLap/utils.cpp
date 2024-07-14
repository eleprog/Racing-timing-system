#include "utils.h"

String int2timestamp(uint32_t timestamp) {
    String milliseconds = String(timestamp % 1000);
    while(milliseconds.length() < 3) {
        milliseconds = '0' + milliseconds;
    }
    String seconds = String(timestamp / 1000 % 60);
    while(seconds.length() < 2) {
        seconds = '0' + seconds;
    }
    String minutes = String(timestamp / 1000 / 60);

    String str = minutes + ":" + seconds + "." + milliseconds;
    return str;
}