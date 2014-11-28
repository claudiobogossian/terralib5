/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file Grouping.h

  \brief This class contains the parameters needed for grouping the values of a Property.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_GROUPING_H
#define __TERRALIB_MAPTOOLS_INTERNAL_GROUPING_H

// TerraLib
#include "Config.h"
#include "Enums.h"

// STL
#include <cstdlib>
#include <string>
#include <vector>

namespace te
{
// Forward declarations
  namespace dt
  {
    class Property;
  }

  namespace map
  {
    class GroupingItem;

    /*!
      \class Grouping

      \brief This class contains the parameters needed for grouping the values of a Property.

      \ingroup map

      \sa GroupingItem, Layer
    */
    class TEMAPEXPORT Grouping
    {
      public:

        /*!
          \brief It constructs a new Grouping instance.

          \param propertyName  The property name whose values will be used to make the grouping.
          \param type          The grouping type.
          \param precision     The precision to be set to the property values.
        */
        Grouping(const std::string& propertyName, GroupingType type, size_t precision = 6);

        /*! \brief Destructor. */
        ~Grouping();

        /*! \brief Copy constructor. */
        Grouping(const Grouping& rhs);

        /*!
          \brief It gets the property name whose values will be grouped.

          \return The property name.
        */
        std::string getPropertyName() const;

        /*!
          \brief It sets the property name whose values will be grouped.

          \param name The property name.
        */
        void setPropertyName(const std::string& name);

        /*!
          \brief It gets the property type whose values will be grouped.

          \return The property type.
        */
        int getPropertyType() const;

        /*!
          \brief It sets the property type whose values will be grouped.

          \param type The property type.
        */
        void setPropertyType(const int& type);

        /*!
          \brief It gets the grouping type.

          \return The grouping type.
        */
        const GroupingType getType() const;

        /*!
          \brief It sets the grouping type.

          \param type The grouping type.
        */
        void setType(GroupingType type);

        /*!
          \brief It gets the precision used for the property values.

          \output The precision used for the property values.
        */
        const size_t getPrecision() const;

        /*!
          \brief It sets the precision to be used for the property values.

          \param precision The precision value.
        */
        void setPrecision(size_t precision);

        /*!
          \brief It gets the number of slices used in the Equal Steps and Quantil groupings.

          \output The number of slices.
        */
        const size_t getNumSlices() const;

        /*!
          \brief It sets the number of slices for the EqualSteps and Quantil groupings.

          \param numSlices The number of slices.
        */
        void setNumSlices(size_t numSlices);

        /*!
          \brief It gets the standard deviation used in the Standard Deviation grouping.

          \output The standard deviation.
        */
        const double getStdDeviation() const;

       /*!
          \brief It sets the standard deviation for the Standard Deviation grouping.

          \param stdDeviation The standard deviation.
        */
        void setStdDeviation(double stdDeviation);

        /*!
          \brief It gets the vector of grouping items.

          \return The vector of grouping items.
        */
        const std::vector<te::map::GroupingItem*>& getGroupingItems() const;

        /*!
          \brief It sets the vector of grouping items.

          \param The vector of grouping items.
        */
        void setGroupingItems(std::vector<te::map::GroupingItem*>& items);

        /*! \brief It gets the grouping visibility. */
        bool isVisible() const;

        /*!
          \brief It sets the grouping visibility.

          \param visible The visibility value.
        */
        void setVisibility(bool visible);

        /*! \brief It gets the grouping summary. It is used only in case 1 to n.
        */
        std::string getSummary() const;

        /*!
          \brief It gets the grouping summary. It is used only in case 1 to n.

          \param summary The summary mode. It can be: "MIN", "MAX", "SUM", "AVERAGE", "MEDIAN", "STDDEV" or "VARIANCE"
        */
        void setSummary(const std::string& summary);

      private:

        std::string m_propertyName;                  //!< The property name whose values will be used to make the grouping.
        int m_propertyType;                          //!< The property type whose values will be used to make the grouping.
        GroupingType m_type;                         //!< The grouping type.
        size_t m_precision;                          //!< The precision of the values.
        size_t m_numSlices;                          //!< The number of slices used in the Equal Steps and Quantil groupings.
        double m_stdDeviation;                       //!< The standard deviation used in the Standard Deviation grouping.
        bool m_isVisible;                            //!< A flag that indicates if the grouping is visible.
        std::vector<te::map::GroupingItem*> m_items; //!< The vector of grouping items.
        std::string m_summary;                       //!< The summary used in case 1 to n.
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_GROUPING_H

