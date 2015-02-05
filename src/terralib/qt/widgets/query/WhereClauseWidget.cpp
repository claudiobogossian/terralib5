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
  \file terralib/qt/widgets/query/WhereClauseWidget.cpp

  \brief This file has the WhereClauseWidget class.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../dataaccess/datasource/DataSourceCatalog.h"
#include "../../../dataaccess/query/BinaryFunction.h"
#include "../../../dataaccess/query/DataSetName.h"
#include "../../../dataaccess/query/Distinct.h"
#include "../../../dataaccess/query/Field.h"
#include "../../../dataaccess/query/Fields.h"
#include "../../../dataaccess/query/From.h"
#include "../../../dataaccess/query/LiteralDouble.h"
#include "../../../dataaccess/query/LiteralGeom.h"
#include "../../../dataaccess/query/LiteralInt16.h"
#include "../../../dataaccess/query/LiteralInt32.h"
#include "../../../dataaccess/query/LiteralString.h"
#include "../../../dataaccess/query/OrderByItem.h"
#include "../../../dataaccess/query/PropertyName.h"
#include "../../../dataaccess/query/Select.h"
#include "../../../dataaccess/query/SQLVisitor.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/Geometry.h"
#include "../../../geometry/GeometryCollection.h"
#include "../../../geometry/GeometryProperty.h"
#include "ui_WhereClauseWidgetForm.h"
#include "WhereClauseWidget.h"

// Qt
#include <QIcon>
#include <QMessageBox>
#include <QToolButton>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);


te::qt::widgets::WhereClauseWidget::WhereClauseWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::WhereClauseWidgetForm)
{
  m_ui->setupUi(this);

  m_ds.reset();

  // set icons
  m_ui->m_addWhereClausePushButton->setIcon(QIcon::fromTheme("list-add"));
  m_ui->m_clearAllPushButton->setIcon(QIcon::fromTheme("edit-clear"));

  //connects
  connect(m_ui->m_addWhereClausePushButton, SIGNAL(clicked()), this, SLOT(onAddWhereClausePushButtonClicked()));
  connect(m_ui->m_clearAllPushButton, SIGNAL(clicked()), this, SLOT(onClearAllPushButtonClicked()));
  connect(m_ui->m_valueValueRadioButton, SIGNAL(clicked()), this, SLOT(onValuePropertyRadioButtonClicked()));
  connect(m_ui->m_restrictValueComboBox, SIGNAL(activated(QString)), this, SLOT(onRestrictValueComboBoxActivated(QString)));

  m_ui->m_sqlTextEdit->setReadOnly(true);

  m_count = 0;
  m_srid = 0;
}

te::qt::widgets::WhereClauseWidget::~WhereClauseWidget()
{
  clear();
}

Ui::WhereClauseWidgetForm* te::qt::widgets::WhereClauseWidget::getForm() const
{
  return m_ui.get();
}

