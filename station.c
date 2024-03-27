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
    TrainStation* train_station = (TrainStation*) malloc(sizeof(TrainStation));
    if(platforms_no < 0){
        return NULL;
    }
    train_station -> platforms_no = platforms_no;
    train_station -> platforms = malloc(platforms_no*sizeof(Train*));
    for(int i = 0; i < train_station -> platforms_no; i++){
        train_station -> platforms[i] = malloc(sizeof(Train));
        train_station -> platforms[i] -> locomotive_power = -1;
        train_station -> platforms[i] -> train_cars = NULL;
    }
    return train_station;
}


/* Elibereaza memoria alocata pentru gara.
 *
 * station: gara existenta
 */
void close_train_station(TrainStation *station) {
    if(station == NULL){
        return;
    }
    if(station -> platforms_no < 1){
        free(station);
        station = NULL;
        return;
    }
    if(station -> platforms == NULL){
        free(station);
        station = NULL;
        return;
    }
    for(int i = 0; i < (station -> platforms_no); i++){
        if(station -> platforms[i] == NULL){
            free(station -> platforms);
            free(station);
            return;
        }
        if(station -> platforms[i] -> locomotive_power == -1){
            free(station -> platforms[i]);
        }
        else{
            TrainCar* tmp;
            while(station -> platforms[i] -> train_cars != NULL){
                tmp = station -> platforms[i] -> train_cars;
                station -> platforms[i] -> train_cars = station -> platforms[i] -> train_cars -> next;
                free(tmp);
                tmp = NULL;
            }
            free(station -> platforms[i]);
            station -> platforms[i] = NULL;
        }
    }
    free(station -> platforms);
    station -> platforms = NULL;
    free(station);
    station = NULL;
}


/* Afiseaza trenurile stationate in gara.
 *
 * station: gara existenta
 * f: fisierul in care se face afisarea
 */
