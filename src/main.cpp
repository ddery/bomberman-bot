#include <bits/stdc++.h>
#include "game_state.h"

using namespace std;

int main(int argc, char** argv) {
    GameState gamestate(argc, argv);

    cout<<"Map width : "<<gamestate.get_map_width()<<endl;
    cout<<"Map height : "<<gamestate.get_map_height()<<endl;
    cout<<"Round : "<<gamestate.get_round()<<endl;

    vector<Player> players = gamestate.get_player_vector();
    cout<<endl<<"Players :"<<endl;
    for (int i = 0; i < (int) players.size(); i++)
        cout<<players[i].name<<" ("<<players[i].key<<")"<<endl;
    cout<<endl;

    for (int i = 1; i <= gamestate.get_map_height(); i++) {
        for (int j = 1; j <= gamestate.get_map_width(); j++)
            cout<<gamestate[i][j].type<<" ";
        cout<<endl;
    }

    return 0;
}
