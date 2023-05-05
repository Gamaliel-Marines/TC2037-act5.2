#include <iostream>
#include <iomanip>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include "utils.h"

using namespace std;

const int NUMBERS = 10000; 
const int THREADS = 8;


//función para saber si un número es primo
bool isPrime (int num){
    if (num < 2) {
        return false;
    } 
    
    else {
        for (int i = 2; i <= sqrt(num); i++) {
            if (num % i == 0) {
                return false;
            }
        } 
        return true;
    }
}

//función para sumar los números primos
double addPrime (int size) {
    double acum = 0;
    for (int i = 0; i <= size; i++) {
        if (isPrime(i)) {
            acum = acum + i;
        }
    } return acum;
}

typedef struct {
  int start, end;
  int *arr;
} Block;

pthread_mutex_t mutex_lock_sum;

//función para sumar los números primos de forma paralela
void* addPrimeParallel(void* param){
    double *acum = 0;
    Block* block = (Block*) param;

    acum = new double;

    for(int i = block->start; i < block->end; i++){

        if(isPrime(block->arr[i])){
            *acum = *acum + block->arr[i];
        }
    }

    return ((void**) acum);

}


int main (int argc, char* argv[]) {
    int *a, block_size;
	double ms, result, *acum;
    
    Block blocks[THREADS];
    pthread_t tids[THREADS];


	a = new int[NUMBERS];

    for (int i = 0; i <= NUMBERS; i++) {
        a[i] = i;
    }

    block_size = NUMBERS / THREADS;
    
    for (int i = 0; i < THREADS; i++) {
        blocks[i].arr = a;
        blocks[i].start = i * block_size;

        if (i != (THREADS - 1)) {
            blocks[i].end = (i + 1) * block_size;
        } else {
            blocks[i].end = NUMBERS;
        }
    }

	cout << "Starting...\n";
    ms = 0;
    for (int i = 0; i < N; i++) {
        start_timer();
        result = addPrime(NUMBERS);
        ms += stop_timer();
    }
    cout << "El resultado obtenido de forma secuencial es: " << setprecision(2) << result << "\n";
    cout << "Avg execution time:  " << setprecision(2) << (ms / N) << "\n";

	ms = 0;

    for (int j = 0; j < N; j++) {
        start_timer();
        result = 0;

        for (int i = 0; i < THREADS; i++) {
            pthread_create(&tids[i], NULL, addPrimeParallel, (void*)&blocks[i]);
        }

        for (int i = 0; i < THREADS; i++) {
            pthread_join(tids[i], (void**) &acum);
            result += (*acum);
            delete acum;
        }

        ms += stop_timer();
    }

	cout << "El resultado obtenido de forma paralela es: " << setprecision(2) << result << "\n";
	cout << "Avg execution time:  " << setprecision(2) << (ms / N) << "\n";

	delete [] a;
    return 0;
}