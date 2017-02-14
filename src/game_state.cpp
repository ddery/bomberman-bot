#include <bits/stdc++.h>
#include "game_state.h"
#include "json.hpp"

using namespace std;

const int GameState::FREE = 1;
const int GameState::WALL = 2;
const int GameState::BRICK = 4;
const int GameState::BOMB = 8;
const int GameState::PLAYER = 16;
const int GameState::SUPER_POWER_UP = 32;
const int GameState::RADIUS_POWER_UP = 64;
const int GameState::BAG_POWER_UP = 128;

const int GameState::DO_NOTHING = -1;
const int GameState::GO_UP = 1;
const int GameState::GO_LEFT = 2;
const int GameState::GO_RIGHT = 3;
const int GameState::GO_DOWN = 4;
const int GameState::PUT_BOMB = 5;
const int GameState::TRIGGER_BOMB = 6;

const int GameState::OCCUPIEABLE = GameState::FREE | GameState::SUPER_POWER_UP | GameState::RADIUS_POWER_UP | GameState::BAG_POWER_UP;
const int GameState::POWER_UP = GameState::SUPER_POWER_UP | GameState::RADIUS_POWER_UP | GameState::BAG_POWER_UP;

void GameState::load_game_state(const char* file) {
  ifstream in(file);
  if (in.is_open()) {
    string temp;
    getline(in, temp);

    Json::Object* state_json = (Json::Object*) Json::read(temp.c_str());

    this->round = ((Json::Number*) state_json->get("CurrentRound"))->value();
    this->map_width = ((Json::Number*) state_json->get("MapWidth"))->value();
    this->map_height = ((Json::Number*) state_json->get("MapHeight"))->value();
    this->map = new Entity[this->map_width * this->map_height];

    load_game_player(state_json);
    load_game_map(state_json);

    delete state_json;
    in.close();
  } else
    exit(-1);
}

void GameState::load_game_player(Json::Object* state_json) {
    Json::Array* players_json = (Json::Array*) state_json->get("RegisteredPlayerEntities");
    for (int i = 0; i < players_json->size(); i++) {
        Json::Object* player_json = (Json::Object*) players_json->get(i);

        string name = ((Json::String*) player_json->get("Name"))->value();
        char key = ((Json::String*) player_json->get("Key"))->value()[0];
        int points = ((Json::Number*) player_json->get("Points"))->value();
        bool killed = ((Json::Boolean*) player_json->get("Killed"))->value();
        int bomb_bag = ((Json::Number*) player_json->get("BombBag"))->value();
        int bomb_radius = ((Json::Number*) player_json->get("BombRadius"))->value();

        Json::Object* location_json = (Json::Object*) player_json->get("Location");

        int location_x = ((Json::Number*) location_json->get("X"))->value();
        int location_y = ((Json::Number*) location_json->get("Y"))->value();

        Player *player = new Player(name.c_str(), key, points, killed, bomb_bag, bomb_radius, {location_x,location_y});
        this->players.push_back(*player);
    }
}

