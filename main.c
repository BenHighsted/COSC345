/**
 *
 * COSC345 'Dungeon Fall' Assignment 2 2019
 * Ben Highsted, Matthew Neil, Jasmine Hindson
 *
 * Last Edited: Tue Jul 16 15:55:24 NZST 2019
 *
 */

#include <stdio.h>//standard includes
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <SDL2/SDL.h>//SDL2 includes
#include <SDL2/SDL_image.h>
#include "SDL2_ttf/SDL_ttf.h"
#include "SDL2_mixer/SDL_mixer.h"

#define TICK_INTERVAL 15

static const int width = 1000;//width and height of the window
static const int height = 700;
static Uint32 next_time;

/** Method works out how much time is left and determines how much to slow down for the current PC
 *  https://www.libsdl.org/release/SDL-1.2.15/docs/html/guidetimeexamples.html
 *  Essentially caps the fps to keep it consistent across multiple computers
 **/
Uint32 time_left(void)
{
    Uint32 now;
    now = SDL_GetTicks();
    if(next_time <= now){
        return 0;
    }else{
        return next_time - now;
    }
}

/** Method destroys and frees the surface/textures passed to it. Mainly used to save line space. **/
void destroyAndFree (SDL_Surface* surface, SDL_Texture* texture)
{
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

int main(int argc, char **argv)
{
    next_time = SDL_GetTicks() + TICK_INTERVAL;//determines how fast the program should run
    
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    Mix_Music *music = NULL;
    
    //Load the music
    //music = Mix_LoadMUS("");
    
    //If there was a problem loading the music
    //if(music == NULL)
    //{
    //    return false;
    //}
    
    Mix_PlayMusic(music, -1);
    
    SDL_Init(SDL_INIT_VIDEO);//Initialize SDL and creates a window/renderer
    SDL_Window *window = SDL_CreateWindow("Dungeon Fall", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    TTF_Init();
    IMG_Init(IMG_INIT_JPG);//Image initialisation code
    
    TTF_Font *font = NULL, *font2 = NULL;//creates font variables and assigns them to fonts out of /Library/Fonts.
    font = TTF_OpenFont("/Library/Fonts/Georgia.ttf", 100);//Only works for MACOS currently.
    font2 = TTF_OpenFont("/Library/Fonts/Arial.ttf", 100);
    if(!font) {
        printf("Failed to find font");
        return false;
    }
    
    /** Variable Declarations **/
    int score = 00000001, attempts = 0, counterAdd = 5, x = 475, y = 250;
    int startx = 300, starty = 530, fallx = 300, fally = -100, wallLeftX = -800, wallRightX = 800;
    int mode = 0, pos = 0, position = 185;
    
    float menuCounter = 0, fallCounter = 0, backCounter = 0, backCounter2 = 0;
    float counter = 0.0, counter2 = 0.0, counter3 = 0.0, counter4 = 0.0;
    float obx = (rand() % 520)+200, ob2x = (rand() % 550)+200, ob3x = (rand() % 500)+200;
    float ob4x = (rand() % 520)+200, ob5x = (rand() % 550)+200, ob6x = (rand() % 500)+200;
    int speed = 4, oby = 810, ob2y = 1110;
    float wallPos1 = 100, wallPos2 = 300, wallPos3 = 500, wallPos4 = 700;
    
    bool add = true, fall = false, first_loop = false, running = true;
    bool setup = true, switchModes = false, complete = false;
    bool move_left = false, move_right = false, move_up = false, move_down = false;
    bool main_menu = true, game_over = false, rightmove = false;
    bool character_description = false, main_menu_screen = true;
    bool sprite1 = true, sprite2 = false, sprite3 = false;
    
    char *array = (char *) malloc(64);
    char *array2 = (char *) malloc(64);
    
    /** Rectangle Declarations **/
    SDL_Rect wall_rect = {-800, 0, 1000, 700};
    SDL_Rect wall_rect2 = wall_rect;
    wall_rect2.x = 800;
    SDL_Rect wall_rect1_2 = wall_rect;
    wall_rect1_2.y = 700;
    SDL_Rect wall_rect2_2 = wall_rect2;
    wall_rect2_2.y = 700;
    SDL_Rect Message_rect = {505, 5, 35, 20};
    SDL_Rect Message_rect2 = {450, 0, 50, 30};
    SDL_Rect MainMenu_rect = {300, 400, 400, 50};
    SDL_Rect MainMenu_rect2 = {250, 300, 500, 50};
    SDL_Rect Title_rect = {150, 50, 700, 300};
    SDL_Rect Title_background_rect = {0, 0, 1000, 700};
    SDL_Rect Title_background_rect2 = {0, 699, 1000, 700};
    SDL_Rect background_rect = {0, 0, 1000, 700};
    SDL_Rect background_rect2 = {0, 699, 1000, 700};
    SDL_Rect sprite1_rect = {200, 340, 130, 130};
    SDL_Rect sprite_rect2 = {350, 50, 220, 220};
    SDL_Rect sprite2_rect = {400, 340, 130, 130};
    SDL_Rect sprite3_rect = {600, 340, 130, 130};
    
    SDL_Event event;//starts SDL event
    
    //Background and wall textures
    SDL_Surface *image = IMG_Load("bricks.png");//From: https://www.deviantart.com/skazdal/art/Rock-bricks-texture-670434391
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_Surface *background = IMG_Load("bricksBackground.png");//From: http://pixelartmaker.com/art/31b17490e7ef5d8
    SDL_Texture *backTexture = SDL_CreateTextureFromSurface(renderer, background);
    //Border and Sprite textures
    SDL_Surface *borderSurface = IMG_Load("border.png");//Following images created by Jasmine Hindson
    SDL_Texture *border = SDL_CreateTextureFromSurface(renderer, borderSurface);
    SDL_Surface *spriteStraightTexture = IMG_Load("sprite1straight.png");
    SDL_Texture *sprite1StraightTexture = SDL_CreateTextureFromSurface(renderer, spriteStraightTexture);
    SDL_Surface *sprite2Straight = IMG_Load("sprite2straight.png");
    SDL_Texture *sprite2StraightTexture = SDL_CreateTextureFromSurface(renderer, sprite2Straight);
    SDL_Surface *sprite3Straight = IMG_Load("sprite3straight.png");
    SDL_Texture *sprite3StraightTexture = SDL_CreateTextureFromSurface(renderer, sprite3Straight);
    SDL_Surface *spriteTexture = IMG_Load("sprite1falling.png");
    SDL_Texture *sprite1FallingTexture = SDL_CreateTextureFromSurface(renderer, spriteTexture);
    SDL_Surface *sprite2Falling = IMG_Load("sprite2falling.png");
    SDL_Texture *sprite2FallingTexture = SDL_CreateTextureFromSurface(renderer, sprite2Falling);
    SDL_Surface *sprite3Falling = IMG_Load("Sprite3falling.png");
    SDL_Texture *sprite3FallingTexture = SDL_CreateTextureFromSurface(renderer, sprite3Falling);
    //Fireball/Object textures
    SDL_Surface *fireRed = IMG_Load("fireRed.png");//Following images from https://stealthix.itch.io/animated-fires
    SDL_Texture *fireRedTexture = SDL_CreateTextureFromSurface(renderer, fireRed);
    SDL_Surface *fireBlue = IMG_Load("fireBlue.png");
    SDL_Texture *fireBlueTexture = SDL_CreateTextureFromSurface(renderer, fireBlue);
    SDL_Surface *fireGreen = IMG_Load("fireGreen.png");
    SDL_Texture *fireGreenTexture = SDL_CreateTextureFromSurface(renderer, fireGreen);
    //Other
    SDL_Surface* surfaceMessage;
    SDL_Surface* surfaceMessage2;
    SDL_Texture* Message;
    SDL_Texture* Message2;
    
    while(running){
        
        SDL_Delay(time_left());//used to run at the same speed on every device
        next_time += TICK_INTERVAL;
        
        if(event.type == SDL_QUIT) {//if the user clicks the red X to quit
            running = false;
        }
        if(main_menu == true){//if the game state is in the main menu
            if(character_description == true) {//if the user presses enter on a character
                SDL_RenderClear(renderer);
                SDL_Color textColor = {255, 255, 255};
                
                SDL_Rect nameRect = {300, 280, 370, 50};//basic rectangles
                SDL_Rect descriptionRect = {50, 350, 890, 30};
                SDL_Rect descriptionRect2 = {50, 390, 870, 30};
                SDL_Rect descriptionRect3 = {50, 430, 890, 30};
                SDL_Rect descriptionRect4 = {50, 470, 760, 30};
                SDL_Rect infoRect = {370, 550, 200, 30};
                SDL_Texture *sprite = NULL, *nameTexture = NULL;
                SDL_Surface *nameSurface = NULL, *infoSurface = TTF_RenderText_Solid(font, "Press backspace", textColor);
                SDL_Surface *descriptionSurface = NULL, *descriptionSurface2 = NULL;
                SDL_Surface *descriptionSurface3 = NULL, *descriptionSurface4 = NULL;
                SDL_Texture *descriptionTexture = NULL, *descriptionTexture2 = NULL;
                SDL_Texture *descriptionTexture3 = NULL, *descriptionTexture4 = NULL;
                
                SDL_Texture* infoTexture = SDL_CreateTextureFromSurface(renderer, infoSurface);
                //decides which text and sprite needs to be displayed, then displays it.
                if (sprite1 == true) {
                    sprite = sprite1StraightTexture;
                    nameSurface = TTF_RenderText_Solid(font, "Andrew the Lecturer", textColor);
                    nameTexture = SDL_CreateTextureFromSurface(renderer, nameSurface);
                    descriptionSurface = TTF_RenderText_Solid(font, "After seeing all the student evaluations for COSC345 that he had to get through, Andrew sat down at his desk and", textColor);
                    descriptionSurface2 = TTF_RenderText_Solid(font, "began to read. The night was getting darker and darker and Andrew's eyes began to flicker as he started to fall", textColor);
                    descriptionSurface3 = TTF_RenderText_Solid(font, "asleep. As his head hit the desk, his dreams convince him that he is falling down a hole. What a nightmare!", textColor);
                    descriptionSurface4 = TTF_RenderText_Solid(font, "Can you help Andrew down the hole so that he can survive this nightmare?", textColor);
                    descriptionTexture = SDL_CreateTextureFromSurface(renderer, descriptionSurface);
                    descriptionTexture2 = SDL_CreateTextureFromSurface(renderer, descriptionSurface2);
                    descriptionTexture3 = SDL_CreateTextureFromSurface(renderer, descriptionSurface3);
                    descriptionTexture4 = SDL_CreateTextureFromSurface(renderer, descriptionSurface4);
                } else if (sprite2 == true) {
                    sprite = sprite2StraightTexture;
                    nameSurface = TTF_RenderText_Solid(font, "Big Chungus the Bunny", textColor);
                    nameTexture = SDL_CreateTextureFromSurface(renderer, nameSurface);
                    descriptionSurface = TTF_RenderText_Solid(font, "Big Chungus, being the leader of his pack, led his bunny troop into the wilderness. He comes across a group of people", textColor);
                    descriptionSurface2 = TTF_RenderText_Solid(font, "who have set camp for the night. He sees from a distance that they have a massive pile of carrots placed in a box", textColor);
                    descriptionSurface3 = TTF_RenderText_Solid(font, "by the tents. DINNER! He calls his troops to charge, but as they hop towards the food, Big Chungas falls down a hole.", textColor);
                    descriptionSurface4 = TTF_RenderText_Solid(font, "Can you help Big Chungas down the hole so he can get back to his bunny troop?", textColor);
                    descriptionTexture = SDL_CreateTextureFromSurface(renderer, descriptionSurface);
                    descriptionTexture2 = SDL_CreateTextureFromSurface(renderer, descriptionSurface2);
                    descriptionTexture3 = SDL_CreateTextureFromSurface(renderer, descriptionSurface3);
                    descriptionTexture4 = SDL_CreateTextureFromSurface(renderer, descriptionSurface4);
                } else if (sprite3 == true) {
                    sprite = sprite3StraightTexture;
                    nameSurface = TTF_RenderText_Solid(font, "Matthew the Gamer", textColor);
                    nameTexture = SDL_CreateTextureFromSurface(renderer, nameSurface);
                    descriptionSurface = TTF_RenderText_Solid(font, "After a long dusty night of playing Fortnite, Matthew realised that his COSC345 tutorial was in 10mins.", textColor);
                    descriptionSurface2 = TTF_RenderText_Solid(font, "He quickly bolts out the door with his shoes half on, and runs to class. On his way there he trips", textColor);
                    descriptionSurface3 = TTF_RenderText_Solid(font, "over his shoe lace! Silly Matthew. He starts rolling across the ground until he falls down a hole. Oh no.", textColor);
                    descriptionSurface4 = TTF_RenderText_Solid(font, "Can you help Matthew down the hole so he can get back to his COSC345 tutorial?", textColor);
                    descriptionTexture = SDL_CreateTextureFromSurface(renderer, descriptionSurface);
                    descriptionTexture2 = SDL_CreateTextureFromSurface(renderer, descriptionSurface2);
                    descriptionTexture3 = SDL_CreateTextureFromSurface(renderer, descriptionSurface3);
                    descriptionTexture4 = SDL_CreateTextureFromSurface(renderer, descriptionSurface4);
                }
                SDL_RenderCopy(renderer, backTexture, NULL, &background_rect2);//copies all the textures into the renderer
                SDL_RenderCopy(renderer, backTexture, NULL, &Title_background_rect);
                SDL_RenderCopy(renderer, backTexture, NULL, &Title_background_rect2);
                SDL_RenderCopy(renderer, sprite, NULL, &sprite_rect2);
                SDL_RenderCopy(renderer, nameTexture, NULL, &nameRect);
                SDL_RenderCopy(renderer, descriptionTexture, NULL, &descriptionRect);
                SDL_RenderCopy(renderer, descriptionTexture2, NULL, &descriptionRect2);
                SDL_RenderCopy(renderer, descriptionTexture3, NULL, &descriptionRect3);
                SDL_RenderCopy(renderer, descriptionTexture4, NULL, &descriptionRect4);
                SDL_RenderCopy(renderer, infoTexture, NULL, &infoRect);
                SDL_RenderPresent(renderer);
                destroyAndFree(nameSurface, nameTexture);
                destroyAndFree(descriptionSurface, descriptionTexture);
                destroyAndFree(descriptionSurface2, descriptionTexture2);
                destroyAndFree(descriptionSurface3, descriptionTexture3);
                destroyAndFree(descriptionSurface4, descriptionTexture4);
                destroyAndFree(infoSurface, infoTexture);
                
            } else if (main_menu_screen == true) {//standard main menu screen
                counterAdd = 45;
                Title_rect.x = 150;
                Title_rect.y = 2;
                Title_rect.w = 700;
                Title_rect.h = 300;//resets counter speed and the title rect location
                
                menuCounter += 2;//used to animate stuff on the main screen
                fallCounter += 2;
                backCounter += 6;
                backCounter2 += 6;
                
                if(fallCounter >= 500) {//a timer to decide when the character should fall in the background
                    fall = true;
                    fallCounter = 0;
                }
                
                if(starty == 530) {//decides if we need to add or minus from the 'start' animation
                    add = false;
                }else if(starty == 510) {
                    add = true;
                }
                
                SDL_Rect falling_rect = {fallx, fally, 100, 100};//falling character
                SDL_Rect border_rect = {position, 325, 160, 160};//border around current sprite
                
                SDL_Color textColor = {255, 255, 255};
                SDL_RenderClear(renderer);
                
                SDL_RenderCopy(renderer, backTexture, NULL, &background_rect2);//copys created stuff to the renderer
                SDL_RenderCopy(renderer, backTexture, NULL, &Title_background_rect);
                SDL_RenderCopy(renderer, backTexture, NULL, &Title_background_rect2);
                SDL_RenderCopy(renderer, sprite1FallingTexture, NULL, &falling_rect);
                SDL_RenderCopy(renderer, sprite1StraightTexture, NULL, &sprite1_rect);
                SDL_RenderCopy(renderer, sprite2StraightTexture, NULL, &sprite2_rect);
                SDL_RenderCopy(renderer, sprite3StraightTexture, NULL, &sprite3_rect);
                SDL_RenderCopy(renderer, border, NULL, &border_rect);
                
                SDL_Surface* titleMessage = TTF_RenderText_Solid(font, "DUNGEON FALL", textColor);//creates the title
                SDL_Texture* titleMessage2 = SDL_CreateTextureFromSurface(renderer, titleMessage);
                SDL_RenderCopy(renderer, titleMessage2, NULL, &Title_rect);
                
                SDL_Rect MainMenu_rect = {startx, starty, 400, 50};
                SDL_Rect pick_rect = {190, 265, 300, 40};
                SDL_Rect enter_rect = {position+6, 490, 150, 20};
                
                if(add){//animates the 'start' text
                    if(menuCounter == 4) {
                        starty += 1;
                        menuCounter = 0;
                    }
                }else{
                    if(menuCounter == 4) {
                        starty -= 1;
                        menuCounter = 0;
                    }
                }
                
                if(fall){//moves the falling character
                    fally += 3;
                }
                
                if(position == 185) {//determines which sprite is going to be used
                    sprite1 = true;
                    sprite2 = false;
                    sprite3 = false;
                } else if (position == 385) {
                    sprite1= false;
                    sprite2 = true;
                    sprite3 = false;
                } else if(position == 585){
                    sprite1 = false;
                    sprite2 = false;
                    sprite3 = true;
                }
                
                SDL_Surface* mainMessage = TTF_RenderText_Solid(font, "Press space bar to start!", textColor);
                SDL_Texture* mainMessage2 = SDL_CreateTextureFromSurface(renderer, mainMessage);
                SDL_Surface* pickMessage = TTF_RenderText_Solid(font, "Select a character:", textColor);
                SDL_Texture* pickMessage2 = SDL_CreateTextureFromSurface(renderer, pickMessage);
                SDL_Surface* enterSurface = TTF_RenderText_Solid(font, "Press enter to see info", textColor);
                SDL_Texture* enterTexture = SDL_CreateTextureFromSurface(renderer, enterSurface);
                
                SDL_RenderCopy(renderer, pickMessage2, NULL, &pick_rect);
                SDL_RenderCopy(renderer, mainMessage2, NULL, &MainMenu_rect);
                SDL_RenderCopy(renderer, enterTexture, NULL, &enter_rect);
                
                SDL_RenderPresent(renderer);//draws the menu
                
                destroyAndFree(pickMessage, pickMessage2);
                destroyAndFree(titleMessage, titleMessage2);
                destroyAndFree(mainMessage, mainMessage2);
                destroyAndFree(enterSurface, enterTexture);
            }
            
            while(SDL_PollEvent(&event)) {
                if(event.key.keysym.sym == SDLK_SPACE){//start game
                    if(main_menu_screen == true) {
                        main_menu = false;
                        first_loop = true;
                    }
                }
                if(event.key.keysym.sym == SDLK_RETURN) {//look at current character
                    main_menu_screen = false;
                    character_description = true;
                }
                if(event.key.keysym.sym == SDLK_BACKSPACE) {//return from current character
                    character_description = false;
                    main_menu_screen = true;
                }
                if(event.key.keysym.sym == SDLK_RIGHT) {//move between characters
                    if(position != 585) {
                        position += 100;
                    }
                }
                if(event.key.keysym.sym == SDLK_LEFT) {//move between characters
                    if(position != 185) {
                        position -= 100;
                    }
                }
            }
        }else{
            if(game_over == true){//if you lose
                counterAdd = 45;
                
                SDL_Color textColor = {255, 255, 255};
                SDL_Rect game_over_back2 = {0, 0, 1000, 700};
                SDL_Rect GameOver_rect = {230, 50, 500, 100};
                SDL_Rect Attempts_rect = {380, 150, 200, 50};
                SDL_Rect Attempts_rect2 = {585, 150, 20, 45 };
                SDL_Rect Score_rect = {380, 210, 90, 40};
                SDL_Rect Score_rect2 = {480, 210, 100, 30};
                
                sprintf(array, "%d", score);//gets current score
                
                move_left = false;
                move_right = false;
                move_up = false;
                move_down = false;
                
                SDL_RenderClear(renderer);
                
                SDL_RenderCopy(renderer, backTexture, NULL, &game_over_back2);
                
                SDL_Surface* gameOverSurface = TTF_RenderText_Solid(font, "Game Over!", textColor);
                SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
                
                SDL_Surface* mainMessage = TTF_RenderText_Solid(font, "Press space bar to try again!", textColor);
                SDL_Texture* mainMessage2 = SDL_CreateTextureFromSurface(renderer, mainMessage);
                SDL_Surface* mainMessage3 = TTF_RenderText_Solid(font, "Press 'M' to return to the menu!", textColor);
                SDL_Texture* mainMessage4 = SDL_CreateTextureFromSurface(renderer, mainMessage3);
                
                SDL_Surface* attemptMessage = TTF_RenderText_Solid(font, "Attempt number: ", textColor);
                SDL_Texture* attemptMessage2 = SDL_CreateTextureFromSurface(renderer, attemptMessage);
                SDL_Surface* attemptCount = TTF_RenderText_Solid(font, array2, textColor);
                SDL_Texture* attemptCount2 = SDL_CreateTextureFromSurface(renderer, attemptCount);
                
                SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, "Score: ", textColor);
                SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
                SDL_Surface* scoreSurface1 = TTF_RenderText_Solid(font, array, textColor);
                SDL_Texture* scoreTexture1 = SDL_CreateTextureFromSurface(renderer, scoreSurface1);
                
                SDL_RenderCopy(renderer, gameOverTexture, NULL, &GameOver_rect);
                SDL_RenderCopy(renderer, attemptMessage2, NULL, &Attempts_rect);
                SDL_RenderCopy(renderer, attemptCount2, NULL, &Attempts_rect2);
                SDL_RenderCopy(renderer, mainMessage2, NULL, &MainMenu_rect);
                SDL_RenderCopy(renderer, mainMessage4, NULL, &MainMenu_rect2);
                SDL_RenderCopy(renderer, scoreTexture, NULL, &Score_rect);
                SDL_RenderCopy(renderer, scoreTexture1, NULL, &Score_rect2);
                
                SDL_RenderPresent(renderer);
                
                destroyAndFree(gameOverSurface, gameOverTexture);
                destroyAndFree(mainMessage, mainMessage2);
                destroyAndFree(mainMessage3, mainMessage4);
                destroyAndFree(attemptMessage, attemptMessage2);
                destroyAndFree(attemptCount, attemptCount2);
                destroyAndFree(scoreSurface, scoreTexture);
                destroyAndFree(scoreSurface1, scoreTexture1);
                
                while(SDL_PollEvent(&event)) {
                    if(event.key.keysym.sym == SDLK_SPACE) {//start again
                        game_over = false;
                        first_loop = true;
                    }
                    if(event.key.keysym.sym == SDLK_m) {//returns to main menu
                        game_over = false;
                        main_menu = true;
                    }
                }
                if(game_over == false) {//reset variables
                    score = 0;
                    x = 475;
                    y = 250;
                    oby = 600;
                    ob2y = 900;
                    mode = 0;
                    wallLeftX = -800;
                    wallRightX = 800;
                    wall_rect.x = wallLeftX;
                    wall_rect2.x = wallRightX;
                    wall_rect1_2.x = wallLeftX;
                    wall_rect2_2.x = wallRightX;
                }
            } else {//no menus currently running, start and run game
                SDL_Texture* spriteFallingTexture = NULL;
                if(sprite1 == true) {//selects the correct sprite that the player chose
                    spriteFallingTexture = sprite1FallingTexture;
                } else if (sprite2 == true) {
                    spriteFallingTexture = sprite2FallingTexture;
                } else if (sprite3 == true) {
                    spriteFallingTexture = sprite3FallingTexture;
                }
                
                SDL_RenderClear(renderer);
                
                float count = 0;
                fallx = 475;
                fally = -100;
                
                if(first_loop) {//reset wall locations if its the first run through the loop
                    wall_rect.y = -700;
                    wall_rect2.y = -700;
                    wall_rect1_2.y = -700;
                    wall_rect2_2.y = -700;
                }
                while(first_loop) {//starts the falling animation
                    SDL_Delay(time_left());
                    next_time += TICK_INTERVAL;
                    
                    SDL_RenderClear(renderer);
                    count += 10;
                    //code below moves the walls and background for the first loop animation
                    SDL_Color textColor = {255, 255, 255};
                    counter = 0;
                    background_rect.y -= 3;
                    if(background_rect.y <= -700) {
                        background_rect.y = 700;
                    }
                    SDL_RenderCopy(renderer, backTexture, NULL, &background_rect);
                    counter2 = 0;
                    background_rect2.y -= 3;
                    Title_rect.y -= 3;
                    if(background_rect2.y <= -700) {
                        background_rect2.y = 700;
                    }
                    SDL_RenderCopy(renderer, backTexture, NULL, &background_rect2);
                    wall_rect.y -= 3;
                    if(wall_rect.y <= -700) {
                        wall_rect.y = 700;
                    }
                    SDL_RenderCopy(renderer, texture, NULL, &wall_rect);
                    wall_rect2.y -= 3;
                    counter3 = 0;
                    if(wall_rect2.y <= -700) {
                        wall_rect2.y = 700;
                    }
                    SDL_RenderCopy(renderer, texture, NULL, &wall_rect2);
                    wall_rect1_2.y -= 3;
                    if(wall_rect1_2.y <= -1400) {
                        wall_rect1_2.y = 700;
                    }
                    SDL_RenderCopy(renderer, texture, NULL, &wall_rect1_2);
                    counter4 = 0;
                    wall_rect2_2.y -= 3;
                    if(wall_rect2_2.y <= -1400) {
                        wall_rect2_2.y = 700;
                    }
                    SDL_RenderCopy(renderer, texture, NULL, &wall_rect2_2);
                    
                    SDL_Surface* titleMessage = TTF_RenderText_Solid(font, "DUNGEON FALL", textColor);
                    SDL_Texture* titleMessage2 = SDL_CreateTextureFromSurface(renderer, titleMessage);
                    SDL_RenderCopy(renderer, titleMessage2, NULL, &Title_rect);
                    
                    if(count > 1500) {//enough time has passed, start the character falling
                        SDL_Rect falling_rect = {fallx, fally, 80, 80};
                        SDL_RenderCopy(renderer, spriteFallingTexture, NULL, &falling_rect);
                        if(fally != 250) {
                            fally += 2;
                        }
                        if(fally >= 250) {
                            first_loop = false;
                            counter3 = 0.0;
                            counter4 = 0.0;
                        }
                    }
                    SDL_RenderPresent(renderer);
                    destroyAndFree(titleMessage, titleMessage2);
                }//animation ends here
                
                while(SDL_PollEvent(&event)){
                    //determines which way to move the character based on the key presses
                    if(event.type == SDL_KEYUP) {
                        if(event.key.keysym.sym == SDLK_RIGHT) {
                            move_right = false;
                        }
                        if(event.key.keysym.sym == SDLK_LEFT) {
                            move_left = false;
                        }
                        if(event.key.keysym.sym == SDLK_DOWN) {
                            move_down = false;
                        }
                        if(event.key.keysym.sym == SDLK_UP) {
                            move_up = false;
                        }
                    }
                    //code below makes movements smooth, if a key is held down
                    if(event.type == SDL_KEYDOWN) {
                        if(event.key.keysym.sym == SDLK_RIGHT) {
                            move_right = true;
                            rightmove = true;
                        }
                        if(event.key.keysym.sym == SDLK_LEFT) {
                            move_left = true;
                            rightmove = false;
                        }
                        if(event.key.keysym.sym == SDLK_DOWN) {
                            move_down = true;
                        }
                        if(event.key.keysym.sym == SDLK_UP) {
                            move_up = true;
                        }
                    }
                }
                if(move_left) {//if left is pressed, move object left and so on.
                    x -= 5;
                }
                if(move_right) {
                    x += 5;
                }
                if(move_up) {
                    y -= 5;
                }
                if(move_down) {
                    y += 5;
                }
                
                counter += counterAdd;//increment counters
                counter2 += counterAdd;
                counter3 += counterAdd;
                counter4 += counterAdd;
                if(score == 2000 || score == 4000 || score == 6000 || score == 8000 || score == 10000) {
                    counterAdd += 6;//if score has increased by a good amount increase falling speed
                }
                if(x < wallLeftX + 1000 || x > wallRightX - 80) {//detects if a player has hit a wall
                    game_over = true;
                }
                
                if(y > 550 || y < 0) {//detects if a player goes to high or low
                    game_over = true;
                }
                //moves the background
                if(counter >= 100) {
                    background_rect.y -= 2;
                    counter = 0;
                }
                if(background_rect.y <= -700) {
                    background_rect.y = 700;
                }
                SDL_RenderCopy(renderer, backTexture, NULL, &background_rect);
                if(counter2 >= 100) {
                    counter2 = 0;
                    background_rect2.y -= 2;
                }
                if(background_rect2.y <= -700) {
                    background_rect2.y = 700;
                }
                SDL_RenderCopy(renderer, backTexture, NULL, &background_rect2);
                
                //draws a rectangle and decides which way it should face
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
                SDL_Rect rect = {x, y, 80, 80};
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_RenderFillRect(renderer, &rect);
                if(rightmove) {
                    float angle = 0.0;
                    SDL_Point center = {8, 8};
                    SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
                    
                    SDL_RenderCopyEx(renderer, spriteFallingTexture, NULL , &rect, angle, &center, flip);
                }else{
                    SDL_RenderCopy(renderer, spriteFallingTexture, NULL, &rect);
                }
                
                //moves side walls
                if(counter3 > 50) {
                    wall_rect.y -= 4;
                }
                if(wall_rect.y <= -700) {
                    wall_rect.y = 700;
                }
                SDL_RenderCopy(renderer, texture, NULL, &wall_rect);
                
                if(counter3 > 50) {
                    wall_rect2.y -= 4;
                    counter3 = 0;
                }
                if(wall_rect2.y <= -700) {
                    wall_rect2.y = 700;
                }
                SDL_RenderCopy(renderer, texture, NULL, &wall_rect2);
                
                if(counter4 > 50) {
                    wall_rect1_2.y -= 4;
                }
                if(wall_rect1_2.y <= -700) {
                    wall_rect1_2.y = 700;
                }
                SDL_RenderCopy(renderer, texture, NULL, &wall_rect1_2);
                
                if(counter4 > 50) {
                    counter4 = 0;
                    wall_rect2_2.y -= 4;
                }
                if(wall_rect2_2.y <= -700) {
                    wall_rect2_2.y = 700;
                }
                
                SDL_RenderCopy(renderer, texture, NULL, &wall_rect2_2);
                
                score++;//adds 1 to the players current score
                
                if(score % 1000 == 0) {//swap between walls closing in and objects
                    switchModes = true;
                }
                if(mode == 0) {//mode 0 so objects
                    oby -= speed;
                    ob2y -= speed;
                    
                    if(!switchModes) {
                        if(x + 20 >= obx && x <= obx + 100) {//collision detection for fireballs
                            if(oby + 50 >= y && oby <= y + 50) {
                                game_over = true;
                            }
                        }
                        if(x + 20 >= ob2x && x <= ob2x + 70) {//collision detection for fireballs
                            if(oby + 50 >= y && oby <= y + 50) {
                                game_over = true;
                            }
                        }
                        
                        if(x + 20 >= ob3x && x <= ob3x + 100) {//collision detection for fireballs
                            if(oby + 50 >= y && oby <= y + 50) {
                                game_over = true;
                            }
                        }
                        
                        if(oby < -50) {//resets the objects to be used again once off screen
                            obx = (rand() % 520) + 200;
                            ob2x = (rand() % 550) + 200;
                            ob3x = (rand() % 500) + 200;
                            oby = 600;
                        }
                        
                        if(x + 20 >= ob4x && x <= ob4x + 100) {//collision detection for fireballs
                            if(ob2y + 50 >= y && ob2y <= y + 50) {
                                game_over = true;
                            }
                        }
                        if(x + 20 >= ob5x && x <= ob5x + 70) {//collision detection for fireballs
                            if(ob2y + 50 >= y && ob2y <= y + 50) {
                                game_over = true;
                            }
                        }
                        
                        if(x + 20 >= ob6x && x <= ob6x + 100) {//collision detection for fireballs
                            if(ob2y + 50 >= y && ob2y <= y + 50) {
                                game_over = true;
                            }
                        }
                        
                        if(ob2y < -50) {//resets the objects to be used again once off screen
                            ob4x = (rand() % 520) + 200;
                            ob5x = (rand() % 550) + 200;
                            ob6x = (rand() % 500) + 200;
                            ob2y = 600;
                        }
                    } else {
                        if(oby <= -50 && ob2y <= -50) {
                            mode = 1;
                            oby = 600;
                            ob2y = 900;
                            setup = true;
                            switchModes = false;
                        }
                    }
                    //draws the objects
                    SDL_Rect rect2 = {obx, oby, 100, 50};
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                    SDL_RenderCopy(renderer, fireBlueTexture, NULL, &rect2);
                    SDL_Rect rect3 = {ob2x, oby, 70, 50};
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                    SDL_RenderCopy(renderer, fireGreenTexture, NULL, &rect3);
                    SDL_Rect rect4 = {ob3x, oby, 100, 50};
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                    SDL_RenderCopy(renderer, fireRedTexture, NULL, &rect4);
                    SDL_Rect rect5 = {ob4x, ob2y, 100, 50};
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                    SDL_RenderCopy(renderer, fireBlueTexture, NULL, &rect5);
                    SDL_Rect rect6 = {ob5x, ob2y, 70, 50};
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                    SDL_RenderCopy(renderer, fireGreenTexture, NULL, &rect6);
                    SDL_Rect rect7 = {ob6x, ob2y, 100, 50};
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                    SDL_RenderCopy(renderer, fireRedTexture, NULL, &rect7);
                    
                } else {//mode 2, so walls
                    int speed2 = 2;//sets the speed the walls will move
                    if(!switchModes) {
                        if(setup) {//setup moves the walls in
                            wallLeftX += speed2;
                            wallRightX -= speed2;
                            wall_rect.x = wallLeftX;
                            wall_rect2.x = wallRightX;
                            wall_rect1_2.x = wallLeftX;
                            wall_rect2_2.x = wallRightX;
                            
                            if(wallLeftX >= -625) {//walls are moved in so finish setup
                                setup = false;
                                complete = false;
                                pos = rand() % 4;
                            }
                        } else {//setup finished so start wall stage
                            wall_rect.x = wallLeftX;
                            wall_rect2.x = wallRightX;
                            wall_rect1_2.x = wallLeftX;
                            wall_rect2_2.x = wallRightX;
                            //now that stage has begun, walls will randomly generate a position and move to that position
                            if(pos == 0) {
                                if(!complete) {
                                    
                                    wallLeftX -= speed2;
                                    wallRightX -= speed2;
                                    
                                    if(wallLeftX <= wallPos1 - 1000) {
                                        pos = rand() % 4;//randomly picks a position
                                    }
                                }
                            } else if(pos == 1) {//position 1, so move to that location
                                if(!complete) {
                                    if(wallLeftX < wallPos2 - 1000) {
                                        wallLeftX += speed2;
                                        wallRightX += speed2;
                                        if(wallLeftX >= wallPos2 - 1000) {
                                            pos = rand() % 4;
                                        }
                                    } else {
                                        wallLeftX -= speed2;
                                        wallRightX -= speed2;
                                        if(wallLeftX <= wallPos2 - 1000) {
                                            pos = rand() % 4;
                                        }
                                    }
                                }
                            } else if(pos == 2) {
                                if(!complete) {
                                    if(wallLeftX < wallPos3 - 1000) {
                                        wallLeftX += speed2;
                                        wallRightX += speed2;
                                        if(wallLeftX >= wallPos3 - 1000) {
                                            pos = rand() % 4;
                                        }
                                    } else {
                                        wallLeftX -= speed2;
                                        wallRightX -= speed2;
                                        if(wallLeftX <= wallPos3 - 1000) {
                                            pos = rand() % 4;
                                        }
                                    }
                                }
                            } else {
                                if(!complete){
                                    wallLeftX += speed2;
                                    wallRightX += speed2;
                                    if(wallLeftX >= wallPos4 - 1000) {
                                        //complete = true;
                                        pos = rand() % 4;
                                    }
                                }
                            }
                        }
                        
                        if(score % 1000 == 0) {//score reached 1000 more than the last stage, so switch
                            switchModes = true;
                        }
                        
                    } else {//switches back to objects by returning walls to their original position
                        if(wallLeftX > -800) {
                            wallLeftX -= speed2;
                        } else if (wallLeftX < -800) {
                            wallLeftX += speed2;
                        }
                        if(wallRightX < 800) {
                            wallRightX += speed2;
                        } else if (wallRightX > 800) {
                            wallRightX -= speed2;
                        }
                        if(wallLeftX <= -800 && wallRightX >= 800) {
                            wallLeftX = -800;
                            wallRightX = 800;
                            mode = 0;
                            switchModes = false;
                        }
                        wall_rect.x = wallLeftX;
                        wall_rect2.x = wallRightX;
                        wall_rect1_2.x = wallLeftX;
                        wall_rect2_2.x = wallRightX;
                    }
                }
                if(game_over) {//game over, so store current attempts in an array
                    attempts++;
                    sprintf(array2, "%d", attempts);
                }
                
                SDL_Color textColor = {255, 255, 255};
                
                sprintf(array, "%d", score);//gets the current score and displays it at the top of the screen
                surfaceMessage = TTF_RenderText_Solid(font2, array, textColor);
                surfaceMessage2 = TTF_RenderText_Solid(font, "Score: ", textColor);
                
                Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
                Message2 = SDL_CreateTextureFromSurface(renderer, surfaceMessage2);
                
                SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
                SDL_RenderCopy(renderer, Message2, NULL, &Message_rect2);
                
                SDL_RenderPresent(renderer);
                
                destroyAndFree(surfaceMessage, Message);
                destroyAndFree(surfaceMessage2, Message2);
            }
        }
    }
    
    free(array);
    
    TTF_CloseFont(font);//releases font resource
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    
    IMG_Quit();//SDL Quits
    TTF_Quit();
    SDL_Quit();
    
    return 0;
}
