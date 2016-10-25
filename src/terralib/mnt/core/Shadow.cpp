/*!
\file terralib/mnt/core/Shadow.cpp

\brief This file contains a class to shadow image generation.

*/

#include "Shadow.h"
#include "Utils.h"

//terralib
#include "../../common/progress/TaskProgress.h"
#include "../../core/translator/Translator.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../geometry/GeometryProperty.h"
#include "../../raster/BandProperty.h"
#include "../../raster/Grid.h"
#include "../../raster/RasterFactory.h"
#include "../../raster/RasterProperty.h"
#include "../../srs/SpatialReferenceSystemManager.h"

te::mnt::Shadow::Shadow()
  :m_out(0),
  m_srid(0),
  m_azimuth(0.),
  m_elevation(0.),
  m_relief(0.),
  m_vmin(0), 
  m_vmax(0),
  m_minval(0),
  m_maxval(0),
  m_dummy(0),
  m_outputWidth(0),
  m_outputHeight(0),
  m_resxo(0),
  m_resyo(0)
{
}

te::mnt::Shadow::~Shadow()
{
  if (m_out)
    delete m_out;
  inDset.release();
  m_inDsetType.release();
}

void te::mnt::Shadow::setInput(te::da::DataSourcePtr inDsrc,
  std::string inDsetName,
  std::auto_ptr<te::da::DataSetType>& inDsetType)
{
  m_inDsrc = inDsrc;
  m_inDsetName = inDsetName;
  m_inDsetType = inDsetType;

}

void te::mnt::Shadow::setOutput(std::map<std::string, std::string> &dsinfo, std::string outRstDSType)
{
  m_outDsinfo = dsinfo;
  m_outRstDSType = outRstDSType;
}

void te::mnt::Shadow::setParams(double azimuth, double elevation, double relief, 
  double vmin, double vmax, double minval, double maxval, double dummy,
  unsigned int outputWidth, unsigned int outputHeight, double resxo, double resyo)
{
  m_azimuth = azimuth;
  m_elevation = elevation;
  m_relief = relief;
  m_vmin = vmin;
  m_vmax = vmax;
  m_minval = minval;
  m_maxval = maxval;
  m_dummy = dummy;
  m_outputWidth = outputWidth;
  m_outputHeight = outputHeight;
  m_resxo = resxo;
  m_resyo = resyo;
}

bool te::mnt::Shadow::run()
{
  //get input raster
  if (!m_inDsetType.get()->hasRaster())
    throw te::common::Exception(TE_TR("Layer isn't Regular Grid."));

  te::rst::RasterProperty* rasterProp = te::da::GetFirstRasterProperty(m_inDsetType.get());
  inDset = m_inDsrc->getDataSet(m_inDsetName);
  std::unique_ptr<te::rst::Raster> raster = inDset->getRaster(rasterProp->getName());
  if (raster->getNumberOfBands() > 1)
    throw te::common::Exception(TE_TR("Layer isn't Regular Grid."));

  m_out = GenerateImage(raster.get());

  raster.release();
  return true;
}

bool te::mnt::Shadow::calcLocalGradient(te::rst::Raster* inputRaster, int line, int col, double vmin, double vmax, double& dx, double& dy, double *val)
{
  dx = 0;
  dy = 0;

  double factor = 1;

  double deltaX = inputRaster->getResolutionX() * factor;
  double deltaY = inputRaster->getResolutionY() * factor;

  if (((val[1] <= vmax) && (val[1] >= vmin)) && ((val[3] <= vmax) && (val[3] >= vmin)) &&
    ((val[6] <= vmax) && (val[6] >= vmin)) && ((val[4] <= vmax) && (val[4] >= vmin)))
  {
    if (((val[0] <= vmax) && (val[0] >= vmin)) && ((val[2] <= vmax) && (val[2] >= vmin)) &&
      ((val[5] <= vmax) && (val[5] >= vmin)) && ((val[7] <= vmax) && (val[7] >= vmin)))
    {//Calculate dzdx and dzdy derivative values with neighbourhood 8
      dx = ((val[7] + (2 * val[4]) + val[2]) - (val[5] + (2 * val[3]) + val[0])) / (8 * deltaX);
      dy = ((val[7] + (2 * val[6]) + val[5]) - (val[2] + (2 * val[1]) + val[0])) / (8 * deltaY);
    }
    else
    {//Calculate dzdx and dzdy derivative values with the neighbourhood 4
      dx = (val[4] - val[3]) / (2 * deltaX);
      dy = (val[6] - val[1]) / (2 * deltaY);
    }
  }
  else
  {//neighborhood 4 using extreme neighbors
    if (((val[0] <= vmax) && (val[0] >= vmin)) && ((val[2] <= vmax) && (val[2] >= vmin)) &&
      ((val[5] <= vmax) && (val[5] >= vmin)) && ((val[7] <= vmax) && (val[7] >= vmin)))
    {
      dx = ((val[7] + val[2]) - (val[5] + val[0])) / (4 * deltaX);
      dy = ((val[7] + val[5]) - (val[2] + val[0])) / (4 * deltaY);
    }
    else
      return false;
  }

  return true;
}


