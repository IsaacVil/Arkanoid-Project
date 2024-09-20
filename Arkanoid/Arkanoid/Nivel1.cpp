#include <allegro5/allegro_primitives.h>
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
using namespace std;

const int anchurabola = 25;
vector<Bola> bolas;
float velocidad_bola = 200.0;
int max_bolas = 5;
int frames = 1;

void iniciarnivel1(ALLEGRO_FONT* fuente, int& puntos, ALLEGRO_TIMER* temporizador_bola, ALLEGRO_EVENT_QUEUE* coladeeventos, ALLEGRO_EVENT& evento) {

    // Dibuja el espacio del juego en sí
    al_draw_filled_rectangle(430, 40, 1310, 1000, al_map_rgb(145, 145, 145));
    al_draw_filled_rectangle(440, 50, 1300, 1000, al_map_rgb(0, 0, 155));

    // Mostrar los puntos
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 1500, 300, 0, "Puntos");
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 1500, 350, 0, to_string(puntos).c_str());

    // Procesar los eventos de la paleta
    paleta(coladeeventos, evento);

}