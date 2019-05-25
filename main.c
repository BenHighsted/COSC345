/**
 *
 * COSC345 'Dungeon Fall' Assignment 2 2019
 * Ben Highsted, Matthew Neil, Jasmine Hindson
 *
 */

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
    SDL_Window *window = SDL_CreateWindow("Dungeon Fall", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
    
    // Create a renderer (accelerated and in sync with the display refresh rate)
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    int x = 475, y = 250;;
    float location = 375;
    bool running = true;
    int score = 00000001, attempts = 0, counterAdd = 5;
    float counter = 0.0, counter2 = 0.0, counter3 = 0.0, counter4 = 0.0;

    int startx = 300, starty = 400, fallx = 300, fally = -50;
    float menuCounter = 0, fallCounter = 0, backCounter = 0, backCounter2 = 0;
    bool add = true, fall = false, first_loop = false, game_over_loop = false;
    
    bool move_left = false, move_right = false, move_up = false, move_down = false;
    bool main_menu = true, game_over = false, rightmove = false;
    
    char *array = (char *) malloc(64);
    
    TTF_Init();
    IMG_Init(IMG_INIT_JPG);
    
    TTF_Font *font = NULL, *font2 = NULL;
    
    //Open the fonts
    font = TTF_OpenFont("/Library/Fonts/Georgia.ttf", 100);
    font2 = TTF_OpenFont("/Library/Fonts/Arial.ttf", 100);
    
    if(!font){
        printf("Failed to find font");
        return false;
    }
    
    SDL_Rect wall_rect = {0, 0, 200, 600};
    SDL_Rect wall_rect2 = wall_rect;
    wall_rect2.x = 800;
    SDL_Rect wall_rect1_2 = wall_rect;
    wall_rect1_2.y = 600;
    SDL_Rect wall_rect2_2 = wall_rect2;
    wall_rect2_2.y = 600;
    SDL_Rect Message_rect = {505, 5, 35, 20};
    SDL_Rect Message_rect2 = {450, 0, 50, 30};
    SDL_Rect MainMenu_rect = {300, 400, 400, 50};
    SDL_Rect MainMenu_rect2 = {250, 300, 500, 50};
    SDL_Rect Title_rect = {150, 50, 700, 300};
    SDL_Rect Title_background_rect = {0, 0, 1000, 600};
    SDL_Rect Title_background_rect2 = {0, 600, 1000, 600};
    SDL_Rect background_rect = {0, 0, 1000, 600};
    SDL_Rect background_rect2 = {0, 600, 1000, 600};
    
    SDL_Event event;
    
    SDL_Surface *image = IMG_Load("bricks.png");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    
    SDL_Surface *background = IMG_Load("bricksBackground.png");
    SDL_Texture *backTexture = SDL_CreateTextureFromSurface(renderer, background);
    
    SDL_Surface *yoshi = IMG_Load("fatyoshi.png");
    SDL_Texture *yoshiTexture = SDL_CreateTextureFromSurface(renderer, yoshi);

    SDL_Surface* surfaceMessage;
    SDL_Surface* surfaceMessage2;
    SDL_Texture* Message;
    SDL_Texture* Message2;
    
    while(running){
        // Process events
        if(event.type == SDL_QUIT){
            running = false;
        }
        if(main_menu == true){
            counterAdd = 5;
            Title_rect.x = 150;
            Title_rect.y = 50;
            Title_rect.w = 700;
            Title_rect.h = 300;
            menuCounter += 2;
            fallCounter += 0.5;
            backCounter += 6;
            backCounter2 += 6;
            
            if(fallCounter == 500){
                fall = true;
                fallCounter = 0;
            }
            
            if(starty == 400){
                add = false;
            }else if(starty == 380){
                add = true;
            }
            
            SDL_Rect falling_rect = {fallx, fally, 50, 50};
            
            SDL_Color textColor = {255, 255, 255};
            SDL_RenderClear(renderer);
            
            SDL_RenderCopy(renderer, backTexture, NULL, &background_rect2);
            
            SDL_RenderCopy(renderer, backTexture, NULL, &Title_background_rect);
            SDL_RenderCopy(renderer, backTexture, NULL, &Title_background_rect2);
            SDL_RenderCopy(renderer, yoshiTexture, NULL, &falling_rect);//copied here to appear behind text
            
            SDL_Surface* titleMessage = TTF_RenderText_Solid(font, "DUNGEON FALL", textColor);
            SDL_Texture* titleMessage2 = SDL_CreateTextureFromSurface(renderer, titleMessage);
            SDL_RenderCopy(renderer, titleMessage2, NULL, &Title_rect);
            
            SDL_Rect MainMenu_rect = {startx, starty, 400, 50};
            
            if(add){
                if(menuCounter == 30){
                    starty++;
                    menuCounter = 0;
                }
            }else{
                if(menuCounter == 30){
                    starty--;
                    menuCounter = 0;
                }
            }
            
            if(fall){
                fally++;
            }
            
            SDL_Surface* mainMessage = TTF_RenderText_Solid(font, "Press space bar to start!", textColor);
            SDL_Texture* mainMessage2 = SDL_CreateTextureFromSurface(renderer, mainMessage);
            
            SDL_RenderCopy(renderer, mainMessage2, NULL, &MainMenu_rect);
            
            SDL_RenderPresent(renderer);//present the title page
            
            SDL_DestroyTexture(titleMessage2);//free resources
            SDL_DestroyTexture(mainMessage2);
            SDL_FreeSurface(titleMessage);
            SDL_FreeSurface(mainMessage);
            
            while(SDL_PollEvent(&event)){
                if(event.key.keysym.sym == SDLK_SPACE){
                    main_menu = false;
                    first_loop = true;
                }
            }
        }else{
            if(game_over == true){
                counterAdd = 5;
                //attempts++;
                
                SDL_Color textColor = {255, 255, 255};
                SDL_Rect game_over_back2 = {0, 0, 1000, 600};

                score = 0;
                
                move_left = false;
                move_right = false;
                move_up = false;
                move_down = false;
                
                SDL_RenderClear(renderer);
                
                SDL_RenderCopy(renderer, backTexture, NULL, &game_over_back2);
                
                SDL_Surface* mainMessage = TTF_RenderText_Solid(font, "Press space bar to try again!", textColor);
                SDL_Texture* mainMessage2 = SDL_CreateTextureFromSurface(renderer, mainMessage);
                
                SDL_Surface* mainMessage3 = TTF_RenderText_Solid(font, "Press 'M' to return to the menu!", textColor);
                SDL_Texture* mainMessage4 = SDL_CreateTextureFromSurface(renderer, mainMessage3);
                
                SDL_RenderCopy(renderer, mainMessage2, NULL, &MainMenu_rect);
                SDL_RenderCopy(renderer, mainMessage4, NULL, &MainMenu_rect2);
                
                SDL_RenderPresent(renderer);
                
                SDL_FreeSurface(mainMessage);
                SDL_DestroyTexture(mainMessage2);
                
                SDL_FreeSurface(mainMessage3);
                SDL_DestroyTexture(mainMessage4);
                while(SDL_PollEvent(&event)){
                    if(event.key.keysym.sym == SDLK_SPACE){
                        game_over = false;
                        //first_loop = true;
                        game_over_loop = true;
                    }
                    if(event.key.keysym.sym == SDLK_m){
                        game_over = false;
                        main_menu = true;
                    }
                }
            }else{

            SDL_RenderClear(renderer);
            float count = 0;
            fallx = 475;
            fally = -100;
            
            if(first_loop){
                wall_rect.y = -600;
                wall_rect2.y = -600;
                wall_rect1_2.y = -600;
                wall_rect2_2.y = -600;
            }
            
            while(first_loop){
                SDL_RenderClear(renderer);
                count += 0.5;
                counter3 += 5;
                counter4 += 5;
                counter2+= 5;
                counter+= 5;
                    
                SDL_Color textColor = {255, 255, 255};
                    
                if(counter >= 100){
                    counter = 0;
                    background_rect.y -= 3;
                }
                if(background_rect.y <= -600){
                    background_rect.y = 600;
                }
                    
                SDL_RenderCopy(renderer, backTexture, NULL, &background_rect);
                    
                if(counter2 >= 100){
                    counter2 = 0;
                    background_rect2.y -= 3;
                    Title_rect.y -= 3;
                        
                }
                if(background_rect2.y <= -600){
                    background_rect2.y = 600;
                }
                    
                SDL_RenderCopy(renderer, backTexture, NULL, &background_rect2);
                    
                if(counter3 > 50){
                    wall_rect.y -= 3;
                }
                if(wall_rect.y <= -600){
                    wall_rect.y = 600;
                }
                SDL_RenderCopy(renderer, texture, NULL, &wall_rect);
                if(counter3 > 50){
                    wall_rect2.y -= 3;
                    counter3 = 0;
                }
                if(wall_rect2.y <= -600){
                    wall_rect2.y = 600;
                }
                SDL_RenderCopy(renderer, texture, NULL, &wall_rect2);
                    
                if(counter4 > 50){
                    wall_rect1_2.y -= 3;
                }
                if(wall_rect1_2.y <= -1200){
                    wall_rect1_2.y = 600;
                }
                SDL_RenderCopy(renderer, texture, NULL, &wall_rect1_2);
                    
                if(counter4 > 50){
                    counter4 = 0;
                    wall_rect2_2.y -= 3;
                }
                if(wall_rect2_2.y <= -1200){
                    wall_rect2_2.y = 600;
                }
                SDL_RenderCopy(renderer, texture, NULL, &wall_rect2_2);
                    
                SDL_Surface* titleMessage = TTF_RenderText_Solid(font, "DUNGEON FALL", textColor);
                SDL_Texture* titleMessage2 = SDL_CreateTextureFromSurface(renderer, titleMessage);
                SDL_RenderCopy(renderer, titleMessage2, NULL, &Title_rect);
                    
                if(count > 1500){
                    SDL_Rect falling_rect = {fallx, fally, 50, 50};
                    SDL_RenderCopy(renderer, yoshiTexture, NULL, &falling_rect);
                    if(fally != 250){
                        fally += 1.5;
                    }
                    if(fally >= 250){
                        first_loop = false;
                        counter3 = 0.0;
                        counter4 = 0.0;
                    }
                }
                SDL_RenderPresent(renderer);
                SDL_DestroyTexture(titleMessage2);
                SDL_FreeSurface(titleMessage);
                    
            }
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
                        rightmove = true;
                    }
                    if(event.key.keysym.sym == SDLK_LEFT){
                        move_left = true;
                        rightmove = false;
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
                
            counter += counterAdd;
            counter2 += counterAdd;
            counter3 += counterAdd;
            counter4 += counterAdd;
            if(score == 5000 || score == 10000 || score == 20000 || score == 30000 || score == 40000 || score == 50000){//speeds up falling pace
                counterAdd += 3;
            }
                
            int wallWidth = 200;

            if(x < wallWidth || x > width - wallWidth - 50){
                x = 475;
                y = 250;
                game_over = true;
            }
            
            if(y > 550 || y < 0){
                x = 475;
                y = 250;
                game_over = true;
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
            
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
            
            // Draw a rectangle
            SDL_Rect rect = {x, y, 50, 50};
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_RenderFillRect(renderer, &rect);
            
                
            if(rightmove){
                float angle = 0.0; // set the angle.
                SDL_Point center = {8, 8}; // the center where the texture will be rotated.
                SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL; // the flip of the texture.
                
                    // now, try to draw something
                SDL_RenderCopyEx(renderer, yoshiTexture, NULL , &rect, angle, &center, flip);
            }else{
                SDL_RenderCopy(renderer, yoshiTexture, NULL, &rect);
            }
            
            if(location > 550){
                location = 200;
            }

            score++;
            sprintf(array, "%d", score);

            SDL_Color textColor = { 255, 255, 255 };
                
            surfaceMessage = TTF_RenderText_Solid(font2, array, textColor);
            surfaceMessage2 = TTF_RenderText_Solid(font, "Score: ", textColor);
             
            Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
            Message2 = SDL_CreateTextureFromSurface(renderer, surfaceMessage2);
                
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
            SDL_DestroyTexture(Message);
            SDL_DestroyTexture(Message2);
            SDL_FreeSurface(surfaceMessage);
            SDL_FreeSurface(surfaceMessage2);
        }
        }
    }
    
    free(array);
    
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
