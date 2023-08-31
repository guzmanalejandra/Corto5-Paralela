
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <chrono>

using namespace std;
using namespace std::chrono;

#define INFILE "numerosSecuencial.csv"
#define OUTFILE "ordenadosSecuencial.csv"

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
    par_qsort(data, lo, h);
    par_qsort(data, l, hi);
}

int main() {
    int N;
    cout << "Ingrese el número de elementos aleatorios que desea generar: ";
    cin >> N;

    // Empieza el timer
    auto start = high_resolution_clock::now();

    // Escribe los numeros random al INFILE
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

    // Lee los numeros del INFILE
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

    // Sort del array implementando quicksor
    par_qsort(arr, 0, N - 1);

    // Escribe en el outfile
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

    // Limpieza de la memoria Heap
    delete[] arr;

    // Frena el timer y calcula el tiempo
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "Los números aleatorios se generaron, ordenaron y se escribieron en " << OUTFILE << endl;
    cout << "Tiempo total de ejecución: " << duration.count() << " milisegundos" << endl;

    return 0;
}
