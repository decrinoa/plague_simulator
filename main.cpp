#include <iostream>
#include "board.h"

int main() {
  Board griglia(240);
  Disease tetano{0.5,0.001,3};

  griglia.placePeople(1000);
  griglia.placePeople(1,State::Infected)

  //ANIMATION
  
  griglia.draw(5);

}
