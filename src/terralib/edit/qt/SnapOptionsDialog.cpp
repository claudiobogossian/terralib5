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
  \file terralib/edit/qt/SnapOptionsWidget.cpp

  \brief A widget used to configure geometry snap options.
*/

// TerraLib
#include "SnapOptionsDialog.h"
#include "ui_SnapOptionsDialogForm.h"

// STL
#include <cassert>

te::edit::SnapOptionsWidget::SnapOptionsWidget(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::SnapOptionsDialogForm)
{
  m_ui->setupUi(this);
}

te::edit::SnapOptionsWidget::~SnapOptionsWidget()
{
}
