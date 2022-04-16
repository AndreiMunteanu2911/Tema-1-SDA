#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "station.h"

/* Creeaza o gara cu un numar fix de peroane.
 *
 * platforms_no: numarul de peroane ale garii
 *
 * return: gara creata
 */
TrainStation* open_train_station(int platforms_no) {
    TrainStation* statie = (TrainStation*)malloc(sizeof(TrainStation));
    if (statie == NULL)
    {
        printf("Eroare alocare memorie");
        return;
    }
    statie->platforms = (Train**)malloc(sizeof(Train*) * platforms_no);
    for (int i = 0; i < platforms_no; i++)
    {
        statie->platforms[i] = NULL;
    }
    statie->platforms_no = platforms_no;
    return statie;
}


/* Elibereaza memoria alocata pentru gara.
 *
 * station: gara existenta
 */
void close_train_station(TrainStation* station) {
    for (int i = 0; i < station->platforms_no; i++)
    {
        free(station->platforms[i]);
    }
    free(station);
}


/* Afiseaza trenurile stationate in gara.
 *
 * station: gara existenta
 * f: fisierul in care se face afisarea
 */
void show_existing_trains(TrainStation* station, FILE* f) {
    if (f == NULL)
    {
        return;
    }
    if (station == NULL)
    {
        return;
    }
    for (int i = 0; i < station->platforms_no; i++)
    {
        fprintf(f, "%d: ", i);
        if (station->platforms[i] != NULL)
        {
            fprintf(f, "(%d)", station->platforms[i]->locomotive_power);
            TrainCar* copie = station->platforms[i]->train_cars;
            while (copie != NULL)
            {
                fprintf(f, "-|%d|", copie->weight);
                copie = copie->next;
            }
        }
        fprintf(f, "\n");
    }
}


/* Adauga o locomotiva pe un peron.
 *
 * station: gara existenta
 * platform: peronul pe care se adauga locomotiva
 * locomotive_power: puterea de tractiune a locomotivei
 */
void arrive_train(TrainStation* station, int platform, int locomotive_power) {
    if (station == NULL)
    {
        return;
    }
    if (station->platforms[platform] != NULL)
    {
        return;
    }
    if (platform > station->platforms_no)
    {
        return;
    }
    Train* locomotiva = (Train*)malloc(sizeof(Train));
    locomotiva->locomotive_power = locomotive_power;
    locomotiva->train_cars = (Train*)malloc(sizeof(Train));
    locomotiva->train_cars->next = NULL;
    locomotiva->train_cars->weight = NULL;
    locomotiva->train_cars = NULL;
    station->platforms[platform] = locomotiva;
}


/* Elibereaza un peron.
 *
 * station: gara existenta
 * platform: peronul de pe care pleaca trenul
 */
void leave_train(TrainStation* station, int platform) {
    if (station == NULL)
    {
        return;
    }
    if (station->platforms == NULL)
    {
        return;
    }
    if (platform >= station->platforms_no)
    {
        return;
    }
    if (platform < 0)
    {
        return;
    }
    if (station->platforms[platform] == NULL)
    {
        return;
    }
    TrainCar* pos = (TrainCar*)malloc(sizeof(TrainCar));
    TrainCar* copie = (TrainCar*)malloc(sizeof(TrainCar));
    pos = station->platforms[platform]->train_cars;
    station->platforms[platform]->locomotive_power = NULL;
    copie = pos;
    while (pos != NULL)
    {
        pos = pos->next;
        free(copie);
        copie = pos;
    }
    free(copie);
    free(pos);
    station->platforms[platform] = NULL;
}


/* Adauga un vagon la capatul unui tren.
 *
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 * weight: greutatea vagonului adaugat
 */
