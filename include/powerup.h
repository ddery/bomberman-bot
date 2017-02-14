#ifndef _H_POWERUP
#define _H_POWERUP

class PowerUp;

#include <bits/stdc++.h>
#include "location.h"

using namespace std;

class PowerUp {
	public :
        PowerUp(int, Location);
		int type;
		Location location;
};

#endif
