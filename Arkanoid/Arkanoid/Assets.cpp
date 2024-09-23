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
float escaladoX1 = devolverx();
float escaladoY1 = devolvery();
using namespace std;

const int anchurainicio = 440 * escaladoX1;
const int anchurafinal = 1300 * escaladoX1;

float plataforma_x = ((anchurainicio + anchurafinal) / 2.0) * escaladoX1;
float plataforma_y = (1000 - 50) * escaladoY1; // Posiciona la plataforma en la parte inferior de la ventana
float plataforma_anchura = 180 * escaladoX1; // Ancho de la plataforma
float plataforma_altura = 20 * escaladoY1;   // Alto de la plataforma
float velocidad = 50 * escaladoX1;          // Velocidad de movimiento

bool juegoPerdido = false;
float tiempoMensaje = 0.0f;

int vidas = 3;

ALLEGRO_COLOR colorarcoiris(float tiempo) {
    float valor = fmod(tiempo, 1.0f);
    float r = fabs(sin(valor * 2.0f * 3.14159f));
    float g = fabs(sin((valor + 0.333f) * 2.0f * 3.14159f));
    float b = fabs(sin((valor + 0.666f) * 2.0f * 3.14159f));
    return al_map_rgb_f(r, g, b);
}
ALLEGRO_COLOR colorplataforma = al_map_rgb(0, 255, 0);
ALLEGRO_COLOR colorplataforma2 = al_map_rgb(155, 255, 155);

typedef struct Bola {
    float x;
    float y;
    float velocidadX;
    float velocidadY;
    float radio;
    Bola* siguiente; // Puntero al siguiente nodo
}* ptrbola;

ptrbola listabola = nullptr;

typedef struct Bloque {
    int plataformax;
    int plataformay;
    int plataformaanchura;
    int plataformaaltura;
    int tipodebola;
    ALLEGRO_COLOR color;
    Bloque* siguiente;
}* ptrbloques;

ptrbola crearBola(float x, float y, float velocidadX, float velocidadY, float radio) {
    ptrbola bola = new(Bola);
    bola->x = x;
    bola->y = y;
    bola->velocidadX = velocidadX;
    bola->velocidadY = velocidadY;
    bola->radio = radio;
    bola->siguiente = nullptr;
    return bola;
}

Bloque crearBloque(int x, int y, int ancho, int alto, int tipo, ALLEGRO_COLOR color) {
    Bloque bloque;
    bloque.plataformax = x;
    bloque.plataformay = y;
    bloque.plataformaanchura = ancho;
    bloque.plataformaaltura = alto;
    bloque.tipodebola = tipo;
    bloque.color = color;
    bloque.siguiente = nullptr;
    return bloque;
}


void agregarBola(ptrbola& bola) {
    bola->siguiente = listabola;
    listabola = bola;
}

void dibujarBolas() {
    ptrbola actual = listabola;
    while (actual != nullptr) {
        al_draw_filled_circle(actual->x, actual->y, actual->radio, al_map_rgb(255, 0, 0));
        actual = actual->siguiente;
    }
}

bool bolaLanzada = false; // Estado para verificar si se ha lanzado la bola

void lanzarBolaDesdePlataforma(float plataforma_x, float plataforma_y, float plataforma_altura) {
    if (!bolaLanzada) {
        float velocidadX = 5.0f; // Ajusta la velocidad X según sea necesario
        float velocidadY = -5.0f; // Lanza la bola hacia arriba
        ptrbola nuevabola = crearBola(plataforma_x, plataforma_y - plataforma_altura / 2.0 - 10.0f, velocidadX, velocidadY, 10.0f);
        agregarBola(nuevabola); // 10.0f es el radio de la bola
        bolaLanzada = true; // Marca la bola como lanzada
    }
}

void actualizarBolas(float plataforma_x, float plataforma_y, float plataforma_anchura, float plataforma_altura) {
    ptrbola actual = listabola;
    ptrbola anterior = nullptr;

    while (actual != nullptr) {
        // Actualiza la posición
        actual->x += actual->velocidadX;
        actual->y += actual->velocidadY;

        // Colisiones con las paredes
        if (actual->x - actual->radio < 440 * escaladoX1 || actual->x + actual->radio > 1300 * escaladoX1) {
            actual->velocidadX *= -1; // Rebotar en la pared izquierda o derecha
        }
        if (actual->y - actual->radio < 40 * escaladoY1) {
            actual->velocidadY *= -1; // Rebotar en la parte superior
        }

        // Colisión con la plataforma
        if (actual->y + actual->radio >= plataforma_y - plataforma_altura / 2.0 &&actual->x >= plataforma_x - plataforma_anchura / 2.0 && actual->x <= plataforma_x + plataforma_anchura / 2.0) {
            actual->velocidadY *= -1; 
            actual->y = plataforma_y - plataforma_altura / 2.0 - actual->radio;
            puntos += 100;
        }

        // Si cae más allá de la pantalla, eliminar la bola
        if (actual->y - actual->radio > 1040) { // Prueba con un valor fijo
            Bola* Aux = actual;
            if (anterior) {
                anterior->siguiente = actual->siguiente; 
            }
            else {
                listabola = actual->siguiente; // Actualiza la cabeza si se elimina el primer nodo
            }
            actual = actual->siguiente; // Avanza al siguiente nodo
            delete Aux; // Libera memoria
        }
        else {
            anterior = actual;
            actual = actual->siguiente; // Avanza al siguiente nodo
        }
    }

    // Verifica si la lista está vacía
    if (listabola == nullptr) {
        juegoPerdido = true;
    }
}

