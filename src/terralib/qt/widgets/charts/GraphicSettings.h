/*  Copyright (C) 2010-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/charts/GraphicSettings.h

  \brief A dialog used to customize a graphic's parameters, weather it is about it's data or it's visual style
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_GRAPHICSETTINGS_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_GRAPHICSETTINGS_H

// QT
#include <QtGui/QDialog>
#include <QtGui/QListWidgetItem>

// TerraLib
#include "../Config.h"

#include <memory>

namespace Ui { class GraphicSettingsDialog; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class GraphicWidget;
      /*!
        \class GraphicSettings

        \brief A dialog used to customize a graphic's parameters.
      */
      class TEQTWIDGETSEXPORT GraphicSettings : public QDialog
      {
        Q_OBJECT

        public:

          GraphicSettings(QWidget* parent = 0);

          ~GraphicSettings();

          public slots:

            void on_itemClicked(QListWidgetItem * current);

        private:

          std::auto_ptr<Ui::GraphicSettingsDialog> m_ui;

          GraphicWidget* m_curComp;

      };

    }   // end namespace widgets
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_GRAPHICSETTINGS_H

