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
  \file terralib/qt/widgets/se/GraphicWidget.h

  \brief A base widget used to configure a graphic.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_GRAPHICWIDGET_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_GRAPHICWIDGET_H

// TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QWidget>

// STL
#include <memory>

// Forward declaraion
namespace Ui { class GraphicWidgetForm; }

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
        \class GraphicWidget

        \brief A base widget used to configure a graphic.

        \sa ExternalGraphicWidget, GlyphGraphicWidget, WellKnownGraphicWidget
      */
      class TEQTWIDGETSEXPORT GraphicWidget : public QWidget
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a graphic widget which is a child of parent, with widget flags set to f. */
          GraphicWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          virtual ~GraphicWidget();

          //@}

        public:

          virtual bool setGraphic(const te::se::Graphic* graphic);

          te::se::Graphic* getGraphic() const;

          /*!
            \brief Pure virtual method that should return a "user friendly" string
                   that informs the graphic type that can be built by the widget.
          */
          virtual QString getGraphicType() const = 0;

          /*! \brief Pure virtual method that should return a QIcon that represents the graphic. i.e. a simple preview. */
          virtual QIcon getGraphicIcon(const QSize& size) const = 0;

        protected:

          /*! \brief Updates the widget form based on internal graphic element. */
          virtual void updateUi();

        protected slots:

          void onGraphicSizeDoubleSpinBoxValueChanged(const QString& text);

          void onGraphicAngleDoubleSpinBoxValueChanged(const QString& text);

          void onGraphicOpacitySliderValueChanged(int value);

          void onGraphicDisplacementChanged(const QString& text);

          void onGraphicAnchorPointChanged(const QString& text);

        signals:

          /*! This signal is emitted when the internal graphic element is changed. */
          void graphicChanged();

        protected:

          std::auto_ptr<Ui::GraphicWidgetForm> m_ui; //!< Widget form.
          te::se::Graphic* m_graphic;                //!< Graphic element that will be configured by this widget.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_GRAPHICWIDGET_H
