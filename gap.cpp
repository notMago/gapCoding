#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>
#include <cstdlib>  // Para usar atoi
#include <cmath>
#include <map>
#include <queue>
#include <string>
#include <fstream>
#include <unistd.h> // Para getpid()

using namespace std;
using namespace std::chrono;

//----------------SOLO PARA IMPRIMIR EL HUFFMANCODE----------------------
template<typename T>
void printHuffmanCodes(const map<T, string>& huffmanCodes) {
    cout << "Codigos de Huffman: " << endl;
    for (const auto& pair : huffmanCodes) {
        cout << pair.first << ": " << pair.second << endl;
    }
}

//-----------------SOLO PARA IMPRIMIR EL HUFFMANCODE----------------

size_t getCurrentRSS() {
    // Tomar el PID del proceso actual
    pid_t pid = getpid();
    // Construir la ruta al archivo /proc/<pid>/statm
    string statmPath = "/proc/" + to_string(pid) + "/statm";
    ifstream statmFile(statmPath);

    if (!statmFile.is_open()) {
        cerr << "Error al abrir el archivo " << statmPath << endl;
        return 0;
    }

    size_t rss = 0;
    statmFile >> rss; // Leer el primer valor en el archivo
    statmFile >> rss; // Leer el segundo valor en el archivo, que es RSS (Resident Set Size)
    statmFile.close();

    // Convertir el valor de páginas a bytes
    long pageSize = sysconf(_SC_PAGESIZE);
    return rss * pageSize;
}

// Define las estructuras y funciones de Huffman
struct HuffmanNode {
    int value;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;
    HuffmanNode(int val, int freq) : value(val), frequency(freq), left(nullptr), right(nullptr) {}
};

// Necesitamos calcular las frecuencias de cada valor en el arreglo de gaps.
// Comparador para la cola de prioridad
struct Compare {
    bool operator()(HuffmanNode* l, HuffmanNode* r) {
        return l->frequency > r->frequency;
    }
};

// Funciónes para calcular las frecuencias en int y double
template<typename T>
map<T, int> calculateFrequencies(const T arr[], int size) {
    map<T, int> frequencies;
    for (int i = 0; i < size; ++i) {
        frequencies[arr[i]]++;
    }
    return frequencies;
}

// Función para construir el árbol de Huffman
template<typename T>
HuffmanNode* buildHuffmanTree(const map<T, int>& frequencies) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> minHeap;

    for (const auto& pair : frequencies) {
        minHeap.push(new HuffmanNode(pair.first, pair.second));
    }

    while (minHeap.size() > 1) {
        HuffmanNode* left = minHeap.top();
        minHeap.pop();
        HuffmanNode* right = minHeap.top();
        minHeap.pop();

        HuffmanNode* newNode = new HuffmanNode(-1, left->frequency + right->frequency);
        newNode->left = left;
        newNode->right = right;

        minHeap.push(newNode);
    }

    return minHeap.top();
}

// Una vez construido el árbol, generamos los códigos de Huffman.
// Función para generar los códigos de Huffman
template<typename T>
void generateHuffmanCodes(HuffmanNode* root, string code, map<T, string>& huffmanCodes) {
    if (!root) return;

    if (root->value != -1) {
        huffmanCodes[root->value] = code;
    }

    generateHuffmanCodes(root->left, code + "0", huffmanCodes);
    generateHuffmanCodes(root->right, code + "1", huffmanCodes);
}

// Función para codificar el arreglo utilizando los códigos de Huffman
template<typename T>
string encodeHuffman(const T arr[], int size, const map<T, string>& huffmanCodes) {
    string encodedString;
    for (int i = 0; i < size; ++i) {
        auto it = huffmanCodes.find(arr[i]);
        if (it != huffmanCodes.end()) {
            encodedString += it->second;
        } else {
            cerr << "Error: el valor " << arr[i] << " no tiene un código de Huffman asociado." << endl;
            return "";
        }
    }
    return encodedString;
}

