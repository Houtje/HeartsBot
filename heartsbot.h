// Header for HeartsBot, by Joris Teunisse
#include <string>

#define AMTOFPLAYERS 4
#define AMTOFCARDS 52
#define HANDSIZE (AMTOFCARDS / AMTOFPLAYERS)

class HeartsBot{
  public:
    HeartsBot();
    ~HeartsBot();
    bool checkSuit(int card, char suit);
    bool handContains(int card);
    bool handEmpty();
    bool handFull();
    bool knowsCard(int card);
    bool justHearts();
    bool shotTheMoon();
    int passCard(int cardNr);
    int passRandomCard();
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
    void resetKnown();
    void setStarting();
  private:
    bool starting;
    int hand[HANDSIZE];
    int knownCards[AMTOFCARDS];
    int amtKnown;
    int oldPoints;
    int points;
    int valid[HANDSIZE];
};
