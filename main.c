//gcc main.c -o main $(sdl2-config --cflags --libs)
//./main

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

const int GRAVITY_FORCE = 5;
const int POS_GROUND = 500;

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;

const int CHARACTER_WIDTH = 50;
const int CHARACTER_HEIGHT = 70;
const int CHARACTER_SPEED = 40;

const int MAP_WIDTH = 5000;
const int MAP_HEIGHT = 200;
const int MAP_SPEED = 2;

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

//Type Map
typedef struct {
    int x;
	int y;
    int width;
	int height;
	SDL_Color color;

} Map;

//Type Tree
typedef struct {
    int x;
	int y;
    int width;
	int height;
	SDL_Color color;

} Tree;


//Fonction appelée à chaque frame, elle actualise la fenetre en faisant apparaitre les éléments
void update(SDL_Renderer * renderer, Map map, Character character, Tree tree){

	//Créer la fenetre
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	//Faire apparaitre le character
	SDL_SetRenderDrawColor(renderer, character.color.r, character.color.g, character.color.b, character.color.a);
	SDL_Rect rectCharacter = { character.x, character.y, character.width, character.height }; // x, y, largeur, hauteur
	SDL_RenderFillRect(renderer, &rectCharacter);

	//Faire apparaitre la map
	SDL_SetRenderDrawColor(renderer, map.color.r, map.color.g, map.color.b, map.color.a);
	SDL_Rect rectMap = { map.x, map.y, map.width, map.height }; // x, y, largeur, hauteur
	SDL_RenderFillRect(renderer, &rectMap);

	//Faire apparaitre l'arbre
	SDL_SetRenderDrawColor(renderer, tree.color.r, tree.color.g, tree.color.b, tree.color.a);
	SDL_Rect rectTree = { tree.x, tree.y, tree.width, tree.height }; // x, y, largeur, hauteur
	SDL_RenderFillRect(renderer, &rectTree);
}


//Fonction qui fait bouger automatiquement la map
void movePositionMap(Map * map, Tree * tree){

	map->x -= MAP_SPEED;

	//déplacement de l'arbre
	tree->x -= MAP_SPEED;

}


//Fonction qui fait bouger automatiquement le character
void movePositionCharacter(Character * character){
/*
	// changement de direction lorsque le character sort de la fenetre
	if(character->x > WINDOW_WIDTH - CHARACTER_WIDTH){
		character->way_x = 2;
	}
	if(character->y > WINDOW_HEIGHT - CHARACTER_HEIGHT){
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
		character->x += CHARACTER_SPEED;
	}
	else{
		character->x -= CHARACTER_SPEED;
	}

	if(character->way_y == 1){
		character->y += CHARACTER_SPEED;
	}
	else{
		character->y -= CHARACTER_SPEED;
	}
*/

	if(character->y < POS_GROUND - CHARACTER_HEIGHT){
		character->y += GRAVITY_FORCE;
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
				character->x -= CHARACTER_SPEED;
				break;

			case SDLK_RIGHT:
				character->x += CHARACTER_SPEED;
				break;

			case SDLK_UP:
				character->y -= CHARACTER_SPEED;
				break;

			case SDLK_DOWN:
				character->y += CHARACTER_SPEED;
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

	//Initialisation de la Map
	SDL_Color colorMap = {0, 255, 20, 255};
	Map map = {0, POS_GROUND, MAP_WIDTH, MAP_HEIGHT, colorMap};  // x, y, width, height, color

	SDL_Color colorTree = {100, 255, 0, 255};
	Tree tree = {2000, 420, 30, 80, colorTree};  // x, y, width, height, color

	//Initialisation du character
	SDL_Color colorCharacter = {255, 0, 0, 255};
	Character character = {0, 0, CHARACTER_WIDTH, CHARACTER_HEIGHT, 1, 1, colorCharacter};  // x, y, width, height, way_x, way_y, color

	while(running){
		
		while(SDL_PollEvent(&event)){
			verifyEventQuit(event, &running);
			verifyEventKeyPressed(event, &running, &character);
		}

		movePositionCharacter(&character);

		movePositionMap(&map, &tree);
		update(renderer, map, character, tree);

		SDL_RenderPresent(renderer);
	}
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	return 0;
}