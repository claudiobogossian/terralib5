/*!
\file terralib/mnt/core/SplineGrass.cpp

\brief This file contains a class to calculate a grid from Sample using spline grass interpolator.

*/

#include "SplineGrass.h"
#include "Utils.h"

#include "../../common/progress/TaskProgress.h"

bool te::mnt::SplineInterpolationGrass::generateGrid()
{
  unsigned int nro_neighb;
  double rx1, ry2;
  unsigned int NCols;
  unsigned int NLines;

  m_tolerance = m_resx / 4;

  std::auto_ptr<te::rst::Raster> rst = Initialize(true, nro_neighb, rx1, ry2, NCols, NLines);

  //divide in x overlaping parts
  unsigned int nPartsX = m_nPartsX;
  unsigned int nPartsY = m_nPartsY;
  double deltx = m_env.getUpperRightX() - m_env.getLowerLeftX();
  double delty = m_env.getUpperRightY() - m_env.getLowerLeftY();

  double ew_region = deltx / (double)nPartsX;
  double ns_region = delty / (double)nPartsY;

  double percentageDist = m_overlapping;

  unsigned int _minimoPontos = std::min(m_minpoints, (unsigned int)m_dataset.size());

  double resX = m_resx;
  double resY = m_resy;

  double zdummy = m_nodatavalue;
  int nsteps = (int)(m_nPartsY*m_nPartsY + 1);

  te::common::TaskProgress task("Generating DTM...", te::common::TaskProgress::UNDEFINED, nsteps);

  double X1 = m_env.getLowerLeftX() + resX / 2.;
  double Y1 = m_env.getLowerLeftY() - resY / 2.;

  for (unsigned int i = 0; i < m_nPartsY; i++)
  {
    double begin_y = Y1 + (i*ns_region);
    double end_y = Y1 + ((i + 1)*ns_region);

    double disty = (end_y - begin_y) * percentageDist;

    for (unsigned int j = 0; j < m_nPartsX; j++)
    {
      if (!task.isActive())
        return false;
      task.pulse();
      //Calculate the init and final area
      double begin_x = X1 + (j*ew_region);
      double end_x = X1 + ((j + 1)*ew_region);

      double distx = (end_x - begin_x) * percentageDist;

      double min_x = begin_x;
      double min_y = begin_y;

      int nOverlaping = 1;
      bool poucosPontos = false;
      do
      {
        double overlapDistX = distx*nOverlaping;
        double overlapDistY = disty*nOverlaping;

        int beginLine = std::max((int)((begin_y - Y1 - overlapDistY) / resY), 0);
        int endLine = std::min((int)((end_y - Y1 + overlapDistY) / resY), (int)NLines);

        min_y = std::max(Y1, begin_y - overlapDistY);
        if (beginLine == 0)
          min_y = Y1;
        else
          min_y = begin_y - overlapDistY;

        int beginCol = std::max((int)((begin_x - X1 - overlapDistX) / resX), 0);
        int endCol = std::min((int)((end_x - X1 + overlapDistX) / resX), (int)NCols);

        min_x = std::max(X1, begin_x - overlapDistX);
        if (beginCol == 0)
          min_x = X1;
        else
          min_x = begin_x - overlapDistX;

        //inits the interpolation
        initInterpolation(beginLine, endLine, beginCol, endCol);

        //add the points control and the overlaping area
        setControlPoints(min_x, min_y);

        poucosPontos = m_obsVect.size() < _minimoPontos;
        nOverlaping++;

      } while (poucosPontos && (nOverlaping < 3));

      /*--------------------------------------*/
      m_N.resize(m_nparameters);
      m_TN.resize(m_nparameters);
      m_parVect.resize(m_nparameters);
      for (unsigned int k = 0; k < m_nparameters; k++)
      {
        m_N[k].resize(m_BW);
      }
      /*--------------------------------------*/

      //calculate the grid
      calculateGrid(min_x, min_y);

      /*--------------------------------------*/
      for (unsigned int k = 0; k < m_nparameters; k++)
      {
        m_N[k].clear();
      }
      m_N.clear();
      m_TN.clear();
      /*--------------------------------------*/

      //calculate the grid
      unsigned int firstLine = (unsigned int)std::max((int)((begin_y - Y1) / resY), 0);
      unsigned int lastLine = std::min((unsigned int)((end_y - Y1) / resY), NLines);

      unsigned int firstCol = (unsigned int)std::max((int)((begin_x - X1) / resX), 0);
      unsigned int lastCol = std::min((unsigned int)((end_x - X1) / resX), NCols);

      //allows calculated values to grid
      unsigned int l, c;
      //allows only the lines of the medium
      for (l = firstLine; l<lastLine; l++)
      {
        for (c = firstCol; c < lastCol; c++)
        {
          te::gm::PointZ pg((X1 + (double)(c*m_resx)), (Y1 + (double)(l*m_resy)), m_nodatavalue);

          double interpolation = 0;
          bool doInterpolation;
          if (m_inter == SplineBilinear)
            interpolation = dataInterpolateBilin(pg.getX(), pg.getY(), min_x, min_y, doInterpolation);
          else
            interpolation = dataInterpolateBicubic(pg.getX(), pg.getY(), min_x, min_y, doInterpolation);

          if (!doInterpolation)
            continue;

          if (nPointInterest())
            interpolation += pointsControlMean();
          else
            interpolation = zdummy;

          pg.setZ(interpolation);

          m_rst->setValue(c, NLines - l - 1, pg.getZ());
        }
      }
      m_parVect.clear();
    }
  }

  rst.release();

  return true;
}

