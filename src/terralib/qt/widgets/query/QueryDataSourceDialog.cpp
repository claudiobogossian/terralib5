/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/query/QueryDataSourceDialog.cpp

  \brief This file defines a class for a Query DataSource Dialog Dialog.
*/

// TerraLib
#include "../../../common/StringUtils.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/DataSetAdapter.h"
#include "../../../dataaccess/dataset/DataSetTypeConverter.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../dataaccess/datasource/DataSourceCapabilities.h"
#include "../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../dataaccess/query/SQLDialect.h"
#include "../../../dataaccess/query/SQLFunctionEncoder.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../datatype/SimpleProperty.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../maptools/DataSetLayer.h"
#include "../datasource/selector/DataSourceSelectorDialog.h"
#include "../layer/utils/DataSet2Layer.h"
#include "QueryDataSourceDialog.h"
#include "ui_QueryDataSourceDialogForm.h"

// Qt
#include <QCursor>
#include <QFileDialog>
#include <QGridLayout>
#include <QMessageBox>
#include <QTextStream>

// STL
#include <cassert>
#include <memory>

// Boost
#include <boost/filesystem.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::qt::widgets::QueryDataSourceDialog::QueryDataSourceDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::QueryDataSourceDialogForm)
{
  m_ui->setupUi(this);

  m_ui->m_applyToolButton->setIcon(QIcon::fromTheme("media-playback-start-green"));
  m_ui->m_clearToolButton->setIcon(QIcon::fromTheme("edit-clear"));
  m_ui->m_applySelToolButton->setIcon(QIcon::fromTheme("pointer-selection"));
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));
  m_ui->m_createLayerlToolButton->setIcon(QIcon::fromTheme("layer-new"));
  

  m_ui->m_saveSqlToolButton->setIcon(QIcon::fromTheme("document-save-as"));
  m_ui->m_openSqlToolButton->setIcon(QIcon::fromTheme("document-open"));

  //table view
  m_tableModel = new te::qt::widgets::DataSetTableModel(m_ui->m_tableView);
  m_ui->m_tableView->setModel(m_tableModel);

  //dataset display
  m_dataSetDisplay = new te::qt::widgets::DataSetDisplay(this);

  m_appMapDisplay = 0;

  QGridLayout* displayGridLayout = new QGridLayout(m_ui->m_displayWidget);
  displayGridLayout->setContentsMargins(0, 0, 0, 0);
  displayGridLayout->addWidget(m_dataSetDisplay);

  // Signals¨& slots
  connect(m_ui->m_dataSourceComboBox, SIGNAL(activated(int)), this, SLOT(onDataSourceSelected(int)));
  connect(m_ui->m_baseDataSetComboBox, SIGNAL(activated(int)), this, SLOT(onBaseDataSetSelected(int)));
  connect(m_ui->m_dataSetListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onDataSetItemClicked(QListWidgetItem*)));
  connect(m_ui->m_applyToolButton, SIGNAL(clicked()), this, SLOT(onApplyPushButtonClicked()));
  connect(m_ui->m_clearToolButton, SIGNAL(clicked()), this, SLOT(onClearPushButtonClicked()));
  connect(m_ui->m_saveSqlToolButton, SIGNAL(clicked()), this, SLOT(onSaveSqlToolButtonClicked()));
  connect(m_ui->m_openSqlToolButton, SIGNAL(clicked()), this, SLOT(onOpenSqlToolButtonClicked()));
  connect(m_ui->m_sqlEditorTextEdit, SIGNAL(textChanged()), this, SLOT(onSQLEditorTextChanged()));
  connect(m_ui->m_applySelToolButton, SIGNAL(clicked()), this, SLOT(onApplySelToolButtonClicked()));
  connect(m_ui->m_createLayerlToolButton, SIGNAL(pressed()), this, SLOT(onCreateLayerToolButtonClicked()));
  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this,  SLOT(onTargetFileToolButtonPressed()));
  connect(m_ui->m_pkTableComboBox, SIGNAL(activated(int)), this, SLOT(onPkTableComboBoxSelected(int)));

  //load data sources information
  loadDataSourcesInformation();

  //
  m_ui->m_helpPushButton->setPageReference("widgets/query/query_datasource.html");
}

