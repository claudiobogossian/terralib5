#include "RasterExamples.h"

// TerraLib
#include <terralib/raster/BandIterator.h>
#include <terralib/raster/BandIteratorWindow.h>
#include <terralib/raster/Grid.h>
#include <terralib/raster/PositionIterator.h>
#include <terralib/raster/RasterFactory.h>
#include <terralib/raster/RasterIterator.h>
#include <terralib/raster/Utils.h>

// STL
#include <ctime>
#include <iostream>
#include <limits>
#include <map>
#include <set>

// Boost
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

void ExemplifyBandIterator()
{
  std::cout << "Example of Band Iterator" << std::endl;

// define raster info and load
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = ""TERRALIB_EXAMPLES_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";
  te::rst::Raster* inraster = te::rst::RasterFactory::open(rinfo);

  te::rst::Band* band = inraster->getBand(0);

// create iterators for band 0
  te::rst::BandIterator<unsigned char> it = te::rst::BandIterator<unsigned char>::begin(band);

  te::rst::BandIterator<unsigned char> itend = te::rst::BandIterator<unsigned char>::end(band);

  double max = std::numeric_limits<unsigned char>::min();

  double value;

  while (it != itend)
  {
// using iterator
    value = *it;

    if (value > max)
      max = value;

    ++it;
  }

  std::cout << std::endl;
  std::cout << "  using iterator" << std::endl;
  std::cout << "    the maximum value for band 0 is " << max << std::endl;

  max = std::numeric_limits<double>::min();

  for (unsigned r = 0; r < inraster->getNumberOfRows(); r++)
    for (unsigned c = 0; c < inraster->getNumberOfColumns(); c++)
    {
      inraster->getValue(c, r, value, 0);

      if (value > max)
        max = value;
    }

  std::cout << "  not using iterator" << std::endl;
  std::cout << "    the maximum value for band 0 is " << max << std::endl << std::endl;

// clean up
  delete inraster;
}

void ExemplifyBandIteratorMask()
{
  std::cout << "Example of Band Iterator Mask" << std::endl;

// define raster info and load
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = ""TERRALIB_EXAMPLES_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";
  te::rst::Raster* inraster = te::rst::RasterFactory::open(rinfo);

  te::rst::Band* band = inraster->getBand(0);

// define raster mask and load
  std::map<std::string, std::string> minfo;
  minfo["URI"] = ""TERRALIB_EXAMPLES_DATA_DIR"/rasters/cbers2b_rgb342_crop_mask_1bit.tif";
  te::rst::Raster* mraster = te::rst::RasterFactory::open(minfo);

// define output raster and create (only one band will be created)
  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = ""TERRALIB_EXAMPLES_DATA_DIR"/rasters/cbers2b_rgb342_crop_mask_8bits.tif";
  te::rst::Grid* ogrid = new te::rst::Grid(*inraster->getGrid());
  std::vector<te::rst::BandProperty*> obprops;
  for (unsigned int i = 0; i < inraster->getNumberOfBands(); i++)
    obprops.push_back(new te::rst::BandProperty(i, te::dt::UCHAR_TYPE, "masked band"));
  te::rst::Raster* outraster = te::rst::RasterFactory::make(ogrid, obprops, orinfo);
  te::rst::FillRaster(outraster, 0.0);

// create iterators for band
  te::rst::BandIteratorWithMask<unsigned char> it = te::rst::BandIteratorWithMask<unsigned char>::begin(band, mraster);
  te::rst::BandIteratorWithMask<unsigned char> itend = te::rst::BandIteratorWithMask<unsigned char>::end(band, mraster);

  std::vector<std::complex<double> > pixels;
  while (it != itend)
  {
    inraster->getValues(it.getColumn(), it.getRow(), pixels);
    outraster->setValues(it.getColumn(), it.getRow(), pixels);

    ++it;
  }

// clean up
  delete inraster;
  delete mraster;
  delete outraster;
}

