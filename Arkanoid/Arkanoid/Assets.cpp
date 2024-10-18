#include <allegro5/allegro_primitives.h>
#include "Arkanoid.h"
#include <string>
#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "Nivel1.h"
#include "Assets.h"
#include "resource.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
const int ancho = GetSystemMetrics(SM_CXSCREEN);
const int altura = GetSystemMetrics(SM_CYSCREEN);
float escaladoX1 = devolverx();
float escaladoY1 = devolvery();
const int anchurainicio = 360 * escaladoX1;
const int anchurafinal = 1300 * escaladoX1;
using namespace std;
float plataforma_x = ((anchurainicio + anchurafinal) / 2.0) * escaladoX1;
float plataforma_y = (1000 - 50) * escaladoY1; // Posiciona la plataforma en la parte inferior de la ventana
float plataforma_anchura = 180 * escaladoX1; // Ancho de la plataforma
float plataforma_altura = 20 * escaladoY1;   // Alto de la plataforma
float velocidad = 60 * escaladoX1;          // Velocidad de movimiento
float tiempoMensaje = 0.0f;
float tiempo_animacion2 = 0.15; // Tiempo en segundos entre frames (ajustado)
float tiempo_transcurrido2 = 0;
float compuertax = 0;
float compuertay = 0;
bool capsulacaida = false;
bool juegoPerdido = false;
bool bolaLanzada = false;
bool mostrarmensaje = false;
bool iniciadoprimeravez = false;
bool animacionterminada = false;
bool random = false;
int contador_frame = 0;
int velocidad_frame = 100;
int frame2 = 0;
int vidas = 3;
int frame_actual2 = 0;
int frames_totales2 = 8;
int vidajugador = 5;
int vidajugador2 = 5;
int elementonivel = 0;
double tiempoInicioCapsula = 0;
double tiempo_anterior2 = 0; // Tiempo del último frame
int tipocapsula = 1;

ALLEGRO_COLOR obtenerColorNeon() {
    int seleccion = rand() % 8;
    switch (seleccion) {
    case 0:
        if (nivel2) {
            return al_map_rgb(0, 255, 255);
        }
        return al_map_rgb(57, 255, 20);

    case 1:
        if (nivel1) {
            return al_map_rgb(57, 255, 20); 
        }
        return al_map_rgb(0, 255, 255);

    case 2:
        if (nivel3) {
            return al_map_rgb(57, 255, 20); 
        }
        return al_map_rgb(255, 0, 255);

    case 3:
        return al_map_rgb(255, 0, 102); 

    case 4:
        if (nivel2) {
            return al_map_rgb(255, 0, 102); 
        }
        return al_map_rgb(102, 255, 102); 

    case 5:
        return al_map_rgb(255, 153, 51); 

    case 6:
        if (nivel1) {
            return al_map_rgb(255, 153, 51); 
        }
        return al_map_rgb(0, 128, 255); 

    case 7:
        if (nivel3) {
            return al_map_rgb(57, 255, 20); 
        }
        return al_map_rgb(204, 0, 255); 

    default:
        return al_map_rgb(204, 0, 255); 
    }
}


ALLEGRO_COLOR colorarcoiris(float tiempo) {
    float valor = fmod(tiempo, 1.0f);
    float r = fabs(sin(valor * 2.0f * 3.14159f));
    float g = fabs(sin((valor + 0.333f) * 2.0f * 3.14159f));
    float b = fabs(sin((valor + 0.666f) * 2.0f * 3.14159f));
    return al_map_rgb_f(r, g, b);
}

ALLEGRO_COLOR oscurecerColor(ALLEGRO_COLOR color, float factor) {
    float r, g, b, a;
    al_unmap_rgb_f(color, &r, &g, &b); // Descomponer el color en sus componentes
    // Reducir cada componente del color según el factor
    r *= factor;
    g *= factor;
    b *= factor;
    return al_map_rgb_f(r, g, b); // Volver a componer el color
}

ALLEGRO_COLOR colorplataforma = al_map_rgb(0, 255, 0);
ALLEGRO_COLOR colorplataforma2 = al_map_rgb(155, 255, 155);

typedef struct Bola {
    float x;
    float y;
    float velocidadX;
    float velocidadY;
    float radio;
    Bola* siguiente; // Puntero al siguiente nodo
}* ptrbola;

typedef struct Bloque {
    int plataformax;
    int plataformay;
    int plataformaanchura;
    int plataformaaltura;
    int tipodebloque;
    int golpe;
    ALLEGRO_COLOR color;
    Bloque* siguiente;
}* ptrbloques;

typedef struct Capsula {
    int tipo;
    int x;
    int y;
    Capsula* siguiente;
}* ptrcapsulas;

typedef struct Poderes {
    int tipo;
    int tiempo;
    int usado;
    Poderes* siguiente;
}* ptrpoderes;

typedef struct Enemigos {
    int puntos;
    float x;
    float y;
    float velocidadX;
    float velocidadY;
    float radio;
    Enemigos* siguiente;
}* ptrenemigos;

ptrbola listabola = nullptr;
ptrbloques listabloques = nullptr;
ptrbloques listabloquesvivos = nullptr;
ptrcapsulas listacapsula = nullptr;
ptrpoderes listapoderes = nullptr;
ptrenemigos listaenemigos = nullptr;

ptrbola crearBola(float x, float y, float velocidadX, float velocidadY, float radio) {
    ptrbola bola = new(Bola);
    bola->x = x;
    bola->y = y;
    bola->velocidadX = velocidadX;
    bola->velocidadY = velocidadY;
    bola->radio = radio;
    bola->siguiente = nullptr;
    return bola;
}

ptrbloques crearBloque(int x, int y, int ancho, int alto, int tipo, int cantidadgolpe, ALLEGRO_COLOR color) {
    ptrbloques bloque = new(Bloque);
    bloque->plataformax = x;
    bloque->plataformay = y;
    bloque->plataformaanchura = ancho;
    bloque->plataformaaltura = alto;
    bloque->tipodebloque = tipo;
    bloque->color = color;
    bloque->golpe = cantidadgolpe;
    bloque->siguiente = nullptr;
    return bloque;
}

ptrcapsulas crearCapsula(int x, int y, int tipo) {
    ptrcapsulas nuevo = new(Capsula);
    nuevo->x = x;
    nuevo->y = y;
    nuevo->tipo = tipo;
    nuevo->siguiente = nullptr;
    return nuevo;
}

ptrpoderes crearPoder(int tipo, int tiempo) {
    ptrpoderes nuevo = new(Poderes);
    nuevo->tipo = tipo;
    nuevo->tiempo = tiempo;
    nuevo->usado = 0;
    return nuevo;
}

ptrenemigos crearEnemigos(int puntos, float x, float y, float velocidadX, float velocidadY, float radio) {
    ptrenemigos nuevo = new Enemigos;
    nuevo->puntos = puntos;
    nuevo->x = x;
    nuevo->y = y;
    nuevo->velocidadX = velocidadX;
    nuevo->velocidadY = velocidadY;
    nuevo->radio = radio;
    return nuevo;
}

void agregarBola(ptrbola& bola) {
    bola->siguiente = listabola;
    listabola = bola;
}

void agregarBloques(ptrbloques& bloque) {
    bloque->siguiente = listabloques;
    listabloques = bloque;
}

void agregarBloquesvivos(ptrbloques& bloque) {
    bloque->siguiente = listabloquesvivos;
    listabloquesvivos = bloque;
}
void agregarCapsulas(ptrcapsulas& capsula) {
    capsula->siguiente = listacapsula;
    listacapsula = capsula;
}

void agregarPoderes(ptrpoderes& poder) {
    poder->siguiente = listapoderes;
    listapoderes = poder;
}

void agregarEnemigos(ptrenemigos& enemigo) {
    enemigo->siguiente = listaenemigos;
    listaenemigos = enemigo;
}

void dibujarBloques() {
    ptrbloques actual = listabloques;
    while (actual != nullptr) {
        if (actual->golpe > 0) {
            ALLEGRO_COLOR colorOscuro = oscurecerColor(actual->color, 1.5f);
            al_draw_filled_rectangle(actual->plataformax, actual->plataformay, actual->plataformaanchura, actual->plataformaaltura, actual->color);
            al_draw_filled_rectangle(actual->plataformax + 3 * escaladoX1, actual->plataformay + 40 * escaladoY1, actual->plataformaanchura - 3 * escaladoX1, actual->plataformaaltura - 3 * escaladoY1, oscurecerColor(actual->color, 0.25f));
            al_draw_filled_rectangle(actual->plataformax + 85 * escaladoX1, actual->plataformay + 3 * escaladoY1, actual->plataformaanchura - 3 * escaladoX1, actual->plataformaaltura - 3 * escaladoY1, oscurecerColor(actual->color, 0.25f));
        }
        actual = actual->siguiente;
    }
}

