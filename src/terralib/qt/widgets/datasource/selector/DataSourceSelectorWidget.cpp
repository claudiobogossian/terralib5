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
  \file terralib/qt/widgets/connector/DataSourceSelectorWidget.cpp

  \brief A dialog for connecting to a data source.
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../Exception.h"
#include "../connector/AbstractDataSourceConnector.h"
#include "../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../core/DataSourceType.h"
#include "../core/DataSourceTypeManager.h"
#include "ui_DataSourceSelectorWidgetForm.h"
#include "DataSourceSelectorWidget.h"

// Qt
#include <QtGui/QIcon>
#include <QtGui/QMessageBox>

te::qt::widgets::DataSourceSelectorWidget::DataSourceSelectorWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::DataSourceSelectorWidgetForm),
    m_addButtonEnabled(true),
    m_removeButtonEnabled(true),
    m_editButtonEnabled(true),
    m_createButtonEnabled(true)
{
// add controls
  m_ui->setupUi(this);

// clear some widgets
  m_ui->m_datasourceTypeTitleLabel->clear();
  m_ui->m_datasourceDescriptionLabel->clear();

// add button icons
  m_ui->m_addDataSourceToolButton->setIcon(QIcon::fromTheme("list-add"));
  m_ui->m_removeDataSourceToolButton->setIcon(QIcon::fromTheme("list-remove"));
  m_ui->m_editDataSourceToolButton->setIcon(QIcon::fromTheme("preferences-system"));
  m_ui->m_createDataSourceToolButton->setIcon(QIcon::fromTheme("datasource-new"));

// add the list of data sources available in the system
  std::map<std::string, DataSourceType*>::const_iterator it = DataSourceTypeManager::getInstance().begin();
  std::map<std::string, DataSourceType*>::const_iterator itend = DataSourceTypeManager::getInstance().end();

  while(it != itend)
  {
    QString dsName = QString::fromStdString(it->second->getName());
    QIcon icon = it->second->getIcon(DataSourceType::ICON_DATASOURCE_SMALL);
    QString title = QString::fromStdString(it->second->getTitle());
    QString description = QString::fromStdString(it->second->getDescription());

    QListWidgetItem* item = new QListWidgetItem(icon, title);

    item->setToolTip(description);

    item->setData(Qt::UserRole, QVariant(dsName));

    m_ui->m_datasourceTypeListWidget->addItem(item);

    ++it;
  }

//verify if datasourceTypeListWidget has any item and set the first one as selected equals true.
  if(m_ui->m_datasourceTypeListWidget->count() > 0)
  {
    m_ui->m_datasourceTypeListWidget->setItemSelected(m_ui->m_datasourceTypeListWidget->item(0), true);
    dataSourceTypePressed(m_ui->m_datasourceTypeListWidget->item(0));
  }

// connect signal and slots
  connect(m_ui->m_addDataSourceToolButton, SIGNAL(pressed()), this, SLOT(addDataSourcePushButtonPressed()));
  connect(m_ui->m_removeDataSourceToolButton, SIGNAL(pressed()), this, SLOT(removeDataSourcePushButtonPressed()));
  connect(m_ui->m_editDataSourceToolButton, SIGNAL(pressed()), this, SLOT(editDataSourcePushButtonPressed()));
  connect(m_ui->m_createDataSourceToolButton, SIGNAL(pressed()), this, SLOT(createDataSourcePushButtonPressed()));
  connect(m_ui->m_datasourceTypeListWidget, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(dataSourceTypePressed(QListWidgetItem*)));
  connect(m_ui->m_datasourceListWidget, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(dataSourcePressed(QListWidgetItem*)));
  connect(m_ui->m_datasourceListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(dataSourceDoubleClicked(QListWidgetItem*)));

}

te::qt::widgets::DataSourceSelectorWidget::~DataSourceSelectorWidget()
{
}

Ui::DataSourceSelectorWidgetForm* te::qt::widgets::DataSourceSelectorWidget::getForm() const
{
  return m_ui.get();
}

