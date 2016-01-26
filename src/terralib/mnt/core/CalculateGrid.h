
/*!
\file terralib/mnt/core/CalculateGrid.h

\brief This file contains a class to calculate retangular grid from Samples.
Adapted from SPRING

*/

#ifndef __TERRALIB_MNT_INTERNAL_CALCULATEGRID_H
#define __TERRALIB_MNT_INTERNAL_CALCULATEGRID_H

// Terralib Includes
#include "Config.h"
#include "Enums.h"

#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/datasource/DataSource.h"

#include "../../raster/Raster.h"
#include "../../sam.h"


typedef te::sam::kdtree::AdaptativeNode<te::gm::Coord2D, std::vector<te::gm::PointZ>, te::gm::PointZ> KD_ADAPTATIVE_NODE;
typedef te::sam::kdtree::AdaptativeIndex<KD_ADAPTATIVE_NODE> KD_ADAPTATIVE_TREE;

//MITASOVA PARAMETERS
#define mitasovaMINPOINTS 50
#define mitasovaTENSION   40.
#define mitasovaSMOOTH    0.100000

namespace te
{
  namespace mnt
  {

    class TEMNTEXPORT CalculateGrid
    {
    public:
      /*! \brief Default constructor. */
      CalculateGrid();

      ~CalculateGrid();

      /*!
      \brief Calculate GRID
      \ return true or false.
      */
      bool run();

      /*!
      \brief It sets the Datasource that is being used to generate TIN.
      \param inDsrc The datasource being used.
      \param inDsetName datasource name
      \param inDsetType input DataSetType
      \param type Input type: Sample or Isoline
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
      void setParams( const std::string &attr, double resx, double resy, Interpolator gt, double rad, int pow);

      /*! Function used to set the Spatial Reference System ID  */
      void setSRID(int srid);

      /*! Function used to set the envelope parameter */
      void setEnvelope(te::gm::Envelope &env);

      std::auto_ptr<te::rst::Raster> Initialize(bool spline, unsigned int &nro_neighb, double &rx1, double &ry2, unsigned int &outputWidth, unsigned int &outputHeight);

      /*!
      \brief Interpolates the z value of the pg point3d.
      \ This method interpolates the z value of the pg point3d using the
      \ information on the current partition, the dist double vector and
      \param pg: points to an point3d whose z value must be calculated
      \param points: vector that contains points from point pg to the partition points
      \param distq: points to a double vector that contains distance values from point pg to the partition points.
     */
      void Interpolation(te::gm::PointZ& pg, std::vector<te::gm::PointZ>& points, std::vector<double>& distq);

      /*!
      \brief Interpolates the z value of the pg point3d using an weighted average by quadrant.
      \ Implements the interpolator that uses the average, weighted by the inverse
      \ square distance, of the nearest neighbours by quadrant.*/
      double Interpwq(te::gm::PointZ& pg, std::vector<te::gm::PointZ>& points, std::vector<double>& distq);

      /*!
      \brief Interpolates the z value of the pg point3d using an weighted average by quadrant and by z values.
     \ Implements the interpolator tha uses the average, weighted by the inverse
      \ square distance, of the nearest neighbours by z values and by quadrant. 
      \ This method accepts no repeated z values in the interpolation.*/
      double Interpwqz(te::gm::PointZ& pg, std::vector<te::gm::PointZ>& points, std::vector<double>& distq);

      /*!
      \brief Method to calculate a grid from a vector of samples using a Spline (GRASS) fitting algorithm
      \return true when the z grid value is succesfully evaluated
      \return false when a z grid value could not be evaluated or some error is encountered
      */
      bool GenerateGridSplineGrass();

      /*!
      \brief Method to calculate a grid from a vector of samples using a Spline (Mitasova) fitting algorithm
      \return true when the z grid value is succesfully evaluated
      \return false when a z grid value could not be evaluated or some error is encountered
      */
      bool GenerateGridSplineMitasova();

    protected:

      int m_srid;                                  //!< Attribute with spatial reference information
      te::gm::Envelope m_env;                      //!< Attribute used to restrict the area to generate the raster.

      te::da::DataSourcePtr m_inDsrc;      //!< 
      std::string m_inDsetName;      //!< 
      std::auto_ptr<te::da::DataSetType> m_inDsetType;      //!< 

      std::string m_attrZ;      //!< Attribute used to calculate grid

      std::map<std::string, std::string> m_dsinfo;      //!< 

      double m_resx, m_resy;      //!< grid resolution in X and Y
      Interpolator m_inter;       //!< selected interpolation method
      double m_radius;            //!< radius
      int m_power;                //!< power
      int m_tension;             //!< the tension spline factor considered in the spline fitting process
      int m_smoothM;             //!< smooth factor used

      te::rst::Raster* m_rst;      //!< 

      double m_tolerance;      //!< tolerance used to simplify lines
      double m_nodatavalue;    //!< no data value

      KD_ADAPTATIVE_TREE *m_adaptativeTree;
      std::vector<std::pair<te::gm::Coord2D, te::gm::PointZ> > m_dataset;      //!< 

    }; //class CalculateGrid

  } //mnt
} //te

#endif