void dibujarBloquesvivos() {
    ptrbloques actual = listabloquesvivos;
    while (actual != nullptr) {
        if (actual->golpe > 0) {
            ALLEGRO_COLOR colorOscuro = oscurecerColor(actual->color, 1.5f);
            al_draw_filled_rectangle(actual->plataformax, actual->plataformay, actual->plataformaanchura, actual->plataformaaltura, actual->color);
            al_draw_filled_rectangle(actual->plataformax + 3 * escaladoX1, actual->plataformay + 40 * escaladoY1, actual->plataformaanchura - 3 * escaladoX1, actual->plataformaaltura - 3 * escaladoY1, oscurecerColor(actual->color, 0.25f));
            al_draw_filled_rectangle(actual->plataformax + 85 * escaladoX1, actual->plataformay + 3 * escaladoY1, actual->plataformaanchura - 3 * escaladoX1, actual->plataformaaltura - 3 * escaladoY1, oscurecerColor(actual->color, 0.25f));
        }
        actual = actual->siguiente;
    }
}

void dibujarBolas() {
    ptrbola actual = listabola;
    while (actual != nullptr) {
        if (nivel3) {
            ALLEGRO_COLOR red40 = colorarcoiris(tiempo);
            al_draw_filled_circle(actual->x, actual->y, actual->radio + 3, al_map_rgb(255, 255, 255));
            al_draw_filled_circle(actual->x, actual->y, actual->radio, red40);
            actual = actual->siguiente;
        }
        else {
            al_draw_filled_circle(actual->x, actual->y, actual->radio + 3, al_map_rgb(0, 0, 0));
            al_draw_filled_circle(actual->x, actual->y, actual->radio, al_map_rgb(255, 0, 0));
            actual = actual->siguiente;
        }
    }
}

void cambiarbackground(ALLEGRO_BITMAP*& background1) {
    ALLEGRO_COLOR color;

    if (frames >= 20 && frames <= 27) {
        color = al_map_rgba_f(125 / 255.0, 75 / 255.0, 125 / 255.0, 1.0f);
    }
    else if (frames >= 28 && frames <= 35) {
        color = al_map_rgba_f(100 / 255.0, 50 / 255.0, 100 / 255.0, 1.0f);
    }
    else if (frames >= 36 && frames <= 40) {
        color = al_map_rgba_f(55 / 255.0, 30 / 255.0, 55 / 255.0, 1.0f);
    }
    else if (frames >= 41 && frames <= 51) {
        color = al_map_rgba_f(100 / 255.0, 75 / 255.0, 100 / 255.0, 1.0f);
    }
    else if (frames >= 52 && frames <= 60) {
        color = al_map_rgba_f(120 / 255.0, 95 / 255.0, 120 / 255.0, 1.0f);
    }
    else {
        color = al_map_rgba_f(120 / 255.0, 95 / 255.0, 120 / 255.0, 1.0f);
    }

    float width = al_get_bitmap_width(background1);
    float height = al_get_bitmap_height(background1);

    al_draw_tinted_scaled_bitmap(background1,color,0, 0,width, height,anchurainicio, 50 * escaladoY1,anchurafinal - anchurainicio, (1000 * escaladoY1) - (50 * escaladoY1),0);
}

void dibujarEnemigos() {
    ptrenemigos actual = listaenemigos;
    while (actual != nullptr) {
        al_draw_filled_circle(actual->x, actual->y, actual->radio, al_map_rgb(255, 0, 0));
        actual = actual->siguiente;
    }
}

void capsulasdibujadas(ptrcapsulas capsu) {
    int x = capsu->x;
    int y = capsu->y;
    ALLEGRO_BITMAP* capsula;
    if (capsu->tipo == 1) {
        capsula = al_load_bitmap("sprites/capsulas/1.png");
    }
    else if (capsu->tipo == 2) {
        capsula = al_load_bitmap("sprites/capsulas/2.png");
    }
    else if (capsu->tipo == 3) {
        capsula = al_load_bitmap("sprites/capsulas/3.png");
    }
    else if (capsu->tipo == 4) {
        capsula = al_load_bitmap("sprites/capsulas/4.png");
    }
    else if (capsu->tipo == 5) {
        capsula = al_load_bitmap("sprites/capsulas/5.png");
    }
    else if (capsu->tipo == 6) {
        capsula = al_load_bitmap("sprites/capsulas/6.png");
    }
    else if (capsu->tipo == 7) {
        capsula = al_load_bitmap("sprites/capsulas/7.png");
    }
    else {
        capsula = al_load_bitmap("sprites/capsulas/1.png");
    }
    int ancho_frame2 = al_get_bitmap_width(capsula) / frames_totales2;
    int alto_frame2 = al_get_bitmap_height(capsula);
    float escala2 = 3.5 * escaladoX;

    // Obtener el tiempo actual y calcular el delta (diferencia de tiempo)
    double tiempo_actual2 = al_get_time();
    double delta2 = tiempo_actual2 - tiempo_anterior2;
    tiempo_anterior2 = tiempo_actual2;

    // Ciclo de animación
    tiempo_transcurrido2 += delta2;  // Aumenta el tiempo transcurrido

    if (tiempo_transcurrido2 >= tiempo_animacion2) {
        // Cambiar al siguiente frame
        frame_actual2 = (frame_actual2 + 1) % frames_totales2;
        tiempo_transcurrido2 = 0;  // Reiniciar el tiempo transcurrido
    }

    // Dibujar el frame actual escalado en la pantalla
    al_draw_scaled_bitmap(
        capsula,
        frame_actual2 * ancho_frame2, 0, // Coordenadas del origen del frame actual
        ancho_frame2, alto_frame2,       // Dimensiones del frame original
        x, y,                        // Posición en la pantalla donde se dibuja
        ancho_frame2 * escala2, alto_frame2 * escala2, // Dimensiones escaladas
        0                                // Sin flags adicionales
    );
    if (capsu->y >= plataforma_y - plataforma_altura / 2.0 && capsu->x >= plataforma_x - plataforma_anchura - (5.0 * escaladoX) / 2.0 && capsu->x <= plataforma_x + plataforma_anchura / 2.0 && capsu->y <= 950 * escaladoY1) {
        capsu->x = 19999999;
        capsulacaida = true;
        tipocapsula = capsu->tipo;
    }
    else {
        capsu->y = capsu->y + 3;
    }
}

void dibujarCapsulas() {
    ptrcapsulas actual = listacapsula;
    ptrcapsulas anterior = nullptr;
    while (actual != nullptr) {
        if (actual->x == 19999999) {  // Comparación correcta con ==
            if (anterior == nullptr) {
                listacapsula = actual->siguiente;  // Eliminar el primer nodo
            }
            else {
                anterior->siguiente = actual->siguiente;  // Eliminar un nodo intermedio
            }
            ptrcapsulas bloque_a_eliminar = actual;  // Guardar el bloque para eliminarlo
            ptrpoderes nuevo = crearPoder(bloque_a_eliminar->tipo, al_get_time());
            agregarPoderes(nuevo);
            actual = actual->siguiente;  // Avanzar al siguiente bloque
            delete bloque_a_eliminar;  // Liberar la memoria del bloque eliminado
        }
        else {
            capsulasdibujadas(actual);  // Dibujar solo si no fue eliminada
            anterior = actual;  // Mover el puntero 'anterior'
            actual = actual->siguiente;  // Mover al siguiente nodo
        }
    }
}

int achocado(ptrbola bola, ptrbloques bloque) {
    int margen = 1;
    if (bola->x - bola->radio <= bloque->plataformax + margen && bola->x + bola->radio >= bloque->plataformax - margen &&
        bola->y >= bloque->plataformay && bola->y <= bloque->plataformaaltura &&
        bola->velocidadX > 0) {
        jugadoractual->bolasreb += 1;
        return 1;
    }
    if (bola->x - bola->radio <= bloque->plataformaanchura + margen && bola->x + bola->radio >= bloque->plataformaanchura - margen &&
        bola->y >= bloque->plataformay && bola->y <= bloque->plataformaaltura &&
        bola->velocidadX < 0) {
        jugadoractual->bolasreb += 1;
        return 1;
    }

    if (bola->y - bola->radio <= bloque->plataformay + margen && bola->y + bola->radio >= bloque->plataformay - margen &&
        bola->x >= bloque->plataformax && bola->x <= bloque->plataformaanchura &&
        bola->velocidadY > 0) {
        jugadoractual->bolasreb += 1;
        return 2;
    }

    // Colisión con la parte inferior del bloque
    if (bola->y - bola->radio <= bloque->plataformaaltura + margen && bola->y + bola->radio >= bloque->plataformaaltura - margen &&
        bola->x >= bloque->plataformax && bola->x <= bloque->plataformaanchura &&
        bola->velocidadY < 0) {
        jugadoractual->bolasreb += 1;
        return 2;
    }
    return 0;
}