void te::mnt::SplineInterpolationGrass::initInterpolation(int beginLine, int endLine, int beginCol, int endCol)
{
  //define splines number
  m_nsplx = (unsigned int)(endCol - beginCol);
  m_nsply = (unsigned int)(endLine - beginLine);

  //define parameters number
  m_nparameters = m_nsplx * m_nsply;

  //define north and west step
  m_passoWidth = m_resx;
  m_passoHeight = m_resy;

  //define lambda
  m_lambda = 1;

  //define o bandwich
  m_BW = 0;
  if (m_inter == SplineBilinear)
    m_BW = (2 * m_nsply + 1);
  else //bicubico
    m_BW = (4 * m_nsply + 3);
}

void te::mnt::SplineInterpolationGrass::setControlPoints(double xMin, double yMin)
{
  int i_x; /* x = (xMin + (i_x * passoWidth) + csi_x) */
  double csi_x;

  int  i_y; /* y = (yMin + (i_y * passoHeight) + csi_y) */
  double csi_y;

  std::size_t nVectorPoints = m_dataset.size();

  //initialize mean and points
  m_mean = 0;
  m_npoints = 0;

  //calculate mean and points
  for (unsigned int i = 0; i<nVectorPoints; i++)
  {
    te::gm::PointZ pp3d = m_dataset.at(i).second;
    node_x(pp3d.getX(), i_x, csi_x, xMin, m_passoWidth);
    node_y(pp3d.getY(), i_y, csi_y, yMin, m_passoHeight);

    if ((i_x >= -1) && (i_x < (int)m_nsplx) && (i_y >= -1) && (i_y < (int)m_nsply))
    {
      m_mean += pp3d.getZ();
      m_npoints++;
    }
  }
  m_mean /= (double)m_npoints;

  //create point list
  m_obsVect.clear();
  for (unsigned int i = 0; i<nVectorPoints; i++)
  {
    te::gm::PointZ pp3d = m_dataset[i].second;
    node_x(pp3d.getX(), i_x, csi_x, xMin, m_passoWidth);
    node_y(pp3d.getY(), i_y, csi_y, yMin, m_passoHeight);

    if ((i_x >= -1) && (i_x < (int)m_nsplx) && (i_y >= -1) && (i_y < (int)m_nsply))
    {
      //update new point
      te::gm::PointZ npp3d = pp3d;
      npp3d.setZ(pp3d.getZ() - m_mean);

      //add a new point
      m_obsVect.push_back(npp3d);
    }
  }
}

