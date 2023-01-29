/* Constants */
#define TRUE 1
#define FALSE 0
#define MAX 11

//struct of player
typedef struct player{
  char *name;// hacer puntero
  float hp;
  int hpmax, dam, evading, defense, doub, ability, win, lost;
}Player;

//strcut of node of player
typedef struct NodePlayer{
  void *data;
  int numplayer;
  struct NodePlayer *nxt;
  struct NodePlayer *prv;
}NodePlayer;

//struct of players list 
typedef struct PlayerList{
  NodePlayer *root;
  unsigned int nelem;
}PlayerList;

//struct of node of best player
typedef struct NodeBest{
  void *data;
  struct NodeBest *nxt;
  struct NodeBest *prv;
}NodeBest;

//strcut of best list 
typedef struct BestList{
  NodeBest *root;
  unsigned int nelem;
}BestList;

//strcut of play 
typedef struct Playersgame{
  NodePlayer *player1;
  NodePlayer *player2;
}Playersgame;

//pointer to magics funtions 
typedef void (*magics)(Playersgame *pgame);

//struct for magics events
typedef struct magic{
  magics mag1;
  magics mag2;
  magics mag3;
  magics mag4;
  Playersgame *players;
}strmagic;


//funtions
void PrintNode(BestList *blist);

/*********************PlayerList******************************/
void PrintPlayer(NodePlayer *player);
PlayerList *InitplayerList();
void Addplayer(PlayerList *gamelist,int test);
void RemovePlayer(PlayerList *gamelist,int num,BestList *bestlist);
NodePlayer *SearchNum(PlayerList *gamelist,int n);
NodePlayer *SearchName(PlayerList *gamelist,char *name);
void PrintPlayerList(PlayerList *clist);

/*********************BestList******************************/
BestList *InitBestList();
void ShowBest(BestList *bestlist);
void AddBest(NodePlayer *player1, NodePlayer *player2, BestList *bestlist);
void PrintBestList(BestList *bestlist);

/*********************gameplay******************************/
void Play(Playersgame *players);

