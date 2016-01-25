
/*!
\file terralib/mnt/core/SplineGrassMitasova.h

\brief This file contains a class to calculate retangular grid from Samples using Spline Grass Mitasova Interpolation.
Adapted from SPRING

*/

#ifndef __TERRALIB_MNT_INTERNAL_SPLINEGRASSMITASOVA_H
#define __TERRALIB_MNT_INTERNAL_SPLINEGRASSMITASOVA_H

#include "CalculateGrid.h"

namespace te
{
  namespace mnt
  {

    class TEMNTEXPORT SplineInterpolationGrassMitasova : public CalculateGrid
    {
    public:
      //! Contructors
      SplineInterpolationGrassMitasova(int minPoints = mitasovaMINPOINTS, double tension = mitasovaTENSION, double smooth = mitasovaSMOOTH);

      //! Destructor
      virtual ~SplineInterpolationGrassMitasova();

      void setControlPoints(double xMin, double xMax, double yMin, double yMax, int nMax, double xOrigem, double yOrigem, double dnorm);

      bool calculateGrid();

    private:
      int m_minimoPontos;
      double m_tension;
      double m_smooth;

      int G_ludcmp(std::vector< std::vector<double> > &, int, std::vector<int> &, double &);
      void G_lubksb(std::vector< std::vector<double> > &a, int n, std::vector<int> &indx, std::vector<double> &b);
      double amax1(double arg1, double arg2);
      double amin1(double arg1, double arg2);
      double IL_crst(double r, double fi);
      int IL_matrix_create(std::vector< std::vector<double> > &matrix, std::vector<int> &indx, double fi, double rsm, int KMAX2);
      bool IL_grid_calc(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax, int KMAX2, std::vector<double>& b, double fi, double dnorm);
      int IL_check_at_points_2d(std::vector<double> &b, double &ertot, double zmin, double dnorm, double fi);

      std::vector<te::gm::PointZ> m_pointsRegion;	//!< Control Points of grid region

    }; //class SplineInterpolationGrassMitasova
  } //mnt
} //te

#endif