/*!
* \brief Normal system computation for bilinear spline interpolation
*/

void te::mnt::SplineInterpolationGrass::normalDefBilin(double xMin, double yMin)
{
  double  alpha[2][2];  /* coefficients */

  int i_x;  /* x = (xMin + (i_x * passoWidth) + csi_x) */
  double  csi_x;

  int   i_y;  /* y = (yMin + (i_y * passoHeight) + csi_y) */
  double  csi_y;

  /*--------------------------------------*/
  for (unsigned int k = 0; k < m_nparameters; k++)
  {
    for (unsigned int h = 0; h < m_BW; h++)
      m_N[k][h] = 0.;/* Normal matrix inizialization */
    m_TN[k] = 0.;/* Normal vector inizialization */
  }
  /*--------------------------------------*/

  unsigned int n0;
  for (unsigned int i = 0; i < m_npoints; i++)
  {
    te::gm::PointZ pp3D = m_obsVect[i];
    node_x(pp3D.getX(), i_x, csi_x, xMin, m_passoWidth);
    node_y(pp3D.getY(), i_y, csi_y, yMin, m_passoHeight);

    if ((i_x >= -1) && (i_x < (int)m_nsplx) && (i_y >= -1) && (i_y < (int)m_nsply))
    {
      csi_x = csi_x / m_passoWidth;
      csi_y = csi_y / m_passoHeight;

      alpha[0][0] = phi(csi_x, csi_y);
      alpha[0][1] = phi(csi_x, 1 - csi_y);
      alpha[1][0] = phi(1 - csi_x, csi_y);
      alpha[1][1] = phi(1 - csi_x, 1 - csi_y);

      for (unsigned int k = 0; k <= 1; k++)
      {
        for (unsigned int h = 0; h <= 1; h++)
        {
          if (((i_x + k) <= (m_nsplx - 1)) && ((i_y + h) <= (m_nsply - 1)))
          {
            for (unsigned int m = k; m <= 1; m++)
            {
              if (m == k) n0 = h;
              else n0 = 0;

              for (unsigned int n = n0; n <= 1; n++)
              {
                if (((i_x + m) < m_nsplx) && ((i_y + n) < m_nsply))
                {
                  m_N[(unsigned int)order(i_x + (int)k, i_y + (int)h, m_nsply)][(unsigned int)(order(i_x + (int)m, i_y + (int)n, m_nsply) - \
                    order(i_x + (int)k, i_y + (int)h, m_nsply))] += alpha[k][h] * alpha[m][n];
                }
              }
            }
            m_TN[(unsigned int)order(i_x + (int)k, i_y + (int)h, m_nsply)] += pp3D.getZ()* alpha[k][h];
          }
        }
      }
    }
  }

  return;
}

/*!
* \brief Normal system computation for bicubic spline interpolation
*/

