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

const int anchurainicio = 360 * escaladoX1;
const int anchurafinal = 1300 * escaladoX1;

float plataforma_x = ((anchurainicio + anchurafinal) / 2.0) * escaladoX1;
float plataforma_y = (1000 - 50) * escaladoY1; // Posiciona la plataforma en la parte inferior de la ventana
float plataforma_anchura = 180 * escaladoX1; // Ancho de la plataforma
float plataforma_altura = 20 * escaladoY1;   // Alto de la plataforma
float velocidad = 60 * escaladoX1;          // Velocidad de movimiento
float tiempoMensaje = 0.0f;
float tiempo_animacion2 = 0.15; // Tiempo en segundos entre frames (ajustado)
float tiempo_transcurrido2 = 0;
bool capsulacaida = false;
bool juegoPerdido = false;
bool bolaLanzada = false;
bool mostrarmensaje = false;
bool iniciadoprimeravez = false;
int contador_frame = 0;
int velocidad_frame = 100;
int frame2 = 0;
int vidas = 3;
int frame_actual2 = 0;
int frames_totales2 = 8;
double tiempoInicioCapsula = 0;
double tiempo_anterior2 = 0; // Tiempo del último frame
int tipocapsula = 1;



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

typedef struct Bloque {
    int plataformax;
    int plataformay;
    int plataformaanchura;
    int plataformaaltura;
    int tipodebloque;
    int golpe;
    ALLEGRO_COLOR color;
    Bloque* siguiente;
}* ptrbloques;

typedef struct Capsula {
    int tipo;
    int x;
    int y;
    Capsula* siguiente;
}* ptrcapsulas;

typedef struct Poderes {
    int tipo;
    int tiempo;
    int usado;
    Poderes* siguiente;
}* ptrpoderes;

ptrbola listabola = nullptr;
ptrbloques listabloques = nullptr;
ptrcapsulas listacapsula = nullptr;
ptrpoderes listapoderes = nullptr;
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

ptrbloques crearBloque(int x, int y, int ancho, int alto, int tipo, int cantidadgolpe, ALLEGRO_COLOR color) {
    ptrbloques bloque = new(Bloque);
    bloque->plataformax = x;
    bloque->plataformay = y;
    bloque->plataformaanchura = ancho;
    bloque->plataformaaltura = alto;
    bloque->tipodebloque = tipo;
    bloque->color = color;
    bloque->golpe = cantidadgolpe;
    bloque->siguiente = nullptr;
    return bloque;
}

ptrcapsulas crearCapsula(int x, int y, int tipo) {
    ptrcapsulas nuevo = new(Capsula);
    nuevo->x = x;
    nuevo->y = y;
    nuevo->tipo = tipo;
    nuevo->siguiente = nullptr;
    return nuevo;
}

ptrpoderes crearPoder(int tipo, int tiempo) {
    ptrpoderes nuevo = new(Poderes);
    nuevo->tipo = tipo;
    nuevo->tiempo = tiempo;
    nuevo->usado = 0;
    return nuevo;
}

void agregarBola(ptrbola& bola) {
    bola->siguiente = listabola;
    listabola = bola;
}

void agregarBloques(ptrbloques& bloque) {
    bloque->siguiente = listabloques;
    listabloques = bloque;
}

void agregarCapsulas(ptrcapsulas& capsula) {
    capsula->siguiente = listacapsula;
    listacapsula = capsula;
}

void agregarPoderes(ptrpoderes& nuevo) {
    nuevo->siguiente = listapoderes;
    listapoderes = nuevo;
}

