#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#ifdef __linux__
//funcao do linux para setar o tamanho do terminal
void setTerminalSize() {

}
enum falseoutrue {FALSE, TRUE};
#elif _WIN32
//funcao do windows para setar o tamanho do terminal
#include <windows.h>
void checkWindowSize(int screenWidth, int screenHeight)
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        int width, height;  
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;  
        if (width < screenWidth || height < screenHeight)
        {
            printf("\nO tamanho do terminal esta muito pequeno. Por favor redefina o tamanho.");
            printf("\nTamanho sugerido: %d X %d", screenWidth, screenHeight);
            printf("\nTamanho atual  : %d X %d", width, height);
            printf("\n\n");
            exit(1);
        }
    }

#elif __APPLE__ 
    enum boolean {FALSE, TRUE};

#endif

enum colours  {RESET,INCREASED_INTENSITY,BLACK=30,RED,GREEN,YELLOW,BLUE,PURPLE,CYAN,WHITE,\
               RED_BG=41,GREEN_BG,YELLOW_BG,BLUE_BG,PURPLE_BG,CYAN_BG,WHITE_BG,\
               LIGHTBLACK_BG=100,LIGHTRED_BG,LIGHTGREEN_BG,LIGHTYELLOW_BG,LIGHTBLUE_BG,LIGHTPURPLE_BG,LIGHTCYAN_BG,PUREWHITE_BG};

char Nomes[10][20];

int playerCount, jogadoresAtivos;

int housesCount = 40;

int startingMoney = 200; //Dinheiro Inicial dos players, TODO: pode ser ajustado?

int SCREENSIZE_X = 172;

int SCREENSIZE_Y = 45;

void clearScreen(){
    system("clear || cls");
}

int rollDice(){
    int dice = (rand()%6)+1;
    return dice;
}

void BubbleSort(int *vetor, size_t tamanho) //função para ordenar vetores de forma crescente: order("nome do vetor", "tamanho do vetor");
{
    for (int i = 0; i < tamanho - 1; ++i) {
        for (int j = i + 1; j < tamanho; ++j) {
            if (vetor[i] > vetor[j]) {
                int temp = vetor[i];
                vetor[i] = vetor[j];
                vetor[j] = temp;
            }
        }
    }
}

void order(int *vetor, size_t tamanho) //função para ordenar vetores de forma crescente: order("nome do vetor", "tamanho do vetor");
{
    for (int i = 0; i < tamanho - 1; ++i) {
        for (int j = i + 1; j < tamanho; ++j) {
            if (vetor[i] < vetor[j]) {
                int temp = vetor[i];
                vetor[i] = vetor[j];
                vetor[j] = temp;
            }
        }
    }
}

struct player{ //Definindo a as variáveis de cada jogador
        int ID;
        char nome[20];
        int money;
        int netWorth; //money + valor de suas propriedades
        int pos;
        int turno;
        unsigned int prisao:1;
        unsigned int faliu:1;
        int turnosPrisao;
        int colour;
        int properties[40];
        int numberofProperties;
    };
struct player players[10], *currentPlayer;