void te::mnt::SplineInterpolationGrass::normalDefBicubic(double xMin, double yMin)
{
  double  alpha[4][4];  /* coefficients */

  int i_x;  /* x = (xMin + (i_x * passoWidth) + csi_x) */
  double  csi_x;

  int   i_y;  /* y = (yMin + (i_y * passoHeight) + csi_y) */
  double  csi_y;

  /*--------------------------------------*/
  for (unsigned int k = 0; k < m_nparameters; k++)
  {
    for (unsigned int h = 0; h < m_BW; h++)
      m_N[k][h] = 0.; /* Normal matrix inizialization */
    m_TN[k] = 0.; /* Normal vector inizialization */
  }
  /*--------------------------------------*/

  for (unsigned int i = 0; i < m_npoints; i++)
  {
    te::gm::PointZ pp3D = m_obsVect[i];
    node_x(pp3D.getX(), i_x, csi_x, xMin, m_passoWidth);
    node_y(pp3D.getY(), i_y, csi_y, yMin, m_passoHeight);

    if ((i_x >= -2) && (i_x <= (int)m_nsplx) && (i_y >= -2) && (i_y <= (int)m_nsply))
    {
      csi_x = csi_x / m_passoWidth;
      csi_y = csi_y / m_passoHeight;

      alpha[0][0] = phi_44(1 + csi_x, 1 + csi_y);
      alpha[0][1] = phi_43(1 + csi_x, csi_y);
      alpha[0][2] = phi_43(1 + csi_x, 1 - csi_y);
      alpha[0][3] = phi_44(1 + csi_x, 2 - csi_y);

      alpha[1][0] = phi_34(csi_x, 1 + csi_y);
      alpha[1][1] = phi_33(csi_x, csi_y);
      alpha[1][2] = phi_33(csi_x, 1 - csi_y);
      alpha[1][3] = phi_34(csi_x, 2 - csi_y);

      alpha[2][0] = phi_34(1 - csi_x, 1 + csi_y);
      alpha[2][1] = phi_33(1 - csi_x, csi_y);
      alpha[2][2] = phi_33(1 - csi_x, 1 - csi_y);
      alpha[2][3] = phi_34(1 - csi_x, 2 - csi_y);

      alpha[3][0] = phi_44(2 - csi_x, 1 + csi_y);
      alpha[3][1] = phi_43(2 - csi_x, csi_y);
      alpha[3][2] = phi_43(2 - csi_x, 1 - csi_y);
      alpha[3][3] = phi_44(2 - csi_x, 2 - csi_y);

      int n0;
      for (int k = -1; k <= 2; k++)
      {
        for (int h = -1; h <= 2; h++)
        {
          if (((i_x + k) >= 0) && ((i_x + k) < (int)m_nsplx) && ((i_y + h) >= 0) && ((i_y + h) < (int)m_nsply))
          {
            for (int m = k; m <= 2; m++)
            {
              if (m == k) n0 = h;
              else n0 = -1;

              for (int n = n0; n <= 2; n++) {
                if (((i_x + m) >= 0) && ((i_x + m) < (int)m_nsplx) && ((i_y + n) >= 0) && ((i_y + n) < (int)m_nsply)) {
                  m_N[(unsigned int)order(i_x + k, i_y + h, m_nsply)][(unsigned int)(order(i_x + m, i_y + n, m_nsply) - \
                    order(i_x + k, i_y + h, m_nsply))] += alpha[(unsigned int)(k + 1)][(unsigned int)(h + 1)] * alpha[(unsigned int)(m + 1)][(unsigned int)(n + 1)];
                }
              }
            }
            m_TN[(unsigned int)order(i_x + k, i_y + h, m_nsply)] += pp3D.getZ() * alpha[(unsigned int)(k + 1)][(unsigned int)(h + 1)];
          }
        }
      }
    }
  }

  return;
}

/*!
* \brief 1-DELTA discretization
*/
void te::mnt::SplineInterpolationGrass::nCorrectGrad()
{
  unsigned int i;
  unsigned int parNum;

  double alpha[3];
  double lambdaX, lambdaY;

  lambdaX = m_lambda * (m_passoHeight / m_passoWidth);
  lambdaY = m_lambda * (m_passoWidth / m_passoHeight);

  parNum = m_nsplx *  m_nsply;

  alpha[0] = 2 * lambdaX + 2 * lambdaY;
  alpha[1] = -lambdaX;
  alpha[2] = -lambdaY;

  for (i = 0; i < parNum; i++) {
    m_N[i][0] += alpha[0];

    if ((i + 1) < parNum)
      m_N[i][1] += alpha[2];

    if ((i + 1 * m_nsply) < parNum)
      m_N[i][1 * m_nsply] += alpha[1];
  }

  return;
}

