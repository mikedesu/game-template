//
//  main.cpp
//  rpg-game
//
//  Created by darkmage on 5/25/15.
//  Copyright (c) 2015 Mike Bell. All rights reserved.
//

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
#define PRINT_FPS_TO_STDOUT 1

#define mPrint(s) (std::cout << lastPathComponent(__FILE__) << ":" << __LINE__ << ": " << s << std::endl)

const int ArrowKeyLeft  = 1073741904;
const int ArrowKeyRight = 1073741903;
const int ArrowKeyUp    = 1073741906;
const int ArrowKeyDown  = 1073741905;

const int DefaultNumRectsWide   = 32;
const int DefaultNumRectsHigh   = 24;
const int DefaultScreenWidth    = 640;
const int DefaultScreenHeight   = 480;

int NumRectsWide = DefaultNumRectsWide;
int NumRectsHigh = DefaultNumRectsHigh;

int ScreenWidth  = DefaultScreenWidth;
int ScreenHeight = DefaultScreenHeight;

#define RECT_WIDTH  (ScreenWidth/NumRectsWide)
#define RECT_HEIGHT (ScreenHeight/NumRectsHigh)

int RectWidth   = RECT_WIDTH;
int RectHeight  = RECT_HEIGHT;

int frameCount  = 0;

//graphics management functions
bool graphics_init();
bool loadMedia();
void close();
SDL_Texture *loadTexture(std::string path);
void drawBlockInCenter(int r, int g, int b, int a);
void clearScreen();
void renderFrame();

//file path functions
std::string filePathComponent(const std::string& str);
std::string lastPathComponent(const std::string& str);

SDL_Window   *gWindow        = NULL;
SDL_Surface  *gScreenSurface = NULL;
SDL_Renderer *gRenderer      = NULL;
SDL_Texture  *gTexture       = NULL;

LTexture gTextTexture;
Game game;
TTF_Font *gFont;

float avgFPS;

int main( int argc, char* args[] ) {
    if (!graphics_init()) {
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
                        Uint16 mod = e.key.keysym.mod;
                        // checks for left shift, right shift, or capslock
                        bool is_uppercase = mod == KMOD_LSHIFT || mod == KMOD_RSHIFT || mod == KMOD_CAPS;
                        if (keycode == ArrowKeyLeft) {
                            //cameraMoveLeft();
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

bool graphics_init() {
    bool success = true;
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        printf("SDL could not initialize: %s\n", SDL_GetError());
        success = false;
    }
    else {
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DefaultScreenWidth, DefaultScreenHeight, SDL_WINDOW_SHOWN);
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
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
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
#ifdef DEBUG
    printf( "loadMedia()\n" );
#endif
    bool success = true;
    SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
    int fontSize = 14;
#ifdef DEBUG
        printf( "opening font...\n" );
#endif
    gFont = TTF_OpenFont("Terminus.ttf", fontSize);
    if ( gFont == NULL ) {
        printf("Failed to load font!\n");
        success = false;
    }
    else {
        gTextTexture.setFont(gFont);
        gTextTexture.setRenderer(gRenderer);
    }
    if( !gTextTexture.loadFromRenderedText( "FPS: ", textColor ) ) {
        printf( "Failed to load text texture!\n" );
        success = false;
    }
    return success;
}

void close() {
    SDL_DestroyTexture(gTexture);
    
    gTexture = NULL;
    
    SDL_DestroyRenderer(gRenderer);
    
    gRenderer = NULL;
    
    SDL_DestroyWindow(gWindow);
    
    gWindow = NULL;
    
    TTF_CloseFont( gTextTexture.getFont() );
    
    gTextTexture.free();

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture *loadTexture(std::string path) {
    SDL_Texture *newTexture = NULL;
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (!loadedSurface) {
        printf("Unable to load surface. SDL Error: %s\n", SDL_GetError());
    }
    else {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (!newTexture) {
            printf("Unable to create texture. SDL Error: %s\n", SDL_GetError());
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

#pragma mark - Key Press Info Functions
void print_key_info(int keycode) {
    if (keycode == ArrowKeyLeft) {
        printf("Arrow Key Left\n");
    }
    // Right
    else if (keycode == ArrowKeyRight) {
        printf("Arrow Key Right\n");
    }
    // Up
    else if (keycode == ArrowKeyUp) {
        printf("Arrow Key Up\n");
    }
    // Down
    else if (keycode == ArrowKeyDown) {
        printf("Arrow Key Down\n");
    }
    else {
        printf("%c\n", keycode);
    }
}

#pragma mark - Drawing Functions

void clearScreen() {
    int r = 0x00;
    int g = 0x00;
    int b = 0x00;
    int a = 0xFF;
    SDL_SetRenderDrawColor(gRenderer, r, g, b, a);
    SDL_RenderClear(gRenderer);
}

void renderFrame() {
    clearScreen();
    SDL_Color textColor = { 0xff, 0xff, 0xff, 0xff } ;
    gTextTexture.loadFromRenderedText( "FPS: " + std::to_string(avgFPS), textColor);
    gTextTexture.render( 20, 20 );
    SDL_RenderPresent(gRenderer);
    frameCount++;
}

#pragma mark - File Path Functions

std::string filePathComponent(const std::string& str) {
    //std::size_t found = str.find_last_of("/\\");
    return str.substr(0, str.find_last_of("/\\"));
}

std::string lastPathComponent(const std::string& str) {
    //std::size_t found = str.find_last_of("/\\");
    return str.substr(str.find_last_of("/\\")+1);
}

