/*!
\file terralib/mnt/core/SplineGrassMitasova.cpp

\brief This file contains a class to calculate a grid from Sample using spline grass mitasova interpolator.

*/

#include "SplineGrassMitasova.h"
#include "Utils.h"

#include "../../common/progress/TaskProgress.h"

#include <algorithm>
#include <vector>


te::mnt::SplineInterpolationGrassMitasova::SplineInterpolationGrassMitasova(int minPoints, double tension, double smooth)
{
  m_minimoPontos = minPoints * 2;
  m_minimoPontos = m_minimoPontos;
  m_tension = tension;
  m_smooth = smooth;
}

te::mnt::SplineInterpolationGrassMitasova::~SplineInterpolationGrassMitasova()
{

}

void te::mnt::SplineInterpolationGrassMitasova::setControlPoints(double xMin, double xMax, double yMin, double yMax, int nMax, double xOrigem, double yOrigem, double dnorm)
{
  std::size_t nVectorPoints = m_dataset.size();

  m_pointsRegion.clear();
  for (std::size_t  i = 0; i<nVectorPoints; i++)
  {
    te::gm::PointZ pp3d = m_dataset.at(i).second;
    if ((pp3d.getX() >= xMin) && (pp3d.getX() < xMax) && (pp3d.getY() >= yMin) && (pp3d.getY() < yMax))
    {
      //update new point
      double nX = (pp3d.getX() - xOrigem) / dnorm;
      double nY = (pp3d.getY() - yOrigem) / dnorm;
      te::gm::PointZ npp3d(nX, nY, pp3d.getZ());

      //add a new point
      m_pointsRegion.push_back(npp3d);

      if (m_pointsRegion.size() == nMax)
        break;
    }
  }
}

double te::mnt::SplineInterpolationGrassMitasova::amax1(double arg1, double arg2)
{
  double res;

  if (arg1 >= arg2)
  {
    res = arg1;
  }
  else
  {
    res = arg2;
  }
  return res;
}

double te::mnt::SplineInterpolationGrassMitasova::amin1(double arg1, double arg2)
{
  double res;

  if (arg1 <= arg2)
  {
    res = arg1;
  }
  else
  {
    res = arg2;
  }
  return res;
}

double te::mnt::SplineInterpolationGrassMitasova::IL_crst(double r, double fi)
/*
* Radial basis function - completely regularized spline with
* tension (d=2)
*/
{
  double rfsta2 = fi * fi * r / 4.;

  static double c[4] = { 8.5733287401, 18.0590169730, 8.6347608925,
    0.2677737343 };
  static double b[4] = { 9.5733223454, 25.6329561486, 21.0996530827,
    3.9584969228 };
  double ce = 0.57721566;

  static double u[10] = { 1.e+00, -.25e+00,
    .055555555555556e+00, -.010416666666667e+00, /*fixed bug 415.. repl. by 416..*/
    .166666666666667e-02, -2.31481481481482e-04,
    2.83446712018141e-05, -3.10019841269841e-06,
    3.06192435822065e-07, -2.75573192239859e-08 };
  double x = rfsta2;
  double res;

  double e1, ea, eb;

  if (x < 1.e+00)
  {
    res = x * (u[0] + x * (u[1] + x * (u[2] + x * (u[3] + x * (u[4] + x *
      (u[5] + x * (u[6] + x * (u[7] + x * (u[8] + x * u[9])))))))));
    return (res);
  }

  if (x > 25.e+00)
    e1 = 0.00;
  else
  {
    ea = c[3] + x * (c[2] + x * (c[1] + x * (c[0] + x)));
    eb = b[3] + x * (b[2] + x * (b[1] + x * (b[0] + x)));
    e1 = (ea / eb) / (x * exp(x));
  }
  res = e1 + ce + log(x);
  return (res);
}

void te::mnt::SplineInterpolationGrassMitasova::G_lubksb(std::vector< std::vector<double> > &a, int n, std::vector<int> &indx, std::vector<double> &b)
{
  int i, ii, ip, j;
  double sum;

  ii = -1;
  for (i = 0; i<n; i++)
  {
    ip = indx[i];
    sum = b[ip];
    b[ip] = b[i];
    if (ii >= 0)
      for (j = ii; j<i; j++)
        sum -= a[i][j] * b[j];
    else if (sum)
      ii = i;
    b[i] = sum;
  }
  for (i = n - 1; i >= 0; i--)
  {
    sum = b[i];
    for (j = i + 1; j<n; j++)
      sum -= a[i][j] * b[j];
    b[i] = sum / a[i][i];
  }
}

