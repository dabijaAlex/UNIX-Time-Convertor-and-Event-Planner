#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timelib.h"

const int NR_5 = 5;
const unsigned int MAX_UNSIGNED = 4294967295;

int main() {
    int T = 0;
    scanf("%d", &T);
    TTimezone* timezones = malloc(T * sizeof(TTimezone));
    if (timezones == NULL) {
        free(timezones);
        return -1;
    }
    for (int i = 0; i < T; i++) {
        scanf("%s", timezones[i].name);
        scanf("%hhd", &timezones[i].utc_hour_difference);
    }
    int P = 0;
    int durataEv = 0, F = 0;
    char tz[NR_5];
    scanf("%d", &P);
    unsigned int *aux = NULL;
    unsigned int lowest_timestamp = MAX_UNSIGNED;
    unsigned int latest_timestamp = 0;
    TPersoane **persoane = malloc(P * sizeof(TPersoane *));
    if (persoane == NULL) {
        free(persoane);
        return -1;
    }
    for (int i = 0; i < P; i++) {
        persoane[i] = malloc(sizeof(TPersoane));
        if (persoane[i] == NULL) {
            return -1;
        }
        scanf("%s", persoane[i]->name);
        scanf("%s", tz);
        scanf("%d", &persoane[i]->nrIntervale);
        if (persoane[i]->nrIntervale == 0) {
            persoane[i]->liber = 0;
            persoane[i]->interval = malloc(sizeof(TDateTimeTZ));
            if (persoane[i]->interval == NULL) {
                free(persoane[i]->interval);
                return -1;
            }
            persoane[i]->interval->tz = malloc(sizeof(TTimezone));
            if (persoane[i]->interval->tz == NULL) {
                free(persoane[i]->interval->tz);
                return -1;
            }
            snprintf(persoane[i]->interval->tz->name, sizeof(persoane[i]->interval->tz->name), "AAA");
            persoane[i]->interval->tz->utc_hour_difference = 0;

            persoane[i]->interval->date.year = 0;
            persoane[i]->interval->date.month = 0;
            persoane[i]->interval->date.day = 0;
            persoane[i]->interval->time.hour = 0;
            persoane[i]->interval->durata = 0;
            persoane[i]->interval->time.min = 0;
            persoane[i]->interval->time.sec = 0;
            persoane[i]->interval->unixTimestamp = 0;
            persoane[i]->timezone_index = 0;
            persoane[i]->size_lib = 0;
        }
        if (persoane[i]->nrIntervale != 0) {
            persoane[i]->interval = malloc(persoane[i]->nrIntervale * sizeof(TDateTimeTZ));
            if (persoane[i]->interval == NULL) {
                free(persoane[i]->interval);
                return -1;
            }
            persoane[i]->size_lib = 0;
            persoane[i]->liber = malloc(1* sizeof(unsigned int));
            for (int j = 0; j < persoane[i]->nrIntervale; j++) {
                scanf("%u", &persoane[i]->interval[j].date.year);
                scanf("%hhu", &persoane[i]->interval[j].date.month);
                scanf("%hhu", &persoane[i]->interval[j].date.day);
                scanf("%hhu", &persoane[i]->interval[j].time.hour);
                scanf("%d", &persoane[i]->interval[j].durata);
                persoane[i]->interval[j].time.min = 0;
                persoane[i]->interval[j].time.sec = 0;
                persoane[i]->interval[j].tz = malloc(sizeof(TTimezone));
                if (persoane[i]->interval[j].tz == NULL) {
                    free(persoane[i]->interval[j].tz);
                    return -1;
                }
                unsigned long size = sizeof(persoane[i]->interval[j].tz->name);
                snprintf(persoane[i]->interval[j].tz->name, size, "%s", tz);
                for (int k = 0; k < T; k ++) {
                    if (strcmp(timezones[k].name, persoane[i]->interval[j].tz->name) == 0) {
                        persoane[i]->interval[j].tz->utc_hour_difference = timezones[k].utc_hour_difference;
                        persoane[i]->timezone_index = k;
                    }
                }
                persoane[i]->interval[j].unixTimestamp = convertDateTimeTZToUnixTimestamp(persoane[i]->interval[j]);
                int x = persoane[i]->size_lib + persoane[i]->interval[j].durata;
                aux = realloc(persoane[i]->liber, x * sizeof(unsigned int));
                if (aux)
                    persoane[i]->liber = aux;
                else
                    return -1;
                int c = 0;
                if (lowest_timestamp > persoane[i]->interval[j].unixTimestamp)
                    lowest_timestamp = persoane[i]->interval[j].unixTimestamp;
                for (int k = persoane[i]->size_lib; k < persoane[i]->size_lib + persoane[i]->interval[j].durata; k++) {
                    persoane[i]->liber[k] = persoane[i]->interval[j].unixTimestamp + c * HOUR;
                    if (latest_timestamp < persoane[i]->interval[j].unixTimestamp + c * HOUR)
                        latest_timestamp = persoane[i]->interval[j].unixTimestamp + c * HOUR;
                    c++;
                }
                persoane[i]->size_lib = persoane[i]->size_lib + persoane[i]->interval[j].durata;
            }
        }
    }
    scanf("%d", &F);
    scanf("%d", &durataEv);
    latest_timestamp = latest_timestamp - HOUR * (durataEv - 1);
    int ok = 0;
    unsigned int k = lowest_timestamp;
    while (k <= latest_timestamp) {
        int cati_liberi = 0;
        for (int i = 0; i < P; i++) {
            for (int j = 0; j < persoane[i]->size_lib; j++) {
                if (k == persoane[i]->liber[j]) {
                    if (j + durataEv - 1 < persoane[i]->size_lib) {
                        if ((persoane[i]->liber[j+durataEv-1] - persoane[i]->liber[j]) /HOUR == durataEv - 1)
                            cati_liberi++;
                    }
                }
            }
        }
        if (cati_liberi >= F) {
            ok = 1;
            break;
        }
        k = k + HOUR;
    }
    if (!ok) {
        printf("imposibil");
        for (int i = 0; i < P; i++) {
            for (int j = 0; j < persoane[i]->nrIntervale; j++) {
                free(persoane[i]->interval[j].tz);
            }
            if (persoane[i]->nrIntervale == 0)
                free(persoane[i]->interval->tz);
            free(persoane[i]->liber);
            free(persoane[i]->interval);
            free(persoane[i]);
        }
        free(persoane);
        free(timezones);
        return 0;
    }
    TPersoane *aux2 = NULL;
    TDateTimeTZ datetimetz;
    for (int i = 0; i < P-1; i++) {
        for (int j = i + 1; j < P; j ++) {
            if (strcmp(persoane[i]->name, persoane[j]->name) > 0) {
                aux2 = persoane[i];
                persoane[i] = persoane[j];
                persoane[j] = aux2;
            }
        }
    }
    for (int i = 0; i < P; i++) {
        int ok = 0;
        for (int j = 0; j < persoane[i]->size_lib; j++) {
            if (k == persoane[i]->liber[j]) {
                ok = 1;
            }
        }
        if (ok) {
            datetimetz = convertUnixTimestampToDateTimeTZ(k, timezones, persoane[i]->timezone_index);
            printf("%s: ", persoane[i]->name);
            printDateTimeTZ(datetimetz);
            printf("\n");
        } else {
            printf("%s: invalid\n", persoane[i]->name);
        }
    }
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < persoane[i]->nrIntervale; j++) {
            free(persoane[i]->interval[j].tz);
        }
        if (persoane[i]->nrIntervale == 0)
            free(persoane[i]->interval->tz);
        free(persoane[i]->liber);
        free(persoane[i]->interval);
        free(persoane[i]);
    }
    free(persoane);
    free(timezones);
    return 0;
}
