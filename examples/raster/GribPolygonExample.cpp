#include "RasterExamples.h"

// TerraLib
#include <terralib/raster.h>
#include <terralib/geometry.h>

#if TE_EXAMPLE_USE_GRIB
#include <terralib/grib/Raster.h>
#include <terralib/grib/Band.h>
#endif  // TE_EXAMPLE_USE_GRIB

// STL
#include <iostream>
#include <map>
#include <memory>

te::gm::Polygon* createPolygon(const double& llx, const double& lly, const double& urx, const double& ury)
{
  te::gm::Polygon* p = new te::gm::Polygon(0, te::gm::PolygonType);

  te::gm::LinearRing* s = new te::gm::LinearRing(6, te::gm::LineStringType);

  s->setPoint(0, llx, lly); // lower left
  s->setPoint(1, llx, ury); // upper left
  s->setPoint(2, urx, ury); // upper rigth
  s->setPoint(3, urx, lly); // lower rigth
  s->setPoint(4, llx + (urx - llx) / 2.0 , lly  + (ury - lly) / 2.0); // lower rigth
  s->setPoint(5, llx, lly); // closing

  p->push_back(s);

  return p;
}

// pensar na possibilidade de poder clipar o raster = o raster de saida nao precisa ser das mesmas dimensoes do de entrada!

void Mask(te::rst::Raster* iraster,
          const te::gm::Polygon* poly,
          te::rst::Raster* oraster)
{
  double resx = iraster->getResolutionX();
  double resy = iraster->getResolutionY();

  const te::rst::Grid* grid = iraster->getGrid();

  const te::gm::Envelope* pmbr = poly->getMBR();

  te::gm::Coord2D cccci = grid->geoToGrid(pmbr->getLowerLeftX(), pmbr->getUpperRightY());
  te::gm::Coord2D ccccf = grid->geoToGrid(pmbr->getUpperRightX(), pmbr->getLowerLeftY());

  int ri = (int) cccci.y;
  int rf = (int) ccccf.y;

  int ci = (int) cccci.x;
  int cf = (int) ccccf.x;

  double areaPixel = resx * resy;
  double areaInside;
  double value;

  for(int r = ri; r <= rf; ++r)
  {
    double lly = grid->getExtent()->getUpperRightY() - resy * (r + 1);
    double ury = lly + resy;
    for(int c = ci; c <= cf; ++c)
    {
      double llx = resx * c + grid->getExtent()->getLowerLeftX();

      double urx = llx + resx;

      std::auto_ptr<te::gm::Polygon> pixelPoly(createPolygon(llx, lly, urx, ury));

      if (pixelPoly->within(poly))
        areaInside = areaPixel;
      else
      {
        std::auto_ptr<te::gm::Geometry> inter(pixelPoly->intersection(poly));

        if(inter->getGeomTypeId() == te::gm::PolygonType)
        {
          te::gm::Polygon* pinter = static_cast<te::gm::Polygon*>(inter.get());

          areaInside = pinter->getArea();
        }
        else
          areaInside = 0.0;
      }

      value = areaInside / areaPixel;

      oraster->setValue(c, r, /*255 * */value);
    }
  }
}

te::rst::Raster* Mask(te::rst::Raster* iraster,
                      const te::gm::Polygon* poly,
                      const std::map<std::string, std::string>& routinfo = std::map<std::string, std::string>(),
                      const std::string& rtype = TE_DEFAULT_RASTER_TYPE)
{
  te::rst::Grid* g = new te::rst::Grid(*iraster->getGrid());

  std::vector<te::rst::BandProperty*> bp;

  bp.push_back(new te::rst::BandProperty(0, te::dt::DOUBLE_TYPE));

  // create output
  // CRIAR UM RASTER EM QUE TODOS OS BLOCOS SAO ZERO
  std::auto_ptr<te::rst::Raster> oraster(te::rst::RasterFactory::make(rtype, g, bp, routinfo));

  for (unsigned r = 0; r < oraster->getNumberOfRows(); ++r)
    for (unsigned c = 0; c < oraster->getNumberOfColumns(); ++c)
      oraster->setValue(c, r, 0.0);

  Mask(iraster, poly, oraster.get());

  return oraster.release();
}

void GribPolygonExample()
{
  try
  {
    std::cout << "This test uses Grib and Geometry." << std::endl << std::endl;

    // load input raster
    //std::map<std::string, std::string> rinfo;

    //rinfo["URI"] = ""TERRALIB_DATA_DIR"/rasters/Z500.grb";

    //te::rst::Raster* graster = te::rst::RasterFactory::open("GRIB", rinfo);

    std::auto_ptr<te::rst::Raster> iraster(te::rst::RasterFactory::open("URI", TERRALIB_DATA_DIR  "/rasters/cbers2b_rgb342_crop.tif"));

    // create rectangle
    const te::gm::Envelope* mbr = iraster->getExtent();

    //te::gm::Coord2D center = mbr->getCenter();

    //ouble w = mbr->getWidth();
    //double h = mbr->getHeight();

    std::auto_ptr<te::gm::Polygon> poly(createPolygon(mbr->getLowerLeftX() + 500.0, mbr->getLowerLeftY() + 500.0, mbr->getUpperRightX() - 500.0, mbr->getUpperRightY() - 500.0));

    // describe output
    std::map<std::string, std::string> routinfo;

    routinfo["URI"] = TERRALIB_DATA_DIR "/rasters/Z500_out.tif";

    std::auto_ptr<te::rst::Raster> newRaster(Mask(iraster.get(), poly.get(), routinfo));

    std::cout << "Done!" << std::endl << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in GribPolygonExample(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in GribPolygonExample()!" << std::endl;
  }

}
