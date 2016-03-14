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
  \file terralib/vp/qt/DifferenceDialog.h

  \brief Difference operation dialog.
*/

#ifndef __TERRALIB_VP_QT_INTERNAL_DIFFERENCEDIALOG_H
#define __TERRALIB_VP_QT_INTERNAL_DIFFERENCEDIALOG_H

// TerraLib
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../maptools/AbstractLayer.h"
#include "../AlgorithmParams.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QDialog>

namespace Ui { class DifferenceDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class DoubleListWidget;
    }
  }

  namespace vp
  {
    /*!
      \class DifferenceDialog

      \brief A dialog used to execute vector difference.
    */
    class TEVPEXPORT DifferenceDialog : public QDialog
    {
      Q_OBJECT

      public:

        DifferenceDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

        ~DifferenceDialog();

        /*!
          \brief Set the layer that can be used

          \param layers   List of AbstractLayerPtr
        */
        void setLayers(std::list<te::map::AbstractLayerPtr> layers);

        te::map::AbstractLayerPtr getLayer();

      protected:

        te::da::Select* getSelectQueryFromLayer(te::map::AbstractLayerPtr layer, bool onlySelectedObjects);

        te::da::DataSet* getDataSetFromLayer(te::map::AbstractLayerPtr layer, bool onlySelectedObjects);

        std::vector<std::pair<std::string, std::string> > getSelectedProperties();

        void updateInputLayerComboBox();

        void updateDifferenceLayerComboBox();

        void updateDoubleListWidget();

      protected slots:

        void onInputLayerComboBoxChanged(int index);

        void onDifferenceLayerComboBoxChanged(int index);

        void onTargetDatasourceToolButtonPressed();

        void onTargetFileToolButtonPressed();

        void onOkPushButtonClicked();
        
        void onCancelPushButtonClicked();

        

      private:

        std::auto_ptr<Ui::DifferenceDialogForm> m_ui;
        std::list<te::map::AbstractLayerPtr> m_layers;        //!< The vector layers in Layer Explorer.
        te::map::AbstractLayerPtr m_inputSelectedLayer;       //!< Input layer selected.
        te::map::AbstractLayerPtr m_differenceSelectedLayer;  //!< Difference layer selected.

        std::vector<te::vp::InputParams> m_inputParams;       //!< A vector of input parameters.
        te::vp::AlgorithmParams* m_params;                    //!< Algorithm parameters.

        te::da::DataSourceInfoPtr m_outputDatasource;         //!< DataSource information.
        te::map::AbstractLayerPtr m_layerResult;              //!< Generated Layer.
        
        bool m_toFile;                                        //!< The result is in a file?

        std::auto_ptr<te::qt::widgets::DoubleListWidget> m_doubleListWidget;
    };
  }   // end namespace vp
}     // end namespace te

#endif  // __TERRALIB_VP_QT_INTERNAL_DIFFERENCEDIALOG_H
