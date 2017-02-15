// Source code for HeartsField, by Joris Teunisse
#include "heartsfield.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <cstring>

// Int to string conversion
#define ItoS(x) static_cast< std::ostringstream & >( \
        (std::ostringstream() << std::dec << x)).str()

HeartsField::HeartsField(){
  amtOfCards = 52;
  gameNr = 0;
  gameWon = false;
  mc = false;
  for(int i = 0; i < AMTOFPLAYERS; i++)
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
  for(int i = 0; i < AMTOFPLAYERS; i++)
    bots[i].callHand(i);
  std::cout << std::endl;

  int passAmt = 3;
  int passedCards[AMTOFPLAYERS * passAmt];
  for(int i = 0; i < AMTOFPLAYERS * passAmt; i++)
    passedCards[i] = bots[i/3].passCard();
  for(int i = 0; i < AMTOFPLAYERS; i++){
    for(int j = 0; j < passAmt; j++)
      bots[(i+gameNr)%4].receiveCard(passedCards[i*passAmt+j]);
  }

  std::cout << "Hands after passing:" << std::endl;
  for(int i = 0; i < AMTOFPLAYERS; i++)
    bots[i].callHand(i);
  std::cout << std::endl;
}

// Evaluate the cards on the table after a trick.
void HeartsField::evaluateTrick(bool output){
  int highest = 0;
  int nextTurn = 0;
  int points = 0;
  if(output)
    std::cout << "Cards on table:" << std::endl;
  for(int i = 0; i < AMTOFPLAYERS; i++){
    if(output)
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
  if(output)
    std::cout << "Turn switches to bot " << turn << "." << std::endl << std::endl;
}

// Evaluates the points of the bots, and determines who is in the lead.
// A bot wins if it has the lowest score by the time 100 points are obtained
// by a player.
bool HeartsField::evaluatePoints(bool output){
  int winner = 0;
  int loser = 0;
  for(int i = 0; i < AMTOFPLAYERS; i++){
    if(bots[i].shotTheMoon()){
      bots[i].addPoints(-26);
      for(int j = 1; j < AMTOFPLAYERS; i++)
        bots[(i+j)%AMTOFPLAYERS].addPoints(26);
    }

    if(output)
      std::cout << "Bot " << i << " has " << bots[i].getPoints() <<
    ((bots[i].getPoints() == 1) ? " point." : " points.") << std::endl;
    if(bots[i].getPoints() < bots[winner].getPoints())
      winner = i;
    if(bots[i].getPoints() > bots[loser].getPoints())
      loser = i;
    bots[i].backupPoints();
  }
  if(bots[loser].getPoints() < 100){
    if(output)
      std::cout << "Bot " << winner << " is in the lead!" << std::endl;
  }
  else{
    if(output)
      std::cout << "Bot " << winner << " has won!" << std::endl;
    gameWon = true;
  }
  return winner;
}

int HeartsField::randomPlayout(int botNr, int skipNr){
  int points = bots[botNr].getPoints();
  // Play turns until one (and thus, all) player has emptied the hand.
  while(!bots[0].handEmpty()){
    for(int i = turn; i < turn + AMTOFPLAYERS; i++){
      if(skipNr > 0){
        skipNr--;
        continue;
      }
      // The actual number of the player.
      int j = i % AMTOFPLAYERS;
      if(j == turn){
        int moves = bots[j].validMoves(j, '$', heartsBroken, firstTrick);
        cardsOnTable[j] = bots[j].playRandomCard(moves);
        suit = determineSuit(cardsOnTable[j]);
      }
      else{
        int moves = bots[j].validMoves(j, suit, heartsBroken, firstTrick);
        cardsOnTable[j] = bots[j].playRandomCard(moves);
      }
      if(intToCard(cardsOnTable[j]).find('h') != std::string::npos
      && heartsBroken == false){
        heartsBroken = true;
      }
    }
    evaluateTrick(false);
  }
  evaluatePoints(false);
  return (bots[botNr].getPoints() - points);
}

int HeartsField::playMCCard(int skipNr, int botNr, int moves){
  double leastPoints = 1000.0;
  int bestMove = 0;
  for(int i = 0; i < moves; i++){
    // std::cout << "----MOVE NUMBER " << i << std::endl;
    double points = 0.0;
    HeartsField copy = *this;
    copy.cardsOnTable[botNr] = copy.bots[botNr].playCard(i);
    for(int j = 0; j < 20; j++){
      // std::cout << "--SIMULATION " << j << std::endl;
      HeartsField temp = copy;
      points += temp.randomPlayout(botNr, skipNr);
    }
    points /= 20.0;
    if(points < leastPoints){
      std::cout << "Move " << i << " got less average points: "
      << points << "!" << std::endl;
      bestMove = i;
      leastPoints = points;
    }
  }
  return bots[botNr].playCard(bestMove);
}

// Play a game of Hearts.
void HeartsField::playGame(){
  // Pass cards between the players.
  passCards();

  // Determine what bot has the 2 of clubs, which flags the starting player.
  for(int i = 0; i < AMTOFPLAYERS; i++){
    if(bots[i].handContains(2)){
      std::cout << "Bot " << i << " starts." << std::endl;
      turn = i;
      bots[i].setStarting();
    }
  }

  // Play turns until one (and thus, all) player has emptied the hand.
  while(!bots[0].handEmpty()){
    for(int i = turn; i < turn + AMTOFPLAYERS; i++){
      // The actual number of the player.
      int j = i % AMTOFPLAYERS;
      if(j == turn){
        int moves = bots[j].validMoves(j, '$', heartsBroken, firstTrick);
        if(j == 0 && mc)
          cardsOnTable[j] = playMCCard(i, j, moves);
        else
          cardsOnTable[j] = bots[j].playRandomCard(moves);
        suit = determineSuit(cardsOnTable[j]);
      }
      else{
        int moves = bots[j].validMoves(j, suit, heartsBroken, firstTrick);
        if(j == 0 && mc)
          cardsOnTable[j] = playMCCard(i, j, moves);
        else
          cardsOnTable[j] = bots[j].playRandomCard(moves);
      }
      if(intToCard(cardsOnTable[j]).find('h') != std::string::npos
      && heartsBroken == false){
        std::cout << "Hearts has been broken!" << std::endl;
        heartsBroken = true;
      }
    }
    evaluateTrick(true);
  }
  evaluatePoints(true);
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
  int handSize = amtOfCards / AMTOFPLAYERS;
  for(int i = 0; i < amtOfCards; i++)
    bots[i / handSize].addToHand(deck[i]);

  // Hearts have not yet been broken; has to be reset for consecutive games.
  // Also, it is the first trick, and the next game.
  heartsBroken = false;
  firstTrick = true;
  gameNr++;
}

// Sets up a game of Hearts.
void HeartsField::setup(char *parms[]){
  srand(time(NULL));
  int i = 0;
  if(parms[1] != NULL && !strcmp(parms[1], "-mc"))
    mc = true;
  while(!gameWon){
    std::cout << "Game " << i << " start." << std::endl;
    deal();
    playGame();
    if(!gameWon)
      std::cout << "End of game " << i << "." << std::endl << std::endl;
    i++;
  }
}
