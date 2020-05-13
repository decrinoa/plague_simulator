#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <cassert>

enum class State : char { Empty, Susceptible, Infected, Recovered};

class Board {
  std::vector<State> board_;
  int n_;
  
  int countInfectedNeighbours(int i);
  void change(int i);
  
public:
  inline Board(int n) : board_(n*n), n_{n}, displayCreated_{false} { assert(n > 0); };

}

#endif
