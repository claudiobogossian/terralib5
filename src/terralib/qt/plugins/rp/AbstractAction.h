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
  \file terralib/qt/plugins/rp/AbstractAction.h

  \brief This file defines the abstract class AbstractAction
*/

#ifndef __TE_QT_PLUGINS_RP_INTERNAL_ABSTRACTACTION_H
#define __TE_QT_PLUGINS_RP_INTERNAL_ABSTRACTACTION_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "Config.h"

// Qt
#include <QObject>
#include <QMenu>
#include <QAction>

namespace te
{
  namespace qt
  {
    namespace af
    {
      namespace evt
      {
        struct Event;
      }
    }
    namespace plugins
    {
      namespace rp
      {
        /*!
          \class AbstractAction

          \brief This is an abstract class used to register actions into rp pluging.

        */
        class AbstractAction : public QObject
        {
          Q_OBJECT

          public:

            /*!
              \brief Constructor.

              \param menu The parent menu object.
            */
            AbstractAction(QMenu* menu, QMenu* popupMenu);

            /*! 
              \brief Destructor. 
            */
            virtual ~AbstractAction();

          protected slots:

            /*!
              \brief Slot function used when a action was selected.

              \param checked Flag used in case a toggle action.
            */
            virtual void onActionActivated(bool checked) = 0;

            /*!
              \brief Slot function used when a action was selected from layer pop up.

              \param checked Flag used in case a toggle action.
            */
            virtual void onPopUpActionActivated(bool checked);

          protected:

            /*!
              \brief Create and set the actions parameters.

              \param name The action name.

              \param pixmap The action pixmap name.
            */
            void createAction(std::string name, std::string pixmap = "");
            
            /*!
              \brief Create and set the actions parameters.

              \param name The action name.

              \param pixmap The action pixmap name.
            */
            void createPopUpAction(std::string name, std::string pixmap = "");

            /*!
              \brief Add a new layer into layer explorer widget

              \param layer The layer auto pointer

            */
            void addNewLayer(te::map::AbstractLayerPtr layer);

            /*!
              \brief Get the selected layer from layer explorer in app

              \return The layer auto pointer

            */
            te::map::AbstractLayerPtr getCurrentLayer();

            /*!
              \brief Get the list of layers from app

              \return The list pf layer auto pointers

            */
            std::list<te::map::AbstractLayerPtr> getLayers();


          Q_SIGNALS:

            void triggered(te::qt::af::evt::Event* e);

          public:
            QAction* getAction()
            {
              return m_action;
            }

          protected:

            QMenu* m_menu;          //!< Parent Menu.
            QMenu* m_popupMenu;     //!< Parent popMenu.
            QAction* m_action;      //!< Action used to call the process.
            QAction* m_popupAction; //!< Action used to call the process from pop up layer.
        };

      } // end namespace rp
    }   // end namespace plugins
  }     // end namespace qt
}       // end namespace te

#endif //__TE_QT_PLUGINS_RP_INTERNAL_ABSTRACTACTION_H