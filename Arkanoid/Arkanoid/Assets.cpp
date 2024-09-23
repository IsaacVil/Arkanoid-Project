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
#include <cstdlib>
#include <ctime>
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
float velocidad = 60 * escaladoX1;          // Velocidad de movimiento

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

ALLEGRO_COLOR oscurecerColor(ALLEGRO_COLOR color, float factor) {
    float r, g, b, a;
    al_unmap_rgb_f(color, &r, &g, &b); // Descomponer el color en sus componentes
    // Reducir cada componente del color según el factor
    r *= factor;
    g *= factor;
    b *= factor;
    return al_map_rgb_f(r, g, b); // Volver a componer el color
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
    int golpe;
    ALLEGRO_COLOR color;
    Bloque* siguiente;
}* ptrbloques;

ptrbloques listabloques = nullptr;

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

void agregarBola(ptrbola& bola) {
    bola->siguiente = listabola;
    listabola = bola;
}

ptrbloques crearBloque(int x, int y, int ancho, int alto, int tipo, int cantidadgolpe, ALLEGRO_COLOR color) {
    ptrbloques bloque = new(Bloque);
    bloque->plataformax = x;
    bloque->plataformay = y;
    bloque->plataformaanchura = ancho;
    bloque->plataformaaltura = alto;
    bloque->tipodebola = tipo;
    bloque->color = color;
    bloque->golpe = cantidadgolpe;
    bloque->siguiente = nullptr;
    return bloque;
}

void agregarBloques(ptrbloques& bloques) {
    bloques->siguiente = listabloques;
    listabloques = bloques;
}

void dibujarBloques() {
    ptrbloques actual = listabloques;
    while (actual != nullptr) {
        if (actual->golpe > 0) {
            ALLEGRO_COLOR colorOscuro = oscurecerColor(actual->color, 1.5f);
            al_draw_filled_rectangle(actual->plataformax, actual->plataformay, actual->plataformaanchura, actual->plataformaaltura, actual->color);
            al_draw_filled_rectangle(actual->plataformax + 3, actual->plataformay + 3, actual->plataformaanchura - 3, actual->plataformaaltura - 40, colorOscuro);
            al_draw_filled_rectangle(actual->plataformax + 3, actual->plataformay + 3, actual->plataformax + 15, actual->plataformaaltura - 3, colorOscuro);
        }
        actual = actual->siguiente;
    }
}

int achocado(ptrbola bola, ptrbloques bloque) {
    int margen = 1;
    if (bola->x - bola->radio <= bloque->plataformax + margen && bola->x + bola->radio >= bloque->plataformax - margen &&
        bola->y >= bloque->plataformay && bola->y <= bloque->plataformaaltura &&
        bola->velocidadX > 0) {
        return 1;
    }
    if (bola->x - bola->radio <= bloque->plataformaanchura + margen && bola->x + bola->radio >= bloque->plataformaanchura - margen &&
        bola->y >= bloque->plataformay && bola->y <= bloque->plataformaaltura &&
        bola->velocidadX < 0) {
        return 1;
    }

    if (bola->y - bola->radio <= bloque->plataformay + margen && bola->y + bola->radio >= bloque->plataformay - margen &&
        bola->x >= bloque->plataformax && bola->x <= bloque->plataformaanchura &&
        bola->velocidadY > 0) {
        return 2;
    }

    // Colisión con la parte inferior del bloque
    if (bola->y - bola->radio <= bloque->plataformaaltura + margen && bola->y + bola->radio >= bloque->plataformaaltura - margen &&
        bola->x >= bloque->plataformax && bola->x <= bloque->plataformaanchura &&
        bola->velocidadY < 0) {
        return 2;
    }
    return 0;
}

