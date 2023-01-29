#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "player.h"

//main variable of the game 
int vg=TRUE;


/*********************BestList******************************/
//initialize list of best players 
BestList *InitBestList(){
  BestList *gamelist=(BestList*) malloc(sizeof(BestList));
  gamelist->root=NULL;
  gamelist->nelem=0;

  return gamelist;
}

//initialize node of best players
NodeBest *InitNodeBest(){
  NodeBest *newbest=(NodeBest*)malloc(sizeof(NodeBest));
  newbest->data=NULL;
  newbest->nxt=newbest;
  newbest->prv=newbest;
  return newbest;
}

//show best player
void ShowBest(BestList *bestlist){
  PrintPlayer(bestlist->root->data);
}

//search for a player in the best list 
NodeBest *SearchBest(BestList *gamelist,int n){
  NodeBest *tmp=gamelist->root;

  //if list empty return
  if(gamelist->nelem==0) return NULL;

  //check head
  if(((NodePlayer*)tmp->data)->numplayer==n) return tmp;

  tmp=tmp->nxt;
  //go through the list
  while(tmp!=gamelist->root){
    //check node
    if(((NodePlayer*)tmp->data)->numplayer==n){
      return tmp;
      
    }
    tmp=tmp->nxt;
  }
  
  return NULL;
}

//add new best player
NodeBest *AddNodeBest(NodeBest *root,NodeBest *newplayer){
  root->prv->nxt=newplayer;
  newplayer->prv=root->prv;
  newplayer->nxt=root;
  root->prv=newplayer;

  return newplayer;
}

//find tne position of a player in the best list
int FindPos(BestList *bestlist, NodeBest *player){
  int cont=0;
  NodeBest *tmp=bestlist->root;
  if(tmp !=player){
    tmp=tmp->nxt;
    cont++;
    //go through list
    while(tmp!=bestlist->root){
      //check node
      if(tmp==player) return cont;
      tmp=tmp->nxt;
      cont++;
    }
  }

  return cont;
}

//add player in a specific position
NodeBest *AddNodePos(BestList *bestlist,NodeBest *newplayer, int pos){
  NodeBest *tmp=bestlist->root;
  //reach position
  for(int i=0;i<pos;i++) tmp=tmp->nxt;
  //add player
  tmp->prv->nxt=newplayer;
  newplayer->prv=tmp->prv;
  newplayer->nxt=tmp;
  tmp->prv=newplayer;
  

  return bestlist->root;
}

//remove player of the best list
void DelBest(BestList *bestlist,int n,int ele){
  NodeBest *tmp=bestlist->root;
  //check head
  if(((NodePlayer*)tmp->data)->numplayer==n){
    //list only have 1 node 
    if(tmp->nxt==tmp) bestlist->root=NULL;
    
    //list have more than 1 node 
    else{ 
      //remove node
      tmp->prv->nxt=tmp->nxt;
      tmp->nxt->prv=tmp->prv;
      bestlist->root=tmp->nxt;
      tmp->prv=NULL;
      tmp->nxt=NULL;
    }
  }
  //go through the list
  else{
    tmp=tmp->nxt;
    while(tmp!=bestlist->root){
      //check node
      if(((NodePlayer*)tmp->data)->numplayer==n){
        //remove node
        tmp->prv->nxt=tmp->nxt;
        tmp->nxt->prv=tmp->prv;
        tmp->prv=NULL;
        tmp->nxt=NULL;
        break;
      }
      tmp=tmp->nxt;
    }
    
  }
  bestlist->nelem--;
  //eliminate node
  if(ele) free(tmp);
}

//cheack if the player have to be reorganize becuase of he's losses in the best list
void CheckLosses(BestList *bestlist,NodePlayer *player){
  int pos;
  NodeBest *best;
  //check if list is empty
  if(bestlist->nelem!=0){
    
    //check if loser player is in best list 
    if(SearchBest(bestlist, player->numplayer) != NULL){
      //save the node of loser
      best=SearchBest(bestlist, player->numplayer);
      //check number of players in best list
      if(bestlist->nelem==2){
        //check if loser is the head
        if(best == bestlist->root){
          //check if loser have the same wins than the next player
          if(((Player*)((NodePlayer*)best->data)->data)->win ==((Player*)((NodePlayer*)best->nxt->data)->data)->win){
            //check if loser have more losses tan next player
            if(((Player*)((NodePlayer*)best->data)->data)->lost >((Player*)((NodePlayer*)best->nxt->data)->data)->lost){
              //change positions
              best=best->nxt;
              DelBest(bestlist, ((NodePlayer*)best->data)->numplayer,FALSE);
              bestlist->root=AddNodeBest(bestlist->root, best);
            }
          }
        }
      }
      else{
        //check wins of loser and next player
        while(((Player*)((NodePlayer*)best->data)->data)->win == ((Player*)((NodePlayer*)best->nxt->data)->data)->win && best->nxt!=bestlist->root){
          //check if loser have more losses tan next player
          if(((Player*)((NodePlayer*)best->data)->data)->lost > ((Player*)((NodePlayer*)best->nxt->data)->data)->lost){
            //change positions
            pos=FindPos(bestlist, best);
            DelBest(bestlist, ((NodePlayer*)best->data)->numplayer,FALSE);
            bestlist->root=AddNodePos(bestlist, best,pos-1);
          }
          else break;
        }
      }
    }
  }
}

