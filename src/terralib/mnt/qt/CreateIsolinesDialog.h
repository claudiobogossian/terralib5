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
\file terralib/mnt/qt/CreateIsolinesDialog.h

\brief A dialog ISolines generation
*/
#ifndef __TERRALIB_MNT_INTERNAL_CREATEISOLINESDIALOG_H
#define __TERRALIB_MNT_INTERNAL_CREATEISOLINESDIALOG_H

//terralib
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../maptools/AbstractLayer.h"
#include "../core/Config.h"
#include "../core/Enums.h"

// STL
#include <list>
#include <map>
#include <memory>

// Qt
#include <QDialog>

namespace Ui { class CreateIsolinesDialogForm; }

namespace te
{
  namespace mnt
  {
    class TEMNTEXPORT CreateIsolinesDialog : public QDialog
    {
      Q_OBJECT

    public:

      CreateIsolinesDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

      ~CreateIsolinesDialog();

      void setLayers(std::list<te::map::AbstractLayerPtr> layers);

      te::map::AbstractLayerPtr getLayer();

      void getMinMax(te::map::AbstractLayerPtr inputLayer, double &min, double &max);

      protected slots:

      void onInputComboBoxChanged(int index);
      void onDummyLineEditEditingFinished();
      void onStepFixeEnabled(bool);
      void on_stepVariableraEnabled(bool);
      void oninsertpushButtonClicked();
      void ondeletepushButtonClicked();
      void ondeleteallpushButtonClicked();
      void onTargetDatasourceToolButtonPressed();
      void onTargetFileToolButtonPressed();
      void onHelpPushButtonClicked();
      void onOkPushButtonClicked();
      void onCancelPushButtonClicked();

    private:

      std::auto_ptr<Ui::CreateIsolinesDialogForm> m_ui;

      te::da::DataSourceInfoPtr m_outputDatasource;                                     //!< DataSource information.
      std::string m_outputArchive;                                                      //!< Archive information.
      bool m_toFile;
      std::list<te::map::AbstractLayerPtr> m_layers;                                    //!< List of layers.
      te::map::AbstractLayerPtr m_inputLayer;                                        //!< Input layer
      te::map::AbstractLayerPtr m_outputLayer;                                          //!< Generated Layer.

      mntType m_inputType;  //!< Input type (TIN, GRID)
      double m_min;
      double m_max;
      double m_dummy;
      bool m_hasDummy = false;

    };
  } //mnt
} //te

#endif  //__TERRALIB_MNT_INTERNAL_CREATEISOLINESDIALOG_H
