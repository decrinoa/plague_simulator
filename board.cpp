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

//PUBLIC

void Board::placePeople(int numberOfPeople, State const&state) {
  //Actually there's no need for assert >= 0: putting a value < 0 does not break the program
  assert(numberOfPeople >= 0);
  
  //the number of people to put in the grid must be less (or equal) to the number of empty cells
  assert(numberOfPeople <= count(State::Empty));

  std::random_device gen;
  
  //It will contain the position of all empty cells
  std::vector<int> emptyCells;
  for (int i = 0, end = board_.size(); i < end; ++i) {
    if (board_[i] == State::Empty)
      emptyCells.push_back(i);
  }

  shuffle (emptyCells.begin(), emptyCells.end(), gen);
  for (int i = 0; i < numberOfPeople; ++i) {
    board_[emptyCells[i]] = state;
  }
}
