#include <bits/stdc++.h>
#include "game_state.h"

using namespace std;

bool cmp(Bomb& a, Bomb& b) {
    return a.time_left < b.time_left;
};

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

        int direction_x[4] = {0,0,1,-1};
        int direction_y[4] = {1,-1,0,0};

        for (int _i = 0; _i < 4; _i++) {
            int _x = direction_x[_i];
            int _y = direction_y[_i];

            for (int i = 1; i <= bomb.radius && (gamestate[y+i*_y][x+i*_x].type & bomb_occupy); i++) {
                res[y+i*_y][x+i*_x] = min(res[y+i*_y][x+i*_x],time);
    		    if (gamestate[y+i*_y][x+i*_x].type & GameState::BOMB)
                    break;
    		}
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
    for (int i = 1; i <= gamestate.get_map_height(); i++)
        for (int j = 1; j <= gamestate.get_map_width(); j++)
            if (kena_bomb[i][j] == 0)
                kena_bomb[i][j] = INF;

    int arah = GameState::DO_NOTHING;
    int cur = 0;
    int x = me->location.x;
    int y = me->location.y;
    jarak[y][x].first = 0;

    bool **visited = new bool*[gamestate.get_map_height()+1];
    for (int i = 1; i <= gamestate.get_map_height(); i++)
        visited[i] = new bool[gamestate.get_map_width()]();

    priority_queue<pair<int, Location> > q;
    q.push(make_pair(cur,(Location) {x,y}));
    while(!q.empty()){
		pair<int, Location> t = q.top(); q.pop();

        x = t.second.x;
		y = t.second.y;
		cur = -t.first;
		int next_langkah = cur+1;

        if (visited[y][x])
            continue;
        visited[y][x] = true;

        int direction_x[4] = {x,x,x+1,x-1};
        int direction_y[4] = {y+1,y-1,y,y};
        int action[4] = {GameState::GO_DOWN,GameState::GO_UP,GameState::GO_RIGHT,GameState::GO_LEFT};

        for (int i = 0; i < 4; i++) {
            int _y = direction_y[i];
            int _x = direction_x[i];
            int _a = action[i];
            int _jarak = (gamestate[_y][_x].type & GameState::BRICK) ? next_langkah + 8 : next_langkah;

            if (gamestate[_y][_x].type & (GameState::OCCUPIEABLE | GameState::BRICK | GameState::PLAYER)){
                if (jarak[_y][_x].first > _jarak) {
                    jarak[_y][_x].first = _jarak;
                    jarak[_y][_x].second = t.second == me->location ? _a : jarak[y][x].second;
                }
    			if((kena_bomb[_y][_x]-1 == jarak[_y][_x].first) && kena_bomb[_y][_x] > 0)
    				jarak[_y][_x].second = GameState::DO_NOTHING;
    			q.push(make_pair(-jarak[_y][_x].first,(Location) {_x,_y}));
    		}
        }
	}

    for (int i = 1; i <= gamestate.get_map_height(); i++)
        delete visited[i];
    delete visited;

	return jarak;
}

int ** generate_brick(GameState& gamestate) {
    int width = gamestate.get_map_width();
    int height = gamestate.get_map_height();

    int **res = new int*[height + 1];
    for (int i = 1; i <= height; i++) {
        res[i] = new int[width+1];
        for (int j = 1; j <= width; j++)
            res[i][j] = 0;
    }

    Player * me = gamestate.get_me();

    for (int i = 1; i <= height; i++) {
        for (int j = 1; j <= width; j++) {
            if(gamestate[i][j].type & GameState::BRICK){
                for(int k = 1; k <= me->bomb_radius; k++){
                    if(gamestate[i+k][j].type & (GameState::BRICK | GameState::WALL))
                        break;
                    else
                        res[i+k][j]++;
                }
                for(int k = 1; k <= me->bomb_radius; k++){
                    if(gamestate[i-k][j].type & (GameState::BRICK | GameState::WALL))
                        break;
                    else
                        res[i-k][j]++;
                }
                for(int k = 1; k <= me->bomb_radius; k++){
                    if(gamestate[i][j+k].type & (GameState::BRICK | GameState::WALL))
                        break;
                    else
                        res[i][j+k]++;
                }
                for(int k = 1; k <= me->bomb_radius; k++){
                    if(gamestate[i][j-k].type & (GameState::BRICK | GameState::WALL))
                        break;
                    else
                        res[i][j-k]++;
                }
            }
        }
    }
    return res;
}
