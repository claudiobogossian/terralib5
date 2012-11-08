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
#include "Utils.h"

std::pair<double, double> te::rp::GetSpectralBandInfo(std::string bandName)
{
  static std::map<std::string, std::pair<double, double> > BandInfo;

  BandInfo["CBERS2_CCD_BLUE"] = std::pair<double, double> (0.450000, 0.520000);
  BandInfo["CBERS2_CCD_GREEN"] = std::pair<double, double> (0.520000, 0.590000);
  BandInfo["CBERS2_CCD_RED"] = std::pair<double, double> (0.630000, 0.690000);
// needs more Band Info from other sensors/bands

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
