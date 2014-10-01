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
  \file terralib/qt/widgets/charts/HistogramDialog.h

  \brief A dialog used to define the basic parameters of a new histogram.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMDIALOG_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMDIALOG_H

//TerraLib
#include "../Config.h"

// Qt
#include <QDialog>

//STL
#include <memory>

namespace Ui { class HistogramDialogForm; }

namespace te
{
    //forward declarations
  namespace da  { class DataSet; class DataSetType; }

  namespace qt
  {
    namespace widgets
    {

    //forward declarations
    class ChartDisplayWidget;
    class HistogramDataWidget;

      /*!
        \class HistogramDialog

        \brief A Dialog used to customize a Histogram parameters and generate it's ChartDisplayWidget.
      */
      class TEQTWIDGETSEXPORT HistogramDialog : public QDialog
      {

        Q_OBJECT

        public:

          /*!
            \brief Constructor

            \param dataSet The dataset that will be used to generate a histogram.
            \param dataType The dataset that will be used to generate a histogram.
            \param parent this widget's parent
            \param f Window flags used to configure this dialog
          */
          HistogramDialog(te::da::DataSet* dataSet, te::da::DataSetType* dataType, QWidget* parent = 0,  Qt::WindowFlags f = 0);

          /*!
            \brief Destructor
          */
          ~HistogramDialog();

          /*!
            \brief Returns a pointer to the generated ChartDisplayWidget

            \return A ChartDisplayWidget type pointer to the generated chart.
            \note The caller will take ownership of the returned pointer. 
          */
          te::qt::widgets::ChartDisplayWidget* getDisplayWidget();

        protected slots:

          void onOkPushButtonClicked();

        private:

          std::auto_ptr<Ui::HistogramDialogForm>  m_ui;                   //!< The dialog form.
          HistogramDataWidget*                    m_histogramDataWidget;  //!< The histogram's data widget used to configure the basic parameters of the new histogram.
          ChartDisplayWidget*                     m_displayWidget;        //!< The display's widget that will be populated by this widget.
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMDIALOG_H