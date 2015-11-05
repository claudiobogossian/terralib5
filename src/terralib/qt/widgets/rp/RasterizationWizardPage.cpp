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

// Boost - Don't change the this include order
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>

//Terralib
#include "../../../color/ColorBar.h"
#include "../../../common/Globals.h"
#include "../../../common/STLUtils.h"
#include "../../../common/Translator.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../dataaccess/query_h.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../maptools/GroupingAlgorithms.h"
#include "../../../maptools/GroupingItem.h"
#include "../../../maptools/Utils.h"
#include "../../../se.h"
#include "../../../se/SymbolizerColorFinder.h"
#include "../../../se/Utils.h"
#include "../../../srs/SpatialReferenceSystemManager.h"
#include "../../widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../colorbar/ColorBar.h"
#include "../colorbar/ColorCatalogWidget.h"
#include "../se/LineSymbolizerWidget.h"
#include "../se/PointSymbolizerWidget.h"
#include "../se/PolygonSymbolizerWidget.h"
#include "../se/SymbologyPreview.h"
#include "RasterizationWizardPage.h"
#include "ui_RasterizationWizardPageForm.h"

// Qt
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>

// BOOST
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/json_parser.hpp>

//#define MAX_SLICES 200
//#define PRECISION 15
#define NO_TITLE "No Value"

