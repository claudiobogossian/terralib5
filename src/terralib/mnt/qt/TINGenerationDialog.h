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
\file terralib/mnt/qt/TINGenerationDialog.h

\brief A dialog TIN generation
*/
#ifndef __TERRALIB_MNT_INTERNAL_TINGENERATIONDIALOG_H
#define __TERRALIB_MNT_INTERNAL_TINGENERATIONDIALOG_H

// Terralib
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../maptools/AbstractLayer.h"
#include "../core/Config.h"

// STL
#include <list>
#include <map>
#include <memory>

// Qt
#include <QDialog>

namespace Ui { class TINGenerationDialogForm; }

namespace te
{
  namespace mnt
  {
    class TEMNTEXPORT TINGenerationDialog : public QDialog
    {
      Q_OBJECT

    public:
      TINGenerationDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

      ~TINGenerationDialog();

      /*!
      \brief Set the layer that can be used
      \param layers   List of AbstractLayerPtr
      */
      void setLayers(std::list<te::map::AbstractLayerPtr> layers);

      protected slots:

      void onIsolinesComboBoxChanged(int index);
      void onSamplesComboBoxChanged(int index);
      void onScalePushButtonClicked();
      void onYesToggled();
      void onNoToggled();
      void onTargetDatasourceToolButtonPressed();
      void onTargetFileToolButtonPressed();
      void onHelpPushButtonClicked();
      void onOkPushButtonClicked();
      void onCancelPushButtonClicked();

    private:

      std::auto_ptr<Ui::TINGenerationDialogForm> m_ui;
      te::da::DataSourceInfoPtr m_outputDatasource;                                     //!< DataSource information.
      std::string m_outputArchive;                                                      //!< Archive information.
      bool m_toFile;
      std::list<te::map::AbstractLayerPtr> m_layers;                                    //!< List of layers.
      te::map::AbstractLayerPtr m_isolinesLayer;                                        //!< Isolines layer
      te::map::AbstractLayerPtr m_samplesLayer;                                        //!< Points layer
      bool m_hasiso;
      bool m_hassample;
      double m_scale;
      double m_tol;
      double m_breaktol;
      double m_distance;
      double m_edgeSize;
    };
  }
}

#endif
