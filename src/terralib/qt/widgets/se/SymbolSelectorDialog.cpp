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
#include "../../../common/Exception.h"
#include "../../../serialization/qt/widgets/Symbol.h"
#include "Symbol.h"
#include "SymbolInfoDialog.h"
#include "SymbolLibrary.h"
#include "SymbolLibraryManager.h"
#include "SymbolPreviewWidget.h"
#include "SymbologyPreview.h"
#include "SymbolSelectorDialog.h"
#include "ui_SymbolSelectorDialogForm.h"

// Qt
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

// STL
#include <cassert>

te::qt::widgets::SymbolSelectorDialog::SymbolSelectorDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::SymbolSelectorDialogForm)
{
  // Setup
  m_ui->setupUi(this);
  m_ui->m_symbolLibraryTreeWidget->setIconSize(QSize(32, 32));

  // Preview
  m_preview = new SymbolPreviewWidget(QSize(120, 120), this);

  // Adjusting...
  QGridLayout* previewLayout = new QGridLayout(m_ui->m_previewGroupBox);
  previewLayout->addWidget(m_preview);

  // Signals & slots
  connect(m_ui->m_symbolLibraryTreeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), SLOT(onCurrentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
  connect(m_ui->m_showSymbolInfoPushButton, SIGNAL(pressed()), SLOT(onShowSymbolInfoPushButtonPressed()));
  connect(m_ui->m_loadSymbolLibraryPushButton, SIGNAL(pressed()), SLOT(onLoadSymbolLibraryPushButtonPressed()));
  connect(m_ui->m_searchLineEdit, SIGNAL(textChanged(const QString&)), SLOT(onSearchLineEditTextChanged(const QString&)));

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
  Symbol* symbol = getSelectedSymbol();

  if(symbol == 0)
    return 0;

  return symbol->clone();
}

void te::qt::widgets::SymbolSelectorDialog::onCurrentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
  Symbol* symbol = getSymbolFromItem(current);

  if(symbol == 0)
    return;

  m_preview->updatePreview(symbol);
}

void te::qt::widgets::SymbolSelectorDialog::onShowSymbolInfoPushButtonPressed()
{
  Symbol* symbol = getSelectedSymbol();

  if(symbol == 0)
    return;

  SymbolInfoDialog info;
  info.setSymbolInfo(symbol->getInfo());
  info.setReadMode();

  info.exec();
}

void te::qt::widgets::SymbolSelectorDialog::onLoadSymbolLibraryPushButtonPressed()
{
  QString path = QFileDialog::getOpenFileName(this, tr("Select a TerraLib Symbol Library File"), "", "XML (.xml)");
  if(path.isNull())
    return;

  try
  {
     /*te::serialize::ReadSymbolLibrary(path.toStdString());
     initialize();*/
  }
  catch(te::common::Exception& e)
  {
    QString message = tr("The selected symbol library could not be loaded.\n Details: )");
    message += e.what();
    QMessageBox::critical(this, tr("Error"), message);
  }
}

void te::qt::widgets::SymbolSelectorDialog::onSearchLineEditTextChanged(const QString& text)
{
  QList<QTreeWidgetItem*> items = m_ui->m_symbolLibraryTreeWidget->findItems(text, Qt::MatchContains | Qt::MatchRecursive, 0);
  items.append(m_ui->m_symbolLibraryTreeWidget->findItems(text, Qt::MatchContains | Qt::MatchRecursive, 1));
  items.append(m_ui->m_symbolLibraryTreeWidget->findItems(text, Qt::MatchContains | Qt::MatchRecursive, 2));

  filter(items);
}