te::qt::widgets::RasterizationWizardPage::RasterizationWizardPage(QWidget *parent)
  : QWizardPage(parent),
    m_cb(0)
{
  m_ui.reset(new Ui::RasterizationWizardPageForm);

  m_ui->setupUi(this);

  m_ui->m_tableWidget->resizeColumnsToContents();
  
  m_ui->m_precSpinBox->setValue(6);

  QGridLayout* l = new QGridLayout(m_ui->m_colorBarWidget);
  l->setContentsMargins(0, 0, 0, 0);
  m_colorBar = new  te::qt::widgets::ColorCatalogWidget(m_ui->m_colorBarWidget);
  l->addWidget(m_colorBar);

//configure page
  this->setTitle(tr("Rasterization"));
  this->setSubTitle(tr("Define the rasterization parameters."));

  //connects
  connect(m_colorBar, SIGNAL(colorBarChanged()), this, SLOT(onColorBarChanged()));
  connect(m_ui->m_attrComboBox, SIGNAL(activated(int)), this, SLOT(onAttrComboBoxActivated(int)));
  connect(m_ui->m_applyPushButton, SIGNAL(clicked()), this, SLOT(onApplyPushButtonClicked()));
  connect(m_ui->m_tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(onTableWidgetItemDoubleClicked(QTableWidgetItem*)));
  connect(m_ui->m_loadPushButton, SIGNAL(clicked()), this, SLOT(onLoadPushButtonClicked()));
  connect(m_ui->m_savePushButton, SIGNAL(clicked()), this, SLOT(onSavePushButtonClicked()));
  connect(m_ui->m_selectAllPushButton, SIGNAL(clicked()), this, SLOT(onSelectAllPushButtonClicked()));
  connect(m_ui->m_unselectAllPushButton, SIGNAL(clicked()), this, SLOT(onUnselectAllPushButtonClicked()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this, SLOT(onTargetFileToolButtonPressed()));

  connect(m_ui->m_resXLineEdit, SIGNAL(editingFinished()), this, SLOT(onResXLineEditEditingFinished()));
  connect(m_ui->m_resYLineEdit, SIGNAL(editingFinished()), this, SLOT(onResYLineEditEditingFinished()));
}

te::qt::widgets::RasterizationWizardPage::~RasterizationWizardPage()
{

}

void te::qt::widgets::RasterizationWizardPage::setLayer(te::map::AbstractLayerPtr layer)
{
  if (!layer)
    return;

  m_layer = layer;

  listAttributes();

  m_ui->m_layerLineEdit->setText(m_layer->getTitle().c_str());

  int srid = m_layer->getSRID();
  std::string sridName = te::srs::SpatialReferenceSystemManager::getInstance().getName(srid);

  m_ui->m_layerSRIDLabel->setText(sridName.c_str());
}

std::vector<std::string> te::qt::widgets::RasterizationWizardPage::getDistinctClasses(const std::string& attrName)
{
  std::string dsId = m_layer->getDataSourceId();

  std::auto_ptr<te::da::DataSetType> dst = m_layer->getSchema();

  te::da::DataSourcePtr source = te::da::DataSourceManager::getInstance().find(dsId);

  te::da::FromItem* t = new te::da::DataSetName(dst->getName());
  te::da::From* from = new te::da::From;
  from->push_back(t);

  te::da::Distinct* dist = new te::da::Distinct;
  te::da::Expression* propName = new te::da::PropertyName(attrName);
  dist->push_back(propName);

  te::da::OrderByItem* obi = new te::da::OrderByItem(propName);

  te::da::OrderBy* order = new te::da::OrderBy;
  order->push_back(obi);

  te::da::Select* select = new te::da::Select();
  select->setDistinct(dist);
  select->setFrom(from);
  select->setOrderBy(order);

  std::auto_ptr<te::da::DataSet> result = source->query(select);

  result->moveBeforeFirst();

  std::vector<std::string> classes;

  while (result->moveNext())
  {
    std::string className = result->getValue(attrName)->toString();

    classes.push_back(className);
  }

  return classes;


}

void te::qt::widgets::RasterizationWizardPage::onApplyPushButtonClicked()
{
  if (m_manual)
  {
    int reply = QMessageBox::question(this, tr("Edit Legend"), tr("Manual changes will be lost. Continue?"), QMessageBox::Yes | QMessageBox::Cancel);

    if (reply != QMessageBox::Yes)
      return;
  }

  int prec = m_ui->m_precSpinBox->value();

  std::string attr = m_ui->m_attrComboBox->currentText().toStdString();
  int attrIdx = m_ui->m_attrComboBox->currentIndex();
  int attrType = m_ui->m_attrComboBox->itemData(attrIdx).toInt();

  te::common::FreeContents(m_legend);
  m_legend.clear();

  std::string mean = "";

  int nullValues = 0;

  bool update = false;

  std::vector<std::string> vec;

  getDataAsString(vec, attr, nullValues);

  te::map::GroupingByUniqueValues(vec, attrType, m_legend, prec);

  buildSymbolizer(mean);

  createStringNullGroupingItem(nullValues);

  updateUi(update);

  m_manual = false;

  emit applyPushButtonClicked();
}

void te::qt::widgets::RasterizationWizardPage::onAttrComboBoxActivated(int idx)
{

}

void te::qt::widgets::RasterizationWizardPage::onColorBarChanged()
{
  if (m_layer.get())
  {
    buildSymbolizer();

    updateUi();
  }
}

void te::qt::widgets::RasterizationWizardPage::onTableWidgetItemChanged(QTableWidgetItem* item)
{

}

void te::qt::widgets::RasterizationWizardPage::onTableWidgetItemDoubleClicked(QTableWidgetItem* item)
{
  int curRow = m_ui->m_tableWidget->currentRow();
  int curCol = m_ui->m_tableWidget->currentColumn();

  if (curCol == 4)
  {
    te::map::GroupingItem* gi = m_legend[curRow];

    std::vector<te::se::Symbolizer*> symbVec = gi->getSymbolizers();

    QDialog* dialog = new QDialog(this);
    QBoxLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom, dialog);

    QDialogButtonBox* bbox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, dialog);

    QWidget* symbWidget = 0;

    if (symbVec[0]->getType() == "PolygonSymbolizer")
    {
      symbWidget = new te::qt::widgets::PolygonSymbolizerWidget(dialog);
      te::qt::widgets::PolygonSymbolizerWidget* polygonSymbolizerWidget = (te::qt::widgets::PolygonSymbolizerWidget*)symbWidget;
      polygonSymbolizerWidget->setSymbolizer((te::se::PolygonSymbolizer*)symbVec[0]);
    }
    else if (symbVec[0]->getType() == "LineSymbolizer")
    {
      symbWidget = new te::qt::widgets::LineSymbolizerWidget(dialog);
      te::qt::widgets::LineSymbolizerWidget* lineSymbolizerWidget = (te::qt::widgets::LineSymbolizerWidget*)symbWidget;
      lineSymbolizerWidget->setSymbolizer((te::se::LineSymbolizer*)symbVec[0]);
    }
    else if (symbVec[0]->getType() == "PointSymbolizer")
    {
      symbWidget = new te::qt::widgets::PointSymbolizerWidget(dialog);
      te::qt::widgets::PointSymbolizerWidget* pointSymbolizerWidget = (te::qt::widgets::PointSymbolizerWidget*)symbWidget;
      pointSymbolizerWidget->setSymbolizer((te::se::PointSymbolizer*)symbVec[0]);
    }

    layout->addWidget(symbWidget);
    layout->addWidget(bbox);

    connect(bbox, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(bbox, SIGNAL(rejected()), dialog, SLOT(reject()));

    if (dialog->exec() == QDialog::Rejected)
    {
      delete dialog;
      return;
    }

    if (symbVec[0]->getType() == "PolygonSymbolizer")
    {
      symbVec.clear();
      te::qt::widgets::PolygonSymbolizerWidget* polygonSymbolizerWidget = (te::qt::widgets::PolygonSymbolizerWidget*)symbWidget;
      symbVec.push_back(polygonSymbolizerWidget->getSymbolizer());
    }
    else if (symbVec[0]->getType() == "LineSymbolizer")
    {
      symbVec.clear();
      te::qt::widgets::LineSymbolizerWidget* lineSymbolizerWidget = (te::qt::widgets::LineSymbolizerWidget*)symbWidget;
      symbVec.push_back(lineSymbolizerWidget->getSymbolizer());
    }
    else if (symbVec[0]->getType() == "PointSymbolizer")
    {
      symbVec.clear();
      te::qt::widgets::PointSymbolizerWidget* pointSymbolizerWidget = (te::qt::widgets::PointSymbolizerWidget*)symbWidget;
      symbVec.push_back(pointSymbolizerWidget->getSymbolizer());
    }

    gi->setSymbolizers(symbVec);

    QPixmap pix = te::qt::widgets::SymbologyPreview::build(symbVec, QSize(24, 24));
    QIcon icon(pix);

    QTableWidgetItem* newItem = new QTableWidgetItem(icon, "");
    newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    m_ui->m_tableWidget->setItem(curRow, 0, newItem);

    delete dialog;

    updateUi(true);

    emit applyPushButtonClicked();
  }
}