te::qt::widgets::QueryDataSourceDialog::~QueryDataSourceDialog()
{
  m_keyWords.clear();
}

void te::qt::widgets::QueryDataSourceDialog::setLayerList(std::list<te::map::AbstractLayerPtr> layerList)
{
  m_layerList = layerList;

  if(m_ui->m_baseDataSetComboBox->count() > 0)
    onBaseDataSetSelected(0);
}

void te::qt::widgets::QueryDataSourceDialog::setAppMapDisplay(te::qt::widgets::MapDisplay* appMapDisplay)
{
  m_appMapDisplay = appMapDisplay;
}

void te::qt::widgets::QueryDataSourceDialog::loadDataSourcesInformation()
{
  m_ui->m_dataSourceComboBox->clear();

  std::map<std::string, te::da::DataSourceInfoPtr>::const_iterator it = te::da::DataSourceInfoManager::getInstance().begin();
  std::map<std::string, te::da::DataSourceInfoPtr>::const_iterator itend =te::da::DataSourceInfoManager::getInstance().end();

  while(it != itend)
  {
    if(it->second->getType() != "GDAL")
      m_ui->m_dataSourceComboBox->addItem(it->second->getTitle().c_str(), QVariant(it->second->getId().c_str()));

    ++it;
  }

  if(m_ui->m_dataSourceComboBox->count() != 0)
    onDataSourceSelected(m_ui->m_dataSourceComboBox->currentIndex());
}

void te::qt::widgets::QueryDataSourceDialog::buildMap()
{
  m_keyWords.clear();

  // add defaul SQL key words
  m_keyWords.insert(std::map<std::string, Qt::GlobalColor>::value_type("SELECT", Qt::blue));
  m_keyWords.insert(std::map<std::string, Qt::GlobalColor>::value_type("FROM", Qt::blue));
  m_keyWords.insert(std::map<std::string, Qt::GlobalColor>::value_type("WHERE", Qt::blue));
  m_keyWords.insert(std::map<std::string, Qt::GlobalColor>::value_type("JOIN", Qt::blue));
  m_keyWords.insert(std::map<std::string, Qt::GlobalColor>::value_type("INNER", Qt::blue));
  m_keyWords.insert(std::map<std::string, Qt::GlobalColor>::value_type("LEFT", Qt::blue));
  m_keyWords.insert(std::map<std::string, Qt::GlobalColor>::value_type("RIGHT", Qt::blue));
  m_keyWords.insert(std::map<std::string, Qt::GlobalColor>::value_type("AS", Qt::blue));
  m_keyWords.insert(std::map<std::string, Qt::GlobalColor>::value_type("GROUP", Qt::blue));
  m_keyWords.insert(std::map<std::string, Qt::GlobalColor>::value_type("ORDER", Qt::blue));
  m_keyWords.insert(std::map<std::string, Qt::GlobalColor>::value_type("BY", Qt::blue));

  //add data source specific key words
  std::string dataSourceId = m_ui->m_dataSourceComboBox->itemData(m_ui->m_dataSourceComboBox->currentIndex()).toString().toStdString();

  te::da::DataSourcePtr ds = te::da::GetDataSource(dataSourceId);

  const te::da::SQLDialect* dialect = ds->getDialect();

  std::vector<std::string> names = dialect->getRegisteredNames();

  for(std::size_t t = 0; t < names.size(); ++t)
  {
    QString s = names[t].c_str();
    QString sUpper = s.toUpper();

    m_keyWords.insert(std::map<std::string, Qt::GlobalColor>::value_type(sUpper.toStdString(), Qt::red));
  }

}

