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
  \file terralib/qt/widgets/se/BasicFillWidget.h

  \brief A widget used to build a basic fill element.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_BASICFILLWIDGET_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_BASICFILLWIDGET_H

// TerraLib
#include "../Config.h"
#include "AbstractFillWidget.h"

// STL
#include <memory>

// Forward declaraion
namespace Ui { class BasicFillWidgetForm; }

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
      class ColorPickerToolButton;

      /*!
        \class BasicFillWidget

        \brief A widget used to build a basic fill element.

        \sa AbstractFillWidget, GraphicFillWidget
      */
      class TEQTWIDGETSEXPORT BasicFillWidget : public AbstractFillWidget
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a basic fill widget which is a child of parent, with widget flags set to f. */
          BasicFillWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~BasicFillWidget();

          //@}

        public:

          /** @name Re-implementation of Pure Virtual Method
          *   AbstractFillWidget methods.
          */
          //@{

          bool setFill(const te::se::Fill* fill);

          te::se::Fill* getFill() const;

          QString getFillType() const;

          //@}

        protected:

          /*! \brief Internal method to initialize the widget (e.g.: color, combos, icons, etc.) */
          void initialize();

          /*! \brief Updates the widget form based on internal fill element. */
          void updateUi();

          /*! \brief Updates the widget form element used to visualize the fill color. */
          void updateUiFillColor();

        protected slots:

          void onColorChanged(const QColor& color);

          void onFillOpacitySliderValueChanged(int value);

        signals:

          /*! This signal is emitted when the internal fill element is changed. */
          void fillChanged();

        private:

          std::auto_ptr<Ui::BasicFillWidgetForm> m_ui; //!< Widget form.
          ColorPickerToolButton* m_colorPicker;        //!< Widget used to pick a color.
          QColor m_color;                              //!< Auxiliary color attribute.
          te::se::Fill* m_fill;                        //!< Fill element that will be configured by this widget.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_BASICFILLWIDGET_H