void te::qt::widgets::SymbolSelectorDialog::initialize()
{
  // Gets the loaded libraries
  std::pair<std::map<std::string, SymbolLibrary*>::const_iterator,
            std::map<std::string, SymbolLibrary*>::const_iterator> iteratorsLibrary = SymbolLibraryManager::getInstance().getIterator();
  
  std::map<std::string, SymbolLibrary*>::const_iterator itLibrary;
  for(itLibrary = iteratorsLibrary.first; itLibrary != iteratorsLibrary.second; ++itLibrary) // for each library
  {
    QString libraryName = QString::fromStdString(itLibrary->second->getName());

    QTreeWidgetItem* libraryItem = new QTreeWidgetItem(m_ui->m_symbolLibraryTreeWidget, LIBRARY);
    libraryItem->setText(0, libraryName);
    libraryItem->setData(0, Qt::UserRole, libraryName);

    std::pair<std::map<std::string, Symbol*>::const_iterator,
              std::map<std::string, Symbol*>::const_iterator> iteratorsSymbol = itLibrary->second->getIterator();

    std::map<std::string, Symbol*>::const_iterator itSymbol;

    for(itSymbol = iteratorsSymbol.first; itSymbol != iteratorsSymbol.second; ++itSymbol) // for each Symbol
    {
      // The current symbol
      Symbol* symbol = itSymbol->second;

      // Bulding the Qt symbol item
      QTreeWidgetItem* symbolItem = new QTreeWidgetItem(libraryItem, SYMBOL);
      symbolItem->setText(0, symbol->getInfo().m_name.c_str());
      symbolItem->setIcon(0, SymbologyPreview::build(symbol->getSymbolizers(), m_ui->m_symbolLibraryTreeWidget->iconSize()));
      symbolItem->setToolTip(0, formatSymbolInfo(symbol->getInfo()));
      symbolItem->setData(0, Qt::UserRole, QVariant(QString(symbol->getInfo().m_id.c_str())));
      symbolItem->setText(1, symbol->getInfo().m_author.c_str());
      symbolItem->setText(2, symbol->getInfo().m_tags.c_str());
      symbolItem->setText(3, symbol->getInfo().m_description.c_str());
    }
  }

  m_ui->m_symbolLibraryTreeWidget->sortItems(0, Qt::AscendingOrder);
  m_ui->m_symbolLibraryTreeWidget->resizeColumnToContents(0);
  m_ui->m_symbolLibraryTreeWidget->expandAll();
}

void te::qt::widgets::SymbolSelectorDialog::filter(const QList<QTreeWidgetItem*>& items)
{
  for(int i = 0; i < m_ui->m_symbolLibraryTreeWidget->topLevelItemCount(); ++i)
  {
    QTreeWidgetItem* library = m_ui->m_symbolLibraryTreeWidget->topLevelItem(i);
    assert(library && library->type() == LIBRARY);

    for(int j = 0; j < library->childCount(); ++j)
    {
      QTreeWidgetItem* symbol = library->child(j);
      assert(symbol && symbol->type() == SYMBOL);
      bool hide = items.indexOf(symbol) == -1;
      symbol->setHidden(hide);
    }
  }
  update();
}

te::qt::widgets::Symbol* te::qt::widgets::SymbolSelectorDialog::getSelectedSymbol() const
{
  QList<QTreeWidgetItem*> selected = m_ui->m_symbolLibraryTreeWidget->selectedItems();

  if(selected.empty())
    return 0;

  return getSymbolFromItem(selected.at(0));
}

te::qt::widgets::Symbol* te::qt::widgets::SymbolSelectorDialog::getSymbolFromItem(QTreeWidgetItem* item) const
{
  assert(item);
  
  if(item->type() != SYMBOL)
    return 0;

  // Gets the library
  QTreeWidgetItem* parent = item->parent();
  assert(parent);

  QString name = parent->data(0, Qt::UserRole).toString();
  assert(!name.isEmpty());

  SymbolLibrary* symbolLibrary = SymbolLibraryManager::getInstance().findByName(name.toStdString());
  assert(symbolLibrary);

  // Gets the symbol
  QString id = item->data(0, Qt::UserRole).toString();
  assert(!id.isEmpty());

  Symbol* symbol = symbolLibrary->findById(id.toStdString());
  assert(symbol);

  return symbol;
}

QString te::qt::widgets::SymbolSelectorDialog::formatSymbolInfo(const te::qt::widgets::SymbolInfo& info) const
{
  QString information("<h3>Symbol Information</h3><ul>");
  information += "<li><b>Name: </b>" + QString(info.m_name.c_str()) + "</li>";
  information += "<li><b>Author: </b>" + QString(info.m_author.c_str()) + "</li>";
  information += "<li><b>Tags: </b>" + QString(info.m_tags.c_str()) + "</li>";
  information += "<li><b>Description: </b>" + QString(info.m_description.c_str()) + "</li>";
  information += "</ul>";

  return information;
}
