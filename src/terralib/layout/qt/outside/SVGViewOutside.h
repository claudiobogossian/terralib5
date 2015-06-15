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
  \file terralib/layout/qt/outside/SVGViewOutside.h

  \brief
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_SVGVIEW_OUTSIDE_H
#define __TERRALIB_LAYOUT_INTERNAL_SVGVIEW_OUTSIDE_H

// TerraLib
#include "../../../qt/widgets/utils/DoubleListWidget.h"
#include "../../../maptools/AbstractLayer.h"
#include "../../core/Config.h"
#include "../../core/pattern/mvc/OutsideObserver.h"

// STL
#include <memory>
#include <string>
#include <vector>
#include <list>

// Qt
#include <QDialog>

namespace Ui { class SVGView; }

namespace te
{
  namespace layout
  {
    class OutsideController;
    class Observable;

    class TELAYOUTEXPORT SVGViewOutside : public QDialog, public OutsideObserver 
    {
      Q_OBJECT

      public:

        SVGViewOutside(OutsideController* controller, Observable* o);

        ~SVGViewOutside();

        /*!
          \brief Load layers in double widget.
        */
        virtual void init();
        
        virtual void updateObserver( ContextItem context );

        virtual void setPosition( const double& x, const double& y );

        virtual te::gm::Coord2D getPosition();

      signals:

        void updateProperty(Property prop);

        void updateProperties(std::vector<Property> props);
        
      protected slots:

        void onOkPushButtonClicked();

        void onCancelPushButtonClicked();

      private:

        std::auto_ptr<Ui::SVGView> m_ui;
        std::auto_ptr<te::qt::widgets::DoubleListWidget> m_widget;        
    };
  }    
}     

#endif

