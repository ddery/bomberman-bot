#ifndef _H_ENTITY
#define _H_ENTITY

#include <bits/stdc++.h>
#include "player.h"
#include "bomb.h"

using namespace std;

typedef struct {
  int type;
  Player* player;
  Bomb* bomb;
} Entity;

#endif
