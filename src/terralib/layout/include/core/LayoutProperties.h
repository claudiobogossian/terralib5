#ifndef LAYOUTPROPERTIES_H
#define LAYOUTPROPERTIES_H

#include <set>
#include <vector>
#include "LayoutProperty.h"

namespace te
{
  namespace layout
  {
    class LayoutProperties
    {
      public:
        LayoutProperties();
        virtual ~LayoutProperties(void);

        virtual bool addProperty(LayoutProperty property);
        virtual bool removeProperty(std::string name);
        virtual std::vector<LayoutProperty> getProperties();

      protected:
        std::vector<LayoutProperty> _properties;
        std::set<LayoutProperty> _properties1;

    };

    inline LayoutProperties::LayoutProperties()
    {
    }

    inline LayoutProperties::~LayoutProperties( void )
    {
    }

    inline bool LayoutProperties::addProperty(LayoutProperty property)
    {
      bool result = true;
      _properties.push_back(property);      
      return result;
    }

    inline bool LayoutProperties::removeProperty(std::string name)
    {
      bool result = true;
      return result;
    }

    inline std::vector<LayoutProperty> LayoutProperties::getProperties()
    {
      return _properties;
    }

  }
}

#endif