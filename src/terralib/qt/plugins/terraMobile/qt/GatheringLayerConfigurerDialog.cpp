/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
\file src/terraMobilePlugin/qt/GatheringLayerConfigurerDialog.cpp

\brief This interface is used to get the input parameters for Gathering Layer Configurer operation.
*/

// TerraLib
#include <terralib/common/progress/ProgressManager.h>
#include <terralib/common/progress/TaskProgress.h>
#include <terralib/common/STLUtils.h>
#include "GatheringLayerConfigurerDialog.h"
#include "ui_GatheringLayerConfigurerDialogForm.h"

// Qt
#include <QFileDialog>
#include <QMessageBox>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::qt::plugins::terramobile::GatheringLayerConfigurerDialog::GatheringLayerConfigurerDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
  m_ui(new Ui::GatheringLayerConfigurerDialogForm)
{
  // add controls
  m_ui->setupUi(this);

}

te::qt::plugins::terramobile::GatheringLayerConfigurerDialog::~GatheringLayerConfigurerDialog()
{

}

void te::qt::plugins::terramobile::GatheringLayerConfigurerDialog::setLayerList(std::list<te::map::AbstractLayerPtr> list)
{
 
}