/*!
* \brief Tcholetsky decomposition -> T= Lower Triangular Matrix
*/

bool te::mnt::SplineInterpolationGrass::tcholDec(std::vector< std::vector<double> > &T)
{
  unsigned int i, j, k;
  double  somma;

  for (i = 0; i < m_nparameters; i++)
  {
    for (j = 0; j < m_BW; j++)
    {
      somma = m_N[i][j];
      for (k = 1; k < m_BW; k++)
        if ((int(i - k) >= 0) && ((j + k) < m_BW))
          somma -= T[i - k][k] * T[i - k][j + k];
      if (j == 0) {
        if (somma <= 0.0)
          return false;
        T[i][0] = sqrt(somma);
      }
      else T[i][j] = somma / T[i][0];
    }
  }

  return true;
}

/*!
* \brief Data interpolation in a generic point
*/

double te::mnt::SplineInterpolationGrass::dataInterpolateBilin(double x, double y, double xMin, double yMin, bool &interp)
{
  double  z;  /* abscissa, ordinate and associated value */

  int k, h; /* counters		*/
  double  alpha[2][2];  /* coefficients */

  int i_x, i_y; /* x = (xMin + (i_x * passoWidth) + csi_x) */
  double  csi_x, csi_y; /* y = (yMin + (i_y * passoHeight) + csi_y) */

  z = 0;

  interp = false;

  node_x(x, i_x, csi_x, xMin, m_passoWidth);
  node_y(y, i_y, csi_y, yMin, m_passoHeight);

  if ((i_x >= -1) && (i_x <(int)m_nsplx) && (i_y >= -1) && (i_y < (int)m_nsply))
  {
    csi_x = csi_x / m_passoWidth;
    csi_y = csi_y / m_passoHeight;

    alpha[0][0] = phi(csi_x, csi_y);
    alpha[0][1] = phi(csi_x, 1 - csi_y);

    alpha[1][0] = phi(1 - csi_x, csi_y);
    alpha[1][1] = phi(1 - csi_x, 1 - csi_y);

    for (k = 0; k <= 1; k++)
    {
      for (h = 0; h <= 1; h++)
      {
        if (((i_x + k) >= 0) && ((i_x + k) < (int)m_nsplx) && ((i_y + h) >= 0) && ((i_y + h) < (int)m_nsply))
          z += m_parVect[(unsigned int)order(i_x + k, i_y + h, m_nsply)] * alpha[(unsigned int)k][(unsigned int)h];
      }
    }

    interp = true;
  }

  return z;
}

/*!
* \brief Data interpolation in a generic point
*/