void te::qt::widgets::QueryDataSourceDialog::onDataSourceSelected(int index)
{
  m_ui->m_baseDataSetComboBox->clear();
  m_ui->m_dataSetListWidget->clear();
  m_ui->m_attrDataSetListWidget->clear();
  m_ui->m_pkTableComboBox->clear();
  m_ui->m_pkAttrComboBox->clear();

  std::string dataSourceId = m_ui->m_dataSourceComboBox->itemData(index).toString().toStdString();

  te::da::DataSourcePtr ds = te::da::GetDataSource(dataSourceId);

  if(!ds->isOpened())
    ds->open();

  std::vector<std::string> dataSetNames;
  te::da::GetDataSetNames(dataSetNames, dataSourceId);

  for(std::size_t t = 0; t < dataSetNames.size(); ++t)
  {
    m_ui->m_baseDataSetComboBox->addItem(dataSetNames[t].c_str());
    m_ui->m_dataSetListWidget->addItem(dataSetNames[t].c_str());
    m_ui->m_pkTableComboBox->addItem(dataSetNames[t].c_str());
  }

  if(m_ui->m_baseDataSetComboBox->count() > 0)
    onBaseDataSetSelected(0);

  if (m_ui->m_pkTableComboBox->count() > 0)
    onPkTableComboBoxSelected(0);

  buildMap();
}

void te::qt::widgets::QueryDataSourceDialog::onBaseDataSetSelected(int index)
{
  std::string dataSet = m_ui->m_baseDataSetComboBox->itemText(index).toStdString();

  m_ui->m_layerComboBox->clear();

  std::list<te::map::AbstractLayerPtr>::iterator it = m_layerList.begin();

  while(it != m_layerList.end())
  {
    te::map::AbstractLayerPtr l = *it;

    std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

    te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(l.get());

    if(dsLayer && dsType->getName() == dataSet)
      m_ui->m_layerComboBox->addItem(l->getTitle().c_str(), QVariant::fromValue(l));

    ++it;
  }
}

void te::qt::widgets::QueryDataSourceDialog::onDataSetItemClicked(QListWidgetItem* item)
{
  m_ui->m_attrDataSetListWidget->clear();

  if(!item)
    return;

  std::string dataSourceId = m_ui->m_dataSourceComboBox->itemData(m_ui->m_dataSourceComboBox->currentIndex()).toString().toStdString();

  std::string dataSetName = item->text().toStdString();

  te::da::DataSetType* dsType = te::da::GetDataSetType(dataSetName, dataSourceId);

  std::vector<te::dt::Property*> propVec = dsType->getProperties();

  for(std::size_t t = 0; t < propVec.size(); ++t)
  {
    m_ui->m_attrDataSetListWidget->addItem(propVec[t]->getName().c_str());
  }

  delete dsType;
}

void te::qt::widgets::QueryDataSourceDialog::onPkTableComboBoxSelected(int index)
{
  std::string dataSetName = m_ui->m_pkTableComboBox->itemText(index).toStdString();

  std::string dataSourceId = m_ui->m_dataSourceComboBox->itemData(m_ui->m_dataSourceComboBox->currentIndex()).toString().toStdString();

  te::da::DataSetType* dsType = te::da::GetDataSetType(dataSetName, dataSourceId);

  std::vector<te::dt::Property*> propVec = dsType->getProperties();

  for (std::size_t t = 0; t < propVec.size(); ++t)
  {
    m_ui->m_pkAttrComboBox->addItem(propVec[t]->getName().c_str());
  }

  delete dsType;
}

