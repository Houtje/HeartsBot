// Header for HeartsBot, by Joris Teunisse
#include <string>

#define HANDSIZE 13

class HeartsBot{
  public:
    HeartsBot();
    ~HeartsBot();
    bool checkSuit(int card, char suit);
    bool handContains(int card);
    bool handEmpty();
    bool justHearts();
    bool shotTheMoon();
    int passCard();
    int getPoints();
    int playRandomCard(int amtOfMoves);
    int playCard(int cardNr);
    int validMoves(char suit, bool heartsBroken, bool firstTrick);
    std::string getValidCard(int i);
    std::string toCard(int i);
    void addPoints(int amount);
    void addToHand(int card);
    void backupPoints();
    void callHand(int i);
    void receiveCard(int card);
    void setStarting();
  private:
    bool starting;
    int hand[HANDSIZE];
    int oldPoints;
    int points;
    int valid[HANDSIZE];
};
