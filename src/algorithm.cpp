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

	for(int i = 1; i <= gamestate.get_map_height(); i++)
		for(int j = 1; j <= gamestate.get_map_width(); j++)
			res[i][j] = res[i][j]==999 ? 0 : res[i][j];

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
	int** kena_bomb = generate_kena_bomb(gamestate);

    int arah = GameState::DO_NOTHING;
    int cur = 0;
    int x = me->location.x;
    int y = me->location.y;

    priority_queue<pair<pair<int, Location>,int> > q;
    q.push({{cur,{x,y}},arah});
	jarak[y][x].first = cur;
	jarak[y][x].second = arah;

    while(!q.empty()){
		pair<pair<int, Location>,int> t = q.top(); q.pop();
		x = t.first.second.x;
		y = t.first.second.y;
		cur = t.first.first*(-1);
		int next_langkah = cur+1;
		if((gamestate[y+1][x].type & (GameState::OCCUPIEABLE | GameState::BRICK)) && jarak[y+1][x].first==INF){
			jarak[y+1][x].first = (gamestate[y+1][x].type & GameState::BRICK) ? next_langkah+8 : next_langkah;
			jarak[y+1][x].second = t.second == GameState::DO_NOTHING ? GameState::GO_DOWN : t.second;
			if((kena_bomb[y+1][x] < jarak[y+1][x].first) && kena_bomb[y+1][x] != 0)
				jarak[y+1][x].second = GameState::DO_NOTHING;
			q.push({{jarak[y+1][x].first*(-1),{x,y+1}},jarak[y+1][x].second});
		}
		if((gamestate[y][x+1].type & (GameState::OCCUPIEABLE | GameState::BRICK)) && jarak[y][x+1].first==INF){
			jarak[y][x+1].first = (gamestate[y][x+1].type & GameState::BRICK) ? next_langkah+8 : next_langkah;
			jarak[y][x+1].second = t.second == GameState::DO_NOTHING ? GameState::GO_RIGHT : t.second;
			if((kena_bomb[y][x+1] < jarak[y][x+1].first) && kena_bomb[y][x+1] != 0)
				jarak[y][x+1].second = GameState::DO_NOTHING;
			q.push({{jarak[y][x+1].first*(-1),{x+1,y}},jarak[y][x+1].second});
		}
		if((gamestate[y-1][x].type & (GameState::OCCUPIEABLE | GameState::BRICK)) && jarak[y-1][x].first==INF){
			jarak[y-1][x].first = (gamestate[y-1][x].type & GameState::BRICK) ? next_langkah+8 : next_langkah;
			jarak[y-1][x].second = t.second == GameState::DO_NOTHING ? GameState::GO_UP : t.second;
			if((kena_bomb[y-1][x] < jarak[y-1][x].first)  && kena_bomb[y-1][x] != 0)
				jarak[y-1][x].second = GameState::DO_NOTHING;
			q.push({{jarak[y-1][x].first*(-1),{x,y-1}},jarak[y-1][x].second});
		}
		if((gamestate[y][x-1].type & (GameState::OCCUPIEABLE | GameState::BRICK)) && jarak[y][x-1].first==INF){
			jarak[y][x-1].first = (gamestate[y][x-1].type & GameState::BRICK) ? next_langkah+8 : next_langkah;
			jarak[y][x-1].second = t.second == GameState::DO_NOTHING ? GameState::GO_LEFT : t.second;
			if((kena_bomb[y][x-1] < jarak[y][x-1].first) && kena_bomb[y][x-1] != 0)
				jarak[y][x-1].second = GameState::DO_NOTHING;
			q.push({{jarak[y][x-1].first*(-1),{x-1,y}},jarak[y][x-1].second});
		}
	}

	return jarak;
}
