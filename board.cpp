#include "board.h"
#include <cassert>
#include <iostream>
#include <random>
#include <algorithm>  //for shuffle
#include <cmath>
#include <fstream>


/*int Board::countInfectedNeighbours(int i) {
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
*/
/*int Board::countNearerThanRadius(int position, double radius) {
  assert(board_[position] == State::Susceptible);

  int count = 0;

  for (int i = 0, end = board_.size(); i < end; ++i) {
    if (board_[i] == State::Infected) {
      //col = (i%n), row = (i/n)
      double distance = sqrt((i%n_ - position%n_)*(i%n_ - position%n_) + (i/n_ - position/n_)*(i/n_ - position/n_));
      if (distance <= radius) ++count;
    }
  }

  return count;
}
*/
void Board::change(int i) { 
  //change State of cell from E->S, S->I, I->R
  
  switch(board_[i]) {
    case State::Empty :
      board_[i] == State::Susceptible;
      break;
    case State::Susceptible :
      board_[i] == State::Infected;
      break;
    case State::Infected :
      board_[i] == State::Recovered;
      break;
    default: 
      std::cout << "Error in change(int i) funcion: board_[" << i << "] is Recovered or an unknown State\n";
  }
}

//PUBLIC
State& Board::state(int i) {
  assert(i >= 0 && i <= board_.size());
  return board_[i];
}

void Board::setInfected(int row, int col) {
  assert (row < n_ && col < n_);
  board_[row * n_ + col] = State::Infected;
}

int Board::count(State const& state) {
  int count = 0;
  for (auto const& v : board_) {
    if (v == state)
      ++count;
  }
  return count;
}

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

//------------------------------------------------------------------------
void Board::evolve(Disease &disease, bool quarantine) {
  assert(disease.beta > 0 && disease.beta < 1);
  assert(disease.gamma  >= 0 && disease.gamma <= 1 );
  assert(disease.radius >= 1);
  assert(disease.manifestation >= 0 && disease.manifestation <= 1);
  
  std::random_device gen;   
  std::uniform_real_distribution<double> dist(0.0,1.0);

  std::vector<int> toChange;
  //qui bisogna mettere il legame con la funzione quarantine che è da definire//
   
  if (quarantine) { //RETURN FROM QUARANTINE
    int recoveredFromQuarantine = 0;
    for (int i = 0; i < peopleInQuarantine_; ++i) {
      if (dist(gen) < disease.gamma) {      
        placePeople(1, State::Recovered);  //place 1 person on the grid
        ++recoveredFromQuarantine;
      }
    }
    peopleInQuarantine_ -= recoveredFromQuarantine;
  }
  
  for (int i = 0, end = board_.size(); i < end; ++i) {
    if (board_[i] == State::Susceptible) {
      
      for (int j = 0, endJ = board_.size(); j < endJ; ++j) {
        if (board_[j] == State::Infected) {
          
          if(distance(i,j) <= disease.radius) {
            if (dist(gen) < disease.beta) { 
              toChange.push_back(i);
              break;    
            }
          }
          else{    //if (distance > radius)
            // Se la colonna dell'infetto è maggiore del suscettibile che sto controllando (i), e sono tra loro   
            // troppo lontani, saranno troppo lontani anche tutti i prossimi infettti su tutta la linea. 
            // Aumento quindi j di quelli rimanenti nella riga, passo cioè alla riga dopo
            
            //questa non l'ho capita :/ me la dovete spiegare (MF)
            if (j%n_ > i%n_)
              j += n_ - (j%n_);
          }
        }  
      }
    }
        if (board_[i] == State::Infected) {   //RECOVER OF INFECTED
      if (dist(gen) < disease.gamma) //cioè con probabilità = gamma
        toChange.push_back(i);
      if (quarantine && dist(gen) < disease.manifestation) {  //questo aggiunge alla quarantena ma la quarantena la dobbiamo sistemare (dico a te MG)
        board_[i] = State::Empty;
        ++peopleInQuarantine_;
      }
    }
  }

  for (auto const& v : toChange) 
    change(v);

  //UPDATE HISTORY
  history_.push_back({count(State::Susceptible), count(State::Infected), count(State::Recovered), peopleInQuarantine_});  //legame con history
}
//----------------------------------------------------