#define TINY 1.0e-20;

int te::mnt::SplineInterpolationGrassMitasova::G_ludcmp(std::vector< std::vector<double> > &a, int n, std::vector<int> &indx, double &d)
{
  int i, imax = 0, j, k;
  double big, dum, sum, temp;
  std::vector<double> vv;

  vv.resize(n);
  d = 1.0;
  for (i = 0; i<n; i++)
  {
    big = 0.0;
    for (j = 0; j<n; j++)
      if ((temp = fabs(a[i][j])) > big)
        big = temp;
    if (big == 0.0)
    {
      d = 0.0;
      vv.clear();
      return 0; /* Singular matrix  */
    }
    vv[i] = 1.0 / big;
  }
  for (j = 0; j<n; j++)
  {
    for (i = 0; i<j; i++)
    {
      sum = a[i][j];
      for (k = 0; k<i; k++)
        sum -= a[i][k] * a[k][j];
      a[i][j] = sum;
    }
    big = 0.0;
    for (i = j; i<n; i++)
    {
      sum = a[i][j];
      for (k = 0; k<j; k++)
        sum -= a[i][k] * a[k][j];
      a[i][j] = sum;
      if ((dum = vv[i] * fabs(sum)) >= big)
      {
        big = dum;
        imax = i;
      }
    }
    if (j != imax)
    {
      for (k = 0; k<n; k++)
      {
        dum = a[imax][k];
        a[imax][k] = a[j][k];
        a[j][k] = dum;
      }
      d = -(d);
      vv[imax] = vv[j];
    }
    indx[j] = imax;
    if (a[j][j] == 0.0)
      a[j][j] = TINY;
    if (j != n)
    {
      dum = 1.0 / (a[j][j]);
      for (i = j + 1; i<n; i++)
        a[i][j] *= dum;
    }
  }
  vv.clear();
  return 1;
}

#undef TINY

int te::mnt::SplineInterpolationGrassMitasova::IL_matrix_create(
  std::vector< std::vector <double> > &matrix,           /* matrix */
  std::vector<int> &indx, double fi, double rsm, int KMAX2
  )
  /*
  Creates system of linear equations represented by matrix using given points
  and interpolating function interp()
  */
{
  double xx, yy;
  double rfsta2, r;
  double d;
  double fstar2 = fi * fi / 4.;
  std::vector<double> A;
  double RO, amaxa;
  std::size_t i, i1, j, k, k1, k2, l, m;

  std::size_t n_points = m_pointsRegion.size();

  std::size_t n1 = n_points + 1;

  /*
  C
  C      GENERATION OF MATRIX
  C
  C      FIRST COLUMN
  C
  */
  for (k = 0; k < ((KMAX2 + 2)*(KMAX2 + 2)); ++k)
    A.push_back(0.);

  for (k = 1; k <= n_points; k++)
  {
    i1 = k + 1;
    A[i1] = 1.;
  }
  /*
  C
  C      OTHER COLUMNS
  C
  */
  RO = -rsm;

  for (k = 1; k <= n_points; k++)
  {
    k1 = k * n1 + 1;
    k2 = k + 1;
    i1 = k1 + k;
    if (rsm >= 0.) /*indicates variable smoothing */
    {
      A[i1] = RO; /* constant smoothing*/
    }

    for (l = k2; l <= n_points; l++)
    {
      te::gm::PointZ pp3dK = m_pointsRegion[k - 1];
      te::gm::PointZ pp3dL = m_pointsRegion[l - 1];

      xx = pp3dK.getX() - pp3dL.getX();
      yy = pp3dK.getY() - pp3dL.getY();

      {
        r = xx*xx + yy*yy;
        rfsta2 = fstar2 * (xx * xx + yy * yy);
      }

      i1 = k1 + l;
      A[i1] = IL_crst(r, fi);
    }
  }
  /*
  C
  C       SYMMETRISATION
  C
  */
  amaxa = 1.;
  for (k = 1; k <= n1; k++)
  {
    k1 = (k - 1) * n1;
    k2 = k + 1;
    for (l = k2; l <= n1; l++)
    {
      m = (l - 1) * n1 + k;
      A[m] = A[k1 + l];
      amaxa = amax1(A[m], amaxa);
    }
  }
  m = 0;
  for (i = 0; i <= n_points; i++) {
    for (j = 0; j <= n_points; j++) {
      m++;
      matrix[i][j] = A[m];
    }
  }

  if (G_ludcmp(matrix, (int)n_points + 1, indx, d) <= 0)
  { /* find the inverse of the matrix */
    A.clear();
    return -1;
  }

  A.clear();
  return 1;
}