void te::qt::widgets::RasterizationWizardPage::onLoadPushButtonClicked()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open..."),
    QString(), tr("LEG (*.leg *.LEG);;"), 0, QFileDialog::DontConfirmOverwrite);

  if (fileName.isEmpty())
    return;

  m_ui->m_tableWidget->setRowCount(0);

  boost::property_tree::ptree pt;
  boost::property_tree::json_parser::read_json(fileName.toStdString(), pt);

  boost::property_tree::ptree legend = pt.get_child("Legend");

  std::string attrName = legend.get<std::string>("Attribute");
  std::string precision = legend.get<std::string>("Precision");

  m_ui->m_attrComboBox->setCurrentText(attrName.c_str());
  m_ui->m_precSpinBox->setValue(boost::lexical_cast<double>(precision));

  std::vector<std::vector<std::string> > items;

  BOOST_FOREACH(boost::property_tree::ptree::value_type &v, legend.get_child("Items"))
  {
    std::vector<std::string> item;
    item.push_back(v.second.get<std::string>("ClassName"));
    item.push_back(v.second.get<std::string>("Red"));
    item.push_back(v.second.get<std::string>("Green"));
    item.push_back(v.second.get<std::string>("Blue"));

    items.push_back(item);
  }

  m_ui->m_tableWidget->setRowCount(items.size());

  for (std::size_t i = 0; i < items.size(); ++i)
  {
    m_ui->m_tableWidget->setItem(i, 0, new QTableWidgetItem(items[i][0].c_str()));
    m_ui->m_tableWidget->setItem(i, 1, new QTableWidgetItem(items[i][1].c_str()));
    m_ui->m_tableWidget->setItem(i, 2, new QTableWidgetItem(items[i][2].c_str()));
    m_ui->m_tableWidget->setItem(i, 3, new QTableWidgetItem(items[i][3].c_str()));

    QColor color;
    color.setRgb(boost::lexical_cast<int>(items[i][1]), boost::lexical_cast<int>(items[i][2]), boost::lexical_cast<int>(items[i][3]));

    QPixmap pix(24, 24);
    QPainter p(&pix);

    p.fillRect(0, 0, 12, 24, color);
    p.fillRect(12, 0, 12, 24, color);
    p.setBrush(Qt::transparent);
    p.setPen(Qt::black);
    p.drawRect(0, 0, 23, 23);

    QIcon icon(pix);

    QTableWidgetItem* item = new QTableWidgetItem(icon, "");
    //item->setBackgroundColor(color);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    m_ui->m_tableWidget->setItem(i, 4, item);

    {
      QTableWidgetItem* item = new QTableWidgetItem();
      item->setCheckState(Qt::Checked);
      m_ui->m_tableWidget->setItem(i, 5, item);
    }

  }

}

