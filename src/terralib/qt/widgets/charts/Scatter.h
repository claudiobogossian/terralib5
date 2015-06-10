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
  \file terralib/qt/widgets/charts/Scatter.h

  \brief A class to represent a scatter. 
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_SCATTER_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_SCATTER_H

//TerraLib
#include "../Config.h"
#include "ScatterUtils.h"

#include <QPointF>

namespace te
{
  // Forward declarations
  namespace da  { class ObjectIdSet;}

  namespace qt
  {
    namespace widgets
    {
      /*!
        \class Scatter

        \brief A class to represent a scatter.

        \ingroup widgets
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

          /*! \brief Destructor. */
          ~Scatter();

          /*!            
            \brief Calculates the minimum and maximum values for both the X and Y axis
          */
          void calculateMinMaxValues();

          /*!            
            \brief It returns the size of the scatter.

            \return The number of the scatter elements on the X axis.
          */
          std::size_t sizeX();

          /*!            
            \brief It returns the size of the scatter.

            \return The number of the scatter elements on the Y axis.
          */
          std::size_t sizeY();

          /*!
            \brief It returns the vector containing the values of the scatter X axis. 

            \return A vector containing the values of the scatter X axis.

          */
          std::vector<double> getXValues();

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
            \brief It returns the vector containing the values of the scatter Y axis. 

            \return A vector containing the values of the scatter Y axis.

          */
          std::vector<double> getYValues();

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
            \brief It sets the values of the X axis.

            \param xValues The vector containing the new values.
          */
          void setXValues(std::vector<double> xValues);

          /*!
            \brief It sets the minimum value of the X axis.  
          */
          void setMinX(double& new_minX);

          /*!
            \brief It sets the maximum value of the X axis. 
          */
          void setMaxX(double& new_maxX);

          /*!
            \brief It sets the values of the Y axis.  

            \param yValues The vector containing the new values.
          */
          void setYValues(std::vector<double> yValues);

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

            \param xValue  The new value to be added to the vector.
              
          */
          void addX(double& xValue);

          /*!
            \brief It adds a new value to the vector containing the Y axis values. 

            \param yValue  The new value to be added to the vector.
          */
          void addY(double& yValue);

          /*!
            \brief It adds the x and Y axis values to the scatter's vectors and the associeted objectId to the scatter's multi_index container.

            \param xValue  The new x axis value to be added to the vector.
            \param yValue  The new y axis value to be added to the vector.
            \param oid The ObjectId that will be added to the container

            \note it will take ownership of the given ObjectId pointer
          */
          void addData(double& xValue, double& yValue, te::da::ObjectId* oid);

          /*!
            \brief It returns a pointer to an ObjectIdSet that contains all of the scatter's selected  points.

            \param xValue  The x axis value to be searched.
            \param yValue  The x axis value to be searched.

            \note The caller will take ownership of the returned pointer
          */
          te::da::ObjectIdSet* find(double& xValue, double& yValue);

          /*!
            \brief It returns a pointer to an ObjectIdSet that contains all of the scatter's selected  points.

            \param selectedPoints  A vector containing all the points within the selection box
 
            \note The caller will take ownership of the returned pointer
          */
          te::da::ObjectIdSet* find(std::vector<QPointF> selectedPoints);

          /*!
            \brief It returns a pair representing the point that contais the secected ObjectId.

            \param oid  The ObjectId to be searched.
            \note Returns NULL if no matching point was found
          */
          const std::pair<double, double> find(const te::da::ObjectId* oid);

        private:

          PointToObjectIdSet m_valuesOids;  //!< The points and ObjecIds ordered in a boost multi index container
          std::vector<double> m_xValues;    //!< Double values for axis X
          std::vector<double> m_yValues;    //!< Double values for axis Y

          double m_minX; //!< The minimum value of the x Axis.
          double m_maxX; //!< The maximum value of the x Axis.
          double m_minY; //!< The minimum value of the y Axis.
          double m_maxY; //!< The maximum value of the y Axis.
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_SCATTER_H

