#ifndef TIMELIB_H_INCLUDED
#define TIMELIB_H_INCLUDED

#define LUNA_31 2678400
#define NR_100 100
#define LUNA_30 2592000
#define ZI 86400
#define AN_365 31536000
#define HOUR 3600
#define MIN 60
#define IANUARIE 1
#define FEBRUARIE 2
#define MARTIE 3
#define APRILIE 4
#define MAI 5
#define IUNIE 6
#define IULIE 7
#define AUGUST 8
#define SEPTEMBRIE 9
#define OCTOMBRIE 10
#define NOIEMBRIE 11
#define DECEMBRIE 12
#define SIZE_STR 15
#define CEVA_NR 7
#define AN_1970 1970
#define LUNA_30_ZILE 30
#define NR_10 10

typedef struct {
    unsigned char hour;
    unsigned char min;
    unsigned char sec;
} TTime;

typedef struct {
    unsigned char day;
    unsigned char month;
    unsigned int year;
} TDate;

typedef struct {
    char name[NR_10/2];
    signed char utc_hour_difference;
} TTimezone;

typedef struct {
    TDate date;
    TTime time;
    TTimezone *tz;
    int durata;
    unsigned int unixTimestamp;
} TDateTimeTZ;

typedef struct {
    TDateTimeTZ *interval;
    char name[NR_10];
    int nrIntervale;
    unsigned int* liber;
    int size_lib;
    int timezone_index;
} TPersoane;

TTime convertUnixTimestampToTime(unsigned int timestamp);
TDate convertUnixTimestampToDateWithoutLeapYears(unsigned int timestamp);
TDate convertUnixTimestampToDate(unsigned int timestamp);
TDateTimeTZ convertUnixTimestampToDateTimeTZ(unsigned int timestamp, TTimezone *timezones, int timezone_index);
unsigned int convertDateTimeTZToUnixTimestamp(TDateTimeTZ datatz);
void printDateTimeTZ(TDateTimeTZ datetimetz);

#endif  // TIMELIB_H_INCLUDED
