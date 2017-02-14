#ifndef _H_ENTITY
#define _H_ENTITY

#include <bits/stdc++.h>
#include "player.h"
#include "bomb.h"
#include "powerup.h"

using namespace std;

typedef struct {
  int type;
  Player* player;
  Bomb* bomb;
  PowerUp* powerup;
} Entity;

#endif
