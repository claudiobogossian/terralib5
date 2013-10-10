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
  \file  terralib/qt/widgets/externalTable/DataPropertiesWidget.h

  \brief A class used to configure the properties of a new textual file based layer
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_DATAPROPERTIES_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_DATAPROPERTIES_H

//TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QWidget>

namespace Ui { class dataPropertiesWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {

      /*!
        \class DatapPropertiesWidget

        \brief A class to represent a chart display
      */
      class TEQTWIDGETSEXPORT DatapPropertiesWidget : public QWidget
      {

        Q_OBJECT

        public:

      /*!
            \brief Constructor

            It constructs a DatapPropertiesWidget.

            \param parent this widget's parent
            \param f Window flags used to configure this widget

          */
          DatapPropertiesWidget(QWidget* parent = 0,  Qt::WindowFlags f = 0);

          /*!
            \brief Destructor
          */
          ~DatapPropertiesWidget();

        //protected slots:

          void onInputDataToolButtonTriggered();

        private:

          std::auto_ptr<Ui::dataPropertiesWidgetForm>  m_ui;  //!< The widget's form.
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_DATAPROPERTIES_H