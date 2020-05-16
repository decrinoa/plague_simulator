#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <cassert>

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
