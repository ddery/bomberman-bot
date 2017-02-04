#ifndef _H_GAME_STATE
#define _H_GAME_STATE

#include <bits/stdc++.h>
#include "player.h"
#include "entity.h"

using namespace std;

class GameState {

  public:
    GameState(int argc, char** argv);
    ~GameState();

    static const int FREE = 0;
    static const int WALL = 1;
    static const int BRICK = 2;
    static const int BOMB = 3;
    static const int PLAYER = 4;
    static const int SUPER_POWER_UP = 5;
    static const int RADIUS_POWER_UP = 6;
    static const int BAG_POWER_UP = 7;

  private:
    int map_width;
    int map_height;
    int round;
    Entity *map;
    Player *me;
    vector<Player> players;

    void load_game_state(const char*);

};

#endif
