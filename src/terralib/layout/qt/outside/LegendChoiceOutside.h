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
  \file terralib/layout/qt/outside/LegendChoiceOutside.h

  \brief 
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_LEGEND_CHOICE_OUTSIDE_H
#define __TERRALIB_LAYOUT_INTERNAL_LEGEND_CHOICE_OUTSIDE_H

// TerraLib
#include "../../../qt/widgets/utils/DoubleListWidget.h"
#include "../../core/pattern/mvc/AbstractOutsideView.h"
#include "../../core/Config.h"
#include "../../core/property/Property.h"

// STL
#include <map>
#include <memory>

// Qt
#include <QDialog>

namespace Ui { class LegendChoice; }

namespace te
{
  namespace layout
  {
    class AbstractOutsideController;

    class TELAYOUTEXPORT LegendChoiceOutside : public QDialog, public AbstractOutsideView
    {
      Q_OBJECT

      public:

        LegendChoiceOutside(AbstractOutsideController* controller);

        ~LegendChoiceOutside();
        
        virtual void setPosition( const double& x, const double& y );

        virtual te::gm::Coord2D getPosition();

      signals:

        void updateProperty(Property prop);
        
      protected slots:

        void onOkPushButtonClicked();

        void onCancelPushButtonClicked();

      private:

        std::auto_ptr<Ui::LegendChoice> m_ui;
        std::auto_ptr<te::qt::widgets::DoubleListWidget> m_widget;
    };
  }   
}    

#endif



