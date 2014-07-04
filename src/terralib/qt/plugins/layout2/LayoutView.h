/*  Copyright (C) 2013-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file LayoutView.h
   
  \brief 

  \ingroup layout
*/

#ifndef	__TE_QT_PLUGINS_LAYOUT2_INTERNAL_LAYOUT_VIEW_H 
#define __TE_QT_PLUGINS_LAYOUT2_INTERNAL_LAYOUT_VIEW_H

// TerraLib
#include "../../../layout/qt/core/View.h"

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace layout2
      {
        class OutsideArea;

        class LayoutView : public te::layout::View
        {
          Q_OBJECT //for slots/signals

          public:
            LayoutView(QWidget* widget = (QWidget*)0);
            ~LayoutView();

            virtual void config();        
        
            void setOutsideArea(OutsideArea* outsideArea);
                                
          public slots:

            virtual void onSelectionChanged();
            virtual void onAddItemFinalized();

          protected:
        
            void hideEvent ( QHideEvent * event );

            void closeEvent ( QCloseEvent * event );

            virtual void	showEvent ( QShowEvent * event );
                                
          protected:
            OutsideArea*  m_outsideArea;
         };
      }
    }
  }
}
#endif