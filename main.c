//gcc main.c -o main $(sdl2-config --cflags --libs)
//./main

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;

const int RECT_WIDTH = 50;
const int RECT_HEIGHT = 70;

//Type Character
typedef struct {
    int x;
	int y;
    int width;
	int height;
    int way_x;
    int way_y;
	SDL_Color color;

} Character;


//Fonction appelée à chaque frame, elle actualise la fenetre
void updateCharacter(SDL_Renderer * renderer, Character character){

	//Créer la fenetre
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	//Faire apparaitre le character
	SDL_SetRenderDrawColor(renderer, character.color.r, character.color.g, character.color.b, character.color.a);
	SDL_Rect rect = { character.x, character.y, character.width, character.height }; // x, y, largeur, hauteur
	SDL_RenderFillRect(renderer, &rect);
}

//Fonction qui fait bouger le character
void movePositionCharacter(Character * character){

	// changement de direction lorsque le character sort de la fenetre
	if(character->x > WINDOW_WIDTH - RECT_WIDTH){
		character->way_x = 2;
	}
	if(character->y > WINDOW_HEIGHT - RECT_HEIGHT){
		character->way_y = 2;
	}
	if(character->x < 0){
		character->way_x = 1;
	}
	if(character->y < 0){
		character->way_y = 1;
	}

	//Avancement de x et y dans la direction indiquée
	if(character->way_x == 1){
		character->x += 2;
	}
	else{
		character->x -= 2;
	}

	if(character->way_y == 1){
		character->y += 2;
	}
	else{
		character->y -= 2;
	}
        
}

//Fonction qui vérifie si la fenetre a été fermée
void verifyEventQuit(SDL_Event event, bool * running){
	if(event.type == SDL_QUIT){
		(*running) = false;
	}
}

//Fonction qui vérifie les événements clavier
void verifyEventKeyPressed(SDL_Event event, bool * running, Character * character){
	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {

			case SDLK_LEFT:
				character->x -= 20;
				break;

			case SDLK_RIGHT:
				character->x += 20;
				break;

			case SDLK_UP:
				character->y -= 20;
				break;

			case SDLK_DOWN:
				character->y += 20;
				break;

			case SDLK_ESCAPE:
				(*running) = false;
				break;

			default:
				break;
		}
	}
}



int main(int argc, const char * argv[]) {

	//Initialisation de la fenetre
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window;
	SDL_Renderer *renderer;
	window = SDL_CreateWindow(" Mario is real", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL); //SDL_WINDOW_FULLSCREEN

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	bool running = true;
	SDL_Event event;

	//Initialisation du character
	SDL_Color color = {255, 0, 0, 255};
	Character character = {0, 0, RECT_WIDTH, RECT_HEIGHT, 1, 1, color};  // x, y, width, height, way_x, way_y, color

	while(running){
		
		while(SDL_PollEvent(&event)){
			verifyEventQuit(event, &running);
			verifyEventKeyPressed(event, &running, &character);
		}

		movePositionCharacter(&character);
		updateCharacter(renderer, character);

		SDL_RenderPresent(renderer);
	}
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	return 0;
}