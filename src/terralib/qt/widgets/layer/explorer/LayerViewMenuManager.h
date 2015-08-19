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
 * \file LayerViewMenuManager.h
 *
 * \brief Class for manage the menus of the LayerViewItem.
 */
#ifndef __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_LAYERVIEWMENUMANAGER_H
#define __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_LAYERVIEWMENUMANAGER_H

#include "../../Config.h"

// Qt
#include <QObject>

// STL
#include <memory>

// Forward declarations
class QAction;

namespace te
{
  namespace common
  {
    template<typename T> struct QueueT;
  }

  namespace qt
  {
    namespace widgets
    {
      class LayerItemView;
      typedef te::common::QueueT<QAction*> QueueAction;


      /*!
      * \brief The LayerViewMenuManager class
      */
      class TEQTWIDGETSEXPORT LayerViewMenuManager: public QObject
      {
        Q_OBJECT

      public:

        /*!
        * \brief The LMENUACTIONS enum
        */
        enum LMENUACTIONS
        {
          VECTOR_LAYER,
          RASTER_LAYER,
          FOLDER_LAYER,
          MULTI_LAYERS,
          NO_LAYERS,
          ALL_LAYERS,
          INVALID_LAYERS
        };

        /*!
        * \brief LayerViewMenuManager
        * \param view
        */
        LayerViewMenuManager(LayerItemView* view);

        ~LayerViewMenuManager();

        /*!
        * \brief addAction
        * \param mnu
        * \param act
        * \param pos
        */
        void addAction(LMENUACTIONS mnu, QAction* act, int pos = -1);

        /*!
        * \brief eventFilter
        * \param watched
        * \param event
        * \return
        */
        bool eventFilter(QObject* watched, QEvent* event);

      protected:

        LayerItemView* m_view;

        std::auto_ptr<QueueAction> m_VL_actions;
        std::auto_ptr<QueueAction> m_RL_actions;
        std::auto_ptr<QueueAction> m_FL_actions;
        std::auto_ptr<QueueAction> m_ML_actions;
        std::auto_ptr<QueueAction> m_NL_actions;
        std::auto_ptr<QueueAction> m_AL_actions;
        std::auto_ptr<QueueAction> m_IL_actions;
      };

    }
  }
}

#endif // __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_LAYERVIEWMENUMANAGER_H