void te::qt::widgets::QueryDataSourceDialog::onApplyPushButtonClicked()
{
  m_ui->m_sqlEditorTextEdit->setFocus();

  if(m_ui->m_sqlEditorTextEdit->toPlainText().isEmpty())
    return;

  std::string dataSourceId = m_ui->m_dataSourceComboBox->itemData(m_ui->m_dataSourceComboBox->currentIndex()).toString().toStdString();

  te::da::DataSourcePtr ds = te::da::GetDataSource(dataSourceId);

  std::string dataSetName = m_ui->m_baseDataSetComboBox->currentText().toStdString();

  te::da::DataSetTypePtr dsType(te::da::GetDataSetType(dataSetName, dataSourceId));

  std::string sql = "";

  if(m_ui->m_sqlEditorTextEdit->textCursor().selectedText().isEmpty())
    sql = m_ui->m_sqlEditorTextEdit->toPlainText().toStdString();
  else
    sql = m_ui->m_sqlEditorTextEdit->textCursor().selectedText().toStdString();

  //get dataset
  std::auto_ptr<te::da::DataSet> dataSet;
  
  try
  {
   dataSet = ds->query(sql);
  }
  catch(const std::exception& e)
  {
    m_dataSetDisplay->clear();
    m_tableModel->setDataSet(0, ds->getEncoding());

    std::string errorMessage = "SQL Error: ";
    errorMessage += e.what();
    errorMessage += "\n";
    errorMessage += "-------------------------------------------------------------------------\n";

    m_ui->m_logPlainTextEdit->appendPlainText(errorMessage.c_str());

    m_ui->m_tabWidget->setCurrentIndex(1);

    return;
  }

  std::string message = "SQL Done: ";
  message += sql;
  message += "\n";
  message += "-------------------------------------------------------------------------\n";

  m_ui->m_logPlainTextEdit->appendPlainText(message.c_str());

  //draw dataset
  m_dataSetDisplay->clear();

  bool draw = false;
  for(std::size_t t = 0;  t < dataSet->getNumProperties(); ++t)
  {
    int type = dataSet->getPropertyDataType(t);

    if(type == te::dt::GEOMETRY_TYPE)
    {
      draw = true;
      break;
    }
  }

  if(draw)
    m_dataSetDisplay->draw(dsType, ds, dataSet.get());
  else
    m_dataSetDisplay->clear();

  //show dataset on table
  m_tableModel->setDataSet(dataSet.release(), ds->getEncoding());

  m_ui->m_tabWidget->setCurrentIndex(0);
}

void te::qt::widgets::QueryDataSourceDialog::onClearPushButtonClicked()
{
  m_ui->m_sqlEditorTextEdit->clear();

  m_dataSetDisplay->clear();

  m_tableModel->setDataSet(0, te::common::LATIN1);
}

void te::qt::widgets::QueryDataSourceDialog::onSQLEditorTextChanged()
{
  disconnect(m_ui->m_sqlEditorTextEdit, SIGNAL(textChanged()), this, SLOT(onSQLEditorTextChanged()));

  QString sql = m_ui->m_sqlEditorTextEdit->toPlainText();

  int curPos = m_ui->m_sqlEditorTextEdit->textCursor().position();

  disconnect(m_ui->m_sqlEditorTextEdit, SIGNAL(textChanged()), this, SLOT(onSQLEditorTextChanged()));

  m_ui->m_sqlEditorTextEdit->clear();

  QStringList words = sql.split(' ');

  bool isAttrValue = false;

  for(int i = 0; i < words.size(); ++i)
  {
    QString w = words.value(i).toUpper();

    std::string strW = w.toStdString();

    std::map<std::string, Qt::GlobalColor>::iterator it = m_keyWords.find(strW);

    bool removeAttrValue = false;

    if(it != m_keyWords.end())
    {
      m_ui->m_sqlEditorTextEdit->setFontWeight(QFont::Bold);
      m_ui->m_sqlEditorTextEdit->setTextColor(it->second);
    }
    else
    {
      bool okNum = false;

      words.value(i).toDouble(&okNum);
      
      m_ui->m_sqlEditorTextEdit->setFontWeight(QFont::Normal);
      
      if(okNum)
        m_ui->m_sqlEditorTextEdit->setTextColor(Qt::darkGreen);
      else
      {
        if(!strW.empty())
        {
          std::string initChar = strW.substr(0, 1);
          if(!isAttrValue && (initChar == "'" || initChar == "\""))
            isAttrValue = true;

          if(strW.size() > 1)
          {
            std::string lastChar = strW.substr(strW.size() - 1, 1);
            if(isAttrValue && (lastChar == "'" || lastChar == "\""))
              removeAttrValue = true;
          }
        }

        if(isAttrValue)
          m_ui->m_sqlEditorTextEdit->setTextColor(Qt::magenta);
        else
          m_ui->m_sqlEditorTextEdit->setTextColor(Qt::black);
      }
    }

    m_ui->m_sqlEditorTextEdit->insertPlainText(words.value(i));

    if(removeAttrValue)
      isAttrValue = false;

    if(i < words.size() - 1)
      m_ui->m_sqlEditorTextEdit->insertPlainText(" ");
  }

  QTextCursor c = m_ui->m_sqlEditorTextEdit->textCursor();
  c.setPosition(curPos);
  m_ui->m_sqlEditorTextEdit->setTextCursor(c);

  connect(m_ui->m_sqlEditorTextEdit, SIGNAL(textChanged()), this, SLOT(onSQLEditorTextChanged()));
}