void add_train_car(TrainStation* station, int platform, int weight) {
    if (station == NULL)
    {
        return;
    }
    if (station->platforms == NULL)
    {
        return;
    }
    if (platform >= station->platforms_no)
    {
        return;
    }
    if (platform < 0)
    {
        return;
    }
    if (station->platforms[platform] == NULL)
    {
        return;
    }
    TrainCar* vagon = (TrainCar*)malloc(sizeof(TrainCar));
    vagon->weight = weight;
    vagon->next = NULL;
    if (station->platforms[platform]->train_cars == NULL)
    {
        station->platforms[platform]->train_cars = vagon;
    }
    else
    {
        TrainCar* copie = (TrainCar*)malloc(sizeof(TrainCar));
        copie = station->platforms[platform]->train_cars;
        while (copie->next != NULL)
        {
            copie = copie->next;
        }
        copie->next = vagon;
    }
}


/* Scoate vagoanele de o anumita greutate dintr-un tren.
 *
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 * weight: greutatea vagonului scos
 */
void remove_train_cars(TrainStation* station, int platform, int weight) {
    if (station == NULL)
    {
        return;
    }
    if (station->platforms == NULL)
    {
        return;
    }
    if (platform >= station->platforms_no)
    {
        return;
    }
    if (platform < 0)
    {
        return;
    }
    if (station->platforms[platform] == NULL)
    {
        return;
    }
    if (station->platforms[platform]->train_cars == NULL)
    {
        return;
    }
    TrainCar* copie = (TrainCar*)malloc(sizeof(TrainCar));
    copie = station->platforms[platform]->train_cars;
    while (copie != NULL && copie->weight == weight)
    {
        station->platforms[platform]->train_cars = copie->next;
        free(copie);
        copie = station->platforms[platform]->train_cars;
    }
    TrainCar* prev = (TrainCar*)malloc(sizeof(TrainCar));
    prev = station->platforms[platform]->train_cars;
    while (copie != NULL)
    {
        while (copie != NULL && copie->weight != weight)
        {
            prev = copie;
            copie = copie->next;
        }
        if (copie == NULL)
            return;
        prev->next = copie->next;
        free(copie);
        copie = prev->next;
    }
}


/* Muta o secventa de vagoane dintr-un tren in altul.
 *
 * station: gara existenta
 * platform_a: peronul pe care se afla trenul de unde se scot vagoanele
 * pos_a: pozitia primului vagon din secventa
 * cars_no: lungimea secventei
 * platform_b: peronul pe care se afla trenul unde se adauga vagoanele
 * pos_b: pozitia unde se adauga secventa de vagoane
 */
