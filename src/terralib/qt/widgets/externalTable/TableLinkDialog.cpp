/*  Copyright (C) 2010-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/externaltable/TableLinkDialog.h

  \brief A Qt dialog that allows users to create a new query layer based on the information of two distinct datasets
*/

// TerraLib
#include "../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../dataaccess/Enums.h"
#include "../../../dataaccess/query/BinaryFunction.h"
#include "../../../dataaccess/query/DataSetName.h"
#include "../../../dataaccess/query/Expression.h"
#include "../../../dataaccess/query/Field.h"
#include "../../../dataaccess/query/Join.h"
#include "../../../dataaccess/query/JoinConditionOn.h"
#include "../../../dataaccess/query/PropertyName.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../maptools/QueryLayer.h"
#include "../../../memory/DataSet.h"
#include "../../../se/Utils.h"
#include "../table/DataSetTableView.h"
#include "FieldsDialog.h"
#include "TableLinkDialog.h"
#include "ui_TableLinkDialogForm.h"

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

//QT
#include <QMessageBox>

// STL
#include <cassert>

te::qt::widgets::TableLinkDialog::TableLinkDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::TableLinkDialogForm)
{
  m_ui->setupUi(this);
  m_ui->m_dataToolButton->setIcon(QIcon::fromTheme("view-data-table"));
  m_ui->m_dataToolButton->setToolTip(tr("View dataset rows"));
  m_ui->m_advancedToolButton->setIcon(QIcon::fromTheme("preferences-system"));
  m_ui->m_advancedToolButton->setToolTip(tr("View advanced options"));

  //Adjusting the dataSetTableView that will be used to display the tabular dataset's data
  m_tabularView.reset(new DataSetTableView(m_ui->m_tabularFrame));
  QGridLayout* tabularLayout = new QGridLayout(m_ui->m_tabularFrame);
  tabularLayout->addWidget(m_tabularView.get());
  tabularLayout->setContentsMargins(0, 0, 0, 0);

  m_tabularView->setAlternatingRowColors(true);
  m_tabularView->verticalHeader()->setVisible(false);
  m_tabularView->setSelectionMode(QAbstractItemView::NoSelection);
  m_tabularView->hide();
  m_ui->m_dataPreviewGroupBox->hide();

  //Adjusting the doubleListWidget that will be used to configure the query's fields.
  m_fieldsDialog.reset(new te::qt::widgets::FieldsDialog(this));
  m_ui->m_tabularFrame->hide();
  m_ui->m_helpPushButton->setPageReference("widgets/external_table/table_link_dialog.html");

  //Connecting signals and slots
  connect(m_ui->m_dataSet2ComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onDataCBIndexChanged(int)));
  connect(m_ui->m_dataToolButton, SIGNAL(clicked()), this, SLOT(onDataToolButtonnClicked()));
  connect(m_ui->m_advancedToolButton, SIGNAL(clicked()), this, SLOT(onAdvancedToolButtonnClicked()));
  connect(m_ui->m_OkPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
}

te::qt::widgets::TableLinkDialog::~TableLinkDialog()
{
}

void te::qt::widgets::TableLinkDialog::setInputLayer(te::map::AbstractLayerPtr inLayer)
{
  m_inputLayer.reset((te::map::DataSetLayer*)inLayer.get());
  m_ui->m_dataSet1LineEdit->setText(QString::fromStdString(m_inputLayer->getDataSetName()));
  m_ds = te::da::DataSourceManager::getInstance().find(m_inputLayer->getDataSourceId());

  if(!m_ds->isOpened())
    m_ds->open();

  getDataSets();
}