void te::qt::widgets::QueryDataSourceDialog::onSaveSqlToolButtonClicked()
{
  //select file
  QString path = QFileDialog::getSaveFileName(this, tr("Set a SQL file..."), "", tr("SQL File *.sql"));
  
  if(path.isNull())
    return;

 //open file
  QFile file(path);
  
  if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) 
  {
    QMessageBox::warning(this, tr("Query DataSource"), file.errorString());
    return;
  }

  // save to file
  QTextStream out(&file);

  out << m_ui->m_sqlEditorTextEdit->toPlainText();

  file.close();
}

void te::qt::widgets::QueryDataSourceDialog::onOpenSqlToolButtonClicked()
{
  //select file
  QString path = QFileDialog::getOpenFileName(this, tr("Select a SQL file..."), "", tr("SQL File *.sql"));
  
  if(path.isNull())
    return;

  //open file
  QFile file(path);
  
  if(!file.open(QIODevice::ReadOnly)) 
  {
    QMessageBox::warning(this, tr("Query DataSource"), file.errorString());
    return;
  }

  // show file
  QTextStream in(&file);

  m_ui->m_sqlEditorTextEdit->clear();

  while(!in.atEnd()) 
  {
    QString line = in.readLine();

    m_ui->m_sqlEditorTextEdit->append(line);
  }

  file.close();
}

void te::qt::widgets::QueryDataSourceDialog::onApplySelToolButtonClicked()
{
  if(m_ui->m_sqlEditorTextEdit->toPlainText().isEmpty())
  {
    QMessageBox::information(this, tr("Warning"), tr("SQL not defined."));
    return;
  }

  QVariant varLayer = m_ui->m_layerComboBox->itemData(m_ui->m_layerComboBox->currentIndex(), Qt::UserRole);
  te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

  if(!layer.get())
  {
    QMessageBox::warning(this, tr("Query DataSource"), tr("No layer selected."));
    return;
  }

  QApplication::setOverrideCursor(Qt::WaitCursor);

  std::string dataSourceId = m_ui->m_dataSourceComboBox->itemData(m_ui->m_dataSourceComboBox->currentIndex()).toString().toStdString();

  te::da::DataSourcePtr ds = te::da::GetDataSource(dataSourceId);

  std::string dataSetName = m_ui->m_baseDataSetComboBox->currentText().toStdString();

  te::da::DataSetTypePtr dsType(te::da::GetDataSetType(dataSetName, dataSourceId));

  std::string sql = "";

  if(m_ui->m_sqlEditorTextEdit->textCursor().selectedText().isEmpty())
    sql = m_ui->m_sqlEditorTextEdit->toPlainText().toStdString();
  else
    sql = m_ui->m_sqlEditorTextEdit->textCursor().selectedText().toStdString();

  //get dataset
  std::auto_ptr<te::da::DataSet> dataSet;
  
  try
  {
    dataSet = ds->query(sql);
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Query DataSource"), tr("Error executing SQL."));
    QApplication::restoreOverrideCursor();
    return;
  }

  try
  {
    if(m_ui->m_newSelRadioButton->isChecked())
    {
      // Generates the oids
      dataSet->moveBeforeFirst();
      te::da::ObjectIdSet* oids = te::da::GenerateOIDSet(dataSet.get(), dsType.get());

      oids->setExpressionByInClause();

      layer->clearSelected();
      layer->select(oids);
    }
    else if(m_ui->m_addSelRadioButton->isChecked())
    {
      // Generates the oids
      dataSet->moveBeforeFirst();
      te::da::ObjectIdSet* oids = te::da::GenerateOIDSet(dataSet.get(), dsType.get());

      oids->setExpressionByInClause();

      layer->select(oids);
    }

    if(m_appMapDisplay)
    {
      m_appMapDisplay->refresh();
    }
  }
  catch(te::common::Exception& e)
  {
    QMessageBox::warning(this, tr("Query DataSource"), tr("Error selecting objects: ") + e.what());
    QApplication::restoreOverrideCursor();
    return;
  }

  QApplication::restoreOverrideCursor();

  QMessageBox::information(this, tr("Query DataSource"), tr("Selection done."));
}


