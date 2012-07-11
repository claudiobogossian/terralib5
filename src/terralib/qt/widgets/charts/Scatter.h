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
  \file Scatter.h

  \brief A class to represent a scatter. 
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_SCATTER_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_SCATTER_H

//STL
#include <vector>

//TerraLib
#include "../Config.h"

namespace te
{
  // Forward declarations
  namespace dt { class AbstractData; } 
 
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class Scatter

        \brief A class to represent a scatter.
      */
      class TEQTWIDGETSEXPORT Scatter 
      {
        public:

          /*!
            \brief Constructor
          */
          Scatter(const std::vector<double>& axisX, const std::vector<double>& axisY);

          /*!
            \brief Constructor
          */
          Scatter();

          /*!            
            \brief Calculates the minimum and maximum values for both the X and Y axis
          */
          void te::qt::widgets::Scatter::calculateMinMaxValues();
          
          /*!            
            \brief It returns the size of the scatter.

            \return The number of the scatter elements.
          */
          std::size_t size();

          /*!            
            \brief It returns the idx-th value of the scatter X axis, as a double. 

            \param idx  The index of the element.
            
            \return The idx-th value of the scatter X axis, as a double.  
          */
          double getX(unsigned int idx);

          /*!            
            \brief It returns a pointer to the first value of the scatter X axis. 

            \return A pointer to the first value of the scatter X axis. 

            \note The caller will not take the ownership of the returned pointer.  
          */
          double* getX();

          /*!            
            \brief It returns the idx-th value of the scatter Y axis, as a double. 

            \param idx  The index of the element.
            
            \return The idx-th value of the scatter Y axis, as a double.  
          */
          double getY(unsigned int idx);

          /*!            
            \brief It returns a pointer to the first value of the scatter Y axis. 

            \return A pointer to the first value of the scatter Y axis.

            \note The caller will not take the ownership of the returned pointer.  
          */
          double* getY();

          /*!            
            \brief It returns a reference to the internal string vector related to the X axis. 

            \return A reference to the internal string vector related to the X axis. 

          */
          std::vector<std::string>& getXString();

          /*!            
          \brief It returns a reference to the internal string vector related to the Y axis. 

          \return A reference to the internal string vector related to the Y axis. 
 
          */
          std::vector<std::string>& getYString();

          /*!            
            \brief It returns the minimum value of the X axis. 

            \return The minimum value of the X axis. 

            \note The caller will not have the ownership of the returned pointer. 
          */
          double getMinX();

          /*!            
            \brief It returns the maximum value of the X axis. 

            \return The maximum value of the X axis. 

            \note The caller will not have the ownership of the returned pointer. 
          */
          double getMaxX();

          /*!            
            \brief It returns the minimum value of the Y axis. 

            \return The minimum value of the Y axis. 

            \note The caller will not have the ownership of the returned pointer. 
          */
          double getMinY();

          /*!            
            \brief It returns the maximum value of the Y axis. 

            \return The maximum value of the Y axis. 

            \note The caller will not have the ownership of the returned pointer. 
          */
          double getMaxY();

          /*!            
            \brief It sets the minimum value of the X axis.  
          */
          void setMinX(double& new_minX);

          /*!            
            \brief It sets the maximum value of the X axis. 
          */
          void setMaxX(double& new_maxX);

          /*!            
            \brief It sets the minimum value of the Y axis. 
          */
          void setMinY(double& new_minY);

          /*!            
            \brief It sets the maximum value of the Y axis. 
          */
          void setMaxY(double& maxY);

          /*!            
            \brief It adds a new value to the vector containing the X axis values. 

            \param new_XValue  The new value to be added to the vector.
              
          */
          void addX(double& xValue);

          /*!            
            \brief It adds a new value to the vector containing the Y axis values. 

            \param new_YValue  The new value to be added to the vector.
              
          */
          void addY(double& yValue);

          /*! \brief Destructor. */
          ~Scatter();
        
        private:

          std::vector<double> m_xValues;      /*! Double values for axis X */
          std::vector<double> m_yValues;      /*! Double values for axis Y */
          
          std::vector<std::string> m_xString;  /*! String values for axis X */
          std::vector<std::string> m_yString;  /*! String values for axis Y */
          
          double m_minX;
          double m_maxX;
          double m_minY;
          double m_maxY;                   
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_SCATTER_H

