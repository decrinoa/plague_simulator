#include <iostream>
#include <chrono>
#include <thread>
#include "board.h"

int main() {
  Board griglia(200);
  Disease tetano{0.8, 0.01, 5, 0.5};

  griglia.placePeople(699);
  griglia.placePeople(1,State::Infected);

  //ESEMPIO DI UTILIZZO DELLA CLASSE SENZA LA FUNZIONE Board::animate()
  /*
  bool frameByFrame = false;

  int frame = 0; 
  while (griglia.count(State::Infected) > 0) {
    std::cout << "S: " << griglia.count(State::Susceptible) << " - I: " << griglia.count(State::Infected) << 
      " - R: " << griglia.count(State::Recovered) << " - Q: " << griglia.peopleInQuarantine() << "\n\n";
    griglia.draw();
    std::cout << "Frame " << frame << '\n';
    
    if (frameByFrame) {
      std::cout << "Press RETURN to continue\n";
      std::cin.ignore();
    }
    else {
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(10ms);
    }
    system("clear");

    griglia.evolve(tetano);
    griglia.move();
    ++frame;
  }

  griglia.draw();
  std::cout << "END OF SIMULATION: Press return to close the window \n";
  std::cin.ignore();    */

  //FINE ESEMPIO. E' del tutto equivalente a chiamare la funzione animate
  
  griglia.animate(tetano,0);
  
  griglia.save("data.dat");
  
}
