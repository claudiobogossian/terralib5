/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/raster/Reprojection.cpp

  \brief It contains the algorithm to reproject raster data.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../geometry/Coord2D.h"
#include "../geometry/Envelope.h"
#include "../srs/Converter.h"
#include "Band.h"
#include "BandProperty.h"
#include "Exception.h"
#include "Grid.h"
#include "Interpolator.h"
#include "Raster.h"
#include "RasterFactory.h"
#include "Reprojection.h"

// STL
#include <algorithm>  // for max and min
#include <cstdlib>    // for abs

bool IsPointOnLine(te::gm::Coord2D& p, te::gm::Coord2D& q, te::gm::Coord2D& t, double tol);

bool InterpolateIn(te::rst::Raster const * const rin, te::rst::Raster* rout, te::gm::Envelope* box, te::srs::Converter* conv, int m = te::rst::NearestNeighbor);

te::rst::Raster* te::rst::Reproject(te::rst::Raster const * const rin, int srid, const std::map<std::string, std::string>& routinfo, int m)
{
  return te::rst::Reproject(rin, srid, 1, 1, -1, -1, 0, 0, routinfo, m);
}

te::rst::Raster* te::rst::Reproject(te::rst::Raster const * const rin, int srid, double llx, double lly, double urx, double ury, const std::map<std::string, std::string>& routinfo, int m)
{
  return te::rst::Reproject(rin, srid, llx, lly, urx, ury, 0, 0, routinfo, m);
}

te::rst::Raster* te::rst::Reproject(te::rst::Raster const * const rin, int srid, double llx, double lly, double urx, double ury, double resx, double resy, const std::map<std::string, std::string>& routinfo, int m)
{
  if (srid == rin->getSRID())
    return 0;

  te::srs::Converter* converter = new te::srs::Converter();
  try
  {
    converter->setSourceSRID(rin->getSRID());
    converter->setTargetSRID(srid);
  }
  catch(...)
  {
    throw te::rst::Exception("Input/Output SRID not recognized.");
  }

  unsigned int ncols = rin->getNumberOfColumns();
  unsigned int nrows = rin->getNumberOfRows();

  te::gm::Envelope* roi = new te::gm::Envelope(llx, lly, urx, ury);
  if (!roi->isValid())
  {
    delete roi;
    roi = 0;
  }
  else
  {
    ncols = static_cast<unsigned int>((urx-llx)/rin->getResolutionX())+1;
    nrows = static_cast<unsigned int>((ury-lly)/rin->getResolutionY())+1;
  }

  te::gm::Envelope* env = rin->getExtent(srid, roi);
  delete roi;

  if (resx == 0 || resy == 0)
  {
// maintain the same number of pixels
    resx = env->getWidth() / ncols;
    resy = env->getHeight() / nrows;
  }
  else
  {
    ncols = static_cast<unsigned int>(env->getWidth() / resx) + 1;
    nrows = static_cast<unsigned int>(env->getHeight() / resy) + 1;
  }

  te::rst::Grid* g = new te::rst::Grid(ncols, nrows, resx, resy, env, srid);

// copy the band definitions
  std::vector<te::rst::BandProperty*> bands;
  for (unsigned int b=0; b<rin->getNumberOfBands(); ++b)
  {
    te::rst::BandProperty* bb = new te::rst::BandProperty(*(rin->getBand(b)->getProperty()));
    bands.push_back(bb);
  }

// create output raster
  te::rst::Raster* rout = te::rst::RasterFactory::make(g, bands, routinfo);

  bool res = InterpolateIn(rin, rout, env, converter, m);

  if (!res)
  {
    delete rout;
    return 0;
  }

  return rout;
}

