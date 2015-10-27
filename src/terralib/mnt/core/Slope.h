/*!
\file terralib/mnt/core/Slope.h

\brief This file contains a class to generate a Slope grid.
Adapted from SPRING
*/

#ifndef __TERRALIB_MNT_INTERNAL_SLOPE_H
#define __TERRALIB_MNT_INTERNAL_SLOPE_H

// Terralib Includes
#include "Config.h"
#include "Enums.h"
#include "Tin.h"


#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/datasource/DataSource.h"

namespace te
{
  namespace mnt
  {
    class TEMNTEXPORT Slope : public Tin
    {
    public:
      Slope();
      ~Slope();

      void setInput(te::da::DataSourcePtr inDsrc,
        std::string inDsetName,
        std::auto_ptr<te::da::DataSetType> inDsetType);

      void setOutput(std::map<std::string, std::string> &dsinfo);

      void setParams(double resx, double resy, char gradtype, char slopetype, int srid, double dummy);

      bool run();

    private:

      /*!
      \brief Method that calculates a gradient value (slope or aspect) in a given triangle
      \param nodesid is a pointer to the triangle node identifications
      \param gradtype is the type of gradient ('s' slope or 'a' aspect)
      \param slopetype is the type of slope ('g' degree or 'p' percentage)
      \return the calculated gradient z value
      */
      double TriangleGradient(int32_t *nodesid, char gradtype, char slopetype);

      /*!
      \brief METHOD TO CALCULATE GRADIENT VECTOR COMPONENTS IN THE POINT L,C OF THE GRID
      \param raster input grid
      \param l and c are the line and column of the input grid
      \param dzdx and dzdy are values of the first derivatives of z in the x and y directions at location l,c
      \return true when the gradient is evaluated
      \return false when the gradient could not be evaluated
      */
      bool te::mnt::Slope::CalcGradientRst(std::auto_ptr<te::rst::Raster> &raster, unsigned l, unsigned c, double& dzdx, double& dzdy);

    protected:

      te::da::DataSourcePtr m_inDsrc;
      std::string m_inDsetName;
      std::auto_ptr<te::da::DataSetType> m_inDsetType;

      std::map<std::string, std::string> m_dsinfo;
      char m_gradtype, m_slopetype;
  
      mntType m_inputType;  //!< Input type (TIN, GRID)

      double m_tol = 0.000001;
      double m_min, m_max;
      double m_dx, m_dy;
      double m_dummy;
    };
  }
}
#endif //__TERRALIB_MNT_INTERNAL_SLOPE_H
