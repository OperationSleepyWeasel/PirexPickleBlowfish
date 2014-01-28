#ifndef UNITTOKEN_HPP
#define UNITTOKEN_HPP

#include "BoardToken.hpp"

class UnitToken : virtual public BoardToken
{
public:
  UnitToken(Army army, std::string name, Attributes* attributes = NULL);
  ~UnitToken();

  //getters
  Attributes* getEdgeAttributes(Side edge);

  //TODO: make private
  void setEdgeAttributes(Side edge, Attributes* attributes);

private:
  //TODO: two derefferentions - messy, wrap it into class with overloaded [] operator
  Attributes** edges;
};

#endif //UNITTOKEN_HPP
