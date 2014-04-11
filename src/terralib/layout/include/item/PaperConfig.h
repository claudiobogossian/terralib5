/*  Copyright (C) 2014-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file PaperConfig.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_PAPER_CONFIG_H
#define __TERRALIB_LAYOUT_INTERNAL_PAPER_CONFIG_H

// TerraLib
#include "AbstractType.h"

namespace te
{
  namespace layout
  {
    class PaperConfig
    {
      public:

        PaperConfig();
        PaperConfig(LayoutAbstractPaperType paperType);
        virtual ~PaperConfig();

        virtual void setPaperSize(LayoutAbstractPaperType paperType);
        virtual void getPaperSize(double &w, double &h);

        virtual void setPaperOrientation(LayoutOrientationType orientation);
        virtual LayoutOrientationType getPaperOrientantion();

      protected:

        LayoutAbstractPaperType m_paperType;
        LayoutOrientationType   m_paperOrientationType;
    };
  }
}

#endif // __TERRALIB_LAYOUT_INTERNAL_PAPERLAYOUT_CONFIG_H