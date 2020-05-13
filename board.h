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

}

#endif
