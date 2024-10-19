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
const int anchop2 = GetSystemMetrics(SM_CXSCREEN);
const int alturap2 = GetSystemMetrics(SM_CYSCREEN);
float escaladoX1p2 = devolverx();
float escaladoY1p2 = devolvery();
const int anchurainiciop2 = 360 * escaladoX1p2;
const int anchurafinalp2 = 1300 * escaladoX1p2;
using namespace std;
float plataforma_xp2 = 30 * escaladoX1p2;
float plataforma_yp2 = 500 * escaladoY1p2; // Posiciona la plataforma en la parte inferior de la ventana
float plataforma_anchurap2 = 20 * escaladoX1p2; // Ancho de la plataforma
float plataforma_alturap2 = 180 * escaladoY1p2;   // Alto de la plataforma
float plataforma_xp3 = 1880 * escaladoX1p2;
float plataforma_yp3 = 500 * escaladoY1p2; // Posiciona la plataforma en la parte inferior de la ventana
float plataforma_anchurap3 = 20 * escaladoX1p2; // Ancho de la plataforma
float plataforma_alturap3 = 180 * escaladoY1p2;   // Alto de la plataforma
float velocidadp2 = 60 * escaladoX1p2;          // Velocidad de movimiento
float tiempoMensajep2 = 0.0f;
float tiempo_animacion2p2 = 0.15; // Tiempo en segundos entre frames (ajustado)
float tiempo_transcurrido2p2 = 0;
float compuertaxp2 = 0;
float compuertayp2 = 0;
bool juegoPerdidop2 = false;
bool juegoPerdidop3 = false;
bool bolaLanzadap2 = false;
bool bolaLanzadap3 = false;
bool mostrarmensajep2 = false;
bool iniciadoprimeravezp2 = false;
bool animacionterminadap2 = false;
bool juegoperdidoseriop1 = false;
bool juegoperdidoseriop2 = false;
bool randomp2 = false;
int contador_framep2 = 0;
int velocidad_framep2 = 100;
int frame2p2 = 0;
int vidasp2 = 3;
int frame_actual2p2 = 0;
int frames_totales2p2 = 8;
int vidajugador = 5;
int vidajugador2 = 5;
int elementonivelp2 = 0;
double tiempoInicioCapsulap2 = 0;
double frameenemigosp2 = 0.0f;
double tiempo_anterior2p2 = 0; // Tiempo del último frame

typedef struct Bolap2 {
    float x;
    float y;
    float velocidadX;
    float velocidadY;
    float radio;
    Bolap2* siguiente; // Puntero al siguiente nodo
}*ptrbolap2;

ptrbolap2 crearBolap2(float x, float y, float velocidadX, float velocidadY, float radio) {
    ptrbolap2 bola = new(Bolap2);
    bola->x = x;
    bola->y = y;
    bola->velocidadX = velocidadX;
    bola->velocidadY = velocidadY;
    bola->radio = radio;
    bola->siguiente = nullptr;
    return bola;
}

ptrbolap2 listabolap2 = nullptr;
ptrbolap2 listabolap3 = nullptr;

void agregarBolap2(ptrbolap2& bola) {
    bola->siguiente = listabolap2;
    listabolap2 = bola;
}

void agregarBolap3(ptrbolap2& bola) {
    bola->siguiente = listabolap3;
    listabolap3 = bola;
}

void lanzarBolaDesdePlataformap2(float plataforma_x, float plataforma_y, float plataforma_altura) {
    if (!bolaLanzadap2) {
        float velocidadX = 4.0f * escaladoX1p2;
        float velocidadY = 5.0f * escaladoY1p2; // Lanza la bola hacia arriba
        ptrbolap2 nuevabola = crearBolap2(plataforma_x + 50, plataforma_y, velocidadX, velocidadY, 10.0f);
        agregarBolap2(nuevabola); // 10.0f es el radio de la bola
        bolaLanzadap2 = true; // Marca la bola como lanzada
    }
}

void lanzarBolaDesdePlataformap3(float plataforma_x, float plataforma_y, float plataforma_altura) {
    if (!bolaLanzadap3) {
        float velocidadX = -4.0f * escaladoX1p2;
        float velocidadY = 5.0f * escaladoY1p2; // Lanza la bola hacia arriba
        ptrbolap2 nuevabola = crearBolap2(plataforma_x - 50, plataforma_y, velocidadX, velocidadY, 10.0f);
        agregarBolap3(nuevabola); // 10.0f es el radio de la bola
        bolaLanzadap3 = true; // Marca la bola como lanzada
    }
}

