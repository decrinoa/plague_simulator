#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <cassert>
#include <SFML/Graphics.hpp>
#include <cmath> //per sqrt

struct Disease {
  double const& beta;   //probability of infection being nearer than radius to an infected person
  double const& gamma;  //probability of recovery
  double const& radius = 1.41421356237; //sqrt(2)
  double const& manifestation = 0.5;
};

enum class State{ Empty, Susceptible, Infected, Recovered}; //In un futuro aggiungere anche Dead

class Board {
  struct Situation {
    int susceptible;
    int infected;
    int recovered;
    int quarantined;
  };
  
  std::vector<State> board_;
  int n_;
  sf::RenderWindow window_;
  bool displayCreated_;
  std::vector<Situation> history_;
  int peopleInQuarantine_;
  
  //int countInfectedNeighbours(int i);
  //int countNearerThanRadius(int position, double radius);

  //cambia lo state della cella i: E->S S->I, I->R
  void change(int i);
  inline double distance(int i, int j) {     //get distance between two cells
    return sqrt((i%n_ - j%n_)*(i%n_ - j%n_) + (i/n_ - j/n_)*(i/n_ - j/n_)); }
  
public:
  inline Board(int n) : board_(n*n), n_{n}, displayCreated_{false} { assert(n > 0); };
  
  State& state(int i);  //get board[i]
  void setInfected(int row, int col);  //place an infected person on the board
  inline int peopleInQuarantine() { return peopleInQuarantine_;};
  
  int count(State const& state);
  inline int countPeople() { return n_*n_ - count(State::Empty); };
  void placePeople(int numberOfPeople, State const&state = State::Susceptible); //places randomly nPeople on the board
  void evolve(Disease &disease, bool quarantine = true);
  void move();
  void print();
  //draw board with SFML. Offset è da intendere come espresso numero di celle, non in pixel
  void draw(int cellSize = -1, int offset = 2, std::string windowTitle = "Plague simulator");
  void save(std::string fileName = "output.dat"); //stampa su doc dati per grafici 
};

#endif  //BOARD_H

