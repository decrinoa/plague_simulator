#include <iostream>
#include <chrono>
#include <thread>
#include "board.h"

bool frameByFrame = true;

int main() {
  Board griglia(200);
  Disease tetano{0.8, 0.01, 5, 0.5};

  griglia.placePeople(699);
  griglia.placePeople(10,State::Infected);

  griglia.animate(tetano, 1, true);
  
  griglia.save("data.dat");
  
}