void printArrGAP(int arr[], int size) {
    int cont=0;
    for (int i = 0; i < size; ++i) {
        if (arr[i] != 0){
            cont ++;
            //cout << arr[i] << " ";
        }
    }
    cout<< "Reducido en: " << cont <<  endl;
}
void printArrGAP(double arr[], int size) {
    int cont=0;
    for (int i = 0; i < size; ++i) {
        if (arr[i] != 0){
            cont ++;
            //cout << arr[i] << " ";
        }
    }
    cout<< "Reducido en: " << cont <<  endl;
}


//Funciones para imprimir arreglos
void printArr(int arr[], int size) {
    for (int i = 0; i < size; ++i) 
            cout << arr[i] << " ";
}
void printArr(double arr[], int size) {
    for (int i = 0; i < size; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

//Funcion para generar un arreglo con distribucion lineal
void generateLinearArray(int arr[], int size, int epsilon) {
    arr[0] = rand() % 100; // Primer valor aleatorio
    for (int i = 1; i < size; ++i) {
        arr[i] = arr[i - 1] + rand() % epsilon;
    }
}

//Funcion para generar un arreglo con distribucion normal
void generateNormalArray(double arr[], int size, double mean, double stddev) {
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<> d(mean, stddev);
    for (int i = 0; i < size; ++i) {
        if (d(gen) > 0) {
            double num = round( d(gen) * 100.0) / 100.0; //Para dejarlo con 2 cifras significativas
            arr[i] = num;
        }
    }
    sort(arr, arr + size);
}

// Codificación y decodificación de gaps
template<typename T>
void gapCoding(const T input[], int size, T output[]) {
    if (size == 0) return;
    output[0] = input[0];  // El primer elemento se inserta tal cual
    for (int i = 1; i < size; ++i) {
        output[i] = input[i] - input[i - 1];
    }
}

// Función de búsqueda binaria
template<typename T>
int binarySearch(T arr[], int size, T key) {
    int left = 0, right = size - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == key) return mid;
        if (arr[mid] < key) left = mid + 1;
        else right = mid - 1;
    }
    return -1; // No encontrado
}

// Medir el tiempo de búsqueda
template<typename T>
void measureBinarySearch(T arr[], int size, T key) {
    auto start = high_resolution_clock::now();
    binarySearch(arr, size, key);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(end - start).count();
    cout << "Tiempo de búsqueda: " << duration << " nanosegundos" << endl;
}


// Función para generar la estructura sample
template<typename T>
void generateSample(const T input[], int size, T sample[], int m) {
    int delta = size / m;
    for (int i = 0; i < m; ++i) {
        sample[i] = input[i * delta];
    }
}

// Función para reconstruir un valor del arreglo original usando la estructura sample
template<typename T>
T reconstructValue(const T sample[], const T gaps[], int b, int idx) {
    int segment = idx / b;
    T value = sample[segment];
    for (int i = segment * b; i < idx; ++i) {
        value += gaps[i];
    }
    return value;
}

// Función de búsqueda binaria en un arreglo gap-coded
template<typename T>
int gapCodedBinarySearch(const T sample[], const T gaps[], int size, T key, int b) {
    int left = 0, right = size - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        T midVal = reconstructValue(sample, gaps, b, mid);
        if (midVal == key) return mid;
        if (midVal < key) left = mid + 1;
        else right = mid - 1;
    }
    return -1; // No encontrado
}

// Medir el tiempo de búsqueda en un arreglo gap-coded
template<typename T>
void measureGapCodedBinarySearch(const T sample[], const T gaps[], int size, T key, int b) {
    auto start = high_resolution_clock::now();
    gapCodedBinarySearch(sample, gaps, size, key, b);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(end - start).count();
    cout << "Tiempo de búsqueda: " << duration << " nanosegundos" << endl;
}