void te::qt::widgets::RasterizationWizardPage::onSavePushButtonClicked()
{
  int rowCount = m_ui->m_tableWidget->rowCount();

  if (rowCount < 1)
    return;

  QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
    QString(), tr("LEG (*.leg *.LEG);;"), 0, QFileDialog::DontConfirmOverwrite);

  if (fileName.isEmpty())
    return;

  boost::property_tree::ptree pt;
  boost::property_tree::ptree legend;

  std::string attrName = m_ui->m_attrComboBox->currentText().toStdString();
  std::string precision = m_ui->m_precSpinBox->text().toStdString();

  legend.add("Attribute", attrName);
  legend.add("Precision", precision);

  boost::property_tree::ptree items;

  for (std::size_t i = 0; i < m_ui->m_tableWidget->rowCount(); ++i)
  {
    std::string idx = boost::lexical_cast<std::string>(i + 1);
    std::string className = m_ui->m_tableWidget->item(i, 0)->text().toStdString();
    std::string r = m_ui->m_tableWidget->item(i, 1)->text().toStdString();
    std::string g = m_ui->m_tableWidget->item(i, 2)->text().toStdString();
    std::string b = m_ui->m_tableWidget->item(i, 3)->text().toStdString();

    boost::property_tree::ptree item;
    item.add("ClassName", className);
    item.add("Red", r);
    item.add("Green", g);
    item.add("Blue", b);

    items.add_child("Item", item);
  }

  legend.add_child("Items", items);

  pt.add_child("Legend", legend);

  boost::property_tree::write_json(fileName.toStdString(), pt);
}

void te::qt::widgets::RasterizationWizardPage::getDataAsString(std::vector<std::string>& vec, const std::string& attrName, int& nullValues)
{
  assert(m_layer.get());

  if (te::da::HasLinkedTable(m_layer->getSchema().get()))
  {
    getLinkedDataAsString(vec, attrName, nullValues);
    return;
  }

  std::auto_ptr<te::map::LayerSchema> dsType(m_layer->getSchema());

  std::size_t idx;

  for (std::size_t t = 0; t < dsType->getProperties().size(); ++t)
  {
    if (dsType->getProperty(t)->getName() == attrName)
    {
      idx = t;
      break;
    }
  }

  std::auto_ptr<te::da::DataSet> ds(m_layer->getData());

  ds->moveBeforeFirst();

  while (ds->moveNext())
  {
    if (!ds->isNull(idx))
      vec.push_back(ds->getAsString(idx));
    else
      ++nullValues;
  }
}

