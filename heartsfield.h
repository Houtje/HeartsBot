// Header for HeartsField, by Joris Teunisse
#include "heartsbot.h"

class HeartsField{
  public:
    HeartsField();
    ~HeartsField();
    bool evaluateGame(bool output);
    bool playGame(bool mc);
    char determineSuit(int card);
    int getWinner();
    int playMCCard(int botNr, int moves, bool clairvoyant);
    int randomPlayout(int botNr);
    std::string toCard(int i);
    void deal();
    void dealUnknown(int *deck, int cardsInDeck, int player, int *toDeal);
    void evaluateTrick(bool output);
    void passCards();
    void shuffleDeck(int *deck, int size);
  private:
    HeartsBot bots[AMTOFPLAYERS];
    char suit;
    int cardsOnTable[AMTOFPLAYERS];
    int gameNr;
    int first;
    int winner;
    bool firstTrick;
    bool heartsBroken;
};
