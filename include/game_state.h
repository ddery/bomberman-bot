#ifndef _H_GAME_STATE
#define _H_GAME_STATE

#include <bits/stdc++.h>
#include "player.h"
#include "bomb.h"
#include "entity.h"
#include "json.hpp"

using namespace std;

class GameState {

  public:
    GameState(int argc, char** argv);
    ~GameState();

    static const int FREE = 0;
    static const int WALL = 1;
    static const int BRICK = 2;
    static const int BOMB = 4;
    static const int PLAYER = 8;
    static const int SUPER_POWER_UP = 16;
    static const int RADIUS_POWER_UP = 32;
    static const int BAG_POWER_UP = 64;

    static const int DO_NOTHING = -1;
    static const int GO_UP = 1;
    static const int GO_LEFT = 2;
    static const int GO_RIGHT = 3;
    static const int GO_DOWN = 4;
    static const int PUT_BOMB = 5;
    static const int TRIGGER_BOMB = 6;

    Player* get_player_by_key(char key);

    int get_map_width();
    int get_map_height();
    int get_round();
    vector<Player> get_player_vector();

    Entity* operator[] (int);

    void decide_move(int);

  private:
    string path;

    int map_width;
    int map_height;
    int round;
    Entity *map;
    Player *me;
    vector<Player> players;
    vector<Bomb> bombs;

    void load_game_state(const char*);
    void load_game_player(Json::Object*);
    void load_game_map(Json::Object*);

};

#endif