// Función para medir el tiempo de construcción del árbol de Huffman
template<typename T>
HuffmanNode* measureHuffmanTreeConstruction(const map<T, int>& frequencies) {
    auto start = high_resolution_clock::now();
    HuffmanNode* huffmanTree = buildHuffmanTree(frequencies);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(end - start).count();
    cout << "Tiempo de construcción del árbol de Huffman: " << duration << " nanosegundos" << endl;
    return huffmanTree;
}

// Función para medir el tiempo de codificación Huffman
template<typename T>
void measureHuffmanEncoding(const T arr[], int size, const map<T, string>& huffmanCodes) {
    auto start = steady_clock::now();
    encodeHuffman(arr, size, huffmanCodes);
    auto end = steady_clock::now();
    auto duration = duration_cast<nanoseconds>(end - start).count();
    cout << "Tiempo de codificacion Huffman: " << duration << " nanosegundos" << endl;
}


int main(int argc, char* argv[]) {
    // Verificar que se ha pasado el argumento del tamaño del arreglo
    if (argc != 3) {
        cerr << "Uso: " << argv[0] << " <largo del arreglo> <diferencia entre nums>" << endl;
        return 1;
    }
    cout << endl << "----------------------------------------------------"<< endl;
    // Obtener el tamaño del arreglo y b desde los argumentos de la línea de comandos
    int size = atoi(argv[1]);
    int b = 3;
    const int epsilon = atoi(argv[2]);

    // Generar un arreglo con distribución lineal
    int* linearArray = new int[size];
    generateLinearArray(linearArray, size, epsilon);

    // Generar un arreglo con distribución normal
    double* normalArray = new double[size];
    generateNormalArray(normalArray, size, 0.0, 1.0);

    /**
    //Imprimir los arreglos generados
    cout << "Arreglo con distribucion lineal:\t";
    printArr(linearArray, size);

    cout << "Arreglo con distribucion normal:\t";
    printArr(normalArray, size);
    
    cout << "----------------------------------------------------" << endl << endl;
     */

    //SIN HUFFMAN
    // Búsqueda binaria en el arreglo lineal
    int keyInt = linearArray[size / 2];
    double keyDouble = normalArray[size / 2];

    cout << "Busqueda binaria en el arreglo lineal - ";
    measureBinarySearch(linearArray, size, keyInt);

    cout << "Busqueda binaria en el arreglo normal - ";
    measureBinarySearch(normalArray, size, keyDouble);

    // Medir uso de memoria después de la busqueda binaria 
    size_t BSmemoryUsage = getCurrentRSS();
    cout << "Uso de memoria despues de busqueda binaria: " << BSmemoryUsage << " bytes" << endl;

    cout << "----------------------------------------------------"<< endl;

    // Codificar los arreglos utilizando gap coding
    int* gapCodedLinearArray = new int[size];
    double* gapCodedNormalArray = new double[size];

    gapCoding(linearArray, size, gapCodedLinearArray);
    gapCoding(normalArray, size, gapCodedNormalArray);

    
    // Imprimir los arreglos codificados con gap coding
    cout << "Arreglo lineal codificado con gap coding. "<<endl;
    cout << "Arreglo normal codificado con gap coding. "<<endl;
    /**printArr(gapCodedLinearArray, size);

    
    printArr(gapCodedNormalArray, size);

    cout << "----------------------------------------------------" << endl;
    */

    // Generar la estructura sample para ambos arreglos
    int m = log2(size);
    //int m = size / b;
    int* sampleLinearArray = new int[m];
    double* sampleNormalArray = new double[m];

    generateSample(linearArray, size, sampleLinearArray, m);
    generateSample(normalArray, size, sampleNormalArray, m);

    // Búsqueda binaria en el arreglo lineal utilizando la estructura sample
    cout << "BS linarArray utilizando gap y sample. ";
    measureGapCodedBinarySearch(sampleLinearArray, gapCodedLinearArray, size, keyInt, b);

    // Búsqueda binaria en el arreglo normal utilizando la estructura sample
    cout << "BS en normalArray utilizando gap y sample. ";
    measureGapCodedBinarySearch(sampleNormalArray, gapCodedNormalArray, size, keyDouble, b);
    
    // Medir uso de memoria después de la busqueda binaria con sample
    size_t BSSampleMemoryUsage = getCurrentRSS();
    cout << "Uso de memoria despues de busqueda binaria con sample:" << BSSampleMemoryUsage << " bytes" << endl;
    cout << "----------------------------------------------------"<< endl;

    // Calcular frecuencias para huffman
    // Calcular y mostrar las frecuencias de ambos arreglos
    auto freqInt = calculateFrequencies(linearArray, size);
    auto freqDouble = calculateFrequencies(normalArray, size);
    
    //Huffmann de aqui palante ######################################################
    // Medir el tiempo de construcción del árbol de Huffman para ambos arreglos
    cout << "Int:\n ";
    HuffmanNode* huffmanTreeInt = measureHuffmanTreeConstruction<int>(freqInt);
    cout << "Double:\n ";
    HuffmanNode* huffmanTreeDouble = measureHuffmanTreeConstruction<double>(freqDouble);

    // Construir los árboles de Huffman para ambos arreglos
    //HuffmanNode* huffmanTreeInt = buildHuffmanTree(freqInt);
    //HuffmanNode* huffmanTreeDouble = buildHuffmanTree(freqDouble);

    
    // Generar los códigos de Huffman para ambos árboles
    map<int, string> huffmanCodesInt;
    generateHuffmanCodes(huffmanTreeInt, "", huffmanCodesInt);
    map<double, string> huffmanCodesDouble;
    generateHuffmanCodes(huffmanTreeDouble, "", huffmanCodesDouble);


   // Codificación de gaps
    int* gapCodedInt = new int[size];
    gapCoding(linearArray, size, gapCodedInt);
    cout << "Arreglo con codificacion de gaps (int):" << endl;
    printArrGAP(gapCodedInt, size);

    double* gapCodedDouble = new double[size];
    gapCoding(normalArray, size, gapCodedDouble);
    cout << "Arreglo con codificacion de gaps (double):" << endl;
    printArrGAP(gapCodedDouble, size);


    cout << "----------------------------------------------------"<< endl;
    // Estructuras sample
    //int m = size / b;
    //int m = rand() % size; // Primer valor aleatorio
    int* sampleInt = new int[m];
    generateSample(linearArray, size, sampleInt, m);

    double* sampleDouble = new double[m];
    generateSample(normalArray, size, sampleDouble, m);

    // Medir tiempos de búsqueda
    measureBinarySearch(linearArray, size, 50);
    measureGapCodedBinarySearch(sampleInt, gapCodedInt, size, 50, b);

    // Medir tiempos de construcción y codificación de Huffman
    //measureHuffmanTreeConstruction(freqInt);
    //measureHuffmanTreeConstruction(freqDouble);


    //-------------SOLO PARA VERIFICAR EL HUFFMANCODE------------------------
    //printHuffmanCodes(huffmanCodesInt);
    //printHuffmanCodes(huffmanCodesDouble);
    //------------SOLO PARA VERIFICAR EL HUFFMANCODE---------------------------

    measureHuffmanEncoding(linearArray, size, huffmanCodesInt);
    measureHuffmanEncoding(normalArray, size, huffmanCodesDouble);

    // Medir uso de memoria después de la codificacion de Huffman
    size_t HuffmanMemoryUsage = getCurrentRSS();
    cout << "Uso de memoria despues de la codificacion de Huffmann: " << HuffmanMemoryUsage << " bytes" << endl;

    // Limpiar memoria
    delete[] linearArray;
    delete[] normalArray;
    delete[] gapCodedInt;
    delete[] gapCodedDouble;
    delete[] sampleInt;
    delete[] sampleDouble;
    
    return 0;
}