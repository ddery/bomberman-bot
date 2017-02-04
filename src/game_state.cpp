#include <bits/stdc++.h>
#include "game_state.h"
#include "json.hpp"

using namespace std;

void GameState::load_game_state(const char* file) {
  ifstream in(file);
  if (in.is_open()) {
    string temp;
    getline(in, temp);

    Json::Object* state_json = (Json::Object*) Json::read(temp.c_str());

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

      Player *player = new Player(name.c_str(), key, points, killed, bomb_bag, bomb_radius, make_pair(location_x,location_y));
      this->players.push_back(*player);
    }

    this->round = ((Json::Number*) state_json->get("CurrentRound"))->value();
    this->map_width = ((Json::Number*) state_json->get("MapWidth"))->value();
    this->map_height = ((Json::Number*) state_json->get("MapHeight"))->value();

    delete state_json;
    in.close();
  } else
    exit(-1);
}

GameState::GameState(int argc, char** argv) {
  if (argc < 3)
    exit(-1);

  string player_key = argv[1];
  string path = argv[2];

  load_game_state((path+"/state.json").c_str());
  for (Player player : this->players)
    if (player_key[0] == player.key) {
      this->me = &player;
      break;
    }
}

GameState::~GameState() {
  delete this->map;
}
