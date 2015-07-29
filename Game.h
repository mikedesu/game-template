#ifndef _GAME_H
#define _GAME_H
#include <string>
class Game {
public:
    Game();
    std::string getName();
    void setName(std::string s);
private:
    std::string name;
};
#endif 

