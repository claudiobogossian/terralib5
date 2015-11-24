/*!
\file terralib/mnt/core/TINCalculateGrid.cpp

\brief This file contains a class to calculate a grid from TIN.

*/

#include "TINCalculateGrid.h"
#include "Utils.h"

#include "../../raster.h"
#include "../../raster/BandProperty.h"
#include "../../raster/Grid.h"
#include "../../raster/RasterFactory.h"

#include <limits>
#include <math.h>

te::mnt::TINCalculateGrid::~TINCalculateGrid()
{
  delete m_rst;
}

/*!
\brief Method that generates a regular Grid from a given TIN
\param grid is a pointer to the grid that will be created
\param gridtype is the type of interpolator fitted to each triangle
\return TRUE if the regular grid is created or FALSE otherwise
*/

bool te::mnt::TINCalculateGrid::run()
{
  LoadTin(m_inDsrc, m_inDsetName);

//Create greater box 1/2 resolution
  double rx1 = m_env.getLowerLeftX() - m_resx/2.; 
  double ry2 = m_env.getUpperRightY() + m_resy/2.;

  unsigned int outputWidth = (unsigned int)ceil(m_env.getWidth() / m_resx);
  unsigned int outputHeight = (unsigned int)ceil(m_env.getHeight() / m_resy);

  te::gm::Coord2D ulc(rx1, ry2);

  te::rst::Grid* grid = new te::rst::Grid(outputWidth, outputHeight, m_resx, m_resy, &ulc, m_srid);

  std::vector<te::rst::BandProperty*> bands;

  bands.push_back(new te::rst::BandProperty(0, te::dt::DOUBLE_TYPE, "DTM GRID"));
  bands[0]->m_nblocksx = 1;
  bands[0]->m_nblocksy = (int)outputHeight;
  bands[0]->m_blkw = (int)outputWidth;
  bands[0]->m_blkh = 1;
  bands[0]->m_colorInterp = te::rst::GrayIdxCInt;
  bands[0]->m_noDataValue = m_nodatavalue;

  // create raster
  std::auto_ptr<te::rst::Raster> rst(te::rst::RasterFactory::make("GDAL", grid, bands, m_dsinfo));

  m_rst = rst.get();

  double dummyvalue = m_rst->getBand(0)->getProperty()->m_noDataValue;

  int32_t fbnode = 0;

  if (m_gridtype == Quintico)    //Quintic Surface Adjust without breaklines
  {
    fbnode = m_fbnode;
    m_fbnode = 0;
  }

  if ((m_gridtype == QuinticoBrkLine) || (m_gridtype == Quintico))    //Quintic Surface Adjust
    if (!m_nsderiv.size())
      // Calculate derivatives
      if (!NodeDerivatives())
        return false;

  int32_t neighsid[3];
  int32_t  nodesid[3];
  te::gm::PointZ p3da[3];
  int32_t flin, llin, fcol, lcol;
  short j;
  double	coef[27];

  //  To each triangle
  for (unsigned int i = 0; i < m_triang.size(); i++)
  { // Find Triangle Box
    if (!NodesId((int32_t)i, nodesid)) 
      continue;
    if (!DefineInterLinesColumns(nodesid, flin, llin, fcol, lcol))
      continue;
    for (j = 0; j < 3; j++)
    {
      p3da[j].setX(m_node[(unsigned int)nodesid[j]].getNPoint().getX());
      p3da[j].setY(m_node[(unsigned int)nodesid[j]].getNPoint().getY());
      p3da[j].setZ(m_node[(unsigned int)nodesid[j]].getZ());
    }

    if ((p3da[0].getZ() == dummyvalue) ||
      (p3da[1].getZ() == dummyvalue) ||
      (p3da[2].getZ() == dummyvalue))
    {
       FillGridValue((int32_t)i, flin, llin, fcol, lcol, dummyvalue);
    }
    else if ((p3da[0].getZ() == p3da[1].getZ()) &&
      (p3da[0].getZ() == p3da[2].getZ()) && (m_gridtype == Linear))
      FillGridValue((int32_t)i, flin, llin, fcol, lcol, p3da[0].getZ());
    else if ((m_gridtype == Quintico) || (m_gridtype == QuinticoBrkLine))
    {
      if (!NeighborsId((int32_t)i, neighsid))
        continue;
      for (j = 0; j < 3; j++)
        if (neighsid[j] == -1)
          break;
      if (j == 3)
      {
        if (DefineAkimaCoeficients((int32_t)i, nodesid, p3da, coef))
          FillGridQuintic((int32_t)i, flin, llin, fcol, lcol, coef);
      }
      else
        FillGridValue((int32_t)i, flin, llin, fcol, lcol, m_nodatavalue);
    }
    else
      FillGridLinear((int32_t)i, p3da, flin, llin, fcol, lcol);
  }

  if (m_gridtype == Quintico)    //Quintic Surface Adjust without breaklines
    m_fbnode = fbnode;

  rst.release();

  return true;
}


void te::mnt::TINCalculateGrid::setInput(te::da::DataSourcePtr inDsrc,
  std::string inDsetName,
  std::auto_ptr<te::da::DataSetType> inDsetType)
{
  m_inDsrc = inDsrc;
  m_inDsetName = inDsetName;
  m_inDsetType = inDsetType;
}

void te::mnt::TINCalculateGrid::setOutput(std::map<std::string, std::string> &dsinfo)
{
  m_dsinfo = dsinfo;
}