void dibujarBloques() {
    ptrbloques actual = listabloques;
    while (actual != nullptr) {
        if (actual->golpe > 0) {
            ALLEGRO_COLOR colorOscuro = oscurecerColor(actual->color, 1.5f);
            al_draw_filled_rectangle(actual->plataformax, actual->plataformay, actual->plataformaanchura, actual->plataformaaltura, actual->color);
            al_draw_filled_rectangle(actual->plataformax + 3 * escaladoX1, actual->plataformay + 40 * escaladoY1, actual->plataformaanchura - 3 * escaladoX1, actual->plataformaaltura - 3 * escaladoY1, oscurecerColor(actual->color, 0.25f));
            al_draw_filled_rectangle(actual->plataformax + 85 * escaladoX1, actual->plataformay + 3 * escaladoY1, actual->plataformaanchura - 3 * escaladoX1, actual->plataformaaltura - 3 * escaladoY1, oscurecerColor(actual->color, 0.25f));
        }
        actual = actual->siguiente;
    }
}

void dibujarBolas() {
    ptrbola actual = listabola;
    while (actual != nullptr) {
        al_draw_filled_circle(actual->x, actual->y, actual->radio, al_map_rgb(255, 0, 0));
        actual = actual->siguiente;
    }
}

void capsulasdibujadas(ptrcapsulas capsu) {
    int x = capsu->x;
    int y = capsu->y;
    ALLEGRO_BITMAP* capsula;
    if (capsu->tipo == 1) {
        capsula = al_load_bitmap("sprites/capsulas/1.png");
    }
    else if (capsu->tipo == 2) {
        capsula = al_load_bitmap("sprites/capsulas/2.png");
    }
    else if (capsu->tipo == 3) {
        capsula = al_load_bitmap("sprites/capsulas/3.png");
    }
    else if (capsu->tipo == 4) {
        capsula = al_load_bitmap("sprites/capsulas/4.png");
    }
    else if (capsu->tipo == 5) {
        capsula = al_load_bitmap("sprites/capsulas/5.png");
    }
    else if (capsu->tipo == 6) {
        capsula = al_load_bitmap("sprites/capsulas/6.png");
    }
    else if (capsu->tipo == 7) {
        capsula = al_load_bitmap("sprites/capsulas/7.png");
    }
    else {
        capsula = al_load_bitmap("sprites/capsulas/1.png");
    }
    int ancho_frame2 = al_get_bitmap_width(capsula) / frames_totales2;
    int alto_frame2 = al_get_bitmap_height(capsula);
    float escala2 = 3.5 * escaladoX;

    // Obtener el tiempo actual y calcular el delta (diferencia de tiempo)
    double tiempo_actual2 = al_get_time();
    double delta2 = tiempo_actual2 - tiempo_anterior2;
    tiempo_anterior2 = tiempo_actual2;

    // Ciclo de animación
    tiempo_transcurrido2 += delta2;  // Aumenta el tiempo transcurrido

    if (tiempo_transcurrido2 >= tiempo_animacion2) {
        // Cambiar al siguiente frame
        frame_actual2 = (frame_actual2 + 1) % frames_totales2;
        tiempo_transcurrido2 = 0;  // Reiniciar el tiempo transcurrido
    }

    // Dibujar el frame actual escalado en la pantalla
    al_draw_scaled_bitmap(
        capsula,
        frame_actual2 * ancho_frame2, 0, // Coordenadas del origen del frame actual
        ancho_frame2, alto_frame2,       // Dimensiones del frame original
        x, y,                        // Posición en la pantalla donde se dibuja
        ancho_frame2 * escala2, alto_frame2 * escala2, // Dimensiones escaladas
        0                                // Sin flags adicionales
    );
    if (capsu->y >= plataforma_y - plataforma_altura / 2.0 && capsu->x >= plataforma_x - plataforma_anchura - (5.0 * escaladoX) / 2.0 && capsu->x <= plataforma_x + plataforma_anchura / 2.0 && capsu->y <= 950 * escaladoY1) {
        capsu->x = 19999999;
        capsulacaida = true;
        tipocapsula = capsu->tipo;
    }
    else {
        capsu->y = capsu->y + 3;
    }
}