bool capsularandom() {
    if (nivel1) {
        if (random1 == 2) {
            return true;
        }
    }
    else if (nivel2) {
        if (random2 == 2) {
            return true;
        }
    }
    else if (nivel3) {
        if (random3 == 2) {
            return true;
        }
    }
    return false;
}

void manejarcolision(ptrbola& bola) {
    ptrbloques bloque = listabloques;
    ptrbloques anterior = nullptr; // Para llevar un seguimiento del bloque anterior

    while (bloque != nullptr) {
        if (achocado(bola, bloque) == 1) {
            bola->velocidadX = -bola->velocidadX;
            puntos += 150;
            bloque->golpe--;
        }
        else if (achocado(bola, bloque) == 2) {
            bola->velocidadY = -bola->velocidadY;
            puntos += 150;
            bloque->golpe--;
        }

        // Si el bloque ha sido golpeado hasta 0
        if (bloque->golpe <= 0) {
            jugadoractual->objetosdestruidos += 1;
            if (anterior == nullptr) {
                // El bloque a eliminar es el primero en la lista
                listabloques = bloque->siguiente;
            }
            else {
                // El bloque a eliminar no es el primero
                anterior->siguiente = bloque->siguiente;
            }
            ptrbloques bloque_a_eliminar = bloque; // Guardar el bloque para eliminarlo
            bloque = bloque->siguiente; // Avanzar al siguiente bloque
            bool valor = capsularandom();
            if (valor) {
                ptrcapsulas nuevo = crearCapsula(((bloque_a_eliminar->plataformax + bloque_a_eliminar->plataformaanchura) / 2), bloque_a_eliminar->plataformaaltura, bloque_a_eliminar->tipodebloque);
                agregarCapsulas(nuevo);
            }
            delete bloque_a_eliminar; // Liberar la memoria del bloque eliminado
        }
        else {
            anterior = bloque; // Actualizar el bloque anterior solo si no se eliminó
            bloque = bloque->siguiente; // Avanzar al siguiente bloque
        }
    }
}

void manejarcolisionvivos(ptrbola& bola) {
    ptrbloques bloque = listabloquesvivos;
    ptrbloques anterior = nullptr; // Para llevar un seguimiento del bloque anterior

    while (bloque != nullptr) {
        if (achocado(bola, bloque) == 1) {
            bola->velocidadX = -bola->velocidadX;
            bloque->golpe--;
        }
        else if (achocado(bola, bloque) == 2) {
            bola->velocidadY = -bola->velocidadY;
            bloque->golpe--;
        }
        else {
            anterior = bloque; // Actualizar el bloque anterior solo si no se eliminó
            bloque = bloque->siguiente; // Avanzar al siguiente bloque
        }
    }
}

void lanzarBolaDesdePlataforma(float plataforma_x, float plataforma_y, float plataforma_altura) {
    if (!bolaLanzada || true) {
        float velocidadX = (4.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0f))) + elementonivel * escaladoX1;
        float velocidadY = -5.0f - elementonivel * escaladoY1; // Lanza la bola hacia arriba
        ptrbola nuevabola = crearBola(plataforma_x, plataforma_y - plataforma_altura / 2.0 - 10.0f, velocidadX, velocidadY, 10.0f);
        jugadoractual->totalbolas += 1;
        agregarBola(nuevabola); // 10.0f es el radio de la bola
        bolaLanzada = true; // Marca la bola como lanzada
    }
    
}

void actualizarBolas(float plataforma_x, float plataforma_y, float plataforma_anchura, float plataforma_altura) {
    ptrbola actual = listabola;
    ptrbola anterior = nullptr;

    while (actual != nullptr) {
        // Actualiza la posición
        actual->x += actual->velocidadX;
        actual->y += actual->velocidadY;

        // Colisiones con las paredes
        if (actual->x - actual->radio < anchurainicio + 44 * escaladoX1 || actual->x + actual->radio > anchurafinal - 44 * escaladoX1) {
            actual->velocidadX *= -1; // Rebotar en la pared izquierda o derecha
        }
        if (actual->y - actual->radio < 95 * escaladoY1) {
            actual->velocidadY *= -1; // Rebotar en la parte superior
        }

        manejarcolision(actual);
        manejarcolisionvivos(actual);

        // Colisión con la plataforma
        if (actual->y + actual->radio >= plataforma_y - plataforma_altura / 2.0 && actual->x >= plataforma_x - (plataforma_anchura / 2.0) && actual->x <= plataforma_x + (plataforma_anchura / 2.0)) {
            actual->velocidadY *= -1; 
            actual->y = plataforma_y - plataforma_altura / 2.0 - actual->radio;
            puntos += 50;
        }

        // Si cae más allá de la pantalla, eliminar la bola
        if (actual->y - actual->radio > 1040 * escaladoY1) { // Prueba con un valor fijo
            Bola* Aux = actual;
            if (anterior) {
                anterior->siguiente = actual->siguiente; 
            }
            else {
                listabola = actual->siguiente; // Actualiza la cabeza si se elimina el primer nodo
            }
            actual = actual->siguiente; // Avanza al siguiente nodo
            delete Aux; // Libera memoria
            jugadoractual->bolasper += 1;
        }
        else {
            anterior = actual;
            actual = actual->siguiente; // Avanza al siguiente nodo
        }
    }

    // Verifica si la lista está vacía
    if (listabola == nullptr) {
        juegoPerdido = true;
    }
}



void liberarBolas() {
    while (listabola != nullptr) {
        ptrbola Aux = listabola;
        listabola = listabola->siguiente;
        delete Aux;
    }
}

void liberarBloques() {
    while (listabloques != nullptr) {
        ptrbloques Aux = listabloques;
        listabloques = listabloques->siguiente;
        delete Aux;
    }
}

void liberarBloquesvidas(){
    while (listabloques != nullptr) {
        ptrbloques Aux = listabloquesvivos;
        listabloques = listabloques->siguiente;
        delete Aux;
    }
}
void liberarCapsulas() {
    while (listacapsula != nullptr) {
        ptrcapsulas Aux = listacapsula;
        listacapsula = listacapsula->siguiente;
        delete Aux;
    }
}
void liberarPoderes() {
    while (listapoderes != nullptr) {
        ptrpoderes Aux = listapoderes;
        listapoderes = listapoderes->siguiente;
        delete Aux;
    }
}


void velocidadbola(int mas) {
    ptrbola aux = listabola;
    if (mas == 0) {
        while (aux != nullptr) {
            if (aux->velocidadX > 0) {
                aux->velocidadX = aux->velocidadX - 3;
            }
            else {
                aux->velocidadX = aux->velocidadX + 3;
            }
            if (aux->velocidadY > 0) {
                aux->velocidadY = aux->velocidadY - 3;
            }
            else {
                aux->velocidadY = aux->velocidadY + 3;
            }
            aux = aux->siguiente;
        }
    }
    else if (mas == 1) { //Devolver a la normalidad
        while (aux != nullptr) {
            if (aux->velocidadX > 0) {
                aux->velocidadX = aux->velocidadX + 3;
            }
            else {
                aux->velocidadX = aux->velocidadX - 3;
            }
            if (aux->velocidadY > 0) {
                aux->velocidadY = aux->velocidadY + 3;
            }
            else{
                aux->velocidadY = aux->velocidadY - 3;
            }
            aux = aux->siguiente;
        }
    }
    else if (mas == 3) {
        while (aux != nullptr) {
            if (aux->velocidadX > 0) {
                aux->velocidadX = aux->velocidadX - 2;
            }
            else {
                aux->velocidadX = aux->velocidadX + 2;
            }
            if (aux->velocidadY > 0) {
                aux->velocidadY = aux->velocidadY - 2;
            }
            else {
                aux->velocidadY = aux->velocidadY + 2;
            }
            aux = aux->siguiente;
        }
    }
    else if (mas == 2) {
        while (aux != nullptr) {
            if (aux->velocidadX > 0) {
                aux->velocidadX = aux->velocidadX + 2;
            }
            else {
                aux->velocidadX = aux->velocidadX - 2;
            }
            if (aux->velocidadY > 0) {
                aux->velocidadY = aux->velocidadY + 2;
            }
            else {
                aux->velocidadY = aux->velocidadY - 2;
            }
            aux = aux->siguiente;
        }
    }
}

