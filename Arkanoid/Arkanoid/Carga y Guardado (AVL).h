#pragma once
//Isaac Villalobos Bonilla
#include <iostream>
#include <cstdlib>  // Para rand(), srand()
#include <ctime>    // Para time()
#include <vector> 
using namespace std;
typedef struct abb {
    int codigo;
    string nombre;
    int totalbolas;
    int bolasreb;
    int bolasper;
    int objetosdestruidos;
    abb* izquierdo;
    abb* derecho;
    int altura;
}*abbptr;
int maxi(int a, int b);
int alturaavl(abbptr Raiz);
abbptr nodo(int codigo, int totalbolas, int bolasreb, int bolasper, int objetosdestr, string nombre);
void RotarDerecha(abbptr& raiz);
void RotarIzquierda(abbptr& raiz);
int obtenerbalance(abbptr N);
void insertaravl(abbptr& avl, abbptr nuevo);
void insertar(abbptr& abb, abbptr nuevo);
void printearIRD(abbptr arbol);
void printearRID(abbptr arbol);
void eliminar(abbptr arbol);
void guardarNodo(FILE* archivo, abbptr nodo);
void recorrerYGuardar(FILE* archivo, abbptr nodo);
void guardardatosusuarios(abbptr arbol);
void cargarDatosUsuarios(abbptr& arbol);
void obtenerTop5Codigos(abbptr arbol, std::vector<abbptr>& nodos, int& contador);
vector<abbptr> mostrarTop5(abbptr arbol);