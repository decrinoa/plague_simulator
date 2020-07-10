#include "board.h"
#include <cassert>
#include <iostream>
#include <random>
#include <algorithm>  //for shuffle
#include <cmath>
#include <fstream>
#include <chrono>
#include <thread>

//Funzioni non usate
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
}*/

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
}*/

void Board::change(int i) {  
  switch(board_[i]) {
    case State::Empty :
      board_[i] = State::Susceptible;
      break;
    case State::Susceptible :
      board_[i] = State::Infected;
      break;
    case State::Infected :
      board_[i] = State::Recovered;
      break;
    default: 
      std::cout << "Error in change(int i) funcion: board_[" << i << "] is Recovered or an unknown State\n";
  }
}

void Board::placePeople(int numberOfPeople, State const&state, bool returnFromQuarantine) {
  //Actually there's no need for assert >= 0: putting a value < 0 does not break the program
  assert(numberOfPeople >= 0);
  
  //the number of people to put in the grid must be less (or equal) to the number of empty cells
  assert(numberOfPeople <= count(State::Empty));

  //Se un utente cambia il numero di persone nella griglia dopo averla visualizzata almeno una volta, cambierà anche la 
  //grandezza del quadrato della quarantena che dipende appunto da countPeople().
  //Non cambia però la grandezza della window, che deve quindi essere distrutta e ricreata quando viene richiamato draw.
  //Per capire se placePeople è stato chiamato da evolve (quando una persona guarita ritorna dalla quarantena) o se invece
  //è stato chiamato dall'utente, si è resa pubblica (quindi disponibile all'utente) solo la funzione senza bool.
  //Evolve invece chiamerà la funzione privata con il booleano = true.
  if (!returnFromQuarantine && displayCreated_) {
    window_.close();
    displayCreated_ = false;
  }
  
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

//  -PUBLIC-
State& Board::state(unsigned int i) {
  assert(i <= board_.size());
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

void Board::placePeople(int numberOfPeople, State const& state) {
  //La placePeople pubblica (quindi disponibile all'utente) non fa altro che chiamare quella privata passando il 
  //parametro returnFromQuarantine = false. Si veda il commento della placePeople privata per un chiarimento
  placePeople(numberOfPeople, state, false);
}

void Board::evolve(Disease const& disease, bool quarantine) {
  assert(disease.beta > 0 && disease.beta <= 1);
  assert(disease.gamma  >= 0 && disease.gamma <= 1 );
  assert(disease.radius >= 1);
  assert(disease.manifestation >= 0 && disease.manifestation <= 1);
  
  std::random_device gen;   
  std::uniform_real_distribution<double> dist(0.0,1.0);

  std::vector<int> toChange;
  //qui bisogna mettere il legame con la funzione quarantine che è da definire//  //DP: Che?
   
  if (quarantine) { //RETURN FROM QUARANTINE
    int recoveredFromQuarantine = 0;
    for (int i = 0; i < peopleInQuarantine_; ++i) {
      if (dist(gen) < disease.gamma) { //cioè con probabilità = gamma
        placePeople(1, State::Recovered, true);  //place 1 person on the grid
        ++recoveredFromQuarantine;
      }
    }
    peopleInQuarantine_ -= recoveredFromQuarantine;
  }
  
  for (int i = 0, end = board_.size(); i < end; ++i) {
    if (board_[i] == State::Susceptible) {  //INFECTION OF SUSCEPTIBLE
      
      for (int j = 0, endJ = board_.size(); j < endJ; ++j) {
        if (board_[j] == State::Infected) {
          
          if(distance(i,j) <= disease.radius) {
            if (dist(gen) < disease.beta) { 
              toChange.push_back(i);
              break;    //break è importante. Altrimenti lo i può venire aggiunto più volte a toChange e diventerà subito recovered
            }
          }

          // Se la colonna dell'infetto è maggiore del suscettibile che sto controllando (i) (cioè se l'infetto è più a destra del 
          // suscettibile che sto controllando), e sono tra loro troppo lontani (else = distance > radius), saranno troppo lontani 
          // rispetto al suscettibile che sto controllando anche tutti i prossimi infetti su tutta la linea.
          // Aumento quindi j del numero di persone rimanenti nella riga (n - colonna di j) e passo così alla riga dopo
          // Attenzione: è importante che la condizione nel for sia j < endJ e non j != endJ. Infatti in questo modo j può 
          // diventare anche maggiore di endJ            
          else if (j%n_ > i%n_)
            j += n_ - (j%n_);
        }  
      }
    }
    
    if (board_[i] == State::Infected) {   //RECOVERY OF INFECTED
      if (dist(gen) < disease.gamma) //cioè con probabilità = gamma
        toChange.push_back(i);
      if (quarantine && dist(gen) < disease.manifestation) {  //QUARANTINE OF INFECTED
        board_[i] = State::Empty;
        ++peopleInQuarantine_;
      }
    }
  }

  for (auto const& v : toChange) 
    change(v);

  //UPDATE HISTORY
  history_.push_back({count(State::Susceptible), count(State::Infected), count(State::Recovered), peopleInQuarantine_});  //legame con history  //DP: Che?
}

void Board::move() {
  
  //equal probability of going to the left (cell i-1), up (i-n), right (i+1), down (i+n) or stay
  std::random_device gen;
  std::uniform_int_distribution<int> dist(0, 5); //5 possibility

  //It's better to start once from begin and once from end, otherwise there'll be a tendency to go to the top right corner
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

      switch (direction) {
        case 0: //left
          //check if left is empty only if i it's not in the first column
          ///WARNING: Do not swap these checkings. If you access board_[i-1] before cheking if i % n_ != 0, 
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

void Board::draw(int cellSize, int offset, int frame, std::string const& windowTitle) {
  assert((cellSize > 0 || cellSize == -1) && offset >= 0);

  //se non viene passato il parametro cellSize, la dimensione delle celle è quella massima che fa entrare la griglia nello schermo
  if (cellSize == -1)
    cellSize = (sf::VideoMode::getDesktopMode().height - 60) / (n_ + 2*offset); //tolti 60 pixel per la barra delle applicazioni
  
  int sqrtPeople = sqrt(countPeople());
  unsigned int windowWidth = (n_ + 2*sqrtPeople + 4*offset)*cellSize;
  unsigned int windowHeight = (n_ + 2*offset)*cellSize;
  
  if (!displayCreated_) {
    //create display    
    window_.create(sf::VideoMode( windowWidth, windowHeight), windowTitle, sf::Style::Close);
    displayCreated_ = true;
  }

  //update display
  window_.clear(sf::Color::Black);

  sf::RectangleShape border(sf::Vector2f(n_*cellSize, n_*cellSize));
  border.setFillColor(sf::Color::Transparent);
  border.setOutlineColor(sf::Color::White);
  border.setOutlineThickness(cellSize/2);
  border.setPosition(offset*cellSize, offset*cellSize);
  window_.draw(border);

  sf::CircleShape circle(cellSize/2); //radius = cellSize/2
  circle.setOutlineThickness(0);
  circle.setOutlineColor(sf::Color::White);

  for (int i = 0, end = board_.size(); i < end; ++i) {
    if (board_[i] == State::Susceptible) {
      circle.setFillColor(sf::Color::White);
      circle.setPosition((i % n_ +offset) * cellSize, (i / n_ +offset) * cellSize);   //col = (i%n), row = (i/n)
      window_.draw(circle);
    }

    if (board_[i] == State::Infected) {
      circle.setFillColor(sf::Color::Red);
      circle.setPosition((i % n_ +offset) * cellSize, (i / n_ +offset) * cellSize);   //col = (i%n), row = (i/n)
      window_.draw(circle);
    }

    if (board_[i] == State::Recovered) {
      circle.setFillColor(sf::Color::Blue);
      circle.setPosition((i % n_ +offset) * cellSize, (i / n_ +offset) * cellSize);   //col = (i%n), row = (i/n)
      window_.draw(circle);
    }
  }

  //draw quarantine
  border.setSize(sf::Vector2f(2*sqrtPeople*cellSize, 2*sqrtPeople*cellSize));
  border.setPosition((3*offset + n_)*cellSize, offset*cellSize);
  window_.draw(border);
  
  circle.setFillColor(sf::Color::Red);
  circle.setPosition((3*offset + n_)*cellSize + cellSize/2, offset*cellSize + cellSize/2);
  for (int i = 0; i < peopleInQuarantine_; ++i) {    
    window_.draw(circle);         //disegna il cerchio
    if ((i+1) % sqrtPeople == 0)
      circle.move((sqrtPeople-1)*-2*cellSize, 2*cellSize);   //muove il cerchio indietro (all'inizio) e sotto di 2 celle
    else
      circle.move(2*cellSize, 0);   //e lo muove di 2 celle
  }
  
  //draw text
  sf::Font font;
  if (!font.loadFromFile("stencil.ttf")) //se il caricamento fallisce 
    font.loadFromFile("roman.ttf");
  
  int fontSize = sqrtPeople*cellSize/4;
  
  sf::Text text("Quarantine", font, fontSize);
  text.setPosition((3*offset + n_)*cellSize, (offset + 2*sqrtPeople)*cellSize);
  window_.draw(text);

  //text.setFillColor(sf::Color::White);
  text.setString("S: " + std::to_string(count(State::Susceptible)));
  text.move(0,4*offset*cellSize + fontSize);
  window_.draw(text);

  text.setFillColor(sf::Color::Red);
  text.setString("I: " + std::to_string(count(State::Infected)));
  text.move(0,offset*cellSize + fontSize);
  window_.draw(text);

  text.setFillColor(sf::Color::Blue);
  text.setString("R: " + std::to_string(count(State::Recovered)));
  text.move(0,offset*cellSize + fontSize);
  window_.draw(text);

  text.setFillColor(sf::Color::Red);
  text.setString("Q: " + std::to_string(peopleInQuarantine_));
  text.move(0,offset*cellSize + fontSize);
  window_.draw(text);

  if (frame != -1) {
    //low right corner
    text.setFillColor(sf::Color::White);
    //text.setCharacterSize(fontSize/2);
    text.setPosition(text.getPosition().x, windowHeight - offset*cellSize - fontSize);
    text.setString("Frame: " + std::to_string(frame));
    window_.draw(text);
  }

  window_.display();
}

void Board::animate(Disease const& disease, int infectedBeforeQuarantine, bool frameByFrame, bool move, int cellSize, int offset, std::string const& windowTitle) {
  //assert solo di animate
  assert(infectedBeforeQuarantine >= 0);
  
  // Non so quanto ci sia bisogno di questi asserts, visto che ci sono già dopo. Credo sia un controllo superfluo che rallenta un po'
  /*
  //asserts di draw()
  assert((cellSize > 0 || cellSize == -1) && offset >= 0);

  //asserts di evolve
  assert(disease.beta > 0 && disease.beta <= 1);
  assert(disease.gamma  >= 0 && disease.gamma <= 1 );
  assert(disease.radius >= 1);
  assert(disease.manifestation >= 0 && disease.manifestation <= 1);
  */
  
  int frame = 0;

  while (count(State::Infected) > 0 /*|| peopleInQuarantine() > 0 */ ) {
    std::cout << "S: " << count(State::Susceptible) << " - I: " << count(State::Infected) << 
                " - R: " << count(State::Recovered) << " - Q: " << peopleInQuarantine_ << "\n\n";
    
    draw(cellSize, offset, frame, windowTitle);

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

    bool quarantine = false;
    if (infectedBeforeQuarantine != 0)
      quarantine = count(State::Infected) > infectedBeforeQuarantine;

    evolve(disease, quarantine);
    if (move) {
      //la variabile ha lo stesso nome della funzione. Per specificare che uso la funzione devo anche scrivere Board::
      Board::move();  
    }

    ++frame;
  }

  draw(cellSize, offset, frame, windowTitle);

  std::cout << "Simulation ended.\n";
  std::cout << "Press RETURN to close the window\n";
  std::cin.ignore();

  window_.close();
  displayCreated_ = false;  
}

void Board::save(std::string const& fileName) {
  std::ofstream out(fileName);
  auto end = history_.end();
  auto begin = history_.begin();
  for (auto it = history_.begin(); it != end; ++it) {
    out << it - begin << '\t' << it->susceptible << '\t' << it->infected << '\t' << it->recovered << '\t' << it->quarantined << '\n';
  }
  out.close();
  
  std::cout << "Data saved.\n";
}
