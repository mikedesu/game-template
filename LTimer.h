#ifndef _LTIMER_H
#define _LTIMER_H

#include <SDL2/SDL.h>

class LTimer {
public:
    LTimer();
    void start();
    void stop();
    void pause();
    void unpause();
    Uint32 getTicks();
    bool isStarted();
    bool isPaused();
    Uint32 mStartTicks;
    Uint32 mPausedTicks;
private:
    //The timer status
    bool mPaused;
    bool mStarted;
};

#endif 

