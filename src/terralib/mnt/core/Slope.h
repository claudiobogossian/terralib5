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

      void setParams(double resx, double resy, char gradtype, char slopetype);

      bool run();

    private:

      /*!
      \brief Method that calculates the lines and the columns intercepted by a triangle
      \param grid is a pointer to a grid object that will be created
      \param nodesid is a vector with nodes identification of the current triangle
      \param flin and llin are the first and the last lines (rows) of the grid
      \param fcol and lcol are the first and the last columns of the grid
      \return true if the gradient grid is filled or false otherwise
      */
      bool DefineInterLinesColuns(int32_t *nodesid, int32_t &flin, int32_t &llin, int32_t &fcol, int32_t &lcol);


      /*!
      \brief Method that calculates a gradient value (slope or aspect) in a given triangle
      \param nodesid is a pointer to the triangle node identifications
      \param gradtype is the type of gradient ('s' slope or 'a' aspect)
      \param slopetype is the type of slope ('g' degree or 'p' percentage)
      \return the calculated gradient z value
      */
      double TriangleGradient(int32_t *nodesid, char gradtype, char slopetype);

    protected:

      te::da::DataSourcePtr m_inDsrc;
      std::string m_inDsetName;
      std::auto_ptr<te::da::DataSetType> m_inDsetType;

      std::map<std::string, std::string> m_dsinfo;
      double m_resx, m_resy;
      char m_gradtype, m_slopetype;
  
      mntType m_inputType;  //!< Input type (TIN, GRID)

      te::rst::Raster* m_rst;

      double m_tol = 0.000001;
    };
  }
}
#endif //__TERRALIB_MNT_INTERNAL_SLOPE_H
