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
  \file LineModel.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_POLYGON_MODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_POLYGON_MODEL_H

// TerraLib
#include "../core/pattern/mvc/ItemModelObservable.h"
#include "../core/ContextItem.h"
#include "LineModel.h"

namespace te
{
  namespace layout
  {
    class Utils;

    class PolygonModel : public LineModel
    {
      public:

        PolygonModel();

        virtual ~PolygonModel();

        virtual te::layout::Properties* getProperties() const;

        virtual void updateProperties(te::layout::Properties* properties, bool notify = true);

        /*!
          \brief Gets the fill color of the shape
        */
        virtual const te::color::RGBAColor& getFillColor() const;

        /*!
          \brief Sets the fill color of the shape
        */
        virtual void setFillColor(const te::color::RGBAColor& color);

      protected:

        te::color::RGBAColor m_fillColor; //!< The fill color of the shape
    };
  }
}

#endif
