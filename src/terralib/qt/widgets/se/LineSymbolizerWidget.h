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
  \file terralib/qt/widgets/se/LineSymbolizerWidget.h

  \brief A widget used to build a line symbolizer element.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_LINESYMBOLIZERWIDGET_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_LINESYMBOLIZERWIDGET_H

// TerraLib
#include "../Config.h"

// Qt
#include <QWidget>

// STL
#include <memory>

// Forward declaraion
namespace Ui { class LineSymbolizerWidgetForm; }

namespace te
{
// Forward declarations
  namespace se
  {
    class LineSymbolizer;
    class Symbolizer;
  }

  namespace qt
  {
    namespace widgets
    {
// Forward declarations
      class BasicStrokeWidget;

      /*!
        \class LineSymbolizerWidget

        \brief A widget used to build a line symbolizer element.
      */
      class TEQTWIDGETSEXPORT LineSymbolizerWidget : public QWidget
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a line symbolizer widget which is a child of parent, with widget flags set to f. */
          LineSymbolizerWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~LineSymbolizerWidget();

          //@}

        public:

          /*!
            \brief Sets a line symbolizer element to this widget.

            \param symb A valid line symbolizer element.

            \note The widget will NOT take the ownership of the given symbolizer.
            \note The widget form will be update based on given symbolizer parameters.
          */
          void setSymbolizer(const te::se::LineSymbolizer* symb);

          /*!
            \brief Gets the configured line symbolizer element.

            \return The configured line symbolizer element.
            
            \note The caller will take the ownership of the returned symbolizer.
          */
          te::se::Symbolizer* getSymbolizer() const;

        protected:

          /*! \brief Updates the widget form based on internal line symbolizer element. */
          void updateUi();

        protected slots:

          void onStrokeChanged();

          void onStrokeGroupBoxToggled(bool on);

        signals:

          /*! This signal is emitted when the internal line symbolizer element is changed. */
          void symbolizerChanged();

        private:

          std::auto_ptr<Ui::LineSymbolizerWidgetForm> m_ui;    //!< Widget form.
          te::qt::widgets::BasicStrokeWidget* m_strokeWidget;  //!< Basic Stroke Widget used to configure the line symbolizer stroke element.
          te::se::LineSymbolizer* m_symb;                      //!< Line symbolizer element that will be configured by this widget.
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_LINESYMBOLIZERWIDGET_H
