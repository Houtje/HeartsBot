// Source code for HeartsField, by Joris Teunisse
#include "heartsfield.h"
#include <cstdlib>
#include <iostream>
#include <sstream>

// Int to string conversion
#define toString(i) static_cast< std::ostringstream & >( \
        (std::ostringstream() << std::dec << i)).str()

// Constructor
HeartsField::HeartsField(){
  gameNr = 0;
  winner = 0;
  for(int i = 0; i < AMTOFPLAYERS; i++)
    cardsOnTable[i] = 0;
}

// Destructor
HeartsField::~HeartsField(){

}

int HeartsField::getWinner(){
  return winner;
}

// Converts an integer number to a name representing a card.
std::string HeartsField::toCard(int i){
  if(i == 0)
    return "null";
  else{
    std::string card;
    if(i < 15){
      card += "c";
      card += toString(i);
    }
    else if(i < 28){
      card += "d";
      card += toString(i - 13);
    }
    else if(i < 41){
      card += "h";
      card += toString(i - 26);
    }
    else{
      card += "s";
      card += toString(i - 39);
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

// Lets all bots pass 3 cards to the next player. All bots call their hand
// before and after the passing.
void HeartsField::passCards(){
  for(int i = 0; i < AMTOFPLAYERS; i++)
    bots[i].callHand(i);
  std::cout << std::endl;

  int passAmt = 3;
  int passedCards[AMTOFPLAYERS * passAmt];
  for(int i = 0; i < AMTOFPLAYERS * passAmt; i++)
    passedCards[i] = bots[i/passAmt].passRandomCard();
  for(int i = 0; i < AMTOFPLAYERS; i++){
    for(int j = 0; j < passAmt; j++)
      bots[(i+gameNr)%4].addToHand(passedCards[i*passAmt+j]);
  }

  std::cout << "Hands after passing:" << std::endl;
  for(int i = 0; i < AMTOFPLAYERS; i++)
    bots[i].callHand(i);
  std::cout << std::endl;
}

// Evaluate the cards on the table after a trick, adding points to the bot
// with the highest card of the suit.
void HeartsField::evaluateTrick(bool output){
  int highest = 0;
  int next = 0;
  int points = 0;
  if(output)
    std::cout << "Cards on table:" << std::endl;
  for(int i = 0; i < AMTOFPLAYERS; i++){
    if(output)
      std::cout << toCard(cardsOnTable[i]) << std::endl;
    if(determineSuit(cardsOnTable[i]) == suit && cardsOnTable[i] > highest){
      highest = cardsOnTable[i];
      next = i;
    }
    if(determineSuit(cardsOnTable[i]) == 'h')
      points++;
    else if(cardsOnTable[i] == 51)
      points += 13;
  }
  bots[next].addPoints(points);
  first = next;
  if(firstTrick)
    firstTrick = false;
  for(int i = 0; i < AMTOFPLAYERS; i++)
    cardsOnTable[i] = 0;
  if(output)
    std::cout << "Turn switches to bot " << first << "." << std::endl << std::endl;
}

// Evaluates the points of the bots, and determines who is in the lead.
// A bot wins if it has the lowest score by the time 100 points are obtained
// by a player. Returns whether the game has ended.
bool HeartsField::evaluateGame(bool output){
  int loser = 0;
  for(int i = 0; i < AMTOFPLAYERS; i++){
    if(bots[i].shotTheMoon()){
      bots[i].addPoints(-26);
      for(int j = 1; j < AMTOFPLAYERS; i++)
        bots[(i+j)%AMTOFPLAYERS].addPoints(26);
      break;
    }
  }
  for(int i = 0; i < AMTOFPLAYERS; i++){
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
    return false;
  }
  if(output)
    std::cout << "Bot " << winner << " has won!" << std::endl;
  return true;
}

// Does a random playout for the current game situation, and returns how many
// points bot botNr has gained.
int HeartsField::randomPlayout(int botNr){
  int points = bots[botNr].getPoints();
  int j;
  while(!bots[0].handEmpty()){
    for(int i = first; i < first + AMTOFPLAYERS; i++){
      j = i % AMTOFPLAYERS;
      if(cardsOnTable[j] != 0)
        continue;
      int moves = bots[j].validMoves((j == first ? '$' : suit), heartsBroken, firstTrick);
      cardsOnTable[j] = bots[j].playRandomCard(moves);
      if(j == first)
        suit = determineSuit(cardsOnTable[j]);
      if(determineSuit(cardsOnTable[j]) == 'h' && heartsBroken == false)
        heartsBroken = true;
    }
    evaluateTrick(false);
  }
  evaluateGame(false);
  return (bots[botNr].getPoints() - points);
}

// Plays a card for bot botNr from a number of valid moves, using the Monte
// Carlo strategy to determine the best card.
int HeartsField::playMCCard(int botNr, int moves, bool clairvoyant){
  std::cout << "Playing a MC card with " << moves << " moves." << std::endl;
  int bestMove = 0;
  double leastPoints = 1000.0;
  double points;
  HeartsField copy, temp;
  for(int i = 0; i < moves; i++){
    points = 0.0;
    copy = *this;
    copy.cardsOnTable[botNr] = copy.bots[botNr].playCard(i);
    for(int j = 0; j < 100; j++){
      temp = copy;
      /* TODO: RANDOMDISTRIBUTE
       * -voor alle kaarten in handen vd bots
       * -get de kaart en kijk of ie known is
       * -zo ja geef m terug, zo nee zet m in een deck
       * -shuffle t deck (kostersmethode) en geef het terug met addtohand
       *
       * TODO 2: die known functie maken
       * -elke opgegooide kaart vd anderen hieraan toevoegen
       * -ook de 3 kaarten die je passt.
       * -advanced: ook een unknownfunctie die meldt welke suits een bot
       *  niet heeft, hiermee wordt rekening gehouden door de functie hierboven.
       */
      if(!clairvoyant){
        int toShuffle[AMTOFCARDS] = {0};
        int toDeal[AMTOFPLAYERS] = {0};
        int cardsInDeck = 0;
        for(int k = botNr+1; k < botNr + AMTOFPLAYERS; k++){
          int cardNr = 0;
          while(true){
            int card = temp.bots[k % AMTOFPLAYERS].passCard(cardNr);
            if(card == -1)
              break;
            else if(card == 0){
              cardNr++;
              continue;
            }
            if(!temp.bots[botNr].knowsCard(card)){
              toShuffle[cardsInDeck] = card;
              toDeal[k]++;
              cardsInDeck++;
            }
            else
              temp.bots[k % AMTOFPLAYERS].addToHand(card);
            cardNr++;
          }
        }
        /*for(int kat = 0; kat < AMTOFCARDS; kat++)
          std::cout << toCard(toShuffle[kat]) << " ";
        std::cout << "#:" << cardsInDeck << std::endl;*/
        temp.dealUnknown(toShuffle, cardsInDeck, botNr, toDeal);
      }
      points += temp.randomPlayout(botNr);
    }
    points /= 100.0;
    if(points < leastPoints){
      std::cout << "Move " << i << " (" << bots[botNr].getValidCard(i) <<
      ") averaged less points: " << points << "!" << std::endl;
      bestMove = i;
      leastPoints = points;
    }
  }
  return bots[botNr].playCard(bestMove);
}

// Play a game of Hearts, either with or without a Monte Carlo player.
bool HeartsField::playGame(bool mc){
  // Several variable changes to indicate the next game.
  heartsBroken = false;
  firstTrick = true;
  gameNr++;

  deal();

  passCards();

  // Flag the starting player.
  for(int i = 0; i < AMTOFPLAYERS; i++){
    if(bots[i].handContains(2)){
      std::cout << "Bot " << i << " starts." << std::endl;
      first = i;
      bots[i].setStarting();
    }
  }

  // Play turns until one (and thus, all) player has emptied the hand.
  while(!bots[0].handEmpty()){
    for(int i = first; i < first + AMTOFPLAYERS; i++){
      int j = i % AMTOFPLAYERS;
      int moves = bots[j].validMoves((j == first ? '$' : suit), heartsBroken, firstTrick);
      if(j == 0 && mc)
        cardsOnTable[j] = playMCCard(j, moves, false);
      else
        cardsOnTable[j] = bots[j].playRandomCard(moves);
      if(j == first)
        suit = determineSuit(cardsOnTable[j]);
      if(determineSuit(cardsOnTable[j]) == 'h' && heartsBroken == false){
        std::cout << "Hearts has been broken!" << std::endl;
        heartsBroken = true;
      }
    }
    evaluateTrick(true);
  }
  return evaluateGame(true);
}

// Deal the deck to the players.
void HeartsField::deal(){
  int deck[AMTOFCARDS];
  for(int i = 0; i < AMTOFCARDS; i++)
    deck[i] = i + 2;
  for(int i = 0; i < 100; i++){
    int r = rand() % AMTOFCARDS;
    int temp = deck[r];
    deck[r] = deck[i % AMTOFCARDS];
    deck[i % AMTOFCARDS] = temp;
  }
  for(int i = 0; i < AMTOFCARDS; i++)
    bots[i / (AMTOFCARDS / AMTOFPLAYERS)].addToHand(deck[i]);
}

void HeartsField::dealUnknown(int *deck, int cardsInDeck, int player, int *toDeal){
  for(int i = 0; i < 100; i++){
    int r = rand() % cardsInDeck;
    int temp = deck[r];
    deck[r] = deck[i % cardsInDeck];
    deck[i % cardsInDeck] = temp;
  }

  int receiver = (player+1) % AMTOFPLAYERS;
  for(int i = 0; i < cardsInDeck; i++){
    while(toDeal[receiver] == 0){
      if(receiver == player)
        std::cout << "ERROR: Not enough cards can be dealt back!" << std::endl;
      else{
        receiver++;
        receiver %= AMTOFPLAYERS;
      }
    }
    bots[receiver].addToHand(deck[i]);
    toDeal[receiver]--;
  }

  std::cout << "After shuffling:" << std::endl;
  for(int i = 0; i < AMTOFPLAYERS; i++)
    bots[i].callHand(i);
  std::cout << std::endl;
}
