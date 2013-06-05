/*  Copyright (C) 2010-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/charts/Histogram.h

  \brief A class to represent a histogram. 
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAM_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAM_H

//TerraLib
#include "../Config.h"

//STL
#include <map>
#include <set>
#include <string>
#include <vector>

namespace te
{

  namespace da    { class ObjectId; }

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
            \brief Destructor.
          */
          ~Histogram();

            /*!            
            \brief It returns the histogram's type.  

            \return The histogram's type.  

          */

          int& getType();
      
          /*!            
            \brief It sets the histogram's type. 

            \param new_type The new type 
          */

          void setType(int new_type);

          /*!            
            \brief It returns the map containing the histogram values. 

            \return A  map containing the histogram values. 
          */

          std::map<double, unsigned int>* getValues();

          /*!            
            \brief It sets the map containing the histogram values. 

            \param new_values The new map of values
          */

          void setValues(std::map<double, unsigned int>* new_values);

          /*!            
            \brief It returns the map containing the histogram values' objectIds. 

            \return A  map containing the histogram values' objectIds. 
          */

          std::map<double, std::vector<te::da::ObjectId*> > getValuesOIDs();

          /*!            
            \brief It sets the map containing the histogram values' objectIds. 

            \param new_values The new map of values' objectIds
          */

          void setValuesOIDs( std::map<double, std::vector<te::da::ObjectId*> > new_ids);

          /*!            
            \brief It returns the map containing the histogram String values. 

            \return A  map containing the histogram values. 
          */

          std::map<std::string, unsigned int>* getStringValues();
      
          /*!            
            \brief It sets the map containing the histogram String values. 

            \param new_values The new map of values
          */

          void setStringValues(std::map<std::string, unsigned int>* new_values);

          /*!            
            \brief It returns the histogram's minimum value. 

            \return The histogram's minimum value. 
          */

          /*!            
            \brief It returns the map containing the histogram string values' objectIds. 

            \return A  map containing the histogram values' objectIds. 
          */

          std::map<std::string, std::vector<te::da::ObjectId*> > getStringOIDs();

          /*!            
            \brief It sets the map containing the histogram string values' objectIds. 

            \param new_values The new map of values' objectIds
          */

          void setStringOIDs( std::map<std::string, std::vector<te::da::ObjectId*> > new_ids);

          double& getMinValue();
      
          /*!            
            \brief It sets the histogram's minimum value. 

            \param new_minValue The new minimum value.
          */

          void setMinValue(double new_minValue);

          /*!            
            \brief It returns the histogram's interval. 

            \return The histogram's interval. 
          */

          double& getInterval();
      
          /*!            
            \brief It sets the histogram's interval. 

            \param new_values The new histogram's interval.
          */

          void setInterval(double new_Interval);

           /*!            
            \brief It returns the histogram's string set of intervals. 

            \return The histogram's interval. 
          */

          std::set <std::string>& getStringInterval();
      
          /*!            
            \brief It sets the histogram's string set of intervals. 

            \param new_values The new histogram's interval.
          */

          void setStringInterval( std::set <std::string> new_Interval);

           /*!            
            \brief It adds a new value to the map containing the histogram values. 

            \param new_value The value that will be added.
          */

          void add(std::pair<double, int> new_value);

        private:

          int m_histogramType;                                                 //!< Histogram's type
          std::map<double, unsigned int>* m_values;                            //!< Histogram's numeric values
          std::map<double, std::vector<te::da::ObjectId*> > m_valuesOIDs;       //!< The objectIds organized by the histogram's intervals;
          std::map<std::string, unsigned int>* m_StringValues;                 //!< Histogram string values 
          std::map<std::string, std::vector<te::da::ObjectId*> > m_stringOIDs;  //!< The objectIds organized by the histogram's string intervals;
          double m_minValue;                                                   //!< Histogram's minimum numeric value
          double m_interval;                                                   //!< Histogram's numeric interval
          std::set <std::string> m_StringIntervals;                            //!< Histogram unique strings set, represents string intervals
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAM_H