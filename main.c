//gcc main.c player.c TestPlayerCode.c -o main_bin -lm -pthread
//./main_bin
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "player.h"
#include "TestPlayerCode.h"



int main(){
  srand (time(NULL));
  setbuf(stdout, NULL);
  unsigned int choice,x=TRUE;
  char buf[MAX];
  

  /***************************************TestPlayerCode****************************************/
  int test=FALSE;
  if(test){
    TestCode();
    return 0;
  }
  /********************************************************************************************/



  //list of bestplayer
  BestList *bestplayers=InitBestList();

  //playergames
  Playersgame *pgame=malloc(sizeof(Playersgame));
  NodePlayer *player1=NULL;
  NodePlayer *player2=NULL;
  printf(
    "-----------------------------------------------------------------------\n                              ALIEN WARFERE\n-----------------------------------------------------------------------\n");
  sleep(2);

  //init list of of players
  PlayerList *players=InitplayerList();

  //start 2 players
  printf("add 2 players for start\n");
  printf("Press Enter to continue\n");
  getchar();
  Addplayer(players,0);
  Addplayer(players,0);

  //start menu
  while(x){
    printf("\n1.Add New Player\n2.Remove Player\n3.Show Players\n4.Show Best Player\n5.Search Player\n6.Play\n7.quit\nchoice:");
    scanf("%d",&choice);
    fflush(stdin);
    //verify data
    while(choice<1 || choice>7){
      printf("invalid data, enter again:");
      scanf("%d",&choice);
      fflush(stdin);
    }
    printf("\n");
    switch(choice){
      case 1:
      //ad player to the list
        Addplayer(players,0);
      break;
      case 2:
        printf("\nWhich player you want to remove?:");
        scanf("%d",&choice);
        fflush(stdin);
        //player not found
        if(SearchNum(players, choice)==NULL){ 
          printf("player does not exist\n");
          fflush(stdin);
          getchar();
          getchar();
        }
        //remove player for the list 
        else RemovePlayer(players,choice,bestplayers);
      break;
      case 3:
      //show all the players
        PrintPlayerList(players);
        printf("Press Enter to continue\n");
        fflush(stdin);
        getchar();
        getchar();
      break;
      case 4:
      //not best player
        if (bestplayers->nelem==0){
          printf("There is not best player\n");
        }
        else{
          printf("1.Show best player\n2.Show list of best players\nchoice:");
          scanf("%d",&choice);
          fflush(stdin);
          //verify data
          while(choice<1 || choice>2){
            printf("invalid data, enter again:");
            scanf("%d",&choice);
            fflush(stdin);
          }
          printf("\n");
          //show best player
          if(choice==1) ShowBest(bestplayers);

          //show list of best players 
          else PrintBestList(bestplayers); 
        }
        printf("Press Enter to continue\n");
        fflush(stdin);
        getchar();
        getchar();
      break;
      case 5:
        printf("1.search by number of player\n2.search by name\nchoice:");
        scanf("%d",&choice);
        fflush(stdin);
        //verify data
        while(choice<1 || choice>2){
          printf("invalid data, enter again:");
          scanf("%d",&choice);
          fflush(stdin);
        }
        //search by nunber of player
        if(choice==1){
          printf("\nwhich number you are searching:");
          scanf("%d",&choice);
          fflush(stdin);
          printf("\n");
          if(SearchNum(players,choice)==NULL){
            printf("player not found\n");
          }
          else{
            PrintPlayer(SearchNum(players,choice));
          }
        }
        //search by name of alien
        else{
          printf("\nwhich name you are searching:");
          fflush(stdin);
          scanf("%s",buf);
          fflush(stdin);
          printf("\n");
          if(SearchName(players,buf)==NULL){
            printf("name not found\n");
          }
          else{
            PrintPlayer(SearchName(players,buf));
          }
        }
        printf("Press Enter to continue\n");
        fflush(stdin);
        getchar();
        getchar();
      break;
      case 6:
      //star game whit list of 2 players
        if(players->nelem==2){
          player1=players->root;
          player2=players->root->nxt;
          pgame->player1=players->root;
          pgame->player2=players->root->nxt;
        }
        //star game whit list of 3 or more players
        else{
          printf("number of the first player:");
          scanf("%d",&choice);
          fflush(stdin);
          player1=SearchNum(players, choice);
          //verify player exists
          while(player1==NULL){
            printf("player not found\n");
            printf("number of the first player:");
            scanf("%d",&choice);
            fflush(stdin);
            player1=SearchNum(players, choice);
          }
          pgame->player1=player1;
          
          printf("number of the second player:");
          scanf("%d",&choice);
          fflush(stdin);
          player2=SearchNum(players, choice);
          
          //verify player exists
          if(player2->numplayer==player1->numplayer){
            printf("player already choice\n");
            choice=0;
            player2=NULL;
          }
          while(player2==NULL){
            if(choice!=0) printf("player not found\n");
            printf("number of the first player:");
            scanf("%d",&choice);
            fflush(stdin);
            player2=SearchNum(players, choice);
            if(player2!=NULL){
              if(player2->numplayer==player1->numplayer){
                printf("player already choice\n");
                choice=0;
                player2=NULL;
              }
            }
          }
          pgame->player2=player2;
        }
        //run game
        Play(pgame);
        //modify list of best players
        AddBest(player1, player2, bestplayers);

        //restart variables player 1
        ((Player*)player1->data)->hp=((Player*)player1->data)->hpmax;
        ((Player*)player1->data)->doub=FALSE;
        ((Player*)player1->data)->ability=0;
        ((Player*)player1->data)->defense=FALSE;
        
        //restart variables player 2
        ((Player*)player2->data)->hp=((Player*)player2->data)->hpmax;
        ((Player*)player2->data)->doub=FALSE;
        ((Player*)player2->data)->ability=0;
        ((Player*)player2->data)->defense=FALSE;
      break;
      case 7:
      //end manu
        x=FALSE;
        printf("Thanks for playing\n");
      break;
    }
  }
  
  
  
  

  return 0;
}