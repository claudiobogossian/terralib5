#include "SymbologyWidgets.h"

// TerraLib
#include <terralib/qt/widgets/se/RasterSymbolizerDialog.h>
#include <terralib/raster.h>
#include <terralib/raster/RasterSummary.h>
#include <terralib/raster/RasterSummaryManager.h>
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>
#include <terralib/se.h>
#include <terralib/gdal/Platform.h>

// Qt
#include <QtGui/QApplication>

void RasterSymbologyWidgets()
{
  int argc = 1;
  QApplication app(argc, 0);

  // open raster
  te::gdal::Platform::initialize();

  // set input raster name
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = "D:/landsat.tif";

// open input raster
  te::da::DataSource* ds = te::da::DataSourceFactory::make("GDAL");
  ds->open(rinfo);

  te::da::DataSourceTransactor* tr = ds->getTransactor();
  te::da::DataSetType* dt = tr->getCatalogLoader()->getDataSetType("landsat.tif");
  te::rst::RasterProperty* rstp = static_cast<te::rst::RasterProperty*>(dt->getProperties()[0]->clone());

  te::da::DataSet* dataSet = tr->getDataSet("landsat.tif");
  te::rst::Raster* raster = dataSet->getRaster();
//  const te::rst::RasterSummary* rsummary = te::rst::RasterSummaryManager::getInstance().get(raster, te::rst::SUMMARY_ALL);
//  const std::complex<double>* cmin = rsummary->at(0).m_minVal;
//  const std::complex<double>* cmax = rsummary->at(0).m_maxVal;
//  double min = cmin->real();
//  double max = cmax->real();

  te::se::RasterSymbolizer* rs = 0;

  // Creates the rastersymbolizer dialog
  te::qt::widgets::RasterSymbolizerDialog dlg;

  dlg.setProperty(rstp);

  if(dlg.exec() == QDialog::Accepted)
  {
    rs = static_cast<te::se::RasterSymbolizer*>(dlg.getRasterSymbolizer());
  }

  te::gdal::Platform::finalize();
}
