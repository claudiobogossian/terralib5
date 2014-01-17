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
  \file terralib/qt/widgets/rp/ClassifierWizardPage.cpp

  \brief This file defines a class for a Classifier Wizard page.
*/

// TerraLib
#include "../../../common/StringUtils.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../raster/Raster.h"
#include "../../../rp/ClassifierISOSegStrategy.h"
#include "../../widgets/canvas/Canvas.h"
#include "../../widgets/canvas/MapDisplay.h"
#include "ClassifierWizardPage.h"
#include "RasterNavigatorWidget.h"
#include "RasterNavigatorDialog.h"
#include "ui_ClassifierWizardPageForm.h"

// Qt
#include <QGridLayout>
#include <QtGui/QMessageBox>

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>


// stl
#include <memory>

te::qt::widgets::ClassifierWizardPage::ClassifierWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::ClassifierWizardPageForm),
    m_countSamples(0),
    m_layer(0)
{
// setup controls
  m_ui->setupUi(this);

  fillClassifierTypes();

//connects
  connect(m_ui->m_tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(onItemChanged(QTableWidgetItem*)));
  connect(m_ui->m_removeToolButton, SIGNAL(clicked()), this, SLOT(onRemoveToolButtonClicked()));
  connect(m_ui->m_acquireToolButton, SIGNAL(toggled(bool)), this, SLOT(showNavigator(bool)));

  //configure raster navigator
  m_navigatorDlg.reset(new te::qt::widgets::RasterNavigatorDialog(this, Qt::Tool));
  m_navigatorDlg->setWindowTitle(tr("Display"));
  m_navigatorDlg->setMinimumSize(550, 400);
  //m_navigatorDlg->getWidget()->hideGeomTool(true);
  m_navigatorDlg->getWidget()->hideInfoTool(true);
  m_navigatorDlg->getWidget()->hidePickerTool(true);

  //connects
  connect(m_navigatorDlg.get(), SIGNAL(navigatorClosed()), this, SLOT(onNavigatorClosed()));
  connect(m_navigatorDlg->getWidget(), SIGNAL(mapDisplayExtentChanged()), this, SLOT(onMapDisplayExtentChanged()));
  connect(m_navigatorDlg->getWidget(), SIGNAL(geomAquired(te::gm::Polygon*, te::qt::widgets::MapDisplay*)), 
    this, SLOT(onGeomAquired(te::gm::Polygon*, te::qt::widgets::MapDisplay*)));


//configure page
  this->setTitle(tr("Classifier"));
  this->setSubTitle(tr("Select the type of classifier and set their specific parameters."));

  m_ui->m_removeToolButton->setIcon(QIcon::fromTheme("list-remove"));
  m_ui->m_acquireToolButton->setIcon(QIcon::fromTheme("wand"));
}

te::qt::widgets::ClassifierWizardPage::~ClassifierWizardPage()
{
  std::map<std::string, ClassifierSamples>::iterator it = m_samples.begin();

  while(it != m_samples.end())
  {
    delete it->second.m_poly;

    ++it;
  }

  m_samples.clear();
}

void te::qt::widgets::ClassifierWizardPage::set(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

  m_navigatorDlg->set(m_layer);

  listBands();
}

te::rp::Classifier::InputParameters te::qt::widgets::ClassifierWizardPage::getInputParams()
{
  int idx = m_ui->m_classifierTypeComboBox->currentIndex();
  int type = m_ui->m_classifierTypeComboBox->itemData(idx).toInt();

  te::rp::Classifier::InputParameters algoInputParams;

  if(type == CLASSIFIER_ISOSEG)
  {
    algoInputParams.m_strategyName = "isoseg";

    te::rp::ClassifierISOSegStrategy::Parameters classifierparameters;
    classifierparameters.m_acceptanceThreshold = m_ui->m_acceptanceThresholdComboBox->currentText().toDouble();

    algoInputParams.setClassifierStrategyParams(classifierparameters);
  }

  //get bands
  QList<QListWidgetItem*> selectedBands = m_ui->m_inputRasterBandsListWidget->selectedItems();

  QList<QListWidgetItem*>::const_iterator it = selectedBands.begin();
  QList<QListWidgetItem*>::const_iterator itend = selectedBands.end();

  while(it != itend)
  {
    algoInputParams.m_inputRasterBands.push_back((*it)->text().toUInt());

    ++it;
  }

  //get polygons
  std::map<std::string, ClassifierSamples>::iterator itSamples = m_samples.begin();

  std::vector<te::gm::Polygon*> polyVec;

  while(itSamples != m_samples.end())
  {
    polyVec.push_back(itSamples->second.m_poly);

    ++itSamples;
  }

  algoInputParams.m_inputPolygons = polyVec;

  return algoInputParams;
}

te::rp::Classifier::OutputParameters te::qt::widgets::ClassifierWizardPage::getOutputParams()
{
  te::rp::Classifier::OutputParameters algoOutputParams;

  return algoOutputParams;
}