void ExemplifyRasterIterator()
{
  std::cout << "Example of Raster Iterator" << std::endl;

// define raster info and load
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = ""TERRALIB_EXAMPLES_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";
  te::rst::Raster* inraster = te::rst::RasterFactory::open(rinfo);

// create iterators for bands 0 and 1
  std::vector<unsigned int> bands;

  bands.push_back(0);
  bands.push_back(1);
  
  te::rst::RasterIterator<unsigned char> it = te::rst::RasterIterator<unsigned char>::begin(inraster, bands);

  te::rst::RasterIterator<unsigned char> itend = te::rst::RasterIterator<unsigned char>::end(inraster, bands);

// this example will compute the mean values for bands 0 and 1
  double mean0 = 0.0;
  double mean1 = 0.0;

  int N = 0;

  while (it != itend)
  {
// using iterator
    mean0 += (*it)[0];
    mean1 += (*it)[1];
    ++N;

    ++it;
  }

  std::cout << std::endl;
  std::cout << "  the mean for band " << bands[0] << " is " << mean0 / N << std::endl;
  std::cout << "  the mean for band " << bands[1] << " is " << mean1 / N << std::endl << std::endl;

// clean up
  delete inraster;
}

void ExemplifyBandIteratorWindow()
{
  std::cout << "Example of Band Iterator Window" << std::endl;

// define raster info and load
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = ""TERRALIB_EXAMPLES_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";
  te::rst::Raster* inraster = te::rst::RasterFactory::open(rinfo);

// define output raster info
  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = ""TERRALIB_EXAMPLES_DATA_DIR"/rasters/cbers2b_band3_crop_median_5x5.tif";

  te::rst::Grid* grid = new te::rst::Grid(*inraster->getGrid());

  std::vector<te::rst::BandProperty*> bands;
  bands.push_back(new te::rst::BandProperty(0, te::dt::UCHAR_TYPE, "median in 5x5 window of band 3"));

// create output raster in disk
  te::rst::Raster* outraster = te::rst::RasterFactory::make(grid, bands, orinfo);

  te::rst::Band* band = inraster->getBand(0);

// create iterator for band 0 with a window of 5x5
  int W = 5;
  int H = 5;
  te::rst::BandIteratorWindow<unsigned char> it = te::rst::BandIteratorWindow<unsigned char>::begin(band, W, H);

  te::rst::BandIteratorWindow<unsigned char> itend = te::rst::BandIteratorWindow<unsigned char>::end(band, W, H);

  while (it != itend)
  {
    unsigned R = it.getRow();
    unsigned C = it.getColumn();

    std::set<unsigned char> values;

// "walk" through window around iterator
    if ((R > (unsigned)(H / 2) && R < inraster->getNumberOfRows() - (H / 2)) &&
        (C > (unsigned)(W / 2) && C < inraster->getNumberOfColumns() - (W / 2)))
    {
      for (int r = -(H / 2); r <= (H / 2); r++)
        for (int c = -(W / 2); c <= (W / 2); c++)
          values.insert(it.getValue(c, r));
    }
    else
      values.insert(it.getValue());

    std::set<unsigned char>::iterator vit = values.begin();

// the middle position of a set will have the median value
    for (std::size_t i = 0; i < values.size()/2; i++)
      ++vit;

    outraster->setValue(C, R, (double) *vit);

    ++it;
  }

  std::cout << std::endl << "  Check the file " << orinfo["URI"] << std::endl << std::endl;

// clean up
  delete inraster;
  delete outraster;
}

void ExemplifyPolygonIterator()
{
  std::cout << "Example of Polygon Iterator" << std::endl;

// define raster info and load
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = ""TERRALIB_EXAMPLES_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";
  te::rst::Raster* inraster = te::rst::RasterFactory::open(rinfo);

  unsigned int nvalues = 0;
  double sum_pixels = 0.0;
  double mean = 0.0;

// create a polygon to use the iterator (corresponds to the upper left quadrant of the image)
  double xc = (inraster->getExtent()->getUpperRightX() + inraster->getExtent()->getLowerLeftX())/2;
  double yc = (inraster->getExtent()->getUpperRightY() + inraster->getExtent()->getLowerLeftY())/2;
  te::gm::LinearRing* lr = new te::gm::LinearRing(5, te::gm::LineStringType);
  lr->setPoint(0, inraster->getExtent()->getLowerLeftX(), inraster->getExtent()->getLowerLeftY()); // lower left
  lr->setPoint(1, inraster->getExtent()->getLowerLeftX(), yc); // upper left
  lr->setPoint(2, xc, yc); // upper rigth
  lr->setPoint(3, xc, inraster->getExtent()->getLowerLeftY()); // lower rigth
  lr->setPoint(4, inraster->getExtent()->getLowerLeftX(), inraster->getExtent()->getLowerLeftY()); // closing

  te::gm::Polygon* polygon = new te::gm::Polygon(0, te::gm::PolygonType);
  polygon->push_back(lr);

// assuming we have a te::gm::Polygon* polygon
  te::rst::PolygonIterator<double> it = te::rst::PolygonIterator<double>::begin(inraster, polygon);
  te::rst::PolygonIterator<double> itend = te::rst::PolygonIterator<double>::end(inraster, polygon);

  while (it != itend)
  {
    sum_pixels += (*it)[0];
    nvalues++;

    ++it;
  }

// compute mean value dividing the sum of pixels by the amount of pixels
  if (nvalues > 0)
    mean = sum_pixels / nvalues;

  std::cout << "The average pixel value (mean) inside the upper left region of band 0 is: " << mean << std::endl << std::endl;

  std::cout << "Done!" << std::endl << std::endl;

// clean up
  delete inraster;
}

