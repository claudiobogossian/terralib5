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
  \file terralib/qt/widgets/se/GroupingWidget.cpp

  \brief A widget used to build a grouping.
*/

// TerraLib
#include "../../../color/ColorBar.h"
#include "../../../common/Globals.h"
#include "../../../common/STLUtils.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/query/OrderByItem.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../maptools/Enums.h"
#include "../../../maptools/Grouping.h"
#include "../../../maptools/GroupingAlgorithms.h"
#include "../../../maptools/GroupingItem.h"
#include "../../../maptools/Utils.h"
#include "../../../maptools/QueryLayer.h"
#include "../../../se/SymbolizerColorFinder.h"
#include "../../../se/Utils.h"
#include "../colorbar/ColorBar.h"
#include "../colorbar/ColorCatalogWidget.h"
#include "../se/LineSymbolizerWidget.h"
#include "../se/PointSymbolizerWidget.h"
#include "../se/PolygonSymbolizerWidget.h"
#include "../se/SymbologyPreview.h"
#include "GroupingWidget.h"
#include "ui_GroupingWidgetForm.h"

// STL
#include <cassert>

// QT
#include <QDialogButtonBox>
#include <QMessageBox>

#define MAX_SLICES 200
#define PRECISION 15
#define NO_TITLE "No Value"

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::qt::widgets::GroupingWidget::GroupingWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::GroupingWidgetForm),
    m_cb(0)
{
  m_ui->setupUi(this);

  QGridLayout* l = new QGridLayout(m_ui->m_colorBarWidget);
  l->setContentsMargins(0,0,0,0);
  m_colorBar = new  te::qt::widgets::ColorCatalogWidget(m_ui->m_colorBarWidget);
  l->addWidget(m_colorBar);

//connects
  connect(m_colorBar, SIGNAL(colorBarChanged()), this, SLOT(onColorBarChanged()));
  connect(m_ui->m_typeComboBox, SIGNAL(activated(int)), this, SLOT(onTypeComboBoxActivated(int)));
  connect(m_ui->m_attrComboBox, SIGNAL(activated(int)), this, SLOT(onAttrComboBoxActivated(int)));
  connect(m_ui->m_applyPushButton, SIGNAL(clicked()), this, SLOT(onApplyPushButtonClicked()));
  connect(m_ui->m_tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(onTableWidgetItemDoubleClicked(QTableWidgetItem*)));
  connect(m_ui->m_importPushButton, SIGNAL(clicked()), this, SLOT(onImportPushButtonClicked()));

  //m_importGroupingGroupBox
  initialize();
}

te::qt::widgets::GroupingWidget::~GroupingWidget()
{
  delete m_cb;

  te::common::FreeContents(m_legend);
  m_legend.clear();
}

std::auto_ptr<te::map::Grouping> te::qt::widgets::GroupingWidget::getGrouping()
{
  if(m_ui->m_importGroupBox->isChecked())
  {
    QVariant varLayer = m_ui->m_layersComboBox->itemData(m_ui->m_layersComboBox->currentIndex(), Qt::UserRole);
    te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

    te::map::Grouping* ref = layer->getGrouping();

    std::auto_ptr<te::map::Grouping> group(new te::map::Grouping(*ref));

    return group;
  }

  std::string attr = m_ui->m_attrComboBox->currentText().toStdString();
  int attrIdx =  m_ui->m_attrComboBox->currentIndex();
  int attrType = m_ui->m_attrComboBox->itemData(attrIdx).toInt();

  int index = m_ui->m_typeComboBox->currentIndex();
  int type = m_ui->m_typeComboBox->itemData(index).toInt();

  std::auto_ptr<te::map::Grouping> group(new te::map::Grouping(attr, (te::map::GroupingType)type));

  group->setPropertyType(attrType);

  group->setNumSlices(m_ui->m_slicesSpinBox->value());

  group->setPrecision(m_ui->m_precSpinBox->value());

  group->setStdDeviation(m_ui->m_stdDevDoubleSpinBox->value());

  std::vector<te::map::GroupingItem*> groupingItems;
  for(std::size_t t = 0; t < m_legend.size(); ++t)
  {
    te::map::GroupingItem* gi = new te::map::GroupingItem(*m_legend[t]);

    groupingItems.push_back(gi);
  }
  group->setGroupingItems(groupingItems);

  return group;
}