void te::qt::widgets::DataSourceSelectorWidget::setSelectionMode(QAbstractItemView::SelectionMode mode)
{
  m_ui->m_datasourceListWidget->setSelectionMode(mode);
}

void te::qt::widgets::DataSourceSelectorWidget::setButtonsEnabled(bool enabled)
{
  m_addButtonEnabled = enabled;
  m_editButtonEnabled = enabled;
  m_removeButtonEnabled = enabled;
  m_createButtonEnabled = enabled;
}

void te::qt::widgets::DataSourceSelectorWidget::setButtonEnabled(int button, bool enabled)
{
  if(button == BUTTON_ADD_DATASOURCE)
    m_addButtonEnabled = enabled;
  else if(button == BUTTON_EDIT_DATASOURCE)
    m_editButtonEnabled = enabled;
  else if(button == BUTTON_REMOVE_DATASOURCE)
    m_removeButtonEnabled = enabled;
  else if(button == BUTTON_CREATE_DATASOURCE)
    m_createButtonEnabled = enabled;
}

//void te::qt::widgets::DataSourceSelectorWidget::selectDataSourceType(int row)
//{
//  if((row < 0) || (row >= m_ui->m_datasourceTypeListWidget->count()))
//    return;
//
//  QListWidgetItem* item = m_ui->m_datasourceTypeListWidget->item(row);
//
//  if(item == 0)
//    return;
//
//  //m_ui->m_datasourceTypeListWidget->setSele
//}

std::list<te::da::DataSourceInfoPtr> te::qt::widgets::DataSourceSelectorWidget::getSelecteds() const
{
  std::list<te::da::DataSourceInfoPtr> selecteds;

  QList<QListWidgetItem*> items = m_ui->m_datasourceListWidget->selectedItems();

  for(QList<QListWidgetItem*>::Iterator it = items.begin(); it != items.end(); ++it)
  {
    QListWidgetItem* item = *it;

    QVariant udata = item->data(Qt::UserRole);

    QString id = udata.toString();

    if(id.isEmpty())
      throw Exception(TR_QT_WIDGETS("Selected data source has no identification!"));

    te::da::DataSourceInfoPtr selected(te::da::DataSourceInfoManager::getInstance().get(id.toStdString()));

    if(selected.get() == 0)
      throw Exception(TR_QT_WIDGETS("Could not find selected data source!"));

    selecteds.push_back(selected);
  }

  return selecteds;
}

void te::qt::widgets::DataSourceSelectorWidget::setDataSource(const QString& dsType)
{
  int count = m_ui->m_datasourceTypeListWidget->count();
  int found = -1;

  for(int i=0; i<count; i++)
  {
    QListWidgetItem* item = m_ui->m_datasourceTypeListWidget->item(i);
    
    if(item->data(Qt::UserRole).toString() == dsType)
    {
      found = i;
      break;
    }
  }

  if (found != -1)
  {
    QListWidgetItem* item = m_ui->m_datasourceTypeListWidget->item(found);

    m_ui->m_datasourceTypeListWidget->setCurrentItem(item); 
    dataSourceTypePressed(item);
  }
}

void  te::qt::widgets::DataSourceSelectorWidget::showDataSourceWithDatabaseSupport(bool visible)
{
  int count = m_ui->m_datasourceTypeListWidget->count();

  for(int i=0; i<count; i++)
  {
    QListWidgetItem* item = m_ui->m_datasourceTypeListWidget->item(i);

    std::string name = item->data(Qt::UserRole).toString().toStdString();

    const te::qt::widgets::DataSourceType* type = DataSourceTypeManager::getInstance().get(name);

    if(type->hasDatabaseSupport())
      item->setHidden(!visible);
  }
}