te::da::Where* te::qt::widgets::WhereClauseWidget::getWhere()
{
  int row = m_ui->m_whereClauseTableWidget->rowCount();

  if(row == 0)
    return 0;

  te::da::Expression* leftSide = 0;
  std::string lastConnectorName = "";

  for(int i = 0; i < row; ++i)
  {
    //create binary function
    std::string propName = "";
    QTableWidgetItem* itemPropName = m_ui->m_whereClauseTableWidget->item(i, 1);
    if(itemPropName)
      propName = itemPropName->text().toStdString();
    else
    {
      QWidget* w = m_ui->m_whereClauseTableWidget->cellWidget(i, 1);
      QComboBox* cmbBox = dynamic_cast<QComboBox*>(w);
      if(cmbBox)
        propName = cmbBox->currentText().toStdString();
    }

    std::string funcName = "";
    QTableWidgetItem* itemFuncName = m_ui->m_whereClauseTableWidget->item(i, 2);
    if(itemFuncName)
      funcName = itemFuncName->text().toStdString();
    else
    {
      QWidget* w = m_ui->m_whereClauseTableWidget->cellWidget(i, 2);
      QComboBox* cmbBox = dynamic_cast<QComboBox*>(w);
      if(cmbBox)
        funcName = cmbBox->currentText().toStdString();
    }

    int expIdx = -1;
    QTableWidgetItem* itemValue = m_ui->m_whereClauseTableWidget->item(i, 3);
    if(itemValue)
    {
      expIdx = itemValue->data(Qt::UserRole).toInt();
    }
    else
    {
      QWidget* w = m_ui->m_whereClauseTableWidget->cellWidget(i, 3);
      QComboBox* cmbBox = dynamic_cast<QComboBox*>(w);
      if(cmbBox)
        expIdx = m_comboMap[cmbBox].first;
      else
        return 0;
    }


    te::da::Expression* exp1 = new te::da::PropertyName(propName);
    te::da::Expression* exp2 = m_mapExp[expIdx]->m_expression->clone();

    te::da::BinaryFunction* func = new te::da::BinaryFunction(funcName, exp1, exp2);

    //check left side expression
    if(leftSide != 0 && lastConnectorName.empty() == false)
    {
      te::da::BinaryFunction* connectFunc = new te::da::BinaryFunction(lastConnectorName, leftSide, func);
      leftSide = connectFunc;
    }
    else
    {
      leftSide = func;
    }

    //check connector
    QTableWidgetItem* itemConnectorName = m_ui->m_whereClauseTableWidget->item(i, 4);
    if(itemConnectorName)
    {
      if(itemPropName->text().isEmpty() == false)
        lastConnectorName = itemConnectorName->text().toStdString();
      else
        lastConnectorName = "";
    }
    else
    {
      QWidget* w = m_ui->m_whereClauseTableWidget->cellWidget(i, 4);
      QComboBox* cmbBox = dynamic_cast<QComboBox*>(w);
      if(cmbBox)
        lastConnectorName = cmbBox->currentText().toStdString();
    }
  }

  te::da::Where* w = new te::da::Where(leftSide);

  return w;
}

std::string te::qt::widgets::WhereClauseWidget::getWhereString()
{
  std::string sql = "";

  te::da::Where* w = getWhere();

  if(w)
  {
    te::da::SQLVisitor visitor(*m_ds->getDialect(), sql);
    te::da::Expression* exp = w->getExp();

    try
    {
      exp->accept(visitor);
    }
    catch(...)
    {
      delete w;
      return "";
    }
  }

  delete w;

  return sql;
}

void te::qt::widgets::WhereClauseWidget::setDataSource(const te::da::DataSourcePtr& ds)
{
  m_ds = ds;
}

void te::qt::widgets::WhereClauseWidget::setLayerList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_ui->m_layerComboBox->clear();

  std::list<te::map::AbstractLayerPtr>::iterator it = layerList.begin();

  while(it != layerList.end())
  {
    te::map::AbstractLayerPtr l = *it;

    std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

    if(dsType.get())
    {
      if(dsType->hasGeom())
        m_ui->m_layerComboBox->addItem(l->getTitle().c_str(), QVariant::fromValue(l));
    }
    ++it;
  }
}

void te::qt::widgets::WhereClauseWidget::setFromItems(std::vector<std::pair<std::string, std::string> > vec)
{
  m_fromItems = vec;
}

void te::qt::widgets::WhereClauseWidget::setAttributeList(const std::vector<std::string>& vec)
{
  m_ui->m_restrictValueComboBox->clear();
  m_ui->m_valuePropertyComboBox->clear();

  for(size_t t = 0; t <vec.size(); ++t)
  {
    m_ui->m_restrictValueComboBox->addItem(vec[t].c_str());
    m_ui->m_valuePropertyComboBox->addItem(vec[t].c_str());
  }
}

