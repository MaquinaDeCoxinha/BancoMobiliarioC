#include "player.c"

bool running = true; //o while do jogo começa rodando por enquanto

int turnos = 0;
    //quantidade de turnos total, turno atual de qual jogador

int maingame() //loop do jogo
{    
    declarePlayers(players);    //iniciando variáveis
    if(readMapInfo() != EXIT_SUCCESS){
        printf("Erro na funcao readMapInfo");
        printf("Error: %d (%s)\n", errno, strerror(errno));
        getchar();
        return EXIT_FAILURE;
    }

    printTable();   //printando o tabuleiro, que fica fixo

    for(int i=0; i<playerCount; i++)    //colocando os players na casa do início
    {
        currentPlayer = &players[i];
        movePlayer(currentPlayer, 0, 0);
    }

    int playerInic = 0;

    SAIRGAME:
    while(running){ //while do jogo em si
        fflush(stdin);
        ClearRightScreen(0);

        if(turnos >= playerCount){
            turnos = 0; }
        if(playerInic >= playerCount){
            playerInic = 0; }

        currentPlayer = &players[playerInic];

        if(currentPlayer->turno != turnos){
            playerInic++;
            goto SAIRGAME;  }
        if(currentPlayer->faliu == 1){
            turnos++;
            goto SAIRGAME;  }

        if(jogadoresAtivos <= 1) { //VITORIA!!!!!!!!!!!!!!!!!!!
            move(95, 1); printf("%s voce ganhou o jogo!!!", currentPlayer->nome);
            move(95, 2); printf("Pressione qualquer tecla para continuar");
            move(95, 3); fflush(stdin); getchar();
            return EXIT_SUCCESS;
        }
        

        printPlayerInfo(currentPlayer);

        int housesToWalk;
        if(currentPlayer->prisao){
            printPlayerInfo(currentPlayer);
            move(95,5); printf("Voce esta na prisao");
            move(95,6); printf("Faltam %i turnos.", currentPlayer->turnosPrisao);
            move(95,7); printf("O que voce deseja fazer?");
            move(95,8); printf("1 - TENTAR ROLAR"); move(95,9); printf("2 - PAGAR FIANCA(200$)");
            choosepris: move(95,10); fflush(stdin); char opcaoPris = getchar();
            switch (opcaoPris)
            {
                case '1': ;
                    int rollPris1 = rollDice();
                    int rollPris2 = rollDice();
                    move(95,11); printf("%s rolou %i e %i para sair da prisao!", currentPlayer->nome, rollPris1, rollPris2);
                    if(rollPris1==rollPris2) {
                        move(95,12); printf("As duas rolagens foram iguais! %s saiu da prisao!", currentPlayer->nome);
                        move(95,13); printf("Pressione qualquer botao para continuar");
                        move(95,14); fflush(stdin); getchar();
                    } else {
                        move(95,12); printf("As duas rolagens nao foram iguais...");
                        (currentPlayer->turnosPrisao)--;
                        goto turnoacabou;
                    }

                    break;
                case '2':
                    if(currentPlayer->money >= 500) {
                        currentPlayer->money -= 500;
                    } else {
                        move(95,12); printf("Voce nao tem dinheiro suficiente!");
                        move(95,13); printf("Pressione qualquer botao para continuar");
                        move(95,14); fflush(stdin); getchar();
                    }
                    break;
                default:
                    move(95,11); printf("Opcao invalida, por favor tente novamente");
                    move(95,12); printf("Pressione qualquer botao para continuar");
                    move(95,13); fflush(stdin); getchar();
                    ClearRightScreen(10);
                    goto choosepris;
                    break;
            }
            
        }
        if(!(currentPlayer->prisao == 1))
        {
            int rollIsOver = 0;
            int oldLocation;
            housesToWalk = 0;

            while(!rollIsOver)
            {
                ClearRightScreen(4);
                int roll1 = rollDice();
                int roll2 = rollDice();
                move(95,5); printf("%s rolou %d e %d para andar!\n", currentPlayer->nome, roll1, roll2);
                housesToWalk += (roll1 + roll2);
                if(roll1 != roll2){rollIsOver = 1;} 
                else
                {
                    move(95,6); printf("%s rolou numeros iguais para andar, rola de novo!\n", currentPlayer->nome);
                    move(95,7); printf("Pressione qualquer botao para continuar... \n");
                    fflush(stdin);
                    move(95,8); getchar();
                }
            }

            oldLocation = currentPlayer->pos;
            currentPlayer->pos += housesToWalk;

            if(currentPlayer->pos >= 40){currentPlayer->pos -= 40;currentPlayer->money += 200;}

            movePlayer(currentPlayer, oldLocation, currentPlayer->pos);
        }
        currentHouse = &houses[currentPlayer->pos];
        switch (currentHouse->type)
        {
            case HOUSE_STD:
            move(95,6); printf("Voce caiu na casa %s, ", currentHouse->name);
            if(currentHouse->isOwnedBySomeone)
            {
                if(currentHouse->ownerID == currentPlayer->ID) {
                    move(95,7); printf("Essa é uma propriedade sua!.\n");
                    move(95,8); printf("Pressione qualquer botao para continuar");
                    move(95,8); fflush(stdin); getchar();
                    break;
                }
                move(95,7); printf("ela e possuida por %s.\n", players[currentHouse->ownerID].nome);
                move(95,8); printf("Voce deve pagar %d$ ao proprietario.\n", currentHouse->rent);
                // TODO: adicionar o sistema de pagar aluguel
                if(currentPlayer->netWorth < currentHouse->rent) { // Falencia!
                    move(95,9); printf("O jogador %s nao tem como pagar o aluguel!.", currentPlayer->nome);
                    move(95,10); playerLosed(currentPlayer);
                    break; // TODO: TESTAR!!!!!!!!!!!!!!!!!!!!!!!!!!!
                } 
                else if(currentPlayer->money < currentHouse->rent) {
                    while(!(currentPlayer->money >= currentHouse->rent)){    // TODO: TESTAR!!!!!!!!!!!!!!!!!!!!!!!!!!!
                        move(95,9); printf("Voce precisa vender algumas coisas para conseguir pagar o aluguel");
                        move(95,10); printf("Vamos abrir o menu de compra e venda para voce.");
                        move(95,11); printf("Aperte qualquer botao para continuar");
                        fflush(stdin);
                        move(95,12); getchar();
                        if((BuyAndSellHousesMenu(currentPlayer, houses))!= EXIT_SUCCESS) {
                            ClearRightScreen(0);
                            move(95,0); printf("error in BuyAndSellHousesMenu"); }
                        printPlayerInfo(currentPlayer);
                    }
                }
                currentPlayer->money -= currentHouse->rent;
            } 
            else 
            {
                move(95,7); printf("ela nao e possuida por ninguem.\n");
                move(95,8); printf("%s deseja comprar essa propriedade por %i?\n", currentPlayer->nome, currentHouse->cost);
                move(95,9); printf("1 - SIM"); move(95,10); printf("2 - NAO"); move(95,11); printf("3 - COMPRAR E VENDER CASAS");
                move(95,12); fflush(stdin); char opcao = getchar();
                switch (opcao) {
                case '1':
                    currentPlayer->money -= currentHouse->cost;
                    currentHouse->isOwnedBySomeone = TRUE;
                    currentHouse->ownerID = currentPlayer->ID;
                    addPropertieToPlayer(currentPlayer, houses, currentHouse->ID);
                    ClearRightScreen(4);
                    move(95,5); printf("Voce adquiriu a casa %s!.", currentHouse->name); // TODO: adicionar sistema de não poder pagar
                    currentPlayer->properties[currentHouse->ID] = currentHouse->ID;
                    updateHousesRent(currentHouse->ID);
                    move(95,6); printf("Pressione qualquer botao para continuar... \n");
                    fflush(stdin);
                    move(95,7); getchar();
                    break;
                case '2':
                    break;
                case '3':
                    if((BuyAndSellHousesMenu(currentPlayer, houses))!= EXIT_SUCCESS) {
                            ClearRightScreen(0);
                            move(95,0); printf("error in BuyAndSellHousesMenu"); }
                        printPlayerInfo(currentPlayer);
                    break;
                };
            }
            break;

            case HOUSE_CMP:
            move(95,6); printf("Voce caiu na casa %s, ", currentHouse->name);
            if(currentHouse->isOwnedBySomeone)
            {
                if(currentHouse->ownerID == currentPlayer->ID) {
                    move(95,7); printf("Essa é uma propriedade sua!.\n");
                    move(95,8); printf("Pressione qualquer botao para continuar");
                    move(95,8); fflush(stdin); getchar();
                    break;
                }
                move(95,7); printf("ela e possuida por %s.\n", players[currentHouse->ownerID].nome);
                move(95,8); printf("Voce deve pagar %d$ ao proprietario.\n", housesToWalk*(currentHouse->rent));

                if(currentPlayer->netWorth < housesToWalk*(currentHouse->rent)) { // Falencia!
                    move(95,9); printf("O jogador %s nao tem como pagar o aluguel!.", currentPlayer->nome);
                    move(95,10); playerLosed(currentPlayer);
                    break; // TODO: TESTAR!!!!!!!!!!!!!!!!!!!!!!!!!!!
                } 
                else if(currentPlayer->money < housesToWalk*(currentHouse->rent)) {
                    while(!(currentPlayer->money >= housesToWalk*(currentHouse->rent))){    // TODO: TESTAR!!!!!!!!!!!!!!!!!!!!!!!!!!!
                        move(95,9); printf("Voce precisa vender algumas coisas para conseguir pagar o aluguel");
                        move(95,10); printf("Vamos abrir o menu de compra e venda para voce.");
                        move(95,11); printf("Aperte qualquer botao para continuar");
                        fflush(stdin);
                        move(95,12); getchar();
                        if((BuyAndSellHousesMenu(currentPlayer, houses))!= EXIT_SUCCESS) {
                            ClearRightScreen(0);
                            move(95,0); printf("error in BuyAndSellHousesMenu"); }
                        printPlayerInfo(currentPlayer);
                    }
                }
                currentPlayer->money -= housesToWalk*(currentHouse->rent);
            } 
            else 
            {
                move(95,7); printf("ela nao e possuida por ninguem.\n");
                move(95,8); printf("%s deseja comprar essa propriedade por %i?\n", currentPlayer->nome, currentHouse->cost);
                move(95,9); printf("1 - SIM"); move(95,10); printf("2 - NAO"); move(95,11); printf("3 - COMPRAR E VENDER CASAS");
                move(95,12); fflush(stdin); char opcao = getchar();
                switch (opcao) {
                case '1':
                    currentPlayer->money -= currentHouse->cost;
                    currentHouse->isOwnedBySomeone = TRUE;
                    currentHouse->ownerID = currentPlayer->ID;
                    addPropertieToPlayer(currentPlayer, houses, currentHouse->ID);
                    ClearRightScreen(4);
                    move(95,5); printf("Voce adquiriu a casa %s!.", currentHouse->name); // TODO: adicionar sistema de não poder pagar
                    updateHousesRent(currentHouse->ID);
                    move(95,6); printf("Pressione qualquer botao para continuar... \n");
                    fflush(stdin);
                    move(95,7); getchar();
                    break;
                case '2':
                    break;
                case '3':
                    if((BuyAndSellHousesMenu(currentPlayer, houses))!= EXIT_SUCCESS) {
                            ClearRightScreen(0);
                            move(95,0); printf("error in BuyAndSellHousesMenu"); }
                        printPlayerInfo(currentPlayer);
                    break;
                };
            }

            case GO_TO_JAIL:
            move(95,5); printf("Opa! Você está preso!");
            movePlayer(currentPlayer, currentPlayer->pos, 11);
            currentPlayer->pos = 11;
            currentPlayer->prisao = TRUE;
            currentPlayer->turnosPrisao = 3;
            goto turnoacabou;
            break;

            case FREE_DAY:
            move(95,5); printf("Você recebeu um dia de folga! Fique um turno sem jogar.");
            break;
            
            default:
            move(95,5); printf("Casa indefinida! Um beijo do desenvolvedor :3");
            break;
        }

        while(TRUE) {   //Turno do player atual
            ClearRightScreen(0);
            printPlayerInfo;
            changeTextColour(currentPlayer->colour);
            move(95,3); printf("%s ", currentPlayer->nome);
            changeTextColour(RESET); printf("o que deseja fazer?");
            move(95,4); printf("1 - COMPRAR E VENDER CASAS"); move(95,5); printf("2 - PASSAR TURNO"); move(95,6); printf("3 - DESISTIR");
            move(95,8); fflush(stdin); char opcao = getchar();
            switch (opcao){
                case '1':
                    if((BuyAndSellHousesMenu(currentPlayer, houses))!= EXIT_SUCCESS) {
                            ClearRightScreen(0);
                            move(95,0); printf("error in BuyAndSellHousesMenu"); }
                    break;
                case '2':
                    goto turnoacabou;
                    break;
                case '3':
                    move(95,9); printf("Voce tem certeza disso? nao sera possivel voltar... (1 = SIM)");
                    fflush(stdin);
                    move(95,10); char opcao2 = getchar();
                    if(opcao2 == '1') {
                        ClearRightScreen(0);
                        playerLosed(currentPlayer);
                        goto turnoacabou;
                    } else {
                        move(95,11); printf("Tudo bem entao, nada sera feito.");
                        move(95,12); fflush(stdin); getchar();
                    }
                    break;
                default:
                    move(95,9); printf("Opcao invalida, por favor tente novamente.");
                    move(95,12); fflush(stdin); getchar();
                    break;
            };

            move(95,16); printf("Pressione qualquer botao para continuar... \n");
            fflush(stdin);
            move(95,17); getchar();
        }

        turnoacabou:
        move(95,15); printf("Seu turno acabou!");
        move(95,16); printf("Pressione qualquer botao para continuar... \n");
        fflush(stdin);
        move(95,17); getchar();
        turnos++;
    }
    return EXIT_SUCCESS;
}