//cheack if the player have to be reorganize becuase of he's wins in the best list
void CheckWins(BestList *bestlist,NodePlayer *player){
  int pos;
  NodeBest *best;
  if (bestlist!=0){
    if(SearchBest(bestlist, player->numplayer)!=NULL){
      best=SearchBest(bestlist, player->numplayer);
      if(bestlist->nelem==2){
        return;
      }
      else{
        while(((Player*)((NodePlayer*)best->data)->data)->win > ((Player*)((NodePlayer*)best->prv->data)->data)->win){
          pos=FindPos(bestlist, best);
          DelBest(bestlist, ((NodePlayer*)best->data)->numplayer,FALSE);
          bestlist->root=AddNodePos(bestlist, best,pos+1);
        }
      }
    }
  }
}

//count the number of players in best list
int ContBest(BestList *bestlist){
  int cont=0;
  NodeBest *tmp=bestlist->root;
  do{
    cont++;
    tmp=tmp->nxt;
  }while(tmp!=bestlist->root);

  return cont;
}

//add and modify best list 
void AddBest(NodePlayer *player1, NodePlayer *player2, BestList *bestlist){
  int pos;
  NodeBest *newbest=InitNodeBest();
  NodeBest *best;
  //check who is the best player of those who played
  //have the same wins
  if(bestlist->nelem==0){
    if(((Player*)player1->data)->win==((Player*)player2->data)->win && ((Player*)player1->data)->lost==((Player*)player2->data)->lost){
      return;
    }
  }
  //check wins
  if(((Player*)player1->data)->win == ((Player*)player2->data)->win){
    //check how have more losses
    if(((Player*)player1->data)->lost < ((Player*)player2->data)->lost) {
      //save winner
      newbest->data=player1;
    }

    else {
      //save winner
      newbest->data=player2;
    }
  }
  //check wins
  else if (((Player*)player1->data)->win > ((Player*)player2->data)->win){
    //save winner
    newbest->data=player1;
  }
  else {
    //save winner
    newbest->data=player2;
  }
  //organize players
  CheckWins(bestlist, player1);
  CheckWins(bestlist, player2);
  CheckLosses(bestlist, player1);
  CheckLosses(bestlist, player2);

  //check if best list is empty
  if (bestlist->nelem == 0){
    bestlist->root=newbest;
  }
  //check if winner have the same wins than the head
  if(((Player*)((NodePlayer*)bestlist->root->data)->data)->win == ((Player*)((NodePlayer*)newbest->data)->data)->win){
    //check who has more lost
    if(((Player*)((NodePlayer*)bestlist->root->data)->data)->lost > ((Player*)((NodePlayer*)newbest->data)->data)->lost){
      //check winner in best list
      if(SearchBest(bestlist, ((NodePlayer*)newbest->data)->numplayer) == NULL){
        bestlist->root=AddNodeBest(bestlist->root, newbest);
      }
      else{
        //remive winner
        DelBest(bestlist, ((NodePlayer*)newbest->data)->numplayer,FALSE);
        //ad in the head
        bestlist->root=AddNodeBest(bestlist->root, newbest);
      }
    }
  }
  //check who has more lost
  else if (((Player*)((NodePlayer*)bestlist->root->data)->data)->win < ((Player*)((NodePlayer*)newbest->data)->data)->win){
    //check winner in best list
    if(SearchBest(bestlist, ((NodePlayer*)newbest->data)->numplayer) == NULL){
      bestlist->root=AddNodeBest(bestlist->root, newbest);
    }
    else{
      //remive winner
      DelBest(bestlist, ((NodePlayer*)newbest->data)->numplayer,FALSE);
      //ad in the head
      bestlist->root=AddNodeBest(bestlist->root, newbest);
    }
  }
  //increase elements in best list
  bestlist->nelem=ContBest(bestlist);
}

//show list of best players
void PrintBestList(BestList *bestlist){
  NodeBest *tmp=bestlist->root;
  //show head
  PrintPlayer(tmp->data);
  tmp=tmp->nxt;
  //go through the list
  while(tmp!= bestlist->root){
    //show node
    PrintPlayer(tmp->data);
    tmp=tmp->nxt;
  }
}

/*********************PlayerList******************************/

//show node
void PrintPlayer(NodePlayer *player){
  printf("player %d Name:%11s   HP:%.1f   Damege:%d   Evade:%d W:%d L:%d\n",player->numplayer,((Player*)player->data)->name,((Player*)player->data)->hp,((Player*)player->data)->dam,((Player*)player->data)->evading,((Player*)player->data)->win,((Player*)player->data)->lost);
}

