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
  \file Histogram.h

  \brief A class to represent a histogram. 
*/

//STL
#include <map>

//TerraLib
#include "../Config.h"

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAM_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAM_H

namespace te
{
  namespace qt
  {
    namespace widgets
    {
       /*!
        \class Histogram

        \brief A class to represent a Histogram.
      */
      class TEQTWIDGETSEXPORT Histogram 
      {
        public:

          /*!
            \brief Constructor
          */
          Histogram();

          /*!
            \brief Constructor
          */
          Histogram(std::map<double, int> values, double minValue);

          /*! 
            \brief Destructor.
          */
          ~Histogram();

          /*!            
            \brief It returns the map containing the histogram values. 

            \return A  map containing the histogram values. 

            \note The caller will not have the ownership of the returned pointer. 
          */

          std::map<double, int>& getValues();
      
          /*!            
            \brief It sets the map containing the histogram values. 

            \param new_values The new map of values
          */

          void setValues(std::map<double, int> new_values);

          /*!            
            \brief It returns the histogram's minimum value. 

            \return The histogram's minimum value. 

            \note The caller will not have the ownership of the returned pointer. 
          */

          double& getMinValue();
      
          /*!            
            \brief It sets the histogram's minimum value. 

            \param new_minValue The new minimum value.
          */

          void setMinValue(double new_minValue);

          /*!            
            \brief It returns the histogram's interval. 

            \return The histogram's interval. 

            \note The caller will not have the ownership of the returned pointer. 
          */

          double& getInterval();
      
          /*!            
            \brief It sets the histogram's interval. 

            \param new_values The new histogram's interval.
          */

          void setInterval(double new_Interval);

           /*!            
            \brief It adds a new value to the map containing the histogram values. 

            \param new_value The value that will be added.
          */

          void add(std::pair<double, int> new_value);

        private:

          std::map<double, int> m_values;
          double m_minValue;
          double m_interval;
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAM_H