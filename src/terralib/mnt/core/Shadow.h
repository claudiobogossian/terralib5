/*!
\file terralib/mnt/core/Shadow.h

\brief This file contains a class to shadow image generation.
Adapted from SPRING
*/

#ifndef __TERRALIB_MNT_INTERNAL_SHADOW_H
#define __TERRALIB_MNT_INTERNAL_SHADOW_H

// Terralib Includes
#include "Config.h"
#include "Enums.h"

#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/datasource/DataSource.h"

namespace te
{
  namespace mnt
  {
    class TEMNTEXPORT Shadow
    {
    public:
      Shadow();
      ~Shadow();

      void setInput(te::da::DataSourcePtr inDsrc,
        std::string inDsetName,
        const te::da::DataSetTypePtr& inDsetType);

      void setOutput(std::map<std::string, std::string> &dsinfo, std::string outRstDSType);

      void setParams(double azimuth, double elevation, double relief, double dummy, 
        double vmin, double vmax, double minval, double maxval,
        unsigned int outputWidth, unsigned int outputHeight, double resxo, double resyo);

      /*! Function used to set the Spatial Reference System ID  */
      void setSRID(int srid) { m_srid = srid; }

      bool run();

      te::rst::Raster *getOutRst() { return m_out; }

      te::rst::Raster *GenerateImage(te::rst::Raster *raster);

    private:

      bool calcLocalGradient(te::rst::Raster* inputRaster, int line, int col, double vmin, double vmax, double& dx, double& dy, double *val);
      void getRasterElementLine(te::rst::Raster* inputRaster, int line, std::vector< std::complex<double> > &val);

    protected:

      te::da::DataSourcePtr m_inDsrc;
      std::string m_inDsetName;
      te::da::DataSetTypePtr m_inDsetType;
      std::unique_ptr<te::da::DataSet> inDset;

      std::map<std::string, std::string> m_outDsinfo;
      std::string m_outRstDSType;
      te::rst::Raster* m_out;

      int m_srid;                                  //!< Attribute with spatial reference information

      double m_azimuth;
      double m_elevation;
      double m_relief;
      double m_vmin, m_vmax;
      double m_minval;
      double m_maxval;

      double m_dummy;
      unsigned int m_outputWidth;
      unsigned int m_outputHeight;

      double m_resxo, m_resyo;
    };
  }
}
#endif //__TERRALIB_MNT_INTERNAL_SMOOTH_H