void dibujarBolasp2() {
    ptrbolap2 actual = listabolap2;
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

void dibujarBolasp3() {
    ptrbolap2 actual = listabolap3;
    while (actual != nullptr) {
        if (nivel3) {
            ALLEGRO_COLOR red40 = colorarcoiris(tiempo);
            al_draw_filled_circle(actual->x, actual->y, actual->radio + 3, al_map_rgb(255, 255, 255));
            al_draw_filled_circle(actual->x, actual->y, actual->radio, red40);
            actual = actual->siguiente;
        }
        else {
            al_draw_filled_circle(actual->x, actual->y, actual->radio + 3, al_map_rgb(0, 0, 0));
            al_draw_filled_circle(actual->x, actual->y, actual->radio, al_map_rgb(0, 0, 255));
            actual = actual->siguiente;
        }
    }
}

void actualizarBolasp3(float plataforma_x, float plataforma_y, float plataforma_anchura, float plataforma_altura) {
    ptrbolap2 actual = listabolap3;
    ptrbolap2 anterior = nullptr;

    while (actual != nullptr) {
        // Actualiza la posición de la bola
        actual->x += actual->velocidadX;
        actual->y += actual->velocidadY;

        // Colisiones con las paredes
        if (actual->x - actual->radio < 1000 * escaladoX1p2) { // Rebote en la pared izquierda
            actual->velocidadX *= -1;
        }
        if (actual->y - actual->radio < 150 * escaladoY1p2) { // Rebote en el techo
            actual->velocidadY *= -1;
        }
        if (actual->y + actual->radio > 880 * escaladoY1p2) { // Rebote en el piso
            actual->velocidadY *= -1;
        }

        // Colisión con la paleta (vertical, a la izquierda)
        if (actual->x + actual->radio >= plataforma_x - plataforma_anchura / 2.0 && // Verifica el lado derecho de la paleta
            actual->y >= plataforma_y - plataforma_altura / 2.0 && // Dentro del rango vertical de la paleta
            actual->y <= plataforma_y + plataforma_altura / 2.0) {
            actual->velocidadX *= -1; // Rebota en la paleta
            actual->x = plataforma_x - plataforma_anchura / 2.0 - actual->radio; // Ajusta la posición para evitar solapamiento
        }

        // Si la bola cae fuera de los límites de la pantalla
        if (actual->x - actual->radio > 1920 * escaladoX) { // Si la bola sale de la pantalla por la izquierda
            Bolap2* Aux = actual;
            if (anterior) {
                anterior->siguiente = actual->siguiente;
            }
            else {
                listabolap3 = actual->siguiente; // Actualiza la cabeza si se elimina el primer nodo
            }
            actual = actual->siguiente;
            delete Aux; // Libera memoria
        }
        else {
            anterior = actual;
            actual = actual->siguiente; // Avanza al siguiente nodo
        }
    }

    // Verifica si la lista está vacía
    if (listabolap3 == nullptr) {
        juegoPerdidop3 = true;
    }
}

void actualizarBolasp2(float plataforma_x, float plataforma_y, float plataforma_anchura, float plataforma_altura) {
    ptrbolap2 actual = listabolap2;
    ptrbolap2 anterior = nullptr;

    while (actual != nullptr) {
        // Actualiza la posición de la bola
        actual->x += actual->velocidadX;
        actual->y += actual->velocidadY;

        // Colisiones con las paredes
        if (actual->x + actual->radio > 900 * escaladoX1p2) { // Rebote en la pared derecha
            actual->velocidadX *= -1;
        }
        if (actual->y - actual->radio < 150 * escaladoY1p2) { // Rebote en el techo
            actual->velocidadY *= -1;
        }
        if (actual->y + actual->radio > 880 * escaladoY1p2) { // Rebote en el piso
            actual->velocidadY *= -1;
        }

        // Colisión con la paleta (vertical)
        if (actual->x - actual->radio <= plataforma_x + plataforma_anchura / 2.0 && // Verifica el lado izquierdo de la paleta
            actual->y >= plataforma_y - plataforma_altura / 2.0 && // Dentro del rango vertical de la paleta
            actual->y <= plataforma_y + plataforma_altura / 2.0) {
            actual->velocidadX *= -1; // Rebota en la paleta
            actual->x = plataforma_x + plataforma_anchura / 2.0 + actual->radio; // Ajusta la posición para evitar solapamiento
        }

        // Si la bola cae fuera de los límites de la pantalla
        if (actual->x - actual->radio < 0) { // Si la bola sale de la pantalla por la izquierda
            Bolap2* Aux = actual;
            if (anterior) {
                anterior->siguiente = actual->siguiente;
            }
            else {
                listabolap2 = actual->siguiente; // Actualiza la cabeza si se elimina el primer nodo
            }
            actual = actual->siguiente;
            delete Aux; // Libera memoria
        }
        else {
            anterior = actual;
            actual = actual->siguiente; // Avanza al siguiente nodo
        }
    }

    // Verifica si la lista está vacía
    if (listabolap2 == nullptr) {
        juegoPerdidop2 = true;
    }
}

void paletap3(ALLEGRO_EVENT_QUEUE*& coladeeventos, ALLEGRO_EVENT& evento, ALLEGRO_FONT* fuente) {
    ALLEGRO_BITMAP* imagenPlataforma = al_load_bitmap("sprites/nave/normalpl1.png");
    al_draw_scaled_bitmap(imagenPlataforma, 0, 0, al_get_bitmap_width(imagenPlataforma), al_get_bitmap_height(imagenPlataforma), plataforma_xp3 - plataforma_anchurap3 / 2.0,
        plataforma_yp3 - plataforma_alturap3 / 2.0, plataforma_anchurap3, plataforma_alturap3, 0);

    if (!bolaLanzadap3 && (!(nivel3))) {
        al_draw_filled_circle(plataforma_xp3 - 10, plataforma_yp3, 13.0f, al_map_rgb(0, 0, 0));
        al_draw_filled_circle(plataforma_xp3 - 10, plataforma_yp3, 10.0f, al_map_rgb(0, 0, 255)); // Color rojo para la bola
    }
    if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (evento.keyboard.keycode) {
        case ALLEGRO_KEY_I:
            plataforma_yp3 -= velocidadp2;
            break;
        case ALLEGRO_KEY_K:
            plataforma_yp3 += velocidadp2;
            break;
        case ALLEGRO_KEY_ENTER:
            lanzarBolaDesdePlataformap3(plataforma_xp3 - 10, plataforma_yp3, plataforma_alturap3);
            break;
        }
    }
    if (plataforma_yp3 - plataforma_alturap3 / 2.0 < (110 + 30) * escaladoX) {
        plataforma_yp3 += velocidadp2;
    }
    else if (plataforma_yp3 - plataforma_alturap3 / 2.0 > (810) * escaladoX) {
        plataforma_yp3 -= velocidadp2;
    }
    if (bolaLanzadap3 == true) {
        actualizarBolasp3(plataforma_xp3, plataforma_yp3, plataforma_anchurap3, plataforma_alturap3);
    }
    dibujarBolasp3();
}