void  te::qt::widgets::WhereClauseWidget::setGeomAttributeList(const std::vector<std::string>& vec, int srid)
{
  m_srid = srid;

  m_ui->m_geomAttrComboBox->clear();

  for(size_t t = 0; t <vec.size(); ++t)
  {
    m_ui->m_geomAttrComboBox->addItem(vec[t].c_str());
  }
}

void te::qt::widgets::WhereClauseWidget::setOperatorsList(const std::vector<std::string>& vec)
{
  m_ui->m_OperatorComboBox->clear();

  for(size_t t = 0; t <vec.size(); ++t)
  {
    m_ui->m_OperatorComboBox->addItem(vec[t].c_str());
  }
}

void te::qt::widgets::WhereClauseWidget::setSpatialOperatorsList(const std::vector<std::string>& vec)
{
  m_ui->m_SpatialOperatorComboBox->clear();

  for(size_t t = 0; t <vec.size(); ++t)
  {
    m_ui->m_SpatialOperatorComboBox->addItem(vec[t].c_str());
  }
}

void te::qt::widgets::WhereClauseWidget::setConnectorsList(const std::vector<std::string>& vec)
{
  m_connectorsList.clear();

  m_connectorsList.append("");

  for(size_t t = 0; t <vec.size(); ++t)
  {
    m_connectorsList.append(vec[t].c_str());
  }
}

void te::qt::widgets::WhereClauseWidget::clear()
{
  te::common::FreeContents(m_mapExp);

  m_mapExp.clear();

  m_comboMap.clear();

  m_ui->m_whereClauseTableWidget->setRowCount(0);

  m_count = 0;
}

void te::qt::widgets::WhereClauseWidget::resetInterface()
{
  clear();

  m_ds.reset();
  m_srid = 0;

  m_ui->m_restrictValueComboBox->clear();
  m_ui->m_valuePropertyComboBox->clear();
  m_ui->m_valueValueComboBox->clear();
  m_ui->m_geomAttrComboBox->clear();
  m_ui->m_OperatorComboBox->clear();
  m_ui->m_SpatialOperatorComboBox->clear();
  m_ui->m_sqlTextEdit->clear();
  m_ui->m_tabWidget->setCurrentIndex(0);
}

