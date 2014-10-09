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
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../geometry/MultiPolygon.h"
#include "../../../raster/Raster.h"
#include "../../../raster/PositionIterator.h"
#include "../../../rp/ClassifierEMStrategy.h"
#include "../../../rp/ClassifierKMeansStrategy.h"
#include "../../../rp/ClassifierISOSegStrategy.h"
#include "../classification/ROIManagerDialog.h"
#include "../classification/ROIManagerWidget.h"
#include "ClassifierWizardPage.h"
#include "ui_ClassifierWizardPageForm.h"

// Qt
#include <QGridLayout>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QMessageBox>

// stl
#include <memory>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::qt::widgets::ClassifierWizardPage::ClassifierWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::ClassifierWizardPageForm),
    m_layer(0)
{
// setup controls
  m_ui->setupUi(this);

  fillClassifierTypes();

  QIntValidator* intMaxPoints = new QIntValidator(this);
  intMaxPoints->setBottom(0);
  m_ui->m_kMeansMaxPointsLineEdit->setValidator(intMaxPoints);

  QDoubleValidator* doubleStopCriteria = new QDoubleValidator(this);
  doubleStopCriteria->setBottom(0.0);
  m_ui->m_kMeansStopCriteriaLineEdit->setValidator(doubleStopCriteria);

//configure page
  this->setTitle(tr("Classifier"));
  this->setSubTitle(tr("Select the type of classifier and set their specific parameters."));

  //configure roi manager
  m_roiMngDlg.reset(new te::qt::widgets::ROIManagerDialog(this, Qt::Tool));

  //connects
  connect(m_ui->m_acquireToolButton, SIGNAL(toggled(bool)), this, SLOT(showROIManager(bool)));
  connect(m_ui->m_samAcquireToolButton, SIGNAL(toggled(bool)), this, SLOT(showROIManager(bool)));
  connect(m_roiMngDlg.get(), SIGNAL(roiManagerClosed()), this, SLOT(onROIManagerClosed()));
  connect(m_ui->m_classifierTypeComboBox, SIGNAL(activated(int)), this, SIGNAL(completeChanged()));
  connect(m_ui->m_inputRasterBandsListWidget, SIGNAL(itemSelectionChanged()), this, SIGNAL(completeChanged()));

  connect(m_roiMngDlg->getWidget(), SIGNAL(roiSetChanged(te::cl::ROISet*)), this, SLOT(onRoiSetChanged(te::cl::ROISet*)));

  m_ui->m_acquireToolButton->setIcon(QIcon::fromTheme("wand"));
  m_ui->m_samAcquireToolButton->setIcon(QIcon::fromTheme("wand"));
}

te::qt::widgets::ClassifierWizardPage::~ClassifierWizardPage()
{

}

bool te::qt::widgets::ClassifierWizardPage::isComplete() const
{
  int idx = m_ui->m_classifierTypeComboBox->currentIndex();
  int type = m_ui->m_classifierTypeComboBox->itemData(idx).toInt();

  if(type == CLASSIFIER_ISOSEG)
  {
    if(m_ui->m_isosegLayersComboBox->currentText().isEmpty())
      return false;
  }
  else if(type == CLASSIFIER_MAP)
  {
    if(m_ui->m_mapTableWidget->rowCount() == 0)
      return false;
  }
  else if(type == CLASSIFIER_SAM)
  {
    if(m_ui->m_samTableWidget->rowCount() == 0)
      return false;
  }

  if(m_ui->m_inputRasterBandsListWidget->selectedItems().empty())
    return false;

  return true;
}

void te::qt::widgets::ClassifierWizardPage::set(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

  m_roiMngDlg->set(m_layer);

  listBands();
}