void paletap2(ALLEGRO_EVENT_QUEUE*& coladeeventos, ALLEGRO_EVENT& evento, ALLEGRO_FONT* fuente) {
    ALLEGRO_BITMAP* imagenPlataforma = al_load_bitmap("sprites/nave/normalpl2.png");
    al_draw_scaled_bitmap(imagenPlataforma, 0, 0, al_get_bitmap_width(imagenPlataforma), al_get_bitmap_height(imagenPlataforma), plataforma_xp2 - plataforma_anchurap2 / 2.0,
        plataforma_yp2 - plataforma_alturap2 / 2.0, plataforma_anchurap2, plataforma_alturap2, 0);

    if (!bolaLanzadap2 && (!(nivel3))) {
        al_draw_filled_circle(plataforma_xp2 + 10, plataforma_yp2, 13.0f, al_map_rgb(0, 0, 0));
        al_draw_filled_circle(plataforma_xp2 + 10, plataforma_yp2, 10.0f, al_map_rgb(255, 0, 0)); // Color rojo para la bola
    }
    if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (evento.keyboard.keycode) {
        case ALLEGRO_KEY_W:
            plataforma_yp2 -= velocidadp2;
            break;
        case ALLEGRO_KEY_S:
            plataforma_yp2 += velocidadp2;
            break;
        case ALLEGRO_KEY_SPACE:
            lanzarBolaDesdePlataformap2(plataforma_xp2 + 10, plataforma_yp2, plataforma_alturap2);
            break;
        }
    }
    if (plataforma_yp2 - plataforma_alturap2 / 2.0 < (110 + 20) * escaladoX) {
        plataforma_yp2 += velocidadp2;
    }
    else if (plataforma_yp2 - plataforma_alturap2 / 2.0 > (810) * escaladoX) {
        plataforma_yp2 -= velocidadp2;
    }
    if (bolaLanzadap2 == true) {
        actualizarBolasp2(plataforma_xp2, plataforma_yp2, plataforma_anchurap2, plataforma_alturap2);
    }
    dibujarBolasp2();
}

