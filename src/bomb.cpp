#include <bits/stdc++.h>
#include "bomb.h"

using namespace std;

Bomb::Bomb(Player* player = NULL, int radius = 1, int time_left = 4, bool explode = 0, pair<int,int> location = {1,1}) {
    this->owner = player;
    this->radius = radius;
    this->time_left = time_left;
    this->is_exploding = explode;
    this->location = location;
}