void te::mnt::Shadow::getRasterElementLine(te::rst::Raster* inputRaster, int line, std::vector< std::complex<double> > &val)
{
  int ncols = inputRaster->getNumberOfColumns();
  std::vector< std::complex<double> > aux;
  for (int col = 0; col < ncols; col++)
  {
    inputRaster->getValues(col, line, aux);
    val[col] = aux[0];
  }
}

te::rst::Raster *te::mnt::Shadow::GenerateImage(te::rst::Raster *raster)
{
  double resxi = raster->getResolutionX();
  double resyi = raster->getResolutionY();
  int X1 = raster->getExtent()->getLowerLeftX();
  int Y2 = raster->getExtent()->getUpperRightY();
  te::gm::Coord2D ulc(X1, Y2);
  te::rst::Grid* grid = new te::rst::Grid(m_outputWidth, m_outputHeight, m_resxo, m_resyo, &ulc, m_srid);
  std::vector<te::rst::BandProperty*> bands;
  bands.push_back(new te::rst::BandProperty(0, te::dt::DOUBLE_TYPE, "Shadow Image"));
  bands[0]->m_nblocksx = 1;
  bands[0]->m_nblocksy = (int)m_outputHeight;
  bands[0]->m_blkw = (int)m_outputWidth;
  bands[0]->m_blkh = 1;
  bands[0]->m_colorInterp = te::rst::GrayIdxCInt;

  int nlines = raster->getNumberOfRows();
  int ncols = raster->getNumberOfColumns();

  double pi = 3.1415927;
  double teta = (90. - m_azimuth) * (pi / 180.);
  double phi = (m_elevation * pi) / (double)180;
  double exRelief = m_relief;

  //Set coefficients of the ilumination
  double cx = cos(teta) * cos(phi);
  double cy = sin(teta) * cos(phi);
  double cz = sin(phi);

  double ambi = m_minval + ((m_maxval - m_minval) * 0.2);
  double difu = m_maxval - ((m_maxval - m_minval) * 0.2);

 // te::common::TaskProgress task("Generating shadow image...", te::common::TaskProgress::UNDEFINED, (int)(m_outputHeight*m_outputWidth));

  // create raster
  te::rst::Raster *out = te::rst::RasterFactory::make(m_outRstDSType, grid, bands, m_outDsinfo);
  for (int i = 0; i < out->getNumberOfRows(); i++)
    for (int j = 0; j < out->getNumberOfColumns(); j++)
      out->setValue(j,i, m_dummy);

  std::vector < std::complex<double> > val1(ncols);
  std::vector < std::complex<double> > val2(ncols);
  std::vector < std::complex<double> > val3(ncols);

  for (int line = 0; line < nlines - 2; line++)
  {
    //if (!task.isActive())
    //  throw te::common::Exception(TE_TR("Canceled by user"));
    //task.pulse();

    if (line == 0)
    {
      getRasterElementLine(raster, line, val1);
      getRasterElementLine(raster, line + 1, val2);
      getRasterElementLine(raster, line + 2, val3);
    }
    else
    {
      val1.swap(val2);
      val2.swap(val3);
      getRasterElementLine(raster, line + 2, val3);
    }

    for (int col = 1; col < ncols - 1; col++)
    {
      double val[8];
      val[0] = val1[col - 1].real();
      val[1] = val1[col].real();
      val[2] = val1[col + 1].real();
      val[3] = val2[col - 1].real();
      val[4] = val2[col + 1].real();
      val[5] = val3[col - 1].real();
      val[6] = val3[col].real();
      val[7] = val3[col + 1].real();
      double val0 = val2[col].real();

      if ((val0 >= m_vmin) && (val0 <= m_vmax))
      {
        double dzdx, dzdy;
        if (calcLocalGradient(raster, line, col, m_vmin, m_vmax, dzdx, dzdy, val))
        {
          dzdx = (dzdx * exRelief);
          dzdy = (dzdy * exRelief);

          double d = sqrt((dzdx * dzdx) + (dzdy * dzdy) + 1);
          double costeta = (-(dzdx * cx) - (dzdy * cy) + cz) / d;

          if (costeta < 0)
            costeta = 0;

          double value = ambi + difu * costeta;
          out->setValue(col, line, value);
        }
        else
          out->setValue(col, line, 0);
      }
      else
       out->setValue(col, line, m_dummy);
    } //for (int col = 1; col < ncols - 1; col++)
  } //for (int line = 0; line < nlines; line++)

  val1.clear();
  val2.clear();
  val3.clear();

  return out;
}
