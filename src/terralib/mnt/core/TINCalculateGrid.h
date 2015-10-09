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

    /*!
    \class TINCalculateGrid
    \brief Class to calculate grid from TIN.

    \ingroup mnt

    */

    class TEMNTEXPORT TINCalculateGrid : public Tin
    {
    public:
      TINCalculateGrid(){};
      ~TINCalculateGrid();

      bool run();
      
      /*!
      \brief It sets the Datasource that is being used to generate TIN.
      \param inDsrc The datasource being used.
      \param inDsetName datasource name
      \param inDsetType input DataSetType
      */
      void setInput(te::da::DataSourcePtr inDsrc,
        std::string inDsetName,
        std::auto_ptr<te::da::DataSetType> inDsetType);

      /*!
      \brief It sets the Datasource that is being used to save TIN.
      */
      void setOutput(std::map<std::string, std::string> &dsinfo);

      /*!
      \brief It sets the parameters that is being used to save TIN.
      \param resx, resy resolution X and Y.
      \param gt interpolator
      */
      void setParams(double resx, double resy, Interpolator gt);

    protected:

      bool DefineInterLinesColumns(int32_t *nodesid, int32_t &flin, int32_t &llin, int32_t &fcol, int32_t &lcol);

      bool FillGridValue(int32_t triid, int32_t flin, int32_t llin, int32_t fcol, int32_t lcol, double zvalue);

      bool FillGridLinear(int32_t triid, te::gm::PointZ *p3da, int32_t flin, int32_t llin, int32_t fcol, int32_t lcol);

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
      bool FillGridQuintic(int32_t triid, te::gm::PointZ *p3da, int32_t flin, int32_t llin, int32_t fcol, int32_t lcol, double *coef);

      te::da::DataSourcePtr m_inDsrc;
      std::string m_inDsetName;
      std::auto_ptr<te::da::DataSetType> m_inDsetType;

      std::map<std::string, std::string> m_dsinfo;

      double m_resx, m_resy;
      Interpolator m_gridtype;

      te::rst::Raster* m_rst;
    };
  }
}

#endif
