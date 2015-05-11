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
  \file terralib/qt/widgets/se/AbstractGraphicWidget.h

  \brief Abstract class that represents a widget that can be used to build a graphic element.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_ABSTRACTGRAPHICWIDGET_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_ABSTRACTGRAPHICWIDGET_H

// TerraLib
#include "../Config.h"

// Qt
#include <QWidget>

namespace te
{
// Forward declarations
  namespace se
  {
    class Graphic;
  }

  namespace qt
  {
    namespace widgets
    {
      /*!
        \class AbstractGraphicWidget

        \brief Abstract class that represents a widget that can be used to build a graphic element.

        \sa ExternalGraphicWidget, GlyphGraphicWidget, WellKnownGraphicWidget
      */
      class TEQTWIDGETSEXPORT AbstractGraphicWidget : public QWidget
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a graphic widget which is a child of parent, with widget flags set to f. */
          AbstractGraphicWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          virtual ~AbstractGraphicWidget();

          //@}

        public:

          /*!
            \brief Gets the configured graphic element.

            \return The configured graphic element.
            
            \note The caller will take the ownership of the returned graphic.
          */
          te::se::Graphic* getGraphic() const;

          /*!
            \brief Sets a graphic element to this widget.

            \param graphic A valid graphic element.

            \return It return true if the widget can deal with the given graphic. Otherwise, it returns false.

            \note The widget will NOT take the ownership of the given graphic.
            \note The widget form will be update based on given graphic parameters.
          */
          virtual bool setGraphic(const te::se::Graphic* graphic) = 0;

          /*! \brief Pure virtual method that should return a "user friendly" string that informs the graphic type that can be built by the widget. */
          virtual QString getGraphicType() const = 0;

          /*! \brief Pure virtual method that should return a QIcon that represents the graphic. i.e. a simple preview. */
          virtual QIcon getGraphicIcon(const QSize& size) const = 0;

        protected slots:

          void setGraphicSize(const QString& size);

          void setGraphicAngle(const QString& angle);

          void setGraphicOpacity(int value);

          void setGraphicDisplacement(const QString& dx, const QString& dy);

          void setGraphicAnchorPoint(const QString& ax, const QString& ay);

        signals:

          /*! This signal is emitted when the internal graphic element is changed. */
          void graphicChanged();

        protected:

          te::se::Graphic* m_graphic; //!< Graphic element that will be configured by this widget.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_ABSTRACTGRAPHICWIDGET_H
