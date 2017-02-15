// Header for HeartsBot, by Joris Teunisse
#include <string>

#define HANDSIZE 13

class HeartsBot{
  public:
    HeartsBot();
    ~HeartsBot();
    void setStarting();
    void addToHand(int card);
    void callHand(int i);
    void receiveCard(int card);
    void addPoints(int amount);
    void backupPoints();
    bool handContains(int card);
    bool handEmpty();
    bool justHearts();
    bool checkSuit(int card, char suit);
    bool shotTheMoon();
    int passCard();
    int getPoints();
    int playRandomCard(int amtOfMoves);
    int playCard(int cardNr);
    int validMoves(int botNr, char suit, bool heartsBroken, bool firstTrick);
    std::string intToCard(int i);
  private:
    int hand[HANDSIZE];
    int valid[HANDSIZE];
    int oldPoints;
    int points;
    bool starting;
};
