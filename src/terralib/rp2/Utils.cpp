/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/rp/Utils.cpp

  \brief Utility functions for the raster processor module.
*/

// TerraLib
#include "../geometry/Point.h"
#include "../raster/RasterIterator.h"
#include "Utils.h"

std::vector<std::string> te::rp::GetBandNames()
{
  static std::vector<std::string> bandNames;

  bandNames.push_back("CBERS2_CCD_1_BLUE");
  bandNames.push_back("CBERS2_CCD_2_GREEN");
  bandNames.push_back("CBERS2_CCD_3_RED");
  bandNames.push_back("CBERS2_CCD_4_NIR");
  bandNames.push_back("CBERS2_CCD_5_PAN");

  bandNames.push_back("LANDSAT5_TM_1_BLUE");
  bandNames.push_back("LANDSAT5_TM_2_GREEN");
  bandNames.push_back("LANDSAT5_TM_3_RED");
  bandNames.push_back("LANDSAT5_TM_4_NIR");
  bandNames.push_back("LANDSAT5_TM_5_SWIR");
  bandNames.push_back("LANDSAT5_TM_6_THERMAL");
  bandNames.push_back("LANDSAT5_TM_7_MIR");

  bandNames.push_back("LANDSAT7_ETM+_1_BLUE");
  bandNames.push_back("LANDSAT7_ETM+_2_GREEN");
  bandNames.push_back("LANDSAT7_ETM+_3_RED");
  bandNames.push_back("LANDSAT7_ETM+_4_NIR");
  bandNames.push_back("LANDSAT7_ETM+_5_SWIR");
  bandNames.push_back("LANDSAT7_ETM+_6_THERMAL");
  bandNames.push_back("LANDSAT7_ETM+_7_MIR");
  bandNames.push_back("LANDSAT7_ETM+_8_PAN");

  bandNames.push_back("WV2_MUL_1_COASTAL");
  bandNames.push_back("WV2_MUL_2_BLUE");
  bandNames.push_back("WV2_MUL_3_GREEN");
  bandNames.push_back("WV2_MUL_4_YELLOW");
  bandNames.push_back("WV2_MUL_5_RED");
  bandNames.push_back("WV2_MUL_6_REDEDGE");
  bandNames.push_back("WV2_MUL_7_NIR1");
  bandNames.push_back("WV2_MUL_8_NIR2");
  bandNames.push_back("WV2_PAN");

  return bandNames;
}

std::pair<double, double> te::rp::GetSpectralBandInfo(std::string bandName)
{
  static std::map<std::string, std::pair<double, double> > BandInfo;

  BandInfo["CBERS2_CCD_1_BLUE"] = std::pair<double, double> (0.45, 0.52);
  BandInfo["CBERS2_CCD_2_GREEN"] = std::pair<double, double> (0.52, 0.59);
  BandInfo["CBERS2_CCD_3_RED"] = std::pair<double, double> (0.63, 0.69);
  BandInfo["CBERS2_CCD_4_NIR"] = std::pair<double, double> (0.77, 0.89);
  BandInfo["CBERS2_CCD_5_PAN"] = std::pair<double, double> (0.51, 0.73);

  BandInfo["LANDSAT5_TM_1_BLUE"] = std::pair<double, double> (0.45, 0.52);
  BandInfo["LANDSAT5_TM_2_GREEN"] = std::pair<double, double> (0.52, 0.60);
  BandInfo["LANDSAT5_TM_3_RED"] = std::pair<double, double> (0.63, 0.69);
  BandInfo["LANDSAT5_TM_4_NIR"] = std::pair<double, double> (0.76, 0.90);
  BandInfo["LANDSAT5_TM_5_SWIR"] = std::pair<double, double> (1.55, 1.75);
  BandInfo["LANDSAT5_TM_6_THERMAL"] = std::pair<double, double> (10.40, 12.50);
  BandInfo["LANDSAT5_TM_7_MIR"] = std::pair<double, double> (2.08, 2.35);

  BandInfo["LANDSAT7_ETM+_1_BLUE"] = std::pair<double, double> (0.45, 0.515);
  BandInfo["LANDSAT7_ETM+_2_GREEN"] = std::pair<double, double> (0.525, 0.605);
  BandInfo["LANDSAT7_ETM+_3_RED"] = std::pair<double, double> (0.63, 0.69);
  BandInfo["LANDSAT7_ETM+_4_NIR"] = std::pair<double, double> (0.75, 0.90);
  BandInfo["LANDSAT7_ETM+_5_SWIR"] = std::pair<double, double> (1.55, 1.75);
  BandInfo["LANDSAT7_ETM+_6_THERMAL"] = std::pair<double, double> (10.40, 12.5);
  BandInfo["LANDSAT7_ETM+_7_MIR"] = std::pair<double, double> (2.09, 2.35);
  BandInfo["LANDSAT7_ETM+_8_PAN"] = std::pair<double, double> (0.52, 0.90);

  BandInfo["WV2_MUL_1_COASTAL"] = std::pair<double, double> (0.4, 0.45);
  BandInfo["WV2_MUL_2_BLUE"] = std::pair<double, double> (0.45, 0.51);
  BandInfo["WV2_MUL_3_GREEN"] = std::pair<double, double> (0.51, 0.58);
  BandInfo["WV2_MUL_4_YELLOW"] = std::pair<double, double> (0.585, 0.625);
  BandInfo["WV2_MUL_5_RED"] = std::pair<double, double> (0.66, 0.69);
  BandInfo["WV2_MUL_6_REDEDGE"] = std::pair<double, double> (0.705, 0.745);
  BandInfo["WV2_MUL_7_NIR1"] = std::pair<double, double> (0.77, 0.895);
  BandInfo["WV2_MUL_8_NIR2"] = std::pair<double, double> (0.86, 0.104);
  BandInfo["WV2_PAN"] = std::pair<double, double> (0.45, 0.8);

// needs more Band Info from other sensors/bands

  if (BandInfo.find(bandName) == BandInfo.end())
    return std::pair<double, double> (0.0, 1.0);

  return BandInfo[bandName];
}