double te::mnt::SplineInterpolationGrass::dataInterpolateBicubic(double x, double y, double xMin, double yMin, bool &interp)
{
  double  z;  /* abscissa, ordinate and associated value */

  int k, h; /* counters */
  double  alpha[4][4];  /* coefficients */

  int i_x, i_y; /* x = (xMin + (i_x * passoWidth) + csi_x) */
  double  csi_x, csi_y; /* y = (yMin + (i_y * passoHeight) + csi_y) */

  z = 0;
  interp = false;

  node_x(x, i_x, csi_x, xMin, m_passoWidth);
  node_y(y, i_y, csi_y, yMin, m_passoHeight);

  if ((i_x >= -2) && (i_x <= (int)m_nsplx) && (i_y >= -2) && (i_y <= (int)m_nsply)) {

    csi_x = csi_x / m_passoWidth;
    csi_y = csi_y / m_passoHeight;

    alpha[0][0] = phi_44(1 + csi_x, 1 + csi_y);
    alpha[0][1] = phi_43(1 + csi_x, csi_y);
    alpha[0][2] = phi_43(1 + csi_x, 1 - csi_y);
    alpha[0][3] = phi_44(1 + csi_x, 2 - csi_y);

    alpha[1][0] = phi_34(csi_x, 1 + csi_y);
    alpha[1][1] = phi_33(csi_x, csi_y);
    alpha[1][2] = phi_33(csi_x, 1 - csi_y);
    alpha[1][3] = phi_34(csi_x, 2 - csi_y);

    alpha[2][0] = phi_34(1 - csi_x, 1 + csi_y);
    alpha[2][1] = phi_33(1 - csi_x, csi_y);
    alpha[2][2] = phi_33(1 - csi_x, 1 - csi_y);
    alpha[2][3] = phi_34(1 - csi_x, 2 - csi_y);

    alpha[3][0] = phi_44(2 - csi_x, 1 + csi_y);
    alpha[3][1] = phi_43(2 - csi_x, csi_y);
    alpha[3][2] = phi_43(2 - csi_x, 1 - csi_y);
    alpha[3][3] = phi_44(2 - csi_x, 2 - csi_y);

    for (k = -1; k <= 2; k++)
    {
      for (h = -1; h <= 2; h++)
      {
        if (((i_x + k) >= 0) && ((i_x + k) < (int)m_nsplx) && ((i_y + h) >= 0) && ((i_y + h) < (int)m_nsply))
          z += m_parVect[(unsigned int)order(i_x + k, i_y + h, m_nsply)] * alpha[(unsigned int)(k + 1)][(unsigned int)(h + 1)];
      }
    }
    interp = true;
  }

  return z;
}

/*!
* \brief Tcholetsky matrix solution
*/
bool te::mnt::SplineInterpolationGrass::tcholSolve()
{
  std::vector< std::vector<double> > T;
  size_t i, j;

  T = m_N;

  if (!tcholDec(T)) /* T computation		*/
    return false;

  /* Forward substitution */
  m_parVect[0] = m_TN[0] / T[0][0];
  for (i = 1; i < m_nparameters; i++)
  {
    m_parVect[i] = m_TN[i];
    for (j = 0; j < i; j++)
      if ((i - j) < m_BW) m_parVect[i] -= T[j][i - j] * m_parVect[j];
    m_parVect[i] = m_parVect[i] / T[i][0];
  }

  /* Backward substitution */
  m_parVect[m_nparameters - 1] = m_parVect[m_nparameters - 1] / T[m_nparameters - 1][0];
  for (int ii = (int)m_nparameters - 2; ii >= 0; ii--)
  {
    i = (size_t)ii;
    for (j = i + 1; j < m_nparameters; j++)
      if ((j - i) < m_BW) m_parVect[i] -= T[i][j - i] * m_parVect[j];
    m_parVect[i] = m_parVect[i] / T[i][0];
  }
  return true;
}

bool te::mnt::SplineInterpolationGrass::calculateGrid(double xMin, double yMin)
{
  //calcula a grade
  if (m_inter == SplineBilinear)
  {//Bilinear
    normalDefBilin(xMin, yMin);
  }
  else
  {//Bicubica
    normalDefBicubic(xMin, yMin);
  }

  nCorrectGrad();

  //resolve
  if (!tcholSolve())
  {
    return false;
  }
  return true;
}

