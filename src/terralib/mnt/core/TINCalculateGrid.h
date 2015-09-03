/*!
\file terralib/mnt/core/TINCalculateGrid.h

\brief This file contains a class to calculate a grid from a TIN.
Adapted from SPRING
*/

#ifndef __TERRALIB_MNT_INTERNAL_TINCALCULATEGRID_H
#define __TERRALIB_MNT_INTERNAL_TINCALCULATEGRID_H

// Terralib Includes
#include "Config.h"
#include "Tin.h"

#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/datasource/DataSource.h"

#include "../../raster/Raster.h"

namespace te
{
  namespace mnt
  {
    enum GridType
    {
      Linear,
      QuinticoBrkLine,
      Quintico
    };


    /*!
    \class TINCalculateGrid
    \brief Class to calculate grid from TIN.
    */

    class TEMNTEXPORT TINCalculateGrid : public Tin
    {
    public:
      TINCalculateGrid(){};

      bool run();

      void setInput(te::da::DataSourcePtr inDsrc,
        std::string inDsetName,
        std::auto_ptr<te::da::DataSetType> inDsetType);

      void setOutput(std::map<std::string, std::string> &tgrInfo, std::string dsname);

      void setParams(double resx, double resy, GridType gt);

    protected:

      bool DefineInterLinesColumns(int32_t *nodesid, int32_t &flin, int32_t &llin, int32_t &fcol, int32_t &lcol);

      bool FillGridValue(int32_t triid, int32_t flin, int32_t llin, int32_t fcol, int32_t lcol, double zvalue);

      bool FillGridLinear(int32_t triid, te::gm::PointZ *p3da, int32_t flin, int32_t llin, int32_t fcol, int32_t lcol);

     // bool DefineAkimaCoeficients(int32_t triid, int32_t *nodesid, te::gm::PointZ *p3d, double *coef);

      te::da::DataSourcePtr m_inDsrc;
      std::string m_inDsetName;
      std::auto_ptr<te::da::DataSetType> m_inDsetType;

      std::map<std::string, std::string> m_tgrInfo;
      std::string m_outDsetName;

      double m_resx, m_resy;
      GridType m_gridtype;

      te::rst::Raster* m_rst;
    };
  }
}

#endif