void  te::qt::widgets::DataSourceSelectorWidget::showDataSourceWithFileSupport(bool visible)
{
  int count = m_ui->m_datasourceTypeListWidget->count();

  for(int i=0; i<count; i++)
  {
    QListWidgetItem* item = m_ui->m_datasourceTypeListWidget->item(i);

    std::string name = item->data(Qt::UserRole).toString().toStdString();

    const te::qt::widgets::DataSourceType* type = DataSourceTypeManager::getInstance().get(name);

    if(type->hasFileSupport())
      item->setHidden(!visible);
  }
}

void  te::qt::widgets::DataSourceSelectorWidget::showDataSourceWithRasterSupport(bool visible)
{
  int count = m_ui->m_datasourceTypeListWidget->count();

  for(int i=0; i<count; i++)
  {
    QListWidgetItem* item = m_ui->m_datasourceTypeListWidget->item(i);

    std::string name = item->data(Qt::UserRole).toString().toStdString();

    const te::qt::widgets::DataSourceType* type = DataSourceTypeManager::getInstance().get(name);

    if(type->hasRasterSupport())
      item->setHidden(!visible);
  }
}

void  te::qt::widgets::DataSourceSelectorWidget::showDataSourceWithVectorialSupport(bool visible)
{
  int count = m_ui->m_datasourceTypeListWidget->count();

  for(int i=0; i<count; i++)
  {
    QListWidgetItem* item = m_ui->m_datasourceTypeListWidget->item(i);

    std::string name = item->data(Qt::UserRole).toString().toStdString();

    const te::qt::widgets::DataSourceType* type = DataSourceTypeManager::getInstance().get(name);

    if(type->hasVectorialSupport())
      item->setHidden(!visible);
  }
}

void te::qt::widgets::DataSourceSelectorWidget::addDataSourcePushButtonPressed()
{
  QListWidgetItem* item = m_ui->m_datasourceTypeListWidget->currentItem();

  if(item == 0)
    return;

  QVariant udata = item->data(Qt::UserRole);

  QString dsTypeId = udata.toString();

  if(dsTypeId.isEmpty())
    return;

  try
  {
    const DataSourceType* dsType = DataSourceTypeManager::getInstance().get(dsTypeId.toStdString());

    if(dsType == 0)
      throw Exception(TR_QT_WIDGETS("Unknown data source type!"));

    std::auto_ptr<QWidget> connectorw(dsType->getWidget(DataSourceType::WIDGET_DATASOURCE_CONNECTOR, this));

    if(connectorw.get() == 0)
      throw Exception(TR_QT_WIDGETS("This type of data source hasn't provide a dialog for adding a new data source"));

    AbstractDataSourceConnector* connector = dynamic_cast<AbstractDataSourceConnector*>(connectorw.get());

    if(connector == 0)
      throw Exception(TR_QT_WIDGETS("Wrong type of object for adding a new data source!"));

    std::list<te::da::DataSourceInfoPtr> datasources;

    connector->connect(datasources);

    for(std::list<te::da::DataSourceInfoPtr>::iterator it = datasources.begin(); it != datasources.end(); ++it)
    {
      if(it->get() == 0)
        return;

      QListWidgetItem* item = new QListWidgetItem(QString::fromStdString((*it)->getTitle()));
      item->setData(Qt::UserRole, QVariant(QString::fromStdString((*it)->getId())));
      m_ui->m_datasourceListWidget->addItem(item);
      m_ui->m_datasourceListWidget->setCurrentItem(item);
      dataSourcePressed(item);
    }
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this,
                         tr("TerraLib Qt Components"),
                         tr(e.what()));
  }
  catch(...)
  {
    QMessageBox::warning(this,
                         tr("TerraLib Qt Components"),
                         tr("Unknown error while adding a new data source!"));
  }
}