void dibujarCapsulas() {
    ptrcapsulas actual = listacapsula;
    ptrcapsulas anterior = nullptr;
    while (actual != nullptr) {
        if (actual->x == 19999999) {  // Comparación correcta con ==
            if (anterior == nullptr) {
                listacapsula = actual->siguiente;  // Eliminar el primer nodo
            }
            else {
                anterior->siguiente = actual->siguiente;  // Eliminar un nodo intermedio
            }
            ptrcapsulas bloque_a_eliminar = actual;  // Guardar el bloque para eliminarlo
            ptrpoderes nuevo = crearPoder(bloque_a_eliminar->tipo, al_get_time());
            agregarPoderes(nuevo);
            actual = actual->siguiente;  // Avanzar al siguiente bloque
            delete bloque_a_eliminar;  // Liberar la memoria del bloque eliminado
        }
        else {
            capsulasdibujadas(actual);  // Dibujar solo si no fue eliminada
            anterior = actual;  // Mover el puntero 'anterior'
            actual = actual->siguiente;  // Mover al siguiente nodo
        }
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
            if (rand() % 6 == 2) {
                ptrcapsulas nuevo = crearCapsula(((bloque_a_eliminar->plataformax + bloque_a_eliminar->plataformaanchura) / 2), bloque_a_eliminar->plataformaaltura, bloque_a_eliminar->tipodebloque);
                agregarCapsulas(nuevo);
            }
            delete bloque_a_eliminar; // Liberar la memoria del bloque eliminado
        }
        else {
            anterior = bloque; // Actualizar el bloque anterior solo si no se eliminó
            bloque = bloque->siguiente; // Avanzar al siguiente bloque
        }
    }
}

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
        if (actual->x - actual->radio < anchurainicio + 44 * escaladoX1 || actual->x + actual->radio > anchurafinal - 44 * escaladoX1) {
            actual->velocidadX *= -1; // Rebotar en la pared izquierda o derecha
        }
        if (actual->y - actual->radio < 95 * escaladoY1) {
            actual->velocidadY *= -1; // Rebotar en la parte superior
        }

        manejarcolision(actual);

        // Colisión con la plataforma
        if (actual->y + actual->radio >= plataforma_y - plataforma_altura / 2.0 && actual->x >= plataforma_x - plataforma_anchura / 2.0 && actual->x <= plataforma_x + plataforma_anchura / 2.0) {
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
void liberarCapsulas() {
    while (listacapsula != nullptr) {
        ptrcapsulas Aux = listacapsula;
        listacapsula = listacapsula->siguiente;
        delete Aux;
    }
}
void liberarPoderes() {
    while (listapoderes != nullptr) {
        ptrpoderes Aux = listapoderes;
        listapoderes = listapoderes->siguiente;
        delete Aux;
    }
}


void velocidadbola(int mas) {
    ptrbola aux = listabola;
    if (mas == 0) {
        while (aux != nullptr) {
            if (aux->velocidadX > 0) {
                aux->velocidadX = aux->velocidadX - 3;
            }
            else {
                aux->velocidadX = aux->velocidadX + 3;
            }
            if (aux->velocidadY > 0) {
                aux->velocidadY = aux->velocidadY - 3;
            }
            else {
                aux->velocidadY = aux->velocidadY + 3;
            }
            aux = aux->siguiente;
        }
    }
    else if (mas == 1) { //Devolver a la normalidad
        while (aux != nullptr) {
            if (aux->velocidadX > 0) {
                aux->velocidadX = aux->velocidadX + 3;
            }
            else {
                aux->velocidadX = aux->velocidadX - 3;
            }
            if (aux->velocidadY > 0) {
                aux->velocidadY = aux->velocidadY + 3;
            }
            else{
                aux->velocidadY = aux->velocidadY - 3;
            }
            aux = aux->siguiente;
        }
    }
    else if (mas == 3) {
        while (aux != nullptr) {
            if (aux->velocidadX > 0) {
                aux->velocidadX = aux->velocidadX - 2;
            }
            else {
                aux->velocidadX = aux->velocidadX + 2;
            }
            if (aux->velocidadY > 0) {
                aux->velocidadY = aux->velocidadY - 2;
            }
            else {
                aux->velocidadY = aux->velocidadY + 2;
            }
            aux = aux->siguiente;
        }
    }
    else if (mas == 2) {
        while (aux != nullptr) {
            if (aux->velocidadX > 0) {
                aux->velocidadX = aux->velocidadX + 2;
            }
            else {
                aux->velocidadX = aux->velocidadX - 2;
            }
            if (aux->velocidadY > 0) {
                aux->velocidadY = aux->velocidadY + 2;
            }
            else {
                aux->velocidadY = aux->velocidadY - 2;
            }
            aux = aux->siguiente;
        }
    }
}

void lanzarBolaDesdeOtra() {
    float velocidadX = (4.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0f))) * escaladoX1;
    float velocidadX2 = -1 * ((4.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0f))) * escaladoX1);
    float velocidadY = -5.0f * escaladoY1; // Lanza la bola hacia arriba
    ptrbola nuevabola = crearBola(listabola->x, listabola->y, velocidadX, listabola->velocidadY, 10.0f);
    ptrbola nuevabola2 = crearBola(listabola->x, listabola->y, velocidadX2, velocidadY, 10.0f);
    agregarBola(nuevabola); // 10.0f es el radio de la bola
    agregarBola(nuevabola2);
}