void te::qt::widgets::RasterizationWizardPage::getLinkedDataAsString(std::vector<std::string>& vec, const std::string& attrName, int& nullValues)
{
  //assert(m_layer.get());

  //std::auto_ptr<te::map::LayerSchema> dsType(m_layer->getSchema());

  //std::string function = m_ui->m_summaryComboBox->currentText().toStdString();
  //std::vector<std::string> poid;
  //size_t pksize = 0;
  //te::map::QueryLayer* qlayer = 0;
  //te::da::Select* select = 0;

  //// make sorting by object id
  //qlayer = dynamic_cast<te::map::QueryLayer*>(m_layer.get());
  //select = dynamic_cast<te::da::Select*>(qlayer->getQuery()->clone());
  //te::da::Select* selectaux = dynamic_cast<te::da::Select*>(select->clone());
  //te::da::OrderBy* orderBy = new te::da::OrderBy;

  //std::vector<te::dt::Property*> props = dsType->getPrimaryKey()->getProperties();
  //while (++pksize < props.size())
  //{
  //  poid.push_back(props[pksize - 1]->getName());
  //  if (props[pksize - 1]->getDatasetName() != props[pksize]->getDatasetName())
  //    break;
  //}

  //for (size_t i = 0; i < pksize; ++i)
  //  orderBy->push_back(new te::da::OrderByItem(poid[i]));

  //selectaux->setOrderBy(orderBy);
  //qlayer->setQuery(selectaux);

  //std::size_t idx;

  //for (std::size_t t = 0; t < dsType->getProperties().size(); ++t)
  //{
  //  if (dsType->getProperty(t)->getName() == attrName)
  //  {
  //    idx = t;
  //    break;
  //  }
  //}

  //std::vector<std::string> pkdata(pksize), pkdataaux(pksize);
  //std::auto_ptr<te::da::DataSet> ds(m_layer->getData());
  //qlayer->setQuery(select);

  //bool nullValue = false;
  //std::vector<std::string> values;
  //bool isBegin = true;
  //ds->moveBeforeFirst();

  //while (ds->moveNext())
  //{
  //  if (pksize)
  //  {
  //    // it is linked. Remove redundancies.
  //    size_t i;
  //    for (i = 0; i < pksize; ++i)
  //    {
  //      pkdata[i] = ds->getAsString(poid[i]);
  //      if (isBegin)
  //      {
  //        isBegin = false;
  //        pkdataaux[i] = ds->getAsString(poid[i]);
  //      }
  //    }

  //    for (i = 0; i < pksize; ++i)
  //    {
  //      if (pkdata[i] != pkdataaux[i])
  //      {
  //        pkdataaux = pkdata;
  //        break;
  //      }
  //    }
  //    if (i == pksize) // it is the same object
  //    {
  //      if (nullValue == false)
  //      {
  //        if (ds->isNull(idx))
  //          nullValue = true;
  //        else
  //          values.push_back(ds->getAsString(idx));
  //      }
  //      continue;
  //      // read other values
  //    }
  //    else // it is other object
  //    {
  //      // sumarize value according to the required summarization 
  //      if (nullValue)
  //        ++nullValues;
  //      else
  //        vec.push_back(te::da::GetSummarizedValue(values, function));

  //      nullValue = false;
  //      values.clear();

  //      // get new value
  //      if (ds->isNull(idx))
  //        nullValue = true;
  //      else
  //        values.push_back(ds->getAsString(idx));
  //    }
  //  }
  //}
  //// sumarize value according to the required summarization 
  //if (nullValue)
  //  ++nullValues;
  //else
  //  vec.push_back(te::da::GetSummarizedValue(values, function));
  //values.clear();
}

void te::qt::widgets::RasterizationWizardPage::buildSymbolizer(std::string meanTitle)
{
  delete m_cb;

  m_cb = m_colorBar->getColorBar()->getColorBar();

  int legendSize = m_legend.size();

  std::vector<te::color::RGBAColor> colorVec;

  if (meanTitle.empty())
  {
    colorVec = m_cb->getSlices(legendSize);
  }
  else
  {
    int beforeMean = 0;
    int afterMean = 0;
    int meanIdx = 0;

    for (size_t t = 0; t < m_legend.size(); ++t)
    {
      if (m_legend[t]->getTitle() != meanTitle)
      {
        beforeMean++;
      }
      else
      {
        meanIdx = (int)t;
        afterMean = (int)(m_legend.size() - t - 1);
        break;
      }
    }

    std::vector<te::color::RGBAColor> lowerColorVec = m_cb->getLowerMeanSlices(beforeMean);
    te::color::RGBAColor meanColor = m_cb->getMeanSlice();
    std::vector<te::color::RGBAColor> upperColorVec = m_cb->getUpperMeanSlices(afterMean);

    for (size_t t = 0; t < lowerColorVec.size(); ++t)
      colorVec.push_back(lowerColorVec[t]);

    colorVec.push_back(meanColor);

    for (size_t t = 0; t < upperColorVec.size(); ++t)
      colorVec.push_back(upperColorVec[t]);
  }

  if (colorVec.size() != m_legend.size())
    return;

  int geomType = getGeometryType();

  for (size_t t = 0; t < colorVec.size(); ++t)
  {
    std::vector<te::se::Symbolizer*> symbVec;

    te::se::Symbolizer* s = te::se::CreateSymbolizer((te::gm::GeomType)geomType, colorVec[t].getColor());

    symbVec.push_back(s);

    m_legend[t]->setSymbolizers(symbVec);
  }
}

