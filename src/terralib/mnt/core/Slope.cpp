/*!
\file terralib/mnt/core/Slope.cpp

\brief This file contains a class to generate Slope grid.

*/

#include "Slope.h"
#include "Utils.h"

//terralib
#include "../../dataaccess/utils/Utils.h"
#include "../../geometry/GeometryProperty.h"
#include "../../raster.h"
#include "../../raster/BandProperty.h"
#include "../../raster/Grid.h"
#include "../../raster/RasterFactory.h"


te::mnt::Slope::Slope()
{
}

te::mnt::Slope::~Slope()
{
}

void te::mnt::Slope::setInput(te::da::DataSourcePtr inDsrc,
  std::string inDsetName,
  std::auto_ptr<te::da::DataSetType> inDsetType)
{
  m_inDsrc = inDsrc;
  m_inDsetName = inDsetName;
  m_inDsetType = inDsetType;

}

void te::mnt::Slope::setOutput(std::map<std::string, std::string> &dsinfo)
{
  m_dsinfo = dsinfo;
}

void te::mnt::Slope::setParams(double resx, double resy, char grad, char slope)
{
  m_resx = resx;
  m_resy = resy;
  m_gradtype = grad;
  m_slopetype = slope;
}

bool te::mnt::Slope::run()
{
  if (m_inDsetType->hasRaster()) //GRID
  {
    m_inputType = GRID;
  }
  else
  {
    m_inputType = TIN;
    if (!LoadTin(m_inDsrc, m_inDsetName))
      return false;
    // Calculate derivatives
    if (!NodeDerivatives())
      return false;
  }

  //Create greater box 1/2 resolution
  double rx1 = m_env.getLowerLeftX() - m_resx / 2.;
  double ry2 = m_env.getUpperRightY() + m_resy / 2.;

  int outputWidth = (int)ceil(m_env.getWidth() / m_resx);
  int outputHeight = (int)ceil(m_env.getHeight() / m_resy);

  te::gm::Coord2D ulc(rx1, ry2);

  te::rst::Grid* grid = new te::rst::Grid(outputWidth, outputHeight, m_resx, m_resy, &ulc, m_srid);

  std::vector<te::rst::BandProperty*> bands;

  bands.push_back(new te::rst::BandProperty(0, te::dt::DOUBLE_TYPE, "DTM GRID"));
  bands[0]->m_nblocksx = 1;
  bands[0]->m_nblocksy = outputHeight;
  bands[0]->m_blkw = outputWidth;
  bands[0]->m_blkh = 1;
  bands[0]->m_colorInterp = te::rst::GrayIdxCInt;
  bands[0]->m_noDataValue = m_nodatavalue;

  // create raster
  std::auto_ptr<te::rst::Raster> rst(te::rst::RasterFactory::make("GDAL", grid, bands, m_dsinfo));

  m_rst = rst.get();

  double dummyvalue = m_rst->getBand(0)->getProperty()->m_noDataValue;

  if (m_inputType == TIN)
  {
    int32_t nodesid[3];
    int32_t flin, llin, fcol, lcol;
    double m1, m2;

    //To each triangle
    for (int32_t i = 0; i < m_ltriang; i++)
    {
      // Find Triangle Box
      NodesId(i, nodesid);
      if (!DefineInterLinesColuns(nodesid, flin, llin, fcol, lcol))
        continue;

      //Special cases
      m1 = m2 = std::numeric_limits<double>::max();
      if ((m_node[nodesid[1]].getY() - m_node[nodesid[0]].getY()) != 0.0)
        m1 = (m_node[nodesid[1]].getX() - m_node[nodesid[0]].getX()) /
        (m_node[nodesid[1]].getY() - m_node[nodesid[0]].getY());

      if ((m_node[nodesid[2]].getY() - m_node[nodesid[0]].getY()) != 0.0)
        m2 = (m_node[nodesid[2]].getX() - m_node[nodesid[0]].getX()) /
        (m_node[nodesid[2]].getY() - m_node[nodesid[0]].getY());

      if (fabs(m1 - m2) < m_tol)
        FillGridValue(m_rst, i, flin, llin, fcol, lcol, m_nodatavalue);
      else
      {
        // Calculate gradient of triangle
        double decvalue = TriangleGradient(nodesid, m_gradtype, m_slopetype);
        FillGridValue(m_rst, i, flin, llin, fcol, lcol, decvalue);
      }
    }
  }

  return true;
}


