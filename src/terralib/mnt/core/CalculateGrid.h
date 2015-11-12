
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

      std::auto_ptr<te::rst::Raster> Initialize(bool spline, int &nro_neighb, double &rx1, double &ry2, int &outputWidth, int &outputHeight);

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

    class TEMNTEXPORT SplineInterpolationGrass : public CalculateGrid
    {
    public:
      //! Contructors
      SplineInterpolationGrass(int nPartsX, int nPartsY, int minPoints, double overlappin) :
        m_nPartsX(nPartsX), m_nPartsY(nPartsY), m_minpoints(minPoints), m_overlapping(overlappin) {}

      //! Destructor
      ~SplineInterpolationGrass(){}

      void setControlPoints(double xMin, double yMin);
      int  nPointInterest() { return m_npoints; }

      void initInterpolation(int beginLine, int endLine, int beginCol, int endCol);

      bool calculateGrid(double xMin, double yMin);
      bool generateGrid();

      double pointsControlMean() { return m_mean; }

      double dataInterpolateBicubic(double x, double y, double xMin, double yMin, bool &interp);

      double dataInterpolateBilin(double x, double y, double xMin, double yMin, bool &interp);

      static te::gm::LineString* pointListSimplify(te::gm::LineString *ls, double snap, double Zvalue);

    private:

      /* GRASS */
      /*!
      * \brief Ordinate node index computation
      */
      void node_x(double x, int &i_x, double &csi_x, double xMin, double deltaX)
      {
        i_x = (int)((x - xMin) / deltaX);
        csi_x = (double)((x - xMin) - (i_x * deltaX));
      }

      /*!
      * \brief Ordinate node index computation
      */
      void node_y(double y, int &i_y, double &csi_y, double yMin, double deltaY)
      {
        i_y = (int)((y - yMin) / deltaY);
        csi_y = (double)((y - yMin) - (i_y * deltaY));
      }

      /*!
      * \brief  Node order computation
      */
      int order(int i_x, int i_y, int yNum){ return (i_y + i_x * yNum); }

      /*!
      * \brief Design matrix coefficients computation
      */
      double phi_3(double csi) { return ((pow(2 - csi, 3.) - pow(1 - csi, 3.) * 4) / 6.); }
      /*!
      * \brief Design matrix coefficients computation
      */
      double phi_4(double csi){ return (pow(2 - csi, 3.) / 6.); }
      /*!
      * \brief Design matrix coefficients computation
      */
      double phi_33(double csi_x, double csi_y){ return (phi_3(csi_x) * phi_3(csi_y)); }
      /*!
      * \brief Design matrix coefficients computation
      */
      double phi_34(double csi_x, double csi_y){ return (phi_3(csi_x) * phi_4(csi_y)); }
      /*!
      * \brief Design matrix coefficients computation
      */
      double phi_43(double csi_x, double csi_y){ return (phi_4(csi_x) * phi_3(csi_y)); }
      /*!
      * \brief Design matrix coefficients computation
      */
      double phi_44(double csi_x, double csi_y){ return (phi_4(csi_x) * phi_4(csi_y)); }
      /*!
      * \brief Design matrix coefficients computation
      */
      double phi(double csi_x, double csi_y){ return ((1 - csi_x)*(1 - csi_y)); }

      /*tcholBand*/
      bool tcholSolve();
      bool tcholDec(std::vector< std::vector<double> > &);

      /*interpSpline*/
      void nCorrectGrad();

      void normalDefBilin(double xMin, double yMin);

      void normalDefBicubic(double xMin, double yMin);


      /*!
      \brief METHOD TO ADJUST A LINEAR CURVE TO A SET OF LINE POINTS
      \param ptpol is a pointer to a PointList
      \param maxDist is the maximum distance to the line fitted for a set of points
      \return FALSE if there is no memory available, the number of points are lower than 2 or maxDist == 0.0
      \return TRUE if the curve is adjusted with no errors
      */
      static bool AdjustLinear(te::gm::LineString *ptol, double maxDist);

    protected:
      int m_minpoints;           //!< minimum of points considered
      int m_nPartsX, m_nPartsY;  //!< number of parts considered in the x and y directions
      double m_overlapping;         //!< overlap value considered
      int m_nsplx;               //!< Numero de colunas do spline
      int m_nsply;               //!< Numero de linhas do spline
      int m_nparameters;
      int m_BW;
      double m_mean; //!< Media da area local
      int m_npoints; //!< Numero de pontos de interesse
      double m_passoWidth, m_passoHeight, m_lambda;
      std::vector<te::gm::PointZ> m_obsVect; //!< Interpolation and least-square vectors
      std::vector<double> m_TN, m_parVect; //!< Interpolating and least-square vectors
      std::vector< std::vector<double> > m_N; //!< Interpolation and least-square matrix


    }; // class SplineInterpolationGrass


  } //mnt
} //te

#endif