void te::qt::widgets::ClassifierWizardPage::fillClassifierTypes()
{
  m_ui->m_classifierTypeComboBox->clear();

  m_ui->m_classifierTypeComboBox->addItem(tr("ISOSeg"), CLASSIFIER_ISOSEG);
}

void te::qt::widgets::ClassifierWizardPage::listBands()
{
  m_ui->m_inputRasterBandsListWidget->clear();

  assert(m_layer.get());

  //get input raster
  std::auto_ptr<te::da::DataSet> ds = m_layer->getData();

  if(ds.get())
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);

    std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

    if(inputRst.get())
    {
      for(unsigned int i = 0; i < inputRst->getNumberOfBands(); ++i)
      {
        m_ui->m_inputRasterBandsListWidget->addItem(QString::number(i));
      }
    }
  }
}

void te::qt::widgets::ClassifierWizardPage::drawSamples()
{
  assert(m_display);

  m_display->getDraftPixmap()->fill(QColor(0, 0, 0, 0));

  const te::gm::Envelope& mapExt = m_display->getExtent();

  te::qt::widgets::Canvas canvasInstance(m_display->getDraftPixmap());
  canvasInstance.setWindow(mapExt.m_llx, mapExt.m_lly, mapExt.m_urx, mapExt.m_ury);

  canvasInstance.setPolygonContourColor(te::color::RGBAColor(0,0,0, TE_OPAQUE));
  canvasInstance.setPolygonContourWidth(1);
  canvasInstance.setPolygonFillColor(te::color::RGBAColor(255,0,0, TE_OPAQUE));

  std::map<std::string, ClassifierSamples>::iterator it = m_samples.begin();

  while(it != m_samples.end())
  {
    te::gm::Polygon* poly = it->second.m_poly;

    canvasInstance.draw(poly);

    ++it;
  }

  m_display->repaint();
}

void te::qt::widgets::ClassifierWizardPage::updateSamples()
{
  m_ui->m_tableWidget->setRowCount(0);

  std::map<std::string, ClassifierSamples>::iterator it = m_samples.begin();

  while(it != m_samples.end())
  {
    int newrow = m_ui->m_tableWidget->rowCount();
    m_ui->m_tableWidget->insertRow(newrow);
    
    //name
    QTableWidgetItem* itemName = new QTableWidgetItem(QString::fromStdString(it->second.m_name));
    itemName->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
    itemName->setData(Qt::UserRole, QVariant(it->second.m_id.c_str()));
    m_ui->m_tableWidget->setItem(newrow, 0, itemName);

    ++it;
  }

  m_ui->m_tableWidget->sortByColumn(0, Qt::AscendingOrder);

  drawSamples();
}

void te::qt::widgets::ClassifierWizardPage::onMapDisplayExtentChanged()
{
  if(m_samples.empty() == false)
    drawSamples();
}

void te::qt::widgets::ClassifierWizardPage::onGeomAquired(te::gm::Polygon* poly, te::qt::widgets::MapDisplay* map)
{
  assert(m_layer.get());

  m_display = map;

  //component id
  static boost::uuids::basic_random_generator<boost::mt19937> gen;
  boost::uuids::uuid u = gen();
  std::string id = boost::uuids::to_string(u);

  //component name
  QString className = QString(tr("Sample ") + QString::number(m_countSamples++));

  ClassifierSamples cs;
  cs.m_id = id;
  cs.m_name = className.toStdString();
  cs.m_poly = poly;

  m_samples.insert(std::map<std::string, ClassifierSamples >::value_type(id, cs));

  updateSamples();
}

void te::qt::widgets::ClassifierWizardPage::onItemChanged(QTableWidgetItem* item)
{
  std::string id = item->data(Qt::UserRole).toString().toStdString();

  std::string name = item->text().toStdString();

  std::map<std::string, ClassifierSamples >::iterator it = m_samples.find(id);

  bool update = false;

  if(it != m_samples.end())
  {
    if(it->second.m_name != name)
    {
      it->second.m_name = name;
      update = true;
    }
  }

  if(update)
    updateSamples();
}

void te::qt::widgets::ClassifierWizardPage::onRemoveToolButtonClicked()
{
  if(m_ui->m_tableWidget->currentRow() == -1)
    return;

  std::string id = m_ui->m_tableWidget->item(m_ui->m_tableWidget->currentRow(), 0)->data(Qt::UserRole).toString().toStdString();

  std::map<std::string, ClassifierSamples >::iterator it = m_samples.find(id);

  if(it != m_samples.end())
  {
    m_samples.erase(it);

    if(m_samples.empty())
      m_countSamples = 0;

    updateSamples();
  }
}

void te::qt::widgets::ClassifierWizardPage::showNavigator(bool show)
{
  if(show)
    m_navigatorDlg->show();
  else
    m_navigatorDlg->hide();
}

void te::qt::widgets::ClassifierWizardPage::onNavigatorClosed()
{
  m_ui->m_acquireToolButton->setChecked(false);
}

