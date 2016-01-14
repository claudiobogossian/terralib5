/*!
\file terralib/mnt/core/Volume.h

\brief This file contains a class to calculate volume.
Adapted from SPRING
*/

#ifndef __TERRALIB_MNT_INTERNAL_VOLUME_H
#define __TERRALIB_MNT_INTERNAL_VOLUME_H

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
    class TEMNTEXPORT Volume
    {
    public:
      Volume();
      ~Volume();

      void setInput(te::da::DataSourcePtr inrstDsrc,
        std::string inrstDsetName,
        std::auto_ptr<te::da::DataSetType> inrstDsetType,
        te::da::DataSourcePtr invecDsrc,
        std::string invecDsetName,
        std::auto_ptr<te::da::DataSetType> invecDsetType,
        const te::da::ObjectIdSet* OidSet);

      void setParams(double quota, std::string &attr, double dummy);

      /*! Function used to set the Spatial Reference System ID  */
      void setSRID(int srid) { m_srid = srid; }

      bool run();

      void getResults(std::vector<std::string> &polyvec, 
        std::vector<std::string> &cortevec,
        std::vector<std::string> &aterrovec,
        std::vector<std::string> &areavec,
        std::vector<std::string> &iquotavec);

    protected:

      // Find the polygon bounding box.
      bool DefLC(te::gm::Polygon *pol, int& flin, int& llin, int& fcol, int& lcol);

      bool CVGrd(te::gm::Polygon *pol, int flin, int llin, int fcol, int lcol, std::string &polid);

      te::da::DataSourcePtr m_inrstDsrc;
      std::string m_inrstDsetName;
      std::auto_ptr<te::da::DataSetType> m_inrstDsetType;

      te::da::DataSourcePtr m_invecDsrc;
      std::string m_invecDsetName;
      std::auto_ptr<te::da::DataSetType> m_invecDsetType;

      const te::da::ObjectIdSet* m_OidSet;

      std::auto_ptr<te::rst::Raster> m_raster;

      double m_quota;
      std::string m_attr;
      double m_dummy;

      int m_srid;                                  //!< Attribute with spatial reference information

      std::vector<std::string> m_polyvec;       //!< Attribute with structure to save results
      std::vector<std::string> m_cortevec;      //!< Attribute with structure to save results
      std::vector<std::string> m_aterrovec;     //!< Attribute with structure to save results
      std::vector<std::string> m_areavec;       //!< Attribute with structure to save results
      std::vector<std::string> m_iquotavec;     //!< Attribute with structure to save results

    };
  }
}
#endif //__TERRALIB_MNT_INTERNAL_VOLUME_H
