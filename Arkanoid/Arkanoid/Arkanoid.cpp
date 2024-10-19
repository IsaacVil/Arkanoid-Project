#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "Nivel1.h"
#include <sapi.h>
#include "Carga y Guardado (AVL).h"
#include "Assets.h"
#include "dosjugadores.h"
#include "resource.h"
#include <string>
#include <iostream>
#include <windows.h>
#define MAX_NOMBRE 10
using namespace std;
const float FPS = 60.0;
const int ancho = GetSystemMetrics(SM_CXSCREEN);
const int altura = GetSystemMetrics(SM_CYSCREEN);
const int BASE_WIDTH = 1920;
const int BASE_HEIGHT = 1000;
float escaladoX = (float)ancho / BASE_WIDTH;
float escaladoY = (float)altura / BASE_HEIGHT;
bool nombre = true;
bool salida = false;
bool menu = false;
bool niveles = false;
bool nivel1 = false;
bool nivel2 = false;
bool nivel3 = false;
bool transicion1 = false;
bool transicion2 = false;
bool transicion3 = false;
bool personalpuntos = false;
bool mensajenombre = false;
bool nombretts = false;
bool top5 = false;
double tiempotts;
float tiempo = 0.0;
int opcion = 0;
int puntosreales = 0;
int random1;
int random2;
int random3;
int frames;
int bolastotal = 0;
int bolasre = 0;
int bolasper = 0;
int objetosgolpeados = 0;
int tiempomensajenombre;
bool musicadetenida = false;
int frame = 0;
float tiempo_frame = 0.0;
const float FRAME_DURATION = 0.1;
bool activoespera = false;
bool perdio = false;
bool musicajuego = false;
bool duojugadores = false;
int puntos = 0;
double tiempomensajetrans1 = 0;
double tiempomensajetrans2 = 0;
double tiempomensajetrans3 = 0;
double tiempomensajeduo = 0;
char nombreusuario2[MAX_NOMBRE] = "";
char nombreusuario[MAX_NOMBRE] = "_________";
int longitud_nombre = 0;
bool dibujar = true;
abbptr jugadoractual = nullptr;
ALLEGRO_FONT* fuente = nullptr;
ALLEGRO_TIMER* temporizador_bola = nullptr;
ALLEGRO_TIMER* temporizadorcompuerta = nullptr;
ALLEGRO_COLOR color_actual;
abbptr puntuaciones = nullptr;
abbptr top5avl = nullptr;
vector<abbptr> top5vector;
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
    ALLEGRO_BITMAP* totoro = al_load_bitmap("sprites/totoro.png");
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
    ALLEGRO_FONT* fuentenombre = al_load_ttf_font("font/ARCADE_I.Ttf", 55 * escaladoX, 0);

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
    ALLEGRO_SAMPLE* musicanivel2 = al_load_sample("music/nivel2.ogg");
    ALLEGRO_SAMPLE* musicanivel3 = al_load_sample("music/nivel3.ogg");
    ALLEGRO_SAMPLE* musicatop5 = al_load_sample("music/top5.ogg");
    ALLEGRO_SAMPLE* winners = al_load_sample("music/winners.ogg");
    ALLEGRO_SAMPLE* vidaperdidamusica = al_load_sample("music/vidaperdida.ogg");
    ALLEGRO_SAMPLE* victorykir = al_load_sample("music/victorykir.ogg");
    ALLEGRO_SAMPLE_ID id_musica;
    al_play_sample(musicamenu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id_musica);
    cargarDatosUsuarios(puntuaciones);

    HRESULT hr = CoInitialize(NULL);
    ISpVoice* pVoice = NULL;
    hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);

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

        random1 = rand() % 5;
        random2 = rand() % 9;
        random3 = rand() % 12;

        al_draw_tinted_bitmap(background1, al_map_rgba_f(150, 120, 150, 1.0f), 0, 0, 0);
        frames = frame * 2;
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

        if (menu || niveles || nombre) {
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
        else if (duojugadores) {
            if (musicadetenida) {
                al_stop_sample(&id_musica);
                al_play_sample(musicanivel2, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id_musica);
                musicadetenida = false;
            }
        }
        else {
            if (musicadetenida && nivel1) {
                al_stop_sample(&id_musica);
                al_play_sample(musicanivel1, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id_musica);
                musicadetenida = false;
            }
            else if (musicadetenida && nivel2) {
                al_stop_sample(&id_musica);
                al_play_sample(musicanivel2, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id_musica);
                musicadetenida = false;
            }
            else if (musicadetenida && (transicion1 || transicion2)) {
                al_stop_sample(&id_musica);
                al_play_sample(victorykir, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id_musica);
                musicadetenida = false;
            }
            else if (musicadetenida && nivel3) {
                al_stop_sample(&id_musica);
                al_play_sample(musicanivel3, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id_musica);
                musicadetenida = false;
            }
            else if (musicadetenida && transicion3) {
                al_stop_sample(&id_musica);
                al_play_sample(winners, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id_musica);
                musicadetenida = false;
            }
            else if (musicadetenida && top5) {
                al_stop_sample(&id_musica);
                al_play_sample(musicatop5, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id_musica);
                musicadetenida = false;
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        
        tiempo += 1.0 / FPS;
        if (nombre) {
            color_actual = colorarcoiris(tiempo);

            // Dibujar el título estático "ESCRIBE TU NOMBRE" en varias posiciones para dar efecto de sombra
            al_draw_text(fuentenombre, color_actual, 452 * escaladoX, 120 * escaladoY, 0, "ESCRIBE TU NOMBRE");
            al_draw_text(fuentenombre, color_actual, 455 * escaladoX, 120 * escaladoY, 0, "ESCRIBE TU NOMBRE");
            al_draw_text(fuentenombre, al_map_rgb(255, 255, 255), 457 * escaladoX, 120 * escaladoY, 0, "ESCRIBE TU NOMBRE");

            al_draw_text(fuentenombre, color_actual, 402 * escaladoX, 920 * escaladoY, 0, "ENTER PARA CONTINUAR");
            al_draw_text(fuentenombre, color_actual, 405 * escaladoX, 920 * escaladoY, 0, "ENTER PARA CONTINUAR");
            al_draw_text(fuentenombre, al_map_rgb(255, 255, 255), 407 * escaladoX, 920 * escaladoY, 0, "ENTER PARA CONTINUAR");
            // Manejar eventos
            if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                break; // Salir del bucle si se cierra la ventana
            }
            else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    if (longitud_nombre == 0) {
                        tiempomensajenombre = al_get_time(); // Inicializar el temporizador
                        mensajenombre = true;
                    }
                    else {
                        for (int i = 0; i < longitud_nombre; i++) {
                            if (nombreusuario[i] == '_') {
                                // Desplazar los caracteres hacia la izquierda
                                for (int j = i; j < longitud_nombre - 1; j++) {
                                    nombreusuario[j] = nombreusuario[j + 1];

                                }
                                longitud_nombre--; // Disminuir la longitud del nombre
                                nombreusuario[longitud_nombre] = '\0'; // Agregar el terminador nulo
                                i--; // Retroceder para verificar el nuevo carácter en esta posición
                            }
                        }
                        nombre = false;
                        menu = true;
                        string nombrestringjugador(nombreusuario2);
                        for (char& c : nombrestringjugador) {
                            c = toupper(c);  
                        }
                        jugadoractual = nodo(puntosreales, bolastotal, bolasre, bolasper, objetosgolpeados, nombrestringjugador);
                        insertaravl(puntuaciones, jugadoractual);
                        guardardatosusuarios(puntuaciones);
                    }
                }
            }
            else if (evento.type == ALLEGRO_EVENT_KEY_CHAR) {
                if (longitud_nombre < MAX_NOMBRE - 1) {
                    if (evento.keyboard.unichar >= 33 && evento.keyboard.unichar <= 126 && evento.keyboard.unichar != ' ' && evento.keyboard.unichar != '_' && evento.keyboard.unichar != ',') {
                        nombreusuario[longitud_nombre] = (char)evento.keyboard.unichar;
                        nombreusuario2[longitud_nombre] = (char)evento.keyboard.unichar;
                        longitud_nombre++;
                    }
                }
                // Manejar el retroceso (backspace)
                if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && longitud_nombre > 0) {
                    longitud_nombre--;
                    nombreusuario[longitud_nombre] = '_';
                    nombreusuario2[longitud_nombre] = ' ';
                }
            }
            if (mensajenombre) {
                if (al_get_time() - tiempomensajenombre <= 3.0f) {
                    al_draw_text(fuente, al_map_rgb(255, 255, 255), 270 * escaladoX, 720 * escaladoY, 0, "TIENES QUE ESCRIBIR MINIMO 1 LETRA");
                }
                else {
                    mensajenombre = false;
                }
            }

            al_draw_text(fuentetitulo, color_actual, 935, 460, ALLEGRO_ALIGN_CENTER, nombreusuario);
            al_draw_text(fuentetitulo, color_actual, 937, 460, ALLEGRO_ALIGN_CENTER, nombreusuario);
            al_draw_text(fuentetitulo, al_map_rgb(255, 255, 255), 940, 460, ALLEGRO_ALIGN_CENTER, nombreusuario);
        }
        else if (menu) ////////////////////////////////////////////MENU////////////////////////////////////////////
        {
            juegoPerdido = false;
            vidas = 3;
            activoespera = true;
            color_actual = colorarcoiris(tiempo);
            al_draw_text(fuentetitulo, color_actual, 472 * escaladoX, 250 * escaladoY, 0, "ARKANOID");
            al_draw_text(fuentetitulo, color_actual, 475 * escaladoX, 250 * escaladoY, 0, "ARKANOID");
            al_draw_text(fuentetitulo, al_map_rgb(255, 255, 255), 477 * escaladoX, 250 * escaladoY, 0, "ARKANOID");

            if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch (evento.keyboard.keycode) {
                case ALLEGRO_KEY_ENTER:
                    if (opcion == 0) {
                        menu = false;
                        niveles = true;
                    }
                    else if (opcion == 1) {
                        menu = false;
                        duojugadores = true;
                        niveles = false;
                        top5 = false;
                        juegoperdidoseriop1 = false;
                        juegoperdidoseriop2 = false;
                        vidasfull();
                    }
                    else if (opcion == 2) {
                        menu = false;
                        al_stop_sample(&id_musica);
                        musicadetenida = true;
                        musicajuego = false;
                        top5 = true;
                        cargarDatosUsuarios(puntuaciones);
                        top5vector = mostrarTop5(puntuaciones);
                    }
                    opcion = 0;
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
                    if (opcion == 0) {
                        menu = false;
                        niveles = false;
                        duojugadores = false;
                        nivel1 = true;
                        nivel2 = false;
                        nivel3 = false;
                        opcion = 0;
                    }
                    if (opcion == 1) {
                        menu = false;
                        niveles = false;
                        duojugadores = false;
                        nivel1 = false;
                        nivel2 = true;
                        nivel3 = false;
                        opcion = 0;
                    }
                    if (opcion == 2) {
                        menu = false;
                        niveles = false;
                        duojugadores = false;
                        nivel1 = false;
                        nivel2 = false;
                        nivel3 = true;
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
                al_draw_filled_triangle(530 * escaladoX, 644 * escaladoY, 560 * escaladoX, 644 * escaladoY, 545 * escaladoX, 623 * escaladoY, al_map_rgb(0, 0, 205));
            }
            else if (opcion == 1) {
                al_draw_filled_triangle(940 * escaladoX, 644 * escaladoY, 970 * escaladoX, 644 * escaladoY, 955 * escaladoX, 623 * escaladoY, al_map_rgb(0, 0, 205));
            }
            else if (opcion == 2) {
                al_draw_filled_triangle(1320 * escaladoX, 644 * escaladoY, 1350 * escaladoX, 644 * escaladoY, 1335 * escaladoX, 623 * escaladoY, al_map_rgb(0, 0, 205));
            }

        }
        else if (duojugadores) ////////////////////////////////////////////NIVEL 1////////////////////////////////////////////
        {
            if (!(juegoperdidoseriop1 || juegoperdidoseriop2)) {
                tiempo += 0.3 / FPS;
                color_actual = colorarcoiris(tiempo);
                iniciarduojugadores(fuente, fuentesubtitulo, temporizador_bola, coladeevento, evento);
                tiempomensajeduo = al_get_time();
            }
            else if (juegoperdidoseriop1 && juegoperdidoseriop2) {
                string mensaje = "Empatados";
                al_draw_text(fuentetitulo, al_map_rgb(255, 255, 255), 420 * escaladoX, 600 * escaladoY, 0, "EMPATADOS");
                if (al_get_time() - tiempomensajeduo >= 0.5f) {
                    vidasfull();
                    hr = pVoice->Speak(wstring(mensaje.begin(), mensaje.end()).c_str(), SPF_IS_XML, NULL);
                    duojugadores = false;
                    menu = true;
                }

            }
            else if (juegoperdidoseriop2 && (!(juegoperdidoseriop1))) {
                string mensaje = "Jugador 1 GANADOR";
                al_draw_text(fuentetitulo, al_map_rgb(255, 255, 255), 400 * escaladoX, 250 * escaladoY, 0, "JUGADOR 1");
                al_draw_text(fuentetitulo, al_map_rgb(255, 255, 255), 420 * escaladoX, 600 * escaladoY, 0, "GANADOR");
                if (al_get_time() - tiempomensajeduo >= 0.5f) {
                    vidasfull();
                    hr = pVoice->Speak(wstring(mensaje.begin(), mensaje.end()).c_str(), SPF_IS_XML, NULL);
                    duojugadores = false;
                    menu = true;
                }
            }
            else if (juegoperdidoseriop1 && (!(juegoperdidoseriop2))) {
                string mensaje = "Jugador 2 GANADOR";
                al_draw_text(fuentetitulo, al_map_rgb(255, 255, 255), 400 * escaladoX, 250 * escaladoY, 0, "JUGADOR 2");
                al_draw_text(fuentetitulo, al_map_rgb(255, 255, 255), 420 * escaladoX, 600 * escaladoY, 0, "GANADOR");
                if (al_get_time() - tiempomensajeduo >= 0.5f) {
                    vidasfull();
                    hr = pVoice->Speak(wstring(mensaje.begin(), mensaje.end()).c_str(), SPF_IS_XML, NULL);
                    duojugadores = false;
                    menu = true;
                }
            }
        }
        else if (nivel1) ////////////////////////////////////////////NIVEL 1////////////////////////////////////////////
        {
            iniciarnivel1(fuente, temporizador_bola, coladeevento, evento);
            if (!quedanvidas()) {
                nivel1 = false;
                menu = true;
                iniciadoprimeravez = false;
            }
            if (!quedanbloques()) {
                nivel1 = false;
                nivel2 = false;
                nivel3 = false;
                transicion1 = true;
                puntosreales = puntos;
                tiempomensajetrans1 = al_get_time();
                bolaLanzada = false;
                al_stop_sample(&id_musica);
                musicadetenida = true;
                musicajuego = false;
                iniciadoprimeravez = false;
            }
        }
        else if (transicion1 && !nivel2) {
            if (jugadoractual->codigo <= puntosreales) {
                jugadoractual->codigo = puntosreales;
            }
            guardardatosusuarios(puntuaciones);
            //AQUI VA PARA GUARDAR
            al_draw_text(fuente, al_map_rgb(255, 255, 255), 445 * escaladoX, 300 * escaladoY, 0, "Haz Ganado el Nivel 1");
            al_draw_text(fuente, al_map_rgb(255, 255, 255), 445 * escaladoX, 500 * escaladoY, 0, "Continuando al siguiente...");

            if (al_get_time() - tiempomensajetrans1 >= 6.0f) {
                transicion1 = false;
                nivel1 = false;
                nivel3 = false;
                nivel2 = true;
                puntosreales = puntos;
                al_stop_sample(&id_musica);
                musicadetenida = true;
                musicajuego = false;
            }
        }
        else if (nivel2) {
            
            iniciarnivel2(fuente, temporizador_bola, coladeevento, evento);
            if (!quedanvidas()) {
                nivel2 = false;
                menu = true;
                iniciadoprimeravez = false;
            }
            if (!quedanbloques()) {
                nivel1 = false;
                nivel3 = false;
                nivel2 = false;
                transicion2 = true;
                puntosreales = puntos;
                tiempomensajetrans2 = al_get_time();
                bolaLanzada = false;
                al_stop_sample(&id_musica);
                musicadetenida = true;
                musicajuego = false;
                iniciadoprimeravez = false;
            }
        }
        else if (transicion2 && !nivel3) {
            if (jugadoractual->codigo <= puntosreales) {
                jugadoractual->codigo = puntosreales;
            }
            //AQUI VA PARA GUARDAR
            al_draw_text(fuente, al_map_rgb(255, 255, 255), 445 * escaladoX, 300 * escaladoY, 0, "Haz Ganado el Nivel 2");
            al_draw_text(fuente, al_map_rgb(255, 255, 255), 445 * escaladoX, 500 * escaladoY, 0, "Continuando al siguiente...");
            if (al_get_time() - tiempomensajetrans2 >= 6.0f) {
                transicion2 = false;
                nivel1 = false;
                nivel2 = false;
                nivel3 = true;
                puntosreales = puntos;
                al_stop_sample(&id_musica);
                musicadetenida = true;
                musicajuego = false;
            }
        }
        else if (nivel3) {
            iniciarnivel3(fuente, temporizador_bola, coladeevento, evento);
            if (!quedanvidas()) {
                nivel3 = false;
                menu = true;
                iniciadoprimeravez = false;
            }
            if (!quedanbloques()) {
                nivel1 = false;
                nivel3 = false;
                nivel2 = false;
                transicion3 = true;
                tiempomensajetrans3 = al_get_time();
                bolaLanzada = false;
                al_stop_sample(&id_musica);
                musicadetenida = true;
                musicajuego = false;
                iniciadoprimeravez = false;
                puntosreales = puntos;
            }
        }
        else if (transicion3 && !menu) {
            if (jugadoractual->codigo <= puntosreales) {
                jugadoractual->codigo = puntosreales;
            }
            guardardatosusuarios(puntuaciones);
            if (al_get_time() - tiempomensajetrans3 <= 37.0f) {
                int nombre_ancho = al_get_text_width(fuentetitulo, jugadoractual->nombre.c_str());
                float x_centrada = (952 * escaladoX) - nombre_ancho / 2;
                al_draw_text(fuentetitulo, al_map_rgb(215, 215, 255), x_centrada, 100 * escaladoY, 0, jugadoractual->nombre.c_str());
                al_draw_text(fuente, al_map_rgb(215, 215, 255), 602 * escaladoX, 220 * escaladoY, 0, "GRACIAS POR JUGAR");
                al_draw_text(fuentesubtitulo, al_map_rgb(215, 215, 255), 322 * escaladoX, 320 * escaladoY, 0, "El piloto de la nave logro vencer a todos los enemigos y bloques");
                al_draw_text(fuentesubtitulo, al_map_rgb(215, 215, 255), 348 * escaladoX, 390 * escaladoY, 0, "Su perro lo espera en casa, sin saber que el no podra volver");
                al_draw_text(fuentesubtitulo, al_map_rgb(215, 215, 255), 338 * escaladoX, 460 * escaladoY, 0, "Un hombre solitario en el espacio sin manera de volver, el era");
                al_draw_text(fuentesubtitulo, al_map_rgb(215, 215, 255), 322 * escaladoX, 530 * escaladoY, 0, "El sujeto perfecto sin ninguna conexion emocional, excepto por el");
                al_draw_text(fuentesubtitulo, al_map_rgb(215, 215, 255), 645 * escaladoX, 600 * escaladoY, 0, "Esperemos tenga bastante comida...");
                
                int totalxxx = 24;
                int frames_por_fila100 = al_get_bitmap_width(totoro) / totalxxx;
                int total_frames = frames_por_fila100 * (al_get_bitmap_height(totoro) / totalxxx); // Calcula el total de frames disponibles

                frame = frame % total_frames; // Asegúrate de que el frame esté dentro del rango válido

                int frameX100 = (frame % frames_por_fila100) * totalxxx;
                int frameY100 = (frame / frames_por_fila100) * totalxxx;

                int nuevo_ancho = totalxxx * 14;
                int nuevo_alto = totalxxx * 14;

                al_draw_scaled_bitmap(totoro, frameX100, frameY100, totalxxx, totalxxx, 775 * escaladoX, 670 * escaladoY, nuevo_ancho, nuevo_alto, 0);
            }
            else {
                transicion3 = false;
                nivel1 = false;
                nivel2 = false;
                nivel3 = false;
                menu = true;
                puntosreales = puntos;
                al_stop_sample(&id_musica);
                musicadetenida = false;
                musicajuego = false;
            }
        }
        else if (top5) {
            nombretts = false;
            color_actual = colorarcoiris(tiempo);
            if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch (evento.keyboard.keycode) {
                case ALLEGRO_KEY_ENTER:
                    if (opcion == 0 && top5vector[0] != nullptr) {
                        top5 = false;
                        personalpuntos = true;
                        tiempotts = al_get_time();
                        opcion = 0;
                    }
                    else if (opcion == 1 && top5vector[1] != nullptr) {
                        top5 = false;
                        personalpuntos = true;
                        tiempotts = al_get_time();
                        opcion = 1;
                    }
                    else if (opcion == 2 && top5vector[2] != nullptr) {
                        top5 = false;
                        personalpuntos = true;
                        tiempotts = al_get_time();
                        opcion = 2;
                    }
                    else if (opcion == 3 && top5vector[3] != nullptr) {
                        top5 = false;
                        personalpuntos = true;
                        tiempotts = al_get_time();
                        opcion = 3;
                    }
                    else if (opcion == 4 && top5vector[4] != nullptr) {
                        top5 = false;
                        personalpuntos = true;
                        tiempotts = al_get_time();
                        opcion = 4;
                    }
                    break;
                case ALLEGRO_KEY_DOWN:
                case ALLEGRO_KEY_S:
                    opcion = opcion + 1;
                    if (opcion % 5 == 0) {
                        opcion = 0;
                    }
                    break;
                case ALLEGRO_KEY_UP:
                case ALLEGRO_KEY_W:
                    opcion = opcion - 1;
                    if (opcion < 0) {
                        opcion = 4;
                    }
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    top5 = false;
                    opcion = 0;
                    al_stop_sample(&id_musica);
                    musicadetenida = false;
                    musicajuego = false;
                    menu = true;
                    break;
                }
            }
            
            al_draw_text(fuente, color_actual, 801 * escaladoX, 100 * escaladoY, 0, "TOP 5");
            al_draw_text(fuente, color_actual, 803 * escaladoX, 100 * escaladoY, 0, "TOP 5");
            al_draw_text(fuente, al_map_rgb(255, 255, 255), 805 * escaladoX, 100 * escaladoY, 0, "TOP 5");
            if (top5vector[0] != nullptr) {
                if (opcion == 0) {
                    al_draw_text(fuente, color_actual, 352 * escaladoX, 300 * escaladoY, 0, top5vector[0]->nombre.c_str());
                    al_draw_text(fuente, color_actual, 1383 * escaladoX, 300 * escaladoY, 0, to_string(top5vector[0]->codigo).c_str());
                }
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 355 * escaladoX, 300 * escaladoY, 0, top5vector[0]->nombre.c_str());
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 1385 * escaladoX, 300 * escaladoY, 0, to_string(top5vector[0]->codigo).c_str());
            }
            else {
                if (opcion == 0) {
                    al_draw_text(fuente, color_actual, 352 * escaladoX, 300 * escaladoY, 0, "WAITING FOR CHALLENGER");
                }
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 355 * escaladoX, 300 * escaladoY, 0, "WAITING FOR CHALLENGER");
            }
            if (top5vector[1] != nullptr) {
                if (opcion == 1) {
                    al_draw_text(fuente, color_actual, 352 * escaladoX, 400 * escaladoY, 0, top5vector[1]->nombre.c_str());
                    al_draw_text(fuente, color_actual, 1383 * escaladoX, 400 * escaladoY, 0, to_string(top5vector[1]->codigo).c_str());
                }
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 355 * escaladoX, 400 * escaladoY, 0, top5vector[1]->nombre.c_str());
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 1385 * escaladoX, 400 * escaladoY, 0, to_string(top5vector[1]->codigo).c_str());
            }
            else {
                if (opcion == 1) {
                    al_draw_text(fuente, color_actual, 352 * escaladoX, 400 * escaladoY, 0, "WAITING FOR CHALLENGER");
                }
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 355 * escaladoX, 400 * escaladoY, 0, "WAITING FOR CHALLENGER");
            }
            if (top5vector[2] != nullptr) {
                if (opcion == 2) {
                    al_draw_text(fuente, color_actual, 352 * escaladoX, 500 * escaladoY, 0, top5vector[2]->nombre.c_str());
                    al_draw_text(fuente, color_actual, 1383 * escaladoX, 500 * escaladoY, 0, to_string(top5vector[2]->codigo).c_str());
                }
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 355 * escaladoX, 500 * escaladoY, 0, top5vector[2]->nombre.c_str());
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 1385 * escaladoX, 500 * escaladoY, 0, to_string(top5vector[2]->codigo).c_str());
            }
            else {
                if (opcion == 2) {
                    al_draw_text(fuente, color_actual, 352 * escaladoX, 500 * escaladoY, 0, "WAITING FOR CHALLENGER");
                }
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 355 * escaladoX, 500 * escaladoY, 0, "WAITING FOR CHALLENGER");
            }
            if (top5vector[3] != nullptr) {
                if (opcion == 3) {
                    al_draw_text(fuente, color_actual, 352 * escaladoX, 600 * escaladoY, 0, top5vector[3]->nombre.c_str());
                    al_draw_text(fuente, color_actual, 1383 * escaladoX, 600 * escaladoY, 0, to_string(top5vector[3]->codigo).c_str());
                }
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 355 * escaladoX, 600 * escaladoY, 0, top5vector[3]->nombre.c_str());
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 1385 * escaladoX, 600 * escaladoY, 0, to_string(top5vector[3]->codigo).c_str());
            }
            else {
                if (opcion == 3) {
                    al_draw_text(fuente, color_actual, 352 * escaladoX, 600 * escaladoY, 0, "WAITING FOR CHALLENGER");
                }
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 355 * escaladoX, 600 * escaladoY, 0, "WAITING FOR CHALLENGER");
            }
            if (top5vector[4] != nullptr) {
                if (opcion == 4) {
                    al_draw_text(fuente, color_actual, 352 * escaladoX, 700 * escaladoY, 0, top5vector[4]->nombre.c_str());
                    al_draw_text(fuente, color_actual, 1383 * escaladoX, 700 * escaladoY, 0, to_string(top5vector[4]->codigo).c_str());
                }
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 355 * escaladoX, 700 * escaladoY, 0, top5vector[4]->nombre.c_str());
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 1385 * escaladoX, 700 * escaladoY, 0, to_string(top5vector[4]->codigo).c_str());
            }
            else {
                if (opcion == 4) {
                    al_draw_text(fuente, color_actual, 352 * escaladoX, 700 * escaladoY, 0, "WAITING FOR CHALLENGER");
                }
                al_draw_text(fuente, al_map_rgb(255, 255, 255), 355 * escaladoX, 700 * escaladoY, 0, "WAITING FOR CHALLENGER");
            }
            al_draw_text(fuente, color_actual, 362 * escaladoX, 900 * escaladoY, 0, "PRESIONE ENTER PARA MOSTRAR MAS");
            al_draw_text(fuente, al_map_rgb(255, 255, 255), 365 * escaladoX, 900 * escaladoY, 0, "PRESIONE ENTER PARA MOSTRAR MAS");
        }
        if (personalpuntos) {
            color_actual = colorarcoiris(tiempo);
            if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch (evento.keyboard.keycode) {
                case ALLEGRO_KEY_ESCAPE:
                    personalpuntos = false;
                    top5 = true;
                    opcion = 0;
                    break;
                }
            }
            int nombre_ancho = al_get_text_width(fuentetitulo, top5vector[opcion]->nombre.c_str());
            float x_centrada = (952 * escaladoX) - nombre_ancho / 2;

            // Dibujar el nombre centrado
            al_draw_text(fuentetitulo, color_actual, x_centrada, 100 * escaladoY, 0, top5vector[opcion]->nombre.c_str());
            al_draw_text(fuentetitulo, al_map_rgb(255, 255, 255), x_centrada + 3, 100 * escaladoY, 0, top5vector[opcion]->nombre.c_str());

            al_draw_text(fuente, color_actual, 302 * escaladoX, 300 * escaladoY, 0, "PUNTUAJE MAXIMO");
            al_draw_text(fuente, al_map_rgb(255, 255, 255), 305 * escaladoX, 300 * escaladoY, 0, "PUNTUAJE MAXIMO");
            al_draw_text(fuente, color_actual, 1332 * escaladoX, 300 * escaladoY, 0, to_string(top5vector[opcion]->codigo).c_str());
            al_draw_text(fuente, al_map_rgb(255, 255, 255), 1335 * escaladoX, 300 * escaladoY, 0, to_string(top5vector[opcion]->codigo).c_str());
            al_draw_text(fuente, color_actual, 302 * escaladoX, 400 * escaladoY, 0, "BOLAS LANZADAS");
            al_draw_text(fuente, al_map_rgb(255, 255, 255), 305 * escaladoX, 400 * escaladoY, 0, "BOLAS LANZADAS");
            al_draw_text(fuente, color_actual, 1332 * escaladoX, 400 * escaladoY, 0, to_string(top5vector[opcion]->totalbolas).c_str());
            al_draw_text(fuente, al_map_rgb(255, 255, 255), 1335 * escaladoX, 400 * escaladoY, 0, to_string(top5vector[opcion]->totalbolas).c_str());
            al_draw_text(fuente, color_actual, 302 * escaladoX, 500 * escaladoY, 0, "BOLAS REBOTADAS");
            al_draw_text(fuente, al_map_rgb(255, 255, 255), 305 * escaladoX, 500 * escaladoY, 0, "BOLAS REBOTADAS");
            al_draw_text(fuente, color_actual, 1332 * escaladoX, 500 * escaladoY, 0, to_string(top5vector[opcion]->bolasreb).c_str());
            al_draw_text(fuente, al_map_rgb(255, 255, 255), 1335 * escaladoX, 500 * escaladoY, 0, to_string(top5vector[opcion]->bolasreb).c_str());
            al_draw_text(fuente, color_actual, 302 * escaladoX, 600 * escaladoY, 0, "BOLAS PERDIDAS");
            al_draw_text(fuente, al_map_rgb(255, 255, 255), 305 * escaladoX, 600 * escaladoY, 0, "BOLAS PERDIDAS");
            al_draw_text(fuente, color_actual, 1332 * escaladoX, 600 * escaladoY, 0, to_string(top5vector[opcion]->bolasper).c_str());
            al_draw_text(fuente, al_map_rgb(255, 255, 255), 1335 * escaladoX, 600 * escaladoY, 0, to_string(top5vector[opcion]->bolasper).c_str());
            al_draw_text(fuente, color_actual, 302 * escaladoX, 700 * escaladoY, 0, "OBJETOS DESTRUIDOS");
            al_draw_text(fuente, al_map_rgb(255, 255, 255), 305 * escaladoX, 700 * escaladoY, 0, "OBJETOS DESTRUIDOS");
            al_draw_text(fuente, color_actual, 1332 * escaladoX, 700 * escaladoY, 0, to_string(top5vector[opcion]->objetosdestruidos).c_str());
            al_draw_text(fuente, al_map_rgb(255, 255, 255), 1335 * escaladoX, 700 * escaladoY, 0, to_string(top5vector[opcion]->objetosdestruidos).c_str());
            if (!nombretts && al_get_time() - tiempotts >= 0.3 && opcion >= 0 && opcion <= 4) {
                hr = pVoice->Speak(wstring(top5vector[opcion]->nombre.begin(), top5vector[opcion]->nombre.end()).c_str(), SPF_IS_XML, NULL);
                nombretts = true;
            }
            al_draw_text(fuente, color_actual, 452 * escaladoX, 950 * escaladoY, 0, "PRESIONE ESC PARA SALIR");
            al_draw_text(fuente, al_map_rgb(255, 255, 255), 455 * escaladoX, 950 * escaladoY, 0, "PRESIONE ESC PARA SALIR");
        }
        al_flip_display();
    }
    pVoice->Release();
    CoUninitialize();
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