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
  \file terralib/qt/widgets/se/BasicStrokeWidget.h

  \brief A widget used to build a basic stroke element.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_BASICSTROKEWIDGET_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_BASICSTROKEWIDGET_H

// TerraLib
#include "../Config.h"

// Qt
#include <QWidget>

// STL
#include <memory>
#include <vector>
#include <string>

// Forward declaraion
class QButtonGroup;
namespace Ui { class BasicStrokeWidgetForm; }

namespace te
{
// Forward declarations
  namespace se
  {
    class Stroke;
  }

  namespace qt
  {
    namespace widgets
    {
// Forward declarations
      class ColorPickerToolButton;

      /*!
        \class BasicStrokeWidget

        \brief A widget used to build a basic stroke element.
      */
      class TEQTWIDGETSEXPORT BasicStrokeWidget : public QWidget
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a basic stroke widget which is a child of parent, with widget flags set to f. */
          BasicStrokeWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~BasicStrokeWidget();

          //@}

        public:

          /*!
            \brief Sets a stroke element to this widget.

            \param stroke A valid stroke element.

            \note The widget will NOT take the ownership of the given stroke.
            \note The widget form will be update based on given stroke parameters.
          */
          void setStroke(const te::se::Stroke* stroke);

          /*!
            \brief Gets the configured stroke element.

            \return The configured stroke element.
            
            \note The caller will take the ownership of the returned stroke.
          */
          te::se::Stroke* getStroke() const;

        protected:

          /*! \brief Internal method to initialize the widget (e.g.: color, combos, icons, etc.) */
          void initialize();

          /*! \brief Updates the widget form based on internal stroke element. */
          void updateUi();

          /*! \brief Updates the widget form element used to visualize the stroke color. */
          void updateUiStrokeColor();

          /*! 
            \brief Updates the widget form element used to visualize the stroke dash style. 

            \param pattern The stroke dash style pattern.
          */
          void updateUiDashStyle(const std::string& pattern);

          /*!
            \brief Updates the widget form element used to visualize the stroke join style.

            \param style The stroke join style type.
          */
          void updateUiJoinStyle(const std::string& style);

          /*!
            \brief Updates the widget form element used to visualize the stroke cap style.

            \param style The stroke cap style type.
          */
          void updateUiCapStyle(const std::string& style);

          /*! \brief Fills the widget form element used to visualize the stroke cap style. */
          void fillStrokeDashStyleComboBox();

        protected slots:

          void onColorChanged(const QColor& color);

          void onStrokeOpacitySliderValueChanged(int value);

          void onStrokeWidthDoubleSpinBoxValueChanged(const QString& text);

          void onStrokeDashComboBoxCurrentIndexChanged(int index);

          void onStrokeJoinStyleChanged(int style);

          void onStrokeCapStyleChanged(int style);

        signals:

          /*! This signal is emitted when the internal stroke element is changed. */
          void strokeChanged();

        private:

          std::auto_ptr<Ui::BasicStrokeWidgetForm> m_ui; //!< Widget form.
          ColorPickerToolButton* m_colorPicker;          //!< Widget used to pick a color.
          QButtonGroup* m_strokeJoinStyleButtonGroup;    //!< Button group used to control the join styles.
          QButtonGroup* m_strokeCapStyleButtonGroup;     //!< Button group used to control the cap styles.
          QColor m_color;                                //!< Auxiliary color attribute.
          te::se::Stroke* m_stroke;                      //!< Stroke element that will be configured by this widget.
          std::vector<std::string> m_dashes;             //!< Predefined dash styles. TODO: it can be filled out, based on a file, etc (?)
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_BASICSTROKEWIDGET_H
