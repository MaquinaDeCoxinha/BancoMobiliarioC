#include "houses.c"

void printTable(){
    clearScreen();
    restoreConsole();
    printf("\n");
    printf("\n                ST.CH  ELCT  STATES VRGNIA PNSLVA  ST.JA   COM    TEN    NY    FREE         ");
    printf("\n                 AVE   COMP    AVE    AVE   RAIL    AVE   CHEST   AVE   AVE    PARK         ");
    printf("\n        ____________________________________________________________________________        ");
    printf("\n VISIT |      |      |      |      |      |      |      |      |      |      |      |       ");
    printf("\n  JAIL |      |      |      |      |      |      |      |      |      |      |      |       ");
    printf("\n       |______|______|______|______|______|______|______|______|______|______|______|       ");
    printf("\n CNNCT |      |                                                              |      | KNTCY ");
    printf("\n  AVE  |      |                                                              |      |  AVE  ");
    printf("\n       |______|                                                              |______|       ");
    printf("\n VERMNT|      |                                                              |      |       ");
    printf("\n  AVE  |      |                                                              |      |CHANCE ");
    printf("\n       |______|                                                              |______|       ");
    printf("\n       |      |                                                              |      |  IND  ");
    printf("\n CHANCE|      |                                                              |      |  AVE  ");
    printf("\n       |______|                                                              |______|       ");
    printf("\n ORNTL |      |                                                              |      |  ILL  ");
    printf("\n  AVE  |      |                                                              |      |  AVE  ");
    printf("\n       |______|                                                              |______|       ");
    printf("\n  READ |      |                                                              |      | B&O.  ");
    printf("\n  RAIL |      |                                                              |      | RAIL  ");
    printf("\n       |______|                                                              |______|       ");
    printf("\n       |      |                                                              |      |ATLNTC ");
    printf("\n  TAX  |      |                                                              |      |  AVE  ");
    printf("\n       |______|                                                              |______|       ");
    printf("\n BALTIC|      |                                                              |      | VNTNR ");
    printf("\n  AVE  |      |                                                              |      |  AVE  ");
    printf("\n       |______|                                                              |______|       ");
    printf("\n  COM  |      |                                                              |      | WATER ");
    printf("\n CHEST |      |                                                              |      | WORKS ");
    printf("\n       |______|                                                              |______|       ");
    printf("\n MEDTRN|      |                                                              |      | MRVN  ");
    printf("\n  AVE  |      |                                                              |      | GRDNS ");
    printf("\n       |______|______________________________________________________________|______|       ");
    printf("\n       |      |      |      |      |      |      |      |      |      |      |      | GO TO ");
    printf("\n START |      |      |      |      |      |      |      |      |      |      |      |  JAIL ");
    printf("\n       |______|______|______|______|______|______|______|______|______|______|______|       ");
    printf("\n                BRDWK LUXURY  PARK  CHANCE  SHORT PNSLVA   COM    NC    PCFC                ");
    printf("\n                       TAX    PLACE         LINE    AVE   CHEST   AVE   AVE                 ");
}
