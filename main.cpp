#include <SDL2/SDL.h>
#include <SDL_image/SDL_image.h>
#include <SDL_ttf/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "LTimer.h"
#include "Game.h"
#include "LTexture.h"

#define DEBUG_MODE 1
//#define PRINT_FPS_TO_STDOUT 1

#define mPrint(s) (std::cout << lastPathComponent(__FILE__) << ":" << __LINE__ << ": " << s << std::endl)
#define WhiteColor24 {0xff,0xff,0xff}
#define WhiteColor32 {0xff,0xff,0xff,0xff}

/*
const int ArrowKeyLeft  = 1073741904;
const int ArrowKeyRight = 1073741903;
const int ArrowKeyUp    = 1073741906;
const int ArrowKeyDown  = 1073741905;
*/

const int DefaultScreenWidth  = 640; 
const int DefaultScreenHeight = 480;

bool renderDebugPanelOn = true;

//graphics management functions
bool graphicsInit();
bool loadMedia();
void close();
void renderFrame();

//file path functions
std::string lastPathComponent(const std::string& str);

SDL_Window   *gWindow        = NULL;
SDL_Surface  *gScreenSurface = NULL;
SDL_Renderer *gRenderer      = NULL;
SDL_Texture  *gTexture       = NULL;

Game game;
LTexture gFPSTexture;
LTexture gFrameCountTexture;
TTF_Font *gDebugPanelFont;
int frameCount  = 0;
float avgFPS;

int main( int argc, char* args[] ) {
    if (!graphicsInit()) {
        printf("Failed to init");
    }
    else {
        if (!loadMedia()) {
            printf("Failed to load media");
        }
        else {
            bool quit = false;
            LTimer fpsTimer;
            SDL_Event e;
            frameCount = 0;
            fpsTimer.start();
            while (!quit) {
                avgFPS = frameCount / (fpsTimer.getTicks() / 1000.0f);
                if (avgFPS > 2000000) {
                    avgFPS = 0;
                }
                while (SDL_PollEvent(&e)!=0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                    else if (e.type == SDL_KEYDOWN) {
                        SDL_Keycode keycode = e.key.keysym.sym;
                        //Uint16 mod = e.key.keysym.mod;
                        //checks for left shift, right shift, or capslock
                        //bool is_uppercase = mod == KMOD_LSHIFT || mod == KMOD_RSHIFT || mod == KMOD_CAPS;
                        if (keycode == 'd') { 
                            renderDebugPanelOn = !renderDebugPanelOn;
                        }
                    }
                }
                renderFrame();
#ifdef PRINT_FPS_TO_STDOUT
                mPrint("avg fps: " + std::to_string(avgFPS));
#endif
            }
        }
    }
    close();
    return 0;
}

bool graphicsInit() {
    bool success = true;
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        printf("SDL could not initialize: %s\n", SDL_GetError());
        success = false;
    }
    else {
        gWindow = SDL_CreateWindow(game.getName().c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DefaultScreenWidth, DefaultScreenHeight, SDL_WINDOW_SHOWN);
        if (!gWindow) {
            printf("Window could not be created!: %s\n", SDL_GetError());
            success = false;
        }
        else {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (!gRenderer) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
            }
            else {
                SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
                int imgFlags = IMG_INIT_PNG;
                if ( ! (IMG_Init(imgFlags) & imgFlags) ) {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
                if ( TTF_Init() == -1 ) {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    success = false;
                }
                if (success) {
                    gScreenSurface = SDL_GetWindowSurface(gWindow);
                }
            }
        }
    }
    return success;
}

bool loadMedia() {
    bool success = true;
    //load the debug panel font
    SDL_Color textColor = WhiteColor24;
    int fontSize = 14;
    std::string fontName = "Terminus.ttf";
    gDebugPanelFont = TTF_OpenFont(fontName.c_str(), fontSize);
    if ( gDebugPanelFont == NULL ) {
        printf("Failed to load font!\n");
        success = false;
    }
    else {
        //load the debug panel
        gFPSTexture.setFont(gDebugPanelFont);
        gFPSTexture.setRenderer(gRenderer);
        gFrameCountTexture.setFont(gDebugPanelFont);
        gFrameCountTexture.setRenderer(gRenderer);
    }
    if ( !gFPSTexture.loadFromRenderedText( "FPS: ", textColor ) ) {
        printf( "Failed to load text texture!\n" );
        success = false;
    }
    if ( !gFrameCountTexture.loadFromRenderedText( "Frame: ", textColor ) ) {
        printf( "Failed to load text texture!\n" );
        success = false;
    }
    return success;
}

void close() {
    // destroy the texture, renderer, window, and set them to null
    SDL_DestroyTexture(gTexture);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gTexture = NULL;
    gRenderer = NULL;
    gWindow = NULL;
    // close the debug panel font and free the texture
    TTF_CloseFont( gDebugPanelFont );
    gFPSTexture.free();
    gFrameCountTexture.free();
    // have to quit TTF, IMG, and SDL
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

#pragma mark - Drawing Functions

void renderFrame() {
    //clear screen
    int r = 0xff;
    int g = 0xff;
    int b = 0xff;
    int a = 0xff;
    int paddingX = 20;
    int paddingY = 20;
    SDL_SetRenderDrawColor(gRenderer, r, g, b, a);
    SDL_RenderClear(gRenderer);

    if (renderDebugPanelOn) {
        //render debug panel
        //render black box
        r = 0;
        g = 0;
        b = 0;
        a = 0xff;
        SDL_SetRenderDrawColor(gRenderer, r, g, b, a);
        SDL_Rect rect;
        rect.x = paddingX;
        rect.y = paddingY;
        rect.w = 200;
        rect.h = 100;
        SDL_RenderFillRect(gRenderer, &rect);

        //render fps text
        paddingX += 20;
        paddingY += 20;
        SDL_Color textColor = WhiteColor32;
        gFPSTexture.loadFromRenderedText( "FPS:   " + std::to_string(avgFPS), textColor);
        gFPSTexture.render( paddingX, paddingY );
        
        //render frame count text
        paddingX += 0;
        paddingY += 20;
        gFrameCountTexture.loadFromRenderedText( "Frame: " + std::to_string(frameCount), textColor);
        gFrameCountTexture.render( paddingX, paddingY );
    }
    
    //present the renderer
    SDL_RenderPresent(gRenderer);
    frameCount++;
}

#pragma mark - File Path Functions

std::string lastPathComponent(const std::string& str) {
    return str.substr(str.find_last_of("/\\")+1);
}

