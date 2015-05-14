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
  \file terralib/qt/widgets/se/CharMapWidget.h

  \brief A widget used to show the set of characters of a specified font.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_CHARMAPWIDGET_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_CHARMAPWIDGET_H

// TerraLib
#include "../Config.h"

// Qt
#include <QWidget>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class CharMapWidget

        \brief A widget used to show the set of characters of a specified font.
      */
      class TEQTWIDGETSEXPORT CharMapWidget : public QWidget
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs the character map widget which is a child of parent, with widget flags set to f. */
          CharMapWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~CharMapWidget();

          //@}

        public:

          QSize sizeHint() const;

          int getCurrentChar() const;

        public slots:
          
          void updateFont(const QFont& font);

        signals:

          void charSelected(const unsigned int& charCode);

        protected:

          void mouseMoveEvent(QMouseEvent* e);

          void mousePressEvent(QMouseEvent* e);

          void paintEvent(QPaintEvent* e);

        private:

          QFont m_font;                //!< Current font.
          unsigned int m_currentChar;  //!< Current selected char.
          int m_columns;               //!< Character map number of column.
          int m_squareSize;            //!< Cell size of character map.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_CHARMAPWIDGET_H
