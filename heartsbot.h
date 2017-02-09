// Header for HeartsBot, by Joris Teunisse
#include <string>

class HeartsBot{
  public:
    HeartsBot();
    ~HeartsBot();
    void setStarting();
    void addToHand(int card);
    void callHand(int i);
    void receiveCard(int card);
    void addPoints(int amount);
    bool handContains(int card);
    bool handEmpty();
    bool justHearts();
    bool checkSuit(int card, char suit);
    int passCard();
    int getPoints();
    int playCard(char suit, bool heartsBroken, bool firstTrick);
    std::string intToCard(int i);
  private:
    int hand[13];
    int handSize;
    int points;
    bool starting;
};