void lanzarBolaDesdeTODAS() {
    ptrbola aux = listabola;
    while (aux != nullptr) {
        float velocidadX = (4.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0f))) * escaladoX1;
        float velocidadX2 = -1 * ((4.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0f))) * escaladoX1);
        float velocidadY = -5.0f * escaladoY1; // Lanza la bola hacia arriba
        ptrbola nuevabola = crearBola(aux->x, aux->y, velocidadX, aux->velocidadY, 10.0f);
        ptrbola nuevabola2 = crearBola(aux->x, aux->y, velocidadX2, velocidadY, 10.0f);
        agregarBola(nuevabola); // 10.0f es el radio de la bola
        agregarBola(nuevabola2);
        aux = aux->siguiente;
    }
}

void manejarPoderes() {
    ptrpoderes aux = listapoderes;
    ptrpoderes anterior = nullptr; // Puntero para mantener el nodo anterior

    while (aux != nullptr) {
        // Verifica si ha pasado el tiempo del poder
        if (al_get_time() - aux->tiempo >= 5.0) {
            // Restaura los valores de la plataforma si tipo == 1
            if (aux->tipo == 1) {
                plataforma_x += 30; // Restaura la posición de la plataforma
                plataforma_anchura -= 30; // Restaura la anchura de la plataforma
                velocidad = velocidad + 5;
            }
            if (aux->tipo == 3) {
                plataforma_x -= 40; // Restaura la posición de la plataforma
                plataforma_anchura += 40; // Restaura la anchura de la plataforma
                velocidad = velocidad - 10;
            }
            if (aux->tipo == 4) {
                velocidadbola(1);
            }
            if (aux->tipo == 5) {
                velocidadbola(3);
            }
            // Eliminar el nodo aux
            if (anterior != nullptr) {
                anterior->siguiente = aux->siguiente; // Salta el nodo que se va a eliminar
            }
            else {
                listapoderes = aux->siguiente; // Si es el primer nodo, actualiza la cabeza de la lista
            }
            ptrpoderes temp = aux; // Guarda el nodo a eliminar
            aux = aux->siguiente; // Avanza al siguiente nodo
            delete temp; // Libera la memoria del nodo eliminado
        }
        else {
            // Si no ha pasado el tiempo, solo avanzamos
            if (al_get_time() - aux->tiempo <= 5 && aux->usado == 0) {
                if (aux->tipo == 1) {
                    plataforma_x -= 30;
                    plataforma_anchura += 30;
                    velocidad = velocidad - 5;
                    aux->usado = 1; // Cambia a usado correctamente
                }
                else if (aux->tipo == 2) {
                    vidas = vidas + 2;
                    aux->usado = 1;
                }
                else if (aux->tipo == 3) {
                    plataforma_x += 40;
                    plataforma_anchura -= 40;
                    velocidad = velocidad + 10;
                    aux->usado = 1;
                }
                else if (aux->tipo == 4) {
                    velocidadbola(0);
                    aux->usado = 1;
                }
                else if (aux->tipo == 5) {
                    velocidadbola(2);
                    aux->usado = 1;
                }
                else if (aux->tipo == 6) {
                    lanzarBolaDesdeOtra();
                    aux->usado = 1;
                }
                else if (aux->tipo == 7) {
                    lanzarBolaDesdeTODAS();
                    aux->usado = 1;
                }
            }
            anterior = aux; // Actualiza el puntero anterior
            aux = aux->siguiente; // Avanza al siguiente nodo
        }
    }
}
////////////////////////////////////////////////////PALETA////////////////////////////////////////////////////
void paleta(ALLEGRO_EVENT_QUEUE*& coladeeventos, ALLEGRO_EVENT& evento, ALLEGRO_FONT* fuente) {
    ALLEGRO_BITMAP* imagenPlataforma = al_load_bitmap("sprites/nave/normal.png");
    if (!imagenPlataforma) {
        // Manejar el error si la imagen no se carga
        al_draw_filled_rectangle(plataforma_x - plataforma_anchura / 2.0, plataforma_y - plataforma_altura / 2.0, plataforma_x + plataforma_anchura / 2.0, plataforma_y + plataforma_altura / 2.0, colorplataforma);
        al_draw_filled_rectangle(plataforma_x - plataforma_anchura / 2.0 + 15 * escaladoY1, plataforma_y - plataforma_altura / 2.0 - 5 * escaladoY1, plataforma_x - plataforma_anchura / 2.0 + 30 * escaladoY1, plataforma_y + plataforma_altura / 2.0 + 5 * escaladoY1, colorplataforma2);
        al_draw_filled_rectangle(plataforma_x + plataforma_anchura / 2.0 - 15 * escaladoY1, plataforma_y - plataforma_altura / 2.0 - 5 * escaladoY1, plataforma_x + plataforma_anchura / 2.0 - 30 * escaladoY1, plataforma_y + plataforma_altura / 2.0 + 5 * escaladoY1, colorplataforma2);
    }
    else {
        // Dibujar la imagen escalada a las dimensiones del primer rectángulo
        al_draw_scaled_bitmap(imagenPlataforma, 0, 0, al_get_bitmap_width(imagenPlataforma), al_get_bitmap_height(imagenPlataforma), plataforma_x - plataforma_anchura / 2.0, 
        plataforma_y - plataforma_altura / 2.0, plataforma_anchura, plataforma_altura,  0);                                     
    }
    if (!bolaLanzada) {
        al_draw_filled_circle(plataforma_x, plataforma_y - plataforma_altura / 2.0, 10.0f, al_map_rgb(255, 0, 0)); // Color rojo para la bola
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
    if (plataforma_x - plataforma_anchura / 2.0 < anchurainicio + 30 * escaladoX) {
        plataforma_x = (anchurainicio + plataforma_anchura / 2.0) + 30 * escaladoX;
    }
    else if (plataforma_x + plataforma_anchura / 2.0 > anchurafinal - 30 * escaladoX) {
        plataforma_x = (anchurafinal - plataforma_anchura / 2.0) - 30 * escaladoX;
    }
    if (bolaLanzada == true) {
        actualizarBolas(plataforma_x, plataforma_y, plataforma_anchura, plataforma_altura);
    }
    dibujarBolas();
    dibujarBloques();
}

////////////////////////////////////////////////////VIDAS////////////////////////////////////////////////////
bool quedanvidas() {
    if (vidas <= 0) {
        return false;
    }
    return true;
}

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
        al_draw_text(fuente, al_map_rgb(255, 255, 255), 445 * escaladoX1, 300 * escaladoY1, 0, "Haz Perdido Una Vida");
        plataforma_x = ((anchurainicio + anchurafinal) / 2.0) * escaladoX1;
        plataforma_y = (1000 - 50) * escaladoY1;
        plataforma_anchura = 180 * escaladoX1; 
        plataforma_altura = 20 * escaladoY1; 
        velocidad = 60 * escaladoX1;        
        if (al_get_time() - tiempoMensaje >= 5.0f) {
            mostrarmensaje = false; // Oculta el mensaje
            juegoPerdido = false;
        }
    }
}
////////////////////////////////////////////////////VIDAS////////////////////////////////////////////////////
void abrircompuerta() {
    ALLEGRO_BITMAP* abierto = al_load_bitmap("sprites/backgrounds/10.png");

    int total_frames = 6;
    int frame_height = al_get_bitmap_height(abierto) / total_frames;
    int frame_width = al_get_bitmap_width(abierto);

    // Control de frames
    if (contador_frame >= velocidad_frame) {
        frame++;
        contador_frame = 0;
    }
    else {
        contador_frame++;
    }

    if (frame >= total_frames) {
        frame = 0;
    }

    int frameX = 0;
    int frameY = frame * frame_height;

    // Dibujar el bitmap escalado
    al_draw_scaled_bitmap(abierto, frameX, frameY, frame_width, frame_height,
        536 * escaladoX, 49.5 * escaladoY,
        frame_width * (3.9 * escaladoX), frame_height * (4.06 * escaladoY), 0);
}

