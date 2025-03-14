/*
* File: p1.c
* Author: Guilherme Leitão ist199951
* Description: This program simulates an airport management system with several
* functionalities, such as registering and listing new airports and flights and
* managing dates. Besides that, it is also capable of organising that
* information in ascending order according to given departure or arrival dates
* and hours. The program considers the calendar and its days appropriately.
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define IDLEN 4              /*maximum number of characters for an airport id*/
#define CTRYLEN 31           /*maximum number of characters for a country*/
#define CITYLEN 51           /*maximum number of characters for a city*/
#define FLIGHTCODE 7         /*maximum number of characters for a flight code*/

#define HOUR 60              /*number of minutes in an hour*/
#define DAY 24               /*number of hours in a day*/
#define YEAR 12              /*number of months in a year*/

#define MAXAIRPORT 40        /*maximum number of airports that there can be*/
#define MAXFLIGHTS 30000     /*maximum number of flights that there can be*/

/*if minutes exceeds, increase hour*/
#define carryhour(A, B) if (A >= HOUR) {A -= HOUR; B++;}
/*if hours exceeds, increase day*/
#define carryday(A, B) if (A >= DAY) {A -= DAY; B++;}
/*if days exceeds, increase month*/
#define carrymonth1(A, B) if (A == 29 && B == 2) {A = 1; B = 3;}
#define carrymonth2(A, B) if((A == 31 && (B == 4 || B == 6 || B == 9 || B ==\
                            11)) || (A == 32 && (B == 1 || B == 3 || B == 5 ||\
                            B == 7 || B == 8 || B == 10 || B == YEAR)))\
                            {A = 1; B++;}
/*if months exceeds, increase year*/
#define carryyear(A, B) if (A > YEAR) {A = 1; B++;}

/*check if all its three characters are upper case*/
#define notallupper(A) if (!isupper(A[0]) || !isupper(A[1]) || !isupper(A[2]))\
                           {printf("invalid airport ID\n"); continue;}
/*check if the limit of possible airports as been exceeded*/
#define reachedlimitaps(A, B) if (A == B)\
                              {printf("too many airports\n"); continue;}
/*check if the limit of possible flights as been exceeded*/
#define reachedlimitfls(A, B) if (A == B)\
                                 {printf("too many flights\n"); continue;}
/*check if it is a valid flight code*/
#define isvalidflight(A) if(!isupper(A[0]) || !isupper(A[1]) || !isdigit(A[2])\
                         || A[2] == '0' || (strlen(A) >= 4 && !isdigit(A[3]))\
                         || (strlen(A) >= 5 && !isdigit(A[4])) || (strlen(A)\
                         == 6 && !isdigit(A[5])) || strlen(A) > 6)\
                         {printf("invalid flight code\n"); continue;}
/*check if the date is within the valid period*/
#define timeperiodlimit(A, B, C, D, E, F) if(A < D || (A == D && B < E) ||\
                                          (A == D && B == E && C < F) || A > D\
                                          + 1 || (A == D + 1 && B > E) ||\
                                          (A == D + 1 && B == E && C > F))\
                                          {printf("invalid date\n"); continue;}
/*check if the flight duration is valid*/
#define checkduration(A, B) if((A == YEAR && B > 0) || A > YEAR)\
                            {printf("invalid duration\n"); continue;}
/*check if the flight capacity is valid*/
#define checkcapacity(A) if(A < 10 || A > 100)\
                           {printf("invalid capacity\n"); continue;}

#define PRINTALL 0              /*mode to print all in handleap function*/
#define PRINTDEP 1              /*mode to print dep in handleap function*/
#define PRINTARR 2              /*mode to print arr in handleap function*/

typedef struct {

    char id[IDLEN];             /*airport identifier*/
    char ctry[CTRYLEN];         /*country of origin*/
    char city[CITYLEN];         /*city of origin*/

} Airport;

typedef struct {

    char code[FLIGHTCODE];      /*flight code*/
    char depture[IDLEN];        /*departure airport*/
    char arrival[IDLEN];        /*arrival airport*/

    int day, month, year;       /*day, month and year of departure*/
    int hour, min;              /*hour and minute of departure*/
    int aday, amonth, ayear;    /*day, month and year of arrival*/
    int ahour, amin;            /*hour and minute of arrival*/
    
    int pass;                   /*maximum number of passengers*/

} Flight;

