#include <iostream>
#include <chrono>
#include <thread>
#include "board.h"

int main() {
  Board griglia(200);
  Disease tetano{0.5, 0.01, 3, 0.5};

  griglia.placePeople(699);
  griglia.placePeople(10,State::Infected);

  int frame = 0; 
    while (griglia.count(State::Infected) > 0 || griglia.peopleInQuarantine() > 0) {
      std::cout << "S: " << griglia.count(State::Susceptible) << " - I: " << griglia.count(State::Infected) << 
        " - R: " << griglia.count(State::Recovered) << " - Q: " << griglia.peopleInQuarantine() << "\n\n";
    
      //griglia.print();
      griglia.draw();

      std::cout << "Frame " << frame << '\n';
      
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(10ms);
      //std::cout << "Press RETURN to continue\n";
      //std::cin.ignore();
      system("clear");

      griglia.evolve(tetano);
      griglia.move();

      ++frame;
    }
  //griglia.print();
  griglia.draw();

  std::cout << "Press RETURN to close this window\n";
  std::cin.ignore();
  
  griglia.save("data.dat");
  std::cout << "Data saved.\n";
}