//create an alien 
void PersonalAlien(NodePlayer *player){
  int choice;
  ((Player*)player->data)->name=malloc(sizeof(MAX));
  //get name
  printf("\nwhat's your alien's name?(max 11 characters): ");
  fflush(stdin);
  scanf("%s",((Player*)player->data)->name);
  fflush(stdin);
  
  //get HP
  printf("\nhow much HP do you want to have?(min 100 max 120): ");
  scanf("%d",&((Player*)player->data)->hpmax);
  fflush(stdin);
  //verify data
  while(((Player*)player->data)->hpmax<100 || ((Player*)player->data)->hpmax>120){
    printf("invalid data, enter again:");
    scanf("%d",&((Player*)player->data)->hpmax);
    fflush(stdin);
  }
  ((Player*)player->data)->hp=((Player*)player->data)->hpmax;
  //get damage or evade
  printf("\n1.Damage: increased damage\n2.Evade: increased chance of not taking damage\nwhat do you prefer?:");
  scanf("%d",&choice);
  fflush(stdin);
  //verify data
  while(choice<1 || choice>2){
    printf("invalid data, enter again:");
    scanf("%d",&choice);
    fflush(stdin);
  }
  //assign damage and evade
  if (choice==1){
    switch (((Player*)player->data)->hpmax){
      case 100:
        ((Player*)player->data)->dam=20;
        ((Player*)player->data)->evading=50;
      break;
      case 101:
        ((Player*)player->data)->dam=18;
        ((Player*)player->data)->evading=50;
      break;
      case 102:
        ((Player*)player->data)->dam=18;
        ((Player*)player->data)->evading=45;
      break;
      case 103:
        ((Player*)player->data)->dam=17;
        ((Player*)player->data)->evading=45;
      break;
      case 104:
        ((Player*)player->data)->dam=17;
        ((Player*)player->data)->evading=45;
      break;
      case 105:
        ((Player*)player->data)->dam=17;
        ((Player*)player->data)->evading=45;
      break;
      case 106:
        ((Player*)player->data)->dam=17;
        ((Player*)player->data)->evading=40;
      break;
      case 107:
        ((Player*)player->data)->dam=16;
        ((Player*)player->data)->evading=40;
      break;
      case 108:
        ((Player*)player->data)->dam=16;
        ((Player*)player->data)->evading=40;
      break;
      case 109:
        ((Player*)player->data)->dam=16;
        ((Player*)player->data)->evading=40;
      break;
      case 110:
        ((Player*)player->data)->dam=13;
        ((Player*)player->data)->evading=60;
      break;
      case 111:
        ((Player*)player->data)->dam=14;
        ((Player*)player->data)->evading=55;
      break;
      case 112:
        ((Player*)player->data)->dam=15;
        ((Player*)player->data)->evading=30;
      break;
      case 113:
        ((Player*)player->data)->dam=15;
        ((Player*)player->data)->evading=30;
      break;
      case 114:
        ((Player*)player->data)->dam=15;
        ((Player*)player->data)->evading=30;
      break;
      case 115:
        ((Player*)player->data)->dam=14;
        ((Player*)player->data)->evading=25;
      break;
      case 116:
        ((Player*)player->data)->dam=14;
        ((Player*)player->data)->evading=25;
      break;
      case 117:
        ((Player*)player->data)->dam=14;
        ((Player*)player->data)->evading=25;
      break;
      case 118:
        ((Player*)player->data)->dam=13;
        ((Player*)player->data)->evading=20;
      break;
      case 119:
        ((Player*)player->data)->dam=13;
        ((Player*)player->data)->evading=20;
      break;
      case 120:
        ((Player*)player->data)->dam=13;
        ((Player*)player->data)->evading=20;
      break;
    }
  }
  else{
    switch (((Player*)player->data)->hpmax){
      case 100:
        ((Player*)player->data)->dam=10;
        ((Player*)player->data)->evading=80;
      break;
      case 101:
        ((Player*)player->data)->dam=10;
        ((Player*)player->data)->evading=80;
      break;
      case 102:
        ((Player*)player->data)->dam=10;
        ((Player*)player->data)->evading=75;
      break;
      case 103:
        ((Player*)player->data)->dam=11;
        ((Player*)player->data)->evading=75;
      break;
      case 104:
        ((Player*)player->data)->dam=11;
        ((Player*)player->data)->evading=75;
      break;
      case 105:
        ((Player*)player->data)->dam=11;
        ((Player*)player->data)->evading=70;
      break;
      case 106:
        ((Player*)player->data)->dam=11;
        ((Player*)player->data)->evading=70;
      break;
      case 107:
        ((Player*)player->data)->dam=12;
        ((Player*)player->data)->evading=65;
      break;
      case 108:
        ((Player*)player->data)->dam=12;
        ((Player*)player->data)->evading=65;
      break;
      case 109:
        ((Player*)player->data)->dam=12;
        ((Player*)player->data)->evading=65;
      break;
      case 110:
        ((Player*)player->data)->dam=13;
        ((Player*)player->data)->evading=60;
      break;
      case 111:
        ((Player*)player->data)->dam=14;
        ((Player*)player->data)->evading=55;
      break;
      case 112:
        ((Player*)player->data)->dam=13;
        ((Player*)player->data)->evading=50;
      break;
      case 113:
        ((Player*)player->data)->dam=12;
        ((Player*)player->data)->evading=50;
      break;
      case 114:
        ((Player*)player->data)->dam=12;
        ((Player*)player->data)->evading=50;
      break;
      case 115:
        ((Player*)player->data)->dam=11;
        ((Player*)player->data)->evading=45;
      break;
      case 116:
        ((Player*)player->data)->dam=11;
        ((Player*)player->data)->evading=45;
      break;
      case 117:
        ((Player*)player->data)->dam=11;
        ((Player*)player->data)->evading=45;
      break;
      case 118:
        ((Player*)player->data)->dam=10;
        ((Player*)player->data)->evading=40;
      break;
      case 119:
        ((Player*)player->data)->dam=10;
        ((Player*)player->data)->evading=40;
      break;
      case 120:
        ((Player*)player->data)->dam=10;
        ((Player*)player->data)->evading=40;
      break;
    }
  }
  PrintPlayer(player);
  printf("\n");
}

