//Cosc345 Ben Highsted 2019 Assignment Part Two

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <SDL2/SDL_image.h>

#include "SDL2_ttf/SDL_ttf.h"
#include <SDL2/SDL.h>

static const int width = 1000;
static const int height = 600;

int main(int argc, char **argv){
    
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    
    // Create a SDL window
    SDL_Window *window = SDL_CreateWindow("Hello, SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
    
    // Create a renderer (accelerated and in sync with the display refresh rate)
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    int x = 475;
    int y = 250;
    float location = 375;
    bool running = true;
    int score = 00000001;
    float counter = 0.0;
    float counter2 = 0.0;
    float counter3 = 0.0;
    float counter4 = 0.0;
    
    TTF_Init();
    IMG_Init(IMG_INIT_JPG);
    
    TTF_Font *font = NULL, *font2 = NULL;
    
    //Open the font
    font = TTF_OpenFont("/Library/Fonts/Georgia.ttf", 100);
    font2 = TTF_OpenFont("/Library/Fonts/Arial.ttf", 100);
    
    //SDL_Surface* image = IMG_Load("/images/jhin.jpg");
    //SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    
    if(!font){
        printf("Failed to find font");
        return false;
    }
    
    
    
    Uint32 startTime = 0;
    Uint32 endTime = 0;
    Uint32 delta = 0;
    short fps = 60;
    short timePerFrame = 16; // miliseconds
    
    
    
    
    SDL_Rect wall_rect;
    wall_rect.x = 0;
    wall_rect.y = 0;
    wall_rect.w = 200;
    wall_rect.h = 600;
    
    SDL_Rect wall_rect2 = wall_rect;
    wall_rect2.x = 800;
    
    SDL_Rect wall_rect1_2 = wall_rect;
    wall_rect1_2.y = 600;
    
    SDL_Rect wall_rect2_2 = wall_rect2;
    wall_rect2_2.y = 600;
    
    //SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_Rect Image_rect; //Game over text box
    Image_rect.x = 200;
    Image_rect.y = 50;
    Image_rect.w = 600;
    Image_rect.h = 600;
    
    SDL_Rect Message_rect; //create a rect
    Message_rect.x = 505;  //controls the rect's x coordinate
    Message_rect.y = 5; // controls the rect's y coordinte
    Message_rect.w = 35; // controls the width of the rect
    Message_rect.h = 20;
    
    SDL_Rect Message_rect2; //create a rect
    Message_rect2.x = 450;  //controls the rect's x coordinate
    Message_rect2.y = 0; // controls the rect's y coordinte
    Message_rect2.w = 50; // controls the width of the rect
    Message_rect2.h = 30;
    
    SDL_Rect MainMenu_rect; //Main Menu Text Box
    MainMenu_rect.x = 300;
    MainMenu_rect.y = 400;
    MainMenu_rect.w = 400;
    MainMenu_rect.h = 50;
    
    SDL_Rect Title_rect; //Main Menu Title Text Box
    Title_rect.x = 150;
    Title_rect.y = 50;
    Title_rect.w = 800;
    Title_rect.h = 300;
    
    SDL_Rect GameOver_Rect; //Game over text box
    GameOver_Rect.x = 200;
    GameOver_Rect.y = 50;
    GameOver_Rect.w = 300;
    GameOver_Rect.h = 50;
    
    SDL_Rect background_rect;
    background_rect.x = 0;
    background_rect.y = 0;
    background_rect.w = 800;
    background_rect.h = 600;
    
    SDL_Rect background_rect2 = background_rect;
    background_rect.y = 600;
    
    SDL_Event event;
    
    bool move_left = false;
    bool move_right = false;
    bool move_up = false;
    bool move_down = false;
    
    bool main_menu = true;
    bool game_over = false;
    bool beencleared = false;
    
    SDL_Surface *image = IMG_Load("bricks.png");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    
    SDL_Surface *background = IMG_Load("bricksBackground.png");
    SDL_Texture *backTexture = SDL_CreateTextureFromSurface(renderer, background);
    
    SDL_Surface *yoshi = IMG_Load("yoshi.jpg");
    SDL_Texture *yoshiTexture = SDL_CreateTextureFromSurface(renderer, yoshi);
    
    while(running){
        // Process events
        if(event.type == SDL_QUIT){
            running = false;
        }
        if(main_menu == true){
            SDL_Color textColor = {255, 255, 255};
            SDL_RenderClear(renderer);
            SDL_Surface* titleMessage = TTF_RenderText_Solid(font, "TITLE TO BE ENTERED", textColor);
            SDL_Texture* titleMessage2 = SDL_CreateTextureFromSurface(renderer, titleMessage);
            SDL_RenderCopy(renderer, titleMessage2, NULL, &Title_rect);
            SDL_Surface* mainMessage = TTF_RenderText_Solid(font, "Press space bar to start!", textColor);
            SDL_Texture* mainMessage2 = SDL_CreateTextureFromSurface(renderer, mainMessage);
            SDL_RenderCopy(renderer, mainMessage2, NULL, &MainMenu_rect);
            SDL_RenderPresent(renderer);

            while(SDL_PollEvent(&event)){
                if(event.key.keysym.sym == SDLK_SPACE){
                    main_menu = false;
                }
            }
        }else{
            if(game_over){
                //SDL_Delay(2000);
                score = 0;
                if(!beencleared){
                    SDL_RenderClear(renderer);
                    beencleared = true;
                }
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                
                move_left = false;
                move_right = false;
                move_up = false;
                move_down = false;
                
                SDL_Color textColor = { 255, 255, 255 };
                
                SDL_RenderClear(renderer);
                
                SDL_Surface* mainMessage = TTF_RenderText_Solid(font, "Press space bar to start!", textColor);
                SDL_Texture* mainMessage2 = SDL_CreateTextureFromSurface(renderer, mainMessage);
                
                SDL_RenderCopy(renderer, mainMessage2, NULL, &MainMenu_rect);
                SDL_RenderPresent(renderer);
                
                while(SDL_PollEvent(&event)){
                    if(event.key.keysym.sym == SDLK_SPACE){
                        game_over = false;
                    }
                }
                beencleared = false;
                
            }else{
                while(SDL_PollEvent(&event)){
            
                    if(event.type == SDL_KEYUP){
                        if(event.key.keysym.sym == SDLK_RIGHT){
                            move_right = false;
                        }
                        if(event.key.keysym.sym == SDLK_LEFT){
                            move_left = false;
                        }
                        if(event.key.keysym.sym == SDLK_DOWN){
                            move_down = false;
                        }
                        if(event.key.keysym.sym == SDLK_UP){
                            move_up = false;
                        }
                    }
                    if(event.type == SDL_KEYDOWN){
                        if(event.key.keysym.sym == SDLK_RIGHT){
                            move_right = true;
                        }
                        if(event.key.keysym.sym == SDLK_LEFT){
                            move_left = true;
                        }
                        if(event.key.keysym.sym == SDLK_DOWN){
                            move_down = true;
                        }
                        if(event.key.keysym.sym == SDLK_UP){
                            move_up = true;
                        }
                    }
                }
                if(move_left){
                    x -= 1;
                }
                if(move_right){
                    x += 1;
                }
                if(move_up){
                    y -= 1;
                }
                if(move_down){
                    y += 1;
                }
                
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                
                int WallState = rand() % 4;//equals either 0, 1, 2 or 3.
                
                
                    if (!startTime) {
                        // get the time in ms passed from the moment the program started
                        startTime = SDL_GetTicks();
                    } else {
                        delta = endTime - startTime; // how many ms for a frame
                    }
                    
                    
                    // if less than 16ms, delay
                    if (delta < timePerFrame) {
                        SDL_Delay(1);
                    }
                    
                    // if delta is bigger than 16ms between frames, get the actual fps
                    if (delta > timePerFrame) {
                        fps = 1000 / delta;
                    }
                    
                    printf("FPS is: %i \n", fps);
                    
                    startTime = endTime;
                    endTime = SDL_GetTicks();
                
                
                
                
                
                
                /** TO GO HERE IS SOME CODE BASED ON THE ABOVE RAND WHAT WALL STATE IT SHOULD BE IN
                 E.G. wallstate 0 is on the left, and 3 is on the right */
                int counterAdd = 5;
                
                counter += counterAdd;
                counter2 += counterAdd;
                counter3 += counterAdd;
                counter4 += counterAdd;
                if(score == 10000 || score == 20000 || score == 30000 || score == 40000 || score == 50000){//speeds up falling pace
                    counterAdd += 2;
                }
                
                int wallWidth = 200;

                if(x < wallWidth || x > width - wallWidth - 50){
                    game_over = true;
                    x = 475;
                    y = 250;
                }
            
                if(y > 550 || y < 0){
                    game_over = true;
                    x = 475;
                    y = 250;
                }
                
                if(counter >= 100){
                    background_rect.y -= 2;
                    counter = 0;
                }
                if(background_rect.y <= -600){
                    background_rect.y = 600;
                }
                
                SDL_RenderCopy(renderer, backTexture, NULL, &background_rect);
                
                if(counter2 >= 100){
                    counter2 = 0;
                    background_rect2.y -= 2;
                }
                if(background_rect2.y <= -600){
                    background_rect2.y = 600;
                }
                
                SDL_RenderCopy(renderer, backTexture, NULL, &background_rect2);
            
                // Clear screen with black

                // Draw
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color
                
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow color
            
                // Draw a rectangle
                SDL_Rect rect = {x, y, 50, 50};
                SDL_RenderFillRect(renderer, &rect);
                
                SDL_RenderCopy(renderer, yoshiTexture, NULL, &rect);
            
                if(location > 550){
                    location = 200;
                }
                
                char array[64];
                sprintf(array, "%d", score);
                score++;

                SDL_Color textColor = { 255, 255, 255 };
                
                SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font2, array, textColor);
                SDL_Surface* surfaceMessage2 = TTF_RenderText_Solid(font, "Score: ", textColor);
                
                SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
                SDL_Texture* Message2 = SDL_CreateTextureFromSurface(renderer, surfaceMessage2);
                
                SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
                SDL_RenderCopy(renderer, Message2, NULL, &Message_rect2);
                // Show what was drawn
                if(counter3 > 50){
                    wall_rect.y -= 4;
                }
                if(wall_rect.y <= -600){
                    wall_rect.y = 600;
                }
                SDL_RenderCopy(renderer, texture, NULL, &wall_rect);
                
                if(counter3 > 50){
                    wall_rect2.y -= 4;
                    counter3 = 0;
                }
                if(wall_rect2.y <= -600){
                    wall_rect2.y = 600;
                }
                SDL_RenderCopy(renderer, texture, NULL, &wall_rect2);
                
                if(counter4 > 50){
                    wall_rect1_2.y -= 4;
                }
                if(wall_rect1_2.y <= -600){
                    wall_rect1_2.y = 600;
                }
                SDL_RenderCopy(renderer, texture, NULL, &wall_rect1_2);
                
                if(counter4 > 50){
                    counter4 = 0;
                    wall_rect2_2.y -= 4;
                }
                if(wall_rect2_2.y <= -600){
                    wall_rect2_2.y = 600;
                }
                SDL_RenderCopy(renderer, texture, NULL, &wall_rect2_2);
                
                //SDL_RenderCopy(renderer, texture, NULL, &Image_rect);
                
                SDL_RenderPresent(renderer);
            }
        }
    }
    
    // Release resources
    TTF_CloseFont( font );
    
    //Quit SDL_image
    IMG_Quit();
    
    //Quit SDL_ttf
    TTF_Quit();
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
