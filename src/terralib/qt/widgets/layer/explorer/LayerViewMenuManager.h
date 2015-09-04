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
 * \brief Class for manage the menus of the LayerItemView.
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
      * \class LayerViewMenuManager
      *
      * \brief Class for manage the menus of the LayerViewItem.
      *
      * \ingroup widgets
      */
      class TEQTWIDGETSEXPORT LayerViewMenuManager: public QObject
      {
        Q_OBJECT

      public:

        /*!
        * \enum LMENUACTIONS
        *
        * \brief Defines the type of actions we can use.
        */
        enum LMENUACTIONS
        {
          VECTOR_LAYER,   //!< Vector layers menu.
          RASTER_LAYER,   //!< Raster layers menu.
          FOLDER_LAYER,   //!< Folder layers menu.
          MULTI_LAYERS,   //!< Multiple layers selected menu.
          NO_LAYERS,      //!< No selected layers menu.
          ALL_LAYERS,     //!< All kinds of items menu.
          INVALID_LAYERS  //!< Invalid layers menu.
        };

        /*!
        * \name Instantiation Methods
        *
        * Methods related to creation and destruction of the object.
        */
        //@{

        /*!
        * \brief Contructor.
        * \param view The view that is being used.
        */
        LayerViewMenuManager(LayerItemView* view);

        /*!
         * \brief Destructor
         */
        ~LayerViewMenuManager();
        //@}

        /*!
        * \brief Adds an action to some menu.
        * \param mnu The menu we will update.
        * \param act The new action to be added.
        * \param pos The position of the menu. \a pos = -1 signs to append the action.
        */
        void addAction(LMENUACTIONS mnu, QAction* act, int pos = -1);

        /*!
        * \name Reimplementation of QObject
        *
        * Methods reimplemented of the QObject class.
        */
        //@{
        bool eventFilter(QObject* watched, QEvent* event);
        //@}

      protected:

        LayerItemView* m_view;                    //!< View being inspected.

        std::auto_ptr<QueueAction> m_VL_actions;  //!< Vector layers action list.
        std::auto_ptr<QueueAction> m_RL_actions;  //!< Raster layers action list.
        std::auto_ptr<QueueAction> m_FL_actions;  //!< Folder layers action list.
        std::auto_ptr<QueueAction> m_ML_actions;  //!< Multiple selected items action list.
        std::auto_ptr<QueueAction> m_NL_actions;  //!< No selected items action list.
        std::auto_ptr<QueueAction> m_AL_actions;  //!< All kinds of items action list.
        std::auto_ptr<QueueAction> m_IL_actions;  //!< Invalid layers action list.
      };

    }
  }
}

#endif // __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_LAYERVIEWMENUMANAGER_H