//choose an alien
void ChooseCaracter(NodePlayer *player,int test){
  int choice;
  //arrey whit names of aliens
  char *names[]={"Heatblast","Wildmutt","Diamondhead","XLR8","Grey Matter","Four Arms","Stinkfly","Ripjaws","Upgrade","Ghostfreak"};
  if(test==0){
    printf("\n  Name            HP    damage    probability of evading\n1.Heatblast      121     13                35\n2.Wildmutt       123     12                65\n3.Diamondhead    130     10                20\n4.XRL8           113     14                70\n5.Grey Matter    100     13                80\n6.Four Arms      115     15                10\n7.Stinkfly       105     17                70\n8.Ripjaws        100     20                50\n9.Upgrade        118     14                65\n10.Ghostfreak    110     14                75\n11.personal alien: create a new alien\nPlayer %d choose a Alien:",player->numplayer);
    scanf("%d",&choice);
    fflush(stdin);
  }
  else{
    choice=test;
  }

  //verify data
  while(choice<1 || choice>11){
    printf("invalid data, enter again:");
    scanf("%d",&choice);
    fflush(stdin);
  }

  //assign stats
  switch (choice){
    case 1:
      ((Player*)player->data)->name=(char*)malloc(sizeof(*names));
      strcpy(((Player*)player->data)->name,*names);
      ((Player*)player->data)->hp=121;
      ((Player*)player->data)->hpmax=121;
      ((Player*)player->data)->dam=13;
      ((Player*)player->data)->evading=35;
       
    break;
    case 2: 
      ((Player*)player->data)->name=(char*)malloc(sizeof(*(names+1)));
      strcpy(((Player*)player->data)->name,*(names+1));
      ((Player*)player->data)->hp=123;
      ((Player*)player->data)->hpmax=123;
      ((Player*)player->data)->dam=12;
      ((Player*)player->data)->evading=65;
    break;
    case 3: 
      ((Player*)player->data)->name=(char*)malloc(sizeof(*(names+2)));
      strcpy(((Player*)player->data)->name,*(names+2));
      ((Player*)player->data)->hp=130;
      ((Player*)player->data)->hpmax=130;
      ((Player*)player->data)->dam=10;
      ((Player*)player->data)->evading=20;
    break;
    case 4: 
      ((Player*)player->data)->name=(char*)malloc(sizeof(*(names+3)));
      strcpy(((Player*)player->data)->name,*(names+3));
      ((Player*)player->data)->hp=113;
      ((Player*)player->data)->hpmax=113;
      ((Player*)player->data)->dam=14;
      ((Player*)player->data)->evading=70;
    break;
    case 5: 
      ((Player*)player->data)->name=(char*)malloc(sizeof(*(names+4)));
      strcpy(((Player*)player->data)->name,*(names+4));
      ((Player*)player->data)->hp=100;
      ((Player*)player->data)->hpmax=100;
      ((Player*)player->data)->dam=13;
      ((Player*)player->data)->evading=80;
    break;
    case 6: 
      ((Player*)player->data)->name=(char*)malloc(sizeof(*(names+5)));
      strcpy(((Player*)player->data)->name,*(names+5));
      ((Player*)player->data)->hp=115;
      ((Player*)player->data)->hpmax=115;
      ((Player*)player->data)->dam=15;
      ((Player*)player->data)->evading=10;
    break;
    case 7: 
      ((Player*)player->data)->name=(char*)malloc(sizeof(*(names+6)));
      strcpy(((Player*)player->data)->name,*(names+6));
      ((Player*)player->data)->hp=105;
      ((Player*)player->data)->hpmax=105;
      ((Player*)player->data)->dam=17;
      ((Player*)player->data)->evading=70;
    break;
    case 8: 
      ((Player*)player->data)->name=(char*)malloc(sizeof(*(names+7)));
      strcpy(((Player*)player->data)->name,*(names+7));
      ((Player*)player->data)->hp=100;
      ((Player*)player->data)->hpmax=100;
      ((Player*)player->data)->dam=20;
      ((Player*)player->data)->evading=50;
    break;
    case 9: 
      ((Player*)player->data)->name=(char*)malloc(sizeof(*(names+8)));
      strcpy(((Player*)player->data)->name,*(names+8));
      ((Player*)player->data)->hp=118;
      ((Player*)player->data)->hpmax=118;
      ((Player*)player->data)->dam=14;
      ((Player*)player->data)->evading=65;
    break;
    case 10: 
      ((Player*)player->data)->name=(char*)malloc(sizeof(*(names+9)));
      strcpy(((Player*)player->data)->name,*(names+9));
      ((Player*)player->data)->hp=110;
      ((Player*)player->data)->hpmax=110;
      ((Player*)player->data)->dam=14;
      ((Player*)player->data)->evading=75;
    break;
    case 11:
      PersonalAlien(player);
    break;
  }
  ((Player*)player->data)->doub=FALSE;
  ((Player*)player->data)->defense=FALSE;
  ((Player*)player->data)->ability=0;
  ((Player*)player->data)->win=0;
  ((Player*)player->data)->lost=0;
}

