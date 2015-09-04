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


/*!
\brief Method that generates a regular Grid from a given TIN
\param grid is a pointer to the grid that will be created
\param gridtype is the type of interpolator fitted to each triangle
\return TRUE if the regular grid is created or FALSE otherwise
*/

bool te::mnt::TINCalculateGrid::run()
{
  LoadTin(m_inDsrc, m_inDsetName);

  double rx1 = m_env.getLowerLeftX();
  double ry2 = m_env.getUpperRightY();

  int outputWidth = int(m_env.getWidth() / m_resx);
  int outputHeight = int(m_env.getHeight() / m_resy);

  te::gm::Coord2D ulc(m_env.getLowerLeftX(), m_env.getUpperRightY());

  te::rst::Grid* grid = new te::rst::Grid(outputWidth, outputHeight, m_resx, m_resy, &ulc, m_srid);

  std::vector<te::rst::BandProperty*> bands;

  bands.push_back(new te::rst::BandProperty(0, te::dt::DOUBLE_TYPE, "DTM GRID"));
  bands[0]->m_nblocksx = 1;
  bands[0]->m_nblocksy = outputHeight;
  bands[0]->m_blkw = outputWidth;
  bands[0]->m_blkh = 1;
  bands[0]->m_colorInterp = te::rst::GrayIdxCInt;

  m_rst = te::rst::RasterFactory::make(grid, bands, m_tgrInfo);

  double dummyvalue = m_rst->getBand(0)->getProperty()->m_noDataValue;

  int32_t fbnode = 0;

  if (m_gridtype == Quintico)    //Quintic Surface Adjust without breaklines
  {
    fbnode = m_fbnode;
    m_fbnode = 0;
  }

  //if ((m_gridtype == QuinticoBrkLine) || (m_gridtype == Quintico))    //Quintic Surface Adjust
  //  if (!Tnsderiv)
  //    if (NodeDerivatives() == FALSE)
  //      return FALSE;

  int32_t nodesid[3];
  te::gm::PointZ p3da[3];
  int32_t flin, llin, fcol, lcol;
  short j;

  //  To each triangle
  for (int32_t i = 0; i < (int32_t) m_triang.size(); i++)
  { // Find Triangle Box
    NodesId(i, nodesid);
    if (!DefineInterLinesColumns(nodesid, flin, llin, fcol, lcol))
      continue;
    for (j = 0; j < 3; j++)
    {
      p3da[j].setX(m_node[nodesid[j]].getNPoint().getX());
      p3da[j].setY(m_node[nodesid[j]].getNPoint().getY());
      p3da[j].setZ(m_node[nodesid[j]].getZ());
    }

    if ((p3da[0].getZ() == dummyvalue) ||
      (p3da[1].getZ() == dummyvalue) ||
      (p3da[2].getZ() == dummyvalue))
    {
       FillGridValue(i, flin, llin, fcol, lcol, dummyvalue);
    }
    else if ((p3da[0].getZ() == p3da[1].getZ()) &&
      (p3da[0].getZ() == p3da[2].getZ()) && (m_gridtype == Linear))
      FillGridValue(i, flin, llin, fcol, lcol, p3da[0].getZ());
    //else if ((m_gridtype == Quintico) || (m_gridtype == QuinticoBrkLine))
    //{
    //  NeighborsId(i, neighsid);
    //  for (j = 0; j < 3; j++)
    //    if (neighsid[j] == -1)
    //      break;
    //  if (j == 3)
    //  {
    //    DefineAkimaCoeficients(i, nodesid, p3da, coef);
    //    FillGridQuintic(grid, i, p3da, flin, llin, fcol, lcol, coef);
    //  }
    //  else
    //    FillGridValue(grid, i, flin, llin, fcol, lcol, dummyvalue);
    //}
    else
      FillGridLinear(i, p3da, flin, llin, fcol, lcol);
  }

  if (m_gridtype == Quintico)    //Quintic Surface Adjust without breaklines
    m_fbnode = fbnode;

  delete m_rst;
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

void te::mnt::TINCalculateGrid::setOutput(std::map<std::string, std::string> &tgrInfo, std::string dsname)
{
  m_tgrInfo = tgrInfo;
  m_outDsetName = dsname;

}

void te::mnt::TINCalculateGrid::setParams(double resx, double resy, GridType gt)
{
  m_resx = resx;
  m_resy = resy;
  m_gridtype = gt;
}


/*!
\brief Method that calculates the lines and the columns intercepted by a triangle
\param grid is a pointer to a grid object that will be created
\param nodesid is a vector with nodes identification of the current triangle
\param flin and llin are the first and the last lines (rows) of the grid
\param fcol and lcol are the first and the last columns of the grid
\return TRUE if the gradient grid is filled or FALSE otherwise
*/

bool te::mnt::TINCalculateGrid::DefineInterLinesColumns(int32_t *nodesid, int32_t &flin, int32_t &llin, int32_t &fcol, int32_t &lcol)
{
  double rx1 = m_rst->getExtent()->getLowerLeftX() + m_resx/2;
  double ry2 = m_rst->getExtent()->getUpperRightY() - m_resy/2;

  te::gm::Point llpt(FLT_MAX, FLT_MAX);
  te::gm::Point urpt(-FLT_MAX, -FLT_MAX);
  for (size_t j = 0; j < 3; j++)
  {
    llpt = Min(llpt, m_node[nodesid[j]].getNPoint());
    urpt = Max(urpt, m_node[nodesid[j]].getNPoint());
  }

  //  Calculate lines and coluns intercepted
  fcol = (int32_t)((llpt.getX() - rx1) / m_resx + .5);
  lcol = (int32_t)((urpt.getX() - rx1) / m_resx + .5);
  flin = (int32_t)((ry2 - urpt.getY()) / m_resy + .5);
  llin = (int32_t)((ry2 - llpt.getY()) / m_resy + .5);

  if (((int32_t)m_rst->getNumberOfColumns() <= fcol) || (lcol < 0) ||
    ((int32_t)m_rst->getNumberOfRows() <= flin) || (llin < 0))
    return false;

  if (fcol < 0)
    fcol = 0;
  if (flin < 0)
    flin = 0;
  if ((int32_t)m_rst->getNumberOfColumns() <= lcol)
    lcol = m_rst->getNumberOfColumns() - 1;
  if ((int32_t)m_rst->getNumberOfRows() <= llin)
    llin = m_rst->getNumberOfRows() - 1;

  return true;
}


/*!
\brief Method that fills the grid locations, inside a triangle, with a zvalue
\param grid is a pointer to a grid object
\param triid is the triangle identification number
\param flin and llin are the first and the last lines (rows) of the grid
\param fcol and lcol are the first and the last columns of the grid
\param zvalue is the z value to be stored in the grid inside the triangle region
\return TRUE always
*/
bool te::mnt::TINCalculateGrid::FillGridValue(int32_t triid, int32_t flin, int32_t llin, int32_t fcol, int32_t lcol, double zvalue)
{
  int32_t  nlin, ncol;
  double  rx1, ry2;
  te::gm::PointZ pg;
  double dummyvalue = m_rst->getBand(0)->getProperty()->m_noDataValue;

  rx1 = m_rst->getExtent()->getLowerLeftX();
  ry2 = m_rst->getExtent()->getUpperRightY();

  for (nlin = flin; nlin <= llin; nlin++){
    for (ncol = fcol; ncol <= lcol; ncol++){
    //  pg.setX(rx1 + (float)ncol*m_resx);
     // pg.setY(ry2 - (float)nlin*m_resy);
     // if (!(ContainsPoint(triid, pg)))
     //   continue;
        m_rst->setValue(ncol, nlin, zvalue);
    }
  }


  return true;
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
  double x1_x0, x2_x0, y1_y0, y2_y0, z1_z0, z2_z0,
    detx, dety, detz;
  double rx1, ry2;
  double zvalue;
  int32_t nlin, ncol;
  double dummyvalue = m_rst->getBand(0)->getProperty()->m_noDataValue;

  rx1 = m_rst->getExtent()->getLowerLeftX() + m_resx / 2.;
  ry2 = m_rst->getExtent()->getUpperRightY() - m_resy / 2.;

  x1_x0 = p3da[1].getX() - p3da[0].getX();
  x2_x0 = p3da[2].getX() - p3da[0].getX();
  y1_y0 = p3da[1].getY() - p3da[0].getY();
  y2_y0 = p3da[2].getY() - p3da[0].getY();
  z1_z0 = (double)(p3da[1].getZ() - p3da[0].getZ());
  z2_z0 = (double)(p3da[2].getZ() - p3da[0].getZ());
  for (nlin = flin; nlin <= llin; nlin++){
    for (ncol = fcol; ncol <= lcol; ncol++){
      pg.setX(rx1 + (float)ncol*m_resx);
      pg.setY(ry2 - (float)nlin*m_resy);
      if (!(ContainsPoint(triid, pg)))
        continue;
      detx = ((y1_y0 * z2_z0) - (y2_y0 * z1_z0)) *
        (pg.getX() - p3da[0].getX());
      dety = ((z1_z0 * x2_x0) - (z2_z0 * x1_x0)) *
        (pg.getY() - p3da[0].getY());
      detz = (x1_x0 * y2_y0) - (x2_x0 * y1_y0);

      zvalue = ((detx + dety - detz * p3da[0].getZ()) / -detz);

      if (zvalue > m_max)
        zvalue = dummyvalue;
      if (zvalue < m_min)
        zvalue = dummyvalue;

       m_rst->setValue(ncol, nlin, zvalue);
    }
  }

  return true;
}
/*!
\brief Method that defines the coefficients of the Akima polynomium fitted in a given triangle
\param triid is the triangle identification number
\param nodesid is the list of triangle nodes identification
\param p3d is a pointer to a Point3d object
\param coef is a pointer to a double vector containing the polynomium coefficients
\return TRUE if the coefficients are determined with no errors or FALSE otherwise
*/

//bool te::mnt::TINCalculateGrid::DefineAkimaCoeficients(int32_t triid, int32_t *nodesid, te::gm::PointZ *p3d, double *coef)
//{
//  double  a, b, c, d,
//    aa, bb, cc, dd,
//    ad, bc, det,
//    ap, bp, cp, dp,
//    lu, lv,
//    theta, phi, thxu, thphi,
//    cstheta, lusntheta, lvsntheta,
//    e, f, g, h,
//    p00, p01, p02, p03, p04, p05,
//    p10, p11, p12, p13, p14,
//    p20, p21, p22, p23,
//    p30, p31, p32,
//    p40, p41, p50,
//    h1, h2, h3, g1, g2,
//    fg, eh, ee, gg,
//    zu[3], zv[3], zuu[3], zvv[3], zuv[3];
//  short  i, bside;
//  int32_t  lids[3], nodid;
//
//  // Coeficients of conversion from UV to XY coordinates
//  a = p3d[1].getX() - p3d[0].getX();
//  b = p3d[2].getX() - p3d[0].getX();
//  c = p3d[1].getY() - p3d[0].getY();
//  d = p3d[2].getY() - p3d[0].getY();
//
//  aa = a * a;
//  bb = b * b;
//  cc = c * c;
//  dd = d * d;
//
//  // Coeficients of conversion from XY to UV coordinates
//  ad = a * d;
//  bc = b * c;
//  det = ad - bc;
//  ap = d / det;
//  bp = -b / det;
//  cp = -c / det;
//  dp = a / det;
//
//  bside = 0;
//  if (m_fbnode > 0)
//  {
//    // If there are breaklines
//    m_triang[triid].LinesId(lids);
//    for (i = 0; i < 3; i++)
//    {
//      if ((m_line[lids[i]].getNodeTo() >= m_fbnode) &&
//        (m_line[lids[i]].getNodeFrom() >= m_fbnode))
//      {
//        if (m_line[lids[i]].getRightPolygon() == triid)
//          // Triangle at the right side of a breakline
//          bside = 1;
//        else if (m_line[lids[i]].getLeftPolygon() == triid)
//          // Triangle at the left side of a breakline
//          bside = 2;
//        else
//          return false;
//        break;
//      }
//      if (m_line[lids[i]].getNodeTo() >= m_fbnode)
//      {
//        // Triangle at the right side of a breakline
//        bside = 1;
//        break;
//      }
//      if (m_line[lids[i]].getNodeFrom() >= m_fbnode)
//      {
//        // Triangle at the left side of a breakline
//        bside = 2;
//        break;
//      }
//    }
//  }
//
//  // Conversion of derivatives from XY to UV coordinates
//  for (i = 0; i < 3; i++)
//  {
//    if ((m_fbnode == 0) ||
//      ((m_fbnode > 0) && (nodesid[i] < m_fbnode)))
//    {
//      nodid = nodesid[i];
//      fderiv = Tnfderiv;
//      sderiv = Tnsderiv;
//    }
//    else if (bside == 1)
//    {
//      nodid = nodesid[i] - Tfbnode;
//      fderiv = Tnbrfderiv;
//      sderiv = Tnbrsderiv;
//    }
//    else if (bside == 2)
//    {
//      nodid = nodesid[i] - Tfbnode;
//      fderiv = Tnblfderiv;
//      sderiv = Tnbrsderiv;
//    }
//    else{
//      return false;
//    }
//
//    if (fderiv[nodid].Y() == dummyvalue)
//    {
//      zu[i] = 0.; zv[i] = 0.;
//    }
//    else
//    {
//      zu[i] = a * fderiv[nodid].X() +
//        c * fderiv[nodid].Y();
//      zv[i] = b * fderiv[nodid].X() +
//        d * fderiv[nodid].Y();
//    }
//    if (sderiv[nodid].Z() == dummyvalue)
//    {
//      zuu[i] = 0.;
//      zuv[i] = 0.;
//      zvv[i] = 0.;
//    }
//    else
//    {
//      zuu[i] = aa * sderiv[nodid].X() +
//        2.*a*c * (double)sderiv[nodid].Z() +
//        cc * sderiv[nodid].Y();
//      zuv[i] = a*b * sderiv[nodid].X() +
//        (ad + bc) * (double)sderiv[nodid].Z() +
//        c*d * sderiv[nodid].Y();
//      zvv[i] = bb * sderiv[nodid].X() +
//        2.*b*d * (double)sderiv[nodid].Z() +
//        dd * sderiv[nodid].Y();
//    }
//  }
//
//  // Calculate U and V side sizes
//  lu = sqrt(aa + cc);
//  lv = sqrt(bb + dd);
//
//  // Calculate angle between U and V axis and its cosine
//  thxu = atan2(c, a);
//  theta = atan2(d, b) - thxu;
//  cstheta = cos(theta);
//  lusntheta = lu * sin(theta);
//  lvsntheta = lv * sin(theta);
//
//  // Calculate angle between U and S axis and its cosine
//  phi = atan2(d - c, b - a) - thxu;
//  thphi = theta - phi;
//
//  // Coeficients of conversion from ST to UV coordinates when
//  // S axis is paralel to v2v3 side
//  e = sin(thphi) / lusntheta;
//  f = -cos(thphi) / lusntheta;
//  g = sin(phi) / lvsntheta;
//  h = cos(phi) / lvsntheta;
//
//  // Definition of the polynomial coefficients
//  // Using (u,v) = (0,0) -> v1
//  p00 = p3d[0].getZ();
//  p10 = zu[0];
//  p01 = zv[0];
//  p11 = zuv[0];
//  p20 = zuu[0] / 2.;
//  p02 = zvv[0] / 2.;
//
//  // Using (u,v) = (1,0) -> v2 and z(u,v), zu and zuu
//  h1 = p3d[1].getZ() - p00 - p10 - p20;
//  h2 = zu[1] - p10 - zuu[0];
//  h3 = zuu[1] - zuu[0];
//  p30 = 10.*h1 - 4.*h2 + h3 / 2.;
//  p40 = -15.*h1 + 7.*h2 - h3;
//  p50 = 6.*h1 - 3.*h2 + h3 / 2.;
//
//  // Using (u,v) = (0,1) -> v3 and z(u,v), zu and zuu
//  h1 = p3d[2].getZ() - p00 - p01 - p02;
//  h2 = zv[2] - p01 - zvv[0];
//  h3 = zvv[2] - zvv[0];
//  p03 = 10.*h1 - 4.*h2 + h3 / 2.;
//  p04 = -15.*h1 + 7.*h2 - h3;
//  p05 = 6.*h1 - 3.*h2 + h3 / 2.;
//
//  p41 = 5. * lv * cstheta * p50 / lu;
//  p14 = 5. * lu * cstheta * p05 / lv;
//
//  // Using (u,v) = (1,0) -> v2 and z(u,v) and zuv
//  h1 = zv[1] - p01 - p11 - p41;
//  h2 = zuv[1] - p11 - 4.*p41;
//  p21 = 3.*h1 - h2;
//  p31 = -2.*h1 + h2;
//
//  // Using (u,v) = (1,0) -> v3 and z(u,v) and zuv
//  h1 = zu[2] - p10 - p11 - p14;
//  h2 = zuv[2] - p11 - 4.*p14;
//  p12 = 3.*h1 - h2;
//  p13 = -2.*h1 + h2;
//
//  // Using continuity restriction and zvv at v2 and zuu at v3
//  fg = f * g;
//  eh = e * h;
//  ee = e * e;
//  gg = g * g;
//
//  g1 = ee*g*(3.*fg + 2.*eh);
//  g2 = e*gg*(2.*fg + 3.*eh);
//
//  h1 = -5.*ee*ee*f*p50 -
//    ee*e*(fg + eh)*p41 -
//    gg*g*(fg + 4 * eh) -
//    5.*gg*gg*h*p05;
//
//  h2 = zvv[1] / 2. - p02 - p12;
//  h3 = zuu[2] / 2. - p20 - p21;
//
//  p22 = (g1*h2 + g2*h3 - h1) / (g1 + g2);
//  p32 = h2 - p22;
//  p23 = h3 - p22;
//
//  // Polynomial coefficients
//  coef[0] = p00; coef[1] = p01; coef[2] = p02; coef[3] = p03; coef[4] = p04; coef[5] = p05;
//  coef[6] = p10; coef[7] = p11; coef[8] = p12; coef[9] = p13; coef[10] = p14;
//  coef[11] = p20; coef[12] = p21; coef[13] = p22; coef[14] = p23;
//  coef[15] = p30; coef[16] = p31; coef[17] = p32;
//  coef[18] = p40; coef[19] = p41;
//  coef[20] = p50;
//
//  // Coefficients of conversion from XY to UV coordinates
//  coef[21] = ap; coef[22] = bp; coef[23] = cp; coef[24] = dp;
//  coef[25] = p3d[0].X(); coef[26] = p3d[0].Y();
//
//  return true;
//}

