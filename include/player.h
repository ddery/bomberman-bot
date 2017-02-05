#ifndef _H_PLAYER
#define _H_PLAYER

class Player;

#include <bits/stdc++.h>
#include "bomb.h"
#include "location.h"

using namespace std;

class Player {
  public:
    Player(const char*, char, int, bool, int, int, Location);

    string name;
    char key;
    int points;
    bool killed;
    int bomb_bag;
    int bomb_radius;
    Location location;
    vector<Bomb*> bombs;
};

#endif