//initialize node of player
NodePlayer *InitNodePlayer(int num,int test){
  NodePlayer *newplayer=(NodePlayer*)malloc(sizeof(NodePlayer));
  newplayer->data=(Player*)malloc(sizeof(Player));
  newplayer->numplayer=num;
  ChooseCaracter(newplayer,test);
  newplayer->nxt=newplayer;
  newplayer->prv=newplayer;


  return newplayer;
}

//initialize list of players
PlayerList *InitplayerList(){
  PlayerList *gamelist=(PlayerList*) malloc(sizeof(PlayerList));
  gamelist->root=NULL;
  gamelist->nelem=0;

  return gamelist;
}

//add node of player to the list
NodePlayer *AddNode(NodePlayer *root,NodePlayer *newplayer){
  root->prv->nxt=newplayer;
  newplayer->prv=root->prv;
  newplayer->nxt=root;
  root->prv=newplayer;

  return newplayer;
}

//search a player by his num
NodePlayer *SearchNum(PlayerList *gamelist,int n){
  NodePlayer *tmp=gamelist->root;
  if(tmp->numplayer==n){
    return tmp;
  }
  tmp=tmp->nxt;
  while(tmp!=gamelist->root){
    if(tmp->numplayer==n){
      return tmp;
    }
    tmp=tmp->nxt;
  }
  
  return NULL;
}

//check nomber of player
int MisingNum(PlayerList *gamelist){
  int i=1;
  if (gamelist->nelem!=0){
    while(SearchNum(gamelist, i)!=NULL){
      i++;
      if(i>gamelist->nelem) break;
    }
    return i;
  }
  return 1;
}

//add player to the list
void Addplayer(PlayerList *gamelist,int test){
  NodePlayer *newplayer=InitNodePlayer(MisingNum(gamelist),test);
  //if list empty add new player
  if(gamelist->nelem==0){
    gamelist->root=newplayer;
  }
  else{
    gamelist->root=AddNode(gamelist->root, newplayer);
  }
  gamelist->nelem++;
}

//remove player for the list
void RemovePlayer(PlayerList *gamelist,int num,BestList *bestlist){
  //cheak if is only 2 players
  if(gamelist->nelem==2){
    printf("can't remove players\n");
    printf("Press Enter to continue\n");
    fflush(stdin);
    getchar();
    getchar();
    return;
  }
  
  NodePlayer *del;
  NodePlayer *tmp=gamelist->root;
  //remuve head
  if(tmp->numplayer==num){
    del=tmp;
    if(SearchBest(bestlist, del->numplayer)!=NULL) DelBest(bestlist, del->numplayer, TRUE);
    tmp->prv->nxt=tmp->nxt;
    tmp->nxt->prv=tmp->prv;
    gamelist->root=tmp->nxt;
  }
  //go through the list
  else{
    tmp=tmp->nxt;
    while(tmp!=gamelist->root){
      if(tmp->numplayer==num){
        del=tmp;
        //remove node
        if(SearchBest(bestlist, del->numplayer)!=NULL) DelBest(bestlist, del->numplayer, TRUE);
        tmp->prv->nxt=tmp->nxt;
        tmp->nxt->prv=tmp->prv;
      }
      tmp=tmp->nxt;
    }
  }
  //free memory
  gamelist->nelem--;
  del->nxt=NULL;
  del->prv=NULL;
  printf("%s\n",((Player*)del->data)->name);
  free((((Player*)del->data)->name));
  free(del->data);
  free(del);
}

