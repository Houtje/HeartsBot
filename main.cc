// Source code for the main function of the Hearts program, by Joris Teunisse
#include "heartsfield.h"
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]){
  srand(time(NULL));
  std::ofstream out;
  out.open("stats.txt");
  int rounds = 1;
  int args = 1;
  if(argv[args] != NULL && atoi(argv[args]) != 0){
    rounds = atoi(argv[args]);
    args++;
  }
  for(int i = 0; i < rounds; i++){
    HeartsField *field = new HeartsField();
    int j = 0;
    std::cout << "Game " << j << " start." << std::endl;
    while(!field->playGame(argv[args] != NULL && !strcmp(argv[args], "-mc"))){
      std::cout << "End of game " << j << "." << std::endl << std::endl;
      std::cout << "Game " << ++j << " start." << std::endl;
    }
    out << field->getWinner() << '\n';
  }
  out.close();
  return 0;
}
