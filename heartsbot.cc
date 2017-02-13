// Source code for HeartsBot, by Joris Teunisse
#include "heartsbot.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <sstream>

// Int to string conversion
#define ItoS(x) static_cast< std::ostringstream & >( \
        (std::ostringstream() << std::dec << x)).str()

HeartsBot::HeartsBot(){
  oldPoints = 0;
  points = 0;
  starting = false;
  for(int i = 0; i < HANDSIZE; i++)
    hand[i] = 0;
}

HeartsBot::~HeartsBot(){

}

void HeartsBot::setStarting(){
  starting = true;
}

void HeartsBot::addPoints(int amount){
  points += amount;
}

void HeartsBot::backupPoints(){
  oldPoints = points;
}

int HeartsBot::getPoints(){
  return points;
}

// Converts an integer number to a name representing a card.
std::string HeartsBot::intToCard(int i){
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

// Receive a card and add it to the hand.
void HeartsBot::receiveCard(int card){
  for(int j = 0; j < HANDSIZE; j++){
    if(hand[j] == 0){
      hand[j] = card;
      break;
    }
  }
}

// Passes a random card to the field.
int HeartsBot::passCard(){
  int passedCard = 0;
  int randomCard = rand() % HANDSIZE;
  while(hand[randomCard] == 0)
    randomCard = rand() % HANDSIZE;
  passedCard = hand[randomCard];
  hand[randomCard] = 0;
  return passedCard;
}

// Check whether the hand is empty.
bool HeartsBot::handEmpty(){
  for(int i = 0; i < HANDSIZE; i++){
    if(hand[i] != 0)
      return false;
  }
  return true;
}

// Checks whether the hand only contains hearts.
bool HeartsBot::justHearts(){
  for(int i = 0; i < HANDSIZE; i++){
    if(!(checkSuit(hand[i], 'h') || hand[i] == 0))
      return false;
  }
  return true;
}

// Checks whether the hand contains a card.
bool HeartsBot::handContains(int card){
  for(int i = 0; i < HANDSIZE; i++){
    if(hand[i] == card)
      return true;
  }
  return false;
}

// Checks whether the given card belongs to a given suit.
bool HeartsBot::checkSuit(int card, char suit){
  return (intToCard(card).find(suit) != std::string::npos);
}

// Checks whether the bot has 'shot the moon': this means the bot
// has obtained all the penalty cards.
bool HeartsBot::shotTheMoon(){
  return ((oldPoints - points) == 26) ? true : false;
}

// Plays a random card, and removes it from the hand.
// The card played is from the given suit if able.
// Hearts can be played either when hearts have been broken,
// or when not broken but the player does not have the suit.
int HeartsBot::playCard(char suit, bool heartsBroken, bool firstTrick){
  if(starting){
    // Play the opening card.
    for(int i = 0; i < HANDSIZE; i++){
      if(hand[i] == 2){
        hand[i] = 0;
        starting = false;
        return 2;
      }
    }
    std::cout << "ERROR: Wrong starting player!" << std::endl;
  }
  else{
    // Play a card (from the given suit, if able).
    int card = 0;
    int toPlay = 0;
    if(suit != '$'){
      // Is always the first card of the suit: however, since the cards
      // are placed into the hand in a random order this should be no problem.
      for(int i = 0; i < HANDSIZE; i++){
        if(checkSuit(hand[i], suit)){
          toPlay = i;
          card = hand[i];
        }
      }
    }
    while(card == 0){
      toPlay = rand() % HANDSIZE;
      // The card can be played if:
      // -It is not Hearts or the Queen of Spades on the first trick
      // -It is not the first card in the trick
      // -It is not Hearts while they have not been broken and the bot has
      // cards with other suits left
      if((firstTrick && (!checkSuit(hand[toPlay], 'h') && !hand[toPlay] == 52))
      || suit != '$'
      || !(checkSuit(hand[toPlay], 'h') && heartsBroken == false && !justHearts()))
        card = hand[toPlay];
    }
    hand[toPlay] = 0;
    return card;
  }
}

// Adds a card to the hand of the bot.
void HeartsBot::addToHand(int card){
  for(int i = 0; i < HANDSIZE; i++){
    if(hand[i] == 0){
      hand[i] = card;
      return;
    }
  }
  std::cout << "ERROR: Card not added to hand!" << std::endl;
}

// Calls the current hand, with added integer parameter to differentiate
// between different bots.
void HeartsBot::callHand(int i){
  std::cout << "Hand of bot " << i << ":" << std::endl;
  for(int j = 0; j < HANDSIZE; j++)
    std::cout << intToCard(hand[j]) << " ";
  std::cout << std::endl;
}