void te::qt::widgets::WhereClauseWidget::onAddWhereClausePushButtonClicked()
{
  std::string restrictValue = "";
  std::string operatorStr = "";
  std::string valueStr = "";

  if(m_ui->m_criteriaTabWidget->currentIndex() == 0) // criteria by attribute restriction
  {
    int expId = ++m_count;

    if(m_ui->m_restrictValueComboBox->currentText().isEmpty())
    {
      QMessageBox::warning(this, tr("Query Builder"), tr("Restrict value not defined."));
      return;
    }

    if(m_ui->m_valuePropertyRadioButton->isChecked() == false &&
       m_ui->m_valueValueRadioButton->isChecked() == false)
    {
       if(m_ui->m_valueValueComboBox->currentText().isEmpty())
      {
        QMessageBox::warning(this, tr("Query Builder"), tr("Value not defined."));
        return;
      }
    }

    if(m_ui->m_valueValueRadioButton->isChecked())
    {
      if(m_ui->m_valueValueComboBox->currentText().isEmpty())
      {
        QMessageBox::warning(this, tr("Query Builder"), tr("Value not defined."));
        return;
      }
    }

    restrictValue = m_ui->m_restrictValueComboBox->currentText().toStdString();

    if(m_ui->m_OperatorComboBox->currentText().isEmpty())
    {
      QMessageBox::warning(this, tr("Query Builder"), tr("Operator not defined."));
      return;
    }
    operatorStr = m_ui->m_OperatorComboBox->currentText().toStdString();

    ExpressionProperty* ep = new ExpressionProperty();
    ep->m_isAttributeCriteria = true;
    ep->m_property = restrictValue;
    ep->m_operator = operatorStr;

    if(m_ui->m_valuePropertyRadioButton->isChecked())
    {
      valueStr = m_ui->m_valuePropertyComboBox->currentText().toStdString();

      te::da::Expression* exp = new te::da::PropertyName(valueStr);

      ep->m_isPropertyValue = true;
      ep->m_value = valueStr;
      ep->m_expression = exp;

      m_mapExp.insert(std::map<int, ExpressionProperty*>::value_type(expId, ep));
    }
    else if(m_ui->m_valueValueRadioButton->isChecked())
    {
      valueStr = m_ui->m_valueValueComboBox->currentText().toStdString();

      te::da::Expression* exp = getExpression(m_ui->m_valueValueComboBox->currentText(), restrictValue);

      ep->m_isValueValue = true;
      ep->m_value = valueStr;
      ep->m_expression = exp;

      m_mapExp.insert(std::map<int, ExpressionProperty*>::value_type(expId, ep));
    }

    std::string connector = tr("and").toStdString();

    //new entry
    int newrow = m_ui->m_whereClauseTableWidget->rowCount();

    m_ui->m_whereClauseTableWidget->insertRow(newrow);

    //remove button
    QToolButton* removeBtn = new QToolButton(m_ui->m_whereClauseTableWidget);
    removeBtn->setIcon(QIcon::fromTheme("list-remove"));
    connect(removeBtn, SIGNAL(clicked()), this, SLOT(onRemoveWhereClausePushButtonClicked()));
    m_ui->m_whereClauseTableWidget->setCellWidget(newrow, 0, removeBtn);

    //property combo
    QComboBox* cmbProperty = new QComboBox(m_ui->m_whereClauseTableWidget);
    connect(cmbProperty, SIGNAL(activated(QString)), this, SLOT(onComboBoxActivated(QString)));
    std::pair<int, int> pairProperty(expId, 1);
    m_comboMap.insert(std::map< QComboBox*, std::pair<int, int> >::value_type(cmbProperty, pairProperty));
    copyCombo(m_ui->m_restrictValueComboBox, cmbProperty, restrictValue);
    m_ui->m_whereClauseTableWidget->setCellWidget(newrow, 1, cmbProperty);

    //operator combo
    QComboBox* cmbOperator = new QComboBox(m_ui->m_whereClauseTableWidget);
    connect(cmbOperator, SIGNAL(activated(QString)), this, SLOT(onComboBoxActivated(QString)));
    std::pair<int, int> pairOperator(expId, 2);
    m_comboMap.insert(std::map< QComboBox*, std::pair<int, int> >::value_type(cmbOperator, pairOperator));
    copyCombo(m_ui->m_OperatorComboBox, cmbOperator, operatorStr);
    m_ui->m_whereClauseTableWidget->setCellWidget(newrow, 2, cmbOperator);

    //value combo
    QComboBox* cmbValue = new QComboBox(m_ui->m_whereClauseTableWidget);
    connect(cmbValue, SIGNAL(activated(QString)), this, SLOT(onComboBoxActivated(QString)));
    connect(cmbValue, SIGNAL(editTextChanged(QString)), this, SLOT(onComboBoxActivated(QString)));
    std::pair<int, int> pairValue(expId, 3);
    m_comboMap.insert(std::map< QComboBox*, std::pair<int, int> >::value_type(cmbValue, pairValue));
    if(m_ui->m_valuePropertyRadioButton->isChecked())
      copyCombo(m_ui->m_valuePropertyComboBox, cmbValue, valueStr);
    else
    {
      cmbValue->setEditable(true);
      copyCombo(m_ui->m_valueValueComboBox, cmbValue, valueStr);
    }
    m_ui->m_whereClauseTableWidget->setCellWidget(newrow, 3, cmbValue);
    ep->m_valuesComboBox = cmbValue;

    if(ep->m_valuesComboBox->currentText() != valueStr.c_str())
    {
      int nrow = ep->m_valuesComboBox->count();
      ep->m_valuesComboBox->addItem(valueStr.c_str());
      ep->m_valuesComboBox->setCurrentIndex(nrow);
    }

    //connector information
    QComboBox* connectorCmbBox = new QComboBox(m_ui->m_whereClauseTableWidget);
    connect(connectorCmbBox, SIGNAL(activated(QString)), this, SLOT(onComboBoxActivated(QString)));
    std::pair<int, int> pairConnector(expId, 4);
    m_comboMap.insert(std::map< QComboBox*, std::pair<int, int> >::value_type(connectorCmbBox, pairConnector));
    connectorCmbBox->addItems(m_connectorsList);
    m_ui->m_whereClauseTableWidget->setCellWidget(newrow, 4, connectorCmbBox);

    for(int i = 0; i < connectorCmbBox->count(); ++i)
    {
      if(connectorCmbBox->itemText(i).toStdString() == connector)
      {
        connectorCmbBox->setCurrentIndex(i);
        break;
      }
    }
  }
  else // criteria by spatial restriction
  {
    if(m_ui->m_SpatialOperatorComboBox->currentText().isEmpty())
    {
      QMessageBox::warning(this, tr("Query Builder"), tr("Operator not defined."));
      return;
    }
    operatorStr = m_ui->m_SpatialOperatorComboBox->currentText().toStdString();

    restrictValue = m_ui->m_geomAttrComboBox->currentText().toStdString();

    //get layer
    int layerIdx = m_ui->m_layerComboBox->currentIndex();
    QVariant varLayer = m_ui->m_layerComboBox->itemData(layerIdx, Qt::UserRole);
    te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

    std::auto_ptr<const te::map::LayerSchema> schema(layer->getSchema());

    te::gm::GeometryProperty* prop = te::da::GetFirstGeomProperty(schema.get());

    if(!prop)
    {
      QMessageBox::warning(this, tr("Query Builder"), tr("Selected layer has no geometry property."));
      return;
    }

    std::auto_ptr<te::da::DataSet> ds;

    if(m_ui->m_selectedObjCheckBox->isChecked())
    {
      const te::da::ObjectIdSet* selecteds = layer->getSelected();

      if(!selecteds || selecteds->size() == 0)
      {
        QMessageBox::warning(this, tr("Query Builder"), tr("Selected layer has no selected geometries."));
        return;
      }

      ds = layer->getData(selecteds);
    }
    else
    {
      ds = layer->getData();
    }

    ds->moveBeforeFirst();

    size_t dsSize = ds->size();
    size_t count = 0;

    //get all geometries
    while(ds->moveNext())
    {
      
      int expId = ++m_count;

      te::gm::Geometry* geom = ds->getGeometry(prop->getName()).release();

      geom->setSRID(layer->getSRID());

      //convert
      if(layer->getSRID() != m_srid)
      {
        geom->transform(m_srid);
      }

      //valueStr = geom->toString();
      valueStr = tr("Geometry Value").toStdString();

      //create expression
      te::da::LiteralGeom* lGeom = new te::da::LiteralGeom(geom);

      ExpressionProperty* ep = new ExpressionProperty();
      ep->m_isSpatialCriteria = true;
      ep->m_property = restrictValue;
      ep->m_operator = operatorStr;
      ep->m_value = valueStr;
      ep->m_expression = lGeom;

      m_mapExp.insert(std::map<int, ExpressionProperty*>::value_type(expId, ep));

      //set connector
      std::string connector = "";

      if(count < dsSize - 1)
        connector = tr("or").toStdString();

      ++count;

      //new entry
      int newrow = m_ui->m_whereClauseTableWidget->rowCount();

      m_ui->m_whereClauseTableWidget->insertRow(newrow);

      //remove button
      QToolButton* removeBtn = new QToolButton(m_ui->m_whereClauseTableWidget);
      removeBtn->setIcon(QIcon::fromTheme("list-remove"));
      connect(removeBtn, SIGNAL(clicked()), this, SLOT(onRemoveWhereClausePushButtonClicked()));
      m_ui->m_whereClauseTableWidget->setCellWidget(newrow, 0, removeBtn);

      //property combo
      QComboBox* cmbProperty = new QComboBox(m_ui->m_whereClauseTableWidget);
      connect(cmbProperty, SIGNAL(activated(QString)), this, SLOT(onComboBoxActivated(QString)));
      std::pair<int, int> pairProperty(expId, 1);
      m_comboMap.insert(std::map< QComboBox*, std::pair<int, int> >::value_type(cmbProperty, pairProperty));
      copyCombo(m_ui->m_geomAttrComboBox, cmbProperty, restrictValue);
      m_ui->m_whereClauseTableWidget->setCellWidget(newrow, 1, cmbProperty);

       //operator combo
      QComboBox* cmbOperator = new QComboBox(m_ui->m_whereClauseTableWidget);
      connect(cmbOperator, SIGNAL(activated(QString)), this, SLOT(onComboBoxActivated(QString)));
      std::pair<int, int> pairOperator(expId, 2);
      m_comboMap.insert(std::map< QComboBox*, std::pair<int, int> >::value_type(cmbOperator, pairOperator));
      copyCombo(m_ui->m_SpatialOperatorComboBox, cmbOperator, operatorStr);
      m_ui->m_whereClauseTableWidget->setCellWidget(newrow, 2, cmbOperator);

      //value combo
      QTableWidgetItem* itemValue = new QTableWidgetItem(valueStr.c_str());
      itemValue->setData(Qt::UserRole, QVariant(expId));
      m_ui->m_whereClauseTableWidget->setItem(newrow, 3, itemValue);

      //connector information
      QComboBox* connectorCmbBox = new QComboBox(m_ui->m_whereClauseTableWidget);
      connect(connectorCmbBox, SIGNAL(activated(QString)), this, SLOT(onComboBoxActivated(QString)));
      std::pair<int, int> pairConnector(expId, 4);
      m_comboMap.insert(std::map< QComboBox*, std::pair<int, int> >::value_type(connectorCmbBox, pairConnector));
      connectorCmbBox->addItems(m_connectorsList);
      m_ui->m_whereClauseTableWidget->setCellWidget(newrow, 4, connectorCmbBox);
      
      for(int i = 0; i < connectorCmbBox->count(); ++i)
      {
        if(connectorCmbBox->itemText(i).toStdString() == connector)
        {
          connectorCmbBox->setCurrentIndex(i);
          break;
        }
      }
    }
  }

  m_ui->m_whereClauseTableWidget->resizeColumnsToContents();

  //get string sql
  std::string sql = getWhereString();

  m_ui->m_sqlTextEdit->setText(sql.c_str());
}

