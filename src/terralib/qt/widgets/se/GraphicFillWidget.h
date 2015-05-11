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
  \file terralib/qt/widgets/se/GraphicFillWidget.h

  \brief A widget used to build a fill element associated with a graphic.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_GRAPHICFILLWIDGET_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_GRAPHICFILLWIDGET_H

// TerraLib
#include "../Config.h"
#include "AbstractFillWidget.h"

// STL
#include <memory>

// Forward declaraion
namespace Ui { class GraphicFillWidgetForm; }

namespace te
{
// Forward declarations
  namespace se
  {
    class Fill;
  }

  namespace qt
  {
    namespace widgets
    {
// Forward declarations
      class GraphicDialog;

      /*!
        \class GraphicFillWidget

        \brief A widget used to build a fill element associated with a graphic.

        \sa AbstractFillWidget, BasicFillWidget
      */
      class TEQTWIDGETSEXPORT GraphicFillWidget : public AbstractFillWidget
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a graphic fill widget which is a child of parent, with widget flags set to f. */
          GraphicFillWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~GraphicFillWidget();

          //@}

        public:

          /** @name Re-implementation of Pure Virtual Method
          *  AbstractFillWidget methods.
          */
          //@{

          bool setFill(const te::se::Fill* fill);

          te::se::Fill* getFill() const;

          QString getFillType() const;

          //@}

        protected:

          /*! \brief Updates the widget form based on internal fill element. */
          void updateUi();

        protected slots:

          void onGraphicToolButtonPressed();

        signals:

          /*! This signal is emitted when the internal fill element is changed. */
          void fillChanged();

        private:

          std::auto_ptr<Ui::GraphicFillWidgetForm> m_ui; //!< Widget form.
          std::auto_ptr<GraphicDialog> m_graphicDialog;  //!< Graphic dialog used to build the graphic element.
          te::se::Fill* m_fill;                          //!< Fill element that will be configured by this widget.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_GRAPHICFILLWIDGET_H
