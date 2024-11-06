#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timelib.h"
const unsigned int mask_bits_day = 31;
const unsigned int mask_bits_month = 480;
const unsigned int mask_bits_year = 32256;
const int SHIFT_NR_BITS_OF_DAYS = 5;
const int SHIFT_DAYS_AND_MONTHS = 9;
const int TASK_7 = 7, TASK_8 = 8;
const int BITS_32 = 32;
const int NR_15 = 15;

void prelucrare_cate_o_data(unsigned int data_in_biti, int i, TDate v_date[]) {
    unsigned int mask = mask_bits_day;
    v_date[i].day = data_in_biti & mask;
    mask = mask_bits_month;
    v_date[i].month = (data_in_biti & mask) >> SHIFT_NR_BITS_OF_DAYS;
    mask = mask_bits_year;
    data_in_biti = data_in_biti & mask;
    v_date[i].year = AN_1970 + (data_in_biti >> SHIFT_DAYS_AND_MONTHS);
}
void sortare_crescator(int N, TDate v_date[]) {
    for (int i = 0; i < N-1; i++) {
        for (int j = i; j < N; j++) {
            if (v_date[i].year > v_date[j].year) {
                TDate aux;
                aux = v_date[i];
                v_date[i] = v_date[j];
                v_date[j] = aux;
            }
            if (v_date[i].year == v_date[j].year) {
                if (v_date[i].month > v_date[j].month) {
                    TDate aux;
                    aux = v_date[i];
                    v_date[i] = v_date[j];
                    v_date[j] = aux;
                }
                if (v_date[i].month == v_date[j].month) {
                    if (v_date[i].day > v_date[j].day) {
                        TDate aux;
                        aux = v_date[i];
                        v_date[i] = v_date[j];
                        v_date[j] = aux;
                    }
                }
            }
        }
    }
}
void din_nr_in_cuv(char *s, unsigned char x, int size) {
    if (x == IANUARIE)
        snprintf(s, size, "%s", "ianuarie");
    if (x == FEBRUARIE)
        snprintf(s, size, "%s", "februarie");
    if (x == MARTIE)
        snprintf(s, size, "%s", "martie");
    if (x == APRILIE)
        snprintf(s, size, "%s", "aprilie");
    if (x == MAI)
        snprintf(s, size, "%s", "mai");
    if (x == IUNIE)
        snprintf(s, size, "%s", "iunie");
    if (x == IULIE)
        snprintf(s, size, "%s", "iulie");
    if (x == AUGUST)
        snprintf(s, size, "%s", "august");
    if (x == SEPTEMBRIE)
        snprintf(s, size, "%s", "septembrie");
    if (x == OCTOMBRIE)
        snprintf(s, size, "%s", "octombrie");
    if (x == NOIEMBRIE)
        snprintf(s, size, "%s", "noiembrie");
    if (x == DECEMBRIE)
        snprintf(s, size, "%s", "decembrie");
}

