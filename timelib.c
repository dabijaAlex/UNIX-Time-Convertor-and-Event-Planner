#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timelib.h"

#define secan 31536000
#define secanbis 31622400
#define secsi 86400

TTime convertUnixTimestampToTime(unsigned int timestamp) {
    TTime result;
    if (timestamp > ZI - 1)
        timestamp = timestamp - (timestamp / ZI) * ZI;
    result.hour = (unsigned char)(timestamp / HOUR);
    timestamp = timestamp - result.hour * HOUR;
    result.min = (unsigned char)(timestamp / MIN);
    timestamp = timestamp - result.min * MIN;
    result.sec = (unsigned char)timestamp;
    return result;
}

TDate convertUnixTimestampToDateWithoutLeapYears(unsigned int timestamp) {
    TDate result;
    result.day = 1;
    result.month = 1;
    result.year = AN_1970;
    if (timestamp < ZI)
        return result;
    result.year = result.year + (timestamp / AN_365);
    timestamp = timestamp - (timestamp / AN_365) * AN_365;
    int nrZile = 0, ok = 0;
    nrZile = (int)(timestamp / ZI);
    int i = 1;
    while (1) {
        if (i == 2 && nrZile < LUNA_30_ZILE - 2)
            break;
        if (i % 2 == 0 && nrZile < LUNA_30_ZILE)
            break;
        if (i % 2 == 1 && nrZile < LUNA_30_ZILE + 1)
            break;
        if (i % 2 == 1) {
            nrZile = nrZile - LUNA_30_ZILE - 1;
            result.month++;
        }
        if (i % 2 == 0 && i != 2) {
            nrZile = nrZile - LUNA_30_ZILE;
            result.month++;
        }
        if (i == 2) {
            nrZile = nrZile - (LUNA_30_ZILE - 2);
            result.month++;
        }
        if (i == IULIE && ok == 0) {
            i--;
            ok = 1;
        }
        i++;
    }
    result.day = 1 + nrZile;
    return result;
}

TDate convertUnixTimestampToDate(unsigned int timestamp) {
    int i = 1;
    int ok = 0;
    TDate result;
    result.day = 1;
    result.month = 1;
    result.year = AN_1970;
    if (timestamp < ZI)
        return result;
    while (timestamp >= ZI) {
        result.day++;
        timestamp = timestamp - ZI;
        if (i == 2) {
            if (result.year % 4 != 0) {
                if (result.day == LUNA_30_ZILE - 1) {
                        result.day = 1;
                        result.month++;
                        i++;
                    }
            } else {
                if (result.year % NR_100 == 0 && result.year % NR_100*4 != 0) {
                    if (result.day == LUNA_30_ZILE - 1) {
                        result.day = 1;
                        result.month++;
                        i++;
                    }
                } else {
                    if (result.day == LUNA_30_ZILE) {
                        result.day = 1;
                        result.month++;
                        i++;
                    }
                }
            }
        }
        if (i % 2 == 1) {
            if (result.day == LUNA_30_ZILE + 2) {
                result.month++;
                if (i == CEVA_NR && ok == 0) {
                    i--;
                    ok = 1;
                }
                i++;
                result.day = 1;
            }
        }
        if (i % 2 == 0) {
            if (result.day == LUNA_30_ZILE + 1) {
                result.month++;
                i++;
                result.day = 1;
            }
        }
        if (result.month == DECEMBRIE + 1) {
            result.month = 1;
            result.year++;
            i = 1;
            ok = 0;
        }
    }
    return result;
}

TDateTimeTZ convertUnixTimestampToDateTimeTZ(unsigned int timestamp, TTimezone *timezones, int timezone_index) {
    TDateTimeTZ result;
    timestamp = timestamp + HOUR*timezones[timezone_index].utc_hour_difference;
    result.time = convertUnixTimestampToTime(timestamp);
    result.date = convertUnixTimestampToDate(timestamp);
    result.tz = &timezones[timezone_index];
    return result;
}