void te::qt::widgets::GroupingWidget::initialize()
{
  // create color bar
  m_colorBar->getColorBar()->setHeight(20);
  m_colorBar->getColorBar()->setScaleVisible(false);

  // fill grouping type combo box
  m_ui->m_typeComboBox->addItem(tr("Equal Steps"), te::map::EQUAL_STEPS);
  m_ui->m_typeComboBox->addItem(tr("Quantil"), te::map::QUANTIL);
  m_ui->m_typeComboBox->addItem(tr("Standard Deviation"), te::map::STD_DEVIATION);
  m_ui->m_typeComboBox->addItem(tr("Unique Value"), te::map::UNIQUE_VALUE);

  onTypeComboBoxActivated(0);

  //set number of slices
  m_ui->m_slicesSpinBox->setMinimum(1);
  m_ui->m_slicesSpinBox->setMaximum(MAX_SLICES);
  m_ui->m_slicesSpinBox->setValue(5);
  m_ui->m_slicesSpinBox->setSingleStep(1);

  //set standard deviation values
  m_ui->m_stdDevDoubleSpinBox->setMinimum(0.25);
  m_ui->m_stdDevDoubleSpinBox->setMaximum(1.0);
  m_ui->m_stdDevDoubleSpinBox->setValue(0.5);
  m_ui->m_stdDevDoubleSpinBox->setSingleStep(0.25);

  //set number of precision
  m_ui->m_precSpinBox->setMinimum(1);
  m_ui->m_precSpinBox->setMaximum(PRECISION);
  m_ui->m_precSpinBox->setValue(6);
  m_ui->m_precSpinBox->setSingleStep(1);

  //adjust table
  m_ui->m_tableWidget->resizeColumnsToContents();
#if (QT_VERSION >= 0x050000)
  m_ui->m_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#else
  m_ui->m_tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#endif

  m_manual = false;
}

void te::qt::widgets::GroupingWidget::updateUi(bool loadColorBar)
{
  disconnect(m_ui->m_tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(onTableWidgetItemChanged(QTableWidgetItem*)));

  m_ui->m_tableWidget->setRowCount(0);

  int index = m_ui->m_typeComboBox->currentIndex();
  int type = m_ui->m_typeComboBox->itemData(index).toInt();

  if(type == te::map::EQUAL_STEPS || type == te::map::QUANTIL || type == te::map::STD_DEVIATION)
  {
    QStringList list;
    list.append(tr("Symbol"));
    list.append(tr("Title"));
    list.append(tr("Min"));
    list.append(tr("Max"));
    list.append(tr("Count"));

    m_ui->m_tableWidget->setColumnCount(5);
    m_ui->m_tableWidget->setHorizontalHeaderLabels(list);
  }
  else if(type == te::map::UNIQUE_VALUE)
  {
    QStringList list;
    list.append(tr("Symbol"));
    list.append(tr("Title"));
    list.append(tr("Value"));
    list.append(tr("Count"));

    m_ui->m_tableWidget->setColumnCount(4);
    m_ui->m_tableWidget->setHorizontalHeaderLabels(list);
  }

  te::color::ColorBar* cb = 0;

  if(loadColorBar)
  {
    if(!m_legend.empty() && !m_legend[0]->getSymbolizers().empty() && !m_legend[m_legend.size() - 1]->getSymbolizers().empty())
    {
      te::se::SymbolizerColorFinder scf;

      scf.find(m_legend[0]->getSymbolizers()[0]);
      te::color::RGBAColor initColor = scf.getColor();

      scf.find(m_legend[m_legend.size() - 1]->getSymbolizers()[0]);
      te::color::RGBAColor endColor = scf.getColor();

      cb = new te::color::ColorBar(initColor, endColor, 256);
    }
  }

  int count = 0;

  for(std::size_t t = 0; t < m_legend.size(); ++t)
  {
    te::map::GroupingItem* gi = m_legend[t];

    int newrow = m_ui->m_tableWidget->rowCount();
    m_ui->m_tableWidget->insertRow(newrow);

    //symbol
    {
      const std::vector<te::se::Symbolizer*>& ss = gi->getSymbolizers();
      QPixmap pix = te::qt::widgets::SymbologyPreview::build(ss, QSize(24, 24));
      QIcon icon(pix);
      QTableWidgetItem* item = new QTableWidgetItem(icon, "");
      item->setFlags(Qt::ItemIsEnabled);
      m_ui->m_tableWidget->setItem(newrow, 0, item);
    }

    //title
    {
      QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(gi->getTitle()));
      item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
      m_ui->m_tableWidget->setItem(newrow, 1, item);
    }

    if(loadColorBar)
    {
      if(count != 0 && count != m_legend.size() - 1)
      {
        double pos = (1. / (m_legend.size() - 1)) * count;

        if(!gi->getSymbolizers().empty())
        {
          te::se::SymbolizerColorFinder scf;

          scf.find(gi->getSymbolizers()[0]);

          te::color::RGBAColor color = scf.getColor();

          if(cb)
            cb->addColor(color, pos);
        }
      }
    }

    ++count;

    if(type == te::map::EQUAL_STEPS || type == te::map::QUANTIL || type == te::map::STD_DEVIATION)
    {
      if(gi->getTitle() != NO_TITLE)
      {
        //Min
        {
          QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(gi->getLowerLimit()));
          item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
          m_ui->m_tableWidget->setItem(newrow, 2, item);
        }

        //Max
        {
          QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(gi->getUpperLimit()));
          item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
          m_ui->m_tableWidget->setItem(newrow, 3, item);
        }
      }

      //Count
      QTableWidgetItem* item = new QTableWidgetItem(QString::number(gi->getCount()));
      item->setFlags(Qt::ItemIsEnabled);
      m_ui->m_tableWidget->setItem(newrow, 4, item);

    }
    else if(type == te::map::UNIQUE_VALUE)
    {
      if(gi->getTitle() != NO_TITLE)
      {
        //Value
        QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(gi->getValue()));
        item->setFlags(Qt::ItemIsEnabled);
        m_ui->m_tableWidget->setItem(newrow, 2, item);
      }

      //Count
      QTableWidgetItem* item = new QTableWidgetItem(QString::number(gi->getCount()));
      item->setFlags(Qt::ItemIsEnabled);
      m_ui->m_tableWidget->setItem(newrow, 3, item);

    }
  }

  if(cb)
  {
    disconnect(m_colorBar, SIGNAL(colorBarChanged()), this, SLOT(onColorBarChanged()));

    te::qt::widgets::colorbar::ColorBar* cbW = m_colorBar->getColorBar();
    cbW->setColorBar(cb);

    connect(m_colorBar, SIGNAL(colorBarChanged()), this, SLOT(onColorBarChanged()));
  }

  m_ui->m_tableWidget->resizeColumnsToContents();
