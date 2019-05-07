//Cosc345 Ben Highsted 2019 Assignment Part Two

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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

    int x = 375;
    int y = 250;
    float count = 600;
    float location = 375;
    bool running = true;
    int score = 1;
    
    TTF_Init();
    
    TTF_Font *font = NULL;
    //Open the font
    font = TTF_OpenFont( "/Library/Fonts/Arial.ttf", 28 );
    
    if(!font){
        return false;
    }
    
    //SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    
    SDL_Rect Message_rect; //create a rect
    Message_rect.x = 300;  //controls the rect's x coordinate
    Message_rect.y = 0; // controls the rect's y coordinte
    Message_rect.w = 20; // controls the width of the rect
    Message_rect.h = 20;
    
    SDL_Rect Message_rect2; //create a rect
    Message_rect2.x = 250;  //controls the rect's x coordinate
    Message_rect2.y = 0; // controls the rect's y coordinte
    Message_rect2.w = 50; // controls the width of the rect
    Message_rect2.h = 20;
    
    SDL_Rect MainMenu_rect;
    MainMenu_rect.x = 200;
    MainMenu_rect.y = 50;
    MainMenu_rect.w = 300;
    MainMenu_rect.h = 50;
    
    SDL_Event event;
    
    bool move_left = false;
    bool move_right = false;
    bool move_up = false;
    bool move_down = false;
    
    bool main_menu = true;
    
    while(running){
        // Process events
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                running = false;
            }
            
            if(main_menu == true){
                SDL_Color textColor = { 255, 255, 255 };
                SDL_RenderClear(renderer);
                SDL_Surface* mainMessage = TTF_RenderText_Solid(font, "Press space bar to start!", textColor);
                SDL_Texture* mainMessage2 = SDL_CreateTextureFromSurface(renderer, mainMessage);
                SDL_RenderCopy(renderer, mainMessage2, NULL, &MainMenu_rect);
                SDL_RenderPresent(renderer);
                if(event.type == SDLK_SPACE){
                    main_menu = false;
                }
            }else{
            
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
            
            
            int wallWidth = 200;

            if(x < wallWidth || x > width - wallWidth - 50){
                x = 375;
                y = 250;
            }
            
            if(y > 550 || y < 0){
                x = 375;
                y = 250;
            }
            
            // Clear screen with black
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            
            // Draw
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color
            
            SDL_Rect wall1 = {0, 0, 200, 600};
            SDL_RenderFillRect(renderer, &wall1);
            
            SDL_Rect wall2 = {800, 0, 200, 600};
            SDL_RenderFillRect(renderer, &wall2);
            
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow color
            
            // Draw a rectangle
            SDL_Rect rect = {x, y, 50, 50};
            SDL_RenderFillRect(renderer, &rect);
            
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Yellow color
            SDL_Rect object1 = {location += .25, count -= 0.25, 50, 50};
            SDL_RenderFillRect(renderer, &object1);
            
            if(count == -50){
                count = 600;
            }
            if(location > 550){
                location = 200;
            }
            
            char array[64];
            sprintf(array, "%d", score);
            score++;
            //printf("%s\n", array);
            
            SDL_Color textColor = { 255, 255, 255 };
            
            SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, array, textColor);
            SDL_Surface* surfaceMessage2 = TTF_RenderText_Solid(font, "Score: ", textColor);
            
            SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
            SDL_Texture* Message2 = SDL_CreateTextureFromSurface(renderer, surfaceMessage2);
            
            SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
            SDL_RenderCopy(renderer, Message2, NULL, &Message_rect2);
            // Show what was drawn
            SDL_RenderPresent(renderer);
        }
            //score++;
        }
    
    // Release resources
    TTF_CloseFont( font );
    
    //Quit SDL_ttf
    TTF_Quit();
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
