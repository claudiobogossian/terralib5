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
  \file PaperModel.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_PAPER_MODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_PAPER_MODEL_H

// TerraLib
#include "ItemModelObservable.h"
#include "ContextItem.h"

namespace te
{
  namespace layout
  {
    class PaperConfig;

    class PaperModel : public ItemModelObservable
    {
      public:

        PaperModel();
        PaperModel(PaperConfig* paperConfig);
        virtual ~PaperModel();

        virtual void draw( ContextItem context );

        virtual te::color::RGBAColor getShadowColor();
        virtual void setShadowColor(te::color::RGBAColor color);

        virtual void setPaperConfig(PaperConfig* pConfig );
        virtual PaperConfig* getPaperConfig() const;

        virtual void setShadowPadding(double padding);
        virtual double getShadowPadding();

        virtual te::gm::Envelope getBoxWithZoomFactor();

    protected:

      virtual void config();

      PaperConfig* m_paperConfig;
      te::gm::Envelope m_boxPaper;
      te::gm::Envelope m_boxShadow;
      te::color::RGBAColor m_shadowColor;
      double m_shadowPadding;
    };
  }
}

#endif // __TERRALIB_LAYOUT_INTERNAL_PAPERLAYOUT_MODEL_H