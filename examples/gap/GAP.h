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


#include <vector>

typedef te::sam::kdtree::Node<te::gm::Coord2D, int32_t, int32_t> KD_NODE;
typedef te::sam::kdtree::Index<KD_NODE> KD_TREE;

enum Pol_OUT
{
  Polygon,
  Line, 
  Point, 
  RTree
};

class GAP
{
public:

  GAP();

  bool run();
  bool run2();
  bool run1();

  bool LoadPolygons(std::string &filename, std::string &inDsetName, Pol_OUT out);

  bool Polygon2Points(te::gm::Polygon* pol, int32_t id);
  bool Polygon2RTree(te::gm::Polygon* pol);
  bool Polygon2Lines(te::gm::Polygon* pol);

  void Save(te::da::DataSource* source, te::da::DataSet* result, te::da::DataSetType* outDsType);

protected:
  double m_dist_min;
  std::string m_filename1;
  std::string m_inDS1;
  std::string m_filename2;
  std::string m_inDS2;

  int m_srid;

  std::map<size_t, te::gm::Polygon*> m_pol;
  std::vector<te::gm::Line*> m_line;
  te::sam::rtree::Index<te::gm::Line*> m_rtree;

  KD_TREE *m_nodetree;



};
