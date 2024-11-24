//gcc main.c -o main $(sdl2-config --cflags --libs)
//./main

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;

const int RECT_WIDTH = 200;
const int RECT_HEIGHT = 150;


void update(SDL_Renderer * renderer, int x, int y){

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	// Couleur pour le rectangle (rouge)
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_Rect rect = { x, y, RECT_WIDTH, RECT_HEIGHT }; // x, y, largeur, hauteur
	SDL_RenderFillRect(renderer, &rect);

}

bool verifyEventQuit(SDL_Event event, bool running){
	if(event.type == SDL_QUIT){
		running = false;
	}
	return running;
}

void movePosition(int * x, int * y, int * way_x, int * way_y){

	// changement de direction lorsque le rectangle sort de l'écran
	if((*x) > WINDOW_WIDTH - RECT_WIDTH){
		(*way_x) = 2;
	}

	if((*y) > WINDOW_HEIGHT - RECT_HEIGHT){
		(*way_y) = 2;
	}

	if((*x) < 0){
		(*way_x) = 1;
	}

	if((*y) < 0){
		(*way_y) = 1;
	}

	//Avancement de x et y dans la direction indiquée
	if((*way_x) == 1){
		(*x) += 2;
	}
	else{
		(*x) -= 2;
	}

	if((*way_y) == 1){
		(*y) += 2;
	}
	else{
		(*y) -= 2;
	}
        
}



int main(int argc, const char * argv[]) {

	//Initialisation de la fenetre
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window;
	SDL_Renderer *renderer;
	window = SDL_CreateWindow("Projet C !!!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL); //SDL_WINDOW_FULLSCREEN

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	bool running = true;
	SDL_Event event;

	//Initialisation des positions
	int x = 0;
	int y = 0;
	//Initialisation des directions (1 pour avant, 2 pour recul)
	int way_x = 1;
	int way_y = 1;
	
	while(running){
		
		while(SDL_PollEvent(&event)){
			running = verifyEventQuit(event, running);
		}

		movePosition(&x, &y, &way_x, &way_y);

		update(renderer, x, y);

		SDL_RenderPresent(renderer);
	}
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	return 0;
}