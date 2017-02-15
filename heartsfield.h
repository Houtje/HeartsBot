// Header for HeartsField, by Joris Teunisse
#include "heartsbot.h"

#define AMTOFPLAYERS 4

class HeartsField{
  public:
    HeartsField();
    ~HeartsField();
    int playMCCard(int skipNr, int botNr, int moves);
    int randomPlayout(int botNr, int skipNr);
    void setup(char *parms[]);
    void deal();
    void playGame();
    void passCards();
    void evaluateTrick(bool output);
    char determineSuit(int card);
    bool evaluatePoints(bool output);
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
    bool mc;
};
