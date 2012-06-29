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
  \file terralib/qt/widgets/se/GraphicDialog.cpp

  \brief A dialog used to build a graphic element.
*/

// TerraLib
#include "AbstractGraphicWidgetFactory.h"
#include "GlyphGraphicWidget.h"
#include "GraphicDialog.h"
#include "GraphicWidget.h"
#include "ui_GraphicDialogForm.h"
#include "WellKnownGraphicWidget.h"

// Qt
#include <QtGui/QStackedWidget>

te::qt::widgets::GraphicDialog::GraphicDialog(QWidget* parent, const bool noButtons, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::GraphicDialogForm)
{
  m_ui->setupUi(this);

  // Graphic Widgets
  m_graphicWidgets = new QStackedWidget(this);

  // Gets registered graphic widgets
  std::vector<std::string> keys;
  te::qt::widgets::AbstractGraphicWidgetFactory::RegisteredWidgets(keys);
  std::vector<std::string>::reverse_iterator it;
  for(it = keys.rbegin(); it != keys.rend(); ++it)
  {
    te::qt::widgets::GraphicWidget* gw = te::qt::widgets::AbstractGraphicWidgetFactory::make(*it);
    m_ui->m_graphicTypeComboBox->addItem(gw->getGraphicType());
    connect(gw, SIGNAL(graphicChanged()), SLOT(onGraphicChanged()));
    m_graphicWidgets->addWidget(gw);
  }

  // Adjusting...
  QGridLayout* layout = new QGridLayout(m_ui->m_graphicWidgetFrame);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_graphicWidgets);

  if(noButtons)
  {
    m_ui->m_okPushButton->setVisible(false);
    m_ui->m_cancelPushButton->setVisible(false);
  }

  // Signals & slots
  connect(m_ui->m_graphicTypeComboBox, SIGNAL(activated(int)), m_graphicWidgets, SLOT(setCurrentIndex(int)));
}

te::qt::widgets::GraphicDialog::~GraphicDialog()
{
}

te::se::Graphic* te::qt::widgets::GraphicDialog::getGraphic(const te::se::Graphic* initial, QWidget* parent, const QString& title)
{
  GraphicDialog dlg(parent);
  
  if(!title.isEmpty())
    dlg.setWindowTitle(title);

  if(initial)
    dlg.setGraphic(initial);

  if(dlg.exec() == QDialog::Accepted)
    return dlg.getGraphic();
  
  return 0;
}

void te::qt::widgets::GraphicDialog::setGraphic(const te::se::Graphic* graphic)
{
  int i;
  for(i = 0; i < m_graphicWidgets->count(); ++i)
  {
    te::qt::widgets::GraphicWidget* gw = static_cast<te::qt::widgets::GraphicWidget*>(m_graphicWidgets->widget(i));
    if(gw->setGraphic(graphic))
      break;
  }

  if(i == m_graphicWidgets->count())
    return; // TODO: Exception! The given graphic cannot be dealt by any registered graphic widgets.

  // Updating Ui
  m_ui->m_graphicTypeComboBox->setCurrentIndex(i);
  m_graphicWidgets->setCurrentIndex(i);
}

te::se::Graphic* te::qt::widgets::GraphicDialog::getGraphic() const
{
  te::qt::widgets::GraphicWidget* g = static_cast<te::qt::widgets::GraphicWidget*>(m_graphicWidgets->currentWidget());
  return g->getGraphic();
}

QIcon te::qt::widgets::GraphicDialog::getGraphicIcon(const QSize& size)
{
  te::qt::widgets::GraphicWidget* g = static_cast<te::qt::widgets::GraphicWidget*>(m_graphicWidgets->currentWidget());
  return g->getGraphicIcon(size);
}

void te::qt::widgets::GraphicDialog::onGraphicChanged()
{
  // Propagates the signal
  emit graphicChanged();
}
