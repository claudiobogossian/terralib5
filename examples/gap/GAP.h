#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/datatype/SimpleProperty.h>

#include <terralib/geometry/GeometryProperty.h>
#include <terralib/geometry/GEOSReader.h>
#include <terralib/geometry/GEOSWriter.h>
#include <terralib/geometry/Line.h>
#include <terralib/geometry/MultiPolygon.h>

#include <terralib/memory/dataset.h>
#include <terralib/memory/datasetitem.h>

#include <terralib/sam.h>



#include <geos/geom/Coordinate.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/operation/distance/DistanceOp.h>


#include <string>
#include <vector>

typedef te::sam::kdtree::Node<te::gm::Coord2D, std::pair<int32_t, int32_t>, std::pair<int32_t, int32_t> > KD_NODE;
typedef te::sam::kdtree::Index<KD_NODE> KD_TREE;

enum Pol_OUT
{
  Polygon_out,
  Line_out,
  Point_out,
  RTree_out
};

class GAP_inter
{
public:
  GAP_inter()
    : m_polid(-1), m_ptid(-1), m_x(0), m_y(0), m_xint(0), m_yint(0), m_xnew(0), m_ynew(0), 
    m_distance(0), m_distance0(0), m_tipo("") {}

  GAP_inter(int32_t polid, int32_t id, double x, double y, double xint, double yint, double xn, double yn, 
    double distance, double distance0, std::string t, bool inside)
     : m_polid(polid), m_ptid(id), m_x(x), m_y(y), m_xint(xint), m_yint(yint), m_xnew(xn), m_ynew(yn), 
     m_distance(distance), m_distance0(distance0), m_tipo(t), m_inside(inside)  {}

   bool operator < (const GAP_inter&);
   bool operator == (const GAP_inter&);

  int32_t m_polid; //polygon id
  int32_t m_ptid; //point order
   double m_x; //X coordinate 
  double m_y; //y coordinate
  double m_xint; //X coordinate intersection calculated point in segment
  double m_yint; //Y coordinate intersection calculated point in segment
  double m_xnew; //X new positon
  double m_ynew; //X new positon
  double m_distance; //Distance point to segment
  double m_distance0; //Distance point to first point segment
  std::string m_tipo;
  bool m_inside;
};

class GAP_segment
{
public:
  GAP_segment()
    : m_seg(te::gm::LineStringType), m_polid(-1), m_centroid(), m_mindist(0) {}
  GAP_segment(te::gm::Line &seg, int32_t polid, te::gm::Coord2D &cent, double min)
    : m_seg(seg), m_polid(polid), m_centroid(cent), m_mindist(min) {}

  bool isInside(te::gm::Coord2D &pt);
  void insert_inter(GAP_inter *inter);

  te::gm::Line m_seg; //Segment - line with 2 points
  int32_t m_polid; //ID polygon which contains segment
  te::gm::Coord2D m_centroid; //Polygon centroid
  double  m_mindist; //Tolerancy to find closest points
  std::vector<GAP_inter *> m_inter; //Points close this segment

};

class GAP_pt_extern
{
public:
  GAP_pt_extern(){}

  GAP_pt_extern(te::gm::Coord2D pint, int32_t id, double dist)
    : m_ptint(pint), m_seg_id(id), m_dist(dist) {}

  te::gm::Coord2D m_ptint;
  int32_t m_seg_id;
  double m_dist;
};

class GAP
{
public:

  GAP();
  ~GAP() 
  {
    delete m_nodetree;
    delete m_nodetree_step2;
  }

  bool run();

  bool step00(std::vector< std::pair< size_t, te::gm::Polygon* > > &polsout);
  bool step0(std::vector< std::pair< size_t, te::gm::Polygon* > > &pols, std::vector< std::pair< size_t, te::gm::Polygon* > > &polsout);
  bool step1(std::vector< std::pair< size_t, te::gm::Polygon* > > &pols, std::vector< std::pair< size_t, te::gm::Polygon* > > &polsout);
  bool step2(std::vector< std::pair< size_t, te::gm::Polygon* > > &pols, std::vector< std::pair< size_t, te::gm::Polygon* > > &polsout);
  bool step3(std::vector< std::pair< size_t, te::gm::Polygon* > > &pols, std::vector< std::pair< size_t, te::gm::Polygon* > > &polsout);
  bool step4(std::vector< std::pair< size_t, te::gm::Polygon* > > &pols, std::vector< std::pair< size_t, te::gm::Polygon* > > &polsout);

  bool LoadPolygons(std::string &filename, std::string &inDsetName, Pol_OUT out);

  bool Polygon2Points(te::gm::Polygon* pol, int32_t id);
  bool Polygon2RTree(te::gm::Polygon* pol, te::sam::rtree::Index<te::gm::Line*> &rtree);
  bool Polygon2Lines(te::gm::Polygon* pol);

  void Save(te::da::DataSource* source, te::da::DataSet* result, te::da::DataSetType* outDsType);
  bool SavePol(std::vector< std::pair< size_t, te::gm::Polygon* > >& pols, std::string step);

  void verify_polygon(std::vector<te::gm::Coord2D> &pt_out);
  bool verifyIntersections(te::gm::Coord2D &pt0, te::gm::Coord2D &pc, te::sam::rtree::Index<te::gm::Line*> &rtree_pol);
  int pointLocate(te::gm::Coord2D &pt, te::gm::Polygon *pol);


protected:
  double m_dist_min;
  std::string m_filename1; //agregado
  std::string m_inDS1;
  std::string m_filename2; 
  std::string m_inDS2;
  std::string m_filenameout; 
  std::string m_inDSout;

  int m_srid;

  std::map<size_t, te::gm::Polygon*> m_pol;  //poligonos do desflorestamento
  std::vector<te::gm::Line*> m_line;
  te::sam::rtree::Index<te::gm::Line*> m_rtree;

  KD_TREE *m_nodetree; //pontos do agregado
  KD_TREE *m_nodetree_step2; // pontos dos poligonos gerados no step2

  //std::vector< std::pair< size_t, te::gm::Polygon* > > m_pols_1; //poligonos gerados no step1
  //std::vector< std::pair< size_t, te::gm::Polygon* > > m_pols_2; //poligonos gerados mo step2
 // std::vector< std::pair< size_t, te::gm::Polygon* > > m_pols_3; //poligonos gerados mo step3
  //std::vector< std::pair< size_t, te::gm::Polygon* > > m_pols_final; //poligonos gerados mo step3 - segunda passada
  std::map<int32_t, double> m_original_area;

  std::vector<GAP_inter> m_pol_points;

  double m_tol = 0.1e-7;

};
