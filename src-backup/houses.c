#include "definitions.c"
#define MAP_INFO_PATH "map.csv"

enum{HOUSE_STD, HOUSE_CMP, GO_TO_JAIL, FREE_DAY};
enum{ID_H, TYPE_H, NAME_H, SET_ID_H, COST_H, RENT_H, RENTW1_H, RENTW2_H, RENTW3_H, RENTW4_H, RENTW5_H, HCOST_H};
/*
ID_H = Id da propriedade
TYPE_H = tipo da propriedade
NAME_H = nome da propriedade
SET_ID_H = id do set TODO: criar sets de propriedade de 1 a 8
COST_H = custo da propriedade
RENT_H = aluguel base da propriedade
RENTW1_H = aluguel c/ uma casa
RENTW2_H = aluguel c/ duas casas
RENTW3_H = aluguel c/ tres casas
RENTW4_H = aluguel c/ quatro casas
RENTW5_H = aluguel c/ cinco casas
HCOST_H = custo de uma casa
*/

struct house {
    int ID;
    int type;
    int rentWithHouses[5]; //Alugueis de 1 casa até 5 casas
    char name[30];
    int buildCost; //Custo do hotel ou casa
    unsigned int isOwnedBySomeone:1;
    unsigned int isOwnable:1;
    unsigned int isSetCompleted:1;
    int setID;
    int setColor; //Cor associada com o Set
    int ownerID;
    int housesBuilt;
    int cost;
    int rent;
    int baseRent;
};

struct house houses[40], *currentHouse;

void updateHousesRent(int houseID) {
    if(houses[houseID].housesBuilt > 0) {
        houses[houseID].rent = houses[houseID].rentWithHouses[(houses[houseID].housesBuilt)-1]; }
    else {
        houses[houseID].rent = houses[houseID].baseRent;
    }
}

int readMapInfo()
{
    FILE *file = fopen(MAP_INFO_PATH, "r");
    if(!file){
        return EXIT_FAILURE;}

    char buffer[1024];
    int linha = 0;
    int coluna = 0;
    while(fgets(buffer, 1024, file)){
        coluna = 0;
        char* leitura = strtok(buffer, ",");
        while(leitura) {
            if(linha==0){
                break; }
            if(strcmp(leitura, "-")!=0){
                switch(coluna)  {

                    case ID_H:
                        houses[linha-1].ID = atoi(leitura)-1;
                        break;

                    case TYPE_H:    //HOUSE_STD, HOUSE_CMP, GO_TO_JAIL, FREE_DAY
                        if(strcmp(leitura, "HOUSE_STD")==0){
                            houses[linha-1].type = HOUSE_STD;
                            houses[linha-1].isOwnable = TRUE;
                            houses[linha-1].isOwnedBySomeone = FALSE;
                            houses[linha-1].housesBuilt = 0;
                            houses[linha-1].isSetCompleted = FALSE;
                            houses[linha-1].ownerID = NULL;
                        }
                        else if(strcmp(leitura, "HOUSE_CMP")==0){
                            houses[linha-1].type = HOUSE_CMP;
                            houses[linha-1].isOwnable = TRUE;
                            houses[linha-1].isOwnedBySomeone = FALSE;
                            houses[linha-1].housesBuilt = 0;
                            houses[linha-1].isSetCompleted = FALSE;
                            houses[linha-1].ownerID = NULL;
                        }
                        else if(strcmp(leitura, "GO_TO_JAIL")==0){
                            houses[linha-1].type = GO_TO_JAIL;
                            houses[linha-1].isOwnable = FALSE;
                        }
                        else if(strcmp(leitura, "FREE_DAY")==0){
                            houses[linha-1].type = FREE_DAY;
                            houses[linha-1].isOwnable = FALSE;
                        }
                        break;
        
                    case NAME_H:
                        strcpy(houses[linha-1].name, leitura);
                        break;

                    case SET_ID_H:
                        houses[linha-1].setID = atoi(leitura);
                        switch(houses[linha-1].setID) {
                            case 1: houses[linha-1].setColor = LIGHTBLACK_BG; break;
                            case 2: houses[linha-1].setColor = LIGHTCYAN_BG; break;
                            case 3: houses[linha-1].setColor = LIGHTPURPLE_BG; break;
                            case 4: houses[linha-1].setColor = YELLOW; break;
                            case 5: houses[linha-1].setColor = LIGHTRED_BG; break;
                            case 6: houses[linha-1].setColor = LIGHTYELLOW_BG; break;
                            case 7: houses[linha-1].setColor = LIGHTGREEN_BG; break;
                            case 8: houses[linha-1].setColor = LIGHTPURPLE_BG; break;
                        }
                        break;
                    case COST_H:
                    if(leitura!="-"){
                        houses[linha-1].cost = atoi(leitura);
                        break; }
                        else {break;}
                    case RENT_H:
                    if(leitura!="-"){
                        houses[linha-1].baseRent = atoi(leitura);
                        break;}
                        else {break;}
                    case RENTW1_H:
                    if(leitura!="-"){
                        houses[linha-1].rentWithHouses[0] = atoi(leitura);
                        break;}
                        else {break;}
                    case RENTW2_H:
                    if(leitura!="-"){
                        houses[linha-1].rentWithHouses[1] = atoi(leitura);
                        break;}
                        else {break;}
                    case RENTW3_H:
                    if(leitura!="-"){
                        houses[linha-1].rentWithHouses[2] = atoi(leitura);
                        break;}
                        else {break;}
                    case RENTW4_H:
                    if(leitura!="-"){
                        houses[linha-1].rentWithHouses[3] = atoi(leitura);
                        break;}
                        else {break;}
                    case RENTW5_H:
                    if(leitura!="-"){
                        houses[linha-1].rentWithHouses[4] = atoi(leitura);
                        break;}
                        else {break;}
                    case HCOST_H:
                    if(leitura!="-"){
                        houses[linha-1].buildCost = atoi(leitura);
                        break;}
                        else {break;}
                }
            }
            leitura = strtok(NULL, ",");
            coluna++;
        }
        linha++;
    }
    fclose(file);
    return EXIT_SUCCESS;
}

