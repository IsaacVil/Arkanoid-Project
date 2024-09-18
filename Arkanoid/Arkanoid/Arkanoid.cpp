#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>   
#include <allegro5/allegro_acodec.h> 

#include "Nivel1.h"
#include "Assets.h"

#include <string>
#include <iostream>
using namespace std;

const float FPS = 60.0;
const int ancho = 1920;
const int altura = 1000;

int main() {

    if (!al_init()) {
        cerr << "Error al inicializar Allegro." << endl;
        return -1;
    }

    al_init_primitives_addon();

    al_init_font_addon();

    al_init_ttf_addon();

    al_install_keyboard();

    ALLEGRO_DISPLAY* pantalla = al_create_display(ancho, altura);

    ALLEGRO_FONT* fuente = al_load_ttf_font("font/ARCADE_I.Ttf", 40, 0);
    if (!fuente) {
        cerr << "Error al cargar la fuente." << endl;
        al_destroy_display(pantalla);
        return -1;
    }

    ALLEGRO_FONT* fuentetitulo = al_load_ttf_font("font/ARCADE_I.Ttf", 120, 0);
    ALLEGRO_FONT* fuentesubtitulo = al_load_ttf_font("font/ARCADE_I.Ttf", 20, 0);

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_TIMER* temporizador_bola = al_create_timer(5.0);

    ALLEGRO_EVENT_QUEUE* coladeevento = al_create_event_queue();
    if (!coladeevento) {
        cerr << "Error al crear la cola de eventos." << endl;
        al_destroy_timer(timer);
        al_destroy_font(fuente);
        al_destroy_display(pantalla);
        return -1;
    }

    al_register_event_source(coladeevento, al_get_display_event_source(pantalla));
    al_register_event_source(coladeevento, al_get_timer_event_source(timer));
    al_register_event_source(coladeevento, al_get_timer_event_source(temporizador_bola));
    al_register_event_source(coladeevento, al_get_keyboard_event_source());

    al_start_timer(timer);
    al_start_timer(temporizador_bola);

    bool salida = false;
    int puntos = 0;
    bool menu = true;
    bool niveles = false;
    bool nivel1 = false;
    bool top5 = false;
    float tiempo = 0.0;
    int opcion = 0;
    bool musicadetenida = false;

    al_install_audio();
    al_init_acodec_addon();
    if (!al_reserve_samples(1)) {
        cerr << "Error al reservar muestras de audio." << endl;
        return -1;
    }
    ALLEGRO_SAMPLE* musicamenu = al_load_sample("music/menu.ogg");
    ALLEGRO_SAMPLE* musicaniveles = al_load_sample("music/menunivel.ogg");
    ALLEGRO_SAMPLE* musicanivel1 = al_load_sample("music/nivel1.ogg");
    ALLEGRO_SAMPLE_ID id_musica;
    al_play_sample(musicamenu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id_musica);

    while (!salida) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(coladeevento, &evento);
        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            salida = true;
        }

        al_clear_to_color(al_map_rgb(0, 0, 0));

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (menu || niveles) {
            if (!musicadetenida) {
                al_stop_sample(&id_musica);
                al_play_sample(musicaniveles, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id_musica);
                musicadetenida = true;
            }
        }
        else {
            if (musicadetenida) {
                al_stop_sample(&id_musica);
                al_play_sample(musicanivel1, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id_musica);
                musicadetenida = false;
            }
        }
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        if (menu) {
            tiempo += 1.0 / FPS;
            ALLEGRO_COLOR color_actual = colorarcoiris(tiempo);
            al_draw_text(fuentetitulo, color_actual, 472, 250, 0, "ARKANOID");
            al_draw_text(fuentetitulo, color_actual, 475, 250, 0, "ARKANOID");
            al_draw_text(fuentetitulo, al_map_rgb(255, 255, 255), 477, 250, 0, "ARKANOID");

            if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch (evento.keyboard.keycode) {
                case ALLEGRO_KEY_ENTER:
                    if (opcion == 0) {
                        menu = false;
                        niveles = true;
                        opcion = 0;
                    }
                    else if (opcion == 1) {
                        menu = false;
                        niveles = true;
                        opcion = 0;
                    }
                    else if (opcion == 2) {
                        menu = false;
                        top5 = true;
                        opcion = 0;
                    }
                case ALLEGRO_KEY_DOWN:
                case ALLEGRO_KEY_S:
                    opcion = opcion + 1;
                    if (opcion % 3 == 0) {
                        opcion = 0;
                    }
                    break;

                case ALLEGRO_KEY_UP:
                case ALLEGRO_KEY_W:
                    opcion = opcion - 1;
                    if (opcion < 0) {
                        opcion = 2;
                    }
                }
            }

            if (opcion == 0) {
                al_draw_text(fuente, color_actual, 717, 500, 0, "1 JUGADOR");
                al_draw_text(fuente, color_actual, 718, 500, 0, "1 JUGADOR");
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 720, 500, 0, "1 JUGADOR");
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 720, 600, 0, "2 JUGADORES");
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 720, 700, 0, "TOP 5");
            }
            else if (opcion == 1) {
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 720, 500, 0, "1 JUGADOR");
                al_draw_text(fuente, color_actual, 717, 600, 0, "2 JUGADORES");
                al_draw_text(fuente, color_actual, 718, 600, 0, "2 JUGADORES");
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 720, 600, 0, "2 JUGADORES");
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 720, 700, 0, "TOP 5");
            }
            else if (opcion == 2) {
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 720, 500, 0, "1 JUGADOR");
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 720, 600, 0, "2 JUGADORES");
                al_draw_text(fuente, color_actual, 717, 700, 0, "TOP 5");
                al_draw_text(fuente, color_actual, 718, 700, 0, "TOP 5");
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 720, 700, 0, "TOP 5");
            }

            al_draw_text(fuentesubtitulo, al_map_rgb(255, 255, 255), 475, 900, 0, "Creado Por Isaac Villalobos y Kevin Vega. 2024");
        }
        else if (niveles) {
            tiempo += 1.0 / FPS;
            ALLEGRO_COLOR color_actual = colorarcoiris(tiempo);
            al_draw_text(fuentetitulo, color_actual, 555, 200, 0, "NIVELES");
            al_draw_text(fuentetitulo, al_map_rgb(255, 255, 255), 560, 200, 0, "NIVELES");

            if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch (evento.keyboard.keycode) {
                case ALLEGRO_KEY_ENTER:
                    if (opcion == 0) {
                        menu = false;
                        niveles = false;
                        nivel1 = true;
                        opcion = 0;
                    }
                    else if (opcion == 1) {
                        menu = true;
                        niveles = false;
                        opcion = 0;
                    }
                    else if (opcion == 2) {
                        menu = true;
                        niveles = false;
                        opcion = 0;
                    }
                    break;

                case ALLEGRO_KEY_RIGHT:
                case ALLEGRO_KEY_D:
                    opcion = opcion + 1;
                    if (opcion % 3 == 0) {
                        opcion = 0;
                    }
                    break;

                case ALLEGRO_KEY_LEFT:
                case ALLEGRO_KEY_A:
                    opcion = opcion - 1;
                    if (opcion < 0) {
                        opcion = 2;
                    }
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    menu = true;
                    niveles = false;
                    opcion = 0;
                }
            }

            if (opcion == 0) {
                al_draw_filled_rectangle(400, 400, 700, 600, al_map_rgb(0, 205, 0));
                al_draw_filled_rectangle(430, 400, 700, 570, al_map_rgb(0, 255, 0));
                al_draw_filled_rectangle(800, 400, 1100, 600, al_map_rgb(0, 0, 255));
                al_draw_filled_rectangle(1200, 400, 1500, 600, al_map_rgb(255, 0, 0));
            }
            else if (opcion == 1) {
                al_draw_filled_rectangle(400, 400, 700, 600, al_map_rgb(0, 255, 0));
                al_draw_filled_rectangle(800, 400, 1100, 600, al_map_rgb(0, 0, 205));
                al_draw_filled_rectangle(830, 400, 1100, 570, al_map_rgb(0, 0, 255));
                al_draw_filled_rectangle(1200, 400, 1500, 600, al_map_rgb(255, 0, 0));
            }
            else if (opcion == 2) {
                al_draw_filled_rectangle(400, 400, 700, 600, al_map_rgb(0, 255, 0));
                al_draw_filled_rectangle(800, 400, 1100, 600, al_map_rgb(0, 0, 255));
                al_draw_filled_rectangle(1200, 400, 1500, 600, al_map_rgb(205, 0, 0));
                al_draw_filled_rectangle(1230, 400, 1500, 570, al_map_rgb(255, 0, 0));
            }
        }
        else if (nivel1) {
            iniciarnivel1(fuente, puntos, temporizador_bola, coladeevento, evento);
        }
        al_flip_display();
    }

    al_destroy_font(fuente);
    al_destroy_timer(timer);
    al_destroy_event_queue(coladeevento);
    al_destroy_display(pantalla);
}
