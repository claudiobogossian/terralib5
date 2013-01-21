/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

    This file is part of TerraView - A Free and Open Source GIS Application.

    TerraView is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraView is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib Code Editor. See COPYING. If not, write to
    TerraLib Team at <terralib-team@dpi.inpe.br>.
 */

/*!
  \file terraview/TerraView.cpp

  \brief The main class of TerraView.
*/

// TerraView
#include "AboutDialog.h"
#include "TerraView.h"

// STL
#include <memory>

// Qt
#include <QtGui/QAction>
#include <QtGui/QMenu>

TerraView::TerraView(QWidget* parent)
  : te::qt::af::BaseApplication(parent)
{
  QAction* helpAbout = new QAction(QIcon::fromTheme("terraview"), tr("&About..."), this);

  //getMenu()->addAction(helpAbout);

  connect(helpAbout, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
}

TerraView::~TerraView()
{
}

//void TerraView::init(const std::string& configFile)
//{
//}

void TerraView::showAboutDialog()
{
  std::auto_ptr<AboutDialog> dialog(new AboutDialog(this));

  dialog->exec();
}

