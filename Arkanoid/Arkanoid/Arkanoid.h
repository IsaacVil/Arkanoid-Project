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
#include "Carga y Guardado (AVL).h"
#include "Nivel1.h"
#include "Assets.h"
#include "Arkanoid.h"
#include "resource.h"
#include <string>
#include <iostream>
#include <windows.h>
extern ALLEGRO_FONT* fuente;
extern ALLEGRO_COLOR color_actual;
extern bool nivel1;
extern bool nivel2;
extern bool nivel3;
extern float escaladoX;
extern float escaladoY;
extern float tiempo;
extern int puntos;
extern int puntosreales;
extern int frame;
extern int frames;
extern int random1;
extern int random2;
extern int random3;
extern int bolastotal;
extern int bolasre;
extern int bolasper;
extern int objetosgolpeados;
extern abbptr jugadoractual;
float devolverx();
float devolvery();