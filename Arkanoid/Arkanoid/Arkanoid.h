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
extern ALLEGRO_FONT* fuente;
extern ALLEGRO_COLOR color_actual;
extern float escaladoX;
extern float escaladoY;
extern float tiempo;
extern int puntos;
extern int puntosreales;
extern int frame;
#include "resource.h"
float devolverx();
float devolvery();