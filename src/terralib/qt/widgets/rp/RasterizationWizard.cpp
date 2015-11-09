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

//Terralib
#include "../../../common/progress/ProgressManager.h"
#include "../../../dataaccess/datasource/DataSource.h"
#include "../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../datatype/SimpleProperty.h"
#include "../../../geometry/Envelope.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../maptools/GroupingItem.h"
#include "../../../memory/DataSet.h"
#include "../../../memory/DataSetItem.h"
#include "../../../raster/BandProperty.h"
#include "../../../qt/widgets/utils/ScopedCursor.h"
#include "../../../qt/widgets/rp/Utils.h"
#include "../../../raster/Grid.h"
#include "../../../raster/RasterFactory.h"
#include "../../../se/Categorize.h"
#include "../../../se/ColorMap.h"
#include "../../../se/MapItem.h"
#include "../../../se/ParameterValue.h"
#include "../../../se/RasterSymbolizer.h"
#include "../../../se/Recode.h"
#include "../../../se/Utils.h"
#include "../help/HelpPushButton.h"
#include "../layer/search/LayerSearchWidget.h"
#include "../layer/search/LayerSearchWizardPage.h"
#include "../progress/ProgressViewerDialog.h"
#include "RasterizationWizard.h"
#include "RasterizationWizardPage.h"

// BOOST
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

// Qt
#include <QApplication>
#include <QColor>
#include <QMessageBox>
#include <QTableWidget>

te::qt::widgets::RasterizationWizard::RasterizationWizard(QWidget* parent) :
QWizard(parent)
{
  this->setWizardStyle(QWizard::ModernStyle);
  this->setWindowTitle(tr("Rasterization"));

  this->setOption(QWizard::HaveHelpButton, true);
  this->setOption(QWizard::HelpButtonOnRight, false);

  te::qt::widgets::HelpPushButton* helpButton = new te::qt::widgets::HelpPushButton(this);

  this->setButton(QWizard::HelpButton, helpButton);

  helpButton->setPageReference("plugins/rp/rp_rasterization.html");

  addPages();
}

te::qt::widgets::RasterizationWizard::~RasterizationWizard()
{
}

bool te::qt::widgets::RasterizationWizard::validateCurrentPage()
{
  if(currentPage() ==  m_layerSearchPage.get())
  {
    std::list<te::map::AbstractLayerPtr> list = m_layerSearchPage->getSearchWidget()->getSelecteds();

    if(list.empty() == false)
    {
      m_inputLayer = *list.begin();;

      m_vectorPage->setLayer(m_inputLayer);
    }

    return m_layerSearchPage->isComplete();
  }
  else if(currentPage() ==  m_vectorPage.get())
  {
    return execute();
  }

  return true;
}

void te::qt::widgets::RasterizationWizard::setList(std::list<te::map::AbstractLayerPtr> &layerList)
{
  m_layerSearchPage->getSearchWidget()->setList(layerList);
}

te::map::AbstractLayerPtr te::qt::widgets::RasterizationWizard::getOutputLayer()
{
  return m_outputLayer;
}

void te::qt::widgets::RasterizationWizard::addPages()
{
  m_layerSearchPage.reset(new LayerSearchWizardPage(this));
  addPage(m_layerSearchPage.get());

  m_vectorPage.reset(new RasterizationWizardPage(this));
  addPage(m_vectorPage.get());
}

void te::qt::widgets::RasterizationWizard::setDummy(te::rst::Raster* rst, const double dummyValue)
{
  int rows = rst->getNumberOfRows();
  int cols = rst->getNumberOfColumns();

  for (int r = 0; r < rows; ++r)
  {
    for (int c = 0; c < cols; ++c)
    {
      rst->setValue(c, r, dummyValue);
    }
  }
}

