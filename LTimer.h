//
//  LTimer.h
//  rpg-game
//
//  Created by darkmage on 7/17/15.
//  Copyright (c) 2015 Mike Bell. All rights reserved.
//

#ifndef __rpg_game__LTimer__
#define __rpg_game__LTimer__

#include <SDL2/SDL.h>

//The application time based timer
class LTimer {
public:
    //Initializes variables
    LTimer();
    
    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();
    
    //Gets the timer's time
    Uint32 getTicks();
    
    //Checks the status of the timer
    bool isStarted();
    bool isPaused();
 
    Uint32 mStartTicks;
    Uint32 mPausedTicks;
    
private:
    
    //The timer status
    bool mPaused;
    bool mStarted;
};

#endif /* defined(__rpg_game__LTimer__) */
