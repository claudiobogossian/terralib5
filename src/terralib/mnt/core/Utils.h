#include "../../geometry/Coord2D.h"
#include "../../geometry/LineString.h"
#include "../../geometry/PointZ.h"

#include "Config.h"


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
    double Distance(const te::gm::Coord2D &pt1, const te::gm::Coord2D &pt2);
    bool Equal(te::gm::PointZ &p1, te::gm::PointZ &p2, double &tol);

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

  }
}
