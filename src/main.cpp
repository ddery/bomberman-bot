#include <bits/stdc++.h>
#include "game_state.h"

using namespace std;

void debug_map(GameState& gamestate, int** kena_bomb, pair<int,int>** jarak_arah) {
    int width = gamestate.get_map_width();
    int height = gamestate.get_map_height();

    printf("   ");
    for (int i = 1; i <= width; i++)
        printf("%14d", i); printf("\n");
    for (int i = 1; i <= height; i++) {
        printf("%2d ", i);
        for (int j = 1; j <= width; j++) {
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
            printf("(%3d,%3d,%3d) ", kena_bomb[i][j], jarak_arah[i][j].first,jarak_arah[i][j].second);
        }
        cout<<endl;
    }
}

/*
 * generate_kena_bomb
 * author : Dery Rahman
 * tanggal : 13 Februari 2016
 * digunakan untuk menciptakan matriks berukuran sama seperti map yang berisi
 * integer. Integer tersebut menyatakan banyaknya detik yang tersisa sehingga
 * petak yang bersangkutan akan meledak. matriks[y][x] menyatakan detik yang
 * tersisa sehingga petak (x,y) meledak
 * gamestate : berisi game state
 * return int** yang merepresentasikan matriks
 */
int** generate_kena_bomb(GameState& gamestate);

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

int ** generate_brick(GameState& gamestate);

int main(int argc, char** argv) {
    GameState gamestate(argc,argv);

    Player* me=gamestate.get_me();
    vector<Player> players = gamestate.get_player_vector();
    vector<PowerUp> powerups = gamestate.get_powerup_vector();
    vector<Bomb> bombs = gamestate.get_bomb_vector();


    int ** batubata = generate_brick(gamestate);
    int** kena_bomb = generate_kena_bomb(gamestate);
	  pair<int,int> **jarak_arah = cari_jarak_arah(gamestate);
    debug_map(gamestate, batubata, jarak_arah);

    for(int i = 1; i <= gamestate.get_map_height(); i++){
        for(int j = 1; j <= gamestate.get_map_width(); j++)
            cout << batubata[i][j] << " ";
        cout<<"\n";
    }


    Player* op = NULL;
    for (Player player : players)
        if (player.key != me->key) {
            op = &player;
            break;
        }

    Location tujuan = op->location;
    if(powerups.size() > 0){
        int jarakpowerup = jarak_arah[powerups[0].location.y][powerups[0].location.x].first;
        tujuan = powerups[0].location;
        for (PowerUp powerup : powerups)
            if(jarakpowerup > jarak_arah[powerup.location.y][powerup.location.x].first){
                jarakpowerup = jarak_arah[powerup.location.y][powerup.location.x].first;
                tujuan.x = powerup.location.x;
                tujuan.y = powerup.location.y;
            }
    }

    printf("Tujuan : (%d,%d)\n", tujuan.x, tujuan.y);

    int arah = jarak_arah[tujuan.y][tujuan.x].second;

    printf("Arah : %d\n", arah);

    if (kena_bomb[me->location.y][me->location.x] > 0) {
        int jarakhindar = 999;
        for (int i = 1; i <= gamestate.get_map_height(); i++)
            for (int j = 1; j <= gamestate.get_map_width(); j++)
                if (jarakhindar > jarak_arah[i][j].first && kena_bomb[i][j] == 0) {
                    jarakhindar = jarak_arah[i][j].first;
                    tujuan.x = j;
                    tujuan.y = i;
                }
        printf("Tujuan menghindar (%d,%d) -> %d\n", tujuan.x, tujuan.y, jarak_arah[tujuan.y][tujuan.x].second);
        gamestate.decide_move(jarak_arah[tujuan.y][tujuan.x].second);
    } else {
        for (Bomb bomb : bombs)
            if (bomb.owner->key == me->key)
                arah = GameState::TRIGGER_BOMB;

        if (arah ==  GameState::GO_RIGHT && (gamestate[me->location.y][me->location.x+1].type & (GameState::BRICK | GameState::PLAYER)))
            arah = GameState::PUT_BOMB;
        else if (arah ==  GameState::GO_LEFT && (gamestate[me->location.y][me->location.x-1].type & (GameState::BRICK | GameState::PLAYER)))
            arah = GameState::PUT_BOMB;
        else if (arah ==  GameState::GO_DOWN && (gamestate[me->location.y+1][me->location.x].type & (GameState::BRICK | GameState::PLAYER)))
            arah = GameState::PUT_BOMB;
        else if (arah ==  GameState::GO_UP && (gamestate[me->location.y-1][me->location.x].type & (GameState::BRICK | GameState::PLAYER)))
            arah = GameState::PUT_BOMB;
        gamestate.decide_move(arah);
    }

    return 0;
}