double te::rp::GetSpectralBandMin(std::string bandName)
{
  return GetSpectralBandInfo(bandName).first;
}

double te::rp::GetSpectralBandMax(std::string bandName)
{
  return GetSpectralBandInfo(bandName).second;
}

std::pair<double, double> te::rp::GetDigitalNumberBandInfo(std::string bandName)
{
  static std::map<std::string, std::pair<double, double> > DNBandInfo;

  DNBandInfo["CBERS2_CCD_1_BLUE"] = std::pair<double, double> (0.0, 255.0);
  DNBandInfo["CBERS2_CCD_2_GREEN"] = std::pair<double, double> (0.0, 255.0);
  DNBandInfo["CBERS2_CCD_3_RED"] = std::pair<double, double> (0.0, 255.0);
  DNBandInfo["CBERS2_CCD_4_NIR"] = std::pair<double, double> (0.0, 255.0);
  DNBandInfo["CBERS2_CCD_5_PAN"] = std::pair<double, double> (0.0, 255.0);

  DNBandInfo["LANDSAT5_TM_1_BLUE"] = std::pair<double, double> (0.0, 255.0);
  DNBandInfo["LANDSAT5_TM_2_GREEN"] = std::pair<double, double> (0.0, 255.0);
  DNBandInfo["LANDSAT5_TM_3_RED"] = std::pair<double, double> (0.0, 255.0);
  DNBandInfo["LANDSAT5_TM_4_NIR"] = std::pair<double, double> (0.0, 255.0);
  DNBandInfo["LANDSAT5_TM_5_SWIR"] = std::pair<double, double> (0.0, 255.0);
  DNBandInfo["LANDSAT5_TM_6_THERMAL"] = std::pair<double, double> (0.0, 255.0);
  DNBandInfo["LANDSAT5_TM_7_MIR"] = std::pair<double, double> (0.0, 255.0);

  DNBandInfo["LANDSAT7_ETM+_1_BLUE"] = std::pair<double, double> (0.0, 255.0);
  DNBandInfo["LANDSAT7_ETM+_2_GREEN"] = std::pair<double, double> (0.0, 255.0);
  DNBandInfo["LANDSAT7_ETM+_3_RED"] = std::pair<double, double> (0.0, 255.0);
  DNBandInfo["LANDSAT7_ETM+_4_NIR"] = std::pair<double, double> (0.0, 255.0);
  DNBandInfo["LANDSAT7_ETM+_5_SWIR"] = std::pair<double, double> (0.0, 255.0);
  DNBandInfo["LANDSAT7_ETM+_6_THERMAL"] = std::pair<double, double> (0.0, 255.0);
  DNBandInfo["LANDSAT7_ETM+_7_MIR"] = std::pair<double, double> (0.0, 255.0);
  DNBandInfo["LANDSAT7_ETM+_8_PAN"] = std::pair<double, double> (0.0, 255.0);

  DNBandInfo["WV2_MUL_1_COASTAL"] = std::pair<double, double> (0.0, 2048.0);
  DNBandInfo["WV2_MUL_2_BLUE"] = std::pair<double, double> (0.0, 2048.0);
  DNBandInfo["WV2_MUL_3_GREEN"] = std::pair<double, double> (0.0, 2048.0);
  DNBandInfo["WV2_MUL_4_YELLOW"] = std::pair<double, double> (0.0, 2048.0);
  DNBandInfo["WV2_MUL_5_RED"] = std::pair<double, double> (0.0, 2048.0);
  DNBandInfo["WV2_MUL_6_REDEDGE"] = std::pair<double, double> (0.0, 2048.0);
  DNBandInfo["WV2_MUL_7_NIR1"] = std::pair<double, double> (0.0, 2048.0);
  DNBandInfo["WV2_MUL_7_NIR2"] = std::pair<double, double> (0.0, 2048.0);
  DNBandInfo["WV2_PAN"] = std::pair<double, double> (0.0, 2048.0);

// needs more Band Info from other sensors/bands

  if (DNBandInfo.find(bandName) == DNBandInfo.end())
    return std::pair<double, double> (0.0, 255.0);

  return DNBandInfo[bandName];
}

