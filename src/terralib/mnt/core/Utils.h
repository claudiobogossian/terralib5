
/*!
\file terralib/mnt/core/Utils.h

\brief Utility functions for MNT support.
*/

#ifndef __TERRALIB_MNT_INTERNAL_UTILS_H
#define __TERRALIB_MNT_INTERNAL_UTILS_H

#include "Config.h"
#include "Enums.h"

#include "../../common/UnitsOfMeasureManager.h"

#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/datasource/DataSource.h"

#include "../../geometry/Coord2D.h"
#include "../../geometry/LineString.h"
#include "../../geometry/MultiLineString.h"
#include "../../geometry/MultiPoint.h"
#include "../../geometry/PointZ.h"


#ifndef MIN
#define MIN(x, y) ( ((x) < (y)) ? (x) : (y) )
#endif

#ifndef MAX
#define MAX(x, y) ( ((x) > (y)) ? (x) : (y) )
#endif


namespace te
{
  namespace mnt
  {
    size_t ReadPoints(std::string &inDsetName, te::da::DataSourcePtr &inDsrc, std::string &atrZ, double tol, te::gm::MultiPoint &mpt, std::string &geostype, te::gm::Envelope &env);

    size_t ReadSamples(std::string &inDsetName, te::da::DataSourcePtr &inDsrc, std::string &atrZ, double tol, double max, Simplify alg, te::gm::MultiPoint &mpt, te::gm::MultiLineString &isolines, std::string &geostype, te::gm::Envelope &env);

    double Distance(const te::gm::Coord2D &pt1, const te::gm::Coord2D &pt2);
    bool Equal(te::gm::PointZ &p1, te::gm::PointZ &p2, double &tol);

    //DouglasPeucker from TerraAmazon 4
    te::gm::LineString* DouglasPeuckerTA(te::gm::LineString *lineIn, double simpFactor, double Zvalue);

    //DouglasPeucker from libgeos
    te::gm::LineString* GEOS_DouglasPeucker(te::gm::LineString *ls, double snap, double Zvalue);

    TEMNTEXPORT te::gm::LineString* pointListSimplify(te::gm::LineString *ls, double snap, double maxdist, double Zvalue);

    double pointToSegmentDistance(te::gm::PointZ &fseg, te::gm::PointZ &lseg, te::gm::PointZ &pt, te::gm::PointZ *pti);

    // Find center point of triangle using its vertices.
    short findCenter(te::gm::PointZ* vert, double* pcx, double* pcy);

    short segIntersect(te::gm::PointZ &pfr, te::gm::PointZ &pto, te::gm::PointZ &lfr, te::gm::PointZ &lto);
    bool segInterPoint(te::gm::PointZ &pfr, te::gm::PointZ &pto, te::gm::PointZ &lfr, te::gm::PointZ &lto, te::gm::PointZ *pt);

    // Test Vertex values for valid and special cases, returns TRUE if normal,
    // FALSE otherwise.
    bool testVertexValues(double, te::gm::PointZ*);

    // Define intersections of contour with edges of triangle
    bool defineInterTriangle(std::vector<te::gm::PointZ>&, te::gm::PointZ*, double );

    // Define intersection point of contour with edge between two points
    bool defineInterEdge(std::vector<te::gm::PointZ>&, te::gm::PointZ&, te::gm::PointZ&, double);

    //Extract lines from pline and store connected ones in clinlist.
    bool extractLines(std::vector<te::gm::PointZ> &pline, std::vector<te::gm::LineString> &clinlist, double scale);

    bool initLineVector(std::vector<te::gm::PointZ> &pline, std::vector<te::gm::PointZ> &vect);
    bool assembLine(std::vector<te::gm::LineString> &linlout, std::vector<te::gm::PointZ> &vect);

    bool equalFptSpt(te::gm::PointZ & fpt, te::gm::PointZ &spt, double scale);

    te::gm::Point Max(te::gm::Point& p1, te::gm::Point& p2);
    te::gm::Point Min(te::gm::Point& p1, te::gm::Point& p2);

    // Calculate Normal vector of Triangle
    bool triangleNormalVector(te::gm::PointZ*, double*);

    bool normalizeVector(double*);

    bool onSegment(te::gm::PointZ& pt, te::gm::PointZ& fseg, te::gm::PointZ& lseg, double tol);

    int onSameSide(te::gm::Coord2D &pt1, te::gm::Coord2D &pt2, te::gm::Coord2D &fseg, te::gm::Coord2D &lseg);

    bool point3dListFilter(std::vector<te::gm::PointZ> &p3dl, std::vector<bool> &fixed, double tol);

    bool Least_square_fitting(double *vectx, double *vecty, short np, short deg, double *coef);

    bool Gauss_elimination(short m, short n, double mat[6][6]);

    bool SaveIso(std::string& outDsetName, te::da::DataSourcePtr &outDsrc, std::vector<te::gm::LineString> &isolist, std::vector<double> &guidevalues, int srid);

    void Save(te::da::DataSource* source, te::da::DataSet* result, te::da::DataSetType* outDsType);

    TEMNTEXPORT bool convertPlanarToAngle(double& val, te::common::UnitOfMeasurePtr unit);

  }
}

#endif
