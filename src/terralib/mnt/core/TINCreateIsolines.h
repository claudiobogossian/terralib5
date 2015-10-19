/*!
\file terralib/mnt/core/TINCreateIsolines.h

\brief This file contains a class to generate isolines from a TIN.
Adapted from SPRING

*/

#ifndef __TERRALIB_MNT_INTERNAL_TINCREATEISOLINES_H
#define __TERRALIB_MNT_INTERNAL_TINCREATEISOLINES_H

// Terralib Includes
#include "Config.h"
#include "Tin.h"

#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/datasource/DataSource.h"

namespace te
{
  namespace mnt
  {

    /*!
    \class TINCreateIsolines

    \brief Class to generate isolines from TIN.

    \ingroup mnt

    */
    class TEMNTEXPORT TINCreateIsolines : public Tin
    {
      public:
        TINCreateIsolines() {};

        bool run();

        void setInput(te::da::DataSourcePtr inDsrc,
          std::string inDsetName,
          std::auto_ptr<te::da::DataSetType> inDsetType);

        void setOutput(te::da::DataSourcePtr outDsrc, std::string dsname);

        void setParams(std::vector<double> &val, std::vector<double> &gval, double tol);

        bool SaveIso();

      protected:

        te::da::DataSourcePtr m_inDsrc;
        std::string m_inDsetName;
        std::auto_ptr<te::da::DataSetType> m_inDsetType;

        te::da::DataSourcePtr m_outDsrc;
        std::string m_outDsetName;

        std::vector<double> m_values;
        std::vector<double> m_guidevalues;

        std::vector<te::gm::LineString> m_isolist;

        double m_tol;

    };
  }
}

#endif
