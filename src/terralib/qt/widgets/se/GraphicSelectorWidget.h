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
  \file terralib/qt/widgets/se/GraphicSelectorWidget.h

  \brief A widget used to select a graphic element.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_GRAPHICSELECTORWIDGET_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_GRAPHICSELECTORWIDGET_H

// TerraLib
#include "../Config.h"

// Qt
#include <QWidget>

// STL
#include <memory>
#include <string>
#include <vector>

// Forward declaraion
class QStackedWidget;
namespace Ui { class GraphicSelectorWidgetForm; }

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
        \class GraphicSelectorWidget

        \brief A widget used to select a graphic element.
      */
      class TEQTWIDGETSEXPORT GraphicSelectorWidget : public QWidget
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a graphic selector widget dialog which is a child of parent, with widget flags set to f.  */
          GraphicSelectorWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~GraphicSelectorWidget();

          //@}

        public:

          /*!
            \brief Sets the graphic element.

            \return The graphic element.
            
            \note The widget will NOT take the ownership of the given graphic.
          */
          void setGraphic(const te::se::Graphic* graphic);

          /*!
            \brief Gets the configured graphic element.

            \return The configured graphic element.
            
            \note The caller will take the ownership of the returned graphic.
          */
          te::se::Graphic* getGraphic() const;

          /*!
            \brief Gets the current graphic icon.

            \param size The icon size that will be generated.

            \return An icon that represents the graphic. i.e. a simple preview.
          */
          QIcon getGraphicIcon(const QSize& size);

        protected slots:

          void onGraphicTypeComboBoxCurrentIndexChanged(int index);

          void onGraphicChanged();

        signals:

          /*! This signal is emitted when the graphic element is changed. */
          void graphicChanged();

        private:

          std::auto_ptr<Ui::GraphicSelectorWidgetForm> m_ui; //!< Widget form.
          QStackedWidget* m_graphicWidgets;                  //!< Set of Graphic Widgets that can be used to configure the graphic element.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_GRAPHICSELECTORWIDGET_H
