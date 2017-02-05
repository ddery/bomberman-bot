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

    static const int FREE;
    static const int WALL;
    static const int BRICK;
    static const int BOMB;
    static const int PLAYER;
    static const int SUPER_POWER_UP;
    static const int RADIUS_POWER_UP;
    static const int BAG_POWER_UP;

    static const int DO_NOTHING;
    static const int GO_UP;
    static const int GO_LEFT;
    static const int GO_RIGHT;
    static const int GO_DOWN;
    static const int PUT_BOMB;
    static const int TRIGGER_BOMB;

    static const int OCCUPIEABLE;

    Player* get_player_by_key(char key);
    Player* get_me();

    int get_map_width();
    int get_map_height();
    int get_round();
    vector<Player> get_player_vector();
    vector<Bomb> get_bomb_vector();

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
