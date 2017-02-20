#ifndef _H_GAME_STATE
#define _H_GAME_STATE

#include <bits/stdc++.h>
#include "player.h"
#include "bomb.h"
#include "entity.h"
#include "json.hpp"
#include "powerup.h"

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
    static const int POWER_UP;

    Player* get_player_by_key(char key);
    Player* get_me();

    /*
     * get_map_width
     * author : Jauhar Arifin
     * digunakan untuk mendapatkan lebar map yang digunakan
     * return : int yang menyatakan lebar map
     */
    int get_map_width();

    /*
     * get_map_height
     * author : Jauhar Arifin
     * digunakan untuk mendapatkan tinggi map yang digunakan
     * return : int yang menyatakan tinggi map
     */
    int get_map_height();

    /*
     * get_round
     * author : Jauhar Arifin
     * digunakan untuk mendapatkan ronde saat ini
     * return : int yang menyatakan ronde saat ini
     */
    int get_round();

    /*
     * get_player_vector
     * author : Jauhar Arifin
     * digunakan untuk mendapkan vector(array) yang berisi list dari player yang
     * ikut bermain.
     * return : vector<Player> yang menyatakan list player yang bermain
     */
    vector<Player> get_player_vector();

    /*
     * get_bomb_vector
     * author : Jauhar Arifin
     * digunakan untuk mendapkan vector(array) yang berisi list dari bomb yang
     * berada dalam lapangan permainan.
     * return : vector<Bomb> yang menyatakan bomb.
     */
    vector<Bomb> get_bomb_vector();

    /*
     * get_powerup_vector
     * author : Dery Rahman
     * digunakan untuk mendapkan vector(array) yang berisi list dari powerup yang
     * ada di lapangan permainan.
     * return : vector<PowerUp> yang menyatakan list powerup yang ada di map.
     */
    vector<PowerUp> get_powerup_vector();

    /*
     * operator[]
     * author : Jauhar Arifin
     * digunakan untuk mendapatkan entity dari map. Entity dapat berupa player,
     * power up, wall ataupun brick. Untuk mendapatkan entity tersebut, dapat
     * dilakukan dengan cara seperti ini : gamestate[y][x], dimana y dan x menya-
     * takan koordinat petak yang dimaksud.
     * int y : menyatakan koodinat y dari petak yang dimaksud
     * return : Entity* yang menyatakan entity yang dimaksud.
     */
    Entity* operator[] (int);

    /*
     * decide_move
     * author : Jauhar Arifin
     * digunakan untuk menuliskan langkah ke file eksternal. Setelah fungsi ini
     * dijalankan program akan langsung berhenti dan mengembalikan return value
     * nol.
     * move : int yang menyatakan langkah yang akan diambil. Nilainya bisa dida-
     * patkan dari konstanta DO_NOTHING,GO_UP,GO_LEFT,GO_RIGHT,GO_DOWN,PUT_BOMB,
     * dan TRIGGER_BOMB.
     */
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
    vector<PowerUp> powerups;

    void load_game_state(const char*);
    void load_game_player(Json::Object*);
    void load_game_map(Json::Object*);

};

#endif