// int main
int main(){
    choice_mainmenu:
    clearScreen();
    printf("\n                                              888b     d888  .d88888b.  888b    888  .d88888b.  8888888b.   .d88888b.  888    Y88b   d88P           "); 
    printf("\n                                              8888b   d8888 d88P\" \"Y88b 8888b   888 d88P\" \"Y88b 888   Y88b d88P\" \"Y88b 888     Y88b d88P      ");       
    printf("\n                                              88888b.d88888 888     888 88888b  888 888     888 888    888 888     888 888      Y88o88P             "); 
    printf("\n                                              888Y88888P888 888     888 888Y88b 888 888     888 888   d88P 888     888 888       Y888P              "); 
    printf("\n                                              888 Y888P 888 888     888 888 Y88b888 888     888 8888888P\"  888     888 888        888              ");  
    printf("\n                                              888  Y8P  888 888     888 888  Y88888 888     888 888        888     888 888        888               "); 
    printf("\n                                              888   \"   888 Y88b. .d88P 888   Y8888 Y88b. .d88P 888        Y88b. .d88P 888        888              ");  
    printf("\n                                              888       888  \"Y88888P\"  888    Y888  \"Y88888P\"  888         \"Y88888P\"  88888888   888         ");       
    printf("\n\n\n\n\n");
    srand((unsigned)time(NULL));
    //checkWindowSize(172, 40);     // TODO: RESOLVER ISSO AQUI DE ALGUM JEITO
    printf("1- Novo Jogo \n");
    printf("2- Instrucoes \n");
    printf("3- Creditos \n");
    fflush(stdin);
    char choice = getchar();
    if (choice == '1') 
    {
        if(maingame()!= EXIT_SUCCESS)
        {
            printf("Error in maingame");
            getchar();
        }
    }
    else if (choice == '2')
    {
    	clearScreen();
	char url[]="regras.txt";
	char ch;
	    FILE *instrucao;
	    instrucao = fopen(url, "r");
	    while((ch=fgetc(instrucao))!=EOF){
	    putchar(ch);
	    }
	    printf("\nAperte qualquer botao para fechar o menu de instrucoes");
	    fflush(stdin);
	    getchar();
	    fclose(instrucao);
	    goto choice_mainmenu;
    }

    else if (choice == '3')
    {
	    clearScreen();
	    printf("\nTrabalho desenvolvido pelos Alunos:\n");
	    printf("Joao Vitor Pinto Vizeu\n");
	    printf("Joao Ricardo Monteiro Scofield Lauar\n");
	    printf("Joao Marcos\n");
	    printf("Marias Vilas-Boas\n");
	    printf("Bernardo Pache\n");

	    printf("\nProfessora: Priscila Machado Vieira Lima\n");
	    printf("Materia: Algoritmos e Programacao\n");
	    fflush(stdin);
	    getchar();
	    goto choice_mainmenu;
    }
    else
    {
        printf("Opcao invalida! Por favor tente novamente.\n");
        goto choice_mainmenu; 
    }
}