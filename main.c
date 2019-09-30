 /**COSC345 'Dungeon Fall' Assignment 2 2019
  * Ben Highsted, Matthew Neil, Jasmine Hindson
  * Last Edited: Fri Sep 27 16:02:43 NZST 2019 */

/* https://www.freepik.com/free-vector/set-realistic-fire-flames_5199081.html
 * Resource used for the flames on the executable icon */
 #include <stdio.h>//standard includes
 #include <stdlib.h>
 #include <stdbool.h>
 #include <string.h>
 #include <SDL2/SDL.h>//SDL2 includes
 #include <SDL2/SDL_image.h>
 #include "SDL2_ttf/SDL_ttf.h"
 #include "SDL2_mixer/SDL_mixer.h"
 #define TICK_INTERVAL 15
 #define MAXCHAR 1000
 static const int width = 1000, height = 700;//width and height of the window
 static Uint32 next_time;
 /** Method works out how much time is left and determines how much to slow down for the current PC
  *  https://www.libsdl.org/release/SDL-1.2.15/docs/html/guidetimeexamples.html
  *  Essentially caps the fps to keep it consistent across multiple computers **/
 Uint32 time_left(void)
 {
     Uint32 now;
     now = SDL_GetTicks();
     if(next_time <= now)
         return 0;
     else
         return next_time - now;
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
     Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);//opens the SDL_Mixer audio stuff
     Mix_Music *music = NULL;
     Mix_Chunk *fireball = NULL;//loading in the fireball sound effect
     fireball = Mix_LoadWAV("content/fireball.mp3");
     music = Mix_LoadMUS("content/dungeonMusic.mp3");//Non copyrighted music from https://www.youtube.com/watch?v=6Lm4yer6KxE
     Mix_PlayMusic(music, -1);//starts the music playing
     SDL_Init(SDL_INIT_VIDEO);//Initialize SDL and creates a window/renderer
     SDL_Window *window = SDL_CreateWindow("Dungeon Fall", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
     SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
     TTF_Init();//Font initialisation code
     IMG_Init(IMG_INIT_JPG);//Image initialisation code
     TTF_Font *font = NULL, *font2 = NULL;//creates font variables and assigns them to fonts out of /Library/Fonts.
     font = TTF_OpenFont("/Library/Fonts/Georgia.ttf", 100);//Only works for MACOS currently.
     font2 = TTF_OpenFont("/Library/Fonts/Arial.ttf", 100);
     /** Variable Declarations **/
     int score = 00000001, attempts = 0, counterAdd = 5, x = 475, y = 250, option = 0, arrowAnimation = 0;;
     int startx = 300, starty = 600, fallx = 300, fally = -100, wallLeftX = -800, wallRightX = 800;
     int mode = 0, pos = 0, position = 185;
     int startyGameOver = 560, currentScore = 0;
     int speed = 4, oby = -100, ob2y = 10000000;
     float menuCounter = 0, fallCounter = 0, backCounter = 0, backCounter2 = 0, menuCounterGameOver = 0;
     float counter = 0.0, counter2 = 0.0, counter3 = 0.0, counter4 = 0.0;
     float count = 0, count2 = 100;
     float fireObjectsX[] = {(rand() % 480)+200, (rand() % 480)+200, (rand() % 480)+200, (rand() % 480)+200, (rand() % 480)+200, (rand() % 480)+200};
     bool add = true, fall = false, first_loop = false, running = true;
     bool setup = true, switchModes = false, arr = true;;
     bool move_left = false, move_right = false, move_up = false, move_down = false;
     bool main_menu = true, game_over = false, rightmove = false;
     bool character_description = false, main_menu_screen = false;
     bool sprite1 = true, sprite2 = false, sprite3 = false;
     bool first_time = true, first_game_over = true, showHighScore = false;
     bool addGameOver = true, leaderboard = false, reading_first_time = true;
     char *array = (char *) malloc(64), *array2 = (char *) malloc(64), *array3 = (char *) malloc(100);
     /** Rectangle Declarations **/
     SDL_Rect wall_rect = {-800, 0, 1000, 700}, wall_rect2 = {800, 0, 1000, 700};
     SDL_Rect wall_rect1_2 = {-800, 700, 1000, 700}, wall_rect2_2 = {800, 700, 1000, 700};
     SDL_Rect Message_rect = {505, 5, 35, 20}, Message_rect2 = {450, 0, 50, 30};
     SDL_Rect Title_rect = {150, 50, 700, 300};
     SDL_Rect Title_background_rect = {0, 0, 1000, 710}, Title_background_rect2 = {0, 700, 1000, 710};
     SDL_Rect background_rect = {0, 0, 1000, 710}, background_rect2 = {0, 710, 1000, 710};
     SDL_Rect sprite1_rect = {200, 370, 130, 130}, sprite_rect2 = {375, 50, 220, 220};
     SDL_Rect sprite2_rect = {400, 370, 130, 130}, sprite3_rect = {600, 370, 130, 130};
     SDL_Rect source_rect_red = {0, 0, 10, 26}, source_rect_blue = {0, 0, 9, 24};
     SDL_Rect source_rect_green = {0, 0, 9, 25}, gFlashClips = {555, 210, 250, 150};
     SDL_Rect nameRect = {300, 280, 370, 50}, infoRect = {400, 550, 200, 30};
     SDL_Rect leaderTitleRect = {261, 370, 500, 400}, menuTitleRect = {260, 440, 500, 400};
     SDL_Rect gameOverTitleRect = {130, -60, 750, 600}, attemptMessageRect = {280, 130, 400, 300};
     SDL_Rect scoreMessageRect = {280, 190, 400, 300}, highScoreMessageRect = {260, 250, 400, 300};
     SDL_Rect game_over_back2 = {0, 0, 1000, 700}, attemptsRect = {590, 165, 25, 50};
     SDL_Rect highscoreRect = {560, 300, 50, 30}, scoreRect = {530, 240, 50, 30};
     SDL_Rect MainMenu_rect = {startx, starty, 400, 50}, mainMenuRect = {345, 340, 300, 260};
     SDL_Rect falling_rect = {fallx, fally, 100, 100}, border_rect = {position, 355, 160, 160};
     SDL_Rect pick_rect = {190, 290, 300, 40}, enter_rect = {position + 6, 520, 150, 20};
     SDL_Rect arrow = {200, 600, 50, 50}, arrow2 = {600, 600, 50, 50};
     SDL_Rect description_image = {5, 250, 990, 250}, leaderboardTitleRect = {150, 20, 700, 150};
     SDL_Rect instructionRect = {510, 300, 300, 200}, instructionTextRect = {230, 370, 300, 150};
     SDL_Event event;//starts SDL event
     SDL_Color textColor = {255, 255, 255}; //Text color white
     //Background and wall textures
     SDL_Surface *image = IMG_Load("content/bricks.png");//From: https://www.deviantart.com/skazdal/art/Rock-bricks-texture-670434391
     SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
     SDL_Surface *background = IMG_Load("content/bricksBackground.png");//From: http://pixelartmaker.com/art/31b17490e7ef5d8
     SDL_Texture *backTexture = SDL_CreateTextureFromSurface(renderer, background);
     //Border and Sprite textures
     SDL_Surface *borderSurface = IMG_Load("content/border.png");//Following images created by Jasmine Hindson
     SDL_Texture *border = SDL_CreateTextureFromSurface(renderer, borderSurface);
     SDL_Surface *spriteStraightTexture = IMG_Load("content/sprite1straight.png");
     SDL_Texture *sprite1StraightTexture = SDL_CreateTextureFromSurface(renderer, spriteStraightTexture);
     SDL_Surface *sprite2Straight = IMG_Load("content/sprite2straight.png");
     SDL_Texture *sprite2StraightTexture = SDL_CreateTextureFromSurface(renderer, sprite2Straight);
     SDL_Surface *sprite3Straight = IMG_Load("content/sprite3straight.png");
     SDL_Texture *sprite3StraightTexture = SDL_CreateTextureFromSurface(renderer, sprite3Straight);
     SDL_Surface *spriteTexture = IMG_Load("content/sprite1falling.png");
     SDL_Texture *sprite1FallingTexture = SDL_CreateTextureFromSurface(renderer, spriteTexture);
     SDL_Surface *sprite2Falling = IMG_Load("content/sprite2falling.png");
     SDL_Texture *sprite2FallingTexture = SDL_CreateTextureFromSurface(renderer, sprite2Falling);
     SDL_Surface *sprite3Falling = IMG_Load("content/sprite3falling.png");
     SDL_Texture *sprite3FallingTexture = SDL_CreateTextureFromSurface(renderer, sprite3Falling);
     //Fireball/Object textures
     SDL_Surface *fireRed = IMG_Load("content/fire-red.png");//Following images from https://stealthix.itch.io/animated-fires
     SDL_Texture *fireRedTexture = SDL_CreateTextureFromSurface(renderer, fireRed);
     SDL_Surface *fireBlue = IMG_Load("content/fire-blue.png");
     SDL_Texture *fireBlueTexture = SDL_CreateTextureFromSurface(renderer, fireBlue);
     SDL_Surface *fireGreen = IMG_Load("content/fire-green.png");
     SDL_Texture *fireGreenTexture = SDL_CreateTextureFromSurface(renderer, fireGreen);
     //Character Descriptions textures
     SDL_Surface *surfaceMessage, *surfaceMessage2;
     SDL_Texture *Message, *Message2;
     SDL_Surface *image1_description = IMG_Load("content/andrew-description.png");
     SDL_Texture *description_texture1 = SDL_CreateTextureFromSurface(renderer, image1_description);
     SDL_Surface *image2_description = IMG_Load("content/bunny-description.png");
     SDL_Texture *description_texture2 = SDL_CreateTextureFromSurface(renderer, image2_description);
     SDL_Surface *image3_description = IMG_Load("content/matthew-description.png");
     SDL_Texture *description_texture3 = SDL_CreateTextureFromSurface(renderer, image3_description);
     //Game over screen textures
     SDL_Surface *leaderTitleSurface = IMG_Load("content/titleLeaderboard.png");
     SDL_Surface *gameOverTitleSurface = IMG_Load("content/titleGameOver.png");
     SDL_Surface *menuTitleSurface = IMG_Load("content/titleMenu.png");
     SDL_Surface *tryAgainTitleSurface = IMG_Load("content/titleTryAgain.png");
     SDL_Surface *attemptMessageSurface = IMG_Load("content/messageAttempt.png");
     SDL_Surface *scoreMessageSurface = IMG_Load("content/messageScore.png");
     SDL_Surface *highScoreMessageSurface = IMG_Load("content/messageHighScore.png");
     SDL_Surface *newHighscoreSurface = IMG_Load("content/newHighScore.png");
     SDL_Texture *leaderTitleTexture = SDL_CreateTextureFromSurface(renderer, leaderTitleSurface);
     SDL_Texture *gameOverTitleTexture = SDL_CreateTextureFromSurface(renderer, gameOverTitleSurface);
     SDL_Texture *menuTitleTexture = SDL_CreateTextureFromSurface(renderer, menuTitleSurface);
     SDL_Texture *tryAgainTitleTexture = SDL_CreateTextureFromSurface(renderer, tryAgainTitleSurface);
     SDL_Texture *attemptMessageTexture = SDL_CreateTextureFromSurface(renderer, attemptMessageSurface);
     SDL_Texture *scoreMessageTexture = SDL_CreateTextureFromSurface(renderer, scoreMessageSurface);
     SDL_Texture *highScoreMessageTexture = SDL_CreateTextureFromSurface(renderer, highScoreMessageSurface);
     SDL_Texture *newHighscoreTexture = SDL_CreateTextureFromSurface(renderer, newHighscoreSurface);
     //Main Menu screen textures
     SDL_Surface *mainMenuTitleSurface = IMG_Load("content/menuTitle.png");
     SDL_Texture *mainMenuTitleTexture = SDL_CreateTextureFromSurface(renderer, mainMenuTitleSurface);
     SDL_Surface *mainMenuStartSurface = IMG_Load("content/menuStart.png");
     SDL_Texture *mainMenuStartTexture = SDL_CreateTextureFromSurface(renderer, mainMenuStartSurface);
     SDL_Surface *pressBackspace = IMG_Load("content/pressBackspace.png");
     SDL_Texture *pressBackspaceTexture = SDL_CreateTextureFromSurface(renderer, pressBackspace);
     SDL_Surface *pressEnter = IMG_Load("content/pressEnter.png");
     SDL_Texture *pressEnterTexture = SDL_CreateTextureFromSurface(renderer, pressEnter);
     SDL_Surface *selectCharacter = IMG_Load("content/selectCharacter.png");
     SDL_Texture *selectCharacterTexture = SDL_CreateTextureFromSurface(renderer, selectCharacter);
     //Leaderboard textures and text stuff
     char delim[] = " ", delim2[] = "\n", *ptr = (char *) malloc(64), *username = "You";
     SDL_Surface *leaderboardTitleSurface = IMG_Load("content/leaderboardTitle.png");
     SDL_Texture *leaderboardTitleTexture = SDL_CreateTextureFromSurface(renderer, leaderboardTitleSurface);
     //instruction textures
     SDL_Surface *instructionSurface = IMG_Load("content/arrowkeys.png");
     SDL_Texture *instructionTexture = SDL_CreateTextureFromSurface(renderer, instructionSurface);
     SDL_Surface *instructionTextSurface = IMG_Load("content/keystomove.png");
     SDL_Texture *instructionTextTexture = SDL_CreateTextureFromSurface(renderer, instructionTextSurface);
     //main main menu screen stuff
     bool main_menu_test = true, about = false;
     SDL_Surface *mainMenuOptionsSurface = IMG_Load("content/mainMenu.png");
     SDL_Texture *mainMenuOptionsTexture = SDL_CreateTextureFromSurface(renderer, mainMenuOptionsSurface);
     SDL_Surface *blueArrowSurface = IMG_Load("content/arrow.png");
     SDL_Texture *blueArrowTexture = SDL_CreateTextureFromSurface(renderer, blueArrowSurface);
     SDL_Surface *blueArrowSurface2 = IMG_Load("content/arrow2.png");
     SDL_Texture *blueArrowTexture2 = SDL_CreateTextureFromSurface(renderer, blueArrowSurface2);
     SDL_Texture *sprite = NULL, *nameTexture = NULL;
     SDL_Surface *nameSurface = NULL;
     while(running){ //setup complete, starts game loop
         SDL_Delay(time_left());//used to run at the same speed on every device
         next_time += TICK_INTERVAL;
         if(event.type == SDL_QUIT) //if the user clicks the red X to quit
             running = false;//stop running
         if(main_menu){//if the game state is in the main menu
             if(character_description) {//if the user presses enter on a character
                 SDL_RenderClear(renderer);
                 SDL_RenderCopy(renderer, backTexture, NULL, &background_rect2);//copies all the textures into the renderer
                 SDL_RenderCopy(renderer, backTexture, NULL, &Title_background_rect);
                 SDL_RenderCopy(renderer, backTexture, NULL, &Title_background_rect2);
                 SDL_RenderCopy(renderer, nameTexture, NULL, &nameRect);
                 SDL_RenderCopy(renderer, pressBackspaceTexture, NULL, &infoRect);
                 if (sprite1) {  //decides which text and sprite needs to be displayed, then displays it.
                     sprite = sprite1StraightTexture;
                     SDL_RenderCopy(renderer, description_texture1, NULL, &description_image);
                 } else if (sprite2) {
                     sprite = sprite2StraightTexture;
                     SDL_RenderCopy(renderer, description_texture2, NULL, &description_image);
                 } else if (sprite3) {
                     sprite = sprite3StraightTexture;
                     SDL_RenderCopy(renderer, description_texture3, NULL, &description_image);
                 }
                 SDL_RenderCopy(renderer, sprite, NULL, &sprite_rect2);
                 SDL_RenderPresent(renderer);
                 destroyAndFree(nameSurface, nameTexture);
             } else if (main_menu_test){
                 Title_rect.x = 150;
                 Title_rect.y = 2;
                 Title_rect.w = 700;
                 Title_rect.h = 300;
                 SDL_RenderClear(renderer);
                 SDL_RenderCopy(renderer, backTexture, NULL, &background_rect2);//copys created stuff to the renderer
                 SDL_RenderCopy(renderer, backTexture, NULL, &Title_background_rect);
                 SDL_RenderCopy(renderer, backTexture, NULL, &Title_background_rect2);
                 SDL_RenderCopy(renderer, mainMenuOptionsTexture, NULL, &mainMenuRect);
                 SDL_RenderCopy(renderer, mainMenuTitleTexture, NULL, &Title_rect);
                 if(option == 0){
                     arrow.x = 310;
                     arrow.y = 350;
                     arrow2.x = 630;
                     arrow2.y = 350;
                 } else if(option == 1) {
                     arrow.x = 310;
                     arrow.y = 410;
                     arrow2.x = 630;
                     arrow2.y = 410;
                 } else if(option == 2) {
                     arrow.x = 370;
                     arrow.y = 480;
                     arrow2.x = 570;
                     arrow2.y = 480;
                 } else if(option == 3) {
                     arrow.x = 380;
                     arrow.y = 535;
                     arrow2.x = 560;
                     arrow2.y = 535;
                 }
                 arrowAnimation++;
                 if(arr) {
                     arrow.x -= 20;
                     arrow2.x += 20;
                 }
                 if(arrowAnimation == 25){
                     arrowAnimation = 0;
                     if(arr)
                         arr = false;
                     else
                         arr = true;
                 }
                 SDL_RenderCopy(renderer, blueArrowTexture, NULL, &arrow);
                 SDL_RenderCopy(renderer, blueArrowTexture2, NULL, &arrow2);
                 SDL_RenderPresent(renderer);//draws the menu
             } else if (about) {
                 Title_rect.x = 150;
                 Title_rect.y = 2;
                 Title_rect.w = 700;
                 Title_rect.h = 300;
                 SDL_RenderClear(renderer);
                 SDL_RenderCopy(renderer, backTexture, NULL, &background_rect2);//copys created stuff to the renderer
                 SDL_RenderCopy(renderer, backTexture, NULL, &Title_background_rect);
                 SDL_RenderCopy(renderer, backTexture, NULL, &Title_background_rect2);
                 SDL_RenderCopy(renderer, mainMenuTitleTexture, NULL, &Title_rect);
                 //SDL_RenderCopy(renderer, aboutTexture, NULL, &aboutRect);
                 SDL_RenderPresent(renderer);//draws the menu
             }else if (main_menu_screen) {//standard main menu screen
                 counterAdd = 45;
                 Title_rect.x = 150;//resets counter speed and the title rect location
                 Title_rect.y = 2;
                 Title_rect.w = 700;
                 Title_rect.h = 300;
                 menuCounter += 2;//used to animate stuff on the main screen
                 fallCounter += 2;
                 backCounter += 6;
                 backCounter2 += 6;
                 if(fallCounter >= 500) {//a timer to decide when the character should fall in the background
                     fall = true;
                     fallCounter = 0;
                 }
                 if(starty == 605)//decides if we need to add or minus from the 'start' animation
                     add = false;
                 else if(starty == 575)
                     add = true;
                 SDL_RenderClear(renderer);
                 SDL_RenderCopy(renderer, backTexture, NULL, &background_rect2);//copys created stuff to the renderer
                 SDL_RenderCopy(renderer, backTexture, NULL, &Title_background_rect);
                 SDL_RenderCopy(renderer, backTexture, NULL, &Title_background_rect2);
                 SDL_RenderCopy(renderer, sprite1FallingTexture, NULL, &falling_rect);
                 SDL_RenderCopy(renderer, sprite1StraightTexture, NULL, &sprite1_rect);
                 SDL_RenderCopy(renderer, sprite2StraightTexture, NULL, &sprite2_rect);
                 SDL_RenderCopy(renderer, sprite3StraightTexture, NULL, &sprite3_rect);
                 SDL_RenderCopy(renderer, border, NULL, &border_rect);
                 SDL_RenderCopy(renderer, mainMenuTitleTexture, NULL, &Title_rect);
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
                 if(fall)//moves the falling character
                     fally += 3;
                 if(position == 185) {//determines which sprite is going to be used
                     sprite1 = true;
                     sprite2 = false;
                     sprite3 = false;
                 } else if (position == 385) {
                     sprite1 = false;
                     sprite2 = true;
                     sprite3 = false;
                 } else if(position == 585){
                     sprite1 = false;
                     sprite2 = false;
                     sprite3 = true;
                 }
                 SDL_RenderCopy(renderer, selectCharacterTexture, NULL, &pick_rect);
                 SDL_RenderCopy(renderer, mainMenuStartTexture, NULL, &MainMenu_rect);
                 SDL_RenderCopy(renderer, pressEnterTexture, NULL, &enter_rect);
                 SDL_RenderPresent(renderer);//draws the menu
             }
             while(SDL_PollEvent(&event)) {
                 if(event.type == SDL_KEYDOWN){
                     if(event.key.keysym.sym == SDLK_RETURN) {//look at current character
                         main_menu_screen = false;
                         if(main_menu_test == false)
                             character_description = true;
                     }
                     if(event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_RETURN){//start game
                        if(main_menu_test == true){
                            if(option == 0){
                                main_menu_test = false;
                                main_menu_screen = true;
                            } else if(option == 1) {
                                main_menu = false;
                                leaderboard = true;
                            } else if(option == 2) {
                                about = true;
                                main_menu_test = false;
                            } else if(option == 3)
                                exit(0);
                        } else if (main_menu_screen){
                            main_menu = false;
                            first_loop = true;
                        }
                     }
                     if(event.key.keysym.sym == SDLK_BACKSPACE) {//return from current character
                         if(character_description){
                             character_description = false;
                             main_menu_screen = true;
                         } else if (main_menu_screen || about){
                             main_menu_screen = false;
                             about = false;
                             main_menu_test = true;
                             option = 0;
                         }
                     }
                     if(event.key.keysym.sym == SDLK_RIGHT) {//move between characters
                         if(position != 585)
                             position += 200;
                     }
                     if(event.key.keysym.sym == SDLK_LEFT) {//move between characters
                         if(position != 185)
                             position -= 200;
                     }
                     if(event.key.keysym.sym == SDLK_UP) {
                         option--;
                         if(option == -1)
                             option = 3;
                     }
                     if(event.key.keysym.sym == SDLK_DOWN) {
                         option++;
                         if(option == 4)
                             option = 0;
                     }
                 }
             }
         } else {
             if(game_over == true) {//if you lose
                 counterAdd = 45;
                 menuCounterGameOver += 2;
                 sprintf(array, "%d", score);//gets current score
                 move_left = false;
                 move_right = false;
                 move_up = false;
                 move_down = false;
                 if(startyGameOver == 560)
                     addGameOver = false;//decides if we need to add or minus from the 'start' animation
                 else if(startyGameOver == 530)
                     addGameOver = true;
                 SDL_Rect tryAgainTitleRect = {startx-40, startyGameOver+40, 500, 70};
                 if(addGameOver){//animates the 'start' text
                     if(menuCounterGameOver == 4) {
                         startyGameOver += 1;
                         menuCounterGameOver = 0;
                     }
                 } else {
                     if(menuCounterGameOver == 4) {
                         startyGameOver -= 1;
                         menuCounterGameOver = 0;
                     }
                 }
                 SDL_RenderClear(renderer);
                 if(first_game_over) {
                     first_game_over = false;
                     bool been_here = false;
                     int temp = 0;
                     char *array4 = (char *) malloc(10 * sizeof(char));

                     SDL_RWops* fp = SDL_RWFromFile("content/score.txt", "r" );

                     for (int i = 0; i < 1; ++i) {
                         SDL_RWread(fp, &array4[i], sizeof(Sint64)*2, 5);
                         if(strncmp(&array4[i], "x", 1)) {
                             break;
                         }
                     }
                     SDL_RWclose(fp);
                     SDL_RWops* fp2 = SDL_RWFromFile("content/score.txt", "w" );
                     char* newLine = strtok(array4, delim2);
                     while (newLine != NULL) {
                         if(newLine[0] == 'x') {
                             break;
                         }
                         char* next = strchr(newLine, ' ');
                         int x = atoi(next);
                         if(score < x) {
                             char *newLine2 = (char *) malloc(10 * sizeof(char));
                             strcpy(newLine2, newLine);
                             if(temp == 5) {
                                 strcat(newLine2, "\nx");
                             } else {
                                 strcat(newLine2, "\n");
                             }
                             SDL_RWwrite(fp2, newLine2, strlen(newLine2), 1);
                             free(newLine2);
                             temp++;
                         } else {
                             if(been_here == false) {
                                 char *newLine3 = (char *) malloc(10 * sizeof(char));
                                 char *buffer = (char *) malloc(10 * sizeof(char));
                                 strcpy(newLine3, username);
                                 strcat(newLine3, " ");
                                 sprintf(buffer, "%d", score);
                                 strcat(newLine3, buffer);
                                 if(temp == 4 ) {
                                     strcat(newLine3, "\nx");
                                 } else {
                                     strcat(newLine3, "\n");
                                 }
                                 SDL_RWwrite(fp2, newLine3, strlen(newLine3), 1);
                                 free(newLine3);
                                 free(buffer);
                                 temp++;
                                 if(temp != 5) {
                                     char *newLine2 = (char *) malloc(10 * sizeof(char));
                                     strcpy(newLine2, newLine);
                                     strcat(newLine2, "\n");
                                     SDL_RWwrite(fp2, newLine2, strlen(newLine2), 1);
                                     free(newLine2);
                                     temp++;
                                     if(temp == 5) {
                                         char *newLine4 = (char *) malloc(10 * sizeof(char));
                                         strcpy(newLine4, "x");
                                         SDL_RWwrite(fp2, newLine4, strlen(newLine4), 1);
                                         free(newLine4);
                                     }
                                 }
                                 been_here = true;
                             } else {
                                 if(temp != 5) {
                                     char *newLine2 = (char *) malloc(10 * sizeof(char));
                                     strcpy(newLine2, newLine);
                                     strcat(newLine2, "\n");
                                     SDL_RWwrite(fp2, newLine2, strlen(newLine2), 1);
                                     free(newLine2);
                                     temp++;
                                     if(temp == 5) {
                                         char *newLine4 = (char *) malloc(10 * sizeof(char));
                                         strcpy(newLine4, "x");
                                         SDL_RWwrite(fp2, newLine4, strlen(newLine4), 1);
                                         free(newLine4);
                                     }
                                 }
                             }
                             showHighScore = true;
                         }
                         newLine = strtok(NULL, delim2);
                     }
                     free(array4);
                     SDL_RWclose(fp2);
                    }
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
                    if(showHighScore == true) {
                        if(count < 100) {
                            SDL_RenderCopy(renderer, newHighscoreTexture, NULL, &gFlashClips);
                            count+=2;
                        } else {
                            count2-= 2;
                            if(count2 == 0){
                                count = 0;
                                count2 = 100;
                            }
                        }
                    }
                 if(showHighScore == true) {
                     if(count < 100) {
                         SDL_RenderCopy(renderer, newHighscoreTexture, NULL, &gFlashClips);
                         count+=2;
                     } else {
                         count2-= 2;
                         if(count2 == 0){
                             count = 0;
                             count2 = 100;
                         }
                     }
                 }
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
                         main_menu_test = true;
                     }
                     if(event.key.keysym.sym == SDLK_l) {//goes to leaderboard
                         game_over = false;
                         main_menu = false;
                         leaderboard = true;
                     }
                 }
                 if(game_over == false && leaderboard == false) {//reset variables
                     score = 0;
                     x = 475;
                     y = 250;
                     oby = -100;
                     ob2y = 10000;
                     first_time = true;
                     mode = 0;
                     wallLeftX = -800;
                     wallRightX = 800;
                     wall_rect.x = wallLeftX;
                     wall_rect2.x = wallRightX;
                     wall_rect1_2.x = wallLeftX;
                     wall_rect2_2.x = wallRightX;
                     first_game_over = true;
                 }
             } else if(leaderboard == true) {
                 if(reading_first_time) {
                     reading_first_time = false;
                     char first_char;
                     SDL_RenderClear(renderer);
                     SDL_Surface* scoreListSurface = NULL;
                     SDL_Texture* scoreListTexture = NULL;
                     SDL_Rect game_over_back3 = {0, 0, 1000, 700};
                     int positionY = 200;
                     SDL_RenderCopy(renderer, backTexture, NULL, &game_over_back3);
                     SDL_RenderCopy(renderer, leaderboardTitleTexture, NULL, &leaderboardTitleRect);

                     char *array4;
                     array4 = (char *) malloc(10 * sizeof(char));

                     SDL_RWops* fp = SDL_RWFromFile("content/score.txt", "r" );

                     for (int i = 0; i < 1; ++i) {
                         SDL_RWread(fp, &array4[i], sizeof(Sint64)*2, 5);
                         if(strncmp(&array4[i], "x", 1)) {
                             break;
                         }
                     }

                     char* newLine = strtok(array4, delim2);

                     while (newLine != NULL) {
                         if(newLine[0] == 'x') {
                             break;
                         }
                         SDL_Rect score_rect = {350, positionY, 300, 50};
                         first_char = array4[0];
                         if(first_char == 'Y' || first_char == 'B') {
                             score_rect.x = 400;
                             score_rect.w = 200;
                         }
                         scoreListSurface = TTF_RenderText_Solid(font, newLine, textColor);
                         scoreListTexture = SDL_CreateTextureFromSurface(renderer, scoreListSurface);
                         SDL_RenderCopy(renderer, scoreListTexture, NULL, &score_rect);
                         positionY += 75;
                         newLine = strtok(NULL, delim2);
                     }
                     SDL_RWclose(fp);
                     SDL_Rect infoRect2 = {345, 580, 320, 50};
                     SDL_RenderCopy(renderer, pressBackspaceTexture, NULL, &infoRect2);
                     SDL_RenderPresent(renderer);
                     destroyAndFree(scoreListSurface, scoreListTexture);
                 }
                 while(SDL_PollEvent(&event)) {
                     if(event.key.keysym.sym == SDLK_BACKSPACE) {//start again
                         if(main_menu_test == true){
                             leaderboard = false;
                             main_menu = true;
                             reading_first_time = true;
                         } else {
                             game_over = true;
                             leaderboard = false;
                             main_menu = false;
                             reading_first_time = true;
                         }
                     }
                 }
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
                 if(sprite1 == true)
                     spriteFallingTexture = sprite1FallingTexture;//selects the correct sprite that the player chose
                 else if (sprite2 == true)
                     spriteFallingTexture = sprite2FallingTexture;
                 else if (sprite3 == true)
                     spriteFallingTexture = sprite3FallingTexture;
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
                     counter = 0; //code below moves the walls and background for the first loop animation
                     background_rect.y -= 3;
                     if(background_rect.y <= -700)
                         background_rect.y = 700;
                     SDL_RenderCopy(renderer, backTexture, NULL, &background_rect);
                     counter2 = 0;
                     background_rect2.y -= 3;
                     Title_rect.y -= 3;
                     if(background_rect2.y <= -700)
                         background_rect2.y = 700;
                     SDL_RenderCopy(renderer, backTexture, NULL, &background_rect2);
                     wall_rect.y -= 3;
                     if(wall_rect.y <= -700)
                         wall_rect.y = 700;
                     SDL_RenderCopy(renderer, texture, NULL, &wall_rect);
                     wall_rect2.y -= 3;
                     counter3 = 0;
                     if(wall_rect2.y <= -700)
                         wall_rect2.y = 700;
                     SDL_RenderCopy(renderer, texture, NULL, &wall_rect2);
                     wall_rect1_2.y -= 3;
                     if(wall_rect1_2.y <= -1400)
                         wall_rect1_2.y = 700;
                     SDL_RenderCopy(renderer, texture, NULL, &wall_rect1_2);
                     counter4 = 0;
                     wall_rect2_2.y -= 3;
                     if(wall_rect2_2.y <= -1400)
                         wall_rect2_2.y = 700;
                     SDL_RenderCopy(renderer, texture, NULL, &wall_rect2_2);
                     SDL_RenderCopy(renderer, mainMenuTitleTexture, NULL, &Title_rect);
                     SDL_RenderCopy(renderer, instructionTexture, NULL, &instructionRect);
                     SDL_RenderCopy(renderer, instructionTextTexture, NULL, &instructionTextRect);
                     if(count > 1500) {//enough time has passed, start the character falling
                         SDL_Rect falling_rect = {fallx, fally, 80, 80};
                         SDL_RenderCopy(renderer, spriteFallingTexture, NULL, &falling_rect);
                         if(fally != 250)
                             fally += 2;
                         if(fally >= 250) {
                             first_loop = false;
                             counter3 = 0.0;
                             counter4 = 0.0;
                         }
                     }
                     SDL_RenderPresent(renderer);
                 }//animation ends here
                 while(SDL_PollEvent(&event)){
                     if(event.type == SDL_KEYUP) { //determines which way to move the character based on the key presses
                         if(event.key.keysym.sym == SDLK_RIGHT)
                             move_right = false;
                         if(event.key.keysym.sym == SDLK_LEFT)
                             move_left = false;
                         if(event.key.keysym.sym == SDLK_DOWN)
                             move_down = false;
                         if(event.key.keysym.sym == SDLK_UP)
                             move_up = false;
                     }
                     if(event.type == SDL_KEYDOWN) {//code below makes movements smooth, if a key is held down
                         if(event.key.keysym.sym == SDLK_RIGHT) {
                             move_right = true;
                             rightmove = true;
                         }
                         if(event.key.keysym.sym == SDLK_LEFT) {
                             move_left = true;
                             rightmove = false;
                         }
                         if(event.key.keysym.sym == SDLK_DOWN)
                             move_down = true;
                         if(event.key.keysym.sym == SDLK_UP)
                             move_up = true;
                     }
                 }
                 if(move_left)//if left is pressed, move object left and so on.
                     x -= 5;
                 if(move_right)
                     x += 5;
                 if(move_up)
                     y -= 5;
                 if(move_down)
                     y += 5;
                 counter += counterAdd;//increment counters
                 counter2 += counterAdd;
                 counter3 += counterAdd;
                 counter4 += counterAdd;
                 if(score == 2000 || score == 4000 || score == 6000 || score == 8000 || score == 10000)
                     counterAdd += 6;//if score has increased by a good amount increase falling speed
                 if(x < wallLeftX + 1000 || x > wallRightX - 80) {//detects if a player has hit a wall
                     game_over = true;
                     showHighScore = false;
                 }
                 if(y > 700 || y < 0) {//detects if a player goes to high or low
                     game_over = true;
                     showHighScore = false;
                 }
                 if(counter >= 100) { //moves the background
                     background_rect.y -= 2;
                     counter = 0;
                 }
                 if(background_rect.y <= -700)
                     background_rect.y = 700;
                 SDL_RenderCopy(renderer, backTexture, NULL, &background_rect);
                 if(counter2 >= 100) {
                     counter2 = 0;
                     background_rect2.y -= 2;
                 }
                 if(background_rect2.y <= -700)
                     background_rect2.y = 700;
                 SDL_RenderCopy(renderer, backTexture, NULL, &background_rect2);
                 SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);//draws a rectangle and decides which way it should face
                 SDL_Rect rect = {x, y, 80, 80};
                 SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                 SDL_RenderFillRect(renderer, &rect);
                 if(rightmove) {
                     float angle = 0.0;
                     SDL_Point center = {8, 8};
                     SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
                     SDL_RenderCopyEx(renderer, spriteFallingTexture, NULL , &rect, angle, &center, flip);
                 } else
                     SDL_RenderCopy(renderer, spriteFallingTexture, NULL, &rect);
                 if(counter3 > 50)  //moves side walls
                     wall_rect.y -= 4;
                 if(wall_rect.y <= -700)
                     wall_rect.y = 700;
                 SDL_RenderCopy(renderer, texture, NULL, &wall_rect);
                 if(counter3 > 50){
                     wall_rect2.y -= 4;
                     counter3 = 0;
                 }
                 if(wall_rect2.y <= -700)
                     wall_rect2.y = 700;
                 SDL_RenderCopy(renderer, texture, NULL, &wall_rect2);
                 if(counter4 > 50)
                     wall_rect1_2.y -= 4;
                 if(wall_rect1_2.y <= -700)
                     wall_rect1_2.y = 700;
                 SDL_RenderCopy(renderer, texture, NULL, &wall_rect1_2);
                 if(counter4 > 50) {
                     counter4 = 0;
                     wall_rect2_2.y -= 4;
                 }
                 if(wall_rect2_2.y <= -700)
                     wall_rect2_2.y = 700;
                 SDL_RenderCopy(renderer, texture, NULL, &wall_rect2_2);
                 score++;//adds 1 to the players current score
                 if(score % 1000 == 0)//swap between walls closing in and objects
                     switchModes = true;
                 if(mode == 0) {//mode 0 so objects
                     oby -= speed;
                     ob2y -= speed;
                     if(!switchModes) {
                         for(int i=0; i<6; i++){
                             if(x + 20 >= fireObjectsX[i] && x <= fireObjectsX[i] + 100) {//collision detection for fireballs
                                 if(i < 3){
                                     if(oby + 50 >= y && oby <= y + 50) {
                                         game_over = true;
                                         showHighScore = false;
                                     }
                                 } else {
                                     if(ob2y + 50 >= y && ob2y <= y + 50) {
                                         game_over = true;
                                         showHighScore = false;
                                     }
                                 }
                             }
                         }
                         if(oby < -78) {//resets the objects to be used again once off screen
                             Mix_PlayChannel(-1, fireball, 0);
                             while(true){
                                 fireObjectsX[0] = (rand() % 480) + 200;
                                 fireObjectsX[1] = (rand() % 480) + 200;
                                 fireObjectsX[2] = (rand() % 480) + 200;
                                 oby = 700;
                                 if(fireObjectsX[0] - fireObjectsX[1] > 120 || fireObjectsX[1] - fireObjectsX[0] > 120){
                                     if(fireObjectsX[0] - fireObjectsX[2] > 120 || fireObjectsX[2] - fireObjectsX[0] > 120) {
                                         if(fireObjectsX[1] - fireObjectsX[2] > 120 || fireObjectsX[2] - fireObjectsX[1] > 120)
                                             break;
                                     }
                                 }
                             }
                         }
                         if(ob2y < -78 || (first_time && oby <= 350)) {//resets the objects to be used again once off screen
                             first_time = false;
                             Mix_PlayChannel(-1, fireball, 0);
                             while(true){
                                 fireObjectsX[3] = (rand() % 480) + 200;
                                 fireObjectsX[4] = (rand() % 480) + 200;
                                 fireObjectsX[5] = (rand() % 480) + 200;
                                 ob2y = 700;
                                 if(fireObjectsX[3] - fireObjectsX[4] > 120 || fireObjectsX[4] - fireObjectsX[3] > 120){
                                     if(fireObjectsX[3] - fireObjectsX[5] > 120 || fireObjectsX[5] - fireObjectsX[3] > 120) {
                                         if(fireObjectsX[4] - fireObjectsX[5] > 120 || fireObjectsX[5] - fireObjectsX[4] > 120)
                                             break;
                                     }
                                 }
                             }
                         }
                     } else {
                         if(oby <= -50 && ob2y <= -50) {
                             mode = 1;
                             oby = 700;
                             ob2y = 1050;
                             setup = true;
                             switchModes = false;
                         }
                     }
                     SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); //draws the fireballs
                     SDL_Rect rect_blue_1 = {fireObjectsX[0], oby, 30, 78};
                     SDL_RenderCopy(renderer, fireBlueTexture, &source_rect_blue, &rect_blue_1);
                     SDL_Rect rect_blue_2 = {fireObjectsX[0]+30, oby, 30, 78};
                     SDL_RenderCopy(renderer, fireBlueTexture, &source_rect_blue, &rect_blue_2);
                     SDL_Rect rect_blue_3 = {fireObjectsX[0]+60, oby, 30, 78};
                     SDL_RenderCopy(renderer, fireBlueTexture, &source_rect_blue, &rect_blue_3);
                     SDL_Rect rect_blue_4 = {fireObjectsX[0]+90, oby, 30, 78};
                     SDL_RenderCopy(renderer, fireBlueTexture, &source_rect_blue, &rect_blue_4);
                     SDL_Rect rect_blue_5 = {fireObjectsX[3], ob2y, 30, 78};
                     SDL_RenderCopy(renderer, fireBlueTexture, &source_rect_blue, &rect_blue_5);
                     SDL_Rect rect_blue_6 = {fireObjectsX[3]+30, ob2y, 30, 78};
                     SDL_RenderCopy(renderer, fireBlueTexture, &source_rect_blue, &rect_blue_6);
                     SDL_Rect rect_blue_7 = {fireObjectsX[3]+60, ob2y, 30, 78};
                     SDL_RenderCopy(renderer, fireBlueTexture, &source_rect_blue, &rect_blue_7);
                     SDL_Rect rect_blue_8 = {fireObjectsX[3]+90, ob2y, 30, 78};
                     SDL_RenderCopy(renderer, fireBlueTexture, &source_rect_blue, &rect_blue_8);
                     SDL_Rect rect_green_1 = {fireObjectsX[1], oby, 30, 78};
                     SDL_RenderCopy(renderer, fireGreenTexture, &source_rect_green, &rect_green_1);
                     SDL_Rect rect_green_2 = {fireObjectsX[1]+30, oby, 30, 78};
                     SDL_RenderCopy(renderer, fireGreenTexture, &source_rect_green, &rect_green_2);
                     SDL_Rect rect_green_3 = {fireObjectsX[1]+60, oby, 30, 78};
                     SDL_RenderCopy(renderer, fireGreenTexture, &source_rect_green, &rect_green_3);
                     SDL_Rect rect_green_4 = {fireObjectsX[1]+90, oby, 30, 78};
                     SDL_RenderCopy(renderer, fireGreenTexture, &source_rect_green, &rect_green_4);
                     SDL_Rect rect_green_5 = {fireObjectsX[4], ob2y, 30, 78};
                     SDL_RenderCopy(renderer, fireGreenTexture, &source_rect_green, &rect_green_5);
                     SDL_Rect rect_green_6 = {fireObjectsX[4]+30, ob2y, 30, 78};
                     SDL_RenderCopy(renderer, fireGreenTexture, &source_rect_green, &rect_green_6);
                     SDL_Rect rect_green_7 = {fireObjectsX[4]+60, ob2y, 30, 78};
                     SDL_RenderCopy(renderer, fireGreenTexture, &source_rect_green, &rect_green_7);
                     SDL_Rect rect_green_8 = {fireObjectsX[4]+90, ob2y, 30, 78};
                     SDL_RenderCopy(renderer, fireGreenTexture, &source_rect_green, &rect_green_8);
                     SDL_Rect rect_red_1 = {fireObjectsX[2], oby, 30, 78};
                     SDL_RenderCopy(renderer, fireRedTexture, &source_rect_red, &rect_red_1);
                     SDL_Rect rect_red_2 = {fireObjectsX[2]+30, oby, 30, 78};
                     SDL_RenderCopy(renderer, fireRedTexture, &source_rect_red, &rect_red_2);
                     SDL_Rect rect_red_3 = {fireObjectsX[2]+60, oby, 30, 78};
                     SDL_RenderCopy(renderer, fireRedTexture, &source_rect_red, &rect_red_3);
                     SDL_Rect rect_red_4 = {fireObjectsX[2]+90, oby, 30, 78};
                     SDL_RenderCopy(renderer, fireRedTexture, &source_rect_red, &rect_red_4);
                     SDL_Rect rect_red_5 = {fireObjectsX[5], ob2y, 30, 78};
                     SDL_RenderCopy(renderer, fireRedTexture, &source_rect_red, &rect_red_5);
                     SDL_Rect rect_red_6 = {fireObjectsX[5]+30, ob2y, 30, 78};
                     SDL_RenderCopy(renderer, fireRedTexture, &source_rect_red, &rect_red_6);
                     SDL_Rect rect_red_7 = {fireObjectsX[5]+60, ob2y, 30, 78};
                     SDL_RenderCopy(renderer, fireRedTexture, &source_rect_red, &rect_red_7);
                     SDL_Rect rect_red_8 = {fireObjectsX[5]+90, ob2y, 30, 78};
                     SDL_RenderCopy(renderer, fireRedTexture, &source_rect_red, &rect_red_8);
                 } else {//mode 2, so walls
                     int speed2 = 3;//sets the speed the walls will move
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
                                 pos = rand() % 4;
                             }
                         } else {//setup finished so start wall stage
                             wall_rect.x = wallLeftX;
                             wall_rect2.x = wallRightX;
                             wall_rect1_2.x = wallLeftX;
                             wall_rect2_2.x = wallRightX;
                             float wallPos[] = {100, 300, 500, 700}; //now that stage has begun, walls will randomly generate a position and move to that position
                             if(wallLeftX < wallPos[pos] - 1000) {
                                 wallLeftX += speed2;
                                 wallRightX += speed2;
                                 if(wallLeftX >= wallPos[pos] - 1000)
                                     pos = rand() % 4;
                             } else {
                                 wallLeftX -= speed2;
                                 wallRightX -= speed2;
                                 if(wallLeftX <= wallPos[pos] - 1000)
                                     pos = rand() % 4;
                             }
                         }
                         if(score % 1000 == 0)//score reached 1000 more than the last stage, so switch
                             switchModes = true;
                     } else {//switches back to objects by returning walls to their original position
                         if(wallLeftX > -800)
                             wallLeftX -= speed2;
                         else if (wallLeftX < -800)
                             wallLeftX += speed2;
                         if(wallRightX < 800)
                             wallRightX += speed2;
                         else if (wallRightX > 800)
                             wallRightX -= speed2;
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
                 sprintf(array, "%d", score);//gets the current score and displays it at the top of the screen
                 surfaceMessage = TTF_RenderText_Solid(font2, array, textColor);
                 surfaceMessage2 = TTF_RenderText_Solid(font, "Score: ", textColor);
                 Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
                 Message2 = SDL_CreateTextureFromSurface(renderer, surfaceMessage2);
                 SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
                 SDL_RenderCopy(renderer, Message2, NULL, &Message_rect2);
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