void move_train_cars(TrainStation* station, int platform_a, int pos_a,
    int cars_no, int platform_b, int pos_b) {
    if (station == NULL)
    {
        return;
    }
    if (station->platforms[platform_a] == NULL)
    {
        return;
    }
    if (station->platforms[platform_b] == NULL)
    {
        return;
    }
    if (platform_a >= station->platforms_no)
    {
        return;
    }
    if (platform_a < 0)
    {
        return;
    }
    if (platform_b >= station->platforms_no)
    {
        return;
    }
    if (platform_b < 0)
    {
        return;
    }
    if (station->platforms[platform_a] == NULL)
    {
        return;
    }
    if (station->platforms[platform_a]->train_cars == NULL)
    {
        return;
    }
    if (station->platforms[platform_b] == NULL)
    {
        return;
    }
    if (station->platforms[platform_b]->train_cars == NULL)
    {
        return;
    }
    int pos1 = pos_a;
    TrainCar* copie1 = station->platforms[platform_a]->train_cars;
    TrainCar* prev1 = copie1;
    TrainCar* prim = copie1;
    pos1 = pos1 - 1;
    while ((pos1!=0) && (copie1 != NULL))
    {
        if (pos1 == 1)
        {
            prev1 = copie1;
        }
        pos1=pos1-1;
        copie1 = copie1->next;
    }
    if (pos1 != 0)
    {
        return;
    }
    int pos2 = pos_b - 1;
    TrainCar* copie2 = station->platforms[platform_b]->train_cars;
    TrainCar* prev2 = copie2;
    while ((pos2) && (copie2 != NULL))
    {
        if (pos2 == 1)
        {
            prev2 = copie2;
        }
        pos2=pos2-1;
        copie2 = copie2->next;
    }
    if (pos2 != 0)
    {
        return;
    }
    if( (pos_a == 1) && (pos_b == 1))
    {
        copie1 = station->platforms[platform_a]->train_cars;
        copie2 = station->platforms[platform_b]->train_cars;
        station->platforms[platform_b]->train_cars = station->platforms[platform_a]->train_cars;
        while (cars_no && copie1 != NULL)
        {
            if ((cars_no == 1 )|| (copie1->next == NULL))
            {
                prim = copie1;
            }
            cars_no=cars_no-1;
            copie1 = copie1->next;
        }
        if (cars_no != 0)
        {
            station->platforms[platform_b]->train_cars = copie2;
            return;
        }
        station->platforms[platform_a]->train_cars = copie1;
        if (station->platforms[platform_b]->train_cars != NULL)
        {
            prim->next = copie2;
        }
        return;
    }
    if (pos_a == 1)
    {
        copie1 = station->platforms[platform_a]->train_cars;
        if (prev2 != NULL)
        {
            prev2->next = station->platforms[platform_a]->train_cars;
        }
        while ((cars_no )&&( copie1 != NULL))
        {
            if (cars_no == 1)
            {
                prim = copie1;
            }
            cars_no=cars_no-1;
            copie1 = copie1->next;
        }
        station->platforms[platform_a]->train_cars = copie1;
        prim->next = copie2;
        return;
    }
    if (pos_b == 1)
    {
        station->platforms[platform_b]->train_cars = copie1;
        while ((cars_no )&& (copie1 != NULL))
        {
            if (cars_no == 1)
            {
                prim = copie1;
            }
            cars_no=cars_no-1;
            copie1 = copie1->next;
        }
        prev1->next = copie1;
        if (station->platforms[platform_b]->train_cars != NULL)
        {
            prim->next = copie2;
        }
        return;
    }
    prev2->next = copie1;
    while ((cars_no) &&( copie1 != NULL))
    {
        if (cars_no == 1)
        {
            prim = copie1;
        }
        cars_no=cars_no-1;
        copie1 = copie1->next;
    }
    if (cars_no != 0)
    {
        prev2->next = copie2;
        return;
    }
    prev1->next = copie1;
    prim->next = copie2;
}


/* Gaseste trenul cel mai rapid.
 *
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_express_train(TrainStation* station) {
    if (station == NULL)
    {
        return;
    }
    int *v = (int*)malloc(((station->platforms_no) + 1) * sizeof(int));
    for (int i = 0; i < station->platforms_no; i++)
    {
        if (station->platforms[i] != NULL)
        {
            v[i] = station->platforms[i]->locomotive_power;
            TrainCar* copie = station->platforms[i]->train_cars;
            while (copie != NULL)
            {
                v[i] = v[i] -copie->weight;
                copie = copie->next;
            }
        }
    }
    int max = 0;
    for (int i = 0; i < station->platforms_no; i++)
    {
        if (v[i] > v[max])
        {
            max = i;
        }
    }
    return max;
}


/* Gaseste trenul supraincarcat.
 *
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_overload_train(TrainStation* station) {
    if (station == NULL)
    {
        return;
    }
    int suma = 0;
    int max = 0;
    int nr = -1;
    for (int i = 0; i < station->platforms_no; i++)
    {
        if (station->platforms[i] == NULL)
        {
            return;
        }
        if (station->platforms[i]->locomotive_power == NULL)
        {
            return;
        }
        suma = 0;
        TrainCar* copie = (TrainCar*)malloc(sizeof(TrainCar));
        copie = station->platforms[i]->train_cars;
        while (copie != NULL)
        {
            suma =suma + copie->weight;
            copie = copie->next;
        }
        if (station->platforms[i]->locomotive_power < suma)
        {
            nr = i;
            return i;
        }
    }
    return nr;
}


/* Gaseste trenul cu incarcatura optima.
 *
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_optimal_train(TrainStation* station) {
    if (station == NULL)
    {
        return;
    }
    int* v = (int*)malloc(((station->platforms_no) + 1) * sizeof(int));
    for (int i = 0; i < station->platforms_no; i++)
    {
        if (station->platforms[i] != NULL)
        {
            v[i] = station->platforms[i]->locomotive_power;
            TrainCar* copie = station->platforms[i]->train_cars;
            while (copie != NULL)
            {
                v[i] = v[i] - copie->weight;
                copie = copie->next;
            }
        }
    }
    int max = 0;
    for (int i = 0; i < station->platforms_no; i++)
    {
        if ((v[i] < v[max])&&(v[i]>0))
        {
            max = i;
        }
    }
    return max;
}


/* Gaseste trenul cu incarcatura nedistribuita bine.
 *
 * station: gara existenta
 * cars_no: lungimea secventei de vagoane
 *
 * return: peronul pe care se afla trenul
 */
