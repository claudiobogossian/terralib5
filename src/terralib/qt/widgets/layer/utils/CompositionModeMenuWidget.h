/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/layer/utils/CompositionModeMenuWidget.h

  \brief This file defines a class for a CompositionModeMenuWidget.
*/

#ifndef __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_COMPOSITIONMODEMENUWIDGET_H
#define __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_COMPOSITIONMODEMENUWIDGET_H

// TerraLib
#include "../../../../maptools/AbstractLayer.h"
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
        \class CompositionModeMenuWidget

        \brief This class is widget that provides a menu for composition mode selection.
      */
      class TEQTWIDGETSEXPORT CompositionModeMenuWidget : public QWidget
      {
        Q_OBJECT

        public:

          /*! Default Constructor */
          CompositionModeMenuWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! Default Destructor */
          ~CompositionModeMenuWidget();

        public:

          /*!
            \brief Set the layer that can be used

            \param layer  Pointer to the selected layer.
          */
          void setLayer(te::map::AbstractLayerPtr layer);

          QMenu* getMenu();

        public slots:

          void onSourceOverClicked();

          void onPlusClicked();

          void onCompositionModeClicked();

        public:
          te::map::AbstractLayerPtr m_layer;                                          //!< Selected Layer.

          QMenu* m_menu;
          QAction* m_plusAction;
          QAction* m_sourceOverAction;
          QAction* m_compositionModeAction;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_COMPOSITIONMODEDIALOG_H
