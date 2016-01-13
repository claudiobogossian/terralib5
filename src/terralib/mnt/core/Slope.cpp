/*!
\file terralib/mnt/core/Slope.cpp

\brief This file contains a class to generate Slope grid.

*/

#include "Slope.h"
#include "Utils.h"

//terralib
#include "../../common/progress/TaskProgress.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../geometry/GeometryProperty.h"
#include "../../raster.h"
#include "../../raster/BandProperty.h"
#include "../../raster/Grid.h"
#include "../../raster/RasterFactory.h"
#include "../../srs/SpatialReferenceSystemManager.h"

te::mnt::Slope::Slope()
:m_tol(0.000001)
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

void te::mnt::Slope::setParams(double resx, double resy, char grad, char slope, int srid, double dummy)
{
  m_resx = resx;
  m_resy = resy;
  m_gradtype = grad;
  m_slopetype = slope;
  m_srid = srid;
  m_dummy = dummy;
}

bool te::mnt::Slope::run()
{
  std::auto_ptr<te::rst::Raster> in_raster;
  if (m_inDsetType->hasRaster()) //GRID
  {
    m_inputType = GRID;
    te::rst::RasterProperty* rasterProp = te::da::GetFirstRasterProperty(m_inDsetType.get());
    std::auto_ptr<te::da::DataSet> dsRaster = m_inDsrc->getDataSet(m_inDsetName);
    in_raster = dsRaster->getRaster(rasterProp->getName());
    m_env = *in_raster->getExtent();
    m_resx = in_raster->getResolutionX();
    m_resy = in_raster->getResolutionY();
    in_raster->getBand(0)->getProperty()->m_noDataValue = m_dummy;
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
  double rx1 = m_env.getLowerLeftX();
  double ry2 = m_env.getUpperRightY();

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

  if (m_inputType == TIN)
  {
    int32_t nodesid[3];
    int32_t flin, llin, fcol, lcol;
    double m1, m2;

    te::common::TaskProgress task("Calculating Slope...", te::common::TaskProgress::UNDEFINED, (int)m_ltriang);
    //To each triangle
    for (unsigned int i = 0; i < (unsigned int)m_ltriang; i++)
    {
      task.pulse();
      // Find Triangle Box
      if (!NodesId((int32_t)i, nodesid))
        continue;
      if (!DefineInterLinesColumns(nodesid, flin, llin, fcol, lcol))
        continue;

      //Special cases
      m1 = m2 = std::numeric_limits<double>::max();
      if ((m_node[(unsigned int)nodesid[1]].getY() - m_node[(unsigned int)nodesid[0]].getY()) != 0.0)
        m1 = (m_node[(unsigned int)nodesid[1]].getX() - m_node[(unsigned int)nodesid[0]].getX()) /
        (m_node[(unsigned int)nodesid[1]].getY() - m_node[(unsigned int)nodesid[0]].getY());

      if ((m_node[(unsigned int)nodesid[2]].getY() - m_node[(unsigned int)nodesid[0]].getY()) != 0.0)
        m2 = (m_node[(unsigned int)nodesid[2]].getX() - m_node[(unsigned int)nodesid[0]].getX()) /
        (m_node[(unsigned int)nodesid[2]].getY() - m_node[(unsigned int)nodesid[0]].getY());

      if (fabs(m1 - m2) < m_tol)
        FillGridValue((int32_t)i, flin, llin, fcol, lcol, m_nodatavalue);
      else
      {
        // Calculate gradient of triangle
        double decvalue = TriangleGradient(nodesid, m_gradtype, m_slopetype);
        FillGridValue((int32_t)i, flin, llin, fcol, lcol, decvalue);
      }
    }
  }
  else
  {
    double pi180 = 180. / 3.1415927;
    double dzdy, dzdx;
    double zvalue;
    double EPSILON = 1.0e-40;

    m_min = in_raster->getBand(0)->getMinValue(true, 0, 0, outputHeight-1, outputWidth-1).real();
    m_max = in_raster->getBand(0)->getMaxValue(true, 0, 0, outputHeight-1, outputWidth-1).real();
  
    m_dx = m_resx;
    m_dy = m_resy;

    te::common::UnitOfMeasurePtr unitin = te::srs::SpatialReferenceSystemManager::getInstance().getUnit((unsigned int)m_srid);

    if (unitin && unitin->getId() == te::common::UOM_Degree)
    {
      m_dx *= 111000;            // 1 degree = 111.000 meters
      m_dy *= 111000;            // 1 degree = 111.000 meters
    }

    te::common::TaskProgress task("Calculating Slope...", te::common::TaskProgress::UNDEFINED, (int)(in_raster->getNumberOfRows()*in_raster->getNumberOfColumns()));
    for (unsigned l = 1; l < in_raster->getNumberOfRows() - 1; l++)
    {
      for (unsigned c = 1; c < in_raster->getNumberOfColumns() - 1; c++)
      {
        task.pulse();
        if (CalcGradientRst(in_raster, l, c, dzdx, dzdy))
        {
          if (m_gradtype == 's')
          { // Slope
            if (m_slopetype == 'g') // graus
              zvalue = (pi180*atan(sqrt((dzdx*dzdx) + (dzdy*dzdy))));
            else // percentagem
              zvalue = (sqrt((dzdx*dzdx) + (dzdy*dzdy))*100.);
          }
          else
          { // Aspect
            if ((dzdx > (-EPSILON)) && (dzdx < EPSILON)){  // dzdx ~= 0.
              if (dzdy > EPSILON)
                zvalue = 180.0;
              else if (dzdy < (-EPSILON))
                zvalue = 0.0;
              else zvalue = m_nodatavalue; // nao existe exposicao
            }
            else
            {
              zvalue = 90. - (pi180*atan2(dzdy, dzdx));
              if (zvalue < 0.) zvalue = 360. + zvalue;
              zvalue = 360. - zvalue;
            }
          }
        } // if (CalcGradientRst...
        else
          zvalue = m_nodatavalue;

        m_rst->setValue(c, l, zvalue);
      } //for (int c = 1; ...
    } //for (int l = 1; ...
  }

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
    p3da[j].setX(m_node[(unsigned int)nodesid[j]].getX());
    p3da[j].setY(m_node[(unsigned int)nodesid[j]].getY());
    p3da[j].setZ(m_node[(unsigned int)nodesid[j]].getZ());
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

bool te::mnt::Slope::CalcGradientRst(std::auto_ptr<te::rst::Raster> &raster, unsigned l, unsigned c, double& dzdx, double& dzdy)
{
  bool gradient = true;

  double pixlantcant;
  double pixlantc;
  double pixlantcpos;
  double pixlcant;
  double pixlcpos;
  double pixlposcant;
  double pixlposc;
  double pixlposcpos;

  raster->getValue(c - 1, l - 1, pixlantcant);
  raster->getValue(c, l - 1, pixlantc);
  raster->getValue(c + 1, l - 1, pixlantcpos);
  raster->getValue(c - 1, l, pixlcant);
  raster->getValue(c + 1, l, pixlcpos);
  raster->getValue(c - 1, l + 1, pixlposcant);
  raster->getValue(c, l + 1, pixlposc);
  raster->getValue(c + 1, l + 1, pixlposcpos);

  if ((pixlantc <= m_max && pixlantc >= m_min) && (pixlcant <= m_max && pixlcant >= m_min) &&
    (pixlposc <= m_max && pixlposc >= m_min) && (pixlcpos <= m_max && pixlcpos >= m_min))
  {
    if ((pixlantcant <= m_max && pixlantcant >= m_min) && (pixlantcpos <= m_max && pixlantcpos >= m_min) &&
      (pixlposcant <= m_max && pixlposcant >= m_min) && (pixlposcpos <= m_max && pixlposcpos >= m_min))
    {  // vizinhanca 8
      // Calculate dzdx and dzdy derivative values with neighbourhood 8
      dzdx = (((pixlposcpos + 2.*pixlcpos + pixlantcpos)
        - (pixlposcant + 2.*pixlcant + pixlantcant)) / (8.*m_dx));
      dzdy = (((pixlposcpos + 2.*pixlposc + pixlposcant)
        - (pixlantcpos + 2.*pixlantc + pixlantcant)) / (8.*m_dy));
    }
    else
    { // vizinhanca 4 usando os vizinhos do meio
      // Calculate dzdx and dzdy derivative values with the neighbourhood 4
      dzdx = ((pixlcpos - pixlcant) / (2.*m_dx));
      dzdy = ((pixlposc - pixlantc) / (2.*m_dy));
    }
  }
  else  // vizinhanca 4 usando os vizinhos extremos
  {
    if ((pixlantcant <= m_max && pixlantcant >= m_min) && (pixlantcpos <= m_max && pixlantcpos >= m_min) &&
      (pixlposcant <= m_max && pixlposcant >= m_min) && (pixlposcpos <= m_max && pixlposcpos >= m_min))
    {  // visinhanca 8
      // Calculate dzdx and dzdy derivative values with neighbourhood 
      dzdx = (((pixlposcpos + pixlantcpos)
        - (pixlposcant + pixlantcant)) / (4.*m_dx));
      dzdy = (((pixlposcpos + pixlposcant)
        - (pixlantcpos + pixlantcant)) / (4.*m_dy));
    }
    else
      gradient = false;
  }

  return gradient;
}

