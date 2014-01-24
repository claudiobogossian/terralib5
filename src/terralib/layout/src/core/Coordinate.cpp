#include "Coordinate.h"

te::layout::Coordinate::Coordinate()
{
  this->x = 0.0;
  this->y = 0.0;
  this->z = 0.0;
}

te::layout::Coordinate::~Coordinate(){
}

bool te::layout::Coordinate::equals(Coordinate other){
  if (this->x == other.getX() && this->y == other.getY() && this->z == other.getZ()){
    return true;
  }
  return false;
}