
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace std::chrono;

#define INFILE "numeros.csv"
#define OUTFILE "ordenados.csv"

// QuickSort function from qsort.c
void par_qsort(int *data, int lo, int hi)
{
    if(lo > hi) return;
    int l = lo;
    int h = hi;
    int p = data[(hi + lo) / 2];

    while(l <= h){
        while((data[l] - p) < 0) l++;
        while((data[h] - p) > 0) h--;
        if(l <= h){
            int tmp = data[l];
            data[l] = data[h];
            data[h] = tmp;
            l++; h--;
        }
    }
    if (hi - lo < 1000) { // Threshold to avoid excessive parallelism for small chunks
        par_qsort(data, lo, h);
        par_qsort(data, l, hi);
    } else {
        #pragma omp task
        par_qsort(data, lo, h);
        #pragma omp task
        par_qsort(data, l, hi);
    }
}

int main() {
    int N;
    cout << "Ingrese el número de elementos aleatorios que desea generar: ";
    cin >> N;

    // Start the timer
    auto start = high_resolution_clock::now();

    // Write random numbers to INFILE
    ofstream escribirNumeros(INFILE, ios::out);
    if(escribirNumeros.bad()) {
        cerr << "Falló la creación del archivo " << INFILE << endl;
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < N - 1; i++){
        escribirNumeros << rand() % (N * 10) << ",";
    }
    escribirNumeros << rand() % (N * 10) << endl;
    escribirNumeros.close();

    // Read numbers from INFILE and store them in an array in the heap
    ifstream leerNumeros(INFILE);
    if(leerNumeros.bad()) {
        cerr << "No se pudo leer el archivo " << INFILE << endl;
        exit(EXIT_FAILURE);
    }

    int *arr = new int[N];
    string ch;
    int idx = 0;
    while(getline(leerNumeros, ch, ',')) {
        arr[idx] = stoi(ch);
        idx++;
    }
    leerNumeros.close();

    // Sort the array using QuickSort
    #pragma omp parallel
    {
        #pragma omp single
        {
            par_qsort(arr, 0, N - 1);
        }
    }

    // Write sorted numbers to OUTFILE
    ofstream escribirOrdenados(OUTFILE, ios::out);
    if(escribirOrdenados.bad()) {
        cerr << "Falló la creación del archivo " << OUTFILE << endl;
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < N - 1; i++){
        escribirOrdenados << arr[i] << ",";
    }
    escribirOrdenados << arr[N - 1] << endl;
    escribirOrdenados.close();

    // Clean up the heap memory
    delete[] arr;

    // Stop the timer and calculate the duration
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "Los números aleatorios se generaron, ordenaron y se escribieron en " << OUTFILE << endl;
    cout << "Tiempo total de ejecución: " << duration.count() << " milisegundos" << endl;

    return 0;
}