#if (QT_VERSION >= 0x050000)
  m_ui->m_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#else
  m_ui->m_tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#endif

  connect(m_ui->m_tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(onTableWidgetItemChanged(QTableWidgetItem*)));
}

void te::qt::widgets::GroupingWidget::setDataSetType()
{
  listAttributes();
}

void te::qt::widgets::GroupingWidget::setGrouping()
{
  te::map::Grouping* grouping = m_layer->getGrouping();

  setGrouping(grouping);

  emit applyPushButtonClicked();
}

void te::qt::widgets::GroupingWidget::setGrouping(te::map::Grouping* grouping)
{
  if(!grouping)
    return;

  //type
  te::map::GroupingType type = grouping->getType();
  
  for(int i = 0; i < m_ui->m_typeComboBox->count(); ++i)
  {
    if(type == m_ui->m_typeComboBox->itemData(i).toInt())
    {
      m_ui->m_typeComboBox->setCurrentIndex(i);

      onTypeComboBoxActivated(i);

      break;
    }
  }

  //attr name
  std::string attrName = grouping->getPropertyName();

  for(int i = 0; i < m_ui->m_attrComboBox->count(); ++i)
  {
    if(attrName == m_ui->m_attrComboBox->itemText(i).toStdString())
    {
      m_ui->m_attrComboBox->setCurrentIndex(i);
      break;
    }
  }

  //precision
  size_t prec = grouping->getPrecision();

  m_ui->m_precSpinBox->setValue((int)prec);

  //slices
  size_t slices = grouping->getNumSlices();

  m_ui->m_slicesSpinBox->setValue((int)slices);

  //std dev
  float stdDev = grouping->getStdDeviation();

  m_ui->m_stdDevDoubleSpinBox->setValue((double)stdDev);

  //grouping items
  te::common::FreeContents(m_legend);
  m_legend.clear();

  for(size_t t = 0; t < grouping->getGroupingItems().size(); ++t)
  {
    te::map::GroupingItem* gi = new te::map::GroupingItem(*grouping->getGroupingItems()[t]);

    m_legend.push_back(gi);
  }

  updateUi(true);

  onApplyPushButtonClicked();
}

void te::qt::widgets::GroupingWidget::onApplyPushButtonClicked()
{
  if(m_manual)
  {
      int reply = QMessageBox::question(this, tr("Classification"), tr("Manual changes will be lost. Continue?"), QMessageBox::Yes | QMessageBox::Cancel);

      if(reply != QMessageBox::Yes)
        return;
  }

  int index = m_ui->m_typeComboBox->currentIndex();

  int type = m_ui->m_typeComboBox->itemData(index).toInt();
  int slices = m_ui->m_slicesSpinBox->value();
  int prec = m_ui->m_precSpinBox->value();
  double stdDev = m_ui->m_stdDevDoubleSpinBox->value();

  std::string attr = m_ui->m_attrComboBox->currentText().toStdString();
  int attrIdx =  m_ui->m_attrComboBox->currentIndex();
  int attrType = m_ui->m_attrComboBox->itemData(attrIdx).toInt();

  te::common::FreeContents(m_legend);
  m_legend.clear();

  std::string mean = "";

  int nullValues = 0;

  bool update = false;

  if(type == te::map::EQUAL_STEPS)
  {
    std::vector<double> vec;

    getDataAsDouble(vec, attr, attrType, nullValues);

    te::map::GroupingByEqualSteps(vec.begin(), vec.end(), slices, m_legend, prec);

    buildSymbolizer(mean);

    createDoubleNullGroupingItem(nullValues);
  }
  else if(type == te::map::QUANTIL) 
  {
    std::vector<double> vec;

    getDataAsDouble(vec, attr, attrType, nullValues);

    te::map::GroupingByQuantil(vec.begin(), vec.end(), slices, m_legend, prec);

    buildSymbolizer(mean);

    createDoubleNullGroupingItem(nullValues);
  }
  else if(type == te::map::STD_DEVIATION) 
  {
    std::vector<double> vec;

    getDataAsDouble(vec, attr, attrType, nullValues);

    te::map::GroupingByStdDeviation(vec.begin(), vec.end(), stdDev, m_legend, mean, prec);

    buildSymbolizer(mean);

    createDoubleNullGroupingItem(nullValues);

    update = false;
  }
  else if(type == te::map::UNIQUE_VALUE) 
  {
    std::vector<std::string> vec;

    getDataAsString(vec, attr, nullValues);

    te::map::GroupingByUniqueValues(vec, attrType, m_legend, prec);

    buildSymbolizer(mean);

    createStringNullGroupingItem(nullValues);
  }

  updateUi(update);

  m_manual = false;

  emit applyPushButtonClicked();
}

