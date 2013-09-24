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

// Qt
#include <QtCore/QObject>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>

// STL
#include <memory>

te::qt::plugins::layout::LayoutEditorAction::LayoutEditorAction(QMenu* menu)
  : te::qt::plugins::layout::AbstractAction(menu),
  m_layoutEditor(0)
{
  createAction(tr("Layout Editor...").toStdString());
}

te::qt::plugins::layout::LayoutEditorAction::~LayoutEditorAction()
{
  if(m_layoutEditor)
    delete m_layoutEditor;
}

void te::qt::plugins::layout::LayoutEditorAction::onActionActivated(bool checked)
{
  if(m_layoutEditor == 0)
  {
    m_layoutEditor = new te::qt::widgets::LayoutEditor();
    m_layoutEditor->show();

    // ache o parent...
    QMenu* menu = te::qt::af::ApplicationController::getInstance().getMenu("Help");
    QMenuBar* menubar = (QMenuBar*)menu->parent();
    QWidget* parent = (QWidget*)menubar->parent();

    Qt::WindowFlags flags = 	m_layoutEditor->windowFlags();
    m_layoutEditor->setParent(parent, flags);
  }
  m_layoutEditor->show();
}
