#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "player.h"


void TestCode(){
  
  //list of bestplayer
  BestList *bestplayers=InitBestList();

  //playergames
  Playersgame *pgame=malloc(sizeof(Playersgame));
  NodePlayer *player1=NULL;
  NodePlayer *player2=NULL;

  //add 2 players
  printf("Add 2 players\n");
  PlayerList *players=InitplayerList();
  Addplayer(players,1);
  Addplayer(players,2);
  PrintPlayerList(players);
  
  //Add 2 more players
  printf("\nAdd 2 more players\n");
  Addplayer(players,3);
  Addplayer(players,4);
  PrintPlayerList(players);
  
  //Remove player 3
  printf("\nRemove player 3\n");
  RemovePlayer(players,3,bestplayers);
  PrintPlayerList(players);

  //Search player 4
  printf("\nSearch player 4\n");
  PrintPlayer(SearchNum(players,4));
  
  //Search name Heatblast
  printf("\n Search name Heatblast\n");
  PrintPlayer(SearchName(players,"Heatblast"));
  
  //make a play and modify the best list
  player1=SearchNum(players, 1);
  player2=SearchNum(players, 2);
  ((Player*)player1->data)->win++;
  ((Player*)player2->data)->lost++;
  AddBest(player1, player2, bestplayers);
  printf("\nShow best player\n");
  ShowBest(bestplayers);

  //make 2 plays and modify the best list
  player1=SearchNum(players, 2);
  player2=SearchNum(players, 1);
  ((Player*)player1->data)->win++;
  ((Player*)player2->data)->lost++;
  AddBest(player1, player2, bestplayers);

  player1=SearchNum(players, 2);
  player2=SearchNum(players, 1);
  ((Player*)player1->data)->win++;
  ((Player*)player2->data)->lost++;
  AddBest(player1, player2, bestplayers);
  printf("\nShow list of best players\n");
  PrintBestList(bestplayers);
  printf("%d\n",bestplayers->nelem);

  

  //make 2 plays and modify the best list
  player1=SearchNum(players, 1);
  player2=SearchNum(players, 2);
  ((Player*)player1->data)->win++;
  ((Player*)player2->data)->lost++;
  AddBest(player1, player2, bestplayers);

  player1=SearchNum(players, 1);
  player2=SearchNum(players, 2);
  ((Player*)player1->data)->win++;
  ((Player*)player2->data)->lost++;
  AddBest(player1, player2, bestplayers);
  printf("\nShow list of best players\n");
  PrintBestList(bestplayers);
  
  

  //make 2 plays and modify the best list
  player1=SearchNum(players, 2);
  player2=SearchNum(players, 4);
  ((Player*)player1->data)->win++;
  ((Player*)player2->data)->lost++;
  AddBest(player1, player2, bestplayers);

  player1=SearchNum(players, 2);
  player2=SearchNum(players, 4);
  ((Player*)player1->data)->win++;
  ((Player*)player2->data)->lost++;
  AddBest(player1, player2, bestplayers);
  printf("\nShow new best player\n");
  ShowBest(bestplayers);
  
  //make a play and modify the best list
  player1=SearchNum(players, 1);
  player2=SearchNum(players, 2);
  ((Player*)player1->data)->win++;
  ((Player*)player2->data)->lost++;
  AddBest(player1, player2, bestplayers);

  printf("\nShow list of best players\n");
  PrintBestList(bestplayers);
  
  //make 3 plays and modify the best list
  player1=SearchNum(players, 1);
  player2=SearchNum(players, 4);
  ((Player*)player1->data)->lost+=3;
  ((Player*)player2->data)->win+=3;
  AddBest(player1, player2, bestplayers);

  printf("\nShow updated list of best players\n");
  PrintBestList(bestplayers);

  

  //make 2 plays and modify the best list
  player1=SearchNum(players, 2);
  player2=SearchNum(players, 4);
  ((Player*)player1->data)->lost+=2;
  ((Player*)player2->data)->win+=2;
  AddBest(player1, player2, bestplayers);
  
  printf("\nShow updated list of best players\n");
  PrintBestList(bestplayers);

  //add a player
  Addplayer(players, 5);
  printf("\nAdd 1 more player\n");
  PrintPlayerList(players);

  //make 6 plays and modify the best list
  player1=SearchNum(players, 3);
  player2=SearchNum(players, 1);
  ((Player*)player1->data)->win+=6;
  ((Player*)player2->data)->lost+=6;
  AddBest(player1, player2, bestplayers);

  //remove player 4
  printf("\nRemove player 4\n");
  RemovePlayer(players, 4,bestplayers);
  PrintPlayerList(players);
  
  printf("\nShow updated list of best players\n");
  PrintBestList(bestplayers);

  player1=SearchNum(players, 1);
  player2=SearchNum(players, 2);
  ((Player*)player1->data)->win++;
  ((Player*)player2->data)->lost++;
  AddBest(player1, player2, bestplayers);

  printf("\nShow updated list of best players\n");
  PrintBestList(bestplayers);

  player1=SearchNum(players, 2);
  player2=SearchNum(players, 1);
  ((Player*)player1->data)->win+=2;
  ((Player*)player2->data)->lost+=2;
  AddBest(player1, player2, bestplayers);

  printf("\nShow updated list of best players\n");
  PrintBestList(bestplayers);
}