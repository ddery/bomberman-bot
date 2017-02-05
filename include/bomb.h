#ifndef _H_BOMB
#define _H_BOMB

class Bomb;

#include <bits/stdc++.h>
#include "player.h"
#include "location.h"

using namespace std;

class Bomb {
    public:
        Bomb(Player*, int, int, bool, Location);
        int radius;
        int time_left;
        bool is_exploding;
        Player* owner;
        Location location;
};

#endif
