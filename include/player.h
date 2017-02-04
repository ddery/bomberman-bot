#ifndef _H_PLAYER
#define _H_PLAYER

class Player;

#include <bits/stdc++.h>
#include "bomb.h"

using namespace std;

class Player {
  public:
    Player(const char*, char, int, bool, int, int, pair<int,int>);

    string name;
    char key;
    int points;
    bool killed;
    int bomb_bag;
    int bomb_radius;
    pair<int,int> location;
    vector<Bomb*> bombs;
};

#endif