te::da::Join* te::qt::widgets::TableLinkDialog::getJoin()
{
  int pos = m_inputLayer->getDataSetName().find(".");
  std::string inputAlias = m_inputLayer->getDataSetName().substr(pos + 1, m_inputLayer->getDataSetName().size() - 1);

  if(pos != std::string::npos)
    inputAlias = m_inputLayer->getDataSetName().substr(pos + 1, m_inputLayer->getDataSetName().size() - 1);
  else
    inputAlias = m_inputLayer->getDataSetName();

  te::da::DataSetName* inField  = new te::da::DataSetName(m_inputLayer->getDataSetName(), inputAlias);
  te::da::DataSetName* tabField = new te::da::DataSetName(m_ui->m_dataSet2ComboBox->currentText().toStdString(), m_ui->m_dataSetAliasLineEdit->text().toStdString());

  te::da::Expression* exp1 = new te::da::PropertyName(m_ui->m_dataset1ColumnComboBox->currentText().toStdString());
  te::da::Expression* exp2 = new te::da::PropertyName(m_ui->m_dataset2ColumnComboBox->currentText().toStdString());
  te::da::Expression* expression = new te::da::BinaryFunction("=", exp1, exp2);

  te::da::JoinType type = m_fieldsDialog->getJoinType();

  te::da::Join* join = new te::da::Join(inField, tabField, type, new te::da::JoinConditionOn(expression));
  return join;
}

te::da::Select te::qt::widgets::TableLinkDialog::getSelectQuery()
{
  //fields
  te::da::Fields* fields = m_fieldsDialog->getFields();

  //from
  te::da::From* from = new te::da::From;

  //Join
  from->push_back(getJoin());

  //build the select object
  te::da::Select s(fields, from);

  return s;
}

te::map::AbstractLayerPtr te::qt::widgets::TableLinkDialog::getQueryLayer()
{
  static boost::uuids::basic_random_generator<boost::mt19937> gen;
  boost::uuids::uuid u = gen();
  std::string id = boost::uuids::to_string(u);

  std::string title = m_ui->m_layerTitleLineEdit->text().toStdString();

  te::map::QueryLayerPtr layer(new te::map::QueryLayer(id, title));
  layer->setDataSourceId(m_ds->getId());
  layer->setRendererType("ABSTRACT_LAYER_RENDERER");
  layer->setQuery(new te::da::Select(getSelectQuery()));
  layer->computeExtent();

  // SRID
  std::auto_ptr<const te::map::LayerSchema> schema(layer->getSchema());
  te::gm::GeometryProperty* gp = te::da::GetFirstGeomProperty(schema.get());
  layer->setSRID(gp->getSRID());
  
  // style
  layer->setStyle(te::se::CreateFeatureTypeStyle(gp->getGeometryType()));

  return layer;
}

void te::qt::widgets::TableLinkDialog::getDataSets()
{
  std::string dsId = m_ds->getId();

  std::vector<std::string> datasetNames;

  te::da::GetDataSetNames(datasetNames, dsId);

  for (size_t i = 0; i < datasetNames.size(); i++)
  {
    if(datasetNames[i] != m_inputLayer->getDataSetName())
      m_ui->m_dataSet2ComboBox->addItem(QString::fromStdString(datasetNames[i]));
  }

  std::string DsName = m_ui->m_dataSet2ComboBox->currentText().toStdString();
  int pos = DsName.find(".");
  if(pos != std::string::npos)
    m_ui->m_dataSetAliasLineEdit->setText(QString::fromStdString(DsName.substr(pos + 1, DsName.size() - 1)));
  else
    m_ui->m_dataSetAliasLineEdit->setText(QString::fromStdString(DsName));
}

