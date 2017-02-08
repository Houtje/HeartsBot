// Header for HeartsField
// By Joris Teunisse (Last edit: 2-2-2017)
#include "heartsbot.h"

class HeartsField{
  public:
    HeartsField();
    ~HeartsField();
    void setup();
    void deal();
    void playGame();
    void passCards();
    void evaluateTrick();
    void evaluatePoints();
    char determineSuit(int card);
    std::string intToCard(int i);
  private:
    HeartsBot bots[4];
    int cardsOnTable[4];
    int amtOfCards;
    int amtOfPlayers;
    int turn;
    char suit;
    bool heartsBroken;
    bool firstTrick;
};
