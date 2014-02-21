#ifndef LAYOUTPROPERTY_H
#define LAYOUTPROPERTY_H

#include <string>

namespace te
{
  namespace layout
  {
   
    class LayoutProperty
    {
      public:
        LayoutProperty();
        virtual ~LayoutProperty();

        std::string getName();
        void setName(std::string name); 

        std::string getLabel();
        void setLabel(std::string label);

    protected:

      std::string _name;
      std::string _label;
    };
  }
}

#endif