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
  \file terralib/qt/plugins/layout/LayoutEditorAction.cpp

  \brief This file defines the LayoutEditor class
*/

// Terralib
#include "../../../qt/widgets/layout/LayoutEditor.h"
#include "../../af/ApplicationController.h"
#include "../../af/Project.h"
#include "LayoutEditorAction.h"
#include "../../../layout/qt/include/main/MainLayout.h"

// Qt
#include <QtCore/QObject>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>

// STL
#include <memory>

te::qt::plugins::layout2::LayoutEditorAction::LayoutEditorAction(QMenu* menu)
  : te::qt::plugins::layout2::AbstractAction(menu),
  m_layout(0)
{
  createAction(tr("Layout Editor 2...").toStdString());
}

te::qt::plugins::layout2::LayoutEditorAction::~LayoutEditorAction()
{
  if(m_layout)
    delete m_layout;
}

void te::qt::plugins::layout2::LayoutEditorAction::onActionActivated(bool checked)
{
  te::layout::MainLayout::getInstance().init();
}