void te::qt::widgets::DataSourceSelectorWidget::removeDataSourcePushButtonPressed()
{
  std::list<te::da::DataSourceInfoPtr> selecteds = getSelecteds();

  if(selecteds.empty())
    return;

  try
  {
    QListWidgetItem* item = m_ui->m_datasourceTypeListWidget->currentItem();

    QVariant udata = item->data(Qt::UserRole);

    QString dsTypeName = udata.toString();

    if(dsTypeName.isEmpty())
      throw Exception(TR_QT_WIDGETS("Unknown data source type!"));
    
    const DataSourceType* dsType = DataSourceTypeManager::getInstance().get(dsTypeName.toStdString());

    if(dsType == 0)
      throw Exception(TR_QT_WIDGETS("Could not find data source type!"));

    std::auto_ptr<QWidget> connectorw(dsType->getWidget(DataSourceType::WIDGET_DATASOURCE_CONNECTOR, this));

    if(connectorw.get() == 0)
      throw Exception(TR_QT_WIDGETS("This type of data source hasn't provide a dialog for removing data sources"));

    AbstractDataSourceConnector* connector = dynamic_cast<AbstractDataSourceConnector*>(connectorw.get());

    if(connector == 0)
      throw Exception(TR_QT_WIDGETS("Wrong type of object for removing a data source!"));

    connector->remove(selecteds);

    QList<QListWidgetItem*> items = m_ui->m_datasourceListWidget->selectedItems();

    for(QList<QListWidgetItem*>::Iterator it = items.begin(); it != items.end(); ++it)
    {
      item = *it;

      int itemRow = m_ui->m_datasourceListWidget->row(item);

      item = m_ui->m_datasourceListWidget->takeItem(itemRow);

      delete item;
    }

    if(m_ui->m_datasourceListWidget->count() == 0)
    {
      m_ui->m_removeDataSourceToolButton->setEnabled(false);
      m_ui->m_editDataSourceToolButton->setEnabled(false);
      m_ui->m_datasourceDescriptionLabel->clear();
      //emit hasSelectedDataSource(false);
    }
    else
    {
      //item = m_ui->m_datasourceListWidget->item(0);

      //if(item == 0)
      //  throw Exception(TR_QT_WIDGETS("Could not automatically select the first data source item!"));

      //m_ui->m_datasourceListWidget->setCurrentItem(item, QItemSelectionModel::Select);
      //dataSourcePressed(item);
      m_ui->m_datasourceDescriptionLabel->clear();

      //emit hasSelectedDataSource(!m_ui->m_datasourceListWidget->selectedItems().empty());
    }
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this,
                         tr("TerraLib Qt Components"),
                         tr(e.what()));
  }
  catch(...)
  {
    QMessageBox::warning(this,
                         tr("TerraLib Qt Components"),
                         tr("Unknown error while removing a data source!"));
  }
}

void te::qt::widgets::DataSourceSelectorWidget::editDataSourcePushButtonPressed()
{
  std::list<te::da::DataSourceInfoPtr> selecteds = getSelecteds();

  if(selecteds.empty())
    return;

  try
  {
    QListWidgetItem* item = m_ui->m_datasourceTypeListWidget->currentItem();

    QVariant udata = item->data(Qt::UserRole);

    QString dsTypeName = udata.toString();

    if(dsTypeName.isEmpty())
      throw Exception(TR_QT_WIDGETS("Unknown data source type!"));
    
    const DataSourceType* dsType = DataSourceTypeManager::getInstance().get(dsTypeName.toStdString());

    if(dsType == 0)
      throw Exception(TR_QT_WIDGETS("Could not find data source type!"));

    std::auto_ptr<QWidget> connectorw(dsType->getWidget(DataSourceType::WIDGET_DATASOURCE_CONNECTOR, this));

    if(connectorw.get() == 0)
      throw Exception(TR_QT_WIDGETS("This type of data source hasn't provide a dialog for removing data sources"));

    AbstractDataSourceConnector* connector = dynamic_cast<AbstractDataSourceConnector*>(connectorw.get());

    if(connector == 0)
      throw Exception(TR_QT_WIDGETS("Wrong type of object for removing a data source!"));

    connector->update(selecteds);

    QList<QListWidgetItem*> items = m_ui->m_datasourceListWidget->selectedItems();

    for(QList<QListWidgetItem*>::Iterator it = items.begin(); it != items.end(); ++it)
    {
      item = *it;

      QVariant udata = item->data(Qt::UserRole);

      QString dsId = udata.toString();

      if(dsId.isEmpty())
        throw Exception(TR_QT_WIDGETS("Invalid data source id!"));

      te::da::DataSourceInfoPtr ds = te::da::DataSourceInfoManager::getInstance().get(dsId.toStdString());

      item->setText(QString::fromStdString(ds->getTitle()));
    }

    if(items.isEmpty())
      throw Exception(TR_QT_WIDGETS("Update should not remove data source selection!"));

    dataSourcePressed(*(items.begin()));
    m_ui->m_datasourceListWidget->update();
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this,
                         tr("TerraLib Qt Components"),
                         tr(e.what()));
  }
  catch(...)
  {
    QMessageBox::warning(this,
                         tr("TerraLib Qt Components"),
                         tr("Unknown error while updating data source information!"));
  }
}