te::gm::LineString* te::mnt::SplineInterpolationGrass::pointListSimplify(te::gm::LineString *ls, double snap, double Zvalue)
{
  size_t npts = ls->getNPoints();
  size_t npte = npts;

  te::gm::Coord2D* vxy = ls->getCoordinates();

  //  If line is too short or snap is zero do nothing
  if (npts <= 3 || snap == 0.)
  {
    te::gm::LineString *ptlist = new te::gm::LineString(npts, te::gm::LineStringZType);
    for (size_t i = 0; i < npts; i++)
    {
      ptlist->setPointZ(i, vxy[i].getX(), vxy[i].getY(), Zvalue);
    }
    return ptlist;
  }

  size_t npt;
  bool island = false;
  //  Check for islands before defining number of points to be used
  if (vxy[0] == vxy[npte - 1])
  {
    npt = npte - 1;
    island = true;
  }
  else
    npt = npte;

  //  initialize variables
  size_t i = 0;
  size_t numa = 0;
  size_t numpf = npt - 1;

  //  define anchor
  te::gm::Coord2D axy = vxy[numa];

  //  define floating point
  te::gm::Coord2D pfxy = vxy[numpf];

  bool retv;
  double dmax, d,
    a = 0, b = 0,
    aa1 = 0;
  size_t numdmax;

  while (numa < (npt - 1))
  {
    //    Compute coeficients of straight line y=ax+b
    if (pfxy.getX() == axy.getX())
      retv = true;
    else
    {
      retv = false;
      a = (pfxy.getY() - axy.getY()) / (pfxy.getX() - axy.getX());
      b = pfxy.getY() - a * pfxy.getX();
      aa1 = sqrt(a * a + 1.);
    }

    dmax = 0;
    numdmax = numpf;
    size_t k;

    for (k = numa + 1; k < numpf; k++)
    {
      //      Distance between point and line
      if (retv)
        d = fabs(axy.getX() - vxy[k].getX());
      else
        d = fabs(vxy[k].getY() - a*vxy[k].getX() - b) / aa1;

      if (d > dmax)
      {
        dmax = d;
        numdmax = k;
      }
    }

    if (dmax <= snap)
    {
      // Store selected point
      vxy[i++] = axy;

      // Shift anchor
      numa = numpf;
      axy = vxy[numpf];
      numpf = npt - 1;
    }
    else
      // Shift floating point
      numpf = numdmax;

    pfxy = vxy[numpf];
  }

  //  Store results
  vxy[i] = vxy[numa];
  npts = i + 1;

  if ((Distance(vxy[i], vxy[i - 1]) < snap) || (Distance(vxy[i], vxy[0]) < snap))
    npts = i;

  size_t newpts;
  if (island)
    newpts = npts + 1;
  else
    newpts = npts;

  te::gm::LineString *ptlist = new te::gm::LineString(newpts, te::gm::LineStringZType, ls->getSRID());

  for (i = 0; i < npts; i++)
  {
    ptlist->setPointZ(i, vxy[i].getX(), vxy[i].getY(), Zvalue);
  }

  if (island)
    ptlist->setPointZ(npts, vxy[0].getX(), vxy[0].getY(), Zvalue);

  AdjustLinear(ptlist, snap * 5.);

  return ptlist;
}

bool te::mnt::SplineInterpolationGrass::AdjustLinear(te::gm::LineString *ptol, double maxDist)
{
  if (maxDist == 0.)
    return false;

  std::size_t npte = ptol->getNPoints();
  if (npte < 2)
    return false;

  double z = ptol->getZ(0);
  te::gm::Coord2D* vxy = ptol->getCoordinates();
  std::vector<te::gm::PointZ> pts;

  double xA, yA, xB, yB;
  for (std::size_t i = 0; i < npte - 1; i++)
  {
    xA = vxy[i].getX();
    yA = vxy[i].getY();
    xB = vxy[i + 1].getX();
    yB = vxy[i + 1].getY();
    double dist = sqrt((xA - xB)*(xA - xB) + (yA - yB)*(yA - yB));
    int nsteps = (int)((dist / maxDist) + 1);
    if (nsteps > 1000)
      nsteps = 1000;
    for (int j = 0; j < nsteps; j++)
    {
      double t = (double)j / (double)nsteps;
      double x = (xB - xA)*t + xA;
      double y = (yB - yA)*t + yA;
      pts.push_back(te::gm::PointZ(x, y, z));
    }
    if (i == npte - 2)
    {
      double x = xB;
      double y = yB;

      pts.push_back(te::gm::PointZ(x, y, z));
    }
  }

  ptol->setNumCoordinates(pts.size());
  for (size_t i = 0; i < pts.size(); i++)
  {
    ptol->setPointN(i, pts[i]);
  }

  return true;
}

