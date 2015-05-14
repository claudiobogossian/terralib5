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
  \file terralib/qt/widgets/help/HelpPushButton.h

  \brief A specialized button that uses terralib help managers.
*/

#ifndef __TERRALIB_QT_WIDGETS_HELP_INTERNAL_HELPPUSHBUTTON_H
#define __TERRALIB_QT_WIDGETS_HELP_INTERNAL_HELPPUSHBUTTON_H

// TerraLib
#include "../Config.h"

//Qt 
#include <QPushButton>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class HelpPushButton

        \brief Push button that uses te::qt::widgets::HelpManager on its mouse pressed implementation.

        The client must set the page reference, as described in te::qt::widgets::HelpManagerImpl API documentation, before using it or nothing will happen.

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT HelpPushButton : public QPushButton
      {
        Q_OBJECT

        public:

          /*! 
            \brief Constructor.

            \param parent Parent widget, used by Qt.
          */
          HelpPushButton(QWidget* parent = 0);

          /*!
            \brief Sets the documentation page reference. 

            \param ref Reference to a page contained in help file project. See te::qt::widgets::HelpManagerImpl for correct syntax of this value.

            This will be used to open the correct help file when left mouse button pressed. Using it we can make a context sensitive helper.
          */
          void setPageReference(const QString& ref);

          /*!
            \brief Sets the namespace to be used.

            \param ns New namespace.
          */
          void setNameSpace(const QString& ns);

        protected:

          /*!
            \brief Overloaded function of QPushButton. Only LEFT button pressed will show the help.

            \param e The mouse press event.
          */
          void mousePressEvent(QMouseEvent* e);

        protected:

          QString m_pgRef;   //!< Stores the reference for the help file to be presented.
          QString m_ns;      //!< Namespace being used. (Qt help usage)
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_HELP_INTERNAL_HELPPUSHBUTTON_H