void te::qt::widgets::TableLinkDialog::getProperties()
{
  //Clearing contents
  int index = m_ui->m_dataset1ColumnComboBox->currentIndex();
  m_ui->m_dataset1ColumnComboBox->clear();
  m_ui->m_dataset2ColumnComboBox->clear();

  m_fieldsDialog->clearInputValues();
  m_fieldsDialog->clearOutputValues();

  //get the dataset names
  std::vector<std::string> datasetNames = m_ds->getDataSetNames();
  std::vector<std::pair<std::string, std::string> > dataSetSelecteds;
  std::string inputAlias;
  int pos = m_inputLayer->getDataSetName().find(".");

  if(pos != std::string::npos)
    inputAlias = m_inputLayer->getDataSetName().substr(pos + 1, m_inputLayer->getDataSetName().size() - 1);
  else
   inputAlias = m_inputLayer->getDataSetName();

  dataSetSelecteds.push_back(std::make_pair(m_inputLayer->getDataSetName(), inputAlias));
  dataSetSelecteds.push_back(std::make_pair(m_ui->m_dataSet2ComboBox->currentText().toStdString(), m_ui->m_dataSetAliasLineEdit->text().toStdString()));

  std::vector<std::string> propertyNames;
  std::vector<std::string> geomProperties;

  //get properties for each data set
  for(size_t t = 0; t < dataSetSelecteds.size(); ++t)
  {
    //alias name
    std::string alias = dataSetSelecteds[t].second;

    //data set name
    std::string dataSetName = dataSetSelecteds[t].first;

    //get datasettype
    std::auto_ptr<te::da::DataSetType> dsType(0);

    //Acquiring the dataSet properties
    std::vector<std::size_t> dataSetProperties;


    for(unsigned int i = 0; i < datasetNames.size(); ++i)
    {
      if(datasetNames[i] == dataSetName)
      {
        dsType = m_ds->getDataSetType(datasetNames[i]);
        break;
      }
    }

    if(dsType.get())
    {
      for(size_t i = 0; i < dsType->size(); ++i)
      {
        std::string propName = dsType->getProperty(i)->getName();
        std::string fullName = alias + "." + propName;

        if(propName == "FID")
          continue;

        std::auto_ptr<te::da::DataSet> dataSet = m_ds->getDataSet(alias);

        dataSetProperties.push_back(i);

        if(dsType->getProperty(i)->getType() != te::dt::GEOMETRY_TYPE)
          propertyNames.push_back(alias + "." + dataSet->getPropertyName(i));
        else
          geomProperties.push_back(fullName);

        if(t == 0)
          m_ui->m_dataset1ColumnComboBox->addItem(QString::fromStdString(fullName), QVariant(dsType->getProperty(i)->getType()));
        else
        {
          m_ui->m_dataset2ColumnComboBox->addItem(QString::fromStdString(fullName), QVariant(dsType->getProperty(i)->getType()));

          if(i == dsType->size() - 1)
          {
            //Adjusting the table that will display the tabular dataset
            m_tabularView->setDataSet(new te::mem::DataSet(*dataSet.get(), dataSetProperties, 5));
            m_tabularView->resizeColumnsToContents();
          }
        }
      }
    }
  }

  if(index != -1)
    m_ui->m_dataset1ColumnComboBox->setCurrentIndex(index);

  //Adjusting the widget that is used to configure the query's field
  m_fieldsDialog->setLeftLabel("Non-selected fields");
  m_fieldsDialog->setRightLabel("Selected fields");
  m_fieldsDialog->setOutputValues(propertyNames);
  m_fieldsDialog->setFixedOutputValues(geomProperties, "geometry");
}

void te::qt::widgets::TableLinkDialog::done(int r)
{
  if(QDialog::Accepted == r)
  {
     if(m_ui->m_dataset1ColumnComboBox->itemData(m_ui->m_dataset1ColumnComboBox->currentIndex())
       == m_ui->m_dataset2ColumnComboBox->itemData(m_ui->m_dataset2ColumnComboBox->currentIndex()))
      {
          QDialog::done(r);
          return;
      }
      else
      {
        QMessageBox::warning(this, tr("Tabular File"), "The types of the selected columns do not match.");
        return;
      }
  }
  else
  {
      QDialog::done(r);
      return;
  }
}

void te::qt::widgets::TableLinkDialog::onDataCBIndexChanged(int index)
{
  std::string DsName = m_ui->m_dataSet2ComboBox->currentText().toStdString();
  int pos = DsName.find(".");
  if(pos != std::string::npos)
    m_ui->m_dataSetAliasLineEdit->setText(QString::fromStdString(DsName.substr(pos + 1, DsName.size() - 1)));
  else
    m_ui->m_dataSetAliasLineEdit->setText(QString::fromStdString(DsName));

  getProperties();
}

void te::qt::widgets::TableLinkDialog::onDataToolButtonnClicked()
{
  if(m_ui->m_tabularFrame->isHidden())
  {
    m_tabularView->show();
    m_ui->m_tabularFrame->show();
    m_ui->m_dataPreviewGroupBox->show();
  }
  else
  {
    m_ui->m_tabularFrame->hide();
    m_tabularView->hide();
    m_ui->m_dataPreviewGroupBox->hide();
  }
}

void te::qt::widgets::TableLinkDialog::onAdvancedToolButtonnClicked()
{
  int res = m_fieldsDialog->exec();
  if(res != QDialog::Accepted)
  {
    getProperties();
  }
}

void te::qt::widgets::TableLinkDialog::onOkPushButtonClicked()
{
  this->accept();
}