//search a player by his name
NodePlayer *SearchName(PlayerList *gamelist,char *name){
  NodePlayer *tmp=gamelist->root;
  if(strcmp(((Player*)tmp->data)->name, name)==0){
    return tmp;
  }
  tmp=tmp->nxt;
  while(tmp!=gamelist->root){
    if(strcmp(((Player*)tmp->data)->name, name)==0){
      return tmp;
    }
    tmp=tmp->nxt;
  }
  
  return NULL;
}

//show list of players
void PrintPlayerList(PlayerList *gamelist){
  NodePlayer *tmp=gamelist->root;
  PrintPlayer(tmp);
  tmp=tmp->nxt;
  while(tmp!=gamelist->root){
    PrintPlayer(tmp);
    tmp=tmp->nxt;
  }
}


/*********************gameplay******************************/


//funtion to make damage
void damage(Player *playerof,Player *playerdef){
  int prob=rand()%100+1;

  //not evade
  if (prob>playerdef->evading){
    //double damage
    if (playerof->doub){
      //have defense
      if(playerdef->defense){
        playerdef->hp-=playerof->dam;
        //hp less than 0 make it 0
        if(playerdef->hp<0) playerdef->hp=0;
        printf("%s attacks, %s lost %d HP, his HP now is %.1f\n",playerof->name,playerdef->name,playerof->dam,playerdef->hp);
        playerdef->defense=FALSE;
      }
      //don't have defense
      else{
        playerdef->hp-=(playerof->dam)*2;
        //hp less than 0 make it 0
        if(playerdef->hp<0) playerdef->hp=0;
        printf("%s attacks, %s lost %d HP, his HP now is %.1f\n",playerof->name,playerdef->name,(playerof->dam)*2,playerdef->hp);
      }
      playerof->doub=FALSE;
    }
    //normal damage
    else{
      //have defense
      if(playerdef->defense){
        playerdef->hp-=(playerof->dam)/2.0;
        if(playerdef->hp<0) playerdef->hp=0;
        printf("%s attacks, %s lost %.1f HP, his HP now is %.1f\n",playerof->name,playerdef->name,(playerof->dam)/2.0,playerdef->hp);
        playerdef->defense=FALSE;
      }
      //don't have defense
      else{
        playerdef->hp-=playerof->dam;
        if(playerdef->hp<0) playerdef->hp=0;
        printf("%s attacks, %s lost %d HP, his HP now is %.1f\n",playerof->name,playerdef->name,playerof->dam,playerdef->hp);
      }
    }
  }

  //evade
  else{
     if (playerof->doub==1)playerof->doub=0;
     printf("%s evaded the attack\n",playerdef->name);
  }
  printf("Press Enter to continue\n");
  fflush(stdin);
  getchar();
  getchar();
}

//funtion to get defensive
void defend(Player *playerdef,Player *player){
  //activate defense
  //double damage
  if(playerdef->doub){
    //activate defense
    if(playerdef->defense==FALSE){
      playerdef->defense=TRUE;
      printf("%s got defense now, the next attack taking half the damage\n%s don't take advantage of the double damage\n",playerdef->name,playerdef->name);
    }
    //defense already activate
    else{
      printf("%s already has defense,so it has to attack\n",playerdef->name);
      damage(playerdef,player);
    }
    playerdef->doub=0;
  }
  //normal damage
  else {
    //activate defense
    if(playerdef->defense==FALSE){
      playerdef->defense=TRUE;
      printf("%s got defense now, the next attack taking half the damage\n",playerdef->name);
    }
    //defense already activate
    else{
      printf("%s already has defense,so it has to attack\n",playerdef->name);
      damage(playerdef,player);
    }
  }
  printf("Press Enter to continue\n");
  fflush(stdin);
  getchar();
  getchar();
}

//funtion to do special ability
void special(Player *playerof,Player *playerdef){
  int prob=rand()%100+1;

  //not evade
  if (prob>playerdef->evading){
    //doble damage
    if(playerof->doub){
      //have defense
      if(playerdef->defense){
        playerdef->hp-=playerof->ability*5;
        //hp less than 0 make it 0
        if(playerdef->hp<0) playerdef->hp=0;
        printf("%s uses his ability, %s lost %d HP, his HP now is %.1f\n",playerof->name,playerdef->name,playerof->ability*5,playerdef->hp);
        playerdef->defense=FALSE;
      }
      //don't have defense
      else{
        playerdef->hp-=playerof->ability*10;
        //hp less than 0 make it 0
        if(playerdef->hp<0) playerdef->hp=0;
        printf("%s uses his ability, %s lost %d HP, his HP now is %.1f\n",playerof->name,playerdef->name,playerof->ability*10,playerdef->hp);
      }
      playerdef->doub=0;
    }
    //normal damage
    else{
      //have defense
      if(playerdef->defense==TRUE){
        playerdef->hp-=playerof->ability*2.5;
        //hp less than 0 make it 0
        if(playerdef->hp<0) playerdef->hp=0;
        printf("%s uses his ability, %s lost %.1f HP, his HP now is %.1f\n",playerof->name,playerdef->name,(playerof->ability)*2.5,playerdef->hp);
        playerdef->defense=FALSE;
      }
      //don't have defense
      else{
        playerdef->hp-=playerof->ability*5;
        //hp less than 0 make it 0
        if(playerdef->hp<0) playerdef->hp=0;
        printf("%s uses his ability, %s lost %d HP, his HP now is %.1f\n",playerof->name,playerdef->name,playerof->ability*5,playerdef->hp);
      }
    }
  }

  //evade
  else{ 
    if (playerof->doub==1)playerof->doub=0;
    printf("%s evaded the attack\n",playerdef->name);
  }
  
  //make special ability to 0 rounds
    playerof->ability=0;
  printf("Press Enter to continue\n");
  fflush(stdin);
  getchar();
  getchar();
}

