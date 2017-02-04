#include <bits/stdc++.h>
#include "player.h"

using namespace std;

Player::Player(const char* name, char key='A', int points = 0, bool killed = false,
  int bombag = 1, int bombradius = 1, pair<int,int> location = {1,1}) {
    this->name = name;
    this->key = key;
    this->points = points;
    this->killed = killed;
    this->bomb_bag = bombag;
    this->bomb_radius = bombradius;
    this->location = location;
}
