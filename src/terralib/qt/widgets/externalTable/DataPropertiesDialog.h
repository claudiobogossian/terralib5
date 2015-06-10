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
  \file  terralib/qt/widgets/externalTable/DataPropertiesDialog.h

  \brief A dialog used to configure the properties of a new textual file based layer
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_DATAPROPERTIESDIALOG_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_DATAPROPERTIESDIALOG_H


//TerraLib
#include "../../../maptools/DataSetAdapterLayer.h"
#include "../Config.h"

// Qt
#include <QDialog>

//STL
#include <memory>

namespace Ui { class DataPropertiesDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {

    //forward declarations
    class  DatapPropertiesWidget;

      /*!
        \class DataPropertiesDialog

        \brief A dialog used to configure the properties of a new textual file based layer.
      */
      class TEQTWIDGETSEXPORT DataPropertiesDialog : public QDialog
      {

        Q_OBJECT

        public:

          /*!
            \brief Constructor

            \param parent this widget's parent
            \param f Window flags used to configure this dialog
          */
          DataPropertiesDialog(QWidget* parent = 0,  Qt::WindowFlags f = 0);

          /*!
            \brief Destructor
          */
          ~DataPropertiesDialog();

          /*!
            \brief Returns a new DataSetAdapterLayer

            \return A DataSetAdapterLayerPtr with it's DataSetAdapter.
            \note The caller will take ownership of the returned pointer. 
          */
          te::map::DataSetAdapterLayerPtr getDataSetAdapterLayer();

      protected slots:

          void onOkPushButtonClicked();

      private:

        std::auto_ptr<Ui::DataPropertiesDialogForm>  m_ui;                    //!< The dialog form.
        DatapPropertiesWidget*                       m_dataPropertiesWidget;  //!< The widget used to import and configure a textual file.
        te::map::DataSetAdapterLayerPtr              m_DataSetAdapterLayer;   //!< The new Layer.
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_DATAPROPERTIESDIALOG_H
