#include <bits/stdc++.h>
#include "game_state.h"

using namespace std;

void debug_map(GameState& gamestate) {
    int width = gamestate.get_map_width();
    int height = gamestate.get_map_height();

    for (int i = 1; i <= height; i++) {
        for (int j = 1; j <= width; j++)
            if (gamestate[i][j].type & GameState::FREE)
                cout<<" ";
            else if (gamestate[i][j].type & GameState::BOMB)
                cout<<gamestate[i][j].bomb->radius;
            else if (gamestate[i][j].type & GameState::PLAYER)
                cout<<gamestate[i][j].player->key;
            else if (gamestate[i][j].type & GameState::BRICK)
                cout<<"+";
            else if (gamestate[i][j].type & GameState::WALL)
                cout<<"#";
            else
                cout<<gamestate[i][j].type;
        cout<<endl;
    }
}

bool** generate_kena_bomb(GameState& gamestate) {
    int width = gamestate.get_map_width();
    int height = gamestate.get_map_height();

    bool **res = new bool*[height + 1];
    for (int i = 1; i <= height; i++) {
        res[i] = new bool[width+1];
        for (int j = 1; j <= width; j++)
            res[i][j] = false;
    }

    for (Bomb bomb : gamestate.get_bomb_vector()) {
        res[bomb.location.y][bomb.location.x] = true;
        int bomb_occupy = GameState::OCCUPIEABLE | GameState::PLAYER;
        for (int i = 1; i <= bomb.radius && (gamestate[bomb.location.y][bomb.location.x+i].type & bomb_occupy); i++)
            res[bomb.location.y][bomb.location.x+i] = true;
        for (int i = 1; i <= bomb.radius && (gamestate[bomb.location.y][bomb.location.x-i].type & bomb_occupy); i++)
            res[bomb.location.y][bomb.location.x-i] = true;
        for (int i = 1; i <= bomb.radius && (gamestate[bomb.location.y+1][bomb.location.x].type & bomb_occupy); i++)
            res[bomb.location.y+i][bomb.location.x] = true;
        for (int i = 1; i <= bomb.radius && (gamestate[bomb.location.y-1][bomb.location.x].type & bomb_occupy); i++)
            res[bomb.location.y-i][bomb.location.x] = true;
    }

    return res;
}

pair<Location,int> cari_tempat(GameState& gamestate, function<bool (int,int)> objective) {
    bool** visited = new bool*[gamestate.get_map_height()+1];
    for (int i = 1; i <= gamestate.get_map_height(); i++) {
        visited[i] = new bool[gamestate.get_map_width()+1];
        for (int j = 1; j <= gamestate.get_map_width(); j++)
            visited[i][j] = false;
    }
    Player* me = gamestate.get_me();

    int arah = GameState::DO_NOTHING;
    Location lokasi = me->location;

    queue<pair<Location,int> > que;
    que.push(make_pair(me->location,GameState::DO_NOTHING));
    while (!que.empty()) {
        pair<Location,int> t = que.front(); que.pop();
        if (objective(t.first.x,t.first.y)) {
            arah = t.second;
            lokasi = {t.first.x,t.first.y};
            break;
        }
        if (visited[t.first.y][t.first.x])
            continue;
        visited[t.first.y][t.first.x] = true;
        if ((gamestate[t.first.y][t.first.x+1].type & GameState::OCCUPIEABLE) && !visited[t.first.y][t.first.x+1])
            que.push(make_pair((Location){.x=t.first.x+1,.y=t.first.y}, t.second == GameState::DO_NOTHING ? GameState::GO_RIGHT : t.second));
        if ((gamestate[t.first.y][t.first.x-1].type & GameState::OCCUPIEABLE) && !visited[t.first.y][t.first.x-1])
            que.push(make_pair((Location){.x=t.first.x-1,.y=t.first.y}, t.second == GameState::DO_NOTHING ? GameState::GO_LEFT : t.second));
        if ((gamestate[t.first.y+1][t.first.x].type & GameState::OCCUPIEABLE) && !visited[t.first.y+1][t.first.x])
            que.push(make_pair((Location){.x=t.first.x,.y=t.first.y+1}, t.second == GameState::DO_NOTHING ? GameState::GO_DOWN : t.second));
        if ((gamestate[t.first.y-1][t.first.x].type & GameState::OCCUPIEABLE) && !visited[t.first.y-1][t.first.x])
            que.push(make_pair((Location){.x=t.first.x,.y=t.first.y-1}, t.second == GameState::DO_NOTHING ? GameState::GO_UP : t.second));
    }

    for (int i = 1; i <= gamestate.get_map_height(); i++)
        delete visited[i];
    delete visited;

    return make_pair(lokasi,arah);
}