void ExemplifyLineIterator()
{
  std::cout << "Example of Line Iterator" << std::endl;

// define raster info and load
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = ""TERRALIB_EXAMPLES_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";
  te::rst::Raster* inraster = te::rst::RasterFactory::open(rinfo);

// create a line to use the iterator, corresponds to a horizontal line in the middle of the raster
  double yc = (inraster->getExtent()->getUpperRightY() + inraster->getExtent()->getLowerLeftY())/2;
  te::gm::Point p1(inraster->getExtent()->getLowerLeftX(), yc, inraster->getSRID());
  te::gm::Point p2(inraster->getExtent()->getUpperRightX(), yc, inraster->getSRID());
  te::gm::Line* line = new te::gm::Line(p1, p2, te::gm::LineStringType, inraster->getSRID());

  std::vector<double> pixels_in_line;

// assuming we have a te::gm::Line* line
  te::rst::LineIterator<double> it = te::rst::LineIterator<double>::begin(inraster, line);
  te::rst::LineIterator<double> itend = te::rst::LineIterator<double>::end(inraster, line);

  while (it != itend)
  {
    pixels_in_line.push_back((*it)[0]);

    ++it;
  }

  std::cout << "Pixels in line: ";
  for (unsigned int i = 0; i < pixels_in_line.size(); i++)
    std::cout << pixels_in_line[i] << " ";
  std::cout << std::endl << std::endl;

  std::cout << "Done!" << std::endl << std::endl;

// clean up
  delete inraster;
}

void ExemplifyPointSetIterator()
{
  std::cout << "Example of Point Set Iterator" << std::endl;

// define raster info and load
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = ""TERRALIB_EXAMPLES_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";
  te::rst::Raster* inraster = te::rst::RasterFactory::open(rinfo);

// create a vector of random points inside the band's envelope
  int srid = inraster->getSRID();
  std::vector<te::gm::Point*> points;
  double x, y;
  boost::random::mt19937 gen(static_cast<boost::uint32_t>(std::time(0)));
  boost::random::uniform_int_distribution<> distX((int)(inraster->getExtent()->getLowerLeftX()), (int)(inraster->getExtent()->getUpperRightX()));
  boost::random::uniform_int_distribution<> distY((int)(inraster->getExtent()->getLowerLeftY()), (int)(inraster->getExtent()->getUpperRightY()));
  for (unsigned int i = 0; i < 50; i++)
  {
    x = distX(gen);
    y = distY(gen);
    points.push_back(new te::gm::Point(x, y, srid));
  }

  std::vector<double> pixels_in_points;

// assuming we have a std::vector<te::gm::Point*> points
  te::rst::PointSetIterator<double> it = te::rst::PointSetIterator<double>::begin(inraster, points);
  te::rst::PointSetIterator<double> itend = te::rst::PointSetIterator<double>::end(inraster, points);

  while (it != itend)
  {
    pixels_in_points.push_back((*it)[0]);

    ++it;
  }

  std::cout << "Pixels in point set: ";
  for (unsigned int i = 0; i < pixels_in_points.size(); i++)
    std::cout << pixels_in_points[i] << " ";
  std::cout << std::endl << std::endl;

  std::cout << "Done!" << std::endl << std::endl;

// clean up
  delete inraster;
}

void ExemplifyIterators()
{
  try
  {
    std::cout << "This test creates iterators over Bands, Windows, and Rasters." << std::endl << std::endl;

    ExemplifyBandIterator();
    ExemplifyBandIteratorMask();
    ExemplifyRasterIterator();
    ExemplifyBandIteratorWindow();
    ExemplifyPolygonIterator();
    ExemplifyLineIterator();
    ExemplifyPointSetIterator();

    std::cout << "Done!" << std::endl << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried in ExemplifyIterators(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried in ExemplifyIterators()!" << std::endl;
  }

}