void te::mnt::TINCalculateGrid::setParams(double resx, double resy, Interpolator gt)
{
  m_resx = resx;
  m_resy = resy;
  m_gridtype = gt;
}



/*!
\brief Method that fills the grid locations, inside a triangle, with a zvalue linearly evaluated
\param grid is a pointer to a grid object
\param triid is the triangle identification number
\param flin and llin are the first and the last lines (rows) of the grid
\param fcol and lcol are the first and the last columns of the grid
\param zvalue is the z value to be stored in the grid inside the triangle region
\return TRUE always
*/

bool te::mnt::TINCalculateGrid::FillGridLinear(int32_t triid, te::gm::PointZ *p3da, int32_t flin, int32_t llin, int32_t fcol, int32_t lcol)
{
  te::gm::PointZ pg;
  te::gm::Coord2D cg;
  double x1_x0, x2_x0, y1_y0, y2_y0, z1_z0, z2_z0,
    detx, dety, detz;
  double zvalue;
  int32_t nlin, ncol;
  double dummyvalue = m_rst->getBand(0)->getProperty()->m_noDataValue;

  x1_x0 = p3da[1].getX() - p3da[0].getX();
  x2_x0 = p3da[2].getX() - p3da[0].getX();
  y1_y0 = p3da[1].getY() - p3da[0].getY();
  y2_y0 = p3da[2].getY() - p3da[0].getY();
  z1_z0 = (double)(p3da[1].getZ() - p3da[0].getZ());
  z2_z0 = (double)(p3da[2].getZ() - p3da[0].getZ());
  for (nlin = flin; nlin <= llin; nlin++){
    for (ncol = fcol; ncol <= lcol; ncol++){
      cg = m_rst->getGrid()->gridToGeo(ncol, nlin);
      pg.setX(cg.getX());
      pg.setY(cg.getY());
      if (!(ContainsPoint(triid, pg)))
        continue;
      detx = ((y1_y0 * z2_z0) - (y2_y0 * z1_z0)) *
        (pg.getX() - p3da[0].getX());
      dety = ((z1_z0 * x2_x0) - (z2_z0 * x1_x0)) *
        (pg.getY() - p3da[0].getY());
      detz = (x1_x0 * y2_y0) - (x2_x0 * y1_y0);

      zvalue = ((detx + dety - detz * p3da[0].getZ()) / -detz);

      m_rst->setValue((unsigned int)ncol, (unsigned int)nlin, zvalue);
    }
  }

  return true;
}


/*!
\brief Method that fills the grid locations, inside a triangle, with a zvalue evaluated by a quintic polynomium
\param grid is a pointer to a grid object
\param triid is the triangle identification number
\param p3da is a pointer to a Point3d object, vector with 3D samples (not used in this method)
\param flin and llin are the first and the last lines (rows) of the grid
\param fcol and lcol are the first and the last columns of the grid
\param zvalue is the z value to be stored in the grid inside the triangle region
\return TRUE always
*/

bool te::mnt::TINCalculateGrid::FillGridQuintic(int32_t triid, int32_t flin, int32_t llin, int32_t fcol, int32_t lcol, double *coef)
{
  te::gm::PointZ pg;
  te::gm::Coord2D cg;
  double u, v, ap, bp, cp, dp, x0, y0,
    p00, p01, p02, p03, p04, p05,
    p10, p11, p12, p13, p14,
    p20, p21, p22, p23,
    p30, p31, p32,
    p40, p41, p50,
    p0, p1, p2, p3, p4;
  double zvalue;
  int32_t nlin, ncol;

  // Polynomial coefficients
  p00 = coef[0]; p01 = coef[1]; p02 = coef[2]; p03 = coef[3]; p04 = coef[4]; p05 = coef[5];
  p10 = coef[6]; p11 = coef[7]; p12 = coef[8]; p13 = coef[9]; p14 = coef[10];
  p20 = coef[11]; p21 = coef[12]; p22 = coef[13]; p23 = coef[14];
  p30 = coef[15]; p31 = coef[16]; p32 = coef[17];
  p40 = coef[18]; p41 = coef[19];
  p50 = coef[20];

  // Coefficients of conversion from XY to UV coordinates
  ap = coef[21]; bp = coef[22]; cp = coef[23]; dp = coef[24];
  x0 = coef[25]; y0 = coef[26];

  for (nlin = flin; nlin <= llin; nlin++)
  {
    for (ncol = fcol; ncol <= lcol; ncol++)
    {
      cg = m_rst->getGrid()->gridToGeo(ncol, nlin);
      pg.setX(cg.getX());
      pg.setY(cg.getY());
      if (!(ContainsPoint(triid, pg)))
        continue;
      // Converts point from XY to UV
      u = ap*(pg.getX() - x0) +
        bp*(pg.getY() - y0);
      v = cp*(pg.getX() - x0) +
        dp*(pg.getY() - y0);

      // Evaluates the polynomial
      p0 = p00 + v*(p01 + v*(p02 + v*(p03 + v*(p04 + v*p05))));
      p1 = p10 + v*(p11 + v*(p12 + v*(p13 + v*p14)));
      p2 = p20 + v*(p21 + v*(p22 + v*p23));
      p3 = p30 + v*(p31 + v*p32);
      p4 = p40 + v*p41;

      zvalue = p0 + u*(p1 + u*(p2 + u*(p3 + u*(p4 + u*p50))));
      m_rst->setValue((unsigned int)ncol, (unsigned int)nlin, zvalue);
    }
  }

  return true;
}