void te::qt::widgets::GroupingWidget::onTypeComboBoxActivated(int idx)
{
  int type = m_ui->m_typeComboBox->itemData(idx).toInt();

  if(type == te::map::EQUAL_STEPS)
  {
    m_ui->m_slicesSpinBox->setEnabled(true);
    m_ui->m_precSpinBox->setEnabled(true);
    m_ui->m_stdDevDoubleSpinBox->setEnabled(false);
  }
  else if(type == te::map::QUANTIL) 
  {
    m_ui->m_slicesSpinBox->setEnabled(true);
    m_ui->m_precSpinBox->setEnabled(true);
    m_ui->m_stdDevDoubleSpinBox->setEnabled(false);
  }
  else if(type == te::map::STD_DEVIATION) 
  {
    m_ui->m_slicesSpinBox->setEnabled(false);
    m_ui->m_precSpinBox->setEnabled(true);
    m_ui->m_stdDevDoubleSpinBox->setEnabled(true);
  }
  else if(type == te::map::UNIQUE_VALUE) 
  {
    m_ui->m_slicesSpinBox->setEnabled(false);
    m_ui->m_precSpinBox->setEnabled(true);
    m_ui->m_stdDevDoubleSpinBox->setEnabled(false);
  }

  if(m_layer.get())
    listAttributes();
}

void te::qt::widgets::GroupingWidget::onAttrComboBoxActivated(int idx)
{
//  int attrType = m_ui->m_attrComboBox->itemData(idx).toInt();
}

void te::qt::widgets::GroupingWidget::onColorBarChanged()
{
  if(m_layer.get())
  {
    buildSymbolizer();

    updateUi();
  }
}

void  te::qt::widgets::GroupingWidget::onTableWidgetItemChanged(QTableWidgetItem* item)
{
  int index = m_ui->m_typeComboBox->currentIndex();
  int type = m_ui->m_typeComboBox->itemData(index).toInt();

  int curRow = m_ui->m_tableWidget->currentRow();
  int curCol = m_ui->m_tableWidget->currentColumn();

  QString str = item->text();

  if(curCol == 1) // title
  {
    m_legend[curRow]->setTitle(str.toStdString());

    m_manual = true;
  }
  else if(curCol == 2 || curCol == 3) // min and max
  {
    if(type == te::map::EQUAL_STEPS || type == te::map::QUANTIL || type == te::map::STD_DEVIATION)
    {
      bool ok = false;

      str.toDouble(&ok);

      if(!ok)
      {
        if(curCol == 2)
          item->setText(m_legend[curRow]->getLowerLimit().c_str());
        else if(curCol ==3)
          item->setText(m_legend[curRow]->getUpperLimit().c_str());
      }
      else
      {
        if(curCol == 2)
          m_legend[curRow]->setLowerLimit(item->text().toStdString());
        else if(curCol ==3)
          m_legend[curRow]->setUpperLimit(item->text().toStdString());

        m_manual = true;
      }
    }
  }
}

