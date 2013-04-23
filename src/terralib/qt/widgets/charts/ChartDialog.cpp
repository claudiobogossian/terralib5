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
  \file terralib/qt/widgets/charts/ChartDialog.cpp

  \brief A dialog used to display a set of charts.
*/

//Terralib
#include "ChartDisplay.h"
#include "ChartDialog.h"
#include "../../../dataaccess.h"
#include "../../../datatype/Property.h"
#include "ui_histogramDialogForm.h"

//QT
#include <QtGui/QDialog>

te::qt::widgets::ChartDialog::ChartDialog(te::da::DataSet* dataSet, QWidget* parent,  Qt::WindowFlags f){}

te::qt::widgets::ChartDialog::~ChartDialog(){}

void te::qt::widgets::ChartDialog::onSettingsToolButtonClicked(){}