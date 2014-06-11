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
  \file terralib/qt/widgets/st/TrajectoryDialog.h

  \brief A dialog used to define the basic parameters of a new TrajectoryLayer.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_TRAJECTORYDIALOG_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_TRAJECTORYDIALOG_H

//TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// Qt
#include <QDialog>

//STL
#include <memory>

namespace Ui { class TrajectoryDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {

    //forward declarations
    class TemporalPropertiesWidget;
    class TrajectoryPropertiesWidget;

      /*!
        \class TrajectoryDialog

        \brief A Dialog used to generate a new TrajectoryLayer
      */
      class TEQTWIDGETSEXPORT TrajectoryDialog : public QDialog
      {

        Q_OBJECT

        public:

          /*!
            \brief Constructor

            \param parent this widget's parent
            \param f Window flags used to configure this dialog
          */
          TrajectoryDialog(te::map::AbstractLayerPtr layer, QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*!
            \brief Destructor
          */
          ~TrajectoryDialog();

        protected slots:

          void onOkPushButtonClicked();
          void onHelpPushButtonClicked();

        private:

          std::auto_ptr<Ui::TrajectoryDialogForm>    m_ui;                //!< The dialog form.
          std::auto_ptr<TemporalPropertiesWidget>    m_propertiesWidget;  //!< The widget used to configure the general TrajectoryLayer's properties
          std::auto_ptr<TrajectoryPropertiesWidget>  m_uniquePropWidget;  //!< The widget used to configure the unique TrajectoryLayer's properties
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_TRAJECTORYDIALOG_H