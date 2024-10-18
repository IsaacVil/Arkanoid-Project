//Isaac Villalobos Bonilla
#include <iostream>
#include <cstdlib>  // Para rand(), srand()
#include <ctime>    // Para time()
#include <vector> 
#include "Carga y Guardado (AVL).h"
using namespace std;

int maxi(int a, int b)
{
    if (a > b) //Devuelve el que sea mas grande entre el hijo izquierdo o derecho
        return a;
    else
        return b;
}

int alturaavl(abbptr Raiz)
{
    if (Raiz == NULL) //Si la raiz es nulo la altura es 0
        return 0;
    else
        return Raiz->altura; //Devuelve la altura
}

abbptr nodo(int codigo, int totalbolas, int bolasreb, int bolasper, int objetosdestr, string nombre) //CREA NODO PARA QUE LUEGO LO MANDES A INSERTAR
{
    abbptr nodo = new abb;
    nodo->codigo = codigo;
    nodo->nombre = nombre;
    nodo->totalbolas = totalbolas;
    nodo->bolasper = bolasper;
    nodo->bolasreb = bolasreb;
    nodo->objetosdestruidos = objetosdestr;
    nodo->derecho = nullptr;
    nodo->izquierdo = nullptr;
    nodo->altura = 1; //Todo nodo que entra tiene altura de 1
    return nodo;
}

void RotarDerecha(abbptr& raiz)
{
    if (raiz->izquierdo == nullptr) // Verificar si hay un hijo derecho antes de continuar
        return;

    abbptr copiaizquierda = raiz->izquierdo; //Copiamos al nodo izquierdo
    abbptr hijodeizq = copiaizquierda->derecho; //Copiamos al nodo derecho del nodo izquierdo (porque aqui va a caer la raiz original)

    // hace rotacion
    copiaizquierda->derecho = raiz; //Por esto sacamos la copia, aca queda la raiz
    raiz->izquierdo = hijodeizq; //En la raiz izquierda (la cual no tiene elementos que perder pues aca estaba la copiaizq, se pone la copia antes sacada)

    // actualiza alturas
    raiz->altura = maxi(alturaavl(raiz->izquierdo), alturaavl(raiz->derecho)) + 1; //La altura de cada elemento es la altura mas grande de entre sus dos hijos + 1
    copiaizquierda->altura = maxi(alturaavl(copiaizquierda->izquierdo), alturaavl(copiaizquierda->derecho)) + 1;

    raiz = copiaizquierda; //actualizamos la raiz para que sea al que le hicimos la rotacion
}

void RotarIzquierda(abbptr& raiz)
{
    if (raiz->derecho == nullptr) // Verificar si hay un hijo derecho antes de continuar
        return;

    abbptr copiaderecha = raiz->derecho;
    abbptr hijodeder = copiaderecha->izquierdo;

    // hace rotacion
    copiaderecha->izquierdo = raiz;
    raiz->derecho = hijodeder;

    //actualiza alturas
    raiz->altura = maxi(alturaavl(raiz->izquierdo), alturaavl(raiz->derecho)) + 1;
    copiaderecha->altura = maxi(alturaavl(copiaderecha->izquierdo), alturaavl(copiaderecha->derecho)) + 1;

    // retorna nueva raiz
    raiz = copiaderecha;
}

int obtenerbalance(abbptr N)
{
    if (N == NULL)
        return 0;
    else
        return alturaavl(N->izquierdo) - alturaavl(N->derecho); //Si se dobla a derecha sera < -1 y si se dobla a la izquierda sera > 1
}

void insertaravl(abbptr& avl, abbptr nuevo)
{
    if (avl == nullptr) //Si no hay nada solo se hace un lista = nuevo (Esto sirve para la recursividad vista en la parte de abajo)
    {
        avl = nuevo;
    }
    else //Si no es vacio hay que tomar en cuenta unos casos
    {
        if (nuevo->codigo > avl->codigo)  //Si codigo va a la derecha
        {
            insertaravl(avl->derecho, nuevo); //Se mete de manera recursiva
            avl->altura = maxi(alturaavl(avl->izquierdo), alturaavl(avl->derecho)) + 1; //Le actualizamos su altura
        }
        else if (nuevo->codigo <= avl->codigo) //Si codigo va a la izquierda
        {
            insertaravl(avl->izquierdo, nuevo);
            avl->altura = maxi(alturaavl(avl->derecho), alturaavl(avl->izquierdo)) + 1; //Le actualizamos su altura
        }
        int balance = obtenerbalance(avl); //Vemos si hubo desbalance
        if (balance > 1 || balance < -1) //Cualquiera de estos significa desbalance
        {
            //Caso 1: Rotación a la derecha (desbalanceado a la izquierda)
            if (balance > 1 && nuevo->codigo < avl->izquierdo->codigo) //Doblado a la izquierda
            {
                RotarDerecha(avl);
            }
            //Caso 2: Rotación a la izquierda (desbalanceado a la derecha)
            else if (balance < -1 && nuevo->codigo > avl->derecho->codigo) //Doblado a la derecha
            {
                RotarIzquierda(avl);
            }
            //Caso 3: Rotación izquierda-derecha
            else if (balance > 1 && nuevo->codigo > avl->izquierdo->codigo)  //Doblado izquierda y luego derecha (HAY QUE HACER ROTACION DOBLE)   
            {
                RotarIzquierda(avl->izquierdo);
                RotarDerecha(avl);
            }
            //Caso 4: Rotación derecha-izquierda
            else if (balance < -1 && nuevo->codigo < avl->derecho->codigo) //Doblado derecha y luego izquierda (HAY QUE HACER ROTACION DOBLE)   
            {
                RotarDerecha(avl->derecho);
                RotarIzquierda(avl);
            }
        }
    }
}

