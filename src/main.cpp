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
            else if (gamestate[i][j].type & GameState::SUPER_POWER_UP)
                cout<<"$";
            else if (gamestate[i][j].type & GameState::RADIUS_POWER_UP)
                cout<<"!";
            else if (gamestate[i][j].type & GameState::BAG_POWER_UP)
                cout<<"&";
            else
                cout<<gamestate[i][j].type;
        cout<<endl;
    }
}

int** generate_powerup(GameState& gamestate);
int** generate_kena_bomb(GameState& gamestate);

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

/*
 * cari_jarak_arah
 * author : Dery Rahman
 * tanggal : 14 Februari 2016
 * digunakan untuk mendapatkan jarak dari player "me" ke semua petak di peta
 * dalam representasi matriks berukuran seperti peta yang bertipe pair<int,int>.
 * nilai first menunjukkan jarak dari posisi player sekarang, second menunjukkan
 * langkah yang harus dituju untuk menuju ke posisi yang bersangkutan
 * gamestate : berisi game state
 * return : pair<int,int>* yang merepresentasikan matriks
 */
pair<int,int>** cari_jarak_arah(GameState& gamestate);

int main(int argc, char** argv) {
  GameState gamestate(argc,argv);
	Player* me=gamestate.get_me();
	vector<Player> players=gamestate.get_player_vector();
  vector<PowerUp> powerups = gamestate.get_powerup_vector();

	debug_map(gamestate);
	int** kena_bomb = generate_kena_bomb(gamestate);
	int** powerup = generate_powerup(gamestate);

	pair<int,int> ** jarak_arah = cari_jarak_arah(gamestate);

	for(int i=1;i<=gamestate.get_map_height();i++){
		for(int j=1;j<=gamestate.get_map_width();j++)
			printf("%3d",powerup[i][j]);
		cout<<endl;
	}
  cout << endl;
  for(int i=1;i<=gamestate.get_map_height();i++){
  		for(int j=1;j<=gamestate.get_map_width();j++)
  		printf("%3d",kena_bomb[i][j]);
  	cout<<endl;
  }

	for(int i=1;i<=gamestate.get_map_height();i++){
		for(int j=1;j<=gamestate.get_map_width();j++)
			printf("%3d ",jarak_arah[i][j].first);
		cout<<endl;
	}

  for(int i=1;i<=gamestate.get_map_height();i++){
		for(int j=1;j<=gamestate.get_map_width();j++)
			printf("%3d ",jarak_arah[i][j].second);
		cout<<endl;
	}

  Player* op = NULL;
  for (Player player : players)
      if (player.key != me->key) {
          op = &player;
          break;
      }

  Location tujuan = op->location;

  if(powerups.size() != 0){
    int jarakpowerup = jarak_arah[powerups[0].location.y][powerups[0].location.x].first;
    tujuan = powerups[0].location;
    for (int i = 1; i < powerups.size(); i++) {
      if(jarakpowerup > jarak_arah[powerups[i].location.y][powerups[i].location.x].first){
        jarakpowerup = jarak_arah[powerups[i].location.y][powerups[i].location.x].first;
        tujuan.x = powerups[i].location.x;
        tujuan.y = powerups[i].location.y;
      }
    }
  }

  cout << "Tujuan Power Up : "<<tujuan.x << ',';
  cout << tujuan.y << '\n';

  int arah = jarak_arah[tujuan.y][tujuan.x].second;

  cout << "Arah Power Up : " <<arah << endl;

  if (kena_bomb[me->location.y][me->location.x] != 0) {
    /*if(jarak_arah[tujuan.y][tujuan.x].second != GameState::DO_NOTHING){
      gamestate.decide_move(arah);
    }else{*/
      int jarakhindar = 999;
      for (int i = 1; i <= gamestate.get_map_height(); i++) {
        for (int j = 1; j <= gamestate.get_map_width(); j++) {
          if (jarakhindar > jarak_arah[i][j].first && kena_bomb[i][j] == 0) {
              jarakhindar = jarak_arah[i][j].first;
              tujuan.x = j;
              tujuan.y = i;
          }
        }
      }
      cout <<"Tujuan Menghindar : " <<tujuan.x << ',';
      cout << tujuan.y << '\n';
      cout <<"Arah Menghindar : " << jarak_arah[tujuan.y][tujuan.x].second << '\n';
      gamestate.decide_move(jarak_arah[tujuan.y][tujuan.x].second);
    //}
  }else{
    if (arah ==  GameState::GO_RIGHT && (gamestate[me->location.y][me->location.x+1].type & GameState::BRICK))
        arah = GameState::PUT_BOMB;
    else if (arah ==  GameState::GO_LEFT && (gamestate[me->location.y][me->location.x-1].type & GameState::BRICK))
        arah = GameState::PUT_BOMB;
    else if (arah ==  GameState::GO_DOWN && (gamestate[me->location.y+1][me->location.x].type & GameState::BRICK))
        arah = GameState::PUT_BOMB;
    else if (arah ==  GameState::GO_UP && (gamestate[me->location.y-1][me->location.x].type & GameState::BRICK))
        arah = GameState::PUT_BOMB;
    gamestate.decide_move(arah);
  }

    return 0;
}