void te::qt::widgets::DataSourceSelectorWidget::createDataSourcePushButtonPressed()
{
  QListWidgetItem* item = m_ui->m_datasourceTypeListWidget->currentItem();

  if(item == 0)
    return;

  QVariant udata = item->data(Qt::UserRole);

  QString dsTypeId = udata.toString();

  if(dsTypeId.isEmpty())
    return;

  try
  {
    const DataSourceType* dsType = DataSourceTypeManager::getInstance().get(dsTypeId.toStdString());

    if(dsType == 0)
      throw Exception(TR_QT_WIDGETS("Unknown data source type!"));

    std::auto_ptr<QWidget> connectorw(dsType->getWidget(DataSourceType::WIDGET_DATASOURCE_CONNECTOR, this));

    if(connectorw.get() == 0)
      throw Exception(TR_QT_WIDGETS("This type of data source hasn't provide a dialog for adding a new data source"));

    AbstractDataSourceConnector* connector = dynamic_cast<AbstractDataSourceConnector*>(connectorw.get());

    if(connector == 0)
      throw Exception(TR_QT_WIDGETS("Wrong type of object for adding a new data source!"));

    std::list<te::da::DataSourceInfoPtr> datasources;

    connector->create(datasources);

    for(std::list<te::da::DataSourceInfoPtr>::iterator it = datasources.begin(); it != datasources.end(); ++it)
    {
      if(it->get() == 0)
        return;

      QListWidgetItem* item = new QListWidgetItem(QString::fromStdString((*it)->getTitle()));
      item->setData(Qt::UserRole, QVariant(QString::fromStdString((*it)->getId())));
      m_ui->m_datasourceListWidget->addItem(item);
      m_ui->m_datasourceListWidget->setCurrentItem(item);
      dataSourcePressed(item);
    }
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this,
                         tr("TerraLib Qt Components"),
                         tr(e.what()));
  }
  catch(...)
  {
    QMessageBox::warning(this,
                         tr("TerraLib Qt Components"),
                         tr("Unknown error while adding a new data source!"));
  }
}