unsigned int convertDateTimeTZToUnixTimestamp(TDateTimeTZ data_timezone) {
    unsigned int timestamp = 0;
    timestamp = timestamp + data_timezone.time.hour * HOUR;
    timestamp = timestamp + data_timezone.time.min * MIN;
    timestamp = timestamp + data_timezone.time.sec;
    for (int i = AN_1970; i < data_timezone.date.year; i++) {
        if (i % 4 == 0 && i % NR_100 !=0) {
            timestamp = timestamp + AN_365 + ZI;
        } else {
            if (i % NR_100 * 4 == 0)
                timestamp = timestamp + AN_365 + ZI;
            else
                timestamp = timestamp + AN_365;
        }
    }
    timestamp = timestamp + (data_timezone.date.day - 1) * ZI;
    int i = 1;
    while (i < data_timezone.date.month) {
        if (i % 2 == 1)
            timestamp = timestamp + LUNA_31;
        else
            timestamp = timestamp + LUNA_30;
        i++;
    }
    if (data_timezone.date.month > 2) {
        if (data_timezone.date.year % 4 == 0 && data_timezone.date.year % NR_100 !=0) {
                timestamp = timestamp - ZI;
            } else {
                if (data_timezone.date.year % (NR_100 * 4) == 0)
                    timestamp = timestamp - ZI;
                else
                    timestamp = timestamp - 2 * ZI;
            }
    }
    if (data_timezone.date.month > IULIE && data_timezone.date.month % 2 == 1)
        timestamp = timestamp + ZI;
    timestamp = timestamp - data_timezone.tz->utc_hour_difference * HOUR;
    return timestamp;
}
void printDateTimeTZ(TDateTimeTZ datetimetz) {
    char s[SIZE_STR];
    {  // dam lui s valoarea unei luni (string)
        if (datetimetz.date.month == IANUARIE)
            snprintf(s, sizeof(s), "%s", "ianuarie");
        if (datetimetz.date.month == FEBRUARIE)
            snprintf(s, sizeof(s), "%s", "februarie");
        if (datetimetz.date.month == MARTIE)
            snprintf(s, sizeof(s), "%s", "martie");
        if (datetimetz.date.month == APRILIE)
            snprintf(s, sizeof(s), "%s", "aprilie");
        if (datetimetz.date.month == MAI)
            snprintf(s, sizeof(s), "%s", "mai");
        if (datetimetz.date.month == IUNIE)
            snprintf(s, sizeof(s), "%s", "iunie");
        if (datetimetz.date.month == IULIE)
            snprintf(s, sizeof(s), "%s", "iulie");
        if (datetimetz.date.month == AUGUST)
            snprintf(s, sizeof(s), "%s", "august");
        if (datetimetz.date.month == SEPTEMBRIE)
            snprintf(s, sizeof(s), "%s", "septembrie");
        if (datetimetz.date.month == OCTOMBRIE)
            snprintf(s, sizeof(s), "%s", "octombrie");
        if (datetimetz.date.month == NOIEMBRIE)
            snprintf(s, sizeof(s), "%s", "noiembrie");
        if (datetimetz.date.month == DECEMBRIE)
            snprintf(s, sizeof(s), "%s", "decembrie");
    }
    printf("%02u %s %u, ", datetimetz.date.day, s, datetimetz.date.year);
    if (datetimetz.time.hour < NR_10)
        printf("0%u:", datetimetz.time.hour);
    else
        printf("%u:", datetimetz.time.hour);
    if (datetimetz.time.min < NR_10)
        printf("0%u:", datetimetz.time.min);
    else
        printf("%u:", datetimetz.time.min);
    if (datetimetz.time.sec < NR_10)
        printf("0%u ", datetimetz.time.sec);
    else
        printf("%u ", datetimetz.time.sec);

    printf("%s ", datetimetz.tz->name);
    if (datetimetz.tz->utc_hour_difference >= 0)
        printf("(UTC+%d)", datetimetz.tz->utc_hour_difference);
    else
        printf("(UTC%d)", datetimetz.tz->utc_hour_difference);
}