void te::qt::widgets::WhereClauseWidget::onRemoveWhereClausePushButtonClicked()
{
  QToolButton* button = dynamic_cast<QToolButton*>(sender());
  if(button) 
  { 
    int row = -1;

    for(int i = 0; i < m_ui->m_whereClauseTableWidget->rowCount(); ++i)
    {
      QWidget* w = m_ui->m_whereClauseTableWidget->cellWidget(i, 0);
      if(button == w)
      {
        row = i;
        break;
      }
    }

    if(row >= 0)
      m_ui->m_whereClauseTableWidget->removeRow(row);

    m_ui->m_whereClauseTableWidget->resizeColumnsToContents();
  }

  //get string sql
  std::string sql = getWhereString();

  m_ui->m_sqlTextEdit->setText(sql.c_str());
}

void te::qt::widgets::WhereClauseWidget::onValuePropertyRadioButtonClicked()
{
  std::string propertyName = m_ui->m_restrictValueComboBox->currentText().toStdString();

  m_ui->m_valueValueComboBox->clear();
  m_ui->m_valueValueComboBox->addItems(getPropertyValues(propertyName));
}

void te::qt::widgets::WhereClauseWidget::onClearAllPushButtonClicked()
{
  m_ui->m_whereClauseTableWidget->setRowCount(0);
  m_ui->m_whereClauseTableWidget->resizeColumnsToContents();

  //get string sql
  std::string sql = getWhereString();

  m_ui->m_sqlTextEdit->setText(sql.c_str());
}

