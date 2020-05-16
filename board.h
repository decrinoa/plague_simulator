#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <cassert>

struct Disease {
  double const& beta;   //probability of infection being next to an infected person
  double const& gamma;  //probability of recovery
};

enum class State : char { Empty, Susceptible, Infected, Recovered}; //In un futuro aggiungere anche Dead

class Board {
  std::vector<State> board_;
  int n_;
  
  int countInfectedNeighbours(int i);
  void change(int i);
  
public:
  inline Board(int n) : board_(n*n), n_{n}, displayCreated_{false} { assert(n > 0); };
  
  void placePeople(int numberOfPeople, State const&state = State::Susceptible); //places randomly nPeople on the board

}

#endif