void show_existing_trains(TrainStation *station, FILE *f) {
    if(station == NULL){
        return;
    }
    if(station -> platforms_no < 1){
        return;
    }
    if(station -> platforms == NULL){
        return;
    }
    for(int i = 0; i < (station -> platforms_no); i++){
        fprintf(f, "%d: ", i);
        if((station -> platforms[i] -> locomotive_power) != -1){
            fprintf(f, "(%d)", (station -> platforms[i] -> locomotive_power));
            TrainCar* train = station -> platforms [i] -> train_cars;
            while(train != NULL){
                fprintf(f, "-|%d|", train -> weight);
                train = train -> next;
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
void arrive_train(TrainStation *station, int platform, int locomotive_power) {
    if(platform < 0)
        return;
    if(station == NULL)
        return;
    if(platform >= station -> platforms_no)
        return;
    if(station -> platforms[platform] -> locomotive_power == -1)
        station -> platforms[platform] -> locomotive_power = locomotive_power;
}


/* Elibereaza un peron.
 * 
 * station: gara existenta
 * platform: peronul de pe care pleaca trenul
 */
void leave_train(TrainStation *station, int platform) {
    if(platform < 0)
        return;
    if(station == NULL)
        return;
    if(platform >= station -> platforms_no)
        return;
    TrainCar* car;
    while(station -> platforms[platform] -> train_cars != NULL){
        car = station -> platforms[platform] -> train_cars;
        station -> platforms[platform] -> train_cars = station -> platforms[platform] -> train_cars -> next;
        free(car);
    }
    station -> platforms[platform] -> locomotive_power = -1;
}


/* Adauga un vagon la capatul unui tren.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 * weight: greutatea vagonului adaugat
 */
void add_train_car(TrainStation *station, int platform, int weight) {
    
    if(platform < 0)
        return;
    if(station == NULL)
        return;
    if(platform >= station -> platforms_no)
        return;
    if(station -> platforms[platform] -> locomotive_power == -1)
        return;
    if(weight < 0)
        return;
    TrainCar* car = (TrainCar*) malloc(sizeof(TrainCar));
    if(station -> platforms[platform] -> train_cars == NULL){
        station -> platforms[platform] -> train_cars = car;
        station -> platforms[platform] -> train_cars -> weight = weight;
        station -> platforms[platform] -> train_cars -> next = NULL;
        return;
    }
    TrainCar* new_car;
    new_car = station -> platforms[platform] -> train_cars;
    while(new_car -> next != NULL){
        new_car = new_car -> next;
    }
    new_car -> next = car;
    new_car -> next -> weight = weight;
    new_car -> next -> next = NULL;
    
}


/* Scoate vagoanele de o anumita greutate dintr-un tren.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 * weight: greutatea vagonului scos
 */
void remove_train_cars(TrainStation *station, int platform, int weight) {
    if(platform < 0)
        return;
    if(station == NULL)
        return;
    if(platform >= station -> platforms_no)
        return;
    if(station -> platforms[platform] -> locomotive_power == -1)
        return;
    if(station -> platforms[platform] -> train_cars == NULL)
        return;
    if(weight < 0)
        return;
    TrainCar* train_car;
    TrainCar* train;
    TrainCar* train_prev;
    train = station -> platforms[platform] -> train_cars;
    train_prev = train;
    while(station -> platforms[platform] -> train_cars -> weight == weight){
        if(station -> platforms[platform] -> train_cars -> next == NULL){
            train = station -> platforms[platform] -> train_cars;
            station -> platforms[platform] -> train_cars = NULL;
            free(train);
            return;
        }
        else{
            station -> platforms[platform] -> train_cars = station -> platforms[platform] -> train_cars -> next;
            free(train);
            train = station -> platforms[platform] -> train_cars;
        }
    }
    while(train -> next != NULL){
        if(train -> weight == weight){
            train_car = train;
            train_prev -> next = train -> next;
            free(train_car);
            train = train_prev -> next;
            train_car = NULL;
        }
        else{
            train_prev = train;
            train = train -> next;
        }
    }
    if(train -> weight == weight){
        free(train);
        train = NULL;
        train_prev -> next = NULL;
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
void move_train_cars(TrainStation *station, int platform_a, int pos_a, 
                                int cars_no, int platform_b, int pos_b) {
    if(cars_no < 1)
        return;
    if((platform_a < 0) || (platform_b < 0))
        return;
    if(station == NULL)
        return;
    if(pos_a < 1 || pos_b < 1)
        return;
    if((platform_a >= station -> platforms_no) || (platform_b >= station -> platforms_no))
        return;
    if((station -> platforms[platform_a] -> locomotive_power == -1) || (station -> platforms[platform_b] -> locomotive_power == -1))
        return;
    if(station -> platforms[platform_a] -> train_cars == NULL)
        return;
    TrainCar* pointer_a;
    pointer_a = station -> platforms[platform_a] -> train_cars;
    TrainCar* previous_a;
    TrainCar* copy_a;
    copy_a = station -> platforms[platform_a] -> train_cars;
    int i;
    for(i = 1; i < pos_a; i++){
        previous_a = pointer_a;
        pointer_a = pointer_a -> next;
        copy_a = copy_a -> next;
        if(pointer_a == NULL)
            return;
    }
    for(i = 1; i < cars_no; i++){
        copy_a = copy_a -> next;
        if(copy_a == NULL)
            return;
    }
    TrainCar* pointer_b;
    pointer_b = station -> platforms[platform_b] -> train_cars;
    TrainCar* previous_b;
    if((pos_b) > 1 && (station -> platforms[platform_b] -> train_cars == NULL))
        return;
    for(i = 1; i < pos_b; i++){
        previous_b = pointer_b;
        pointer_b = pointer_b -> next;
        if((pointer_b == NULL) && (i < (pos_b - 1)))
            return;
    }
    if((pos_a == 1) && (pos_b == 1)){
        station -> platforms[platform_a] -> train_cars = copy_a -> next;
        station -> platforms[platform_b] -> train_cars = pointer_a;
        copy_a -> next = pointer_b;
    }
    if((pos_a == 1) && (pos_b != 1)){
        station -> platforms[platform_a] -> train_cars = copy_a -> next;
        previous_b -> next = pointer_a;
        copy_a -> next = pointer_b;
    }
    if((pos_a != 1) && (pos_b == 1)){
        previous_a -> next = copy_a -> next;
        station -> platforms[platform_b] -> train_cars = pointer_a;
        copy_a -> next = pointer_b;
    }
    if((pos_a != 1) && (pos_b != 1)){
        previous_a -> next = copy_a -> next;
        previous_b -> next = pointer_a;
        copy_a -> next = pointer_b;
    }
}



/* Gaseste trenul cel mai rapid.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_express_train(TrainStation *station) {
    
    if(station == NULL)
        return -1;
    if(station -> platforms_no < 1)
        return -1; 
    int platform = -1;
    int total = 0;
    int maxim = -1;

    for(int i = 0; i < (station -> platforms_no); i++){
        if(station -> platforms[i] -> locomotive_power > -1){
            total = station -> platforms[i] -> locomotive_power;
            if(station -> platforms[i] -> train_cars != NULL){
                TrainCar* car = station -> platforms[i] -> train_cars;
                while(car -> next != NULL){
                    total = total - (car -> weight);
                    car = car -> next;
                }
                total = total - (car -> weight);
            }
            if(total > maxim){
                maxim = total;
                platform = i;
            }
        }
    }
    return platform;

}


/* Gaseste trenul supraincarcat.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_overload_train(TrainStation *station) {
    
    
    if(station == NULL)
        return -1;
        if(station -> platforms_no < 1)
            return -1;
        int platform = -1;
    int total = 0;
    for(int i = 0; i < (station -> platforms_no); i++){
        if(station -> platforms[i] != NULL){
            if(station -> platforms[i] -> locomotive_power == -1)
                continue;
            total = station -> platforms[i] -> locomotive_power;
            TrainCar* car = station -> platforms[i] -> train_cars;
            while(car != NULL){
                total = total - (car -> weight);
                car = car -> next;
            }
            if(total < 0){
                platform = i;
                break;
            }
        }
    }
    
    return platform; //platform
}


/* Gaseste trenul cu incarcatura optima.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_optimal_train(TrainStation *station) {
    if(station == NULL)
        return -1;
    if(station -> platforms_no < 1)
        return -1; 
    int platform = -1;
    int total = -1;
    int minim = -1;
    int nr = -1;
    for(int i = 0; i < (station -> platforms_no); i++){
        if(station -> platforms[i] -> locomotive_power > -1){
            if(i == 0){
                minim = station -> platforms[i] -> locomotive_power + 1;
            }
            total = station -> platforms[i] -> locomotive_power;
            if(station -> platforms[i] -> train_cars != NULL){
                TrainCar* car = station -> platforms[i] -> train_cars;
                while(car -> next != NULL){
                    total = total - (car -> weight);
                    car = car -> next;
                }
                total = total - (car -> weight);
            }
            if(total < minim){
                minim = total;
                platform = i;
            }
        }
    }
    return platform;
}


/* Gaseste trenul cu incarcatura nedistribuita bine.
 * 
 * station: gara existenta
 * cars_no: lungimea secventei de vagoane
 *
 * return: peronul pe care se afla trenul
 */
int find_heaviest_sequence_train(TrainStation *station, int cars_no, TrainCar **start_car) {
    
    int i, j, total;
    if(station == NULL)
        return -1;
    if(station -> platforms_no < 1)
        return -1;
    if(cars_no == 0)
        return -1;
    int check = 1;
    int pos_max = -1;
    int max_cars = -1;
    TrainCar* start;
    TrainCar* pointer;
    for(i = 0; i < station -> platforms_no; i++){
        if((station -> platforms[i] -> locomotive_power == -1) || (station -> platforms[i] -> train_cars == NULL))
            continue;
        start = station -> platforms[i] -> train_cars;
        pointer = station -> platforms[i] -> train_cars;
loop:
        check = 1;
        total = 0;
        pointer = start;
        for(j = 1; j < cars_no; j++){
            if(pointer -> next != NULL){
                total = total + (pointer -> weight);
                pointer = pointer -> next;
            }
            else{
                check = 0;
            }
        }
        if(check == 1){
            total = total + (pointer -> weight);
            if(total > max_cars){
                pos_max = i;
                max_cars = total;
                *start_car = start;
            }
        }
        if(pointer -> next != NULL){
            start = start -> next;
            goto loop;
        }
    }
    if(pos_max == -1)
        *start_car = NULL;
    return pos_max;
    
}


/* Ordoneaza vagoanele dintr-un tren in ordinea descrescatoare a greutatilor.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 */
void order_train(TrainStation *station, int platform) {
    if(platform < 0)
        return;
    if(station == NULL)
        return;
    if(platform >= station -> platforms_no)
        return;
    if(station -> platforms[platform] -> locomotive_power == -1)
        return;
    if(station -> platforms[platform] -> train_cars == NULL)
        return;
    if(station -> platforms[platform] -> train_cars -> next == NULL)
        return;
    TrainCar* head_car;
    head_car = station -> platforms[platform] -> train_cars;
    TrainCar* prev_car;
    TrainCar* current_car;
    current_car = station -> platforms[platform] -> train_cars;
    TrainCar* temp_car;
    int count = 1;
    int i;
    while(current_car -> next != NULL) {
	    count++;
	    current_car = current_car -> next;
    }
    if(count == 2){
        if((head_car -> weight) < (head_car -> next -> weight)){
            temp_car = head_car -> next;
            head_car -> next -> next = head_car;
            head_car -> next = NULL;
            head_car = temp_car;
            station -> platforms[platform] -> train_cars = head_car;
        }
        return;
    }
    for(i = 1; i < count; i++){
        prev_car = head_car;
        current_car = head_car;
        while(current_car -> next != NULL){
            if((current_car -> weight) < (current_car -> next -> weight)){
                temp_car = current_car -> next;
                current_car -> next = current_car -> next -> next;
                temp_car -> next = current_car;
                if(head_car == current_car){
                    prev_car = temp_car;
                    head_car = temp_car;
                }
                else{
                    prev_car -> next = temp_car;
                    current_car = temp_car;
                }
            }
            prev_car = current_car;
            current_car = current_car -> next;
        }
    }
    station -> platforms[platform] -> train_cars = head_car;
}


/* Scoate un vagon din trenul supraincarcat.
 * 
 * station: gara existenta
 */
void fix_overload_train(TrainStation *station) {
    if(station == NULL)
        return;
    if(station -> platforms_no < 1)
        return;
    int platform = -1;
    int total = 0;
    int i;
    for(i = 0; i < (station -> platforms_no); i++){
        if(station -> platforms[i] != NULL){
            if(station -> platforms[i] -> locomotive_power == -1)
                continue;
            if(station -> platforms[i] -> train_cars == NULL)
                continue;
            total = station -> platforms[i] -> locomotive_power;
            TrainCar* car = station -> platforms[i] -> train_cars;
            while(car -> next != NULL){
                total = total - (car -> weight);
                car = car -> next;
            }
            total = total - (car -> weight);
            if(total < 0){
                platform = i;
                break;
            }
        }
    }
    if(platform == -1)
        return;
    int min, pos = 1, final_pos, minimum = 1000000;
    min = (-1) * total;
    TrainCar* car_at_platform;
    TrainCar* delete;
    car_at_platform = station -> platforms[platform] -> train_cars;
    if(car_at_platform -> weight == min){
        final_pos = 1;
    }
    else{
        if((car_at_platform -> weight < minimum) && (car_at_platform -> weight > min)){
            final_pos = pos;
            minimum = car_at_platform -> next -> weight;
        }
        while(car_at_platform -> next != NULL){
            pos++;
            if(car_at_platform -> next -> weight == min){
                final_pos = pos;
                break;
            }
            else{
                if((car_at_platform -> next -> weight < minimum) && (car_at_platform -> next -> weight > min)){
                    final_pos = pos;
                    minimum = car_at_platform -> next -> weight;
                }
            }
            car_at_platform = car_at_platform -> next;
        }
    }
    if(final_pos == 1){
        delete = station -> platforms[platform] -> train_cars;
        if(station -> platforms[platform] -> train_cars -> next == NULL)
            station -> platforms[platform] -> train_cars = NULL;
        else
            station -> platforms[platform] -> train_cars = station -> platforms[platform] -> train_cars -> next;
        free(delete);
    }
    else{
        car_at_platform = station -> platforms[platform] -> train_cars;
        for(i = 1; i < (final_pos - 1); i++){
            car_at_platform = car_at_platform -> next;
        }
        delete = car_at_platform -> next;
        if(car_at_platform -> next -> next == NULL)
            car_at_platform -> next = NULL;
        else
            car_at_platform -> next = car_at_platform -> next -> next;
        free(delete);
    }
}