bool te::mnt::SplineInterpolationGrassMitasova::calculateGrid()
{
  unsigned int nro_neighb;
  double rx1, ry2;
  unsigned int NCols;
  unsigned int NLines;

  std::auto_ptr<te::rst::Raster> rst = Initialize(true, nro_neighb, rx1, ry2, NCols, NLines);

  double deltx = m_env.getUpperRightX() - m_env.getLowerLeftX();
  double delty = m_env.getUpperRightY() - m_env.getLowerLeftY();

  //divide em x partes overlaping
  int nPartsX = 50;
  int nPartsY = 50;
  double ew_region = deltx / (double)nPartsX;
  double ns_region = delty / (double)nPartsY;

  double X1 = m_env.getLowerLeftX();
  double Y1 = m_env.getLowerLeftY();

  double percentageDist = 0.5;

  int KMAX2 = 2 * 2 * m_minimoPontos;
  double fi = m_tension;
  double rsm = m_smooth;

  std::size_t NPOINT = m_dataset.size();
  double dnorm = sqrt((deltx * delty * m_minimoPontos) / NPOINT);

  std::size_t nallPoints = m_dataset.size();
  te::gm::PointZ pp03d = m_dataset[0].second;
  double zmin = pp03d.getZ();
  double zmax = pp03d.getZ();
  for (std::size_t i = 1; i<nallPoints; i++)
  {
    te::gm::PointZ pp3d = m_dataset[i].second;
    zmin = amin1(zmin, pp3d.getZ());
    zmax = amax1(zmax, pp3d.getZ());
  }

  int nsteps = (int)(nPartsY*nPartsX + 1);
  te::common::TaskProgress task("Generating DTM...", te::common::TaskProgress::UNDEFINED, nsteps);

  for (int i = 0; i < nPartsY; i++)
  {
    double begin_y = Y1 + (i*ns_region);
    double end_y = Y1 + ((i + 1)*ns_region);

    double disty = (end_y - begin_y) * percentageDist;

    for (int j = 0; j < nPartsX; j++)
    {
      task.pulse();

      //computes the beginning and end area
      double begin_x = X1 + (j*ew_region);
      double end_x = X1 + ((j + 1)*ew_region);

      double distx = (end_x - begin_x) * percentageDist;

      int nOverlaping = 1;
      bool poucosPontos = false;
      do
      {
        double overlapDistX = distx*nOverlaping;
        double overlapDistY = disty*nOverlaping;
        //adds control points and the overlaping area
        setControlPoints(begin_x - overlapDistX, end_x + overlapDistX, begin_y - overlapDistY, end_y + overlapDistY, KMAX2, begin_x, begin_y, dnorm);

        poucosPontos = m_pointsRegion.size() < m_minimoPontos;
        nOverlaping++;

      } while (poucosPontos);

      std::vector<int> indx;
      std::vector< std::vector<double> > matrix;
      std::vector<double> b;

      indx.resize(KMAX2 + 1);
      for (int ii = 0; ii < KMAX2 + 1; ++ii)
      {
        std::vector<double> maux;
        maux.resize(KMAX2 + 1);
        matrix.push_back(maux);
      }
      b.resize(KMAX2 + 3);

      //create matrix
      if (IL_matrix_create(matrix, indx, fi, rsm, KMAX2) < 0)
      {
        b.clear();
        indx.clear();
        for (int ii = 0; ii < KMAX2 + 1; ++ii)
          matrix[ii].clear();
        matrix.clear();
        return false;
      }

      std::size_t n_points = m_pointsRegion.size();
      for (std::size_t j = 0; j < n_points; j++)
      {
        te::gm::PointZ pp3d = m_pointsRegion.at(j);
        b[j + 1] = pp3d.getZ();
      }
      //computes the matrix
      for (size_t ib = 0; ib < size_t(KMAX2 + 3); ++ib)
        b.push_back(0);

      G_lubksb(matrix, (int)n_points + 1, indx, b);
      double errotot = 0.;
      IL_check_at_points_2d(b, errotot, zmin, dnorm, fi);

      indx.clear();
      for (unsigned ii = 0; ii < matrix.size(); ++ii)
        matrix[ii].clear();
      matrix.clear();

      //Computes the grid
      if (!IL_grid_calc(begin_x, end_x, begin_y, end_y, zmin, zmax, KMAX2, b, fi, dnorm))
      {
        b.clear();
        return false;
      }
      b.clear();
    }
  }

  rst.release();
  return true;
}