void te::qt::widgets::WhereClauseWidget::onRestrictValueComboBoxActivated(QString value)
{
  if(value.isEmpty() || !m_ui->m_valueValueRadioButton->isChecked())
    return;

  onValuePropertyRadioButtonClicked();
}

void te::qt::widgets::WhereClauseWidget::onComboBoxActivated(QString value)
{
  QComboBox* cmbBox = dynamic_cast<QComboBox*>(sender());
  if(!cmbBox) 
    return;

  int expId = m_comboMap[cmbBox].first;
  int column = m_comboMap[cmbBox].second;

  ExpressionProperty* ep = m_mapExp[expId];

  if(!ep)
    return;

  if(ep->m_isAttributeCriteria)
  {
    //update expression property
    if(column == 1) //property
    {
      ep->m_property = value.toStdString();

      if(ep->m_isValueValue)
      {
        ep->m_valuesComboBox->clear();
        ep->m_valuesComboBox->addItems(getPropertyValues(ep->m_property));
        ep->m_value = ep->m_valuesComboBox->currentText().toStdString();
      }
    }
    else if(column == 2) //operator
    {
      ep->m_operator = value.toStdString();
    }
    else if(column == 3) //value
    {
      ep->m_value = value.toStdString();
    }

    //re-create expression
    te::da::Expression* exp = 0;

    if(ep->m_isValueValue)
    {
      exp = getExpression(ep->m_value.c_str(), ep->m_property);
    }

    if(ep->m_isPropertyValue)
    {
      exp = new te::da::PropertyName(ep->m_property);
    }

    delete ep->m_expression;

    ep->m_expression = exp;
  }

  //get string sql
  std::string sql = getWhereString();

  m_ui->m_sqlTextEdit->setText(sql.c_str());
}

