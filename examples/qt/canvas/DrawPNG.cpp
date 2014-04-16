#include "CanvasExamples.h"

// TerraLib
#include <terralib/geometry.h>
#include <terralib/dataaccess.h>
#include <terralib/qt/widgets/canvas/Canvas.h>

// STL
#include <iostream>
#include <exception>

// Qt
#include <QApplication>

void DrawPNG()
{
// If you want to use any Qt resource you must initialize a QApplication in some place!!!
  int argc = 0;
  QApplication app(argc, 0);

  try
  {
    std::auto_ptr<te::da::DataSource> dsOGR = te::da::DataSourceFactory::make("OGR");

    std::map<std::string, std::string> connInfo;    
    connInfo["URI"] = ""TE_DATA_EXAMPLE_DIR"/data/shp/munic_2001.shp";
    dsOGR->setConnectionInfo(connInfo);
    dsOGR->open();

    std::auto_ptr<te::da::DataSourceTransactor> t = dsOGR->getTransactor();

    std::auto_ptr<te::da::DataSetType> dt = (dsOGR->getDataSetType("munic_2001"));
    std::auto_ptr<te::da::DataSet> dataset = t->getDataSet("munic_2001");
    std::size_t pos = te::da::GetFirstPropertyPos(dataset.get(), te::dt::GEOMETRY_TYPE);
    std::auto_ptr<te::gm::Envelope>  extent = dataset->getExtent(pos);
    te::gm::GeometryProperty* gcol = te::da::GetFirstGeomProperty(dt.get());

    if(extent.get() == 0)
      throw("Extent not loaded!");


    te::qt::widgets::Canvas canvas(800, 600);

    double llx = extent->m_llx;
    double lly = extent->m_lly;
    double urx = extent->m_urx;
    double ury = extent->m_ury;

    canvas.calcAspectRatio(llx,lly,urx,ury);
    canvas.setWindow(llx,lly,urx,ury);

    if(gcol->getGeometryType() == te::gm::MultiPolygonType ||
       gcol->getGeometryType() == te::gm::PolygonType)
    {
      canvas.setPolygonFillColor(te::color::RGBAColor(255, 255, 0, 255));
      canvas.setPolygonContourColor(te::color::RGBAColor(0, 0, 0, 255));
    }
    else if(gcol->getGeometryType() == te::gm::MultiLineStringType)
    {
      canvas.setLineColor(te::color::RGBAColor(0, 255, 0, 255));
    }
    else
    {
      canvas.setPointColor(te::color::RGBAColor(255, 0, 0, 255));
    }

    std::string geomName = (te::da::GetFirstGeomProperty(dt.get()))->getName();

    while(dataset->moveNext())
    {
      std::auto_ptr<te::gm::Geometry> g = dataset->getGeometry(geomName);
      canvas.draw(g.get());
    }

    std::string fileName = dt->getName() + ".png";
    canvas.save(fileName.c_str(), te::map::PNG);
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried in drawPNG example: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried in drawPNG example!" << std::endl;
  }
}