//double damage
void magic1(Playersgame *pgame){
  //activate double damage
  //p1
  ((Player*)pgame->player1->data)->doub=1;
  //p2
  ((Player*)pgame->player2->data)->doub=1;

  printf("\nThe field turns red and the aliens feel stronger, the next round will do double damage\n");

  sleep(4);
}

//healing
void magic2(Playersgame *pgame){
  printf("\nThe field turns golden and the aliens feel their health return, each one adds 15 HP\n");
  
  //healing p1
    //HP is max
  if (((Player*)pgame->player1->data)->hp+15 > ((Player*)pgame->player1->data)->hpmax-1){
    ((Player*)pgame->player1->data)->hp=((Player*)pgame->player1->data)->hpmax;
    printf("%s is full health, his HP is %.1f\n",((Player*)pgame->player1->data)->name,((Player*)pgame->player1->data)->hp);
  }
    //HP increase
  else {
    ((Player*)pgame->player1->data)->hp+=15;
    printf("%s has %.1f now\n",((Player*)pgame->player1->data)->name,((Player*)pgame->player1->data)->hp);
  }

  //healing p2
    //HP is max
  if (((Player*)pgame->player2->data)->hp+15>((Player*)pgame->player2->data)->hpmax-1){
    ((Player*)pgame->player2->data)->hp=((Player*)pgame->player2->data)->hpmax;
    printf("%s is full health, his HP is %.1f\n",((Player*)pgame->player2->data)->name,((Player*)pgame->player2->data)->hp);
  }
   //HP increase
  else {
    ((Player*)pgame->player2->data)->hp+=15;
    printf("%s has %.1f HP now\n",((Player*)pgame->player2->data)->name,((Player*)pgame->player2->data)->hp);
  }
  sleep(4);
}

//damage in the field
void magic3(Playersgame *pgame){
  printf("\nThe field darkens and the aliens feel their health decrease, each one lose 10 HP\n");
  
  //damage p1
  ((Player*)pgame->player1->data)->hp-=10;
  if(((Player*)pgame->player1->data)->hp<0) ((Player*)pgame->player1->data)->hp=0;
  printf("%s has %.1f HP now\n",((Player*)pgame->player1->data)->name,((Player*)pgame->player1->data)->hp);
  
  //damage p2
  ((Player*)pgame->player2->data)->hp-=10;
  if(((Player*)pgame->player2->data)->hp<0) ((Player*)pgame->player2->data)->hp=0;
  printf("%s has %.1f HP now\n",((Player*)pgame->player2->data)->name,((Player*)pgame->player2->data)->hp);
  
  printf("Press Enter to continue\n");
  sleep(4);
}

//swap turns
void magic4(Playersgame *pgame){
  NodePlayer *tmp=pgame->player1;
  pgame->player1=pgame->player2;
  pgame->player2=tmp;

  printf("\nThe turns swapped, give the keyboard to the other player\n");
  printf("Press Enter to continue\n");
  sleep(4);
}

//thread that follows health
void *threadhealth(void* arg){
  Playersgame *players= (Playersgame *) arg;
  
  vg=TRUE;
  //check HP
  while(vg){
    //check both 
    if (((Player*)players->player1->data)->hp==0 && ((Player*)players->player2->data)->hp==0) {
      vg=FALSE;
      printf("\nboth aliens die, no one won\n");
      sleep(2);
    }

    //check player 1
    else if (((Player*)players->player1->data)->hp==0 && ((Player*)players->player2->data)->hp!=0) {
      vg=FALSE;
      printf("\n%s win\n",((Player*)players->player2->data)->name);
      sleep(2);
      ((Player*)players->player2->data)->win++;
      ((Player*)players->player1->data)->lost++;
    }

    //check player 2
    else if (((Player*)players->player2->data)->hp==0 && ((Player*)players->player1->data)->hp!=0) {
      vg=FALSE;
      printf("%s win\n",((Player*)players->player1->data)->name);
      sleep(2);
      ((Player*)players->player1->data)->win++;
      ((Player*)players->player2->data)->lost++;
    }
  }

  pthread_exit(NULL);
  return NULL;
}