void te::qt::widgets::QueryDataSourceDialog::onCreateLayerToolButtonClicked()
{
  // check input parameters
  if(m_ui->m_sqlEditorTextEdit->toPlainText().isEmpty())
  {
    QMessageBox::information(this, tr("Warning"), tr("SQL not defined."));
    return;
  }

  if(m_ui->m_repositoryLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, tr("Warning"), tr("Define a repository for the result."));
    return;
  }
       
  if(m_ui->m_newLayerNameLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, tr("Warning"), tr("Define a name for the resulting layer."));
    return;
  }

  QApplication::setOverrideCursor(Qt::WaitCursor);

  //create dataset
  std::string dataSourceId = m_ui->m_dataSourceComboBox->itemData(m_ui->m_dataSourceComboBox->currentIndex()).toString().toStdString();

  te::da::DataSourcePtr ds = te::da::GetDataSource(dataSourceId);

  std::string inputDataSetName = m_ui->m_baseDataSetComboBox->currentText().toStdString();

  te::da::DataSetTypePtr inputDataSetType(te::da::GetDataSetType(inputDataSetName, dataSourceId));

  std::string sql = "";

  if(m_ui->m_sqlEditorTextEdit->textCursor().selectedText().isEmpty())
    sql = m_ui->m_sqlEditorTextEdit->toPlainText().toStdString();
  else
    sql = m_ui->m_sqlEditorTextEdit->textCursor().selectedText().toStdString();

  std::auto_ptr<te::da::DataSet> dataSet;

  try
  {
    dataSet = ds->query(sql);
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Query DataSource"), tr("Error executing SQL."));
    QApplication::restoreOverrideCursor();
    return;
  }

  if (dataSet->size() == 0)
  {
    QMessageBox::warning(this, tr("Query DataSource"), tr("Query result is empty."));
    QApplication::restoreOverrideCursor();
    return;
  }

  //create / get data source
  te::da::DataSourcePtr outputDataSource;

  if(m_toFile)
  {
    //create new data source
    boost::filesystem::path uri(m_ui->m_repositoryLineEdit->text().toStdString());

    std::map<std::string, std::string> dsInfo;
    dsInfo["URI"] = uri.string();

    boost::uuids::basic_random_generator<boost::mt19937> gen;
    boost::uuids::uuid u = gen();
    std::string id_ds = boost::uuids::to_string(u);

    te::da::DataSourceInfoPtr dsInfoPtr(new te::da::DataSourceInfo);
    dsInfoPtr->setConnInfo(dsInfo);
    dsInfoPtr->setTitle(uri.stem().string());
    dsInfoPtr->setAccessDriver("OGR");
    dsInfoPtr->setType("OGR");
    dsInfoPtr->setDescription(uri.string());
    dsInfoPtr->setId(id_ds);

    te::da::DataSourceInfoManager::getInstance().add(dsInfoPtr);

    outputDataSource = te::da::DataSourceManager::getInstance().get(id_ds, "OGR", dsInfoPtr->getConnInfo());
  }
  else
  {
    outputDataSource = te::da::GetDataSource(m_outputDatasource->getId());
  }

  //get output dataset name
  std::string dataSetName = m_ui->m_newLayerNameLineEdit->text().toStdString();

  std::size_t idx = dataSetName.find(".");
  if (idx != std::string::npos)
        dataSetName=dataSetName.substr(0,idx);

  //save data
  std::auto_ptr<te::da::DataSetType> dsType(new te::da::DataSetType(dataSetName));

  dataSet->moveFirst();

  std::set<std::string> names;

  int srid = 0;

  for(std::size_t t = 0; t < dataSet->getNumProperties(); ++t)
  {
    //check if the property name its duplicated
    std::string propName = dataSet->getPropertyName(t);

    int count = 1;
    while(names.find(propName) != names.end())
    {
      propName += "_";
      propName += te::common::Convert2String(count);
    }

    names.insert(propName);

    //create output property
    te::dt::Property* p = 0;
    if(dataSet->getPropertyDataType(t) != te::dt::GEOMETRY_TYPE)
    {
      p = new te::dt::SimpleProperty(propName, dataSet->getPropertyDataType(t));
    }
    else
    {
      std::auto_ptr<te::gm::Geometry> geom = dataSet->getGeometry(t);

      srid = geom->getSRID();

      p = new te::gm::GeometryProperty(propName, srid, geom->getGeomTypeId());
    }
    
    //add property to output datasetype
    if(p)
    {
      dsType->add(p);

      //check primary key
      if (m_ui->m_pkCheckBox->isChecked())
      {
        std::string pkAttrName = m_ui->m_pkAttrComboBox->itemText(m_ui->m_pkAttrComboBox->currentIndex()).toStdString();

        if (te::common::Convert2UCase(pkAttrName) == te::common::Convert2UCase(p->getName()))
        {
          std::string pkName = dataSetName + "_" + p->getName() + "_pk";
          te::da::PrimaryKey* pk = new te::da::PrimaryKey(pkName, dsType.get());
          pk->add(p->clone());
        }
      }
    }
    else
    {
       QMessageBox::warning(this, tr("Query DataSource"), tr("Error creating output dataset."));
       QApplication::restoreOverrideCursor();
      return;
    }
  }

  dataSet->moveBeforeFirst();

  //create converter in case property name changed
  te::da::DataSetTypeConverter* converter = new te::da::DataSetTypeConverter(dsType.get(), outputDataSource->getCapabilities(), outputDataSource->getEncoding());

  te::da::AssociateDataSetTypeConverterSRID(converter, srid);

  std::auto_ptr<te::da::DataSetAdapter> dsAdapter(te::da::CreateAdapter(dataSet.get(), converter));
  
  //save data
  std::map<std::string, std::string> options;

  outputDataSource->createDataSet(dsType.get(), options);

  outputDataSource->add(dataSetName, dsAdapter.get(), options);

  //create layer
  try
  {
    te::qt::widgets::DataSet2Layer converter(outputDataSource->getId());
      
    te::da::DataSetTypePtr dt(outputDataSource->getDataSetType(dataSetName).release());

    te::map::AbstractLayerPtr layer = converter(dt);

    emit createNewLayer(layer);
  }
  catch(te::common::Exception& e)
  {
    QMessageBox::warning(this, tr("Query DataSource"), tr("Error creating layer. ") + e.what());
    QApplication::restoreOverrideCursor();
    return;
  }

  QApplication::restoreOverrideCursor();

  QMessageBox::information(this, tr("Query DataSource"), tr("Layer created."));
}

