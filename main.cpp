#include <iostream>
#include "board.h"

int main() {
  Board griglia(240);
  Disease tetano{0.5,0.001,3, 0.2};

  griglia.placePeople(1000);
  griglia.placePeople(1,State::Infected);

  //ANIMATION
  
  griglia.draw(5);

  std::cout << "Press RETURN to close this window\n";
  std::cin.ignore();
  
  griglia.save("data.txt");
  std::cout << "Data saved.\n";
}