//threads for magics events
void *threadmagic(void* arg){
  strmagic *pmagics=(strmagic*)arg;
  int even,cont=0;
  
  while(vg){
    //wait one second
    sleep(1);
    cont++;

    //notify players of events
    if (cont%10==0 && cont!=30 && vg)printf("\nA event will hapend in %d seconds\n",30-cont);

    //run event
    if (cont==30 && vg){
      //random
      even=rand()%100+1;

      //doble damage
      if(even<=25) pmagics->mag1(pmagics->players);
      
      //healing
      else if(even>25 && even<=50) pmagics->mag2(pmagics->players);
      
      //field damage
      else if(even>50 && even<=75) pmagics->mag3(pmagics->players);
      
      //swap turns
      else pmagics->mag4(pmagics->players);

      cont=0;
    }
  }

  pthread_exit(NULL);
  return NULL;
}

//game
void Play(Playersgame *players){
   unsigned int order,action;
  //basic information
  printf("\n           Name            HP    damage    probability of evading\n");
  printf("Player1: %11s      %.1f   %d                %d\n",((Player*)players->player1->data)->name,((Player*)players->player1->data)->hp,((Player*)players->player1->data)->dam,((Player*)players->player1->data)->evading);
  printf("Player2: %11s      %.1f   %d                %d\n",((Player*)players->player2->data)->name,((Player*)players->player2->data)->hp,((Player*)players->player2->data)->dam,((Player*)players->player2->data)->evading);
  printf("Press Enter to continue\n");
  fflush(stdin);
  getchar();
  getchar();
  
  //vairable to see who start
  order=rand()%100;
  if (order+1>50) printf("%s is going to start\n",((Player*)players->player1->data)->name);
  else printf("%s is going to start\n",((Player*)players->player2->data)->name);
  printf("Press Enter to continue\n");
  fflush(stdin);
  getchar();

  //information of each action
  printf("\nAttack: make an attack of the damage of your alien\nDefend: the next hit you take will do half the damage\nSpecial ability: each round that passes increases the damage by 5 points\n");
  printf("Press Enter to continue\n");
  fflush(stdin);
  getchar();

  vg=TRUE;

  //thread of health
  pthread_t healt;
  pthread_create(&healt,NULL,threadhealth,(void *) players);

  //struct of magic events
  strmagic events={magic1,magic2,magic3,magic4,players};
  
  //thread of magic events
  pthread_t magic;
  pthread_create(&magic,NULL,threadmagic,(void *)&events);
  
  //main while
  while(vg){
    //actions player 1
    if (order+1>=50){
      //not available to use special ability
      if(((Player*)players->player1->data)->ability>=4)printf("\n1.attack\n2.defend\n3.special ability\n%s is going to:\n",((Player*)players->player1->data)->name);

      //available to use special ability
      else  printf("\n1.attack\n2.defend\n%s is going to:\n",((Player*)players->player1->data)->name);
      scanf("%d",&action);
      fflush(stdin);

      //verify data
      if(((Player*)players->player1->data)->ability>=4){
        while(action<1 || action>3){
          printf("invalid data, enter again:");
          scanf("%d",&action);
          fflush(stdin);
        }
      }
      else{
        while(action<1 || action>2){
          printf("invalid data, enter again:");
          scanf("%d",&action);
          fflush(stdin);
        }
      }
      printf("\n");

      //actions
      if (action==1) damage(players->player1->data, players->player2->data);
      else if (action==2) defend(players->player1->data, players->player2->data);
      else special(players->player1->data, players->player2->data);
      
      //add round to ability
      ((Player*)players->player1->data)->ability++;
      
      //change player
      order-=50;
    }
    //actions player 2
    else{
      //not available to use special ability
      if(((Player*)players->player2->data)->ability>=4)printf("\n1.attack\n2.defend\n3.special ability\n%s is going to:\n",((Player*)players->player2->data)->name);

      //available to use special ability
      else  printf("\n1.attack\n2.defend\n%s is going to:\n",((Player*)players->player2->data)->name);
      scanf("%d",&action);
      fflush(stdin);

      //verify data
      if(((Player*)players->player2->data)->ability>=4){
        while(action<1 || action>3){
          printf("invalid data, enter again:");
          scanf("%d",&action);
          fflush(stdin);
        }
      }
      else{
        while(action<1 || action>2){
          printf("invalid data, enter again:");
          scanf("%d",&action);
          fflush(stdin);
        }
      }
      printf("\n");

      //actions
      if (action==1) damage(players->player2->data, players->player1->data);
      else if (action==2) defend(players->player2->data, players->player1->data);
      else special(players->player2->data, players->player1->data);
      
      //add round to ability
      ((Player*)players->player2->data)->ability++;
      
      //change player
      order+=50;
    }
  }
  //close threads
  pthread_join(healt,NULL);
  pthread_join(magic,NULL);
}