void te::qt::widgets::GroupingWidget::onTableWidgetItemDoubleClicked(QTableWidgetItem* item)
{
  int index = m_ui->m_typeComboBox->currentIndex();
  int type = m_ui->m_typeComboBox->itemData(index).toInt();

  int curRow = m_ui->m_tableWidget->currentRow();
  int curCol = m_ui->m_tableWidget->currentColumn();

  if(curCol == 0)
  {
    te::map::GroupingItem* gi = m_legend[curRow];

    std::vector<te::se::Symbolizer*> symbVec = gi->getSymbolizers();

    QDialog* dialog = new QDialog(this);
    QBoxLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom, dialog);

    QDialogButtonBox* bbox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, dialog);

    QWidget* symbWidget = 0;

    if(symbVec[0]->getType() == "PolygonSymbolizer")
    {
      symbWidget = new te::qt::widgets::PolygonSymbolizerWidget(dialog);
      te::qt::widgets::PolygonSymbolizerWidget* polygonSymbolizerWidget = (te::qt::widgets::PolygonSymbolizerWidget*)symbWidget;
      polygonSymbolizerWidget->setSymbolizer((te::se::PolygonSymbolizer*)symbVec[0]);
    }
    else if(symbVec[0]->getType() == "LineSymbolizer")
    {
      symbWidget = new te::qt::widgets::LineSymbolizerWidget(dialog);
      te::qt::widgets::LineSymbolizerWidget* lineSymbolizerWidget = (te::qt::widgets::LineSymbolizerWidget*)symbWidget;
      lineSymbolizerWidget->setSymbolizer((te::se::LineSymbolizer*)symbVec[0]);
    }
    else if(symbVec[0]->getType() == "PointSymbolizer")
    {
      symbWidget = new te::qt::widgets::PointSymbolizerWidget(dialog);
      te::qt::widgets::PointSymbolizerWidget* pointSymbolizerWidget = (te::qt::widgets::PointSymbolizerWidget*)symbWidget;
      pointSymbolizerWidget->setSymbolizer((te::se::PointSymbolizer*)symbVec[0]);
    }

    layout->addWidget(symbWidget);
    layout->addWidget(bbox);

    connect(bbox, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(bbox, SIGNAL(rejected()), dialog, SLOT(reject()));

    if(dialog->exec() == QDialog::Rejected)
    {
      delete dialog;
      return;
    }

    if(symbVec[0]->getType() == "PolygonSymbolizer")
    {
      symbVec.clear();
      te::qt::widgets::PolygonSymbolizerWidget* polygonSymbolizerWidget = (te::qt::widgets::PolygonSymbolizerWidget*)symbWidget;
      symbVec.push_back(polygonSymbolizerWidget->getSymbolizer());
    }
    else if(symbVec[0]->getType() == "LineSymbolizer")
    {
      symbVec.clear();
      te::qt::widgets::LineSymbolizerWidget* lineSymbolizerWidget = (te::qt::widgets::LineSymbolizerWidget*)symbWidget;
      symbVec.push_back(lineSymbolizerWidget->getSymbolizer());
    }
    else if(symbVec[0]->getType() == "PointSymbolizer")
    {
      symbVec.clear();
      te::qt::widgets::PointSymbolizerWidget* pointSymbolizerWidget = (te::qt::widgets::PointSymbolizerWidget*)symbWidget;
      symbVec.push_back(pointSymbolizerWidget->getSymbolizer());
    }

    gi->setSymbolizers(symbVec);

    QPixmap pix = te::qt::widgets::SymbologyPreview::build(symbVec, QSize(24, 24));
    QIcon icon(pix);

    QTableWidgetItem* newItem = new QTableWidgetItem(icon, "");
    newItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

    m_ui->m_tableWidget->setItem(curRow, 0, newItem);

    delete dialog;

    updateUi(true);

    emit applyPushButtonClicked();
  }
}


void te::qt::widgets::GroupingWidget::getDataAsDouble(std::vector<double>& vec, const std::string& attrName, const int& dataType, int& nullValues)
{
  assert(m_layer.get());

  if(te::da::HasLinkedTable(m_layer.get()))
  {
    getLinkedDataAsDouble(vec, attrName, dataType, nullValues);
    return;
  }

  std::auto_ptr<te::map::LayerSchema> dsType(m_layer->getSchema());

  std::size_t idx;

  for(std::size_t t = 0; t < dsType->getProperties().size(); ++t)
  {
    if(dsType->getProperty(t)->getName() == attrName)
    {
      idx = t;
      break;
    }
  }

  std::auto_ptr<te::da::DataSet> ds(m_layer->getData());

  ds->moveBeforeFirst();

  while(ds->moveNext())
  {
    if(ds->isNull(idx))
    {
      ++nullValues;
      continue;
    }

    if(dataType == te::dt::INT16_TYPE)
      vec.push_back((double)ds->getInt16(idx));
    else if(dataType == te::dt::INT32_TYPE)
      vec.push_back((double)ds->getInt32(idx));
    else if(dataType == te::dt::INT64_TYPE)
      vec.push_back((double)ds->getInt64(idx));
    else if(dataType == te::dt::FLOAT_TYPE)
      vec.push_back((double)ds->getFloat(idx));
    else if(dataType == te::dt::DOUBLE_TYPE)
      vec.push_back(ds->getDouble(idx));
    else if(dataType == te::dt::NUMERIC_TYPE)
    {
      QString strNum = ds->getNumeric(idx).c_str();

      bool ok = false;

      double value = strNum.toDouble(&ok);

      if(ok)
        vec.push_back(value);
    }
  }
}

