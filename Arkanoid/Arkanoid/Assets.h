#pragma once
#include <vector> 
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "Nivel1.h"
#include "Assets.h"
#include "Arkanoid.h"
#include "resource.h"
#include <string>
#include <iostream>
#include <windows.h>
extern bool juegoPerdido;
extern int vidas;
extern bool iniciadoprimeravez;
ALLEGRO_COLOR colorarcoiris(float tiempo);
void dibujarBolas();
bool quedanvidas();
void iniciarnivel1(ALLEGRO_FONT* fuente, ALLEGRO_TIMER* temporizador_bola, ALLEGRO_EVENT_QUEUE* coladeeventos, ALLEGRO_EVENT& evento);