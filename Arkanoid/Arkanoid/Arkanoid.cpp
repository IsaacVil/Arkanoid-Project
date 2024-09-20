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
const float FPS = 60.0;
const int ancho = 1920;
const int altura = 1000;

int main() {


    al_init();

    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_install_keyboard();
    ALLEGRO_DISPLAY* pantalla = al_create_display(ancho, altura);

    ALLEGRO_BITMAP* icono = al_load_bitmap("logo.png");
    ALLEGRO_BITMAP* star = al_load_bitmap("sprites/star.png");
    ALLEGRO_BITMAP* iceworld = al_load_bitmap("sprites/iceworld.png");
    ALLEGRO_BITMAP* galaxy = al_load_bitmap("sprites/galaxy.png");
    ALLEGRO_BITMAP* blackhole = al_load_bitmap("sprites/blackhole.png");
    ALLEGRO_BITMAP* world = al_load_bitmap("sprites/world.png");
    ALLEGRO_BITMAP* world2 = al_load_bitmap("sprites/world2.png");
    ALLEGRO_BITMAP* mundo1 = al_load_bitmap("sprites/nivel1.png");
    ALLEGRO_BITMAP* mundo1sel = al_load_bitmap("sprites/nivel1select.png");
    ALLEGRO_BITMAP* mundo2 = al_load_bitmap("sprites/nivel2.png");
    ALLEGRO_BITMAP* mundo2sel = al_load_bitmap("sprites/nivel2select.png");
    ALLEGRO_BITMAP* mundo3 = al_load_bitmap("sprites/nivel3.png");
    ALLEGRO_BITMAP* mundo3sel = al_load_bitmap("sprites/nivel3select.png");
    ALLEGRO_BITMAP* background1 = al_load_bitmap("sprites/background.png");

    al_set_display_icon(pantalla, icono);
    al_destroy_bitmap(icono);

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
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(1);

    ALLEGRO_SAMPLE* musicamenu = al_load_sample("music/menu.ogg");
    ALLEGRO_SAMPLE* musicaniveles = al_load_sample("music/menunivel.ogg");
    ALLEGRO_SAMPLE* musicanivel1 = al_load_sample("music/nivel1.ogg");
    ALLEGRO_SAMPLE_ID id_musica;
    al_play_sample(musicamenu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id_musica);

    bool salida = false;
    int puntos = 0;
    bool menu = true;
    bool niveles = false;
    bool nivel1 = false;
    bool top5 = false;
    float tiempo = 0.0;
    int opcion = 0;
    bool musicadetenida = false;
    int frame = 0;
    float tiempo_frame = 0.0;
    const float FRAME_DURATION = 0.1;

    while (!salida) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(coladeevento, &evento);
        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            salida = true;
        }

        al_clear_to_color(al_map_rgb(0, 0, 0));

        if (evento.type == ALLEGRO_EVENT_TIMER) {
            tiempo_frame += 1 / FPS;
            if (tiempo_frame >= FRAME_DURATION) {
                tiempo_frame = 0.0;
                frame++;
                if (frame >= 60) {
                    frame = 0; // Repetir la animación
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (menu || niveles) {
            al_draw_tinted_bitmap(background1, al_map_rgba_f(150, 120, 150, 1.0f), 0, 0, 0);
            int frames = frame * 2;
            if (frames == 20 || frames == 21 || frames == 22 || frames == 23 || frames == 24 || frames == 25 || frames == 26 || frames == 27) {
                al_draw_tinted_bitmap(background1, al_map_rgba_f(125, 75, 125, 1.0f), 0, 0, 0);
            }
            else if (frames == 28 || frames == 29 || frames == 30 || frames == 31 || frames == 32 || frames == 33 || frames == 34 || frames == 35) {
                al_draw_tinted_bitmap(background1, al_map_rgba_f(100, 50, 100, 1.0f), 0, 0, 0);
            }
            else if (frames == 36 || frames == 37 || frames == 38 || frames == 39 || frames == 40 || frames == 41 || frames == 42 || frames == 43) {
                al_draw_tinted_bitmap(background1, al_map_rgba_f(55, 30, 55, 1.0f), 0, 0, 0);
            }
            else if (frames == 44 || frames == 45 || frames == 46 || frames == 47 || frames == 48 || frames == 49 || frames == 50 || frames == 51) {
                al_draw_tinted_bitmap(background1, al_map_rgba_f(100, 75, 100, 1.0f), 0, 0, 0);
            }
            else if (frames == 52 || frames == 53 || frames == 54 || frames == 55 || frames == 56 || frames == 57 || frames == 58 || frames == 59 || frames == 60) {
                al_draw_tinted_bitmap(background1, al_map_rgba_f(120, 95, 120, 1.0f), 0, 0, 0);
            }

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
            int frames_por_fila = al_get_bitmap_width(star) / 120;
            int frameX = (frame % frames_por_fila) * 260;
            int frameY = (frame / frames_por_fila) * 260;
            al_draw_bitmap_region(star, frameX, frameY, 260, 260, 50, 100, 0);
            frames_por_fila = al_get_bitmap_width(world) / 120;
            frameX = (frame % frames_por_fila) * 120;
            frameY = (frame / frames_por_fila) * 120;
            al_draw_bitmap_region(world, frameX, frameY, 120, 120, 750, 60, 0);
            frames_por_fila = al_get_bitmap_width(world2) / 120;
            frameX = (frame % frames_por_fila) * 120;
            frameY = (frame / frames_por_fila) * 120;
            al_draw_bitmap_region(world2, frameX, frameY, 120, 120, 1300, 560, 0);
            frames_por_fila = al_get_bitmap_width(iceworld) / 110;
            frameX = (frame % frames_por_fila) * 110;
            frameY = (frame / frames_por_fila) * 110;
            al_draw_bitmap_region(iceworld, frameX, frameY, 110, 110, 1600, 100, 0);
            frames_por_fila = al_get_bitmap_width(galaxy) / 200;
            frameX = (frame % frames_por_fila) * 200;
            frameY = (frame / frames_por_fila) * 200;
            al_draw_bitmap_region(galaxy, frameX, frameY, 200, 200, 1600, 700, 0);
            frames_por_fila = al_get_bitmap_width(blackhole) / 260;
            frameX = (frame % frames_por_fila) * 260;
            frameY = (frame / frames_por_fila) * 260;
            al_draw_bitmap_region(blackhole, frameX, frameY, 260, 260, 95, 670, 0);
        }

        if (menu) ////////////////////////////////////////////MENU////////////////////////////////////////////
        {
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
                    if (opcion == 0 || opcion == 1 || opcion == 2) {
                        menu = false;
                        niveles = false;
                        nivel1 = true;
                        opcion = 0;
                    }
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    menu = true;
                    niveles = false;
                    opcion = 0;
                    break;
                case ALLEGRO_KEY_RIGHT:
                case ALLEGRO_KEY_D:
                    opcion = (opcion + 1) % 3;
                    break;

                case ALLEGRO_KEY_LEFT:
                case ALLEGRO_KEY_A:
                    opcion = (opcion - 1 + 3) % 3;
                    break;
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
        else if (nivel1) ////////////////////////////////////////////NIVEL 1////////////////////////////////////////////
        {
            iniciarnivel1(fuente, puntos, temporizador_bola, coladeevento, evento);
        }
        al_flip_display();
    }

    al_destroy_font(fuente);
    al_destroy_timer(timer);
    al_destroy_event_queue(coladeevento);
    al_destroy_display(pantalla);
    al_destroy_bitmap(star);
    al_destroy_bitmap(blackhole);
    al_destroy_bitmap(world);
    al_destroy_bitmap(world2);
    al_destroy_bitmap(iceworld);
    al_destroy_bitmap(galaxy);
    al_destroy_bitmap(background1);
}