void te::qt::widgets::RasterizationWizardPage::createStringNullGroupingItem(int count)
{
  if (count == 0)
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

void te::qt::widgets::RasterizationWizardPage::updateUi(bool loadColorBar)
{
  disconnect(m_ui->m_tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(onTableWidgetItemChanged(QTableWidgetItem*)));

  m_ui->m_tableWidget->setRowCount(0);

  QStringList list;
  list.append(tr("Class"));
  list.append(tr("R"));
  list.append(tr("G"));
  list.append(tr("B"));
  list.append(tr("Color"));
  list.append(tr("Select"));

  m_ui->m_tableWidget->setColumnCount(6);
  m_ui->m_tableWidget->setHorizontalHeaderLabels(list);

  te::color::ColorBar* cb = 0;

  if (loadColorBar)
  {
    if (!m_legend.empty() && !m_legend[0]->getSymbolizers().empty() && !m_legend[m_legend.size() - 1]->getSymbolizers().empty())
    {
      te::se::SymbolizerColorFinder scf;

      scf.find(m_legend[0]->getSymbolizers()[0]);
      te::color::RGBAColor initColor = scf.getColor();

      scf.find(m_legend[m_legend.size() - 1]->getSymbolizers()[0]);
      te::color::RGBAColor endColor = scf.getColor();

      cb = new te::color::ColorBar(initColor, endColor, 256);
    }
  }

  for (std::size_t t = 0; t < m_legend.size(); ++t)
  {
    te::map::GroupingItem* gi = m_legend[t];

    int newrow = m_ui->m_tableWidget->rowCount();
    m_ui->m_tableWidget->insertRow(newrow);

    //class
    {
      //Value
      QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(gi->getValue()));
      item->setFlags(Qt::ItemIsEnabled);
      m_ui->m_tableWidget->setItem(newrow, 0, item);
    }

    const std::vector<te::se::Symbolizer*>& ss = gi->getSymbolizers();
    te::se::PolygonSymbolizer* ps = (te::se::PolygonSymbolizer*)ss[0];
    const te::se::Fill* fill = ps->getFill();
    te::color::RGBAColor color = te::se::GetColor(fill->getColor());

    //R
    {
      QTableWidgetItem* item = new QTableWidgetItem(QString::number(color.getRed()));
      m_ui->m_tableWidget->setItem(newrow, 1, item);
    }

    //G
    {
      QTableWidgetItem* item = new QTableWidgetItem(QString::number(color.getGreen()));
      m_ui->m_tableWidget->setItem(newrow, 2, item);
    }

    //B
    {
      QTableWidgetItem* item = new QTableWidgetItem(QString::number(color.getBlue()));
      m_ui->m_tableWidget->setItem(newrow, 3, item);
    }

    //Color
    {
      const std::vector<te::se::Symbolizer*>& ss = gi->getSymbolizers();
      QPixmap pix = te::qt::widgets::SymbologyPreview::build(ss, QSize(24, 24));
      QIcon icon(pix);
      QTableWidgetItem* item = new QTableWidgetItem(icon, "");
      item->setFlags(Qt::ItemIsEnabled);
      m_ui->m_tableWidget->setItem(newrow, 4, item);
    }

    //checked
    {
      QTableWidgetItem* item = new QTableWidgetItem();
      item->setCheckState(Qt::Checked);
      m_ui->m_tableWidget->setItem(newrow, 5, item);
    }


    /*
    if (loadColorBar)
    {
      if (count != 0 && count != m_legend.size() - 1)
      {
        double pos = (1. / (m_legend.size() - 1)) * count;

        if (!gi->getSymbolizers().empty())
        {
          te::se::SymbolizerColorFinder scf;

          scf.find(gi->getSymbolizers()[0]);

          te::color::RGBAColor color = scf.getColor();

          if (cb)
            cb->addColor(color, pos);
        }
      }
    }*/
  }

  if (cb)
  {
    disconnect(m_colorBar, SIGNAL(colorBarChanged()), this, SLOT(onColorBarChanged()));

    te::qt::widgets::colorbar::ColorBar* cbW = m_colorBar->getColorBar();
    cbW->setColorBar(cb);

    connect(m_colorBar, SIGNAL(colorBarChanged()), this, SLOT(onColorBarChanged()));
  }

  m_ui->m_tableWidget->resizeColumnsToContents();

  connect(m_ui->m_tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(onTableWidgetItemChanged(QTableWidgetItem*)));
}