void declarePlayers(struct player player[]) {
    printf("Digite o numero de jogadores (max: 10):\n.: ");
    scanf("%i", &playerCount);
    clearScreen();

    for (int i = 0; i < playerCount; i++) {
        printf("\nDigite o nome do jogador %d: ", i + 1);
        scanf(" %s", player[i].nome);

        player[i].ID = i;
        player[i].money = player[i].netWorth = startingMoney;
        player[i].pos = 0;
        player[i].prisao = FALSE;
        player[i].turnosPrisao = 0;

        switch (i) {
            case 0:
                player[i].colour = BLUE;
                break;
            case 1:
                player[i].colour = RED;
                break;
            case 2:
                player[i].colour = GREEN;
                break;
            case 3:
                player[i].colour = YELLOW;
                break;
            case 4:
                player[i].colour = PURPLE;
                break;
            case 5:
                player[i].colour = CYAN;
                break;
            default:
                player[i].colour = CYAN;
                break;
        }&players[i];
    }

    int sorte[playerCount];
    int sorteaux[playerCount];
    int vetorTurno[playerCount];

    for (int i = 0; i < playerCount; i++) {
        int rodar;
        printf("\nAperte um botao para jogar os dados de %s:\n.: ", player[i].nome);
        fflush(stdin);
        getchar();
        int a = rollDice();
        int b = rollDice();
        sorte[i] = a + b;
        sorteaux[i] = a + b;
        printf("\nOs dados deram %i e %i, total de %i\n", a, b, sorte[i]);
    }

    recheck:
    for(int i = 0; i < playerCount; i++) {
        for(int j = 0; j < playerCount; j++) {
            if((sorteaux[i] == sorte[j]) && !(i == j)) {
                printf("\n%s e %s rolaram iguais, rolam para desempatar! \n", player[i].nome, player[j].nome);
                int c = rollDice();
                int d = rollDice();
                if((c > d) || (c==d)) {
                    printf("\n%s rolou maior! leva a vantagem no desempate. \n", player[i].nome);
                    sorteaux[i]++;
                    sorte[i]++;
                } else {
                    printf("\n%s rolou maior! leva a vantagem no desempate. \n", player[j].nome);
                    sorteaux[j]++;
                    sorte[j]++;
                }
                goto recheck;
            }
        }
    }

    order(sorte, playerCount);

    for (int i = 0; i < playerCount; i++) {
        for (int j = 0; j < playerCount; j++) {
            if (sorteaux[i] == sorte[j]) {
                player[i].turno = j;
                printf("\n\nO jogador %s esta na posicao %i\n", player[i].nome, player[i].turno+1);
            }
        }
    }
    jogadoresAtivos = playerCount;
    printf("\nAperte qualquer botao para ir ao jogo\n");
    fflush(stdin);
    getchar();
}

//ANSI ESCAPE CODE FUNÇOES:

void changeTextColour(int colour) {
    printf("\x1b[%dm", colour);
}
void restoreConsole() {
    printf("\x1b[%dm", RESET);
}
void changeCursorTo(int X, int Y){
    printf("\x1b[{%d};{%d}f", X, Y);
}
void moveCursorUp(int X) {
    printf("\x1b[%dA",X);
}
void moveCursorDown(int X) {
    printf("\x1b[%dB",X);
}
void moveCursorRight(int X) {
    printf("\x1b[%dC",X);
}
void moveCursorLeft(int X) {
    printf("\x1b[%dD",X);
}

#define move(X,Y)  printf("%c[%d;%df",0x1B,Y,X)
#define colour(C1,C2)   printf("\033[%d;%dm",C2,C1)

void movePlayer(struct player *currentPlayer,int OldLocationID,int NewLocationID)
{
    int mapXY[40][2]={{ 9,36},{ 9,33},{ 9,30},{ 9,27},{ 9,24},{ 9,21},{ 9,18},{ 9,15},{ 9,12},{ 9, 9},\
                    { 9, 6},{16, 6},{23, 6},{30, 6},{37, 6},{44, 6},{51, 6},{58, 6},{65, 6},{72, 6},\
                    {79, 6},{79, 9},{79,12},{79,15},{79,18},{79,21},{79,24},{79,27},{79,30},{79,33},\
                    {79,36},{72,36},{65,36},{58,36},{51,36},{44,36},{37,36},{30,36},{23,36},{16,36}};
    int playerID = currentPlayer->ID;
    move(mapXY[OldLocationID][0]+(playerID)%4,mapXY[OldLocationID][1]+(playerID)/4);
    printf(" ");
    changeTextColour(currentPlayer->colour);
    if(NewLocationID == 666) {
    } else {
        move(mapXY[NewLocationID][0]+(playerID)%4,mapXY[NewLocationID][1]+(playerID)/4);
        printf("o");
    }
    changeTextColour(RESET);
    move(95,5);
}

int ClearRightScreen(int startLine)
{
    for (int i=startLine;i<SCREENSIZE_Y;i++)
    {
        move(95,i);
        for(int j=0;j<(SCREENSIZE_X-95);j++)
            printf(" ");
    }
    return EXIT_SUCCESS;
}