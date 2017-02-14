// Header for HeartsField, by Joris Teunisse
#include "heartsbot.h"

#define AMTOFPLAYERS 4

class HeartsField{
  public:
    HeartsField();
    ~HeartsField();
    int playMCCard(int botNr);
    void setup(char *parms[]);
    void deal();
    void playGame();
    void passCards();
    void evaluateTrick();
    void evaluatePoints();
    char determineSuit(int card);
    std::string intToCard(int i);
  private:
    HeartsBot bots[AMTOFPLAYERS];
    int cardsOnTable[AMTOFPLAYERS];
    int amtOfCards;
    int amtOfPlayers;
    int turn;
    int gameNr;
    char suit;
    bool heartsBroken;
    bool firstTrick;
    bool gameWon;
};
