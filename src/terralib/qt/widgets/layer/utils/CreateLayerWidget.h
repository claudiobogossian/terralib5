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
\file terralib/qt/widgets/layer/utils/CreateLayerWidget.h

\brief This interface is used to create new layer operation.
*/

#ifndef __TERRALIB_QT_WIDGETS_LAYER_UTILS_INTERNAL_CREATELAYERWIDGET_H
#define __TERRALIB_QT_WIDGETS_LAYER_UTILS_INTERNAL_CREATELAYERWIDGET_H

// TerraLib
#include "../../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../../datatype/SimpleProperty.h"
#include "../../../../maptools/AbstractLayer.h"
#include "../../Config.h"

//STL
#include <memory>

// Qt
#include <QAction>
#include <QMenu>
#include <QSignalMapper>
#include <QWidget>

namespace Ui { class CreateLayerWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class NewPropertyWidget;

      /*!
        \class CreateLayerWidget

        \brief This interface is used to create new layer operation.
      */
      class TEQTWIDGETSEXPORT CreateLayerWidget : public QWidget
      {
        Q_OBJECT

        public:

          /*! Default Constructor */
          CreateLayerWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! Default Destructor */
          ~CreateLayerWidget();

        public:

          void setDataSource(te::da::DataSourceInfoPtr dsInfo);

          bool createLayer(std::string& errorMessage);

        protected slots:

          void onAddPushButtonClicked();

          void onTargetDatasourceToolButtonPressed();

          void onTargetFileToolButtonPressed();

          void onRemoveAttrExecuted(int row);

        protected:

          void listProperties();

        private:

          std::auto_ptr<Ui::CreateLayerWidgetForm> m_ui;

          te::qt::widgets::NewPropertyWidget* m_newPropWidget;

          te::da::DataSourceInfoPtr m_outputDatasource;

          std::vector<te::dt::SimpleProperty*> m_props;

          bool m_toFile;

          QSignalMapper* m_removeMapper;                   //!< The mapper used to know which action was executed.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_LAYER_UTILS_INTERNAL_CREATELAYERWIDGET_H
