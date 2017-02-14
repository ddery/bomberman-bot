#include <bits/stdc++.h>
#include "game_state.h"

using namespace std;

bool cmp(Bomb& a, Bomb& b) {
    return a.time_left < b.time_left;
};

int** generate_powerup(GameState& gamestate) {
	int width = gamestate.get_map_width();
    int height = gamestate.get_map_height();

	int** res = new int*[height + 1];
    for (int i = 1; i <= height; i++) {
        res[i] = new int[width+1];
        for (int j = 1; j <= width; j++)
            res[i][j] = 0;
    }

	vector<PowerUp> powerups = gamestate.get_powerup_vector();

	for (PowerUp powerup : powerups) {
		int y = powerup.location.y;
		int x = powerup.location.x;

		res[y][x]=powerup.type;
	}

	return res;
}

int** generate_kena_bomb(GameState& gamestate) {
    int width = gamestate.get_map_width();
    int height = gamestate.get_map_height();

    int **res = new int*[height + 1];
    for (int i = 1; i <= height; i++) {
        res[i] = new int[width+1];
        for (int j = 1; j <= width; j++)
            res[i][j] = 999;
    }

    vector<Bomb> bombs = gamestate.get_bomb_vector();
    sort(bombs.begin(), bombs.end(), cmp);

    for (Bomb bomb : bombs) {
		int time=bomb.time_left;
		if(res[bomb.location.y][bomb.location.x] < 999)
			time=res[bomb.location.y][bomb.location.x];

		int y = bomb.location.y;
		int x = bomb.location.x;

        res[y][x] = min(res[y][x],time);
        int bomb_occupy = GameState::OCCUPIEABLE | GameState::PLAYER | GameState::BOMB;
        for (int i = 1; i <= bomb.radius && (gamestate[y][x+i].type & bomb_occupy); i++) {
            res[y][x+i] = min(res[y][x+i],time);
			if (gamestate[y][x+i].type & GameState::BOMB)
				break;
		}
        for (int i = 1; i <= bomb.radius && (gamestate[y][x-i].type & bomb_occupy); i++){
            res[y][x-i] = min(res[y][x-i],time);
			if (gamestate[y][x-i].type & GameState::BOMB)
				break;
		}
        for (int i = 1; i <= bomb.radius && (gamestate[y+i][x].type & bomb_occupy); i++){
            res[y+i][x] = min(res[y+i][x],time);
			if (gamestate[y+i][x].type & GameState::BOMB)
				break;
		}
        for (int i = 1; i <= bomb.radius && (gamestate[y-i][x].type & bomb_occupy); i++){
            res[y-i][x] = min(res[y-i][x],time);
			if (gamestate[y-i][x].type & GameState::BOMB)
				break;
		}
    }


    for (int i = 1; i <= height; i++) {
        for (int j = 1; j <= width; j++)
            res[i][j] = (res[i][j] == 999) ? 0:res[i][j];
    }

    return res;
}

pair<int,int>** cari_jarak_arah(GameState& gamestate){
	const int INF=999;
    pair<int,int>** jarak = new pair<int,int>*[gamestate.get_map_height()+1];
    for (int i = 1; i <= gamestate.get_map_height(); i++) {
        jarak[i] = new pair<int,int>[gamestate.get_map_width()+1];
        for (int j = 1; j <= gamestate.get_map_width(); j++){
            jarak[i][j].first = INF;
            jarak[i][j].second = GameState::DO_NOTHING;
		}
    }
    Player* me = gamestate.get_me();

    int arah = GameState::DO_NOTHING;
    int cur = 0;
    int x = me->location.x;
    int y = me->location.y;

    queue<pair<pair<Location, int>,int> > q;
    q.push({{{x,y},cur},arah});
    while(!q.empty()){
		pair<pair<Location, int>,int> t = q.front(); q.pop();
		x = t.first.first.x;
		y = t.first.first.y;
		cur = t.first.second;
		if((gamestate[y+1][x].type & GameState::OCCUPIEABLE) && jarak[y+1][x].first==INF){
			jarak[y+1][x].first = cur+1;
			jarak[y+1][x].second = t.second == GameState::DO_NOTHING ? GameState::GO_DOWN : t.second;
			q.push({{{x,y+1},cur+1},jarak[y+1][x].second});
		}
		if((gamestate[y][x+1].type & GameState::OCCUPIEABLE) && jarak[y][x+1].first==INF){
			jarak[y][x+1].first = cur+1;
			jarak[y][x+1].second = t.second == GameState::DO_NOTHING ? GameState::GO_RIGHT : t.second;
			q.push({{{x+1,y},cur+1},jarak[y][x+1].second});
		}
		if((gamestate[y-1][x].type & GameState::OCCUPIEABLE) && jarak[y-1][x].first==INF){
			jarak[y-1][x].first = cur+1;
			jarak[y-1][x].second = t.second == GameState::DO_NOTHING ? GameState::GO_UP : t.second;
			q.push({{{x,y-1},cur+1},jarak[y-1][x].second});
		}
		if((gamestate[y][x-1].type & GameState::OCCUPIEABLE) && jarak[y][x-1].first==INF){
			jarak[y][x-1].first = cur+1;
			jarak[y][x-1].second = t.second == GameState::DO_NOTHING ? GameState::GO_LEFT : t.second;
			q.push({{{x-1,y},cur+1},jarak[y][x-1].second});
		}
	}

	return jarak;
}