void te::qt::widgets::QueryDataSourceDialog::onTargetDatasourceToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_newLayerNameLineEdit->setEnabled(true);

  te::qt::widgets::DataSourceSelectorDialog dlg(this);
  dlg.exec();

  std::list<te::da::DataSourceInfoPtr> dsPtrList = dlg.getSelecteds();

  if(dsPtrList.size() <= 0)
    return;

  std::list<te::da::DataSourceInfoPtr>::iterator it = dsPtrList.begin();

  m_ui->m_repositoryLineEdit->setText(QString(it->get()->getTitle().c_str()));

  m_outputDatasource = *it;
  
  m_toFile = false;
}

void te::qt::widgets::QueryDataSourceDialog::onTargetFileToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_repositoryLineEdit->clear();
  
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."), QString(), tr("Shapefile (*.shp *.SHP);;"),0, QFileDialog::DontConfirmOverwrite);
  
  if (fileName.isEmpty())
    return;
  
  boost::filesystem::path outfile(fileName.toStdString());

  m_ui->m_repositoryLineEdit->setText(outfile.string().c_str());

  m_ui->m_newLayerNameLineEdit->setText(outfile.leaf().string().c_str());

  m_ui->m_newLayerNameLineEdit->setEnabled(false);
  
  m_toFile = true;
}
