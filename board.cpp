#include board.h
#include <cassert>
#include <iostream>

int Board::countInfectedNeighbours(int i) {
  assert(board_[i] == State::Susceptible);
  
  int count = 0;
  
  //check only if i it's not at the end of board (right side)
  if (i % n_ != 0) {
    if (board_[i - 1] == State::Infected) ++count;
    //check only if i it's not at the first row
    if (i / n_ != 0 && board_[i - n_ - 1] == State::Infected) ++count;
    //check only if i it's not at the last row
    if (i / n_ != n_ - 1 && board_[i - n_ + 1] == State::Infected) ++count;
    }
  //check only if i it's not at the begin of board (left side)
  if (i % n_ != n_ - 1) {
    if (board_[i + 1] == State::Infected) ++count;
    //check only if i it's not at the first row
    if (i / n_ != 0 && board_[i - n_ + 1] == State::Infected) ++count;
    //check only if i it's not at the last row
    if (i / n_ != n_ - 1 && board_[i + n_ + 1] == State::Infected) ++count; 
  }
  //check only if i it's not at the first row
  if (i / n_ != 0 && board_[i - n_] == State::Infected) ++count;
  //check only if i it's not at the last row
  if (i / n_ != n_ - 1 && board_[i + n_] == State::Infected) ++ count;
  
  return count;
}

void Board::change(int i) { 
  //change State of cell from E->S, S->I, I->R
  
  switch(board_[i]) {
    case State::Empty ;
      board_[i] == State::Susceptible;
      break;
    case State::Susceptible;
      board_[i] == State::Infected;
      break;
    case State::Infected;
      board_[i] == State::Recovered;
      break;
    default: 
      std::cout << "Error in change(int i) funcion: board_[" << i << "] is Recovered or an unknown State\n";
  }
}