int main(int argc, char** argv) {
    GameState gamestate(argc, argv);
    Player* me = gamestate.get_me();
    vector<Player> players = gamestate.get_player_vector();

    bool** kena_bomb = generate_kena_bomb(gamestate);

    debug_map(gamestate);
    for (int i = 1; i <= gamestate.get_map_height(); i++) {
        for (int j = 1; j <= gamestate.get_map_width(); j++)
            cout<<kena_bomb[i][j];
        cout<<endl;
    }

    srand(time(0));

    if (kena_bomb[me->location.y][me->location.x]) {
        pair<Location,int> aman = cari_tempat(gamestate, [&] (int x,int y){ return !kena_bomb[y][x];});
        cout<<"Tempat aman : ("<<aman.first.x<<","<<aman.first.y<<") -> "<<aman.second<<endl;
        gamestate.decide_move(aman.second);
    } else {
        Player* op = NULL;
        for (Player player : players)
            if (player.key != me->key) {
                op = &player;
                break;
            }

        Location tujuan = me->location;
        if (me->location.x < op->location.x && !(gamestate[me->location.y][me->location.x+1].type & GameState::WALL))
            tujuan = (Location) {me->location.x+1, me->location.y};
        else if (me->location.x > op->location.x && !(gamestate[me->location.y][me->location.x-1].type & GameState::WALL))
            tujuan = (Location) {me->location.x-1, me->location.y};
        else if (me->location.y < op->location.y && !(gamestate[me->location.y+1][me->location.x].type & GameState::WALL))
            tujuan = (Location) {me->location.x, me->location.y+1};
        else if (me->location.y > op->location.y && !(gamestate[me->location.y-1][me->location.x].type & GameState::WALL))
            tujuan = (Location) {me->location.x, me->location.y-1};
        else {
            int random[] = {0,1,2,3}; random_shuffle(random, random + 4);
            for (int test : random) {
                bool b = false;
                switch (test) {
                    case 0: if (!(gamestate[me->location.y][me->location.x+1].type & GameState::WALL)) { b = true; tujuan = (Location) {me->location.x+1, me->location.y}; } break;
                    case 1: if (!(gamestate[me->location.y][me->location.x-1].type & GameState::WALL)) { b = true; tujuan = (Location) {me->location.x-1, me->location.y}; } break;
                    case 2: if (!(gamestate[me->location.y+1][me->location.x].type & GameState::WALL)) { b = true; tujuan = (Location) {me->location.x, me->location.y+1}; } break;
                    case 3: if (!(gamestate[me->location.y-1][me->location.x].type & GameState::WALL)) { b = true; tujuan = (Location) {me->location.x, me->location.y-1}; } break;
                }
                if (b) break;
            }
        }

        cout<<"Tujuan : "<<tujuan.x<<","<<tujuan.y<<endl;

        int arah = GameState::DO_NOTHING;
        if (!kena_bomb[tujuan.y][tujuan.x]) {
            if (gamestate[tujuan.y][tujuan.x].type & GameState::BRICK)
                arah = GameState::PUT_BOMB;
            else if (tujuan.x > me->location.x)
                arah = GameState::GO_RIGHT;
            else if (tujuan.x < me->location.x)
                arah = GameState::GO_LEFT;
            else if (tujuan.y < me->location.y)
                arah = GameState::GO_UP;
            else if (tujuan.y > me->location.y)
                arah = GameState::GO_DOWN;
        }
        gamestate.decide_move(arah);
    }

    return 0;
}
