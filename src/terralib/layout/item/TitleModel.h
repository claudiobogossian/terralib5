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
  \file TitleModel.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_TITLE_MODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_TITLE_MODEL_H

// TerraLib
#include "DefaultTextModel.h"

namespace te
{
  namespace layout
  {
    class TitleModel : public DefaultTextModel
    {
      public:

        TitleModel();
        virtual ~TitleModel();

        virtual te::layout::Properties* getProperties() const;

        virtual void updateProperties(te::layout::Properties* properties); 

        virtual void setTitle(std::string title);

        virtual std::string getTitle();

        virtual void setSpacing(double value);

        virtual double getSpacing();

        virtual void setPadding(double value);

        virtual double getPadding();

        virtual void setNumberColumns(int value);

        virtual int getNumberColumns();

        virtual void setNumberRows(int value);

        virtual int getNumberRows();

      protected:

        std::string m_title;

        /* Text Table Settings */

        /* General */

        double m_spacing;
        double m_padding;
        te::color::RGBAColor m_tableColor;
        //EnumType* m_widthConstraints;
        double m_width;

        /* Rows */
        bool m_addRow;
        int m_rowNumber;
        te::color::RGBAColor m_evenRow;
        te::color::RGBAColor m_oddRow;

        /* Columns */

        bool m_addColumn;
        int m_columnNumber;
        double m_columnWidth;
        //EnumType* m_columnWidthConstraints;

        /* Border */

        double m_borderWidth;
        te::color::RGBAColor m_borderGridColor;
        //EnumType* m_borderStyle;

        /* Header */

        //EnumType* m_headerAlignment; 
        te::color::RGBAColor m_headerHorizontalColor;
        te::color::RGBAColor m_headerVerticalColor;

        /* Cell */

        //EnumType* m_cellAligment;
    };
  }
}

#endif