int te::qt::widgets::RasterizationWizardPage::getGeometryType()
{
  assert(m_layer.get());

  return te::map::GetGeomType(m_layer);
}

void te::qt::widgets::RasterizationWizardPage::listAttributes()
{
  QString curValue = m_ui->m_attrComboBox->currentText();

  m_ui->m_attrComboBox->clear();

  std::auto_ptr<te::map::LayerSchema> dsType(m_layer->getSchema());

  for (size_t t = 0; t < dsType->getProperties().size(); ++t)
  {
    te::dt::Property* p = dsType->getProperty(t);

    int dataType = p->getType();

    switch (dataType)
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

  if (curValue.isEmpty() == false)
  {
    int idx = m_ui->m_attrComboBox->findText(curValue);

    if (idx != -1)
      m_ui->m_attrComboBox->setCurrentIndex(idx);
  }
}

void te::qt::widgets::RasterizationWizardPage::onSelectAllPushButtonClicked()
{
  int size = m_ui->m_tableWidget->rowCount();

  for (std::size_t i = 0; i < size; ++i)
  {
    QTableWidgetItem* item = m_ui->m_tableWidget->item(i, 5);
    item->setCheckState(Qt::Checked);
  }
}

void te::qt::widgets::RasterizationWizardPage::onUnselectAllPushButtonClicked()
{
  int size = m_ui->m_tableWidget->rowCount();

  for (std::size_t i = 0; i < size; ++i)
  {
    QTableWidgetItem* item = m_ui->m_tableWidget->item(i, 5);
    item->setCheckState(Qt::Unchecked);
  }
}

void te::qt::widgets::RasterizationWizardPage::onResXLineEditEditingFinished()
{
  if (m_ui->m_resXLineEdit->text().isEmpty())
    return;

  te::gm::Envelope env = m_layer->getExtent();
  if (!env.isValid())
  {
    QMessageBox::warning(this, tr("Cellular Spaces"), tr("Invalid bounding box."));
    return;
  }

  double resX = m_ui->m_resXLineEdit->text().toDouble();

  int maxCols = (int)ceil((env.m_urx - env.m_llx) / resX);
  m_ui->m_colsLineEdit->setText(QString::number(maxCols));
}

void te::qt::widgets::RasterizationWizardPage::onResYLineEditEditingFinished()
{
  if (m_ui->m_resYLineEdit->text().isEmpty())
    return;

  te::gm::Envelope env = m_layer->getExtent();
  if (!env.isValid())
  {
    QMessageBox::warning(this, tr("Cellular Spaces"), tr("Invalid bounding box."));
    return;
  }

  double resY = m_ui->m_resXLineEdit->text().toDouble();
  //unitConvertion(resY, getResolutionUnit(), te::srs::SpatialReferenceSystemManager::getInstance().getUnit(m_bbSRID));

  int maxRows = (int)ceil((env.m_ury - env.m_lly) / resY);
  m_ui->m_rowsLineEdit->setText(QString::number(maxRows));
}

bool te::qt::widgets::RasterizationWizardPage::outputDataSourceToFile()
{
  return m_toFile;
}

te::da::DataSourceInfoPtr te::qt::widgets::RasterizationWizardPage::getDataSourceInfo()
{
  return m_outputDatasource;
}

double te::qt::widgets::RasterizationWizardPage::getResX()
{
  return m_ui->m_resXLineEdit->text().toDouble();
}

double te::qt::widgets::RasterizationWizardPage::getResY()
{
  return m_ui->m_resYLineEdit->text().toDouble();
}

te::gm::Envelope te::qt::widgets::RasterizationWizardPage::getEnvelope()
{
  te::gm::Envelope env = m_layer->getExtent();
  return env;
}

int te::qt::widgets::RasterizationWizardPage::getSrid()
{
  return m_layer->getSRID();
}

void te::qt::widgets::RasterizationWizardPage::onTargetFileToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_repositoryLineEdit->clear();

  QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
    QString(), tr("TIFF (*.tif *.TIF);;"), 0, QFileDialog::DontConfirmOverwrite);

  if (fileName.isEmpty())
    return;

  boost::filesystem::path outfile(fileName.toStdString());
  std::string aux = outfile.leaf().string();
  m_ui->m_newLayerNameLineEdit->setText(aux.c_str());
  aux = outfile.string();
  m_ui->m_repositoryLineEdit->setText(aux.c_str());

  m_toFile = true;
  m_ui->m_newLayerNameLineEdit->setEnabled(false);
}