te::da::Expression* te::qt::widgets::WhereClauseWidget::getExpression(const QString& value, const std::string& propName)
{
  std::string dataSetName;
  std::string dataSetAliasName;
  std::string propertyName = propName;

  std::size_t pos = propName.find(".");
  if(pos != std::string::npos)
  {
    dataSetAliasName = propName.substr(0, pos);
    propertyName = propName.substr(pos + 1, propName.size() - 1);
  }

  //get the dataset name
  if(m_fromItems.size() == 1)
  {
    dataSetName = m_fromItems[0].first;
  }
  else
  {
    for(size_t t = 0; t < m_fromItems.size(); ++t)
    {
      if(m_fromItems[t].second == dataSetAliasName)
      {
        dataSetName = m_fromItems[t].first;
        break;
      }
    }
  }

  if(dataSetName.empty())
    return 0;

  //get the dataset property type
  std::auto_ptr<te::da::DataSetType> dsType = m_ds->getDataSetType(dataSetName);

  if(dsType.get())
  {
    te::dt::Property* prop = dsType->getProperty(propertyName);

    te::da::Literal* l = 0;

    if(prop)
    {
      if(prop->getType() == te::dt::DOUBLE_TYPE)
      {
        l = new te::da::LiteralDouble(value.toDouble());
      }
      else if(prop->getType() == te::dt::INT16_TYPE)
      {
        l = new te::da::LiteralInt16(value.toInt());
      }
      else if(prop->getType() == te::dt::INT32_TYPE)
      {
        l = new te::da::LiteralInt32(value.toInt());
      }
      else if(prop->getType() == te::dt::STRING_TYPE)
      {
        l = new te::da::LiteralString(value.toStdString());
      }
    }

    return l;
  }

  return 0;
}

