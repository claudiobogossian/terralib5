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
\file src/terraMobilePlugin/qt/GeoPackagePublisherDialog.cpp

\brief This interface is used to get the input parameters for GeoPackage Publisher operation.
*/

// TerraLib
#include "../../../../common/progress/ProgressManager.h"
#include "../../../../common/progress/TaskProgress.h"
#include "../../../../common/STLUtils.h"
#include "GeoPackagePublisherDialog.h"
#include "ui_GeoPackagePublisherDialogForm.h"

// Qt
#include <QFileDialog>
#include <QMessageBox>


Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::qt::plugins::terramobile::GeoPackagePublisherDialog::GeoPackagePublisherDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
  m_ui(new Ui::GeoPackagePublisherDialogForm)
{
  // add controls
  m_ui->setupUi(this);

}

te::qt::plugins::terramobile::GeoPackagePublisherDialog::~GeoPackagePublisherDialog()
{

}

void te::qt::plugins::terramobile::GeoPackagePublisherDialog::setLayerList(std::list<te::map::AbstractLayerPtr> list)
{

}
