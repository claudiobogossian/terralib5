/*!
\file terralib/mnt/core/Smooth.h

\brief This file contains a class to isolines smooth.
Adapted from SPRING
*/

#ifndef __TERRALIB_MNT_INTERNAL_SMOOTH_H
#define __TERRALIB_MNT_INTERNAL_SMOOTH_H

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
    class TEMNTEXPORT Smooth
    {
    public:
      Smooth();
      ~Smooth();

      void setInput(te::da::DataSourcePtr inDsrc,
        std::string inDsetName,
        std::auto_ptr<te::da::DataSetType> inDsetType);

      void setOutput(te::da::DataSourcePtr outDsrc, std::string dsname);

      void setParams(double factor, double max_dist, bool simpl, std::string &attr);

      /*! Function used to set the Spatial Reference System ID  */
      void setSRID(int srid) { m_srid = srid; }

      bool run();

    private:
      bool AdjustCatmullRom(te::gm::LineString &iso, double snap);

      bool middlePointWithSnap(te::gm::Coord2D &pfr, te::gm::Coord2D &pto, te::gm::Coord2D &pt0, te::gm::Coord2D &pt1, double d);

    protected:

      te::da::DataSourcePtr m_inDsrc;
      std::string m_inDsetName;
      std::auto_ptr<te::da::DataSetType> m_inDsetType;

      te::da::DataSourcePtr m_outDsrc;
      std::string m_outDsetName;

      int m_srid;                                  //!< Attribute with spatial reference information
      double m_factor;
      double m_maxdist;
      bool m_simpl_out;
      std::string m_Zattr;
    };
  }
}
#endif //__TERRALIB_MNT_INTERNAL_SMOOTH_H
