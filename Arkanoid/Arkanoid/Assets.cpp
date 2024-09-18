#include "assets.h"
#include <cstdlib>
#include "Nivel1.h"
#include <allegro5/allegro_primitives.h>
#include <string>
#include <vector>

using namespace std;

const int altura = 950;
const int anchurainicio = 440;
const int anchurafinal = 1300;
const int anchura = 860;

float plataforma_x = (anchurainicio + anchurafinal) / 2.0;
float plataforma_y = altura - 20; // Posiciona la plataforma en la parte inferior de la ventana
float plataforma_anchura = 180; // Ancho de la plataforma
float plataforma_altura = 20;   // Alto de la plataforma
float velocidad = 50;          // Velocidad de movimiento

ALLEGRO_COLOR colorarcoiris(float tiempo) {
    float valor = fmod(tiempo, 1.0f);
    float r = fabs(sin(valor * 2.0f * 3.14159f));
    float g = fabs(sin((valor + 0.333f) * 2.0f * 3.14159f));
    float b = fabs(sin((valor + 0.666f) * 2.0f * 3.14159f));
    return al_map_rgb_f(r, g, b);
}
ALLEGRO_COLOR colorplataforma = al_map_rgb(0, 255, 0);
ALLEGRO_COLOR colorplataforma2 = al_map_rgb(155, 255, 155);

void paleta(ALLEGRO_EVENT_QUEUE*& coladeeventos, ALLEGRO_EVENT& evento) {

    al_draw_filled_rectangle(plataforma_x - plataforma_anchura / 2.0, plataforma_y - plataforma_altura / 2.0, plataforma_x + plataforma_anchura / 2.0, plataforma_y + plataforma_altura / 2.0, colorplataforma);
    al_draw_filled_rectangle(plataforma_x - plataforma_anchura / 2.0 + 15, plataforma_y - plataforma_altura / 2.0 - 5, plataforma_x - plataforma_anchura / 2.0 + 30, plataforma_y + plataforma_altura / 2.0 + 5, colorplataforma2);
    al_draw_filled_rectangle(plataforma_x + plataforma_anchura / 2.0 - 15, plataforma_y - plataforma_altura / 2.0 - 5, plataforma_x + plataforma_anchura / 2.0 - 30, plataforma_y + plataforma_altura / 2.0 + 5, colorplataforma2);

    if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (evento.keyboard.keycode) {
        case ALLEGRO_KEY_A:
            plataforma_x -= velocidad;
            break;
        case ALLEGRO_KEY_D:
            plataforma_x += velocidad;
            break;
        }
    }

    // Ajusta la posición para que se mantenga dentro del dominio [anchurainicio, anchurafinal]
    if (plataforma_x - plataforma_anchura / 2.0 < anchurainicio) {
        plataforma_x = anchurainicio + plataforma_anchura / 2.0;
    }
    else if (plataforma_x + plataforma_anchura / 2.0 > anchurafinal) {
        plataforma_x = anchurafinal - plataforma_anchura / 2.0;
    }
}