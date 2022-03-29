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
        copie = NULL;
        pos = pos->next;
        copie = pos;
    }
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
        TrainCar* mem = (TrainCar*)malloc(sizeof(TrainCar));
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
        station->platforms[platform]->train_cars = copie->next; // Changed head
        free(copie); // free old head
        copie = station->platforms[platform]->train_cars; // Change Temp
    }
    TrainCar* prev = (TrainCar*)malloc(sizeof(TrainCar));
    prev = station->platforms[platform]->train_cars;
    while (copie != NULL)
    {
        // Search for the key to be deleted, keep track of
        // the previous node as we need to change
        // 'prev->next'
        while (copie != NULL && copie->weight != weight)
        {
            prev = copie;
            copie = copie->next;
        }

        // If key was not present in linked list
        if (copie == NULL)
            return;

        // Unlink the node from linked list
        prev->next = copie->next;

        free(copie); // Free memory

        // Update Temp for next iteration of outer loop
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
    /*if (station == NULL)
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
    int length_a = 1;
    int lenght_b = 1;
    TrainCar* p = (TrainCar*)malloc(sizeof(TrainCar));
    p = station->platforms[platform_a]->train_cars;
    {
        while (p != NULL)
        {
            p = p->next;
            length_a++;
        }
    }
    p = station->platforms[platform_b]->train_cars;
    {
        while (p != NULL)
        {
            p = p->next;
            lenght_b++;
        }
    }
    if (pos_b < 1)
    {
        return;
    }
    if (pos_b > lenght_b + 1)
    {
        return;
    }
    free(p);
    TrainCar* p_a = (TrainCar*)malloc(sizeof(TrainCar));
    p_a = station->platforms[platform_a]->train_cars;
    TrainCar* p_b = (TrainCar*)malloc(sizeof(TrainCar));
    p_b = station->platforms[platform_b]->train_cars;
    TrainCar* p_a2 = (TrainCar*)malloc(sizeof(TrainCar));
    p_a2 = station->platforms[platform_a]->train_cars;
    TrainCar* p_a4 = (TrainCar*)malloc(sizeof(TrainCar));
    p_a4 = station->platforms[platform_a]->train_cars;
    for (int i = 0; i < pos_a - 1+cars_no; i++)
    {
        p_a4 = p_a4->next;
    }
    TrainCar* p_a3 = (TrainCar*)malloc(sizeof(TrainCar));
    p_a3 = station->platforms[platform_a]->train_cars;
    for (int i = 0; i < pos_a - 2; i++)
    {
        p_a3 = p_a3->next;
    }
    for (int i = 0; i < pos_a - 1; i++)
    {
        p_a = p_a->next;
    }
    for (int i = 0; i < pos_b - 2; i++)
    {
        p_b = p_b->next;
    }
    for (int i = 0; i < cars_no - 1; i++)
    {
        p_a = p_a->next;
    }
    p_a3->next = p_a4;
    p_a->next = p_b->next;
    p_b->next = p_a;*/
return;
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
    int max = -1;
    for (int i = 0; i < station->platforms_no; i++)
    {
        if (v[i] < 0)
        {
            max = i;
        }
    }
    return max;
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
    // To store current window sum
    int sum = 0;

    // To store maximum sum
    int maxSum = 0;
    start_car = NULL;
    for (int i = 0; i < station->platforms_no;i++)
    {
        printf("%d", i);
        if (station->platforms == NULL)
        {
            break;
        }
        if (station->platforms[i] == NULL)
        {
            break;
        }
        if (station->platforms[i]->train_cars == NULL)
        {
            break;
        }
        // Pointer to the start of window
        TrainCar* start = station->platforms[i]->train_cars;

        // Pointer to the end of window
        TrainCar* end = start;
        int sum = 0;

        for (int contor = 0; contor < cars_no; contor++)
        {
            
            if (end == NULL)
            {
                break;
            }
            end = end->next;
        }

        // Find the sum of first k nodes
        for ( int k = 0; k < cars_no; k++) {
            if (end == NULL)
            {
                break;
            }
            sum += end->weight;
            end = end->next;
        }

        maxSum = sum;
        // Move window by one step and
        // update sum. Node pointed by
        // start is excluded from current
        // window so subtract it. Node
        // pointed by end is added to
        // current window so add its value.
        while (end != NULL) {

            // Subtract the starting element
            // from previous window
            sum -= start->weight;
            start = start->next;

            // Add the element next to the end
            // of previous window
            sum += end->weight;
            end = end->next;

            // Update the maximum sum so far
            // maxSum = max(maxSum, sum);
            if (sum > maxSum)
            {
                start_car = start;
                maxSum = sum;
            }
        }
    }
    if (maxSum == 0)
    {
        maxSum = -1;
    }
    return maxSum;
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
    //if (station == NULL)
    //{
    //    return;
    //}
    //int pos = find_overload_train(station);
    //if (pos == -1)
    //{
    //    return;
    //}
    //TrainCar* p = (TrainCar*)malloc(sizeof(TrainCar));
    //p = station->platforms[pos]->train_cars;
    //int lenght = 0;
    //int current = station->platforms[pos]->locomotive_power;
    //while (p != NULL)
    //{
    //    current = current - p->weight;
    //    lenght++;
    //    p = p->next;
    //}
    //int* v = (int*)malloc(lenght * sizeof(int));
    //p = station->platforms[pos]->train_cars;
    //for (int i = 0; i < lenght; i++)
    //{
    //    if (station->platforms[i] != NULL)
    //    {
    //        v[i] = current;
    //        v[i] = v[i] + p->weight;
    //        p = p->next;
    //    }
    //}
    //int min = 0;
    //int i = 0;
    //for (i = 0; i < lenght; i++)
    //{
    //    if ((v[i] < v[min]) && (v[i] > 0))
    //    {
    //        min = i;
    //    }
    //}
    //TrainCar* copie = (TrainCar*)malloc(sizeof(TrainCar));
    //copie = station->platforms[pos]->train_cars;
    //while (copie != NULL && min==0)
    //{
    //    station->platforms[pos]->train_cars = copie->next; // Changed head
    //    free(copie); // free old head
    //    copie = station->platforms[pos]->train_cars; // Change Temp
    //}
    //TrainCar* prev = (TrainCar*)malloc(sizeof(TrainCar));
    //prev = station->platforms[pos]->train_cars;
    //    // Search for the key to be deleted, keep track of
    //    // the previous node as we need to change
    //    // 'prev->next'
    //    for (int bfvbfb=0; bfvbfb<min; bfvbfb++)
    //    {
    //        prev = copie;
    //        copie = copie->next;
    //    }

    //    // If key was not present in linked list
    //    if (copie == NULL)
    //        return;

    //    // Unlink the node from linked list
    //    prev->next = copie->next;

    //    free(copie); // Free memory

    //    // Update Temp for next iteration of outer loop
    //    copie = prev->next;
    return;
}