void liberarBolas() {
    while (listabola != nullptr) {
        ptrbola Aux = listabola;
        listabola = listabola->siguiente;
        delete Aux;
    }
}

void paleta(ALLEGRO_EVENT_QUEUE*& coladeeventos, ALLEGRO_EVENT& evento, ALLEGRO_FONT* fuente) {
    al_draw_filled_rectangle(plataforma_x - plataforma_anchura / 2.0, plataforma_y - plataforma_altura / 2.0, plataforma_x + plataforma_anchura / 2.0, plataforma_y + plataforma_altura / 2.0, colorplataforma);
    al_draw_filled_rectangle(plataforma_x - plataforma_anchura / 2.0 + 15, plataforma_y - plataforma_altura / 2.0 - 5, plataforma_x - plataforma_anchura / 2.0 + 30, plataforma_y + plataforma_altura / 2.0 + 5, colorplataforma2);
    al_draw_filled_rectangle(plataforma_x + plataforma_anchura / 2.0 - 15, plataforma_y - plataforma_altura / 2.0 - 5, plataforma_x + plataforma_anchura / 2.0 - 30, plataforma_y + plataforma_altura / 2.0 + 5, colorplataforma2);
    if (!bolaLanzada) {
        al_draw_filled_circle(plataforma_x, plataforma_y - plataforma_altura / 2.0, 10.0f, al_map_rgb(255, 0, 0)); // Color rojo para el círculo
    }

    if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (evento.keyboard.keycode) {
        case ALLEGRO_KEY_A:
            plataforma_x -= velocidad;
            break;
        case ALLEGRO_KEY_D:
            plataforma_x += velocidad;
            break;
        case ALLEGRO_KEY_ENTER: 
            lanzarBolaDesdePlataforma(plataforma_x, plataforma_y, plataforma_altura);
            break;
        }
    }

    if (plataforma_x - plataforma_anchura / 2.0 < anchurainicio) {
        plataforma_x = anchurainicio + plataforma_anchura / 2.0;
    }
    else if (plataforma_x + plataforma_anchura / 2.0 > anchurafinal) {
        plataforma_x = anchurafinal - plataforma_anchura / 2.0;
    }
    if (bolaLanzada == true) {
        actualizarBolas(plataforma_x, plataforma_y, plataforma_anchura, plataforma_altura);
    }
    dibujarBolas();

}

bool quedanvidas() {
    if (vidas <= 0) {
        return false;
    }
    return true;
}

bool mostrarmensaje = false;

void hazperdidounavida() {
    mostrarmensaje = true;
    tiempoMensaje = al_get_time(); // Obtén el tiempo actual
    bolaLanzada = false;
}

void perdidavida(ALLEGRO_FONT* fuente) {
    if (mostrarmensaje) {
        al_draw_text(fuente, al_map_rgb(255, 255, 255), 480 * escaladoX1, 300 * escaladoY1, 0, "Haz Perdido Una Vida");
        plataforma_x = ((anchurainicio + anchurafinal) / 2.0) * escaladoX1;
        plataforma_y = (1000 - 50) * escaladoY1;
        plataforma_anchura = 180 * escaladoX1; 
        plataforma_altura = 20 * escaladoY1; 
        velocidad = 50 * escaladoX1;        
        if (al_get_time() - tiempoMensaje >= 5.0f) {
            mostrarmensaje = false; // Oculta el mensaje
            juegoPerdido = false;
        }
    }
}


void iniciarnivel1(ALLEGRO_FONT* fuente, ALLEGRO_TIMER* temporizador_bola, ALLEGRO_EVENT_QUEUE* coladeeventos, ALLEGRO_EVENT& evento) {
    al_draw_filled_rectangle(430 * escaladoX1, 40 * escaladoY1, 1310 * escaladoX1, 1000 * escaladoY1, al_map_rgb(145, 145, 145));
    al_draw_filled_rectangle(440 * escaladoX1, 50 * escaladoY1, 1300 * escaladoX1, 1000 * escaladoY1, al_map_rgb(0, 0, 155));
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 1500 * escaladoX1, 300 * escaladoY1, 0, "Puntos");
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 1500 * escaladoX1, 350 * escaladoY1, 0, to_string(puntos).c_str());
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 1500 * escaladoX1, 400 * escaladoY1, 0, "Vidas");
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 1500 * escaladoX1, 450 * escaladoY1, 0, to_string(vidas).c_str());
    if (juegoPerdido && vidas > 0 && !mostrarmensaje) {
        vidas = vidas - 1;
        hazperdidounavida();
    }
    else if (!mostrarmensaje){
        paleta(coladeeventos, evento, fuente);
    }
    perdidavida(fuente);
}