void te::qt::widgets::DataSourceSelectorWidget::dataSourceTypePressed(QListWidgetItem* item)
{
  m_ui->m_datasourceTypeTitleLabel->clear();
  m_ui->m_datasourceListWidget->clear();
  m_ui->m_datasourceDescriptionLabel->clear();
  //m_ui->m_selectPushButton->setEnabled(false);
  m_ui->m_addDataSourceToolButton->setEnabled(false);
  m_ui->m_removeDataSourceToolButton->setEnabled(false);
  m_ui->m_editDataSourceToolButton->setEnabled(false);
  m_ui->m_createDataSourceToolButton->setEnabled(false);

  //emit hasSelectedDataSource(false);

  if(item == 0)
    return;

  QVariant udata = item->data(Qt::UserRole);

  QString dsTypeName = udata.toString();

  if(dsTypeName.isEmpty())
    return;

  m_ui->m_datasourceTypeTitleLabel->setText(item->text());

  std::vector<te::da::DataSourceInfoPtr> datasources;

  te::da::DataSourceInfoManager::getInstance().getByType(dsTypeName.toStdString(), datasources);

  for(std::size_t i = 0; i < datasources.size(); ++i)
  {
    const te::da::DataSourceInfoPtr& datasource = datasources[i];

    if(datasource.get() == 0)
      continue;

    const std::string& title = datasource->getTitle();

    const std::string& id = datasource->getId();

    QListWidgetItem* item = new QListWidgetItem(title.c_str());

    item->setData(Qt::UserRole, QVariant(QString(id.c_str())));

    m_ui->m_datasourceListWidget->addItem(item);
  }

  //if(m_ui->m_datasourceListWidget->count() != 0)
  //{
  //  QListWidgetItem* selected = m_ui->m_datasourceListWidget->item(0);

  //  m_ui->m_datasourceListWidget->setCurrentItem(selected, QItemSelectionModel::Select);
  //  dataSourcePressed(selected);
  //}

  m_ui->m_addDataSourceToolButton->setEnabled(m_addButtonEnabled);
  m_ui->m_createDataSourceToolButton->setEnabled(m_createButtonEnabled);

  const DataSourceType* dsType = DataSourceTypeManager::getInstance().get(dsTypeName.toStdString());

  if(dsType != 0)
    m_ui->m_datasourceDescriptionLabel->setText(QString::fromStdString(dsType->getDescription()));
}

void te::qt::widgets::DataSourceSelectorWidget::dataSourcePressed(QListWidgetItem* item)
{
  m_ui->m_datasourceDescriptionLabel->clear();

  if(item == 0)
    return;

  QVariant udata = item->data(Qt::UserRole);

  QString id = udata.toString();

  if(id.isEmpty())
    return;

  te::da::DataSourceInfoPtr ds = te::da::DataSourceInfoManager::getInstance().get(id.toStdString());

  if(ds.get() == 0)
  {
    QMessageBox::warning(this,
                         tr("TerraLib Qt Components"),
                         tr("The selected data source is not registered in the system.\nThis may be an error on the connector implementation!"));

    return;
  }

  m_ui->m_datasourceDescriptionLabel->setText(QString::fromStdString(ds->getDescription()));

  m_ui->m_removeDataSourceToolButton->setEnabled(m_removeButtonEnabled);
  m_ui->m_editDataSourceToolButton->setEnabled(m_editButtonEnabled);
  //m_ui->m_createDataSourceToolButton->setEnabled(m_createButtonEnabled);

  //emit hasSelectedDataSource(true);
}

void te::qt::widgets::DataSourceSelectorWidget::dataSourceDoubleClicked(QListWidgetItem * item)
{
  item->setSelected(true);
}

//void te::qt::widgets::DataSourceSelectorDialog::add(DataSourcePtr ds)
//{
//  if(ds.get() == 0)
//    return;
//
//  DataSourceManager::getInstance().add(ds);
//
//  const std::string& title = ds->getTitle();
//
//  const std::string& id = ds->getId();
//
//  QListWidgetItem* item = new QListWidgetItem(title.c_str());
//
//  item->setData(Qt::UserRole, QVariant(QString(id.c_str())));
//
//  m_ui->m_datasourceListWidget->addItem(item);
//
//  dataSourcePressed(item);
//}
//
//void te::qt::widgets::DataSourceSelectorDialog::update(DataSourcePtr ds)
//{
//  if(ds.get() == 0)
//    return;
//
//  QListWidgetItem* item = m_ui->m_datasourceListWidget->currentItem();
//
//  if(item == 0)
//    return; // should be an exception!
//
//  item->setData(Qt::UserRole, QVariant(QString::fromStdString(ds->getId())));
//
//  item->setText(QString::fromStdString(ds->getTitle()));
//
//  m_ui->m_datasourceListWidget->update();
//
//  dataSourcePressed(item);
//}