/*
* uses insertion sort to organize in ascending alphabetical order the
* the vector of airports a[]
*/
void insertionalpha(Airport a[], int n) {

    int i, j;

    for (i = 1; i <= n; i++) {

        Airport v = a[i];

        j = i-1;

        /*multiply by 26 to differentiate the letters that come first*/
        while (j >= 0 && (v.id[0] - 'A')*26*26 + (v.id[1] - 'A')*26 + (v.id[2]\
               - 'A') < (a[j].id[0] - 'A')*26*26 + (a[j].id[1] - 'A')*26 +\
               (a[j].id[2] - 'A')) {

            a[j+1] = a[j];
            j--;

        }

        a[j+1] = v;

    }

}

/*
* uses insertion sort to organize in ascending order the the vector
* of flights f[] according to the time and date of departure
*/
void insertiontimedep(Flight f[], int n) {

    int i, j;

    for (i = 1; i <= n; i++) {

        Flight v = f[i];

        j = i-1;

        /*compares dates and times of flights until it finds an earlier one*/
        while (j >= 0 && (v.year < f[j].year || (v.year == f[j].year && v.month\
        < f[j].month) || (v.year == f[j].year && v.month == f[j].month && v.day\
        < f[j].day) || (v.year == f[j].year && v.month == f[j].month && v.day\
        == f[j].day && v.hour < f[j].hour) || (v.year == f[j].year && v.month\
        == f[j].month && v.day == f[j].day && v.hour == f[j].hour &&\
        v.min < f[j].min))) {

            f[j+1] = f[j];
            j--;

        }

        f[j+1] = v;

    }

}

/*
* uses insertion sort to organize in ascending order the the vector
* of flights f[] according to the time and date of arrival
*/
void insertiontimearr(Flight f[], int n) {

    int i, j;

    for (i = 1; i <= n; i++) {

        Flight v = f[i];

        j = i-1;

        /*compares dates and times of flights until it finds an earlier one*/
        while (j >= 0 && (v.ayear < f[j].ayear || (v.ayear == f[j].ayear &&\
        v.amonth < f[j].amonth) || (v.ayear == f[j].ayear && v.amonth ==\
        f[j].amonth && v.aday < f[j].aday) || (v.ayear == f[j].ayear &&\
        v.amonth == f[j].amonth && v.aday == f[j].aday && v.ahour < f[j].ahour)\
        || (v.ayear == f[j].ayear && v.amonth == f[j].amonth && v.aday == \
        f[j].aday && v.ahour == f[j].ahour && v.amin < f[j].amin))) {

            f[j+1] = f[j];
            j--;

        }

        f[j+1] = v;

    }

}

/*
* returns 1 if the specified airport id 'id1''id2''id3' is in the
* vector of airports aps[]; 0 otherwise
*/
int apexists(Airport aps[], int n, char id1, char id2, char id3){

    int i;

    for(i = 0; i < n; i++)
        /*if the airport id matches, return 1*/
        if(aps[i].id[0] == id1 && aps[i].id[1] == id2 && aps[i].id[2] == id3)
            return 1;

    return 0;

}

/*
* returns 1 if the specified flight id fid[] and date of departure
* is in the vector of flights fls[]; 0 otherwise
*/
int flexists(Flight fls[], int n, char fid[], int day, int month, int year){

    int i;

    for(i = 0; i < n; i++)
        /*if the flight code matches, return 1*/
        if(strcmp(fls[i].code, fid) == 0 && fls[i].day == day &&\
           fls[i].month == month && fls[i].year == year)
            return 1;

    return 0;

}

/*
* prints all airports in vector aps[] after sorting them using insertionalpha()
*/
void printallaps(int n, Airport aps[], Flight fls[], int f){

    int i, j;

    /*sorts the vector of airports alphabetically*/
    insertionalpha(aps, n - 1);

    for(i = 0; i < n; i++){

        int pass = 0;

        printf("%s ", aps[i].id);
        /*remove the new line at end of string*/
        printf("%s ", strtok(aps[i].city, "\n"));
        printf("%s ", aps[i].ctry);

        /*counts the number of flights that departure from the airport*/
        for(j = 0; j < f; j++)
            if(strcmp(fls[j].depture, aps[i].id) == 0)
                pass++;

        printf("%d\n", pass);

    }

}