bool te::mnt::Slope::DefineInterLinesColuns(int32_t *nodesid, int32_t &flin, int32_t &llin, int32_t &fcol, int32_t &lcol)
{
  te::gm::PointZ p3da[3];
  double rx1, ry2;

  rx1 = m_env.getLowerLeftX() + m_resx / 2.;
  ry2 = m_env.getUpperRightY() - m_resy / 2.;

  double llx = std::numeric_limits<double>::max();
  double lly = std::numeric_limits<double>::max();
  double urx = -std::numeric_limits<double>::max();
  double ury = -std::numeric_limits<double>::max();
  for (int j = 0; j < 3; j++)
  {
    llx = std::min(m_node[nodesid[j]].getX(), llx);
    lly = std::min(m_node[nodesid[j]].getY(), lly);
    urx = std::max(m_node[nodesid[j]].getX(), urx);
    ury = std::max(m_node[nodesid[j]].getY(), ury);
  }

  // Calculate lines and coluns intercepted
  fcol = int32_t((llx - rx1) / m_resx);
  lcol = int32_t((urx - rx1) / m_resx);
  flin = int32_t((ry2 - ury) / m_resy);
  llin = int32_t((ry2 - lly) / m_resy);

  if ((m_rst->getNumberOfColumns() <= (unsigned)fcol) || (lcol < 0) ||
    (m_rst->getNumberOfRows() <= (unsigned)flin) || (llin < 0))
    return false;

  if (fcol < 0)
    fcol = 0;
  if (flin < 0)
    flin = 0;
  if (m_rst->getNumberOfColumns() <= (unsigned)lcol)
    lcol = m_rst->getNumberOfColumns() - 1;
  if (m_rst->getNumberOfRows() <= (unsigned)llin)
    llin = m_rst->getNumberOfRows() - 1;

  return true;
}

double te::mnt::Slope::TriangleGradient(int32_t *nodesid, char gradtype, char slopetype)
{
  te::gm::PointZ p3da[3];
  double nvector[3];
  double moduv, decliv;
  double pi180 = 180. / 3.1415927;
  double m1, m2;
  short j;

  for (j = 0; j < 3; j++)
  {
    p3da[j].setX(m_node[nodesid[j]].getX());
    p3da[j].setY(m_node[nodesid[j]].getY());
    p3da[j].setZ(m_node[nodesid[j]].getZ());
  }

  //Special cases
  m1 = m2 = std::numeric_limits<double>::max();

  if ((p3da[1].getY() - p3da[0].getY()) != 0.0)
    m1 = (p3da[1].getX() - p3da[0].getX()) / (p3da[1].getY() - p3da[0].getY());

  if ((p3da[2].getY() - p3da[0].getY()) != 0.0)
    m2 = (p3da[2].getX() - p3da[0].getX()) / (p3da[2].getY() - p3da[0].getY());

  if (fabs(m1 - m2) < m_tol) 
    // Triangle with DUMMY Value
    return m_nodatavalue;
  
  if ((p3da[0].getZ() >= m_nodatavalue) || (p3da[1].getZ() >= m_nodatavalue) ||
    (p3da[2].getZ() >= m_nodatavalue))  // Triangle with DUMMY Value
    return m_nodatavalue;

  if ((p3da[0].getZ() == p3da[1].getZ()) && (p3da[0].getZ() == p3da[2].getZ()))
  {
    if (gradtype == 's')
      // Slope is zero
      return 0.;
    else
      //exposition DUMMY
      return m_nodatavalue;
  }

  triangleNormalVector(p3da, nvector);
  moduv = sqrt(pow(nvector[0], 2.) + pow(nvector[1], 2.) + pow(nvector[2], 2.));

  if (gradtype == 's')
  {
    // Case Slope
    if (slopetype == 'g') // degrees
      decliv = pi180*acos(nvector[2] / moduv);
    else
      decliv = tan(acos(nvector[2] / moduv)) * 100.;
  }
  else
  {
    // Caso exposicao
    decliv = 90. - pi180*atan2(nvector[1], nvector[0]);
    if (decliv < 0.)
      decliv = 360. + decliv;
  }

  return decliv;
}
