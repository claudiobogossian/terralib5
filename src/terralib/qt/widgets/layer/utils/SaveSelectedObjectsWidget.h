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
\file terralib/qt/widgets/layer/utils/SaveSelectedObjectsWidget.h

\brief This interface is used to create a new layer based on a layer with selected objects.
*/

#ifndef __TERRALIB_QT_WIDGETS_LAYER_UTILS_INTERNAL_SAVESELECTEDOBJECTSWIDGET_H
#define __TERRALIB_QT_WIDGETS_LAYER_UTILS_INTERNAL_SAVESELECTEDOBJECTSWIDGET_H

// TerraLib
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../../datatype/SimpleProperty.h"
#include "../../../../maptools/AbstractLayer.h"
#include "../../Config.h"

// Qt
#include <QAction>
#include <QMenu>
#include <QSignalMapper>
#include <QWidget>

namespace Ui { class SaveSelectedObjectsWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class CreateLayerWidget

        \brief This interface is used to create a new layer based on a layer with selected objects.
      */
      class TEQTWIDGETSEXPORT SaveSelectedObjectsWidget : public QWidget
      {
        Q_OBJECT

        public:

          /*! Default Constructor */
          SaveSelectedObjectsWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! Default Destructor */
          ~SaveSelectedObjectsWidget();

        public:
          
          /*! \brief Set parameters. */
          void setParameters(te::map::AbstractLayerPtr layer);

          /*! \brief Update components with imput data. */
          void updateWindowComponents();

          void save(te::da::DataSource* dataSource, te::da::DataSet* dataSet, te::da::DataSetType* dataSetType);

          te::map::AbstractLayerPtr getLayer();

          bool execute(std::string& errorMessage);

        protected slots:

          void onTargetDatasourceToolButtonPressed();

          void onTargetFileToolButtonPressed();

        private:

          std::auto_ptr<Ui::SaveSelectedObjectsWidgetForm> m_ui;

          te::map::AbstractLayerPtr m_layer;                  //!< TerraLib layer auto ptr

          int m_srid;                                         //!< SRID of in/out layer

          const te::da::ObjectIdSet* m_oidSet;                //!< Object Id Set.

          te::da::DataSourceInfoPtr m_outputDatasource;

          te::map::AbstractLayerPtr m_layerResult;

          bool m_toFile;

          QSignalMapper* m_removeMapper;                      //!< The mapper used to know which action was executed.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_LAYER_UTILS_INTERNAL_SAVESELECTEDOBJECTSWIDGET_H
