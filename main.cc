// Source code for the main function of the Hearts program, by Joris Teunisse
#include "heartsfield.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

int main(int argc, char *argv[]){
  srand(time(NULL));
  HeartsField *field = new HeartsField();
  int i = 0;
  std::cout << "Game " << i << " start." << std::endl;
  while(!field->playGame(argv[1] != NULL && !strcmp(argv[1], "-mc"))){
    std::cout << "End of game " << i << "." << std::endl << std::endl;
    std::cout << "Game " << ++i << " start." << std::endl;
  }
  return 0;
}
