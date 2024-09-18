#include "Nivel1.h"
#include "Assets.h"
#include <allegro5/allegro_primitives.h>
#include <string>
#include <vector>

using namespace std;

const int anchurabola = 25;
vector<Bola> bolas;
float velocidad_bola = 200.0;
int max_bolas = 5;

void iniciarnivel1(ALLEGRO_FONT* fuente, int& puntos, ALLEGRO_TIMER* temporizador_bola, ALLEGRO_EVENT_QUEUE* coladeeventos, ALLEGRO_EVENT& evento) {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_filled_rectangle(430, 40, 1310, 1000, al_map_rgb(145, 145, 145));
    al_draw_filled_rectangle(440, 50, 1300, 1000, al_map_rgb(0, 0, 155)); // Dibuja el espacio del juego en sí

    al_draw_text(fuente, al_map_rgb(255, 255, 255), 1500, 300, 0, "Puntos");
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 1500, 350, 0, to_string(puntos).c_str());

    paleta(coladeeventos, evento);
}