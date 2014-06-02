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
#include <QDialog>
#include <QListWidgetItem>

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

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT ChartProperties : public QDialog
      {
        Q_OBJECT

        public:

          /*!
            \brief Constructor

            \param chartWidget The widget that will be editted by this dialog.
            \param parent The dialog's parent widget.
          */
          ChartProperties(te::qt::widgets::ChartDisplayWidget* chartWidget, QWidget* parent = 0);

          /*! 
            \brief Destructor.
          */
          ~ChartProperties();

          public slots:

          /*!
            \brief Called when the user clicks on one of the items available at the list.

            \param current The currently selected item of the list, it will be used to determine which type of chart the user wishes to configure.
          */
            void onItemClicked(QListWidgetItem * current);

          /*!
            \brief Called when the user clicks on the apply button.
                   It will set the configurations to the ChartDisplayWidget being configured by this dialog.
          */
            void onApplyButtonClicked();

        private:

          std::auto_ptr<Ui::ChartPropertiesDialogForm>  m_ui;           // The Dialog Form.
          ChartWidget*                                  m_curComp;      // The widget being displayed
          ChartDisplayWidget*                           m_chartWidget;  // The chart widget that will be configured by this dialog.

      };
    }   // end namespace widgets
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_CHARTPROPERTIES_H

