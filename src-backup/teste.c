#include "board.c"
int main() {
// TODO: Menu Principal aqui <--
declarePlayers(players, playerCount, Nomes);

declareHouses(houses);

initscr();
/*printTable();
refresh();*/
printw("teste");
refresh();
endwin();
}
/*
for(int i=0; i<playerCount; i++){
    currentPlayer = &players[i];
    movePlayer(currentPlayer, 0, 0);
} */
