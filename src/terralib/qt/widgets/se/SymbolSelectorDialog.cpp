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
  \file terralib/qt/widgets/se/SymbolSelectorDialog.cpp

  \brief A dialog used to select a specific symbol.
*/

// TerraLib
#include "Symbol.h"
#include "SymbolLibrary.h"
#include "SymbologyPreview.h"
#include "SymbolSelectorDialog.h"
#include "ui_SymbolSelectorDialogForm.h"

// STL
#include <cassert>

te::qt::widgets::SymbolSelectorDialog::SymbolSelectorDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::SymbolSelectorDialogForm)
{
  // Setup
  m_ui->setupUi(this);
  m_ui->m_symbolListWidget->setIconSize(QSize(32, 32));

  initialize();
}

te::qt::widgets::SymbolSelectorDialog::~SymbolSelectorDialog()
{
}

te::qt::widgets::Symbol* te::qt::widgets::SymbolSelectorDialog::getSymbol(QWidget* parent, const QString& title)
{
  SymbolSelectorDialog dlg(parent);
  
  if(!title.isEmpty())
    dlg.setWindowTitle(title);
  
  if(dlg.exec() == QDialog::Accepted)
    return dlg.getSymbol();
  
  return 0;
}

te::qt::widgets::Symbol* te::qt::widgets::SymbolSelectorDialog::getSymbol() const
{
  QList<QListWidgetItem*> selected = m_ui->m_symbolListWidget->selectedItems();
  if(selected.empty())
    return 0;

  QListWidgetItem* selectedItem = selected.at(0);
  QString id = selectedItem->data(Qt::UserRole).toString();
  assert(!id.isEmpty());

  Symbol* symbol = SymbolLibrary::getInstance().findById(id.toStdString());
  assert(symbol);

  return symbol->clone();
}

void te::qt::widgets::SymbolSelectorDialog::initialize()
{
  // Gets the loaded symbols
  std::pair<std::map<std::string, Symbol*>::const_iterator,
            std::map<std::string, Symbol*>::const_iterator> iterators = SymbolLibrary::getInstance().getIterator();
  
  std::map<std::string, Symbol*>::const_iterator it;
  for(it = iterators.first; it != iterators.second; ++it) // for each Symbol
  {
    // The current symbol
    Symbol* symbol = it->second;

    // Bulding the Qt symbol item
    QListWidgetItem* item = new QListWidgetItem(m_ui->m_symbolListWidget);
    item->setText(symbol->getInfo().m_name.c_str());
    item->setIcon(SymbologyPreview::build(symbol->getSymbolizers(), m_ui->m_symbolListWidget->iconSize()));
    item->setToolTip(formatSymbolInfo(symbol->getInfo()));
    item->setData(Qt::UserRole, QVariant(QString(symbol->getInfo().m_id.c_str())));
  }
}

QString te::qt::widgets::SymbolSelectorDialog::formatSymbolInfo(const te::qt::widgets::SymbolInfo& info) const
{
  QString information("<h3>Symbol Information</h3><ul>");
  information += "<li><b>Name: </b>" + QString(info.m_name.c_str()) + "</li>";
  information += "<li><b>Author: </b>" + QString(info.m_author.c_str()) + "</li>";
  information += "<li><b>Category: </b>" + QString(info.m_category.c_str()) + "</li>";
  information += "<li><b>Tags: </b>" + QString(info.m_tags.c_str()) + "</li>";
  information += "<li><b>Description: </b>" + QString(info.m_description.c_str()) + "</li>";
  information += "</ul>";

  return information;
}
