#include "RasterExamples.h"

// TerraLib
#include <terralib/raster/BandIterator.h>
#include <terralib/raster/BandIteratorWindow.h>
#include <terralib/raster/Grid.h>
#include <terralib/raster/RasterFactory.h>
#include <terralib/raster/RasterIterator.h>

// STL
#include <iostream>
#include <limits>
#include <map>
#include <set>

void ExemplifyBandIterator()
{
  std::cout << "Example of BandIterator" << std::endl;

// define raster info
  std::map<std::string, std::string> rinfo;

  rinfo["URI"] = ""TE_DATA_EXAMPLE_LOCALE"/data/rasters/cbers2b_rgb342_crop.tif";

// open raster from disk
  te::rst::Raster* inraster = te::rst::RasterFactory::open(rinfo);

  te::rst::Band* band = inraster->getBand(0);

// create iterators for band 0
  te::rst::BandIterator<unsigned char> it = te::rst::BandIterator<unsigned char>::begin(band);

  te::rst::BandIterator<unsigned char> itend = te::rst::BandIterator<unsigned char>::end(band);

  double max = std::numeric_limits<double>::min();

  double value;

  while (it != itend)
  {
// using iterator
    value = it.getValue();

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

  delete inraster;
}

void ExemplifyRasterIterator()
{
  std::cout << "Example of RasterIterator" << std::endl;

// define raster info
  std::map<std::string, std::string> rinfo;

  rinfo["URI"] = ""TE_DATA_EXAMPLE_LOCALE"/data/rasters/cbers2b_rgb342_crop.tif";

// open raster from disk
  te::rst::Raster* inraster = te::rst::RasterFactory::open(rinfo);

// create iterators for bands 2 and 1
  std::vector<std::size_t> bands;

  bands.push_back(2);

  bands.push_back(1);

  te::rst::RasterIterator<unsigned char> it = te::rst::RasterIterator<unsigned char>::begin(inraster, bands);

  te::rst::RasterIterator<unsigned char> itend = te::rst::RasterIterator<unsigned char>::end(inraster, bands);

// this example will compute the mean values for bands 2 and 1
  double mean0 = 0.0;

  double mean1 = 0.0;

  int N = 0;

  while (it != itend)
  {
// using iterator
    mean0 += it.getValue(0);

    mean1 += it.getValue(1);

    ++it;

    ++N;
  }

  std::cout << std::endl;
  std::cout << "  the mean for band " << bands[0] << " is " << mean0 / N << std::endl;
  std::cout << "  the mean for band " << bands[1] << " is " << mean1 / N << std::endl << std::endl;

  delete inraster;
}

void ExemplifyBandIteratorWindow()
{
  std::cout << "Example of ExemplifyBandIteratorWindow" << std::endl;

// define raster info
  std::map<std::string, std::string> rinfo;

  rinfo["URI"] = ""TE_DATA_EXAMPLE_LOCALE"/data/rasters/cbers2b_rgb342_crop.tif";

// open raster from disk
  te::rst::Raster* inraster = te::rst::RasterFactory::open(rinfo);

// define output raster info
  std::map<std::string, std::string> orinfo;

  orinfo["URI"] = ""TE_DATA_EXAMPLE_LOCALE"/data/rasters/cbers2b_band3_crop_median_5x5.tif";

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
    unsigned C = it.getCol();

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

  delete inraster;
  delete outraster;
}

void ExemplifyRasterIteratorWindow()
{
  std::cout << "Example of ExemplifyRasterIteratorWindow" << std::endl;

// define raster info
  std::map<std::string, std::string> rinfo;

  rinfo["URI"] = ""TE_DATA_EXAMPLE_LOCALE"/data/rasters/cbers2b_rgb342_crop.tif";

// open raster from disk
  te::rst::Raster* inraster = te::rst::RasterFactory::open(rinfo);

  delete inraster;
}

void ExemplifyIterators()
{
  try
  {
    std::cout << "This test creates iterators over Bands, Windows, and Rasters." << std::endl << std::endl;

    ExemplifyBandIterator();

    ExemplifyRasterIterator();

    ExemplifyBandIteratorWindow();

    ExemplifyRasterIteratorWindow();

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