/*
* prints all flights in the vector fls[]
*/
void printallflights(int n, Flight fls[]){

    int i;

    for(i = 0; i < n; i++){

        printf("%s ", fls[i].code);
        printf("%s ", fls[i].depture);
        printf("%s ", fls[i].arrival);
        printf("%02d-%02d-%d ", fls[i].day, fls[i].month, fls[i].year);
        printf("%02d:%02d\n", fls[i].hour, fls[i].min);

    }

}

/*
* prints specific plane id 'id1''id2''id3' in the vector of planes aps[]
*/
void printap(int n, Airport aps[], char id1, char id2, char id3, Flight fls[],\
             int f){

    int i, j, pass = 0;

    for(i = 0; i < n; i++)

        if(aps[i].id[0] == id1 && aps[i].id[1] == id2 && aps[i].id[2] == id3){

            printf("%c%c%c ", aps[i].id[0], aps[i].id[1], aps[i].id[2]);
            /*remove the new line at end of string*/
            printf("%s ", strtok(aps[i].city, "\n"));
            printf("%s ", aps[i].ctry);

            /*counts the number of flights that departure from the airport*/
            for(j = 0; j < f; j++)
                if(strcmp(fls[j].depture, aps[i].id) == 0)
                    pass++;

            printf("%d\n", pass);

            return;

        }

}

/*
* prints all flights that have as the departure airport id id[] after
* sorting them using insertiontimedep()
*/
void printflightdep(int n, Flight fls[], char c1, char c2, char c3){

    int i;

    /*sorts the vector of airports by order of departure*/
    insertiontimedep(fls, n - 1);

    for(i = 0; i < n; i++)
        if(fls[i].depture[0] == c1 && fls[i].depture[1] == c2 &&\
        fls[i].depture[2] == c3)
        printf("%s %s %02d-%02d-%d %02d:%02d\n", fls[i].code, fls[i].arrival,\
               fls[i].day, fls[i].month, fls[i].year, fls[i].hour, fls[i].min);

}

/*
* prints all flights that have as the arrival airport id id[] after
* sorting them using insertiontimearr()
*/
void printflightarr(int n, Flight fls[], char c1, char c2, char c3){

    int i;

    /*sorts the vector of airports by order of arrival*/
    insertiontimearr(fls, n - 1);

    for(i = 0; i < n; i++)
        if(fls[i].arrival[0] == c1 && fls[i].arrival[1] == c2 &&\
           fls[i].arrival[2] == c3)
            printf("%s %s %02d-%02d-%d %02d:%02d\n", fls[i].code,\
            fls[i].depture, fls[i].aday, fls[i].amonth, fls[i].ayear,\
            fls[i].ahour, fls[i].amin);

}

/*
* checks if the airport exists and, if it does, runs the respective operation
*/
void handleap(Airport aps[], int naps, char c1, char c2, char c3, int mode,\
              Flight flights[], int nfls){


    if(!apexists(aps, naps, c1, c2, c3)){

        printf("%c%c%c: no such airport ID\n", c1, c2, c3);
        return;

    }


    if(mode == PRINTALL){                 /*if we want to print all airports*/

        printap(naps, aps, c1, c2, c3, flights, nfls); 

    } else if(mode == PRINTARR) {  /*if we want to print airports by arrival*/

        printflightarr(nfls, flights, c1, c2, c3);

    } else if(mode == PRINTDEP) {/*if we want to print airports by departure*/

        printflightdep(nfls, flights, c1, c2, c3);

    }

}

