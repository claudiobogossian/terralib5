/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file GroupingItem.h

  \brief This class contains information about a grouping item associated to a layer.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_GROUPINGITEM_H
#define __TERRALIB_MAPTOOLS_INTERNAL_GROUPINGITEM_H

// TerraLib
#include "../se/Symbolizer.h"
#include "Config.h"

// STL
#include <memory>
#include <vector>

namespace te
{
  namespace map
  {
    /*!
      \class GroupingItem

      \brief A GroupingItem contains information about a grouping item associated to a layer.
    
      \sa AbstractLayer, Layer
    */
    class TEMAPEXPORT GroupingItem
    {
      public:

        /*!
          \brief It initializes a new GroupingItem.

          \param from The value of the lower limit of the legend item.
          \param to   The value of the upper limit of the legend item.
        */
        GroupingItem(const std::string& from = "", const std::string& to = "");

        /*! \brief Destructor. */
        ~GroupingItem();

        /*! \brief Copy constructor. */
        GroupingItem(const GroupingItem& rhs);

        /*! Assignment operator.  */
        GroupingItem& operator=(const GroupingItem& rhs);

        /*!
          \brief It gets the value of the legend item.

          \output The value of the legend item.
        */
        const std::string& getValue() const;

        /*!
          \brief It sets value of the legend item.

          \param value The value of the legend item.
        */
        void setValue(const std::string& value);

        /*!
          \brief It gets the lower limit value of the legend item.

          \output The lower limit value of the legend item.
        */
        const std::string& getLowerLimit() const;

        /*!
          \brief It sets the lower limit value of the legend item.

          \param from The lower limit value.
        */
        void setLowerLimit(const std::string& from);

        /*!
          \brief It gets the upper limit value of the legend item.

          \output The upper limit value of the legend item.
        */
        const std::string& getUpperLimit() const;

        /*!
          \brief It sets the upper limit value of the legend item.

          \param to The upper limit value.
        */
        void setUpperLimit(const std::string& to);

        /*!
          \brief It gets the title of the legend item.

          \return The title of the legend item.
        */
        std::string getTitle();

        /*!
          \brief It sets the title of the legend item.

          \param title The title of the legend item..
        */
        void setTitle(const std::string& title);

        /*!
          \brief It gets the number of objects whose values are between the lower and upper limits.

          \output The number of objects whose values are between the lower and upper limits.
        */
        std::size_t getCount() const;

        /*!
          \brief It It sets the number of objects whose values are between the lower and upper limits.

          \param count The number of objects whose values are between the lower and upper limits
        */
        void setCount(std::size_t count);

        /*!
          \brief It gets the symbolizer of the legend item.

          \output The symbolizer of the legend item.
        */
        const std::vector<te::se::Symbolizer*>& getSymbolizers() const;

        /*!
          \brief It sets the symbolizers of the legend item.

          \param symbolizers The symbolizers of the legend item..
        */
        void setSymbolizers(const std::vector<te::se::Symbolizer*>& symbolizers);

      private:

        std::string m_from;                               //!< The value of the lower limit of the legend item.
        std::string m_to;                                 //!< The value of the upper limit of the legend item.
        std::string m_value;                              //!< The value of the legend item.
        std::size_t m_count;                              //!< The number of objects whose values are between the lower and upper limits.
        std::string m_title;                              //!< A text that can be used to identify this legend item.
        std::vector<te::se::Symbolizer*> m_symbolizers;   //!< The color of the legend item.
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_GROUPINGITEM_H

