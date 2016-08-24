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
  \file terralib/qt/widgets/layer/utils/CharEncodingMenuWidget.h

  \brief This file defines a class for a CharEncodingMenuWidget.
*/

#ifndef __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_CHARENCODINGMENUWIDGET_H
#define __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_CHARENCODINGMENUWIDGET_H

// TerraLib
#ifndef Q_MOC_RUN
#include "../../../../maptools/AbstractLayer.h"
#endif
#include "../../Config.h"

//STL
#include <memory>

// Qt
#include <QAction>
#include <QMenu>
#include <QWidget>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class CharEncodingMenuWidget

        \brief This class is widget that provides a menu for char encoding selection.
      */
      class TEQTWIDGETSEXPORT CharEncodingMenuWidget : public QWidget
      {
        Q_OBJECT

        public:

          /*! Default Constructor */
          CharEncodingMenuWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! Default Destructor */
          ~CharEncodingMenuWidget();

        public:

          /*!
            \brief Set the layer that can be used

            \param layer  Pointer to the selected layer.
          */
          void setLayer(te::map::AbstractLayerPtr layer);

          QMenu* getMenu();

        protected:

          void checkLayerDataSourceType();

        public slots:

          void onEncodingClicked();

        public:
          te::map::AbstractLayerPtr m_layer;            //!< Selected Layer.

          QMenu* m_menu;

          QActionGroup* m_actGroup;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_CHARENCODINGMENUWIDGET_H
