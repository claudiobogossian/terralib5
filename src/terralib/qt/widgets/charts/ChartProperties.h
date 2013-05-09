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
  \file terralib/qt/widgets/charts/ChartProperties.h

  \brief A dialog used to customize a graphic's parameters, weather it is about it's data or it's visual style
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_CHARTPROPERTIES_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_CHARTPROPERTIES_H

// QT
#include <QtGui/QDialog>
#include <QtGui/QListWidgetItem>

// TerraLib
#include "../Config.h"

#include <memory>

namespace Ui { class ChartPropertiesDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class ChartWidget;
      class ChartDisplayWidget;
      /*!
        \class ChartProperties

        \brief A dialog used to customize a graphic's parameters.
      */
      class TEQTWIDGETSEXPORT ChartProperties : public QDialog
      {
        Q_OBJECT

        public:

          ChartProperties(te::qt::widgets::ChartDisplayWidget* chartWidget, QWidget* parent = 0);

          ~ChartProperties();

          public slots:

            void onItemClicked(QListWidgetItem * current);
            void onApplyButtonClicked();

        private:

          std::auto_ptr<Ui::ChartPropertiesDialogForm>  m_ui;           // The Dialog Form.
          ChartWidget*                                  m_curComp;      // The widget being displayed
          te::qt::widgets::ChartDisplayWidget*          m_chartWidget;  // The chart widget that will be configured by this dialog.

      };
    }   // end namespace widgets
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_CHARTPROPERTIES_H