//tipocapsula
void iniciarnivel1(ALLEGRO_FONT* fuente, ALLEGRO_TIMER* temporizador_bola, ALLEGRO_EVENT_QUEUE* coladeeventos, ALLEGRO_EVENT& evento) {


    ALLEGRO_BITMAP* imagen = al_load_bitmap("sprites/backgrounds/1.png");
    if (!imagen) {
        al_draw_filled_rectangle(anchurainicio, 50 * escaladoY1, anchurafinal, 1000 * escaladoY1, al_map_rgb(0, 0, 155));
    }
    else 
    {
        al_draw_scaled_bitmap(imagen, 0, 0, al_get_bitmap_width(imagen), al_get_bitmap_height(imagen), anchurainicio, 50 * escaladoY1, anchurafinal - anchurainicio, 
            (1000 * escaladoY1) - (50 * escaladoY1), 0);
    }

    al_draw_text(fuente, al_map_rgb(255, 255, 255), 1350 * escaladoX1, 300 * escaladoY1, 0, "Puntos");
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 1350 * escaladoX1, 350 * escaladoY1, 0, to_string(puntos).c_str());
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 1350 * escaladoX1, 400 * escaladoY1, 0, "Vidas");
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 1350 * escaladoX1, 450 * escaladoY1, 0, to_string(vidas).c_str());
    if (juegoPerdido && vidas > 0 && !mostrarmensaje) {
        vidas = vidas - 1;
        hazperdidounavida();
    }
    else if (!mostrarmensaje){
        paleta(coladeeventos, evento, fuente);
    }
    if (juegoPerdido || !iniciadoprimeravez) {
        liberarBolas();
        liberarBloques();
        liberarPoderes();
        liberarCapsulas();
        int x1 = 480;
        int x2 = 630;
        int x3 = 780;
        int x4 = 930;
        int x5 = 1080;
        int y1 = 200;
        int y2 = 250;
        int multiplicador = 0;
        for (int i = 0; i < 4; i++) {
            ptrbloques nuevo = crearBloque(x1 * escaladoX1, (y1 + (100*multiplicador)) * escaladoY1, (x1+100) * escaladoX1, (y2 + (100 * multiplicador)) * escaladoY1, 1 + rand() % 7, 3, al_map_rgb(rand()% 175 + 80, rand() % 175 + 80, rand() % 175 + 80));
            ptrbloques nuevo1 = crearBloque(x2 * escaladoX1, (y1 + (100 * multiplicador)) * escaladoY1, (x2 + 100) * escaladoX1, (y2 + (100 * multiplicador)) * escaladoY1, 1 + rand() % 7, 3, al_map_rgb(rand() % 175 + 80, rand() % 175 + 80, rand() % 175 + 80));
            ptrbloques nuevo3 = crearBloque(x4 * escaladoX1, (y1 + (100 * multiplicador)) * escaladoY1, (x4 + 100) * escaladoX1, (y2 + (100 * multiplicador)) * escaladoY1, 1 + rand() % 7, 3, al_map_rgb(rand() % 175 + 80, rand() % 175 + 80, rand() % 175 + 80));
            ptrbloques nuevo4 = crearBloque(x5 * escaladoX1, (y1 + (100 * multiplicador)) * escaladoY1, (x5 + 100) * escaladoX1, (y2 + (100 * multiplicador)) * escaladoY1, 1 + rand() % 7, 3, al_map_rgb(rand() % 175 + 80, rand() % 175 + 80, rand() % 175 + 80));
            agregarBloques(nuevo);
            agregarBloques(nuevo1);
            agregarBloques(nuevo3);
            agregarBloques(nuevo4);
            multiplicador++;
        }

        ptrbloques nuevo2 = crearBloque(x3 * escaladoX1, (y1 + (100 * 0)) * escaladoY1, (x3 + 100) * escaladoX1, (y2 + (100 * 0)) * escaladoY1, 1 + rand() % 7, 3, al_map_rgb(rand() % 175 + 80, rand() % 175 + 80, rand() % 175 + 80));
        ptrbloques nuevo33 = crearBloque(x3 * escaladoX1, (y1 + (100 * 3)) * escaladoY1, (x3 + 100) * escaladoX1, (y2 + (100 * 3)) * escaladoY1, 1 + rand() % 7, 3, al_map_rgb(rand() % 175 + 80, rand() % 175 + 80, rand() % 175 + 80));
        ptrbloques nuevos44 = crearBloque(x3 * escaladoX1, (y1 + (150)) * escaladoY1, (x3 + 100) * escaladoX1, (y2 + (150)) * escaladoY1, 1 + rand() % 7, 333333333, al_map_rgb(155, 155, 155));
        agregarBloques(nuevo2);
        agregarBloques(nuevo33);
        agregarBloques(nuevos44);

        x1 = 380;
        x2 = 1180;
        y1 = 150;
        y2 = 200;
        x3 = 480;
        multiplicador = 1;
        for (int i = 0; i < 3; i++) {
            ptrbloques nuevos = crearBloque(x1 * escaladoX1, (y1 + (100 * multiplicador)) * escaladoY1, (x1 + 100) * escaladoX1, (y2 + (100 * multiplicador)) * escaladoY1, 1 + rand() % 7, 333333333, al_map_rgb(155, 155, 155));
            ptrbloques nuevos1 = crearBloque(x2 * escaladoX1, (y1 + (100 * multiplicador)) * escaladoY1, (x2 + 100) * escaladoX1, (y2 + (100 * multiplicador)) * escaladoY1, 1 + rand() % 7, 333333333, al_map_rgb(155, 155, 155));
            agregarBloques(nuevos);
            agregarBloques(nuevos1);
            multiplicador++;
        }
        ptrbloques nuevos4 = crearBloque(((x3)+(150 * 1)) * escaladoX1, (y1 + (500)) * escaladoY1, ((x3 + 100) + (150 * 1)) * escaladoX1, (y2 + (500)) * escaladoY1, 1 + rand() % 7, 333333333, al_map_rgb(155, 155, 155));
        ptrbloques nuevos5 = crearBloque(((x3)+(150 * 3)) * escaladoX1, (y1 + (500)) * escaladoY1, ((x3 + 100) + (150 * 3)) * escaladoX1, (y2 + (500)) * escaladoY1, 1 + rand() % 7, 333333333, al_map_rgb(155, 155, 155));
        agregarBloques(nuevos4);
        agregarBloques(nuevos5);
        iniciadoprimeravez = true;
    }
    dibujarCapsulas();
    manejarPoderes();
    //abrircompuerta();
    perdidavida(fuente);
}
