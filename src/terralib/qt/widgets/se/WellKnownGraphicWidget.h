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
  \file terralib/qt/widgets/se/WellKnownGraphicWidget.h

  \brief A widget used to build a graphic associate with a well-known mark element.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_WELLKNOWNGRAPHICWIDGET_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_WELLKNOWNGRAPHICWIDGET_H

// TerraLib
#include "../Config.h"
#include "AbstractGraphicWidget.h"

// STL
#include <memory>

// Forward declaraion
namespace Ui { class WellKnownGraphicWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
// Forward declarations
      class WellKnownMarkWidget;

      /*!
        \class WellKnownGraphicWidget

        \brief A widget used to build a graphic associate with a well-known mark element.
      */
      class TEQTWIDGETSEXPORT WellKnownGraphicWidget : public AbstractGraphicWidget
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a well known graphic widget which is a child of parent, with widget flags set to f. */
          WellKnownGraphicWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~WellKnownGraphicWidget();

          //@}

        public:

          /** @name Re-implementation of Pure Virtual Method
          *   AbstractGraphicWidget methods.
          */
          //@{

          bool setGraphic(const te::se::Graphic* graphic);

          QString getGraphicType() const;

          QIcon getGraphicIcon(const QSize& size) const;

          //@}

        protected slots:

          void onMarkChanged();

          void onGraphicDisplacementChanged(const QString& text);

          void onGraphicAnchorPointChanged(const QString& text);

        private:

          /*! \brief Updates the widget form based on internal graphic element. */
          void updateUi();

        private:

          std::auto_ptr<Ui::WellKnownGraphicWidgetForm> m_ui; //!< Widget form.
          te::qt::widgets::WellKnownMarkWidget* m_markWidget; //!< Well known mark Widget used to configure the mark element.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_WELLKNOWNGRAPHICWIDGET_H