void lanzarBolaDesdeOtra() {
    float velocidadX = ((4.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0f))) + elementonivel) * escaladoX1;
    float velocidadX2 = (-1 * ((4.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0f))) - elementonivel) * escaladoX1);
    float velocidadY = (- 5.0f - elementonivel)* escaladoY1; // Lanza la bola hacia arriba
    ptrbola nuevabola = crearBola(listabola->x, listabola->y, velocidadX, listabola->velocidadY, 10.0f);
    ptrbola nuevabola2 = crearBola(listabola->x, listabola->y, velocidadX2, velocidadY, 10.0f);
    jugadoractual->totalbolas += 2;
    agregarBola(nuevabola); // 10.0f es el radio de la bola
    agregarBola(nuevabola2);
}

void lanzarBolaDesdeTODAS() {
    ptrbola aux = listabola;
    while (aux != nullptr) {
        float velocidadX = ((4.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0f))) + elementonivel) * escaladoX1;
        float velocidadX2 = ( - 1 * ((4.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0f))) - elementonivel) * escaladoX1);
        float velocidadY = (- 5.0f - elementonivel) * escaladoY1; // Lanza la bola hacia arriba
        ptrbola nuevabola = crearBola(aux->x, aux->y, velocidadX, aux->velocidadY, 10.0f);
        ptrbola nuevabola2 = crearBola(aux->x, aux->y, velocidadX2, velocidadY, 10.0f);
        jugadoractual->totalbolas += 2;
        agregarBola(nuevabola); // 10.0f es el radio de la bola
        agregarBola(nuevabola2);
        aux = aux->siguiente;
    }
}

void manejarPoderes() {
    ptrpoderes aux = listapoderes;
    ptrpoderes anterior = nullptr; // Puntero para mantener el nodo anterior

    while (aux != nullptr) {
        // Verifica si ha pasado el tiempo del poder
        if (al_get_time() - aux->tiempo >= 5.0) {
            // Restaura los valores de la plataforma si tipo == 1
            if (aux->tipo == 1) {
                plataforma_x += 30; // Restaura la posición de la plataforma
                plataforma_anchura -= 30; // Restaura la anchura de la plataforma
                velocidad = velocidad + 5;
            }
            if (aux->tipo == 3) {
                plataforma_x -= 40; // Restaura la posición de la plataforma
                plataforma_anchura += 40; // Restaura la anchura de la plataforma
                velocidad = velocidad - 10;
            }
            if (aux->tipo == 4) {
                velocidadbola(1);
            }
            if (aux->tipo == 5) {
                velocidadbola(3);
            }
            // Eliminar el nodo aux
            if (anterior != nullptr) {
                anterior->siguiente = aux->siguiente; // Salta el nodo que se va a eliminar
            }
            else {
                listapoderes = aux->siguiente; // Si es el primer nodo, actualiza la cabeza de la lista
            }
            ptrpoderes temp = aux; // Guarda el nodo a eliminar
            aux = aux->siguiente; // Avanza al siguiente nodo
            delete temp; // Libera la memoria del nodo eliminado
        }
        else {
            // Si no ha pasado el tiempo, solo avanzamos
            if (al_get_time() - aux->tiempo <= 5 && aux->usado == 0) {
                if (aux->tipo == 1) {
                    plataforma_x -= 30;
                    plataforma_anchura += 30;
                    velocidad = velocidad - 5;
                    aux->usado = 1; // Cambia a usado correctamente
                }
                else if (aux->tipo == 2) {
                    vidas = vidas + 2;
                    aux->usado = 1;
                }
                else if (aux->tipo == 3) {
                    plataforma_x += 40;
                    plataforma_anchura -= 40;
                    velocidad = velocidad + 10;
                    aux->usado = 1;
                }
                else if (aux->tipo == 4) {
                    velocidadbola(0);
                    aux->usado = 1;
                }
                else if (aux->tipo == 5) {
                    velocidadbola(2);
                    aux->usado = 1;
                }
                else if (aux->tipo == 6) {
                    lanzarBolaDesdeOtra();
                    aux->usado = 1;
                }
                else if (aux->tipo == 7) {
                    lanzarBolaDesdeTODAS();
                    aux->usado = 1;
                }
            }
            anterior = aux; // Actualiza el puntero anterior
            aux = aux->siguiente; // Avanza al siguiente nodo
        }
    }
}
////////////////////////////////////////////////////PALETA////////////////////////////////////////////////////
void paleta(ALLEGRO_EVENT_QUEUE*& coladeeventos, ALLEGRO_EVENT& evento, ALLEGRO_FONT* fuente) {
    ALLEGRO_BITMAP* imagenPlataforma = al_load_bitmap("sprites/nave/normal.png");
    if (!imagenPlataforma) {
        // Manejar el error si la imagen no se carga
        al_draw_filled_rectangle(plataforma_x - plataforma_anchura / 2.0, plataforma_y - plataforma_altura / 2.0, plataforma_x + plataforma_anchura / 2.0, plataforma_y + plataforma_altura / 2.0, colorplataforma);
        al_draw_filled_rectangle(plataforma_x - plataforma_anchura / 2.0 + 15 * escaladoY1, plataforma_y - plataforma_altura / 2.0 - 5 * escaladoY1, plataforma_x - plataforma_anchura / 2.0 + 30 * escaladoY1, plataforma_y + plataforma_altura / 2.0 + 5 * escaladoY1, colorplataforma2);
        al_draw_filled_rectangle(plataforma_x + plataforma_anchura / 2.0 - 15 * escaladoY1, plataforma_y - plataforma_altura / 2.0 - 5 * escaladoY1, plataforma_x + plataforma_anchura / 2.0 - 30 * escaladoY1, plataforma_y + plataforma_altura / 2.0 + 5 * escaladoY1, colorplataforma2);
    }
    else {
        // Dibujar la imagen escalada a las dimensiones del primer rectángulo
        al_draw_scaled_bitmap(imagenPlataforma, 0, 0, al_get_bitmap_width(imagenPlataforma), al_get_bitmap_height(imagenPlataforma), plataforma_x - plataforma_anchura / 2.0, 
        plataforma_y - plataforma_altura / 2.0, plataforma_anchura, plataforma_altura,  0);                                     
    }
    if (!bolaLanzada && (!(nivel3))) {
        al_draw_filled_circle(plataforma_x, plataforma_y - plataforma_altura / 2.0, 13.0f, al_map_rgb(0, 0, 0));
        al_draw_filled_circle(plataforma_x, plataforma_y - plataforma_altura / 2.0, 10.0f, al_map_rgb(255, 0, 0)); // Color rojo para la bola
    }
    else if (!bolaLanzada && ((nivel3))) {
        al_draw_filled_circle(plataforma_x, plataforma_y - plataforma_altura / 2.0, 13.0f, al_map_rgb(255, 255, 255));
        al_draw_filled_circle(plataforma_x, plataforma_y - plataforma_altura / 2.0, 10.0f, colorarcoiris(tiempo)); // Color rojo para la bola
    }
    if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (evento.keyboard.keycode) {
        case ALLEGRO_KEY_A:
            plataforma_x -= velocidad;
            break;
        case ALLEGRO_KEY_D:
            plataforma_x += velocidad;
            break;
        case ALLEGRO_KEY_ENTER:
            lanzarBolaDesdePlataforma(plataforma_x, plataforma_y, plataforma_altura);
            break;
        case ALLEGRO_KEY_SPACE:
            lanzarBolaDesdePlataforma(plataforma_x, plataforma_y, plataforma_altura);
            break;
        }
    }
    if (plataforma_x - plataforma_anchura / 2.0 < anchurainicio + 30 * escaladoX) {
        plataforma_x = (anchurainicio + plataforma_anchura / 2.0) + 30 * escaladoX;
    }
    else if (plataforma_x + plataforma_anchura / 2.0 > anchurafinal - 30 * escaladoX) {
        plataforma_x = (anchurafinal - plataforma_anchura / 2.0) - 30 * escaladoX;
    }
    if (bolaLanzada == true) {
        actualizarBolas(plataforma_x, plataforma_y, plataforma_anchura, plataforma_altura);
    }
    dibujarBolas();
    dibujarBloques();
    dibujarBloquesvivos();
}