void te::qt::widgets::ClassifierWizardPage::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_roiMngDlg->setList(layerList);

  m_ui->m_isosegLayersComboBox->clear();

  std::list<te::map::AbstractLayerPtr>::iterator it = layerList.begin();

  while(it != layerList.end())
  {
    te::map::AbstractLayerPtr l = *it;

    std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

    if(dsType->hasGeom())
      m_ui->m_isosegLayersComboBox->addItem(it->get()->getTitle().c_str(), QVariant::fromValue(l));

    ++it;
  }
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

    //get layer
    int idxLayer = m_ui->m_isosegLayersComboBox->currentIndex();
    QVariant varLayer = m_ui->m_isosegLayersComboBox->itemData(idxLayer, Qt::UserRole);
    te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

    //get polygons
    std::auto_ptr<te::da::DataSetType> dsType = layer->getSchema();
    te::gm::GeometryProperty* gp = te::da::GetFirstGeomProperty(dsType.get());

    if(gp && gp->getGeometryType() == te::gm::MultiPolygonType)
    {
      std::auto_ptr<te::da::DataSet> ds = layer->getData();
      ds->moveBeforeFirst();

      while(ds->moveNext())
      {
        te::gm::MultiPolygon* mp = (te::gm::MultiPolygon*)ds->getGeometry(gp->getName()).release();
        te::gm::Polygon* poly = (te::gm::Polygon*)mp->getGeometries()[0];

        algoInputParams.m_inputPolygons.push_back(poly);
      }
    }

    algoInputParams.setClassifierStrategyParams(classifierparameters);
  }
  else  if(type == CLASSIFIER_KMEANS)
  {
    algoInputParams.m_strategyName = "kmeans";

    te::rp::ClassifierKMeansStrategy::Parameters classifierparameters;
    classifierparameters.m_K = m_ui->m_kMeansClusterSpinBox->value();
    classifierparameters.m_maxIterations = m_ui->m_kMeansIterationsSpinBox->value();
    classifierparameters.m_maxInputPoints = m_ui->m_kMeansMaxPointsLineEdit->text().toInt();
    classifierparameters.m_epsilon = m_ui->m_kMeansStopCriteriaLineEdit->text().toDouble();

    algoInputParams.setClassifierStrategyParams(classifierparameters);
  }
  else if(type == CLASSIFIER_MAP)
  {
    algoInputParams.m_strategyName = "map";

    std::auto_ptr<te::da::DataSet> ds = m_layer->getData();
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
    std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

    te::rp::ClassifierMAPStrategy::Parameters classifierparameters;
    classifierparameters.m_trainSamplesPtr = getMAPSamples(m_roiMngDlg->getWidget()->getROISet(), inputRst.get());

    algoInputParams.setClassifierStrategyParams(classifierparameters);
  }
  else if(type == CLASSIFIER_EM)
  {
    algoInputParams.m_strategyName = "em";

    te::rp::ClassifierEMStrategy::Parameters classifierparameters;
    classifierparameters.m_numberOfClusters = m_ui->m_emClusterSpinBox->value();
    classifierparameters.m_maxIterations = m_ui->m_emIterationsSpinBox->value();
    classifierparameters.m_maxInputPoints = m_ui->m_emMaxPointsLineEdit->text().toInt();
    classifierparameters.m_epsilon = m_ui->m_emStopCriteriaLineEdit->text().toDouble();
    classifierparameters.m_clustersMeans = std::vector<std::vector<double> >();

    algoInputParams.setClassifierStrategyParams(classifierparameters);
  }
  else if(type == CLASSIFIER_SAM)
  {
    algoInputParams.m_strategyName = "sam";

    std::auto_ptr<te::da::DataSet> ds = m_layer->getData();
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
    std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

    te::rp::ClassifierSAMStrategy::Parameters classifierparameters;
    classifierparameters.m_trainSamplesPtr = getSAMSamples(m_roiMngDlg->getWidget()->getROISet(), inputRst.get());

    //get angle values
    for(int i = 0; i < m_ui->m_samTableWidget->rowCount(); ++i)
    {
      double val = ((QDoubleSpinBox*)m_ui->m_samTableWidget->cellWidget(i, 3))->value();

      classifierparameters.m_maxAngularDistances.push_back(val);
    }

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
  m_ui->m_classifierTypeComboBox->addItem(tr("KMeans"), CLASSIFIER_KMEANS);
  m_ui->m_classifierTypeComboBox->addItem(tr("MAP - Maximum a Posteriori Probability"), CLASSIFIER_MAP);
  m_ui->m_classifierTypeComboBox->addItem(tr("EM - Expectation Maximization"), CLASSIFIER_EM);
  m_ui->m_classifierTypeComboBox->addItem(tr("SAM - Spectral Angle Mapper"), CLASSIFIER_SAM);
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

te::rp::ClassifierMAPStrategy::Parameters::MClassesSamplesCTPtr te::qt::widgets::ClassifierWizardPage::getMAPSamples(te::cl::ROISet* rs, te::rst::Raster* raster)
{
  te::rp::ClassifierMAPStrategy::Parameters::MClassesSamplesCTPtr mcsctPtr(new te::rp::ClassifierMAPStrategy::Parameters::MClassesSamplesCT());

  std::map<std::string, te::cl::ROI*> roiSetMap = rs->getROISet();
  std::map<std::string, te::cl::ROI*>::iterator it = roiSetMap.begin();

  int count = 1;

  //iterate roi set
  while(it != roiSetMap.end())
  {
    std::map<std::string, te::gm::Polygon*> roiMap = it->second->getPolygons();
    std::map<std::string, te::gm::Polygon*>::iterator itPoly = roiMap.begin();

    te::rp::ClassifierMAPStrategy::Parameters::ClassSamplesContainerT csct;

    //iterate roi
    while(itPoly != roiMap.end())
    {
      te::gm::Polygon* p = itPoly->second;

      te::rst::PolygonIterator<double> itRaster = te::rst::PolygonIterator<double>::begin(raster, p);
      te::rst::PolygonIterator<double> itRasterEnd = te::rst::PolygonIterator<double>::end(raster, p);

      //iterate polygon
      while (itRaster != itRasterEnd)
      {
        te::rp::ClassifierMAPStrategy::Parameters::ClassSampleT cst;

        raster->getValues(itRaster.getColumn(), itRaster.getRow(), cst);

        csct.push_back(cst);

        ++itRaster;
      }

      ++itPoly;
    }

    mcsctPtr->insert(te::rp::ClassifierMAPStrategy::Parameters::MClassesSamplesCT::value_type(count, csct));

    ++count;

    ++it;
  }

  return mcsctPtr;
}

te::rp::ClassifierSAMStrategy::ClassesSamplesTPtr te::qt::widgets::ClassifierWizardPage::getSAMSamples(te::cl::ROISet* rs, te::rst::Raster* raster)
{
  te::rp::ClassifierSAMStrategy::ClassesSamplesTPtr cstPtr(new te::rp::ClassifierSAMStrategy::ClassesSamplesT());

  std::map<std::string, te::cl::ROI*> roiSetMap = rs->getROISet();
  std::map<std::string, te::cl::ROI*>::iterator it = roiSetMap.begin();

  int count = 1;

  //iterate roi set
  while(it != roiSetMap.end())
  {
    std::map<std::string, te::gm::Polygon*> roiMap = it->second->getPolygons();
    std::map<std::string, te::gm::Polygon*>::iterator itPoly = roiMap.begin();

    te::rp::ClassifierSAMStrategy::SamplesT st;

    //iterate roi
    while(itPoly != roiMap.end())
    {
      te::gm::Polygon* p = itPoly->second;

      te::rst::PolygonIterator<double> itRaster = te::rst::PolygonIterator<double>::begin(raster, p);
      te::rst::PolygonIterator<double> itRasterEnd = te::rst::PolygonIterator<double>::end(raster, p);

      //iterate polygon
      while (itRaster != itRasterEnd)
      {
        te::rp::ClassifierSAMStrategy::SampleT s;

        raster->getValues(itRaster.getColumn(), itRaster.getRow(), s);

        st.push_back(s);

        ++itRaster;
      }

      ++itPoly;
    }

    cstPtr->insert(te::rp::ClassifierSAMStrategy::ClassesSamplesT::value_type(count, st));

    ++count;

    ++it;
  }

  return cstPtr;
}

void te::qt::widgets::ClassifierWizardPage::showROIManager(bool show)
{
  if(show)
    m_roiMngDlg->show();
  else
    m_roiMngDlg->hide();
}

void te::qt::widgets::ClassifierWizardPage::onROIManagerClosed()
{
  m_ui->m_acquireToolButton->setChecked(false);
  m_ui->m_samAcquireToolButton->setChecked(false);
}

void te::qt::widgets::ClassifierWizardPage::onRoiSetChanged(te::cl::ROISet* rs)
{
  int idx = m_ui->m_classifierTypeComboBox->currentIndex();
  int type = m_ui->m_classifierTypeComboBox->itemData(idx).toInt();

  if(type == CLASSIFIER_MAP)
  {
    m_ui->m_mapTableWidget->setRowCount(0);

    if(!rs)
      return;

    std::map<std::string, te::cl::ROI*> roiSetMap = rs->getROISet();
    std::map<std::string, te::cl::ROI*>::iterator it = roiSetMap.begin();

    //iterate roi set
    while(it != roiSetMap.end())
    {
      //get roi info
      te::cl::ROI* r = it->second;

      std::string label = r->getLabel();

      std::size_t samples = r->getPolygons().size();
      QString samplesNum;
      samplesNum.setNum(samples);

      QColor color(r->getColor().c_str());

      //add table entry
      int newrow = m_ui->m_mapTableWidget->rowCount();
      m_ui->m_mapTableWidget->insertRow(newrow);

      QPixmap pix(16,16);
      pix.fill(color);
      QIcon icon(pix);

      QTableWidgetItem* itemColor = new QTableWidgetItem(icon, "");
      itemColor->setFlags(Qt::ItemIsEnabled);
      m_ui->m_mapTableWidget->setItem(newrow, 0, itemColor);

      QTableWidgetItem* itemLabel = new QTableWidgetItem(QString::fromStdString(label));
      itemLabel->setFlags(Qt::ItemIsEnabled);
      m_ui->m_mapTableWidget->setItem(newrow, 1, itemLabel);

      QTableWidgetItem* itemSamples = new QTableWidgetItem(samplesNum);
      itemSamples->setFlags(Qt::ItemIsEnabled);
      m_ui->m_mapTableWidget->setItem(newrow, 2, itemSamples);

      ++it;
    }

    m_ui->m_mapTableWidget->resizeColumnsToContents();
  }
  else if(type == CLASSIFIER_SAM)
  {
    m_ui->m_samTableWidget->setRowCount(0);

    if(!rs)
      return;

    std::map<std::string, te::cl::ROI*> roiSetMap = rs->getROISet();
    std::map<std::string, te::cl::ROI*>::iterator it = roiSetMap.begin();

    //iterate roi set
    while(it != roiSetMap.end())
    {
      //get roi info
      te::cl::ROI* r = it->second;

      std::string label = r->getLabel();

      std::size_t samples = r->getPolygons().size();
      QString samplesNum;
      samplesNum.setNum(samples);

      QColor color(r->getColor().c_str());

      //add table entry
      int newrow = m_ui->m_mapTableWidget->rowCount();
      m_ui->m_samTableWidget->insertRow(newrow);

      QPixmap pix(16,16);
      pix.fill(color);
      QIcon icon(pix);

      QTableWidgetItem* itemColor = new QTableWidgetItem(icon, "");
      itemColor->setFlags(Qt::ItemIsEnabled);
      m_ui->m_samTableWidget->setItem(newrow, 0, itemColor);

      QTableWidgetItem* itemLabel = new QTableWidgetItem(QString::fromStdString(label));
      itemLabel->setFlags(Qt::ItemIsEnabled);
      m_ui->m_samTableWidget->setItem(newrow, 1, itemLabel);

      QTableWidgetItem* itemSamples = new QTableWidgetItem(samplesNum);
      itemSamples->setFlags(Qt::ItemIsEnabled);
      m_ui->m_samTableWidget->setItem(newrow, 2, itemSamples);

      QDoubleSpinBox* dsb = new QDoubleSpinBox(m_ui->m_samTableWidget);
      dsb->setMinimum(0.0);
      dsb->setMaximum(1.0);
      dsb->setSingleStep(0.1);
      dsb->setValue(0.1);

      m_ui->m_samTableWidget->setCellWidget(newrow, 3, dsb);

      ++it;
    }

    m_ui->m_samTableWidget->resizeColumnsToContents();
  }

  emit completeChanged();
}