void GameState::load_game_map(Json::Object* state_json) {
    Json::Array* blocks_json = (Json::Array*) state_json->get("GameBlocks");
    for (int i = 0; i < blocks_json->size(); i++) {
        Json::Array* blocks_json_2 = (Json::Array*) blocks_json->get(i);
        for (int j = 0; j < blocks_json_2->size(); j++) {
            Json::Object* block = (Json::Object*) blocks_json_2->get(j);

            Json::Object* location_json = (Json::Object*) block->get("Location");
            int location_x = ((Json::Number*) location_json->get("X"))->value();
            int location_y = ((Json::Number*) location_json->get("Y"))->value();

            Entity *cur_block = this->map + (location_y-1)*this->map_width + (location_x-1);
            cur_block->type = 0;
            cur_block->player = NULL;
            cur_block->bomb = NULL;
            cur_block->powerup = NULL;

            if (block->get("Entity")->getType() == Json::T_OBJECT) {
                Json::Object* entity_json = (Json::Object*) block->get("Entity");
                string type = ((Json::String*) entity_json->get("$type"))->value();
                if (type == "Domain.Entities.PlayerEntity, Domain") {
                    cur_block->type |= GameState::PLAYER;
                    char key = ((Json::String*) entity_json->get("Key"))->value()[0];
                    cur_block->player = this->get_player_by_key(key);
                } else if (type == "Domain.Entities.IndestructibleWallEntity, Domain")
                    cur_block->type |= GameState::WALL;
                else if (type == "Domain.Entities.DestructibleWallEntity, Domain")
                    cur_block->type |= GameState::BRICK;
            }

            if (block->get("Bomb")->getType() == Json::T_OBJECT) {
                Json::Object* bomb_json = (Json::Object*) block->get("Bomb");
                int radius = ((Json::Number*) bomb_json->get("BombRadius"))->value();
                int time_left = ((Json::Number*) bomb_json->get("BombTimer"))->value();
                bool explode = ((Json::Boolean*) bomb_json->get("IsExploding"))->value();
                char owner_key = ((Json::String*) ((Json::Object*) bomb_json->get("Owner"))->get("Key"))->value()[0];
                Player* owner = this->get_player_by_key(owner_key);

                Bomb* bomb = new Bomb(owner, radius, time_left, explode, {location_x, location_y});
                this->bombs.push_back(*bomb);
                owner->bombs.push_back(bomb);

                cur_block->type |= GameState::BOMB;
                cur_block->bomb = bomb;
            }

            if (block->get("PowerUp")->getType() == Json::T_OBJECT) {
                Json::Object* powerup_json = (Json::Object*) block->get("PowerUp");
                string type = ((Json::String*) powerup_json->get("$type"))->value();
				int tipe;
                if (type == "Domain.Entities.PowerUps.SuperPowerUp, Domain")
                    tipe = GameState::SUPER_POWER_UP;
                else if (type == "Domain.Entities.PowerUps.BombRaduisPowerUpEntity, Domain")
                    tipe = GameState::RADIUS_POWER_UP;
                else if (type == "Domain.Entities.PowerUps.BombBagPowerUpEntity, Domain")
                    tipe = GameState::BAG_POWER_UP;

				cur_block->type |= tipe;

				PowerUp* powerup = new PowerUp(tipe,{location_x, location_y});
				this->powerups.push_back(*powerup);

				cur_block->powerup = powerup;
            }

            if (cur_block->type == 0)
                cur_block->type |= GameState::FREE;
        }
    }
}

GameState::GameState(int argc, char** argv) {
  if (argc < 3)
    exit(-1);

  string player_key = argv[1];
  this->path = argv[2];

  load_game_state((this->path+"/state.json").c_str());
  this->me = this->get_player_by_key(player_key[0]);
}

Player* GameState::get_player_by_key(char key) {
    Player* temp = NULL;
    for (int i = 0; i < (int) this->players.size() && temp == NULL; i++)
        if (this->players[i].key == key)
            temp = &(this->players[i]);
    return temp;
}

Player* GameState::get_me() {
    return this->me;
}

int GameState::get_map_width() {
    return this->map_width;
}

int GameState::get_map_height() {
    return this->map_height;
}

int GameState::get_round() {
    return this->round;
}

vector<Player> GameState::get_player_vector() {
    return this->players;
}

vector<Bomb> GameState::get_bomb_vector() {
    return this->bombs;
}

vector<PowerUp> GameState::get_powerup_vector() {
    return this->powerups;
}

Entity* GameState::operator[] (int index) {
    return this->map + (index - 1) * this->map_width - 1;
}

void GameState::decide_move(int move) {
    ofstream out((this->path + "/move.txt").c_str());
    if (out.is_open()) {
        out<<move<<endl;
        out.close();
    }
    exit(0);
}

GameState::~GameState() {
  delete this->map;
}
