// Source code for HeartsField, by Joris Teunisse
#include "heartsfield.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <climits>

// Int to string conversion
#define ItoS(x) static_cast< std::ostringstream & >( \
        (std::ostringstream() << std::dec << x)).str()

HeartsField::HeartsField(){
  amtOfCards = 52;
  amtOfPlayers = 4;
  for(int i = 0; i < 4; i++)
    cardsOnTable[i] = 0;
}

HeartsField::~HeartsField(){

}

// Converts an integer number to a name representing a card.
std::string HeartsField::intToCard(int i){
  if(i == 0)
    return "null";
  else{
    std::string card;
    if(i < 15){
      card += "c";
      card += ItoS(i);
    }
    else if(i < 28){
      card += "d";
      card += ItoS(i - 13);
    }
    else if(i < 41){
      card += "h";
      card += ItoS(i - 26);
    }
    else{
      card += "s";
      card += ItoS(i - 39);
    }
    return card;
  }
}

// Determines the suit of a given card.
char HeartsField::determineSuit(int card){
  if(card < 15)
    return 'c';
  else if(card < 28)
    return 'd';
  else if(card < 41)
    return 'h';
  else
    return 's';
}

// Lets all bots pass 3 cards to the next player.
void HeartsField::passCards(){
  for(int i = 0; i < amtOfPlayers; i++)
    bots[i].callHand(i);
  std::cout << std::endl;

  int passedCards[amtOfPlayers * 3];
  for(int i = 0; i < amtOfPlayers * 3; i++)
    passedCards[i] = bots[i/3].passCard();
  for(int i = 0; i < amtOfPlayers; i++){
    for(int j = 0; j < 3; j++)
      bots[(i+1)%4].receiveCard(passedCards[i*3+j]);
  }

  std::cout << "Hands after passing:" << std::endl;
  for(int i = 0; i < amtOfPlayers; i++)
    bots[i].callHand(i);
  std::cout << std::endl;
}

// Evaluate the cards on the table after a trick.
void HeartsField::evaluateTrick(){
  int highest = 0;
  int nextTurn = 0;
  int points = 0;
  std::cout << "Cards on table:" << std::endl;
  for(int i = 0; i < 4; i++){
    std::cout << intToCard(cardsOnTable[i]) << std::endl;
    if(intToCard(cardsOnTable[i]).find(suit) != std::string::npos
    && cardsOnTable[i] > highest){
      highest = cardsOnTable[i];
      nextTurn = i;
    }
    if(intToCard(cardsOnTable[i]).find('h') != std::string::npos)
      points++;
    else if(intToCard(cardsOnTable[i]).find("s12") != std::string::npos)
      points += 13;
  }
  bots[nextTurn].addPoints(points);
  turn = nextTurn;
  if(firstTrick)
    firstTrick = false;
  std::cout << "Turn switches to bot " << turn << "." << std::endl << std::endl;
}

// Evaluates the points of the bots, and determines who is in the lead.
void HeartsField::evaluatePoints(){
  int lowest = INT_MAX;
  int winner = 0;
  for(int i = 0; i < amtOfPlayers; i++){
    std::cout << "Bot " << i << " has " << bots[i].getPoints() <<
    ((bots[i].getPoints() == 1) ? " point." : " points.") << std::endl;
    if(bots[i].getPoints() < lowest){
      lowest = bots[i].getPoints();
      winner = i;
    }
  }
  std::cout << "Bot " << winner << " is in the lead!" << std::endl;
}

// Play a game of Hearts.
void HeartsField::playGame(){
  // Pass cards between the players.
  passCards();

  // Determine what bot has the 2 of clubs, which flags the starting player.
  for(int i = 0; i < amtOfPlayers; i++){
    if(bots[i].handContains(2)){
      std::cout << "Bot " << i << " starts." << std::endl;
      turn = i;
      bots[i].setStarting();
    }
  }

  // Play turns until one (and thus, all) player has emptied the hand.
  while(!bots[0].handEmpty()){
    for(int i = turn; i < turn + amtOfPlayers; i++){
      if(i == turn){
        cardsOnTable[i] = bots[i].playCard('$', heartsBroken, firstTrick);
        suit = determineSuit(cardsOnTable[i]);
      }
      else
        cardsOnTable[i % 4] = bots[i % 4].playCard(suit, heartsBroken, firstTrick);
      if(intToCard(cardsOnTable[i % 4]).find('h') != std::string::npos
      && heartsBroken == false){
        std::cout << "Hearts has been broken!" << std::endl;
        heartsBroken = true;
      }
    }
    evaluateTrick();
  }
  evaluatePoints();
}

// Deal the deck to the players.
void HeartsField::deal(){
  // Initialize deck of cards
  int deck[amtOfCards];
  for(int i = 0; i < amtOfCards; i++)
    deck[i] = i + 2;

  // Shuffle deck
  for(int i = 0; i < 100; i++){
    int r = rand() % amtOfCards;
    int temp = deck[r];
    deck[r] = deck[i % amtOfCards];
    deck[i % amtOfCards] = temp;
  }

  // Give every bot a now-random hand
  int handSize = amtOfCards / amtOfPlayers;
  for(int i = 0; i < amtOfCards; i++)
    bots[i / handSize].addToHand(deck[i]);

  // Hearts have not yet been broken; has to be reset for consecutive games.
  heartsBroken = false;
  firstTrick = true;
}

// Sets up a game of Hearts.
void HeartsField::setup(){
  srand(time(NULL));
  for(int i = 0; i < 3; i++){
    std::cout << "Game " << i << " start." << std::endl;
    deal();
    playGame();
    std::cout << "End of game " << i << "." << std::endl << std::endl;
  }
}
