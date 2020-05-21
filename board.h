#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <cassert>
#include <SFML/Graphics.hpp>

struct Disease {
  double const& beta;   //probability of infection being nearer than radius to an infected person
  double const& gamma;  //probability of recovery
  double const& radius = 1.41421356237; //sqrt(2)
};

enum class State : char { Empty, Susceptible, Infected, Recovered}; //In un futuro aggiungere anche Dead

class Board {
  std::vector<State> board_;
  int n_;
  bool displayCreated_;
  
  int countInfectedNeighbours(int i);
  int countNearerThanRadius(int position, double radius);
  void change(int i);
  
public:
  inline Board(int n) : board_(n*n), n_{n}, displayCreated_{false} { assert(n > 0); };
  
  void placePeople(int numberOfPeople, State const&state = State::Susceptible); //places randomly nPeople on the board
  
  void draw(int cellSize = 10, std::string windowTitle = "Plague simulator"); //draw board with SFML
}

#endif  //BOARD_H