bool InterpolateIn(te::rst::Raster const * const rin, te::rst::Raster* rout, te::gm::Envelope* box, te::srs::Converter* conv, int m)
{
  te::gm::Coord2D poll = box->getLowerLeft();
  te::gm::Coord2D pour = box->getUpperRight();

// Bring output coordinates to output line/column domain
  te::gm::Coord2D pxoll = rout->getGrid()->geoToGrid(poll.x, poll.y);
  te::gm::Coord2D pxour = rout->getGrid()->geoToGrid(pour.x, pour.y);

// Round output coordinates to nearest exact pixel
  int x1 = (int) (pxoll.x-0.5);
  int y1 = (int) (pxoll.y+0.5);
  pxoll = te::gm::Coord2D(x1, y1);

  int x2 = (int)(pxour.x+0.5);
  int y2 = (int)(pxour.y-0.5);
  pxour = te::gm::Coord2D(x2, y2);

  poll = rout->getGrid()->gridToGeo((int)pxoll.x, (int)pxoll.y);

  te::gm::Coord2D poul = te::gm::Coord2D(poll.x, pour.y);
  te::gm::Coord2D polr = te::gm::Coord2D(pour.x, poll.y);

// Bring coordinates of box four corners to input raster projection

  te::gm::Coord2D pill(poll);
  te::gm::Coord2D piur(pour);
  te::gm::Coord2D piul(poul);
  te::gm::Coord2D pilr(polr);

  conv->invert(pill.x,pill.y);
  conv->invert(piur.x,piur.y);
  conv->invert(piul.x,piul.y);
  conv->invert(pilr.x,pilr.y);

// Check if linear interpolation may be performed on input raster
// Evaluate point at middle of the edges in output domain and check if their
// corresponding points belong to the edges in input domain. If they belong,
// a linear interpolation may be performed, else divide output image
// in four quadrants and try interpolating again.

  te::gm::Coord2D pou((pour.x-poul.x)/2.+poul.x, poul.y), // upper edge
                  pob((polr.x-poll.x)/2.+poll.x, poll.y), // bottom edge
                  pol(poll.x, (poul.y-poll.y)/2.+poll.y), // left edge
                  por(polr.x, (pour.y-polr.y)/2.+polr.y); // right edge

// Evaluate corresponding points in input raster domain
  te::gm::Coord2D piu(pou); conv->invert(piu.x, piu.y);
  te::gm::Coord2D pib(pob); conv->invert(pib.x, pib.y);
  te::gm::Coord2D pil(pol); conv->invert(pil.x, pil.y);
  te::gm::Coord2D pir(por); conv->invert(pir.x, pir.y);

// Check if middle points belong to the edges
// If one of them does not belong to corresponding edge, divide output in four quadrants
  double tol = std::max(rin->getResolutionX(), rin->getResolutionY());

  if (!IsPointOnLine(piul, piur, piu, tol) ||
      !IsPointOnLine(pilr, piur, pir, tol) ||
      !IsPointOnLine(pill, pilr, pib, tol) ||
      !IsPointOnLine(pill, piul, pil, tol))
  {
// center point
    te::gm::Coord2D pom((por.x-pol.x)/2.+pol.x, (pou.y-pob.y)/2.+pob.y);

    te::gm::Envelope quadrantul(pol.x, pol.y, pou.x, pou.y);
    if(!InterpolateIn(rin,rout,&quadrantul,conv))
      return false;

    te::gm::Envelope quadrantur(pom.x, pom.y, pour.x, pour.y);
    if(!InterpolateIn(rin, rout, &quadrantur, conv))
      return false;

    te::gm::Envelope quadrantll(poll.x, poll.y, pom.x, pom.y);
    if(!InterpolateIn(rin, rout, &quadrantll, conv))
      return false;

    te::gm::Envelope quadrantlr(pob.x, pob.y, por.x, por.y);
    if(!InterpolateIn(rin, rout, &quadrantlr, conv))
      return false;

    return true;
  }

// Start linear interpolation on input image.
  te::gm::Coord2D pxill = rin->getGrid()->geoToGrid(pill.x, pill.y);
  te::gm::Coord2D pxiul = rin->getGrid()->geoToGrid(piul.x, piul.y);
  te::gm::Coord2D pxilr = rin->getGrid()->geoToGrid(pilr.x, pilr.y);
  te::gm::Coord2D pxiur = rin->getGrid()->geoToGrid(piur.x, piur.y);

// Evaluate the increments in x and y on both sides of input image
  x1 = (int)pxoll.x-1;
  y1 = (int)pxour.y-1;

  x2 = (int)pxour.x+1;
  y2 = (int)pxoll.y+1;

  double dxl((pxill.x-pxiul.x)/(y2-y1)); // x increment at the beginning of line
  double dyl((pxill.y-pxiul.y)/(y2-y1)); // y increment at the beginning of line
  double dxr((pxilr.x-pxiur.x)/(y2-y1)); // x increment at the end of line
  double dyr((pxilr.y-pxiur.y)/(y2-y1)); // Y increment at the end of line

  // Set initial values for x and y at beginning point on input image
  double xl = pxiul.x-1;                 // x at the beginning of the line
  double yl = pxiul.y-1;                 // y at the beginning of the line

  // Set initial values for x and y at end point on input image
  double xr = pxiur.x+1;                 // x at the end of the line
  double yr = pxiur.y+1;                 // y at the end of the line

  // Evaluate increments for the first line
  double dx = (xr-xl)/(x2-x1);           // inner loop x increment
  double dy = (yr-yl)/(x2-x1);           // inner loop y increment

  double x = xl; // round to left pixel
  double y = yl; // round to left pixel

  // Create the interpolator
  te::rst::Interpolator* interpolator = new te::rst::Interpolator(rin, m);

  int i, j;
  std::complex<double> value;
  for (j = y1; j <= y2; ++j)
  {
    for (i = x1; i <= x2; ++i)
    {
      if (rin->getGrid()->isPointInGrid((unsigned int)(x+0.5), (unsigned int)(y+0.5)) &&
          rout->getGrid()->isPointInGrid(i, j))
      {
        for (std::size_t b = 0; b < rin->getNumberOfBands(); ++b)
        {
          //rin->getValue((int)(x+0.5), (int)(y+0.5), value, b);
          interpolator->getValue(x, y, value, b);
          rout->setValue(i, j, value, b);
        }
      }
      x += dx;
      y += dy;
    }
    xl += dxl;
    yl += dyl;

    xr += dxr;
    yr += dyr;

    x = xl;
    y = yl;

    dx = (xr-xl)/(x2-x1);
    dy = (yr-yl)/(x2-x1);
  }
  return true;
}


bool IsPointOnLine(te::gm::Coord2D& p, te::gm::Coord2D& q, te::gm::Coord2D& t, double tol)
{
  int px = (int)(p.x/tol);
  int py = (int)(p.y/tol);

  int qx = (int)(q.x/tol);
  int qy = (int)(q.y/tol);

  int tx = (int)(t.x/tol);
  int ty = (int)(t.y/tol);

  int dx = abs(px-qx);
  int dy = abs(py-qy);

  if (dx <= 2 && dy <= 2)
    return true;

  int q1 = (qy-py)*(tx-px);
  int q2 = (ty-py)*(qx-px);
  int q3 = qx-px;
  int q4 = qy-py;

  if (q1 == 0 && q2 == 0 && q3 == 0 && q4 == 0)
    return true;

  if (abs(q1 - q2) > (std::max(abs(q3), abs(q4))))
    return false;

  return true;
}
