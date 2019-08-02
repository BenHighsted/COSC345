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
    
    Mix_Chunk *fireball = NULL;//loading in the fireball sound effect
    fireball = Mix_LoadWAV("fireball.mp3");
    
    //find music, use this to load it in!
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
    int startyGameOver = 560, currentScore = 0;
    
    float menuCounter = 0, fallCounter = 0, backCounter = 0, backCounter2 = 0, menuCounterGameOver = 0;
    float counter = 0.0, counter2 = 0.0, counter3 = 0.0, counter4 = 0.0;
    float obx = (rand() % 480)+200, ob2x = (rand() % 480)+200, ob3x = (rand() % 480)+200;
    float ob4x = (rand() % 520)+200, ob5x = (rand() % 550)+200, ob6x = (rand() % 500)+200;
    int speed = 4, oby = -100, ob2y = 10000000;
    float wallPos1 = 100, wallPos2 = 300, wallPos3 = 500, wallPos4 = 700;
    
    bool add = true, fall = false, first_loop = false, running = true;
    bool setup = true, switchModes = false, complete = false;
    bool move_left = false, move_right = false, move_up = false, move_down = false;
    bool main_menu = true, game_over = false, rightmove = false;
    bool character_description = false, main_menu_screen = true;
    bool sprite1 = true, sprite2 = false, sprite3 = false;
    bool first_time = true;
    bool addGameOver = true, leaderboard = false;
    
    char *array = (char *) malloc(64);
    char *array2 = (char *) malloc(64);
    char *array3 = (char *) malloc(64);
    
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
    SDL_Rect Title_background_rect = {0, 0, 1000, 710};
    SDL_Rect Title_background_rect2 = {0, 700, 1000, 710};
    SDL_Rect background_rect = {0, 0, 1000, 710};
    SDL_Rect background_rect2 = {0, 710, 1000, 710};
    SDL_Rect sprite1_rect = {200, 370, 130, 130};
    SDL_Rect sprite_rect2 = {350, 50, 220, 220};
    SDL_Rect sprite2_rect = {400, 370, 130, 130};
    SDL_Rect sprite3_rect = {600, 370, 130, 130};
    
    //SDL_Rect source_rect = {0, 0, 10, 26};
    
    SDL_Rect source_rect_red = {0, 0, 10, 26};
    SDL_Rect source_rect_blue = {0, 0, 9, 24};
    SDL_Rect source_rect_green = {0, 0, 9, 25};
    
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
    SDL_Surface *fireRed = IMG_Load("fire-red.png");//Following images from https://stealthix.itch.io/animated-fires
    SDL_Texture *fireRedTexture = SDL_CreateTextureFromSurface(renderer, fireRed);
    SDL_Surface *fireBlue = IMG_Load("fire-blue.png");
    SDL_Texture *fireBlueTexture = SDL_CreateTextureFromSurface(renderer, fireBlue);
    SDL_Surface *fireGreen = IMG_Load("fire-green.png");
    SDL_Texture *fireGreenTexture = SDL_CreateTextureFromSurface(renderer, fireGreen);
    //Other
    SDL_Surface* surfaceMessage;
    SDL_Surface* surfaceMessage2;
    SDL_Texture* Message;
    SDL_Texture* Message2;

    SDL_Surface *image1_description = IMG_Load("andrew-description.png");
    SDL_Texture *description_texture1 = SDL_CreateTextureFromSurface(renderer, image1_description);
    SDL_Surface *image2_description = IMG_Load("bunny-description.png");
    SDL_Texture *description_texture2 = SDL_CreateTextureFromSurface(renderer, image2_description);
    SDL_Surface *image3_description = IMG_Load("matthew-description.png");
    SDL_Texture *description_texture3 = SDL_CreateTextureFromSurface(renderer, image3_description);
    
    SDL_Rect description_image = {5, 250, 990, 250};
    
    //game over screen
    SDL_Surface *leaderTitleSurface = IMG_Load("titleLeaderboard.png");
    SDL_Surface *gameOverTitleSurface = IMG_Load("titleGameOver.png");
    SDL_Surface *menuTitleSurface = IMG_Load("titleMenu.png");
    SDL_Surface *tryAgainTitleSurface = IMG_Load("titleTryAgain.png");
    SDL_Surface *attemptMessageSurface = IMG_Load("messageAttempt.png");
    SDL_Surface *scoreMessageSurface = IMG_Load("messageScore.png");
    SDL_Surface *highScoreMessageSurface = IMG_Load("messageHighScore.png");
    SDL_Texture *leaderTitleTexture = SDL_CreateTextureFromSurface(renderer, leaderTitleSurface);
    SDL_Texture *gameOverTitleTexture = SDL_CreateTextureFromSurface(renderer, gameOverTitleSurface);
    SDL_Texture *menuTitleTexture = SDL_CreateTextureFromSurface(renderer, menuTitleSurface);
    SDL_Texture *tryAgainTitleTexture = SDL_CreateTextureFromSurface(renderer, tryAgainTitleSurface);
    SDL_Texture *attemptMessageTexture = SDL_CreateTextureFromSurface(renderer, attemptMessageSurface);
    SDL_Texture *scoreMessageTexture = SDL_CreateTextureFromSurface(renderer, scoreMessageSurface);
    SDL_Texture *highScoreMessageTexture = SDL_CreateTextureFromSurface(renderer, highScoreMessageSurface);

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
                SDL_Rect infoRect = {370, 550, 200, 30};
                SDL_Texture *sprite = NULL, *nameTexture = NULL;
                SDL_Surface *nameSurface = NULL, *infoSurface = TTF_RenderText_Solid(font, "Press backspace", textColor);
                
                SDL_Texture* infoTexture = SDL_CreateTextureFromSurface(renderer, infoSurface);
                //decides which text and sprite needs to be displayed, then displays it.
                SDL_RenderCopy(renderer, backTexture, NULL, &background_rect2);//copies all the textures into the renderer
                SDL_RenderCopy(renderer, backTexture, NULL, &Title_background_rect);
                SDL_RenderCopy(renderer, backTexture, NULL, &Title_background_rect2);
                SDL_RenderCopy(renderer, nameTexture, NULL, &nameRect);
                SDL_RenderCopy(renderer, infoTexture, NULL, &infoRect);
                if (sprite1 == true) {
                    sprite = sprite1StraightTexture;
                    SDL_RenderCopy(renderer, description_texture1, NULL, &description_image);
                } else if (sprite2 == true) {
                    sprite = sprite2StraightTexture;
                    SDL_RenderCopy(renderer, description_texture2, NULL, &description_image);
                } else if (sprite3 == true) {
                    sprite = sprite3StraightTexture;
                    SDL_RenderCopy(renderer, description_texture3, NULL, &description_image);
                }
                SDL_RenderCopy(renderer, sprite, NULL, &sprite_rect2);
                SDL_RenderPresent(renderer);
                
                destroyAndFree(nameSurface, nameTexture);
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
                
                if(starty == 605) {//decides if we need to add or minus from the 'start' animation
                    add = false;
                }else if(starty == 575) {
                    add = true;
                }
                
                SDL_Rect falling_rect = {fallx, fally, 100, 100};//falling character
                SDL_Rect border_rect = {position, 355, 160, 160};//border around current sprite
                
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
                SDL_Rect pick_rect = {190, 290, 300, 40};
                SDL_Rect enter_rect = {position+6, 520, 150, 20};
                
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
                menuCounterGameOver += 2;
                
                SDL_Color textColor = {255, 255, 255};
                
                SDL_Rect leaderTitleRect = {261, 370, 500, 400};
                SDL_Rect menuTitleRect = {260, 440, 500, 400};
                SDL_Rect gameOverTitleRect = {130, -60, 750, 600};
                SDL_Rect attemptMessageRect = {280, 130, 400, 300};
                SDL_Rect scoreMessageRect = {280, 190, 400, 300};
                SDL_Rect highScoreMessageRect = {260, 250, 400, 300};
                
                SDL_Rect game_over_back2 = {0, 0, 1000, 700};
                SDL_Rect attemptsRect = {590, 165, 25, 50};
                SDL_Rect highscoreRect = {560, 300, 50, 30};
                SDL_Rect scoreRect = {530, 240, 50, 30};
                
                sprintf(array, "%d", score);//gets current score
                
                move_left = false;
                move_right = false;
                move_up = false;
                move_down = false;
                
                if(startyGameOver == 560) {//decides if we need to add or minus from the 'start' animation
                    addGameOver = false;
                }else if(startyGameOver == 530) {
                    addGameOver = true;
                }
                
                SDL_Rect tryAgainTitleRect = {265, startyGameOver, 500, 400};
                
                if(addGameOver){//animates the 'start' text
                    if(menuCounterGameOver == 4) {
                        startyGameOver += 1;
                        menuCounterGameOver = 0;
                    }
                }else{
                    if(menuCounterGameOver == 4) {
                        startyGameOver -= 1;
                        menuCounterGameOver = 0;
                    }
                }
                
                SDL_RenderClear(renderer);
                
                SDL_RenderCopy(renderer, backTexture, NULL, &game_over_back2);
                
                SDL_Surface* attemptCountSurface = TTF_RenderText_Solid(font, array2, textColor);
                SDL_Texture* attemptCountTexture = SDL_CreateTextureFromSurface(renderer, attemptCountSurface);
                
                SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, array, textColor);
                SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
                
                SDL_Surface* highScoreSurface = TTF_RenderText_Solid(font, array3, textColor);
                SDL_Texture* highScoreTexture = SDL_CreateTextureFromSurface(renderer, highScoreSurface);
                
                SDL_RenderCopy(renderer, leaderTitleTexture, NULL, &leaderTitleRect);
                SDL_RenderCopy(renderer, gameOverTitleTexture, NULL, &gameOverTitleRect);
                SDL_RenderCopy(renderer, menuTitleTexture, NULL, &menuTitleRect);
                SDL_RenderCopy(renderer, tryAgainTitleTexture, NULL, &tryAgainTitleRect);
                SDL_RenderCopy(renderer, attemptMessageTexture, NULL, &attemptMessageRect);
                SDL_RenderCopy(renderer, scoreMessageTexture, NULL, &scoreMessageRect);
                SDL_RenderCopy(renderer, highScoreMessageTexture, NULL, &highScoreMessageRect);
                
                SDL_RenderCopy(renderer, highScoreTexture, NULL, &highscoreRect);
                SDL_RenderCopy(renderer, attemptCountTexture, NULL, &attemptsRect);
                SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
                
                SDL_RenderPresent(renderer);
                
                destroyAndFree(highScoreSurface, highScoreTexture);
                destroyAndFree(attemptCountSurface, attemptCountTexture);
                destroyAndFree(scoreSurface, scoreTexture);
                
                while(SDL_PollEvent(&event)) {
                    if(event.key.keysym.sym == SDLK_SPACE) {//start again
                        game_over = false;
                        leaderboard = false;
                        first_loop = true;
                    }
                    if(event.key.keysym.sym == SDLK_m) {//returns to main menu
                        game_over = false;
                        leaderboard = false;
                        main_menu = true;
                    }
                    if(event.key.keysym.sym == SDLK_l) {//goes to leaderboard
                        game_over = false;
                        main_menu = false;
                        leaderboard = true;
                    }
                }
                
                if(game_over == false) {//reset variables
                    score = 0;
                    x = 475;
                    y = 250;
                    oby = 700;
                    ob2y = 900;
                    mode = 0;
                    wallLeftX = -800;
                    wallRightX = 800;
                    wall_rect.x = wallLeftX;
                    wall_rect2.x = wallRightX;
                    wall_rect1_2.x = wallLeftX;
                    wall_rect2_2.x = wallRightX;
                }
            } else if(leaderboard == true) {
                SDL_RenderClear(renderer);
                SDL_Rect game_over_back3 = {0, 0, 1000, 700};
                SDL_RenderCopy(renderer, backTexture, NULL, &game_over_back3);
                SDL_RenderPresent(renderer);
            } else {//no menus currently running, start and run game
                
                source_rect_red.x += 10;
                if(source_rect_red.x >= 100){
                    source_rect_red.x = 0;
                    source_rect_red.y += 26;
                }
                if(source_rect_red.y >= 156) {
                    source_rect_red.x = 0;
                    source_rect_red.y = 0;
                }
                
                source_rect_blue.x += 9;
                if(source_rect_blue.x >= 90){
                    source_rect_blue.x = 0;
                    source_rect_blue.y += 24;
                }
                if(source_rect_blue.y >= 144) {
                    source_rect_blue.x = 0;
                    source_rect_blue.y = 0;
                }
                
                source_rect_green.x += 9;
                if(source_rect_green.x >= 90){
                    source_rect_green.x = 0;
                    source_rect_green.y += 25;
                }
                if(source_rect_green.y >= 150) {
                    source_rect_green.x = 0;
                    source_rect_green.y = 0;
                }
                
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
                        
                        if(oby < -78) {//resets the objects to be used again once off screen
                            while(true){
                                obx = (rand() % 480) + 200;
                                ob2x = (rand() % 480) + 200;
                                ob3x = (rand() % 480) + 200;
                                oby = 700;
                                if(obx - ob2x > 120 || ob2x - obx > 120){
                                    if(obx - ob3x > 120 || ob3x - obx > 120) {
                                        if(ob2x - ob3x > 120 || ob3x - ob2x > 120) {
                                            break;
                                        }
                                    }
                                }
                            }
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
                        
                        if(ob2y < -78 || (first_time && oby <= 350)) {//resets the objects to be used again once off screen
                            first_time = false;
                            while(true){
                                ob4x = (rand() % 480) + 200;
                                ob5x = (rand() % 480) + 200;
                                ob6x = (rand() % 480) + 200;
                                ob2y = 700;
                                if(ob4x - ob5x > 120 || ob5x - ob4x > 120){
                                    if(ob4x - ob6x > 120 || ob6x - ob4x > 120) {
                                        if(ob5x - ob6x > 120 || ob6x - ob5x > 120) {
                                            break;
                                        }
                                    }
                                }
                            }
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
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                    
                    SDL_Rect rect_blue_1 = {obx, oby, 30, 78};
                    SDL_RenderCopy(renderer, fireBlueTexture, &source_rect_blue, &rect_blue_1);
                    SDL_Rect rect_blue_2 = {obx+30, oby, 30, 78};
                    SDL_RenderCopy(renderer, fireBlueTexture, &source_rect_blue, &rect_blue_2);
                    SDL_Rect rect_blue_3 = {obx+60, oby, 30, 78};
                    SDL_RenderCopy(renderer, fireBlueTexture, &source_rect_blue, &rect_blue_3);
                    SDL_Rect rect_blue_4 = {obx+90, oby, 30, 78};
                    SDL_RenderCopy(renderer, fireBlueTexture, &source_rect_blue, &rect_blue_4);
                    SDL_Rect rect_blue_5 = {ob4x, ob2y, 30, 78};
                    SDL_RenderCopy(renderer, fireBlueTexture, &source_rect_blue, &rect_blue_5);
                    SDL_Rect rect_blue_6 = {ob4x+30, ob2y, 30, 78};
                    SDL_RenderCopy(renderer, fireBlueTexture, &source_rect_blue, &rect_blue_6);
                    SDL_Rect rect_blue_7 = {ob4x+60, ob2y, 30, 78};
                    SDL_RenderCopy(renderer, fireBlueTexture, &source_rect_blue, &rect_blue_7);
                    SDL_Rect rect_blue_8 = {ob4x+90, ob2y, 30, 78};
                    SDL_RenderCopy(renderer, fireBlueTexture, &source_rect_blue, &rect_blue_8);
                    
                    SDL_Rect rect_green_1 = {ob2x, oby, 30, 78};
                    SDL_RenderCopy(renderer, fireGreenTexture, &source_rect_green, &rect_green_1);
                    SDL_Rect rect_green_2 = {ob2x+30, oby, 30, 78};
                    SDL_RenderCopy(renderer, fireGreenTexture, &source_rect_green, &rect_green_2);
                    SDL_Rect rect_green_3 = {ob2x+60, oby, 30, 78};
                    SDL_RenderCopy(renderer, fireGreenTexture, &source_rect_green, &rect_green_3);
                    SDL_Rect rect_green_4 = {ob2x+90, oby, 30, 78};
                    SDL_RenderCopy(renderer, fireGreenTexture, &source_rect_green, &rect_green_4);
                    SDL_Rect rect_green_5 = {ob5x, ob2y, 30, 78};
                    SDL_RenderCopy(renderer, fireGreenTexture, &source_rect_green, &rect_green_5);
                    SDL_Rect rect_green_6 = {ob5x+30, ob2y, 30, 78};
                    SDL_RenderCopy(renderer, fireGreenTexture, &source_rect_green, &rect_green_6);
                    SDL_Rect rect_green_7 = {ob5x+60, ob2y, 30, 78};
                    SDL_RenderCopy(renderer, fireGreenTexture, &source_rect_green, &rect_green_7);
                    SDL_Rect rect_green_8 = {ob5x+90, ob2y, 30, 78};
                    SDL_RenderCopy(renderer, fireGreenTexture, &source_rect_green, &rect_green_8);
                    
                    SDL_Rect rect_red_1 = {ob3x, oby, 30, 78};
                    SDL_RenderCopy(renderer, fireRedTexture, &source_rect_red, &rect_red_1);
                    SDL_Rect rect_red_2 = {ob3x+30, oby, 30, 78};
                    SDL_RenderCopy(renderer, fireRedTexture, &source_rect_red, &rect_red_2);
                    SDL_Rect rect_red_3 = {ob3x+60, oby, 30, 78};
                    SDL_RenderCopy(renderer, fireRedTexture, &source_rect_red, &rect_red_3);
                    SDL_Rect rect_red_4 = {ob3x+90, oby, 30, 78};
                    SDL_RenderCopy(renderer, fireRedTexture, &source_rect_red, &rect_red_4);
                    SDL_Rect rect_red_5 = {ob6x, ob2y, 30, 78};
                    SDL_RenderCopy(renderer, fireRedTexture, &source_rect_red, &rect_red_5);
                    SDL_Rect rect_red_6 = {ob6x+30, ob2y, 30, 78};
                    SDL_RenderCopy(renderer, fireRedTexture, &source_rect_red, &rect_red_6);
                    SDL_Rect rect_red_7 = {ob6x+60, ob2y, 30, 78};
                    SDL_RenderCopy(renderer, fireRedTexture, &source_rect_red, &rect_red_7);
                    SDL_Rect rect_red_8 = {ob6x+90, ob2y, 30, 78};
                    SDL_RenderCopy(renderer, fireRedTexture, &source_rect_red, &rect_red_8);
                    //Mix_PlayChannel( -1, fireball, 0 );
                    
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
                    
                    if(score > currentScore) {
                        currentScore = score;
                        sprintf(array3, "%d", currentScore);
                    }
                }
                
                SDL_Color textColor = {255, 255, 255};
                
                sprintf(array, "%d", score);//gets the current score and displays it at the top of the screen
                surfaceMessage = TTF_RenderText_Solid(font2, array, textColor);
                surfaceMessage2 = TTF_RenderText_Solid(font, "Score: ", textColor);
                
                Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
                Message2 = SDL_CreateTextureFromSurface(renderer, surfaceMessage2);
                
                SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
                SDL_RenderCopy(renderer, Message2, NULL, &Message_rect2);
                
                //SDL_RenderPresent(renderer);
                
                destroyAndFree(surfaceMessage, Message);
                destroyAndFree(surfaceMessage2, Message2);
                SDL_RenderPresent(renderer);

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
