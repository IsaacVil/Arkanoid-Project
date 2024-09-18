#pragma once
#include <vector> // Incluye directamente vector aquí
#include <allegro5/allegro_primitives.h> // Asegúrate de incluir todas las cabeceras necesarias
#include <string>

// Define la estructura Bola
typedef struct Bola {
    float x;
    float y;
    float radio;
    float velocidadx;
    float velocidady;
} Bola;
ALLEGRO_COLOR colorarcoiris(float tiempo);
void paleta(ALLEGRO_EVENT_QUEUE*& coladeeventos, ALLEGRO_EVENT& evento);