double te::rp::GetDigitalNumberBandMax(std::string bandName)
{
  return GetDigitalNumberBandInfo(bandName).second;
}

bool te::rp::NormalizeRaster(te::rst::Raster& inraster, double nmin, double nmax)
{
  if (nmin > nmax)
    return false;

// computing old maximuns and minimums for each band
  std::vector<double> omins;
  std::vector<double> omaxs;
  std::vector<unsigned int> bands;

  for (unsigned int b = 0; b < inraster.getNumberOfBands(); b++)
  {
    omins.push_back(inraster.getBand(b)->getMinValue().real());
    omaxs.push_back(inraster.getBand(b)->getMaxValue().real());

    bands.push_back(b);
  }

// calculating amplitudes to avoid multiple subtractions
  double value;
  const double namplitude = nmax - nmin;
  std::vector<double> oamplitude;
  for (unsigned int b = 0; b < inraster.getNumberOfBands(); b++)
    oamplitude.push_back(omaxs[b] - omins[b]);

// iterating over raster to normalize pixel values
  te::rst::RasterIterator<double> it = te::rst::RasterIterator<double>::begin(&inraster, bands);
  te::rst::RasterIterator<double> itend = te::rst::RasterIterator<double>::end(&inraster, bands);

  while (it != itend)
  {
    for (unsigned int b = 0; b < inraster.getNumberOfBands(); b++)
    {
      value = ((*it)[b] - omins[b]) * namplitude / oamplitude[b] + nmin;

      inraster.setValue(it.getCol(), it.getRow(), value, b);
    }

    ++it;
  }

  return true;
}

std::vector<te::gm::Point*> te::rp::GetRandomPointsInRaster(const te::rst::Raster& inputRaster, unsigned int numberOfPoints)
{
  std::vector<te::gm::Point*> randomPoints;
  double randX;
  double randY;
  for (unsigned int p = 0; p < numberOfPoints; p++)
  {
    inputRaster.getGrid()->gridToGeo(rand() % inputRaster.getNumberOfColumns(), rand() % inputRaster.getNumberOfRows(), randX, randY);
    randomPoints.push_back(new te::gm::Point(randX, randY, inputRaster.getSRID()));
  }

  return randomPoints;
}

double te::rp::GetEuclideanDistance(std::vector<double> v1, std::vector<double> v2)
{
  assert(v1.size() == v2.size());

  double distance = 0.0;
  double v1minusv2;
  for (unsigned int i = 0; i < v1.size(); i++)
  {
    v1minusv2 = v1[i] - v2[i];
    distance += v1minusv2 * v1minusv2;
  }
  if (distance < 0)
    distance = 0.0;

  return sqrt(distance);
}
