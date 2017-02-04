#ifndef _H_BOMB
#define _H_BOMB

class Bomb;

#include <bits/stdc++.h>
#include "player.h"

using namespace std;

class Bomb {
    public:
        Bomb(Player*, int, int, bool, pair<int,int>);
        int radius;
        int time_left;
        bool is_exploding;
        Player* owner;
        pair<int,int> location;
};

#endif