void te::qt::widgets::GroupingWidget::getLinkedDataAsDouble(std::vector<double>& vec, const std::string& attrName, const int& dataType, int& nullValues)
{
  assert(m_layer.get());

  std::auto_ptr<te::map::LayerSchema> dsType(m_layer->getSchema());

  std::string function = "AVERAGE";
  std::vector<std::string> poid;
  size_t pksize = 0;
  te::map::QueryLayer* qlayer = 0;
  te::da::Select* select = 0;

  // make sorting by object id
  qlayer = dynamic_cast<te::map::QueryLayer*>(m_layer.get());
  select = dynamic_cast<te::da::Select*>(qlayer->getQuery()->clone());
  te::da::Select* selectaux = dynamic_cast<te::da::Select*>(select->clone());
  te::da::OrderBy* orderBy = new te::da::OrderBy;

  std::vector<te::dt::Property*> props = dsType->getPrimaryKey()->getProperties();
  while(++pksize < props.size())
  {
    poid.push_back(props[pksize-1]->getName());
    if(props[pksize-1]->getDatasetName() != props[pksize]->getDatasetName())
      break;
  }

  for(size_t i = 0; i < pksize; ++i)
    orderBy->push_back(new te::da::OrderByItem(poid[i]));

  selectaux->setOrderBy(orderBy);
  qlayer->setQuery(selectaux);

  std::size_t idx;

  for(std::size_t t = 0; t < dsType->getProperties().size(); ++t)
  {
    if(dsType->getProperty(t)->getName() == attrName)
    {
      idx = t;
      break;
    }
  }

  std::vector<std::string> pkdata(pksize), pkdataaux(pksize);  
  std::auto_ptr<te::da::DataSet> ds(m_layer->getData());
  qlayer->setQuery(select);

  bool nullValue = false;
  std::vector<double> values;
  bool isBegin = true;
  ds->moveBeforeFirst();

  while(ds->moveNext())
  {
    if(pksize)
    {
      // it is linked. Remove redundancies.
      size_t i;
      for(i = 0; i < pksize; ++i)
      {
        pkdata[i] = ds->getAsString(poid[i]);
        if(isBegin)
        {
          isBegin = false;
          pkdataaux[i] = ds->getAsString(poid[i]);
        }
      }

      for(i = 0; i < pksize; ++i)
      {
        if(pkdata[i] != pkdataaux[i])
        {
          pkdataaux = pkdata;
          break;
        }
      }
      if(i == pksize) // it is the same object
      {
        if(nullValue == false)
        {
          if(ds->isNull(idx))
            nullValue = true;
          else
          {
            if(dataType == te::dt::INT16_TYPE)
              values.push_back((double)ds->getInt16(idx));
            else if(dataType == te::dt::INT32_TYPE)
              values.push_back((double)ds->getInt32(idx));
            else if(dataType == te::dt::INT64_TYPE)
              values.push_back((double)ds->getInt64(idx));
            else if(dataType == te::dt::FLOAT_TYPE)
              values.push_back((double)ds->getFloat(idx));
            else if(dataType == te::dt::DOUBLE_TYPE)
              values.push_back(ds->getDouble(idx));
            else if(dataType == te::dt::NUMERIC_TYPE)
            {
              QString strNum = ds->getNumeric(idx).c_str();

              bool ok = false;

              double value = strNum.toDouble(&ok);

              if(ok)
                values.push_back(value);
            }
          }
        }
        continue;
        // read other values
      }
      else // it is other object
      {
        // sumarize value according to the required summarization 
        if(nullValue)
           ++nullValues;
        else
         vec.push_back(te::da::GetSummarizedValue(values, function));

        nullValue = false;
        values.clear();

        // get new value
        if(ds->isNull(idx))
          nullValue = true;
        else
        {
          if(dataType == te::dt::INT16_TYPE)
            values.push_back((double)ds->getInt16(idx));
          else if(dataType == te::dt::INT32_TYPE)
            values.push_back((double)ds->getInt32(idx));
          else if(dataType == te::dt::INT64_TYPE)
            values.push_back((double)ds->getInt64(idx));
          else if(dataType == te::dt::FLOAT_TYPE)
            values.push_back((double)ds->getFloat(idx));
          else if(dataType == te::dt::DOUBLE_TYPE)
            values.push_back(ds->getDouble(idx));
          else if(dataType == te::dt::NUMERIC_TYPE)
          {
            QString strNum = ds->getNumeric(idx).c_str();

            bool ok = false;

            double value = strNum.toDouble(&ok);

            if(ok)
              values.push_back(value);
          }
        }
      }
    }
  }
  // sumarize value according to the required summarization 
  if(nullValue)
    ++nullValues;
  else
    vec.push_back(te::da::GetSummarizedValue(values, function));
  values.clear();
}

void te::qt::widgets::GroupingWidget::getDataAsString(std::vector<std::string>& vec, const std::string& attrName, int& nullValues)
{
  assert(m_layer.get());

  if(te::da::HasLinkedTable(m_layer.get()))
  {
    getLinkedDataAsString(vec, attrName,  nullValues);
    return;
  }

  std::auto_ptr<te::map::LayerSchema> dsType(m_layer->getSchema());

  std::size_t idx;

  for(std::size_t t = 0; t < dsType->getProperties().size(); ++t)
  {
    if(dsType->getProperty(t)->getName() == attrName)
    {
      idx = t;
      break;
    }
  }

  std::auto_ptr<te::da::DataSet> ds(m_layer->getData());

  ds->moveBeforeFirst();

  while(ds->moveNext())
  {
    if(!ds->isNull(idx))
      vec.push_back(ds->getAsString(idx));
    else
      ++nullValues;
  }
}