bool te::qt::widgets::RasterizationWizard::execute()
{
  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  try
  {
    m_vectorPage->validate();
  }
  catch (const te::common::Exception& e)
  {
    te::common::ProgressManager::getInstance().removeViewer(id);
    QMessageBox::warning(this, tr("Rasterization"), e.what());
    return false;
  }

  te::gm::Envelope envAux = m_vectorPage->getEnvelope();

  te::gm::Envelope* env = new te::gm::Envelope(envAux.m_llx, envAux.m_lly, envAux.m_urx, envAux.m_ury);

  double resX = m_vectorPage->getResX();
  double resY = m_vectorPage->getResY();
  int srid = m_vectorPage->getSrid();

  std::string uriStr = m_vectorPage->getRepositoryName();

  boost::filesystem::path uri(uriStr);

  std::string attrName = m_vectorPage->getAttributeName();

  std::map<std::string, std::vector<int> > infos = m_vectorPage->getInformations();

  if (infos.empty())
  {
    te::common::ProgressManager::getInstance().removeViewer(id);
    QMessageBox::warning(this, tr("Rasterization"), tr("None legend was created!"));
    return false;
  }

  te::rst::Grid* grid = new te::rst::Grid(resX, resY, env, srid);

  // create bands
  std::vector<te::rst::BandProperty*> vecBandProp;

  te::rst::BandProperty* bProp = createBandProperty(infos.size());
  bProp->m_noDataValue = 0;

  vecBandProp.push_back(bProp); 

  std::map<std::string, std::string> dsinfo;
  dsinfo["URI"] = uri.string();

  try
  {
    // create raster
    std::auto_ptr<te::rst::Raster> rst(te::rst::RasterFactory::make("GDAL", grid, vecBandProp, dsinfo));

    setDummy(rst.get(), 0);

    std::auto_ptr<te::da::DataSetType> schema = m_inputLayer->getSchema();
    std::auto_ptr<te::da::DataSet> data = m_inputLayer->getData();

    std::size_t geomPos = te::da::GetFirstSpatialPropertyPos(data.get());

    data->moveBeforeFirst();

    std::vector<te::gm::Geometry*> geoms;
    std::vector<double> values;

    {
      te::common::TaskProgress task;
      task.setMessage(TE_TR("Preparing..."));
      task.setTotalSteps((int)data->size());

      while (data->moveNext())
      {
        std::string classItem = data->getValue(attrName)->toString();

        if (infos.find(classItem) == infos.end())
          continue;

        std::auto_ptr<te::gm::Geometry> geom = data->getGeometry(geomPos);

        if (geom->isValid())
        {
          geoms.push_back(geom.release());
          values.push_back(infos[classItem][0]);
        }

        if (!task.isActive())
        {
          te::common::ProgressManager::getInstance().removeViewer(id);
          QMessageBox::warning(this, tr("Rasterize"), tr("Rasterize operation canceled!"));
          return false;
        }

        task.pulse();
      }
    }

    rst->rasterize(geoms, values, 0);

    rst.reset(0);

    m_outputLayer = te::qt::widgets::createLayer("GDAL", dsinfo);

    std::vector<te::map::GroupingItem*> items = m_vectorPage->getGroupingItems();

    te::se::Recode* r = new te::se::Recode();
    r->setFallbackValue("#000000");
    r->setLookupValue(new te::se::ParameterValue("Rasterdata"));

    te::se::ColorMap* cm = new te::se::ColorMap;

    QTableWidget* table = m_vectorPage->getTableWidget();

    std::size_t count = 0;
    
    std::map<std::string, std::vector<int> >::iterator it = infos.begin();
    for (it = infos.begin(); it != infos.end(); ++it)
    {
      QColor color;
      color.setRgb(it->second[1], it->second[2], it->second[3]);

      double dataDouble = it->second[0];
      std::string colorStr = color.name().toStdString();

      te::se::MapItem* m = new te::se::MapItem();
      m->setValue(new te::se::ParameterValue(colorStr));
      m->setData(dataDouble);
      m->setTitle(it->first);

      r->add(m);
    }

    if (cm)
    {
      cm->setCategorize(0);
      cm->setInterpolate(0);
      cm->setRecode(r);
    }

    te::se::RasterSymbolizer* rs = te::se::GetRasterSymbolizer(m_outputLayer->getStyle());
    rs->setColorMap(cm);

    boost::filesystem::path p(uri);
    p.replace_extension("leg");

    m_vectorPage->saveLegend(p.string());

  }
  catch (const std::exception& e)
  {
    te::common::ProgressManager::getInstance().removeViewer(id);
    QMessageBox::warning(this, tr("Rasterization"), e.what());
    return false;
  }

  te::common::ProgressManager::getInstance().removeViewer(id);

  return true;
}

std::auto_ptr<te::da::DataSetType> te::qt::widgets::RasterizationWizard::createDataSetType(std::string dataSetName, int srid)
{
  std::auto_ptr<te::da::DataSetType> dsType(new te::da::DataSetType(dataSetName));

  //create id property
  te::dt::SimpleProperty* idProperty = new te::dt::SimpleProperty("id", te::dt::INT32_TYPE);
  dsType->add(idProperty);

  //create geometry property
  te::gm::GeometryProperty* geomProperty = new te::gm::GeometryProperty("geom", srid, te::gm::PolygonType);
  dsType->add(geomProperty);

  //create primary key
  std::string pkName = "pk_id";
              pkName+= "_" + dataSetName;
  te::da::PrimaryKey* pk = new te::da::PrimaryKey(pkName, dsType.get());
  pk->add(idProperty);

  return dsType;
}

std::auto_ptr<te::mem::DataSet> te::qt::widgets::RasterizationWizard::createDataSet(te::da::DataSetType* dsType, std::vector<te::gm::Geometry*>& geoms)
{
  std::auto_ptr<te::mem::DataSet> ds(new te::mem::DataSet(dsType));

  for(std::size_t t = 0; t < geoms.size(); ++t)
  {
    //create dataset item
    te::mem::DataSetItem* item = new te::mem::DataSetItem(ds.get());

    //set id
    item->setInt32("id", (int)t);

    //set geometry
    item->setGeometry("geom", geoms[t]);

    ds->add(item);
  }

  return ds;
}

void te::qt::widgets::RasterizationWizard::saveDataSet(te::mem::DataSet* dataSet, te::da::DataSetType* dsType, te::da::DataSourcePtr ds, std::string dataSetName)
{
  //save dataset
  dataSet->moveBeforeFirst();

  std::map<std::string, std::string> options;

  ds->createDataSet(dsType, options);

  ds->add(dataSetName, dataSet, options);
}

te::rst::BandProperty* te::qt::widgets::RasterizationWizard::createBandProperty(const std::size_t size)
{
  int nbytes = std::ceil((log(size) / log(2.)) / 8);

  if (nbytes <= 1)
    return new te::rst::BandProperty(0, te::dt::CHAR_TYPE, "");
  else if (nbytes <= 2)
    return new te::rst::BandProperty(0, te::dt::INT16_TYPE, "");
  else if (nbytes <= 4)
    return new te::rst::BandProperty(0, te::dt::INT32_TYPE, "");
  else
    return new te::rst::BandProperty(0, te::dt::DOUBLE_TYPE, "");
}