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
  \file terralib/qt/widgets/colorbar/ColorCatalogWidget.h

  \brief ...
*/

#ifndef __TERRALIB_QT_WIDGETS_COLORBAR_INTERNAL_COLORCATALOGWIDGET_H
#define __TERRALIB_QT_WIDGETS_COLORBAR_INTERNAL_COLORCATALOGWIDGET_H

// TerraLib
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QtGui/QWidget>


namespace Ui { class ColorCatalogWidgetForm; }

namespace te
{
  // Forward declarations
  namespace color { class ColorBar; }

  namespace qt
  {
    namespace widgets
    {
      // Forward declarations
      namespace colorbar { class ColorBar; }

      /*!
        \class ColorCatalogWidget

        \brief ...
      */
      class TEQTWIDGETSEXPORT ColorCatalogWidget : public QWidget
      {
        Q_OBJECT

        public:

          ColorCatalogWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~ColorCatalogWidget();

        public:

          te::qt::widgets::colorbar::ColorBar* getColorBar();

        protected:

          void loadDefaultColorCatalog();

        protected slots:

          void onCatalogComboBoxActivated(int index);

          void onGroupComboBoxActivated(int index);

          void onSchemaComboBoxActivated(int index);

          void onUseCatalogGroupBoxClicked(bool flag);

        signals:

          void colorBarChanged();

        private:

          std::auto_ptr<Ui::ColorCatalogWidgetForm> m_ui;

          te::qt::widgets::colorbar::ColorBar* m_colorBar;          //!< Widget used to represent a color bar
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_COLORBAR_INTERNAL_COLORCATALOGWIDGET_H