int main() {

    char c;/*character that will read the option input*/
    int curday = 1, curmonth = 1, curyear = 2022;/*default is 1 January 2022*/
    int naps = 0 /*number of airports*/, nfls = 0 /*number of flights*/;

    Airport aps[MAXAIRPORT];/*vector holding all airports*/
    Flight flights[MAXFLIGHTS];/*vector holding all flights*/
    Flight sec[MAXFLIGHTS];/*secondary vector holding all flights*/

    while((c = getchar()) != 'q'){

        if(c == 'a'){

            char apid[IDLEN], apctry[CTRYLEN], apcity[CITYLEN];

            scanf("%s %s", apid, apctry);
            getchar();  /*skip a space*/
            fgets(apcity, CITYLEN, stdin);

            notallupper(apid);
            reachedlimitaps(naps, MAXAIRPORT);
            if(apexists(aps, naps, apid[0], apid[1], apid[2])){

                printf("duplicate airport\n");
                continue;

            }

            printf("airport %s\n", apid);

            /*copy the information to the structure*/
            strcpy(aps[naps].id, apid);
            strcpy(aps[naps].ctry, apctry);
            strcpy(aps[naps++].city, apcity);

        } else if(c == 'l'){

            char ch;

            if((ch = getchar()) == '\n'){/*if no arguments specified print all*/

                printallaps(naps, aps, flights, nfls);

            } else {                     /*else, print specified ones*/

                char c1, c2, c3;

                do{

                    c1 = getchar();
                    c2 = getchar();
                    c3 = getchar();

                    handleap(aps, naps, c1, c2, c3, PRINTALL, flights, nfls);

                } while(getchar() != '\n');

            }

        } else if(c == 'v'){

            char ch;

            if((ch = getchar()) == '\n'){/*if no arguments specified print all*/

                printallflights(nfls, sec);

            } else {                     /*else, register the specified one*/

                char fid[FLIGHTCODE], dapid[IDLEN], aapid[IDLEN];
                int dday, dmonth, dyear, dhour, dmin;
                int ddurhour, ddurmin;
                int cap;
                int year, month, day, hour, min;

                scanf("%s %s %s %d-%d-%d %d:%d %d:%d %d", fid, dapid, aapid,\
                &dday, &dmonth, &dyear, &dhour, &dmin, &ddurhour, &ddurmin,\
                &cap);

                isvalidflight(fid);
                if(flexists(flights, nfls, fid, dday, dmonth, dyear)){

                    printf("flight already exists\n");
                    continue;

                } else if(!apexists(aps, naps, dapid[0], dapid[1], dapid[2])){

                    printf("%s: no such airport ID\n", dapid);
                    continue;

                } else if(!apexists(aps, naps, aapid[0], aapid[1], aapid[2])){

                    printf("%s: no such airport ID\n", aapid);
                    continue;

                }
                reachedlimitfls(nfls, MAXFLIGHTS);
                timeperiodlimit(dyear, dmonth, dday, curyear, curmonth, curday);
                checkduration(ddurhour, ddurmin);
                checkcapacity(cap);

                year = dyear;
                month = dmonth;
                day = dday;
                hour = dhour + ddurhour;
                min = dmin + ddurmin;

                /*carry the hours up*/
                carryhour(min, hour);
                /*carry the days up*/
                carryday(hour, day);
                /*carry the months up*/
                carrymonth1(day, month);
                carrymonth2(day, month);
                /*carry the year up*/
                carryyear(month, year);

                /*copy the information to the main structure*/
                strcpy(flights[nfls].code, fid);
                strcpy(flights[nfls].depture, dapid);
                strcpy(flights[nfls].arrival, aapid);
                flights[nfls].day = dday;
                flights[nfls].month = dmonth;
                flights[nfls].year = dyear;
                flights[nfls].hour = dhour;
                flights[nfls].min = dmin;
                flights[nfls].pass = cap;
                flights[nfls].aday = day;
                flights[nfls].amonth = month;
                flights[nfls].ayear = year;
                flights[nfls].ahour = hour;
                flights[nfls].amin = min;

                /*copy the information to the secondary structure*/
                strcpy(sec[nfls].code, fid);
                strcpy(sec[nfls].depture, dapid);
                strcpy(sec[nfls].arrival, aapid);
                sec[nfls].day = dday;
                sec[nfls].month = dmonth;
                sec[nfls].year = dyear;
                sec[nfls].hour = dhour;
                sec[nfls].min = dmin;
                sec[nfls].pass = cap;
                sec[nfls].aday = day;
                sec[nfls].amonth = month;
                sec[nfls].ayear = year;
                sec[nfls].ahour = hour;
                sec[nfls++].amin = min;

            }

        } else if(c == 'p'){

            char id[IDLEN];

            scanf("%s", id);

            handleap(aps, naps, id[0], id[1], id[2], PRINTDEP, flights, nfls);

        } else if(c == 'c'){

            char id[IDLEN];

            scanf("%s", id);

            handleap(aps, naps, id[0], id[1], id[2], PRINTARR, flights, nfls);

        } else if(c == 't'){

            int day, month, year;

            scanf("%d-%d-%d", &day, &month, &year);

            /*if it is in the past or more than a year ahead*/
            timeperiodlimit(year, month, day, curyear, curmonth, curday);

            curday = day;
            curmonth = month;
            curyear = year;

            printf("%02d-%02d-%d\n", curday, curmonth, curyear);

        }

    }

    return 0;

}