void Board::move() {
  
  //equal probability of going to the left (cell i-1), up (i-n), right (i+1), down (i+n) or stay
  std::random_device gen;
  std::uniform_int_distribution<int> dist(0, 5); //5 possibility

  //It's better to start once from begin and once from end, otherwise there'll be a tendency to go to the top right corner
  //Try to comment these lines and run a 40x40 board with 300 people if you don't believe this
  int begin = 0;
  int end = board_.size();
  int increment = 1;
  if (gen() % 2) { //gen() is a (big) random int number
    begin = end;
    end = 0;
    increment = -1;
  }

  for (int i = begin; i != end; i += increment) {
    if (board_[i] == State::Susceptible || board_[i] == State::Infected || board_[i] == State::Recovered) {
         
      int direction = dist(gen);

      //These checkings is similar to countInfectedNeighbours (so I'll put no comments)
      switch (direction) {
        case 0: //left
          //Warning: Do not swap these checkings. If you access board_[i-1] before cheking if i % n_ != 0, 
          //board[i-1] could not exist -> Undefined behavior
          if (i % n_ != 0 && board_[i-1] == State::Empty) {  
            board_[i-1] = board_[i];  //the new cell take the old state
            board_[i] = State::Empty; //the old cell is now empty
          }
          //it bounces on the border of the grid or onto another person (if the opposite cell is empty):
          else if(i % n_ != n_ -1 && board_[i+1] == State::Empty) {
            board_[i+1] = board_[i];
            board_[i] = State::Empty;
          }
          break;
        case 1: //up
          if (i / n_!= 0 && board_[i-n_] == State::Empty) {
            board_[i-n_] = board_[i];
            board_[i] = State::Empty;
          }
          else if(i / n_ != n_-1 && board_[i+n_] == State::Empty) {
            board_[i+n_] = board_[i];
            board_[i] = State::Empty;
          }
          break;
        case 2: //right
          if (i % n_ != n_- 1 && board_[i+1] == State::Empty) {
            board_[i+1] = board_[i];
            board_[i] = State::Empty;
          }
          else if(i % n_ != 0 && board_[i-1] == State::Empty) {
            board_[i-1] = board_[i];
            board_[i] = State::Empty;
          }
          break;
        case 3: //down
          if (i / n_!= n_- 1 && board_[i+n_] == State::Empty) {
            board_[i+n_] = board_[i];
            board_[i] = State::Empty;
          }
          else if(i / n_ != 0 && board_[i-n_] == State::Empty) {
            board_[i-n_] = board_[i];
            board_[i] = State::Empty;
          }
          //break; //Not necessary 'cause it's the last case: it'll break anyway
        //case 4: //stay
          //do nothing          
      }
    }
  }
}


void Board::draw(int cellSize, std::string windowTitle) {
  if (displayCreated_ == false) {
    //create display
    window_.create(sf::VideoMode(n_*cellSize, n_*cellSize), windowTitle, sf::Style::Close);
    displayCreated_ = true;
  }

  //update display
  window_.clear(sf::Color::Black);

  sf::CircleShape circle(cellSize/2); //radius = cellSize/2
  circle.setOutlineThickness(0);
  circle.setOutlineColor(sf::Color::White);

  for (int i = 0, end = board_.size(); i < end; ++i) {
    if (board_[i] == State::Susceptible) {
      circle.setFillColor(sf::Color::White);
      circle.setPosition((i % n_) * cellSize, (i / n_) * cellSize);   //col = (i%n), row = (i/n)
      window_.draw(circle);
    }

    if (board_[i] == State::Infected) {
      circle.setFillColor(sf::Color::Red);
      circle.setPosition((i % n_) * cellSize, (i / n_) * cellSize);   //col = (i%n), row = (i/n)
      window_.draw(circle);
    }

    if (board_[i] == State::Recovered) {
      circle.setFillColor(sf::Color::Blue);
      circle.setPosition((i % n_) * cellSize, (i / n_) * cellSize);   //col = (i%n), row = (i/n)
      window_.draw(circle);
    }
  }
  
  window_.display();
}

//stampa su doc dati per grafici 
void Board::save(std::string fileName) {
  std::ofstream out(fileName);
  auto end = history_.end();
  auto begin = history_.begin();
  for (auto it = history_.begin(); it != end; ++it) {
    out << it - begin << '\t' << it->susceptible << '\t' << it->infected << '\t' << it->recovered << '\t' << it->quarantined << '\n';
  }
  out.close();
}

void Board::print() {
  for (int i = 0, end = board_.size(); i < end; ++i) {
    if (i != 0 && i % n_ == 0) std::cout << '\n';
    
    switch(board_[i]) {
      case State::Empty :
        std::cout << ' ';
        break;
      case State::Susceptible :
        std::cout << 'O';
        break;
      case State::Infected :
        std::cout << 'X';
        break;
      case State::Recovered :
        std::cout << '.';
        break;
      default:
        std::cout << "Strange error in print() funcion: board_[" << i << "] is an unknown State\n";
    }
    std::cout << ' ';
  }
  std::cout << "\n\n";
}
