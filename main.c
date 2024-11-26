//gcc main.c -o main $(sdl2-config --cflags --libs)
//./main

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define GRAVITY_FORCE 5
#define POS_GROUND 500

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define CHARACTER_WIDTH 50
#define CHARACTER_HEIGHT 70
#define CHARACTER_SPEED 4

#define GROUND_WIDTH 5000
#define GROUND_HEIGHT 200
#define GROUND_SPEED 2

#define NB_OBSTACLES 1

//Type Character
typedef struct {
    int x;
	int y;
    int width;
	int height;
    int way_x;
    int way_y;
	bool isJumping;
	int jumpForce;
	SDL_Color color;

} Character;


//CREATION D'UN TABLEAU DE key CLAVIER
#define LENGTH_TAB 5

//Type key
typedef struct {
	SDL_Keycode SDL_KEY;
	bool isPressed;
	bool isReleased;
} Key;

Key keyTab[] = {{SDLK_LEFT, false, false},
				{SDLK_UP, false, false},
				{SDLK_RIGHT, false, false},
				{SDLK_DOWN, false, false},
				{SDLK_ESCAPE, false, false}
			   };

//Fonction qui regarde si chaque touche a été pressed/released et actualise le keyTab en conséquence 
void updateKeyTab(SDL_Event event, Key * keyTab){

	//Parcours des touches du tableau
	for(int i=0; i<LENGTH_TAB; i++){
		if(event.key.keysym.sym == keyTab[i].SDL_KEY){

			if(event.type == SDL_KEYDOWN){
				keyTab[i].isPressed = true;
				keyTab[i].isReleased = false;
			}
			else if(event.type == SDL_KEYUP){
				keyTab[i].isPressed = false;
				keyTab[i].isReleased = true;
			}
		}
		else{
			keyTab[i].isPressed = false;
			keyTab[i].isReleased = false;
		}
	}
}

void processingKeyTab(Key * keyTab, Character * character, bool * running){

	for(int i=0; i<LENGTH_TAB; i++){
		Key currentKey = keyTab[i];

		//Si on va à gauche
		if(currentKey.SDL_KEY == SDLK_LEFT && currentKey.isPressed){
			character->x -= CHARACTER_SPEED;
		}

		//Si on va à droite
		else if(currentKey.SDL_KEY == SDLK_RIGHT && currentKey.isPressed){
			character->x += CHARACTER_SPEED;
		}

		//Si on canalise le jump
		else if(currentKey.SDL_KEY == SDLK_UP && currentKey.isPressed){
			if(character->isJumping == false){
				if(character->jumpForce <= 20){
					character->jumpForce += 2;
				}
			}
		}

		//Si on relache le jump
		else if(currentKey.SDL_KEY == SDLK_UP && currentKey.isReleased){
			character->isJumping = true;
		}

		//Si on descend
		else if(currentKey.SDL_KEY == SDLK_DOWN && currentKey.isPressed){
			character->y += CHARACTER_SPEED;
		}

		//Si on presse esc pour quitter le jeu
		else if(currentKey.SDL_KEY == SDLK_ESCAPE && (currentKey.isPressed || currentKey.isReleased)){
			(*running) = false;
		}
	}
}

//Type Ground
typedef struct {
    int x;
	int y;
    int width;
	int height;
	SDL_Color color;

} Ground;

//Type Tree
typedef struct {
    int x;
	int y;
    int width;
	int height;
	SDL_Color color;

} Tree;

//Type Stage
typedef struct {
    int numStage; 
	Ground ground;
	Tree obstacles[NB_OBSTACLES];

} Stage;


//Fonction appelée à chaque frame, elle actualise la fenetre en faisant apparaitre les éléments
void resfreshElements(SDL_Renderer * renderer, Character character, Stage stage){

	//Créer la fenetre
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	//Faire apparaitre le character
	SDL_SetRenderDrawColor(renderer, character.color.r, character.color.g, character.color.b, character.color.a);
	SDL_Rect rectCharacter = { character.x, character.y, character.width, character.height }; // x, y, largeur, hauteur
	SDL_RenderFillRect(renderer, &rectCharacter);

	//Faire apparaitre le ground
	SDL_SetRenderDrawColor(renderer, stage.ground.color.r, stage.ground.color.g, stage.ground.color.b, stage.ground.color.a);
	SDL_Rect rectGround = { stage.ground.x, stage.ground.y, stage.ground.width, stage.ground.height }; // x, y, largeur, hauteur
	SDL_RenderFillRect(renderer, &rectGround);

	//Faire apparaitre l'arbre
	SDL_SetRenderDrawColor(renderer, stage.obstacles[0].color.r, stage.obstacles[0].color.g, stage.obstacles[0].color.b, stage.obstacles[0].color.a);
	SDL_Rect rectTree = { stage.obstacles[0].x, stage.obstacles[0].y, stage.obstacles[0].width, stage.obstacles[0].height }; // x, y, largeur, hauteur
	SDL_RenderFillRect(renderer, &rectTree);
}


//Fonction qui fait bouger automatiquement le ground
void movePositionGround(Stage * stage){

	stage->ground.x -= GROUND_SPEED;

	//déplacement de l'arbre
	stage->obstacles[0].x -= GROUND_SPEED;

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

	//On déclanche le saut
	if(character->isJumping){
		character->jumpForce -= 1;
		character->y -= (CHARACTER_SPEED + character->jumpForce);
	}

	//Si le character est au sol, isJumping est false
	if(character->y + CHARACTER_HEIGHT >= POS_GROUND){
		character->isJumping = false;
		character->y = POS_GROUND - CHARACTER_HEIGHT;
	}

	//Si le character est en saut (phase descendante), on applique la gravité
	if(character->isJumping && character->jumpForce == 0){
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
	if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
		switch (event.key.keysym.sym) {

			case SDLK_LEFT:
				character->x -= CHARACTER_SPEED;
				break;

			case SDLK_RIGHT:
				character->x += CHARACTER_SPEED;
				break;

			case SDLK_UP:

				//Si on relache le jump
				if(event.type == SDL_KEYUP){
					character->jumpForce += 1;
					character->isJumping = true;
				}

				//Si on canalise le jump
				if(event.type == SDL_KEYDOWN){
					if(character->isJumping == false){
						if(character->jumpForce <= 4){
							character->jumpForce += 11;
						}
					}
				}

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

	//Initialisation du stage

	SDL_Color colorGround = {0, 255, 20, 255};
	Ground ground = {0, POS_GROUND, GROUND_WIDTH, GROUND_HEIGHT, colorGround};  // x, y, width, height, color

	SDL_Color colorTree = {100, 255, 0, 255};
	Tree tree = {2000, 420, 30, 80, colorTree};  // x, y, width, height, color

	Stage stage = {1, ground, {tree}};

	//Initialisation du character
	SDL_Color colorCharacter = {255, 0, 0, 255};
	Character character = {0, 0, CHARACTER_WIDTH, CHARACTER_HEIGHT, 1, 1, true, 0, colorCharacter};  // x, y, width, height, way_x, way_y, isJumping, jumpForce, color

	while(running){
		
		while(SDL_PollEvent(&event)){
			verifyEventQuit(event, &running);
			//verifyEventKeyPressed(event, &running, &character);
			updateKeyTab(event, keyTab);
			
		}
		processingKeyTab(keyTab, &character, &running);
		movePositionCharacter(&character);

		movePositionGround(&stage);
		resfreshElements(renderer, character, stage);

		SDL_RenderPresent(renderer);
	}
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	return 0;
}