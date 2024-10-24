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
extern bool juegoperdidoseriop1;
extern bool juegoperdidoseriop2;
extern bool ia_activada_p3;
void vidasfull();
void iniciarduojugadores(ALLEGRO_FONT* fuente, ALLEGRO_FONT* fuentesub, ALLEGRO_TIMER* temporizador_bola, ALLEGRO_EVENT_QUEUE* coladeeventos, ALLEGRO_EVENT& evento);