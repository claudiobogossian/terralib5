#include "RPExamples.h"

// TerraLib
#include <terralib/geometry.h>
#include <terralib/raster.h>
#include <terralib/rp/RasterAttributes.h>
#include <terralib/rp/Texture.h>

#include <boost/numeric/ublas/matrix.hpp>

void Texture()
{
  try
  {
    std::cout << "Texture example using cooccurrence matrix." << std::endl << std::endl;

// first open the input image
    std::map<std::string, std::string> rinfo;
    rinfo["URI"] = TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";
    te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

    {
// use raster attributes to compute GLCM matrix, in northeast direction
      te::rp::RasterAttributes rattributes;
      boost::numeric::ublas::matrix<double> glcm_b1 = rattributes.getGLCM(*rin, 1, 1, -1);
      te::rp::Texture metrics = rattributes.getGLCMMetrics(glcm_b1);

// display texture metrics    
      std::cout << "GLCM metrics of full image, band 1" << std::endl;
      std::cout << "  contrast: " << metrics.m_contrast << std::endl;
      std::cout << "  dissimilarity: " << metrics.m_dissimilarity << std::endl;
      std::cout << "  energy: " << metrics.m_energy << std::endl;
      std::cout << "  entropy: " << metrics.m_entropy << std::endl;
      std::cout << "  homogeneity: " << metrics.m_homogeneity << std::endl;
    }

// create raster crop using a polygon
    double xc = (rin->getExtent()->getUpperRightX() + rin->getExtent()->getLowerLeftX()) / 2;
    double yc = (rin->getExtent()->getUpperRightY() + rin->getExtent()->getLowerLeftY()) / 2;
    te::gm::LinearRing* lr = new te::gm::LinearRing(6, te::gm::LineStringType);
    lr->setPoint(0, rin->getExtent()->getLowerLeftX(), rin->getExtent()->getLowerLeftY()); // lower left
    lr->setPoint(1, (rin->getExtent()->getLowerLeftX() + xc) / 2, (rin->getExtent()->getLowerLeftY() + yc) / 2);
    lr->setPoint(2, rin->getExtent()->getLowerLeftX(), yc); // upper left
    lr->setPoint(3, xc, yc); // upper rigth
    lr->setPoint(4, xc, rin->getExtent()->getLowerLeftY()); // lower rigth
    lr->setPoint(5, rin->getExtent()->getLowerLeftX(), rin->getExtent()->getLowerLeftY()); // closing

    te::gm::Polygon* polygon = new te::gm::Polygon(0, te::gm::PolygonType);
    polygon->push_back(lr);
  
    std::map<std::string, std::string> rcropinfo;
    rcropinfo["URI"] = TERRALIB_DATA_DIR"/rasters/polygon_crop_cbers2b_rgb342_crop.tif";
    te::rst::RasterPtr rcrop = te::rst::CropRaster(*rin, *polygon, rcropinfo);

    {
// use raster attributes to compute GLCM matrix from band 2, in southeast direction
      te::rp::RasterAttributes rattributes;
      boost::numeric::ublas::matrix<double> glcm_b2 = rattributes.getGLCM(*rcrop.get(), 2, 1, 1);
      te::rp::Texture metrics = rattributes.getGLCMMetrics(glcm_b2);

// display texture metrics    
      std::cout << "GLCM metrics of image crop image, band 2" << std::endl;
      std::cout << "  contrast: " << metrics.m_contrast << std::endl;
      std::cout << "  dissimilarity: " << metrics.m_dissimilarity << std::endl;
      std::cout << "  energy: " << metrics.m_energy << std::endl;
      std::cout << "  entropy: " << metrics.m_entropy << std::endl;
      std::cout << "  homogeneity: " << metrics.m_homogeneity << std::endl;
    }

// clean up
    delete rin;

    std::cout << "Done!" << std::endl << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in Texture(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in Texture()!" << std::endl;
  }
}

