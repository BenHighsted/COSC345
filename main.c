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
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    int x = 475, y = 250;;
    float location = 375;
    bool running = true;
    int score = 00000001, attempts = 0, counterAdd = 5;
    float counter = 0.0, counter2 = 0.0, counter3 = 0.0, counter4 = 0.0;

    int startx = 300, starty = 400, fallx = 300, fally = -50;
    float menuCounter = 0, fallCounter = 0, backCounter = 0, backCounter2 = 0;
    bool add = true, fall = false, first_loop = false;
    
    bool move_left = false, move_right = false, move_up = false, move_down = false;
    bool main_menu = true, game_over = false, rightmove = false;
    
    char *array = (char *) malloc(64);
    char *array2 = (char *) malloc(64);
    
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
    
    SDL_Rect wall_rect = {-800, 0, 1000, 600};
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
    
    
    //MATTS MERGE CODE
    int wallLeftX = -800;
    int wallRightX = 800;
    
    int phases = 0;
    
    bool setup = true;
    
    float oby = 810;
    float speed = 0.5;
    
    float ob2y = 1110;
    
    float obx = (rand() % 520)+200;
    float ob2x = (rand() % 550)+200;
    float ob3x = (rand() % 500)+200;
    
    float ob4x = (rand() % 520)+200;
    float ob5x = (rand() % 550)+200;
    float ob6x = (rand() % 500)+200;
    
    bool complete;
    
    float wallPos1 = 100, wallPos2 = 300, wallPos3 = 500, wallPos4 = 700;
    int pos;
    
    int mode = 0;
    
    bool switchModes = false;
    
    int points = 0;
    int counted1 = 0;
    int counted2 = 0;
    
    float wallCount = 0.0;
    
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
                
                SDL_Color textColor = {255, 255, 255};
                SDL_Rect game_over_back2 = {0, 0, 1000, 600};
                SDL_Rect GameOver_rect = {230, 50, 500, 100};
                SDL_Rect Attempts_rect = {380, 150, 200, 50};
                SDL_Rect Attempts_rect2 = {585, 150, 20, 45 };

                score = 0;
                
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
                
                SDL_RenderCopy(renderer, gameOverTexture, NULL, &GameOver_rect);
                SDL_RenderCopy(renderer, attemptMessage2, NULL, &Attempts_rect);
                SDL_RenderCopy(renderer, attemptCount2, NULL, &Attempts_rect2);
                SDL_RenderCopy(renderer, mainMessage2, NULL, &MainMenu_rect);
                SDL_RenderCopy(renderer, mainMessage4, NULL, &MainMenu_rect2);
                
                SDL_RenderPresent(renderer);
                
                SDL_FreeSurface(gameOverSurface);
                SDL_DestroyTexture(gameOverTexture);
                SDL_FreeSurface(mainMessage);
                SDL_DestroyTexture(mainMessage2);
                SDL_FreeSurface(mainMessage3);
                SDL_DestroyTexture(mainMessage4);
                SDL_FreeSurface(attemptMessage);
                SDL_DestroyTexture(attemptMessage2);
                SDL_FreeSurface(attemptCount);
                SDL_DestroyTexture(attemptCount2);
                
                while(SDL_PollEvent(&event)){
                    if(event.key.keysym.sym == SDLK_SPACE){
                        game_over = false;
                        first_loop = true;
                    }
                    if(event.key.keysym.sym == SDLK_m){
                        game_over = false;
                        main_menu = true;
                    }
                }
                if(game_over == false){
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
                count += 1;
                counter3 += 10;
                counter4 += 10;
                counter2+= 10;
                counter+= 10;
                    
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

            //if(x < wallWidth || x > width - wallWidth - 50){
                if(x < wallLeftX + 1000 || x > wallRightX - 20){
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
                
                //mode = 1;
                
                if(score % 10000 == 0){
                    switchModes = true;
                }
                if(mode == 0){
                    oby -= speed;
                    ob2y -= speed;
                    
                    if(!switchModes){
                        if(x + 20 >= obx && x <= obx + 70){
                            if(oby + 50 >= y && oby <= y+50){
                                game_over = true;
                            }
                        }
                        if(x + 20 >= ob2x && x <= ob2x + 50){
                            if(oby + 50 >= y && oby <= y+50){
                                game_over = true;
                            }
                        }
                        
                        if(x + 20 >= ob3x && x <= ob3x + 100){
                            if(oby + 50 >= y && oby <= y+50){
                                game_over = true;
                            }
                        }
                        
                        
                        if(oby < -50){
                            obx = (rand() % 520)+200;
                            ob2x = (rand() % 550)+200;
                            ob3x = (rand() % 500)+200;
                            oby = 600;
                            counted1 = 0;
                        }
                        
                        
                        if(x + 20 >= ob4x && x <= ob4x + 70){
                            if(ob2y + 50 >= y && ob2y <= y+50){
                                game_over = true;
                            }
                        }
                        if(x + 20 >= ob5x && x <= ob5x + 50){
                            if(ob2y + 50 >= y && ob2y <= y+50){
                                game_over = true;
                            }
                        }
                        
                        if(x + 20 >= ob6x && x <= ob6x + 100){
                            if(ob2y + 50 >= y && ob2y <= y+50){
                                game_over = true;
                            }
                        }
                        
                        
                        if(ob2y < -50){
                            ob4x = (rand() % 520)+200;
                            ob5x = (rand() % 550)+200;
                            ob6x = (rand() % 500)+200;
                            ob2y = 600;
                            counted2 = 0;
                            phases++;
                        }
                    } else {
                        if(oby <= -50 && ob2y <= -50){
                            mode = 1;
                            oby = 600;
                            ob2y = 900;
                            setup = true;
                            switchModes = false;
                        }
                    }
                    
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    SDL_Rect rect2 = {obx, oby, 70, 50};
                    SDL_RenderFillRect(renderer, &rect2);
                    
                    SDL_Rect rect3 = {ob2x, oby, 50, 50};
                    SDL_RenderFillRect(renderer, &rect3);
                    
                    SDL_Rect rect4 = {ob3x, oby, 100, 50};
                    SDL_RenderFillRect(renderer, &rect4);
                    SDL_Rect rect5 = {ob4x, ob2y, 70, 50};
                    SDL_RenderFillRect(renderer, &rect5);
                    
                    SDL_Rect rect6 = {ob5x, ob2y, 50, 50};
                    SDL_RenderFillRect(renderer, &rect6);
                    
                    SDL_Rect rect7 = {ob6x, ob2y, 100, 50};
                    SDL_RenderFillRect(renderer, &rect7);
                    

                    
                } else {
                    wallCount += 10;
                    if(!switchModes){
                        if(setup){
                            if(wallCount > 30){
                            wallLeftX += 1;
                            wallRightX -= 1;
                            wall_rect.x = wallLeftX;
                            wall_rect2.x = wallRightX;
                            wall_rect1_2.x = wallLeftX;
                            wall_rect2_2.x = wallRightX;
                                wallCount = 0;
                            }
                            if(wallLeftX >= -625){
                                setup = false;
                                complete = false;
                                pos = rand() % 4;
                            }
                        } else {
                            wall_rect.x = wallLeftX;
                            wall_rect2.x = wallRightX;
                            wall_rect1_2.x = wallLeftX;
                            wall_rect2_2.x = wallRightX;
                            
                            if(pos == 0){
                                if(!complete){
                                    if(wallCount > 30){
                                    wallLeftX -= 1;
                                    wallRightX -= 1;
                                        wallCount = 0;
                                    }
                                    if(wallLeftX <= wallPos1-1000){
                                        //complete = true;
                                        pos = rand() % 4;
                                    }
                                }
                            } else if(pos == 1){
                                if(!complete){
                                    
                                    if(wallLeftX < wallPos2 - 1000) {
                                        if(wallCount > 30){
                                            wallLeftX += 1;
                                            wallRightX += 1;
                                            wallCount = 0;
                                        }
                                        if(wallLeftX >= wallPos2-1000){
                                            //complete = true;
                                            pos = rand() % 4;
                                        }
                                    } else {
                                        if(wallCount == 30){
                                        wallLeftX -= 1;
                                        wallRightX -= 1;
                                            wallCount = 0;
                                        }
                                        if(wallLeftX <= wallPos2-1000){
                                            //complete = true;
                                            pos = rand() % 4;
                                        }
                                    }
                                }
                            } else if(pos == 2){
                                if(!complete){
                                    if(wallLeftX < wallPos3 - 1000) {
                                        if(wallCount > 30){
                                        wallLeftX += 1;
                                        wallRightX += 1;
                                            wallCount = 0;
                                        }
                                        if(wallLeftX >= wallPos3-1000){
                                            //complete = true;
                                            pos = rand() % 4;
                                        }
                                    } else {
                                        if(wallCount > 30){
                                        wallLeftX -= 1;
                                        wallRightX -= 1;
                                            wallCount = 0;
                                        }
                                        if(wallLeftX <= wallPos3-1000){
                                            //complete = true;
                                            pos = rand() % 4;
                                        }
                                    }
                                }
                            } else {
                                if(!complete){
                                    if(wallCount > 30){
                                    wallLeftX += 1;
                                    wallRightX += 1;
                                        wallCount = 0;
                                    }
                                     if(wallLeftX >= wallPos4-1000){
                                        //complete = true;
                                        pos = rand() % 4;
                                    }
                                }
                            }
                            
                        }
                        
                        if(score % 10000 == 0){
                            switchModes = true;
                        }
                        
                    } else {
                        wall_rect.x = wallLeftX;
                        wall_rect2.x = wallRightX;
                        wall_rect1_2.x = wallLeftX;
                        wall_rect2_2.x = wallRightX;
                        if(wallLeftX > -800){
                            if(wallCount > 30){
                            wallLeftX -= 1;
                                wallCount = 0;
                            }
                        }
                        if(wallRightX < 800){
                            if(wallCount > 30){
                                
                            wallRightX += 1;
                                wallCount = 0;
                            }
                        }
                        if(wallLeftX <= -800 && wallRightX >= 800){
                            wallLeftX = -800;
                            wallRightX = 800;
                            mode = 0;
                            switchModes = false;
                        }
                    }
                }
                
                if(game_over){
                    attempts++;
                    sprintf(array2, "%d", attempts);
                }
                
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
