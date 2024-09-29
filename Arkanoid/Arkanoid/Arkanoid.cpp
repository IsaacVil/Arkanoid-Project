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
using namespace std;
const float FPS = 60.0;
const int ancho = GetSystemMetrics(SM_CXSCREEN);
const int altura = GetSystemMetrics(SM_CYSCREEN);
const int BASE_WIDTH = 1920;
const int BASE_HEIGHT = 1000;
float escaladoX = (float)ancho / BASE_WIDTH;
float escaladoY = (float)altura / BASE_HEIGHT;
bool salida = false;
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
bool activoespera = false;
bool perdio = false;
bool musicajuego = false;
int puntos = 0;
ALLEGRO_FONT* fuente = nullptr;
ALLEGRO_TIMER* temporizador_bola = nullptr;
ALLEGRO_TIMER* temporizadorcompuerta = nullptr;
float devolverx() {
    return escaladoX;
}
float devolvery() {
    return escaladoY;
}
int main() {


    al_init();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_install_keyboard();
    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
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

    fuente = al_load_ttf_font("font/ARCADE_I.Ttf", 40 * escaladoX, 0);
    if (!fuente) {
        cerr << "Error al cargar la fuente." << endl;
        al_destroy_display(pantalla);
        return -1;
    }

    ALLEGRO_FONT* fuentetitulo = al_load_ttf_font("font/ARCADE_I.Ttf", 120 * escaladoX, 0);
    ALLEGRO_FONT* fuentesubtitulo = al_load_ttf_font("font/ARCADE_I.Ttf", 20 * escaladoX, 0);

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    temporizador_bola = al_create_timer(5.0);

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
    ALLEGRO_SAMPLE* vidaperdidamusica = al_load_sample("music/vidaperdida.ogg");
    ALLEGRO_SAMPLE_ID id_musica;
    al_play_sample(musicamenu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id_musica);

    while (!salida) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(coladeevento, &evento);
        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            salida = true;
        }

        al_set_target_bitmap(al_get_backbuffer(pantalla));

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

        al_draw_tinted_bitmap(background1, al_map_rgba_f(150, 120, 150, 1.0f), 0, 0, 0);
        int frames = frame * 2;
        if (frames == 20 || frames == 21 || frames == 22 || frames == 23 || frames == 24 || frames == 25 || frames == 26 || frames == 27) {
            al_draw_tinted_bitmap(background1, al_map_rgba_f(125, 75, 125, 1.0f), 0, 0, 0);
        }
        else if (frames == 28 || frames == 29 || frames == 30 || frames == 31 || frames == 32 || frames == 33 || frames == 34 || frames == 35) {
            al_draw_tinted_bitmap(background1, al_map_rgba_f(100, 50, 100, 1.0f), 0, 0, 0);
        }
        else if (frames == 36 || frames == 37 || frames == 38 || frames == 39 || frames == 40) {
            al_draw_tinted_bitmap(background1, al_map_rgba_f(55, 30, 55, 1.0f), 0, 0, 0);
        }
        else if (frames == 41 || frames == 42 || frames == 43 || frames == 44 || frames == 45 || frames == 46 || frames == 47 || frames == 48 || frames == 49 || frames == 50 || frames == 51) {
            al_draw_tinted_bitmap(background1, al_map_rgba_f(100, 75, 100, 1.0f), 0, 0, 0);
        }
        else if (frames == 52 || frames == 53 || frames == 54 || frames == 55 || frames == 56 || frames == 57 || frames == 58 || frames == 59 || frames == 60) {
            al_draw_tinted_bitmap(background1, al_map_rgba_f(120, 95, 120, 1.0f), 0, 0, 0);
        }

        if (menu || niveles) {
            if (!musicadetenida || musicajuego) {
                al_stop_sample(&id_musica);
                al_play_sample(musicaniveles, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id_musica);
                musicadetenida = true;
                musicajuego = false;
            }
        }
        else if (juegoPerdido) {
            if (!musicadetenida) {
                al_stop_sample(&id_musica);
                al_play_sample(vidaperdidamusica, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id_musica);
                musicadetenida = true;
                musicajuego = true;
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
        

        if (menu) ////////////////////////////////////////////MENU////////////////////////////////////////////
        {
            juegoPerdido = false;
            vidas = 3;
            activoespera = true;
            tiempo += 1.0 / FPS;
            ALLEGRO_COLOR color_actual = colorarcoiris(tiempo);
            al_draw_text(fuentetitulo, color_actual, 472 * escaladoX, 250 * escaladoY, 0, "ARKANOID");
            al_draw_text(fuentetitulo, color_actual, 475 * escaladoX, 250 * escaladoY, 0, "ARKANOID");
            al_draw_text(fuentetitulo, al_map_rgb(255, 255, 255), 477 * escaladoX, 250 * escaladoY, 0, "ARKANOID");

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
                al_draw_text(fuente, color_actual, 717 * escaladoX, 500 * escaladoY, 0, "1 JUGADOR");
                al_draw_text(fuente, color_actual, 718 * escaladoX, 500 * escaladoY, 0, "1 JUGADOR");
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 720 * escaladoX, 500 * escaladoY, 0, "1 JUGADOR");
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 720 * escaladoX, 600 * escaladoY, 0, "2 JUGADORES");
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 720 * escaladoX, 700 * escaladoY, 0, "TOP 5");
            }
            else if (opcion == 1) {
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 720 * escaladoX, 500 * escaladoY, 0, "1 JUGADOR");
                al_draw_text(fuente, color_actual, 717 * escaladoX, 600 * escaladoY, 0, "2 JUGADORES");
                al_draw_text(fuente, color_actual, 718 * escaladoX, 600 * escaladoY, 0, "2 JUGADORES");
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 720 * escaladoX, 600 * escaladoY, 0, "2 JUGADORES");
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 720 * escaladoX, 700 * escaladoY, 0, "TOP 5");
            }
            else if (opcion == 2) {
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 720 * escaladoX, 500 * escaladoY, 0, "1 JUGADOR");
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 720 * escaladoX, 600 * escaladoY, 0, "2 JUGADORES");
                al_draw_text(fuente, color_actual, 717 * escaladoX, 700 * escaladoY, 0, "TOP 5");
                al_draw_text(fuente, color_actual, 718 * escaladoX, 700 * escaladoY, 0, "TOP 5");
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 720 * escaladoX, 700 * escaladoY, 0, "TOP 5");
            }

            int frames_por_fila = al_get_bitmap_width(star) / 120;
            int frameX = (frame % frames_por_fila) * 260;
            int frameY = (frame / frames_por_fila) * 260;
            al_draw_bitmap_region(star, frameX, frameY, 260, 260, 50 * escaladoX, 100 * escaladoY, 0);
            frames_por_fila = al_get_bitmap_width(world) / 120;
            frameX = (frame % frames_por_fila) * 120;
            frameY = (frame / frames_por_fila) * 120;
            al_draw_bitmap_region(world, frameX, frameY, 120, 120, 750 * escaladoX, 60 * escaladoY, 0);
            frames_por_fila = al_get_bitmap_width(world2) / 120;
            frameX = (frame % frames_por_fila) * 120;
            frameY = (frame / frames_por_fila) * 120;
            al_draw_bitmap_region(world2, frameX, frameY, 120, 120, 1300 * escaladoX, 560 * escaladoY, 0);
            frames_por_fila = al_get_bitmap_width(iceworld) / 110;
            frameX = (frame % frames_por_fila) * 110;
            frameY = (frame / frames_por_fila) * 110;
            al_draw_bitmap_region(iceworld, frameX, frameY, 110, 110, 1600 * escaladoX, 100 * escaladoY, 0);
            frames_por_fila = al_get_bitmap_width(galaxy) / 200;
            frameX = (frame % frames_por_fila) * 200;
            frameY = (frame / frames_por_fila) * 200;
            al_draw_bitmap_region(galaxy, frameX, frameY, 200, 200, 1600 * escaladoX, 700 * escaladoY, 0);
            frames_por_fila = al_get_bitmap_width(blackhole) / 260;
            frameX = (frame % frames_por_fila) * 260;
            frameY = (frame / frames_por_fila) * 260;
            al_draw_bitmap_region(blackhole, frameX, frameY, 260, 260, 95 * escaladoX, 670 * escaladoY, 0);

            al_draw_text(fuentesubtitulo, al_map_rgb(255, 255, 255), 475 * escaladoX, 900 * escaladoY, 0, "Creado Por Isaac Villalobos y Kevin Vega. 2024");
        }
        else if (niveles) {
            juegoPerdido = false;
            vidas = 3;
            if (!activoespera) {
                al_rest(0.1f);
            }
            tiempo += 1.0 / FPS;
            ALLEGRO_COLOR color_actual = colorarcoiris(tiempo);
            al_draw_text(fuentetitulo, color_actual, 555 * escaladoX, 200 * escaladoY, 0, "NIVELES");
            al_draw_text(fuentetitulo, al_map_rgb(255, 255, 255), 560 * escaladoX, 200 * escaladoY, 0, "NIVELES");

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
            int frames_por_fila = al_get_bitmap_width(mundo1sel) / 90;
            int frameX = (frame % frames_por_fila) * 90;
            int frameY = (frame / frames_por_fila) * 90;
            al_draw_scaled_bitmap(mundo1, frameX, frameY, 90, 90, 480 * escaladoX, 440 * escaladoY, 90 * 1.5, 90 * 1.5, 0);
            frames_por_fila = al_get_bitmap_width(mundo2) / 270;
            frameX = (frame % frames_por_fila) * 270;
            frameY = (frame / frames_por_fila) * 270;
            al_draw_scaled_bitmap(mundo2, frameX, frameY, 270, 270, 775 * escaladoX, 340 * escaladoY, 270 * 1.3, 270 * 1.3, 0);
            frames_por_fila = al_get_bitmap_width(mundo3) / 150;
            frameX = (frame % frames_por_fila) * 150;
            frameY = (frame / frames_por_fila) * 150;
            al_draw_scaled_bitmap(mundo3, frameX, frameY, 150, 150, 1200 * escaladoX, 383 * escaladoY, 150 * 1.8, 150 * 1.8, 0);

            if (opcion == 0) {
                /*
                frames_por_fila = al_get_bitmap_width(mundo1sel) / 90;
                frameX = (frame % frames_por_fila) * 90;
                frameY = (frame / frames_por_fila) * 90;
                al_draw_scaled_bitmap(mundo1sel, frameX, frameY, 90, 90, 480, 440, 90 * 1.5, 90 * 1.5, 0);
                */
                al_draw_filled_triangle(530 * escaladoX, 644 * escaladoY, 560 * escaladoX, 644 * escaladoY, 545 * escaladoX, 623 * escaladoY, al_map_rgb(0, 0, 205));
            }
            else if (opcion == 1) {
                /*
                frames_por_fila = al_get_bitmap_width(mundo2sel) / 270;
                frameX = (frame % frames_por_fila) * 270;
                frameY = (frame / frames_por_fila) * 270;
                al_draw_scaled_bitmap(mundo2sel, frameX, frameY, 270, 270, 775, 340, 270 * 1.3, 270 * 1.3, 0);
                */
                al_draw_filled_triangle(940 * escaladoX, 644 * escaladoY, 970 * escaladoX, 644 * escaladoY, 955 * escaladoX, 623 * escaladoY, al_map_rgb(0, 0, 205));
            }
            else if (opcion == 2) {
                /*
                /frames_por_fila = al_get_bitmap_width(mundo3sel) / 150;
                frameX = (frame % frames_por_fila) * 150;
                frameY = (frame / frames_por_fila) * 150;
                al_draw_scaled_bitmap(mundo3sel, frameX, frameY, 150, 150, 1200, 383, 150 * 1.8, 150 * 1.8, 0);
                */
                al_draw_filled_triangle(1320 * escaladoX, 644 * escaladoY, 1350 * escaladoX, 644 * escaladoY, 1335 * escaladoX, 623 * escaladoY, al_map_rgb(0, 0, 205));
            }

        }
        else if (nivel1) ////////////////////////////////////////////NIVEL 1////////////////////////////////////////////
        {
            iniciarnivel1(fuente, temporizador_bola, coladeevento, evento);
            if (!quedanvidas()) {
                nivel1 = false;
                menu = true;
            }
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