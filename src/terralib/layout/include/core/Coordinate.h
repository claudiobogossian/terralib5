#ifndef COORDINATE_H
#define COORDINATE_H

namespace te
{
  namespace layout
  {
    class Coordinate 
    {
      private: 

        double x;
        double y;
        double z;

      public:
        Coordinate();
        ~Coordinate();

        double getX(){
          return this->x;
        }

        void setX(double x){
          this->x = x;
        }

        double getY(){
          return this->y;
        }

        void setY(double y){
          this->y = y;
        }

        double getZ(){
          return this->z;
        }

        void setZ(double z){
          this->z = z;
        }

        virtual bool equals(Coordinate other);
    };
  }
}

#endif