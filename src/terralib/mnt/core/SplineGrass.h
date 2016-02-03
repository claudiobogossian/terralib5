
/*!
\file terralib/mnt/core/SplineGrass.h

\brief This file contains a class to calculate retangular grid from Samples using Spline Grass Interpolation.
Adapted from SPRING

*/

#ifndef __TERRALIB_MNT_INTERNAL_SPLINEGRASS_H
#define __TERRALIB_MNT_INTERNAL_SPLINEGRASS_H

#include "CalculateGrid.h"

namespace te
{
  namespace mnt
  {

    class TEMNTEXPORT SplineInterpolationGrass : public CalculateGrid
    {
    public:
      //! Contructors
      SplineInterpolationGrass(unsigned int nPartsX, unsigned int nPartsY, unsigned int minPoints, double overlappin) :
        m_nPartsX(nPartsX), m_nPartsY(nPartsY), m_minpoints(minPoints), m_overlapping(overlappin) {}

      //! Destructor
      ~SplineInterpolationGrass(){}

      void setControlPoints(double xMin, double yMin);
      unsigned int  nPointInterest() { return m_npoints; }

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
      int order(int i_x, int i_y, unsigned int yNum){ return (i_y + i_x * (int)yNum); }

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
      unsigned int m_nPartsX, m_nPartsY;  //!< number of parts considered in the x and y directions
      unsigned int m_minpoints;           //!< minimum of points considered
      double m_overlapping;         //!< overlap value considered
      unsigned int m_nsplx;               //!< Numero de colunas do spline
      unsigned int m_nsply;               //!< Numero de linhas do spline
      unsigned int m_nparameters;
      unsigned int m_BW;
      double m_mean; //!< Media da area local
      unsigned int m_npoints; //!< Numero de pontos de interesse
      double m_passoWidth, m_passoHeight, m_lambda;
      std::vector<te::gm::PointZ> m_obsVect; //!< Interpolation and least-square vectors
      std::vector<double> m_TN, m_parVect; //!< Interpolating and least-square vectors
      std::vector< std::vector<double> > m_N; //!< Interpolation and least-square matrix

    }; // class SplineInterpolationGrass

  } //mnt
} //te

#endif