void te::qt::widgets::GroupingWidget::getLinkedDataAsString(std::vector<std::string>& vec, const std::string& attrName, int& nullValues)
{
  assert(m_layer.get());

  std::auto_ptr<te::map::LayerSchema> dsType(m_layer->getSchema());

  std::string function = "MIN";
  std::vector<std::string> poid;
  size_t pksize = 0;
  te::map::QueryLayer* qlayer = 0;
  te::da::Select* select = 0;

  // make sorting by object id
  qlayer = dynamic_cast<te::map::QueryLayer*>(m_layer.get());
  select = dynamic_cast<te::da::Select*>(qlayer->getQuery()->clone());
  te::da::Select* selectaux = dynamic_cast<te::da::Select*>(select->clone());
  te::da::OrderBy* orderBy = new te::da::OrderBy;

  std::vector<te::dt::Property*> props = dsType->getPrimaryKey()->getProperties();
  while(++pksize < props.size())
  {
    poid.push_back(props[pksize-1]->getName());
    if(props[pksize-1]->getDatasetName() != props[pksize]->getDatasetName())
      break;
  }

  for(size_t i = 0; i < pksize; ++i)
    orderBy->push_back(new te::da::OrderByItem(poid[i]));

  selectaux->setOrderBy(orderBy);
  qlayer->setQuery(selectaux);

  std::size_t idx;

  for(std::size_t t = 0; t < dsType->getProperties().size(); ++t)
  {
    if(dsType->getProperty(t)->getName() == attrName)
    {
      idx = t;
      break;
    }
  }

  std::vector<std::string> pkdata(pksize), pkdataaux(pksize);  
  std::auto_ptr<te::da::DataSet> ds(m_layer->getData());
  qlayer->setQuery(select);

  bool nullValue = false;
  std::vector<std::string> values;
  bool isBegin = true;
  ds->moveBeforeFirst();

  while(ds->moveNext())
  {
    if(pksize)
    {
      // it is linked. Remove redundancies.
      size_t i;
      for(i = 0; i < pksize; ++i)
      {
        pkdata[i] = ds->getAsString(poid[i]);
        if(isBegin)
        {
          isBegin = false;
          pkdataaux[i] = ds->getAsString(poid[i]);
        }
      }

      for(i = 0; i < pksize; ++i)
      {
        if(pkdata[i] != pkdataaux[i])
        {
          pkdataaux = pkdata;
          break;
        }
      }
      if(i == pksize) // it is the same object
      {
        if(nullValue == false)
        {
          if(ds->isNull(idx))
            nullValue = true;
          else
            values.push_back(ds->getAsString(idx));
        }
        continue;
        // read other values
      }
      else // it is other object
      {
        // sumarize value according to the required summarization 
        if(nullValue)
          ++nullValues;
        else
          vec.push_back(te::da::GetSummarizedValue(values, function));

        nullValue = false;
        values.clear();

        // get new value
        if(ds->isNull(idx))
          nullValue = true;
        else
          values.push_back(ds->getAsString(idx));
      }
    }
  }
  // sumarize value according to the required summarization 
  if(nullValue)
    ++nullValues;
  else
    vec.push_back(te::da::GetSummarizedValue(values, function));
  values.clear();
}

void te::qt::widgets::GroupingWidget::createDoubleNullGroupingItem(int count)
{
  if(count == 0)
    return;

  te::map::GroupingItem* legendItem = new te::map::GroupingItem;
  legendItem->setLowerLimit(te::common::Globals::sm_nanStr);
  legendItem->setUpperLimit(te::common::Globals::sm_nanStr);
  legendItem->setTitle(NO_TITLE);
  legendItem->setCount(count);

  int geomType = getGeometryType();
  std::vector<te::se::Symbolizer*> symbVec;
  te::se::Symbolizer* s = te::se::CreateSymbolizer((te::gm::GeomType)geomType, "#dddddd");
  symbVec.push_back(s);
  legendItem->setSymbolizers(symbVec);

  m_legend.push_back(legendItem);
}

void te::qt::widgets::GroupingWidget::createStringNullGroupingItem(int count)
{
  if(count == 0)
    return;

  te::map::GroupingItem* legendItem = new te::map::GroupingItem;
  legendItem->setValue(te::common::Globals::sm_nanStr);
  legendItem->setTitle(NO_TITLE);
  legendItem->setCount(count);

  int geomType = getGeometryType();
  std::vector<te::se::Symbolizer*> symbVec;
  te::se::Symbolizer* s = te::se::CreateSymbolizer((te::gm::GeomType)geomType, "#dddddd");
  symbVec.push_back(s);
  legendItem->setSymbolizers(symbVec);

  m_legend.push_back(legendItem);
}

int te::qt::widgets::GroupingWidget::getGeometryType()
{
  assert(m_layer.get());

  return te::map::GetGeomType(m_layer);
}

void te::qt::widgets::GroupingWidget::buildSymbolizer(std::string meanTitle)
{
  delete m_cb;

  m_cb = m_colorBar->getColorBar()->getColorBar();

  int legendSize = m_legend.size();

  std::vector<te::color::RGBAColor> colorVec;
  
  if(meanTitle.empty())
  {
    colorVec = m_cb->getSlices(legendSize);
  }
  else
  {
    int beforeMean = 0;
    int afterMean = 0;
    int meanIdx = 0;

    for(size_t t = 0; t < m_legend.size(); ++t)
    {
      if(m_legend[t]->getTitle() != meanTitle)
      {
        beforeMean++;
      }
      else
      {
        meanIdx = t;
        afterMean = m_legend.size() - t - 1;
        break;
      }
    }

    std::vector<te::color::RGBAColor> lowerColorVec = m_cb->getLowerMeanSlices(beforeMean);
    te::color::RGBAColor meanColor = m_cb->getMeanSlice();
    std::vector<te::color::RGBAColor> upperColorVec = m_cb->getUpperMeanSlices(afterMean);

    for(size_t t = 0; t < lowerColorVec.size(); ++t)
      colorVec.push_back(lowerColorVec[t]);

    colorVec.push_back(meanColor);

    for(size_t t = 0; t < upperColorVec.size(); ++t)
      colorVec.push_back(upperColorVec[t]);
  }

  if(colorVec.size() != m_legend.size())
    return;

  int geomType = getGeometryType();

  for(size_t t = 0; t < colorVec.size(); ++t)
  {
    std::vector<te::se::Symbolizer*> symbVec;

    te::se::Symbolizer* s = te::se::CreateSymbolizer((te::gm::GeomType)geomType, colorVec[t].getColor());

    symbVec.push_back(s);

    m_legend[t]->setSymbolizers(symbVec);
  }
}