void manejarcolision(ptrbola& bola) {
    ptrbloques bloque = listabloques;
    ptrbloques anterior = nullptr; // Para llevar un seguimiento del bloque anterior

    while (bloque != nullptr) {
        if (achocado(bola, bloque) == 1) {
            bola->velocidadX = -bola->velocidadX;
            puntos += 150;
            bloque->golpe--;
        }
        else if (achocado(bola, bloque) == 2) {
            bola->velocidadY = -bola->velocidadY;
            puntos += 150;
            bloque->golpe--;
        }

        // Si el bloque ha sido golpeado hasta 0
        if (bloque->golpe <= 0) {
            if (anterior == nullptr) {
                // El bloque a eliminar es el primero en la lista
                listabloques = bloque->siguiente;
            }
            else {
                // El bloque a eliminar no es el primero
                anterior->siguiente = bloque->siguiente;
            }
            ptrbloques bloque_a_eliminar = bloque; // Guardar el bloque para eliminarlo
            bloque = bloque->siguiente; // Avanzar al siguiente bloque
            delete bloque_a_eliminar; // Liberar la memoria del bloque eliminado
        }
        else {
            anterior = bloque; // Actualizar el bloque anterior solo si no se eliminó
            bloque = bloque->siguiente; // Avanzar al siguiente bloque
        }
    }
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
        float velocidadX = (4.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0f)))*escaladoX1;
        float velocidadY = -5.0f * escaladoY1; // Lanza la bola hacia arriba
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

        manejarcolision(actual);

        // Colisión con la plataforma
        if (actual->y + actual->radio >= plataforma_y - plataforma_altura / 2.0 &&actual->x >= plataforma_x - plataforma_anchura / 2.0 && actual->x <= plataforma_x + plataforma_anchura / 2.0) {
            actual->velocidadY *= -1; 
            actual->y = plataforma_y - plataforma_altura / 2.0 - actual->radio;
            puntos += 50;
        }

        // Si cae más allá de la pantalla, eliminar la bola
        if (actual->y - actual->radio > 1040 * escaladoY1) { // Prueba con un valor fijo
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

void liberarBloques() {
    while (listabloques != nullptr) {
        ptrbloques Aux = listabloques;
        listabloques = listabloques->siguiente;
        delete Aux;
    }
}

void paleta(ALLEGRO_EVENT_QUEUE*& coladeeventos, ALLEGRO_EVENT& evento, ALLEGRO_FONT* fuente) {
    al_draw_filled_rectangle(plataforma_x - plataforma_anchura / 2.0, plataforma_y - plataforma_altura / 2.0, plataforma_x + plataforma_anchura / 2.0, plataforma_y + plataforma_altura / 2.0, colorplataforma);
    al_draw_filled_rectangle(plataforma_x - plataforma_anchura / 2.0 + 15 * escaladoY1, plataforma_y - plataforma_altura / 2.0 - 5 * escaladoY1, plataforma_x - plataforma_anchura / 2.0 + 30 * escaladoY1, plataforma_y + plataforma_altura / 2.0 + 5 * escaladoY1, colorplataforma2);
    al_draw_filled_rectangle(plataforma_x + plataforma_anchura / 2.0 - 15 * escaladoY1, plataforma_y - plataforma_altura / 2.0 - 5 * escaladoY1, plataforma_x + plataforma_anchura / 2.0 - 30 * escaladoY1, plataforma_y + plataforma_altura / 2.0 + 5 * escaladoY1, colorplataforma2);
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
    dibujarBloques();
}

bool quedanvidas() {
    if (vidas <= 0) {
        return false;
    }
    return true;
}

bool mostrarmensaje = false;

void hazperdidounavida() {
    if (quedanvidas()) {
        mostrarmensaje = true;
        tiempoMensaje = al_get_time(); // Obtén el tiempo actual
        bolaLanzada = false;
    }
    else {
        mostrarmensaje = false;
        bolaLanzada = false;
        iniciadoprimeravez = false;
    }
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

bool iniciadoprimeravez = false;
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
    if (juegoPerdido || !iniciadoprimeravez) {
        liberarBloques();
        int x1 = 530;
        int x2 = 680;
        int x3 = 830;
        int x4 = 980;
        int x5 = 1130;
        int y1 = 200;
        int y2 = 250;
        int multiplicador = 0;
        for (int i = 0; i < 4; i++) {
            ptrbloques nuevo = crearBloque(x1 * escaladoX1, (y1 + (100*multiplicador)) * escaladoY1, (x1+100) * escaladoX1, (y2 + (100 * multiplicador)) * escaladoY1, 1, 3, al_map_rgb(rand()% 205 + 50, rand() % 205 + 50, rand() % 155));
            ptrbloques nuevo1 = crearBloque(x2 * escaladoX1, (y1 + (100 * multiplicador)) * escaladoY1, (x2 + 100) * escaladoX1, (y2 + (100 * multiplicador)) * escaladoY1, 1, 3, al_map_rgb(rand() % 205 + 50, rand() % 205 + 50, rand() % 155));
            ptrbloques nuevo2 = crearBloque(x3 * escaladoX1, (y1 + (100 * multiplicador)) * escaladoY1, (x3 + 100) * escaladoX1, (y2 + (100 * multiplicador)) * escaladoY1, 1, 3, al_map_rgb(rand() % 205 + 50, rand() % 205 + 50, rand() % 155));
            ptrbloques nuevo3 = crearBloque(x4 * escaladoX1, (y1 + (100 * multiplicador)) * escaladoY1, (x4 + 100) * escaladoX1, (y2 + (100 * multiplicador)) * escaladoY1, 1, 3, al_map_rgb(rand() % 205 + 50, rand() % 205 + 50, rand() % 155));
            ptrbloques nuevo4 = crearBloque(x5 * escaladoX1, (y1 + (100 * multiplicador)) * escaladoY1, (x5 + 100) * escaladoX1, (y2 + (100 * multiplicador)) * escaladoY1, 1, 3, al_map_rgb(rand() % 205 + 50, rand() % 205 + 50, rand() % 155));
            agregarBloques(nuevo);
            agregarBloques(nuevo1);
            agregarBloques(nuevo2);
            agregarBloques(nuevo3);
            agregarBloques(nuevo4);
            multiplicador++;
        }
        ptrbloques bloque1 = crearBloque(650 * escaladoX1, 600 * escaladoY1, 1300 * escaladoX1, 640 * escaladoY1, 1, 999999, al_map_rgb(145, 145, 145));
        agregarBloques(bloque1);
        iniciadoprimeravez = true;
    }
    perdidavida(fuente);
}