int te::mnt::SplineInterpolationGrassMitasova::IL_check_at_points_2d(
  std::vector<double>& b,			/* solution of linear equations */
  double &ertot,		/* total error */
  double zmin,			/* min z-value */
  double dnorm, double fi
  )

  /*
  * Checks if interpolating function interp() evaluates correct z-values at
  * given points. If smoothing is used calculate the maximum error caused
  * by smoothing.
  */

{
  size_t n_points = m_pointsRegion.size();/* number of points */
  double rfsta2, errmax, h, xx, yy, r2, hz, zz, err, r;
  size_t n1, mm, m;
  double fstar2;

  fstar2 = fi * fi / 4.;
  errmax = .0;
  n1 = n_points + 1;
  for (mm = 1; mm <= n_points; mm++)
  {
    te::gm::PointZ pp3dMM = m_pointsRegion.at(mm - 1);
    h = b[0];
    for (m = 1; m <= n_points; m++)
    {
      te::gm::PointZ pp3dM = m_pointsRegion.at(m - 1);
      xx = pp3dMM.getX() - pp3dM.getX();
      yy = pp3dMM.getY() - pp3dM.getY();

      r2 = yy * yy + xx * xx;
      if (r2 != 0.)
      {
        rfsta2 = fstar2 * r2;
        r = r2;
        h = h + b[m] * IL_crst(r, fi);
      }
    }
 
    hz = h + zmin;
    zz = pp3dMM.getZ() + zmin;
    err = hz - zz;

    (ertot) += err * err;
  }

  return 1;
}

bool te::mnt::SplineInterpolationGrassMitasova::IL_grid_calc(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax, int KMAX2, std::vector<double>& b, double fi, double dnorm)
{
  double x_or = xMin;

  double x1 = m_env.getLowerLeftX();
  double y2 = m_env.getUpperRightY();

  double ns_res = m_resy;
  double ew_res = m_resx;

  double deltx = xMax - xMin;
  double delty = yMax - yMin;

  int n_rows = (int)(delty / ns_res);
  int n_cols = (int)(deltx / ew_res);

  size_t n_points = m_pointsRegion.size();

  int ngstc = (int)((x_or - x1) / ew_res + 0.5);
  int nszc = ngstc + n_cols;
  int ngstr = (int)((y2 - yMax) / ns_res + 0.5);
  int nszr = ngstr + n_rows;

  double zminac = 0.;
  double zmaxac = 0.;
  int first_time_z = 1;

  double stepix = ew_res / dnorm;
  double stepiy = ns_res / dnorm;

  std::vector<double> w2;
  std::vector<double> w;

  w.resize(KMAX2 + 9);
  w2.resize(KMAX2 + 9);

  int contadory = 0;

  for (int k = ngstr; k <= nszr; k++)
  {
    double yg = (k - ngstr) * stepiy + stepiy / 2.; // fixed by J.H. in July 01 
    for (int m = 1; m <= n_points; m++)
    {
      te::gm::PointZ pp3d = m_pointsRegion.at(m - 1);
      double wm = yg - pp3d.getY();
      w[m] = wm;
      w2[m] = wm * wm;
    }

    for (int l = ngstc; l <= nszc; l++)
    {
      double xg = (l - ngstc) * stepix + stepix / 2.; //fixed by J.H. in July 01 

      // compute everything for area which is * not masked out
      double h = b[0];
      for (int m = 1; m <= n_points; m++)
      {
        te::gm::PointZ pp3d = m_pointsRegion.at(m - 1);
        double xx = xg - pp3d.getX();
        double xx2 = xx * xx;
        double r2 = xx2 + w2[m];
        double r = r2;

        h = h + b[m] * IL_crst(r, fi);
      }

      double zz = h;
      if (first_time_z)
      {
        first_time_z = 0;
        zmaxac = zminac = zz;
      }
      zmaxac = amax1(zz, zmaxac);
      zminac = amin1(zz, zminac);
      if ((zz > zMax + 0.1 * (zMax - zMin)) || (zz < zMin - 0.1 * (zMax - zMin)))
      {
        static int once = 0;
        if (!once)
        {
          once = 1;
        }
      }
      int row = nszr - contadory;
      if (row >= 0 && row < (int)m_rst->getNumberOfRows() &&
        l >= 0 && l < (int)m_rst->getNumberOfColumns())
        m_rst->setValue((unsigned int)l, (unsigned int)row, zz);
    }
    contadory++;
  }

  w.clear();
  w2.clear();

  return true;
}

