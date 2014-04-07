/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file LayoutUtils.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_LAYOUT_UTILS_H 
#define __TERRALIB_LAYOUT_INTERNAL_LAYOUT_UTILS_H

// TerraLib
#include "../../../geometry/Envelope.h"
#include "../../../geometry/LinearRing.h"
#include "../../../color/RGBAColor.h"
#include "../../../maptools/Canvas.h"
#include "../../../maptools/WorldDeviceTransformer.h"

namespace te
{
  namespace layout
  {
    class LayoutUtils
    {
      public:

        LayoutUtils();
        virtual ~LayoutUtils();
        virtual void drawRectW(te::gm::Envelope box);
        virtual void drawLineW(te::gm::LinearRing* line);
        virtual te::color::RGBAColor** getImageW(te::gm::Envelope box);
        virtual int mm2pixel(double mm);
        virtual void configCanvas(te::gm::Envelope box);
        virtual te::gm::Envelope viewportBox(te::gm::Envelope box);
        virtual te::gm::Envelope viewportBoxFromScene(te::gm::Envelope box);
        virtual te::gm::LinearRing* createSimpleLine(te::gm::Envelope box);

      protected:

        te::map::WorldDeviceTransformer    m_transformer; // World Device Transformer.
    };
  }
}

#endif // __TERRALIB_LAYOUT_INTERNAL_LAYOUT_UTILS_H