int find_heaviest_sequence_train(TrainStation* station, int cars_no, TrainCar** start_car) {
    if (station == NULL)
    {
        return;
    }
    if (station->platforms == NULL)
    {
        return;
    }
    if (station->platforms_no == 0)
    {
        return;
    }
    if (station == NULL)
        return;
    if (station->platforms == NULL)
        return;
    int suma = 0;
    int max = 0;
    int nr = -1;
    for (int i = 0; i < station->platforms_no; i++)
    {
        if (station->platforms[i] == NULL)
        {
            continue;
        }
        if (station->platforms[i]->train_cars == NULL)
        {
            continue;
        }
        suma = 0;
        int car = cars_no;
        TrainCar* copie = station->platforms[i]->train_cars;
        TrainCar* copie2 = station->platforms[i]->train_cars;
        if (cars_no == 1)
        {
            while (copie != NULL) {
                if (max < copie->weight) {
                    max = copie->weight;
                    *start_car = copie;
                    nr = i;
                }
                copie = copie->next;
            }
        }
        while (car && copie2 != NULL && cars_no != 1)
        {
            suma =suma+ copie2->weight;
            car=car-1;
            copie2 = copie2->next;
        }
        if (car == 0 && cars_no != 1) {
            if (suma > max) {
                max = suma;
                *start_car = copie;
                nr = i;
            }
            while (copie2 != NULL) {
                suma =suma+ copie2->weight;
                suma =suma- copie->weight;
                if (suma > max) {
                    max = suma;
                    *start_car = copie->next;
                    nr = i;
                }
                copie = copie->next;
                copie2 = copie2->next;
            }
        }
    }
    if (nr == -1)
    {
        *start_car = NULL;
    }
    return nr;
}


/* Ordoneaza vagoanele dintr-un tren in ordinea descrescatoare a greutatilor.
 *
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 */
void order_train(TrainStation* station, int platform) {
    if (station == NULL)
    {
        return;
    }
    if (station->platforms == NULL)
    {
        return;
    }
    if (platform >= station->platforms_no)
    {
        return;
    }
    if (platform < 0)
    {
        return;
    }
    if (station->platforms[platform] == NULL)
    {
        return;
    }
    if (station->platforms[platform]->train_cars == NULL)
    {
        return;
    }
    TrainCar* t= (TrainCar*)malloc(sizeof(TrainCar));
    TrainCar* s = (TrainCar*)malloc(sizeof(TrainCar));
    int x;
    t = station->platforms[platform]->train_cars;
    while (t != NULL)
    {
        s = t->next;
        while (s != NULL)
        {
            if (t->weight < s->weight)
            {
                x = t->weight;
                t->weight = s->weight;
                s->weight = x;
            }
            s = s->next;
        }
        t = t->next;
    }
}


/* Scoate un vagon din trenul supraincarcat.
 *
 * station: gara existenta
 */
void fix_overload_train(TrainStation* station) {
    return;
}