//ESTO ES PARA INSERTAR ARBOLES BINARIOS DE BUSQUEDA IGNORAR SI NO SE OCUPA (NO SON AVL)
void insertar(abbptr& abb, abbptr nuevo) {
    if (abb != nullptr) {
        if (abb->codigo > nuevo->codigo && abb->izquierdo != nullptr)
        {
            insertar(abb->izquierdo, nuevo);
        }
        else if (abb->codigo > nuevo->codigo && abb->izquierdo == nullptr) {
            abb->izquierdo = nuevo;
        }
        else if (abb->codigo <= nuevo->codigo && abb->derecho != nullptr)
        {
            insertar(abb->derecho, nuevo);
        }
        else if (abb->codigo <= nuevo->codigo && abb->derecho == nullptr) {
            abb->derecho = nuevo;
        }
    }
    else {
        abb = nuevo;
    }
}

//Imprime en orden
void printearIRD(abbptr arbol) {
    if (arbol != nullptr) {
        printearIRD(arbol->izquierdo);
        cout << "_______________" << endl;
        cout << arbol->nombre << endl;
        cout << arbol->codigo << endl;
        cout << arbol->totalbolas << endl;
        cout << arbol->bolasreb << endl;
        cout << arbol->bolasper << endl;
        cout << arbol->objetosdestruidos << endl;
        printearIRD(arbol->derecho);
    }
}

//Imprime en preorden
void printearRID(abbptr arbol) {
    if (arbol != nullptr) {
        cout << arbol->codigo << endl;
        printearRID(arbol->izquierdo);
        printearRID(arbol->derecho);
    }
}

//Para borrar todo el arbol
void eliminar(abbptr arbol) {
    if (arbol != nullptr) //Cuando se meta de manera recursiva para buscar si es nulo no va a hacer nada
    {
        eliminar(arbol->izquierdo);
        eliminar(arbol->derecho);
        delete arbol;
    }
}

void guardarNodo(FILE* archivo, abbptr nodo) {
    if (nodo) {
        fprintf(archivo, "%d ,%s ,%d ,%d ,%d ,%d\n", nodo->codigo, nodo->nombre.c_str(), nodo->totalbolas, nodo->bolasreb, nodo->bolasper, nodo->objetosdestruidos);
    }
}

void recorrerYGuardar(FILE* archivo, abbptr nodo) {
    if (nodo) {
        recorrerYGuardar(archivo, nodo->izquierdo);  // Recorrer subárbol izquierdo
        guardarNodo(archivo, nodo);                  // Guardar el nodo actual
        recorrerYGuardar(archivo, nodo->derecho);    // Recorrer subárbol derecho
    }
}

void guardardatosusuarios(abbptr arbol) {
    const char* nombreArchivo = "top5\\puntuaciones.txt";
    FILE* archivo = nullptr;

    if (fopen_s(&archivo, nombreArchivo, "w+") != 0) {
        cout << "Error al abrir el archivo." << endl;
        return;
    }

    recorrerYGuardar(archivo, arbol);

    fclose(archivo);
}


void cargarDatosUsuarios(abbptr& arbol) {
    const char* nombreArchivo = "top5\\puntuaciones.txt";
    FILE* archivo = nullptr;

    if (fopen_s(&archivo, nombreArchivo, "r+") != 0) {
        arbol = nullptr;
        return;
    }
    else {
        int codigo, totalbolas, bolasreb, bolasper, objetosdestruidos;
        char nombre[10];

        while (fscanf_s(archivo, "%d ,%9[^,] ,%d ,%d ,%d ,%d\n", &codigo, nombre, (unsigned)_countof(nombre), &totalbolas, &bolasreb, &bolasper, &objetosdestruidos) != EOF) {
            string nombreStr(nombre);
            abbptr nuevo = nodo(codigo, totalbolas, bolasreb, bolasper, objetosdestruidos, nombreStr);
            insertaravl(arbol, nuevo);
        }

        fclose(archivo);
    }
}

void completarConNullptr(vector<abbptr>& nodos) {
    while (nodos.size() < 5) {
        nodos.push_back(nullptr); // Añadir nullptr hasta que haya 5 elementos
    }
}

void obtenerTop5Codigos(abbptr arbol, std::vector<abbptr>& nodos, int& contador) {
    if (arbol == nullptr || contador >= 5) {
        return; // Salir si el árbol está vacío o ya tenemos 5 nodos
    }

    // Primero visitamos el subárbol derecho
    obtenerTop5Codigos(arbol->derecho, nodos, contador);

    // Procesamos el nodo actual
    if (contador < 5) { // Solo añadimos si aún no hemos encontrado 5 nodos
        nodos.push_back(arbol); // Añadir el nodo a la lista
        contador++; // Incrementamos el contador
    }

    // Por último, visitamos el subárbol izquierdo
    obtenerTop5Codigos(arbol->izquierdo, nodos, contador);
}

// Función que retorna una lista con los 5 nodos más grandes
vector<abbptr> mostrarTop5(abbptr arbol) {
    vector<abbptr> nodos; // Vector para almacenar los nodos
    int contador = 0; // Contador para llevar el registro de cuántos nodos hemos encontrado
    obtenerTop5Codigos(arbol, nodos, contador);
    completarConNullptr(nodos);
    return nodos; // Retorna la lista de nodos
}