void vidasfull() {
    juegoPerdidop2 = false;
    juegoPerdidop3 = false;
    juegoperdidoseriop1 = false;
    vidajugador = 5;
    juegoperdidoseriop2 = false;
    vidajugador2 = 5;
}

void iniciarduojugadores(ALLEGRO_FONT* fuente, ALLEGRO_FONT* fuentesub, ALLEGRO_TIMER* temporizador_bola, ALLEGRO_EVENT_QUEUE* coladeeventos, ALLEGRO_EVENT& evento) {
    ALLEGRO_BITMAP* imagen = al_load_bitmap("sprites/backgrounds/4.png");
    ALLEGRO_BITMAP* imagen2 = al_load_bitmap("sprites/backgrounds/20.png");
    al_draw_scaled_bitmap(imagen, 0, 0, al_get_bitmap_width(imagen), al_get_bitmap_height(imagen), 0 * escaladoX, 110 * escaladoY, 950 * escaladoX,
        810 * escaladoY, 0);
    al_draw_scaled_bitmap(imagen2, 0, 0, al_get_bitmap_width(imagen2), al_get_bitmap_height(imagen2), 970 * escaladoX, 110 * escaladoY, 970 * escaladoX,
        810 * escaladoY, 0);

    if (juegoPerdidop2 && vidajugador > 1) {
        vidajugador -= 1;
        bolaLanzadap2 = false;
        juegoPerdidop2 = false;
    }
    else if(juegoPerdidop2 && (!(vidajugador > 1))){
        juegoperdidoseriop1 = true;
    }
    if (juegoPerdidop3 && vidajugador2 > 1) {
        vidajugador2 -= 1;
        bolaLanzadap3 = false;
        juegoPerdidop3 = false;
    }
    else if (juegoPerdidop3 && (!(vidajugador2 > 1))) {
        juegoperdidoseriop2 = true;
    }

    al_draw_text(fuente, al_map_rgb(0, 0, 0), 925 * escaladoX, 503 * escaladoY, 0, "VS");
    al_draw_text(fuente, color_actual, 923 * escaladoX, 500 * escaladoY, 0, "VS");
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 925 * escaladoX, 500 * escaladoY, 0, "VS");
    al_draw_text(fuentesub, al_map_rgb(255, 255, 255), 25 * escaladoX, 30 * escaladoY, 0, "VIDAS RESTANTES P1: ");
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 625 * escaladoX, 30 * escaladoY, 0, to_string(vidajugador).c_str());
    al_draw_text(fuentesub, al_map_rgb(255, 255, 255), 980 * escaladoX, 30 * escaladoY, 0, "VIDAS RESTANTES P2: ");
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 1580 * escaladoX, 30 * escaladoY, 0, to_string(vidajugador2).c_str());
    al_draw_text(fuentesub, al_map_rgb(255, 255, 255), 025 * escaladoX, 950 * escaladoY, 0, "P1: Muevete W,S, SPACE para Lanzar la Bola");
    al_draw_text(fuentesub, al_map_rgb(255, 255, 255), 980 * escaladoX, 950 * escaladoY, 0, "P2: Muevete I,K, ENTER para Lanzar la Bola");
    paletap2(coladeeventos, evento, fuente);
    paletap3(coladeeventos, evento, fuente);
}