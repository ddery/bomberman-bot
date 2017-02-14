#include <bits/stdc++.h>
#include "game_state.h"

using namespace std;

pair<int,int>** jarak_arah(GameState& gamestate){
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