////////////////////////////////////////////////////VIDAS////////////////////////////////////////////////////
bool quedanvidas() {
    if (vidas <= 0) {
        puntosreales = 0;
        puntos = 0;
        return false;
    }
    return true;
}

bool quedanbloques() {
    if (listabloques == nullptr && ((!juegoPerdido && iniciadoprimeravez))) {
        return false;
    }
    return true;
}

void hazperdidounavida() {
    if (quedanvidas()) {
        mostrarmensaje = true;
        tiempoMensaje = al_get_time(); // Obtén el tiempo actual
        bolaLanzada = false;
    }
    else {
        mostrarmensaje = false;
        bolaLanzada = false;
        iniciadoprimeravez = false;
    }
    puntos = puntosreales;
}

void perdidavida(ALLEGRO_FONT* fuente) {
    if (mostrarmensaje) {
        al_draw_text(fuente, al_map_rgb(255, 255, 255), 445 * escaladoX1, 300 * escaladoY1, 0, "Haz Perdido Una Vida");
        plataforma_x = ((anchurainicio + anchurafinal) / 2.0) * escaladoX1;
        plataforma_y = (1000 - 50) * escaladoY1;
        plataforma_anchura = 180 * escaladoX1; 
        plataforma_altura = 20 * escaladoY1; 
        velocidad = 60 * escaladoX1;        
        if (al_get_time() - tiempoMensaje >= 5.0f) {
            mostrarmensaje = false; // Oculta el mensaje
            juegoPerdido = false;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void abrircompuerta() {
    ALLEGRO_BITMAP* abierto = al_load_bitmap("sprites/backgrounds/10.png");

    int total_frames = 6;
    int frame_height = al_get_bitmap_height(abierto) / total_frames;
    int frame_width = al_get_bitmap_width(abierto);

    // Variables estáticas
    static double ultimo_cambio = 0.0;
    static int frame = 0;
    static bool en_ultimo_frame = false;  // Para controlar si estamos en el último frame
    static bool animacion_reversa = false; // Para controlar si la animación es en reversa
    static double tiempo_en_ultimo_frame = 0.0;  // Tiempo en que llegamos al último frame
    static double ultimo_cambio_color = 0.0;  // Tiempo del último cambio de color
    int z = rand() % 2;
    if (z == 0 && (!(random))) {
        compuertax = 532;
        compuertay = 49.75;
        random = true;
    }
    else if (z != 0 && (!(random))) {
        compuertax = 998.3;
        compuertay = 49.75;
        random = true;
    }

    double tiempo_actual = al_get_time(); // Obtener el tiempo actual

    // Controlar la animación
    if (!animacionterminada) {
        if (en_ultimo_frame) {
            // Si estamos en el último frame, esperar 8 segundos antes de continuar
            if (tiempo_actual - tiempo_en_ultimo_frame >= 8.0) {
                en_ultimo_frame = false;  // Resetear el indicador de último frame
                animacion_reversa = true;  // Cambiar a animación en reversa
                ultimo_cambio = tiempo_actual;  // Reiniciar el temporizador
            }
        }
        else {
            // Controlar el cambio de frames cada medio segundo (0.5 segundos)
            if (tiempo_actual - ultimo_cambio >= 0.3) {
                if (animacion_reversa) {
                    frame--;  // Animación en reversa

                    if (frame < 0) {
                        frame = 0;  // Detener la reversa en el primer frame
                        animacionterminada = true;
                        animacion_reversa = false;  // Regresar a animación normal
                        random = false;
                    }
                }
                else {
                    frame++;  // Animación normal

                    if (frame >= total_frames) {
                        // Si llegamos al último frame
                        frame = total_frames - 1;  // Mantener el último frame
                        en_ultimo_frame = true;  // Marcar que estamos en el último frame
                        tiempo_en_ultimo_frame = tiempo_actual;  // Guardar el tiempo en que llegamos al último frame
                    }
                }
                ultimo_cambio = tiempo_actual; // Actualizar el tiempo del último cambio de frame
            }
        }

        int frameX = 0;
        int frameY = frame * frame_height;
        if (nivel3) {
            ALLEGRO_COLOR color;
            if (frames >= 20 && frames <= 27) {
                color = al_map_rgba_f(125 / 255.0, 75 / 255.0, 125 / 255.0, 1.0f);
            }
            else if (frames >= 28 && frames <= 35) {
                color = al_map_rgba_f(100 / 255.0, 50 / 255.0, 100 / 255.0, 1.0f);
            }
            else if (frames >= 36 && frames <= 40) {
                color = al_map_rgba_f(55 / 255.0, 30 / 255.0, 55 / 255.0, 1.0f);
            }
            else if (frames >= 41 && frames <= 51) {
                color = al_map_rgba_f(100 / 255.0, 75 / 255.0, 100 / 255.0, 1.0f);
            }
            else if (frames >= 52 && frames <= 60) {
                color = al_map_rgba_f(120 / 255.0, 95 / 255.0, 120 / 255.0, 1.0f);
            }
            else {
                color = al_map_rgba_f(120 / 255.0, 95 / 255.0, 120 / 255.0, 1.0f);
            }

            al_draw_tinted_scaled_bitmap(abierto, color, frameX, frameY, frame_width, frame_height, compuertax * escaladoX, compuertay * escaladoY, frame_width * (4.15 * escaladoX), frame_height * (4.06 * escaladoY), 0);
        }
        else {
            al_draw_scaled_bitmap(abierto, frameX, frameY, frame_width, frame_height,
                compuertax * escaladoX, compuertay * escaladoY,
                frame_width * (4.15 * escaladoX), frame_height * (4.06 * escaladoY), 0);
        }
    }
}

//tipocapsula
void iniciarnivel1(ALLEGRO_FONT* fuente, ALLEGRO_TIMER* temporizador_bola, ALLEGRO_EVENT_QUEUE* coladeeventos, ALLEGRO_EVENT& evento) {
    srand(time(0));
    elementonivel = 0;
    ALLEGRO_BITMAP* imagen = al_load_bitmap("sprites/backgrounds/1.png");
    if (!imagen) {
        al_draw_filled_rectangle(anchurainicio, 50 * escaladoY1, anchurafinal, 1000 * escaladoY1, al_map_rgb(0, 0, 155));
    }
    else 
    {
        al_draw_scaled_bitmap(imagen, 0, 0, al_get_bitmap_width(imagen), al_get_bitmap_height(imagen), anchurainicio, 50 * escaladoY1, anchurafinal - anchurainicio, 
            (1000 * escaladoY1) - (50 * escaladoY1), 0);
    }

    al_draw_text(fuente, al_map_rgb(255, 255, 255), 1350 * escaladoX1, 300 * escaladoY1, 0, "Puntos");
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 1350 * escaladoX1, 350 * escaladoY1, 0, to_string(puntos).c_str());
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 1350 * escaladoX1, 400 * escaladoY1, 0, "Vidas");
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 1350 * escaladoX1, 450 * escaladoY1, 0, to_string(vidas).c_str());
    if (juegoPerdido && vidas > 0 && !mostrarmensaje) {
        vidas = vidas - 1;
        hazperdidounavida();
    }
    else if (!mostrarmensaje){
        paleta(coladeeventos, evento, fuente);
    }
    if (juegoPerdido || !iniciadoprimeravez) {
        liberarBolas();
        liberarBloques();
        liberarBloquesvidas();
        listabloquesvivos = nullptr;
        liberarPoderes();
        liberarCapsulas();
        int x1 = 480;
        int x2 = 630;
        int x3 = 780;
        int x4 = 930;
        int x5 = 1080;
        int y1 = 200;
        int y2 = 250;
        int multiplicador = 0;
        for (int i = 0; i < 4; i++) {
            if (i == 1 || i == 2) {
                ptrbloques nuevo = crearBloque(x1 * escaladoX1, (y1 + (100 * multiplicador)) * escaladoY1, (x1 + 100) * escaladoX1, (y2 + (100 * multiplicador)) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
                ptrbloques nuevo4 = crearBloque(x5 * escaladoX1, (y1 + (100 * multiplicador)) * escaladoY1, (x5 + 100) * escaladoX1, (y2 + (100 * multiplicador)) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
                agregarBloques(nuevo);
                agregarBloques(nuevo4);
            }
            else if (i == 3) {
                ptrbloques nuevo = crearBloque(x1 * escaladoX1, (y1 + (100 * (multiplicador + 2))) * escaladoY1, (x1 + 100) * escaladoX1, (y2 + (100 * (multiplicador + 2))) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
                ptrbloques nuevo4 = crearBloque(x5 * escaladoX1, (y1 + (100 * (multiplicador + 2))) * escaladoY1, (x5 + 100) * escaladoX1, (y2 + (100 * (multiplicador + 2))) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
                agregarBloques(nuevo);
                agregarBloques(nuevo4);
            }
            multiplicador++;
        }
        ptrbloques nuevo22 = crearBloque(x3 * escaladoX1, (y1 + (100 * 0)) * escaladoY1, (x3 + 100) * escaladoX1, (y2 + (100 * 0)) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
        ptrbloques nuevo2 = crearBloque(x3 * escaladoX1, (y1 + (100 * 0)) * escaladoY1, (x3 + 100) * escaladoX1, (y2 + (100 * 0)) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
        ptrbloques nuevo33 = crearBloque(x3 * escaladoX1, (y1 + (100 * 3)) * escaladoY1, (x3 + 100) * escaladoX1, (y2 + (100 * 3)) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
        ptrbloques nuevos44 = crearBloque(x3 * escaladoX1, (y1 + (150)) * escaladoY1, (x3 + 100) * escaladoX1, (y2 + (150)) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
        agregarBloques(nuevo2);
        agregarBloques(nuevo33);
        agregarBloques(nuevos44);

        x1 = 380;
        x2 = 1180;
        y1 = 150;
        y2 = 200;
        x3 = 480;
        multiplicador = 1;
        for (int i = 0; i < 3; i++) {
            if (!(i == 1)) {
                ptrbloques nuevos = crearBloque(x1 * escaladoX1, (y1 + (100 * multiplicador)) * escaladoY1, (x1 + 100) * escaladoX1, (y2 + (100 * multiplicador)) * escaladoY1, 1 + rand() % 7, 333333333, al_map_rgb(45, 45, 85));
                ptrbloques nuevos1 = crearBloque(x2 * escaladoX1, (y1 + (100 * multiplicador)) * escaladoY1, (x2 + 100) * escaladoX1, (y2 + (100 * multiplicador)) * escaladoY1, 1 + rand() % 7, 333333333, al_map_rgb(45, 45, 85));
                agregarBloquesvivos(nuevos);
                agregarBloquesvivos(nuevos1);
            }
            multiplicador++;
        }
        ptrbloques nuevos4 = crearBloque(((x3)+(150 * 1)) * escaladoX1, (y1 + (500)) * escaladoY1, ((x3 + 100) + (150 * 1)) * escaladoX1, (y2 + (500)) * escaladoY1, 1 + rand() % 7, 333333333,  al_map_rgb(45, 45, 85));
        ptrbloques nuevos5 = crearBloque(((x3)+(150 * 3)) * escaladoX1, (y1 + (500)) * escaladoY1, ((x3 + 100) + (150 * 3)) * escaladoX1, (y2 + (500)) * escaladoY1, 1 + rand() % 7, 333333333,  al_map_rgb(45, 45, 85));
        agregarBloquesvivos(nuevos4);
        agregarBloquesvivos(nuevos5);
        iniciadoprimeravez = true;
    }
    dibujarCapsulas();
    manejarPoderes();
    abrircompuerta();
    perdidavida(fuente);
}

void iniciarnivel2(ALLEGRO_FONT* fuente, ALLEGRO_TIMER* temporizador_bola, ALLEGRO_EVENT_QUEUE* coladeeventos, ALLEGRO_EVENT& evento) {
        srand(time(0));
        ALLEGRO_BITMAP* imagen = al_load_bitmap("sprites/backgrounds/2.png");
        if (!imagen) {
            al_draw_filled_rectangle(anchurainicio, 50 * escaladoY1, anchurafinal, 1000 * escaladoY1, al_map_rgb(0, 0, 155));
        }
        else
        {
            al_draw_scaled_bitmap(imagen, 0, 0, al_get_bitmap_width(imagen), al_get_bitmap_height(imagen), anchurainicio, 50 * escaladoY1, anchurafinal - anchurainicio,
                (1000 * escaladoY1) - (50 * escaladoY1), 0);
        }

        al_draw_text(fuente, al_map_rgb(255, 255, 255), 1350 * escaladoX1, 300 * escaladoY1, 0, "Puntos");
        al_draw_text(fuente, al_map_rgb(255, 255, 255), 1350 * escaladoX1, 350 * escaladoY1, 0, to_string(puntos).c_str());
        al_draw_text(fuente, al_map_rgb(255, 255, 255), 1350 * escaladoX1, 400 * escaladoY1, 0, "Vidas");
        al_draw_text(fuente, al_map_rgb(255, 255, 255), 1350 * escaladoX1, 450 * escaladoY1, 0, to_string(vidas).c_str());
        if (juegoPerdido && vidas > 0 && !mostrarmensaje) {
            vidas = vidas - 1;
            hazperdidounavida();
        }
        else if (!mostrarmensaje) {
            paleta(coladeeventos, evento, fuente);
        }
        if (juegoPerdido || !iniciadoprimeravez) {
            liberarBolas();
            liberarBloques();
            liberarBloquesvidas();
            listabloquesvivos = nullptr;
            liberarPoderes();
            liberarCapsulas();
            int x1 = 520;
            int x2 = 550;
            int x3 = 780;
            int x4 = 930;
            int x5 = 1030;
            int y1 = 200;
            int y2 = 250;
            int y11 = 400;
            int y22 = 450;
            int multiplicador = 0;
            ptrbloques nuevo = crearBloque(x3 * escaladoX1, (y1 + (100 * multiplicador)) * escaladoY1, (x3 + 100) * escaladoX1, (y2 + (100 * multiplicador)) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
            ptrbloques nuevo2 = crearBloque(x3 * escaladoX1, (y1 + (53)) * escaladoY1, (x3 + 100) * escaladoX1, (y2 + (53)) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
            ptrbloques nuevo3 = crearBloque(x3 * escaladoX1, (y1 + (106)) * escaladoY1, (x3 + 100) * escaladoX1, (y2 + (106)) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
            ptrbloques nuevo4 = crearBloque((x3 - 103) * escaladoX1, (y1 + (53)) * escaladoY1, ((x3 - 103) + 100) * escaladoX1, (y2 + (53)) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
            ptrbloques nuevo5 = crearBloque((x3 + 103) * escaladoX1, (y1 + (53)) * escaladoY1, ((x3 + 103) + 100) * escaladoX1, (y2 + (53)) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());

            ptrbloques nuevo1 = crearBloque(x2 * escaladoX1, (y1 + 300 - 230) * escaladoY1, (x2 + 100) * escaladoX1, (y2 + 300 - 230) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
            ptrbloques nuevo12 = crearBloque(x2 * escaladoX1, (y1 + (53) + 300 - 230) * escaladoY1, (x2 + 100) * escaladoX1, (y2 + (53) + 300 - 230) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
            ptrbloques nuevo13 = crearBloque(x2 * escaladoX1, (y1 + (106) + 300 - 230) * escaladoY1, (x2 + 100) * escaladoX1, (y2 + (106) + 300 - 230) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
            ptrbloques nuevo14 = crearBloque((x2 - 103) * escaladoX1, (y1 + (53) + 300 - 230) * escaladoY1, ((x2 - 103) + 100) * escaladoX1, (y2 + (53) + 300 - 230) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
            ptrbloques nuevo15 = crearBloque((x2 + 103) * escaladoX1, (y1 + (53) + 300 - 230) * escaladoY1, ((x2 + 103) + 100) * escaladoX1, (y2 + (53) + 300 - 230) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());

            ptrbloques nuevo11 = crearBloque((x5 * escaladoX1) - 20, (y1 - 130 + 200) * escaladoY1, ((x5 + 100) * escaladoX1)-20, (y2 + 200 - 130) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
            ptrbloques nuevo112 = crearBloque((x5 * escaladoX1) - 20, (y1 - 130 + (53) + 200) * escaladoY1, ((x5 + 100) * escaladoX1)-20, (y2 + 200 + (53) - 130) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
            ptrbloques nuevo113 = crearBloque((x5 * escaladoX1) - 20, (y1 - 130 + (106) + 200) * escaladoY1, ((x5 + 100) * escaladoX1)-20, (y2 + 200 + (106) - 130) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
            ptrbloques nuevo114 = crearBloque(((x5 - 103) * escaladoX1) - 20, (y1 - 130 + (53) + 200) * escaladoY1, (((x5 - 103) + 100) * escaladoX1) - 20, (y2 - 130 + (53) + 200) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
            ptrbloques nuevo115 = crearBloque(((x5 + 103) * escaladoX1) - 20, (y1 - 130 + (53) + 200) * escaladoY1, (((x5 + 103) + 100) * escaladoX1) - 20, (y2 - 130 + (53) + 200) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());

            ptrbloques nuevo111 = crearBloque((x5 * escaladoX1) - 20, (y11 - 130 + 200) * escaladoY1, ((x5 + 100) * escaladoX1) - 20, (y22 + 200 - 130) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
            ptrbloques nuevo1112 = crearBloque((x5 * escaladoX1) - 20, (y11 - 130 + (53) + 200) * escaladoY1, ((x5 + 100) * escaladoX1) - 20, (y22 + 200 + (53) - 130) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
            ptrbloques nuevo1113 = crearBloque((x5 * escaladoX1) - 20, (y11 - 130 + (106) + 200) * escaladoY1, ((x5 + 100) * escaladoX1) - 20, (y22 + 200 + (106) - 130) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
            ptrbloques nuevo1114 = crearBloque(((x5 - 103) * escaladoX1) - 20, (y11 - 130 + (53) + 200) * escaladoY1, (((x5 - 103) + 100) * escaladoX1) - 20, (y22 - 130 + (53) + 200) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
            ptrbloques nuevo1115 = crearBloque(((x5 + 103) * escaladoX1) - 20, (y11 - 130 + (53) + 200) * escaladoY1, (((x5 + 103) + 100) * escaladoX1) - 20, (y22 - 130 + (53) + 200) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());

            ptrbloques nuevo1111 = crearBloque(x2 * escaladoX1, (y11 + 300 - 230) * escaladoY1, (x2 + 100) * escaladoX1, (y22 + 300 - 230) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
            ptrbloques nuevo11112 = crearBloque(x2 * escaladoX1, (y11 + (53) + 300 - 230) * escaladoY1, (x2 + 100) * escaladoX1, (y22 + (53) + 300 - 230) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
            ptrbloques nuevo11113 = crearBloque(x2 * escaladoX1, (y11 + (106) + 300 - 230) * escaladoY1, (x2 + 100) * escaladoX1, (y22 + (106) + 300 - 230) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
            ptrbloques nuevo11114 = crearBloque((x2 - 103) * escaladoX1, (y11 + (53) + 300 - 230) * escaladoY1, ((x2 - 103) + 100) * escaladoX1, (y22 + (53) + 300 - 230) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
            ptrbloques nuevo11115 = crearBloque((x2 + 103) * escaladoX1, (y11 + (53) + 300 - 230) * escaladoY1, ((x2 + 103) + 100) * escaladoX1, (y22 + (53) + 300 - 230) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());

            ptrbloques indes = crearBloque((x2 + 233) * escaladoX1, (y11 + 30) * escaladoY1, ((x2 + 233) + 100) * escaladoX1, (y22 + 30) * escaladoY1, 1 + rand() % 7, 333333333, al_map_rgb(45, 45, 85));
            ptrbloques indes1 = crearBloque((x1 - 123) * escaladoX1, (y11 + (53) + 300) * escaladoY1, ((x1 - 123) + 100) * escaladoX1, (y22 + (53) + 300) * escaladoY1, 1 + rand() % 7, 333333333, al_map_rgb(45, 45, 85));
            ptrbloques indes2 = crearBloque((x1 + 100) * escaladoX1, (y11 + (53) + 300) * escaladoY1, ((x1 + 100) + 100) * escaladoX1, (y22 + (53) + 300) * escaladoY1, 1 + rand() % 7, 333333333, al_map_rgb(45, 45, 85));
            ptrbloques indes3 = crearBloque((x1 + 410) * escaladoX1, (y11 + (53) + 300) * escaladoY1, ((x1 + 410) + 100) * escaladoX1, (y22 + (53) + 300) * escaladoY1, 1 + rand() % 7, 333333333, al_map_rgb(45, 45, 85));
            ptrbloques indes4 = crearBloque((x1 + 645) * escaladoX1, (y11 + (53) + 300) * escaladoY1, ((x1 + 645) + 100) * escaladoX1, (y22 + (53) + 300) * escaladoY1, 1 + rand() % 7, 333333333, al_map_rgb(45, 45, 85));

            agregarBloques(nuevo);
            agregarBloques(nuevo2);
            agregarBloques(nuevo3);
            agregarBloques(nuevo4);
            agregarBloques(nuevo5);
            agregarBloques(nuevo1);
            agregarBloques(nuevo12);
            agregarBloques(nuevo13);
            agregarBloques(nuevo14);
            agregarBloques(nuevo15);
            agregarBloques(nuevo11);
            agregarBloques(nuevo112);
            agregarBloques(nuevo113);
            agregarBloques(nuevo114);
            agregarBloques(nuevo115);
            agregarBloques(nuevo111);
            agregarBloques(nuevo1112);
            agregarBloques(nuevo1113);
            agregarBloques(nuevo1114);
            agregarBloques(nuevo1115);
            agregarBloques(nuevo1111);
            agregarBloques(nuevo11112);
            agregarBloques(nuevo11113);
            agregarBloques(nuevo11114);
            agregarBloques(nuevo11115);
            agregarBloquesvivos(indes);
            agregarBloquesvivos(indes1);
            agregarBloquesvivos(indes2);
            agregarBloquesvivos(indes3);
            agregarBloquesvivos(indes4);
            iniciadoprimeravez = true;
        }
        dibujarCapsulas();
        manejarPoderes();
        abrircompuerta();
        perdidavida(fuente);
}

void iniciarnivel3(ALLEGRO_FONT* fuente, ALLEGRO_TIMER* temporizador_bola, ALLEGRO_EVENT_QUEUE* coladeeventos, ALLEGRO_EVENT& evento) {
    srand(time(0));
    ALLEGRO_BITMAP* imagen = al_load_bitmap("sprites/backgrounds/3.png");
    if (!imagen) {
        al_draw_filled_rectangle(anchurainicio, 50 * escaladoY1, anchurafinal, 1000 * escaladoY1, al_map_rgb(0, 0, 155));
    }
    else
    {
        al_draw_scaled_bitmap(imagen, 0, 0, al_get_bitmap_width(imagen), al_get_bitmap_height(imagen), anchurainicio, 50 * escaladoY1, anchurafinal - anchurainicio,
            (1000 * escaladoY1) - (50 * escaladoY1), 0);
        cambiarbackground(imagen);

    }

    al_draw_text(fuente, al_map_rgb(255, 255, 255), 1350 * escaladoX1, 300 * escaladoY1, 0, "Puntos");
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 1350 * escaladoX1, 350 * escaladoY1, 0, to_string(puntos).c_str());
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 1350 * escaladoX1, 400 * escaladoY1, 0, "Vidas");
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 1350 * escaladoX1, 450 * escaladoY1, 0, to_string(vidas).c_str());
    if (juegoPerdido && vidas > 0 && !mostrarmensaje) {
        vidas = vidas - 1;
        hazperdidounavida();
    }
    else if (!mostrarmensaje) {
        paleta(coladeeventos, evento, fuente);
    }
    if (juegoPerdido || !iniciadoprimeravez) {
        elementonivel = 1.0;
        liberarBolas();
        liberarBloques();
        liberarBloquesvidas();
        listabloquesvivos = nullptr;
        liberarPoderes();
        liberarCapsulas();
        int x1 = 480;
        int x2 = 630;
        int x3 = 780;
        int x4 = 930;
        int x5 = 1080;
        int y1 = 200;
        int y2 = 250;
        int multiplicador = 0;
        for (int i = 0; i < 4; i++) {
            ptrbloques nuevo = crearBloque(x1 * escaladoX1, (y1 + (100 * multiplicador)) * escaladoY1, (x1 + 100) * escaladoX1, (y2 + (100 * multiplicador)) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
            ptrbloques nuevo1 = crearBloque(x2 * escaladoX1, (y1 + (100 * multiplicador)) * escaladoY1, (x2 + 100) * escaladoX1, (y2 + (100 * multiplicador)) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
            ptrbloques nuevo3 = crearBloque(x4 * escaladoX1, (y1 + (100 * multiplicador)) * escaladoY1, (x4 + 100) * escaladoX1, (y2 + (100 * multiplicador)) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
            ptrbloques nuevo4 = crearBloque(x5 * escaladoX1, (y1 + (100 * multiplicador)) * escaladoY1, (x5 + 100) * escaladoX1, (y2 + (100 * multiplicador)) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
            if (i == 0 || i == 3) {
                agregarBloques(nuevo1);
                agregarBloques(nuevo3);
            }
            else {
                agregarBloques(nuevo);
                agregarBloques(nuevo1);
                agregarBloques(nuevo3);
                agregarBloques(nuevo4);
            }
            multiplicador++;
        }
        ptrbloques nuevoz = crearBloque(780 * escaladoX1, (100 + (100 * 0)) * escaladoY1, (780 + 100) * escaladoX1, (150 + (100 * 0)) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
        ptrbloques nuevoz1 = crearBloque(780 * escaladoX1, (100 + (100 * 5)) * escaladoY1, (780 + 100) * escaladoX1, (150 + (100 * 5)) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
        ptrbloques nuevo2 = crearBloque(x3 * escaladoX1, (y1 + (100 * 0)) * escaladoY1, (x3 + 100) * escaladoX1, (y2 + (100 * 0)) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
        ptrbloques nuevo33 = crearBloque(x3 * escaladoX1, (y1 + (100 * 3)) * escaladoY1, (x3 + 100) * escaladoX1, (y2 + (100 * 3)) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
        ptrbloques nuevos44 = crearBloque(x3 * escaladoX1, (y1 + (150)) * escaladoY1, (x3 + 100) * escaladoX1, (y2 + (150)) * escaladoY1, 1 + rand() % 7, 333333333, al_map_rgb(85, 45, 85));
        agregarBloques(nuevoz);
        agregarBloques(nuevoz1);
        agregarBloques(nuevo2);
        agregarBloques(nuevo33);
        agregarBloquesvivos(nuevos44);

        x1 = 380;
        x2 = 1180;
        y1 = 150;
        y2 = 200;
        x3 = 480;
        multiplicador = 1;
        for (int i = 0; i < 3; i++) {
            if (i == 1) {
                ptrbloques nuevos = crearBloque(x1 * escaladoX1, (y1 + (100 * multiplicador)) * escaladoY1, (x1 + 100) * escaladoX1, (y2 + (100 * multiplicador)) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
                ptrbloques nuevos1 = crearBloque(x2 * escaladoX1, (y1 + (100 * multiplicador)) * escaladoY1, (x2 + 100) * escaladoX1, (y2 + (100 * multiplicador)) * escaladoY1, 1 + rand() % 7, 3, obtenerColorNeon());
                agregarBloques(nuevos);
                agregarBloques(nuevos1);
            }
            else {
                ptrbloques nuevos = crearBloque(x1 * escaladoX1, (y1 + (100 * multiplicador)) * escaladoY1, (x1 + 100) * escaladoX1, (y2 + (100 * multiplicador)) * escaladoY1, 1 + rand() % 7, 333333333, al_map_rgb(85, 45, 85));
                ptrbloques nuevos1 = crearBloque(x2 * escaladoX1, (y1 + (100 * multiplicador)) * escaladoY1, (x2 + 100) * escaladoX1, (y2 + (100 * multiplicador)) * escaladoY1, 1 + rand() % 7, 333333333, al_map_rgb(85, 45, 85));
                agregarBloquesvivos(nuevos);
                agregarBloquesvivos(nuevos1);
            }
            multiplicador++;
        }
        ptrbloques nuevos4 = crearBloque(((x3)+(150 * 1)) * escaladoX1, (y1 + (500)) * escaladoY1, ((x3 + 100) + (150 * 1)) * escaladoX1, (y2 + (500)) * escaladoY1, 1 + rand() % 7, 333333333, al_map_rgb(85, 45, 85));
        ptrbloques nuevos5 = crearBloque(((x3)+(150 * 3)) * escaladoX1, (y1 + (500)) * escaladoY1, ((x3 + 100) + (150 * 3)) * escaladoX1, (y2 + (500)) * escaladoY1, 1 + rand() % 7, 333333333, al_map_rgb(85, 45, 85));
        agregarBloquesvivos(nuevos4);
        agregarBloquesvivos(nuevos5);
        iniciadoprimeravez = true;
    }
    dibujarCapsulas();
    manejarPoderes();
    abrircompuerta();
    perdidavida(fuente);
}


int plataforma_x2 = 35;
int plataforma_x3 = 1835;
int plataforma_anchura2 = 37;
int plataforma_anchura3 = 1837;
int plataforma_y2 = 500;
int plataforma_y3 = 500;
int plataforma_altura2 = 550;
int plataforma_altura3 = 550;
int bitmap_ancho = 0;
int bitmap_alto = 0;
int bitmap_ancho2 = 0;
int bitmap_alto2 = 0;

void paletas(int cual, ALLEGRO_EVENT_QUEUE*& coladeeventos, ALLEGRO_EVENT& evento, ALLEGRO_FONT* fuente) {
    ALLEGRO_BITMAP* player1 = al_load_bitmap("sprites/Nave/normalpl1.png");
    ALLEGRO_BITMAP* player2 = al_load_bitmap("sprites/Nave/normalpl2.png");

    // Obtén el tamaño original del bitmap
    bitmap_ancho = al_get_bitmap_width(player1);
    bitmap_alto = al_get_bitmap_height(player1);
    bitmap_ancho2 = al_get_bitmap_width(player2);
    bitmap_alto2 = al_get_bitmap_height(player2);

    // Ajusta el escalado, reduciendo los valores si es necesario
    if (cual == 1) {
        al_draw_scaled_bitmap(player1, 0, 0, bitmap_ancho, bitmap_alto,
            plataforma_x2, plataforma_y2,
            bitmap_ancho * 3.5, bitmap_alto * 3.5, // Escalado al 50% como ejemplo
            0);
    }
    else if (cual == 2) {
        al_draw_scaled_bitmap(player2, 0, 0, bitmap_ancho2, bitmap_alto2,
            plataforma_x3, plataforma_y3,
            bitmap_ancho2 * 3.5, bitmap_alto2 * 3.5, // Escalado al 50% como ejemplo
            0);
    }

    al_destroy_bitmap(player1);
    al_destroy_bitmap(player2);
}

void iniciarduojugadores(ALLEGRO_FONT* fuente, ALLEGRO_TIMER* temporizador_bola, ALLEGRO_EVENT_QUEUE* coladeeventos, ALLEGRO_EVENT& evento) {

    ALLEGRO_BITMAP* imagen = al_load_bitmap("sprites/backgrounds/20.png");

        // Definir el punto de rotación en el centro de la imagen
    float centro_x = al_get_bitmap_width(imagen) / 2;
    float centro_y = al_get_bitmap_height(imagen) / 2;

        // Definir el área de destino en la pantalla
    float anchuradisponible = anchurafinal - anchurainicio;
    float alturadisponible = (1000 * escaladoY1) - (50 * escaladoY1);

        // Ajustar el escalado según las dimensiones disponibles y la rotación
    float escaladoX2 = alturadisponible / (al_get_bitmap_width(imagen) * 1.02 * escaladoY1);  // Escalado en X
    float escaladoY2 = anchuradisponible / (al_get_bitmap_height(imagen) * 1.02* escaladoY1); // Escalado en Y

        // Posición central en pantalla
    float pos_x = 430 * escaladoX1;
    float pos_x2 = 1488 * escaladoX1;
    float pos_y = 50 * escaladoY1 + alturadisponible / 2;


        // Dibujar la imagen escalada y rotada 90 grados
    al_draw_scaled_rotated_bitmap(
        imagen,
        centro_x, centro_y,                 // Centro de la imagen para rotación
        pos_x, pos_y,                       // Posición en la pantalla
        escaladoX2, escaladoY2,             // Escalado ajustado en X y Y
        ALLEGRO_PI / 2,                     // Rotar 90 grados (en radianes)
        0);

    al_draw_scaled_rotated_bitmap(
        imagen,
        centro_x, centro_y,                 // Centro de la imagen para rotación
        pos_x2, pos_y,                       // Posición en la pantalla
        escaladoX2, escaladoY2,             // Escalado ajustado en X y Y
        3 * ALLEGRO_PI / 2,                     // Rotar 90 grados (en radianes)
        0);


    ALLEGRO_FONT* fuentetitulo = al_load_ttf_font("font/ARCADE_I.Ttf", 100 * escaladoX, 0);
    al_draw_text(fuentetitulo, al_map_rgb(255, 255, 255), 910 * escaladoX1, 270 * escaladoY1, 0, to_string(vidajugador).c_str());
    al_draw_text(fuentetitulo, color_actual, 910 * escaladoX1, 485 * escaladoY1, 0, "|");
    al_draw_text(fuentetitulo, al_map_rgb(255, 255, 255), 910 * escaladoX1, 700 * escaladoY1, 0, to_string(vidajugador2).c_str());
    paletas(1, coladeeventos, evento, fuente);
    paletas(2, coladeeventos, evento, fuente);
}