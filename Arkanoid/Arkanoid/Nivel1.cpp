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
#include <windows.h>
const int ancho = GetSystemMetrics(SM_CXSCREEN);
const int altura = GetSystemMetrics(SM_CYSCREEN);
float escaladoX2 = devolverx();
float escaladoY2 = devolvery();
using namespace std;
void iniciarnivel1(ALLEGRO_FONT* fuente, int& puntos, ALLEGRO_TIMER* temporizador_bola, ALLEGRO_EVENT_QUEUE* coladeeventos, ALLEGRO_EVENT& evento) {

    // Dibuja el espacio del juego en sí
    al_draw_filled_rectangle(430 * escaladoX2, 40 * escaladoY2, 1310 * escaladoX2, 1000 * escaladoY2, al_map_rgb(145, 145, 145));
    al_draw_filled_rectangle(440 * escaladoX2, 50 * escaladoY2, 1300 * escaladoX2, 1000 * escaladoY2, al_map_rgb(0, 0, 155));

    // Mostrar los puntos
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 1500 * escaladoX2, 300 * escaladoY2, 0, "Puntos");
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 1500 * escaladoX2, 350 * escaladoY2, 0, to_string(puntos).c_str());

    // Procesar los eventos de la paleta

    paleta(coladeeventos, evento);

}