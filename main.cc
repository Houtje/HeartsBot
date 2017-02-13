// Source code for the main function of the Hearts program, by Joris Teunisse
#include "heartsfield.h"

int main(int argc, char *argv[]){
  HeartsField *field = new HeartsField();
  field->setup(argv);
  return 0;
}
