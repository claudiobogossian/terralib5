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
  \file ColorDialogOutside.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_COLOR_DIALOG_OUTSIDE_H 
#define __TERRALIB_LAYOUT_INTERNAL_COLOR_DIALOG_OUTSIDE_H

// Qt
#include <QColorDialog>

// TerraLib
#include "../../core/pattern/mvc/AbstractOutsideView.h"
#include "../../../geometry/Envelope.h"
#include "../../core/Config.h"

// STL
#include <vector>

// Qt
#include <QColor>
#include <QEvent>
#include <QCloseEvent>

namespace te
{
  namespace layout
  {
    class AbstractOutsideController;
    /*!
    \brief 
    
      \ingroup layout

      \sa te::layout::OutsideObserver
    */
    class TELAYOUTEXPORT ColorDialogOutside : public QColorDialog, public AbstractOutsideView
    {
      Q_OBJECT //for slots/signals

      public:

        ColorDialogOutside(AbstractOutsideController* controller);

        virtual ~ColorDialogOutside();

        virtual void init();
        
        virtual void setPosition(const double& x, const double& y);

        virtual te::gm::Coord2D getPosition();

      signals:

        void updateProperty(Property prop);

      public slots:

        virtual void  accept();

      protected slots:

        void  onCurrentColorChanged( const QColor & color );

      protected:

        virtual void  closeEvent ( QCloseEvent * event );

        virtual bool event(QEvent * e);

        QColor m_currentColor;
        bool   m_okClicked;
    };
  }
}

#endif
