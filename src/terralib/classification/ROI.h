/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/classification/ROI.h

  \brief A region of interest (often abbreviated ROI), is a selected subset of 
        samples within a dataset identified for a particular purpose.
*/

#ifndef __TERRALIB_CLASSIFICATION_INTERNAL_ROI_H
#define __TERRALIB_CLASSIFICATION_INTERNAL_ROI_H

// TerraLib
#include "Config.h"

// STL
#include <map>
#include <string>

namespace te
{
  // Forward declarations
  namespace gm 
  { 
    class Polygon;
  }

  namespace cl
  {
    /*!
      \brief A region of interest (often abbreviated ROI), is a selected subset of 
            samples within a dataset identified for a particular purpose. 
            
            An ROI can be taken literally as a polygonal selection from a 2D map. 
            In computer vision and optical character recognition, the ROI defines the borders 
            of an object under consideration. In many applications, symbolic (textual) labels 
            are added to a ROI, to describe its content in a compact manner.
            
            Within an ROI may lie individual points of interest (POIs).

            In this first version, the region will be represented only by polygons.
    */
    class TECLEXPORT ROI
    {
      public:

        /*! \brief Default constructor. */
        ROI(std::string label);

        /*! \brief Virtual destructor. */
        virtual ~ROI();

      public:

        /*! \brief Set the ROI label. */
        void setLabel(std::string label);

        /*! \brief Get the ROI label. */
        std::string getLabel();

        /*! \brief Set the ROI color using the hexadecimal color name. */
        void setColor(std::string color);

        /*! \brief Get the ROI color defined by a hexadecimal color name. */
        std::string getColor();

        /*! \brief Get all polygons belongs to this roi. */
        std::map<std::string, te::gm::Polygon*>& getPolygons();

        /*! 
          \brief Add a new region into this ROI

          \param p  The polygon that represents the region of interest.

          \param id The polygon unique identifier.
        */
        void addPolygon(te::gm::Polygon* p, std::string id);

        /*! 
          \brief Removes a region from this ROI

          \param id The polygon ID that has to be removed.
        */
        void removePolygon(std::string id);

      private:

        std::string m_label; //!< ROI label.
        std::string m_color; //!< The hexa color associated with this ROI.
        std::map<std::string, te::gm::Polygon*> m_roiMap; //!< The map of acquired regions.
    };
  } // end namespace cl
}   // end namespace te


#endif  // __TERRALIB_CLASSIFICATION_INTERNAL_ROI_H