void te::qt::widgets::GroupingWidget::listAttributes()
{
  QString curValue = m_ui->m_attrComboBox->currentText();

  m_ui->m_attrComboBox->clear();

  std::auto_ptr<te::map::LayerSchema> dsType(m_layer->getSchema());

  //grouping type
  int index = m_ui->m_typeComboBox->currentIndex();
  int type = m_ui->m_typeComboBox->itemData(index).toInt();

  if(type == te::map::EQUAL_STEPS ||type == te::map::QUANTIL || type == te::map::STD_DEVIATION)
  {
    for(size_t t = 0; t < dsType->getProperties().size(); ++t)
    {
      te::dt::Property* p = dsType->getProperty(t);

      int dataType = p->getType();

      switch(dataType)
      {
        case te::dt::INT16_TYPE:
        case te::dt::INT32_TYPE:
        case te::dt::INT64_TYPE:
        case te::dt::FLOAT_TYPE:
        case te::dt::DOUBLE_TYPE:
        case te::dt::NUMERIC_TYPE:
          m_ui->m_attrComboBox->addItem(p->getName().c_str(), p->getType());

        default:
          continue;
      }
    }
  }
  else if(type == te::map::UNIQUE_VALUE) 
  {
    for(size_t t = 0; t < dsType->getProperties().size(); ++t)
    {
      te::dt::Property* p = dsType->getProperty(t);

      int dataType = p->getType();

      switch(dataType)
      {
        case te::dt::INT16_TYPE:
        case te::dt::INT32_TYPE:
        case te::dt::INT64_TYPE:
        case te::dt::FLOAT_TYPE:
        case te::dt::DOUBLE_TYPE:
        case te::dt::NUMERIC_TYPE:
        case te::dt::STRING_TYPE:
          m_ui->m_attrComboBox->addItem(p->getName().c_str(), p->getType());

        default:
          continue;
      }
    }
  }

  if(curValue.isEmpty() == false)
  {
    int idx = m_ui->m_attrComboBox->findText(curValue);

    if(idx != -1)
      m_ui->m_attrComboBox->setCurrentIndex(idx);
  }
}

void te::qt::widgets::GroupingWidget::setLayers(te::map::AbstractLayerPtr selectedLayer, std::vector<te::map::AbstractLayerPtr> allLayers)
{
  m_layer = selectedLayer;

  //set data type
  setDataSetType();

  //set grouping
  setGrouping();

  for(std::size_t i = 0; i < allLayers.size(); ++i)
  {
    if(!allLayers[i]->isValid())
      continue;

    std::auto_ptr<te::da::DataSetType> dt(allLayers[i]->getSchema());

    if(dt->hasGeom() && allLayers[i]->getGrouping() && allLayers[i]->getId() != selectedLayer->getId())
    {
      m_ui->m_layersComboBox->addItem(allLayers[i]->getTitle().c_str(), QVariant::fromValue(allLayers[i]));
    }
  }
}

void te::qt::widgets::GroupingWidget::onImportPushButtonClicked()
{
  if(m_ui->m_layersComboBox->currentText() == "")
  {
    QMessageBox::warning(this, tr("Grouping"), tr("There are no other layers with Grouping!"));
    return;
  }

  if(m_manual)
  {
    int reply = QMessageBox::question(this, tr("Grouping"), tr("Manual changes will be lost. Continue?"), QMessageBox::Yes | QMessageBox::Cancel);

    if(reply != QMessageBox::Yes)
      return;
  }

  QVariant varLayer = m_ui->m_layersComboBox->itemData(m_ui->m_layersComboBox->currentIndex(), Qt::UserRole);
  te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

  te::map::Grouping* ref = layer->getGrouping();

  std::auto_ptr<te::da::DataSetType> dt = m_layer->getSchema();

  std::vector<te::dt::Property*> props = dt->getProperties();

  bool isValid = false;
  for(std::size_t i = 0; i < props.size(); ++i)
  {
    te::dt::Property* prop = props[i];
    if((prop->getName() == ref->getPropertyName()) && (prop->getType() == ref->getPropertyType()))
    {
      isValid = true;
      break;
    }
  }

  if(!isValid)
  {
    QMessageBox::warning(this, tr("Grouping"), tr("In existing layers, there is no grouping that can be imported!"));
    return;
  }

  te::map::Grouping* newGrouping = new te::map::Grouping(*ref);

  setGrouping(newGrouping);

  updateUi(true);

  m_manual = false;

  emit applyPushButtonClicked();
}