// nu uita sa dezaloci v_date la sfarsit
int main() {
    int nr_task = 0, N = 0;
    unsigned int o_data_in_biti = 0;
    typedef struct {
        int x, y;
    } memoram_int;
    scanf("%d", &nr_task);
    if (nr_task == TASK_7) {
        scanf("%d", &N);
        TDate *v_date = malloc(N * sizeof(TDate));
        for (int i = 0; i < N; i++) {
            scanf("%u", &o_data_in_biti);
            prelucrare_cate_o_data(o_data_in_biti, i, v_date);
        }
        sortare_crescator(N, v_date);
        for (int i = 0; i < N; i++) {
            char s[SIZE_STR];
            din_nr_in_cuv(s, v_date[i].month, sizeof(s));
            printf("%u %s %u\n", v_date[i].day, s, v_date[i].year);
        }
    }
    if (nr_task == TASK_8) {
        unsigned int o_data_in_biti = 0;
        int size_resturi = 0;
        int cati_biti_cititi = 0;
        unsigned int *pachet_date = malloc(N*sizeof(unsigned int));
        unsigned int resturi = 0;
        unsigned int mask = mask_bits_day + mask_bits_month + mask_bits_year;
        unsigned int copie_pachet_date = 0;
        int i = 0, nr_pachete_date = 0;

        scanf("%d", &N);
        TDate *v_date = malloc(N * sizeof(TDate));
        TDate *v_date2 = malloc(N * sizeof(TDate));
        memoram_int *de_care_inturi_apartine = malloc(N * sizeof(memoram_int));
        while (i < N) {
            size_resturi = 0;
            resturi = 0;
            if (i == 0) {
                scanf("%u", &pachet_date[nr_pachete_date]);
                copie_pachet_date = pachet_date[nr_pachete_date];
                nr_pachete_date++;
            }
            if ((BITS_32 - cati_biti_cititi) < (BITS_32/2)-1) {
                resturi = copie_pachet_date;
                de_care_inturi_apartine[i].y = nr_pachete_date-1;
                scanf("%u", &pachet_date[nr_pachete_date]);
                copie_pachet_date = pachet_date[nr_pachete_date];
                nr_pachete_date++;
                cati_biti_cititi = BITS_32 - cati_biti_cititi;
                size_resturi = cati_biti_cititi;
                cati_biti_cititi = 0;
            }
            o_data_in_biti = mask & copie_pachet_date;
            o_data_in_biti = (o_data_in_biti << size_resturi);
            o_data_in_biti = o_data_in_biti + resturi;
            prelucrare_cate_o_data(o_data_in_biti, i, v_date);
            copie_pachet_date = copie_pachet_date >> (NR_15 - size_resturi);
            cati_biti_cititi = cati_biti_cititi + NR_15 - size_resturi;
            de_care_inturi_apartine[i].x = nr_pachete_date-1;
            if (size_resturi == 0)
                de_care_inturi_apartine[i].y = nr_pachete_date-1;
            v_date2[i].day = v_date[i].day;
            v_date2[i].month = v_date[i].month;
            v_date2[i].year = v_date[i].year;
            i++;
        }
        i = 0;
        int *int_valid = calloc (nr_pachete_date, sizeof(int));
        unsigned int *colInturiControl = malloc(sizeof(unsigned int));
        int nrBitiControl = 0;
        i = 0;
        while (nrBitiControl < nr_pachete_date) {
           unsigned int *aux = NULL;
            aux = realloc(colInturiControl, (i+1) * sizeof(unsigned int));
            if (aux)
                colInturiControl = aux;
            else
                return -1;
            scanf("%u", &colInturiControl[i]);
            nrBitiControl = nrBitiControl + BITS_32;
            i++;
        }
        int cate_valide = 0;
        unsigned int B1 = 0;
        int j = 0;
        for (int i = 0; i < nr_pachete_date; i++) {
            int nr_biti_1 = 0;
            unsigned int mask = 1;
            if (i % BITS_32 == 0 && i != 0)
                j++;
            int k = 0;
            while (k < BITS_32) {
                if ((mask & (pachet_date[i] >> k)) == 1)
                    nr_biti_1++;
                k++;
            }
            B1 = nr_biti_1 % 2;
            if (((colInturiControl[j] >> (i % BITS_32)) & 1) == B1) {
                int_valid[i] = 1;
                cate_valide++;
            }
        }
        int x = 0, y = 0, k = 0;
        for (int i = 0; i < N; i++) {
            x = de_care_inturi_apartine[i].x;
            y = de_care_inturi_apartine[i].y;
            if (int_valid[x] == 1 && int_valid[y] == 1) {
                v_date[k] = v_date2[i];
                k++;
            }
        }
        sortare_crescator(k, v_date);
        for (int i = 0; i < k; i++) {
            char s[SIZE_STR];
            din_nr_in_cuv(s, v_date[i].month, sizeof(s));
            printf("%u %s %u\n", v_date[i].day, s, v_date[i].year);
        }
    }
    return 0;
}