std::string te::qt::widgets::RasterizationWizardPage::getRepositoryName()
{
  if (m_ui->m_repositoryLineEdit->text().isEmpty())
    return "";

  return m_ui->m_repositoryLineEdit->text().toStdString();
}

std::string te::qt::widgets::RasterizationWizardPage::getAttributeName()
{
  return m_ui->m_attrComboBox->currentText().toStdString();
}

std::map<std::string, std::vector<int> > te::qt::widgets::RasterizationWizardPage::getInformations()
{
  std::map<std::string, std::vector<int> > result;

  for (std::size_t i = 0; i < m_ui->m_tableWidget->rowCount(); ++i)
  {
    if (m_ui->m_tableWidget->item(i, 5)->checkState() != Qt::Checked)
      continue;

    std::vector<int> v;
    v.push_back((int)i+1);
    v.push_back(m_ui->m_tableWidget->item(i, 1)->text().toInt());
    v.push_back(m_ui->m_tableWidget->item(i, 2)->text().toInt());
    v.push_back(m_ui->m_tableWidget->item(i, 3)->text().toInt());

    result[m_ui->m_tableWidget->item(i, 0)->text().toStdString()] = v;
  }

  return result;
}

void te::qt::widgets::RasterizationWizardPage::validate()
{
  bool hasErrors = false;

  std::string errors = "";

  if (!m_layer)
  {
    if (hasErrors)
      errors += "\n";

    errors += " - " + std::string(TE_TR("Layer Invalid."));

    hasErrors = true;
  }

  if (m_ui->m_attrComboBox->currentText().isEmpty())
  {
    if (hasErrors)
      errors += "\n";

    errors += " - " + std::string(TE_TR("Attribute not selected."));
    
    hasErrors = true;
  }

  if (m_ui->m_resXLineEdit->text().isEmpty() || m_ui->m_resYLineEdit->text().isEmpty())
  {
    if (hasErrors)
      errors += "\n";

    errors += " - " + std::string(TE_TR("Resolution not informed."));

    hasErrors = true;
  }

  if (m_ui->m_repositoryLineEdit->text().isEmpty())
  {
    if (hasErrors)
      errors += "\n";

    errors += " - " + std::string(TE_TR("Output repository not informed."));

    hasErrors = true;
  }

  if (m_ui->m_tableWidget->rowCount() < 1)
  {
    if (hasErrors)
      errors += "\n";

    errors += " - " + std::string(TE_TR("No legend was created. Choose the attribute and apply."));

    hasErrors = true;
  }

  if (hasErrors)
  {
    std::string msg = std::string(TE_TR("Some errors accured:\n\n"));
    msg += errors;

    throw te::common::Exception(msg);
  }
}

std::vector<te::map::GroupingItem*> te::qt::widgets::RasterizationWizardPage::getGroupingItems()
{
  return m_legend;
}

QTableWidget* te::qt::widgets::RasterizationWizardPage::getTableWidget()
{
  return m_ui->m_tableWidget;
}