void te::qt::widgets::WhereClauseWidget::copyCombo(QComboBox* input, QComboBox* output, std::string curValue)
{
  int idx = 0;
  
  for(int i = 0; i < input->count(); ++i)
  {
    output->addItem(input->itemText(i));

    if(!curValue.empty() && input->itemText(i).toStdString() == curValue)
      idx = i;
  }

  output->setCurrentIndex(idx);
}

QStringList te::qt::widgets::WhereClauseWidget::getPropertyValues(std::string propertyName)
{
  QStringList list;

  if(m_ds.get() == 0)
    return list;

  te::da::Fields* fields = new te::da::Fields;
  te::da::Field* f = new te::da::Field(new te::da::PropertyName(propertyName));
  fields->push_back(f);

  te::da::From* from = new te::da::From;

  for(size_t t = 0; t < m_fromItems.size(); ++t)
  {
    te::da::FromItem* fi = new te::da::DataSetName(m_fromItems[t].first, m_fromItems[t].second);

    from->push_back(fi);
  }

  te::da::Select* select = new te::da::Select();

  select->setFields(fields);
  select->setFrom(from);

  std::auto_ptr<te::da::DataSet> dataset;

  try
  {
    dataset = m_ds->query(*select);
  }
  catch(const std::exception& e)
  {
    std::string msg =  "An exception has occurred: ";
                msg += e.what();

    QMessageBox::warning(0, "Query Error: ", msg.c_str());

    return list;
  }
  catch(...)
  {
    std::string msg =  "An unexpected exception has occurred!";

    QMessageBox::warning(0, "Query Error: ", msg.c_str());

    return list;
  }

  std::size_t pos = propertyName.find(".");
  if(pos != std::string::npos)
  {
    std::string dataSetAliasName = propertyName.substr(0, pos);
    propertyName = propertyName.substr(pos + 1, propertyName.size() - 1);
  }

  std::set<std::string> valuesStr;
  std::set<double> valuesDouble;
  std::set<int> valuesInt;

  bool found = false;
  std::size_t propertyPos;
  for(std::size_t t = 0; t < dataset->getNumProperties(); ++t)
  {
    if(dataset->getPropertyName(t) == propertyName)
    {
      propertyPos = t;
      found = true;
      break;
    }
  }

  if(!found)
  {
    return list;
  }

  int propertyType = dataset->getPropertyDataType(propertyPos);

  if(dataset.get())
  {
    while(dataset->moveNext())
    {
      if(!dataset->isNull(propertyName))
      {
        if(propertyType == te::dt::INT32_TYPE)
        {
          int value = dataset->getInt32(propertyName);

          std::set<int>::iterator it = valuesInt.find(value);

          if(it == valuesInt.end())
            valuesInt.insert(value);
        }
        else if(propertyType == te::dt::DOUBLE_TYPE || propertyType == te::dt::FLOAT_TYPE)
        {
          double value = dataset->getDouble(propertyName);

          std::set<double>::iterator it = valuesDouble.find(value);

          if(it == valuesDouble.end())
            valuesDouble.insert(value);
        }
        else 
        {
          std::string value = dataset->getAsString(propertyName);

          std::set<std::string>::iterator it = valuesStr.find(value);

          if(it == valuesStr.end())
            valuesStr.insert(value);
        }
      }
    }
  }

  if(propertyType == te::dt::INT32_TYPE)
  {
    std::set<int>::iterator it = valuesInt.begin();

    while(it != valuesInt.end())
    {
      list.append(QString::number(*it));

      ++it;
    }
  }
  else if(propertyType == te::dt::DOUBLE_TYPE || propertyType == te::dt::FLOAT_TYPE)
  {
    std::set<double>::iterator it = valuesDouble.begin();

    while(it != valuesDouble.end())
    {
      list.append(QString::number(*it, 'f', 5));

      ++it;
    }
  }
  else 
  {
    std::set<std::string>::iterator it = valuesStr.begin();

    while(it != valuesStr.end())
    {
      list.append((*it).c_str());

      ++it;
    }
  }

  return list;
}
