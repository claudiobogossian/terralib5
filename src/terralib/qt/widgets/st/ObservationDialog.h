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
  \file terralib/qt/widgets/charts/ObservationDialog.h

  \brief A dialog used to define the basic parameters of a new ObservationLayer.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_OBSERVATIONDIALOG_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_OBSERVATIONDIALOG_H

//TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// Qt
#include <QDialog>

#include <memory>

namespace Ui { class ObservationDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {

    //forward declarations
    class TemporalPropertiesWidget;
    class DoubleListWidget;

      /*!
        \class ObservationDialog

        \brief  A Dialog used to generate a new ObservationLayer
      */
      class TEQTWIDGETSEXPORT ObservationDialog : public QDialog
      {

        Q_OBJECT

        public:

          /*!
            \brief Constructor

            \param parent this widget's parent
            \param f Window flags used to configure this dialog
          */
          ObservationDialog(te::map::AbstractLayerPtr layer, QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*!
            \brief Destructor
          */
          ~ObservationDialog();

        protected slots:

          void onOkPushButtonClicked();
          void onHelpPushButtonClicked();

        private:

          std::auto_ptr<Ui::ObservationDialogForm>  m_ui;                  //!< The dialog form.
          std::auto_ptr<TemporalPropertiesWidget>   m_propertiesWidget;    //!< The widget used to configure the ObservationLayer's properties.
          std::auto_ptr<DoubleListWidget>           m_observedPropWidget;  //!< The widget used to configure the layer's observed properties.
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_OBSERVATIONDIALOG_H