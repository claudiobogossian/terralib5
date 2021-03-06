/*!
\file terralib/mnt/core/Tin.h

\brief This file contains a class to define a TIN.
Adapted from SPRING
*/

#ifndef __TERRALIB_MNT_INTERNAL_TIN_H
#define __TERRALIB_MNT_INTERNAL_TIN_H

// Terralib Includes
#include "Config.h"
#include "Enums.h"

#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/datasource/DataSource.h"

#include "../../geometry/Envelope.h"
#include "../../geometry/PointZ.h"

#include "../../sam.h"

#include <vector>

const int32_t MAXTRIANGLES = 50;
const int32_t CLNODES = 10;

typedef te::sam::kdtree::Node<te::gm::Coord2D, int32_t, int32_t> KD_NODE;
typedef te::sam::kdtree::Index<KD_NODE> KD_TREE;


namespace te
{
  namespace mnt
  {
    class TinTriang
    {
    public:
      TinTriang(){
        m_line[0] = -1;
        m_line[1] = -1;
        m_line[2] = -1;
      }

      /*! Set triangle edges.*/
      void setEdges(int32_t fLine, int32_t sLine, int32_t tLine)
      {
        m_line[0] = fLine;
        m_line[1] = sLine;
        m_line[2] = tLine;
      }

      //....Fill in vector of triangle lines Ids.
      bool LinesId(int32_t *linesId)
      {
        if (m_line[0] < 0 || m_line[1] < 0 || m_line[2] < 0)
          return false;
        linesId[0] = m_line[0];
        linesId[1] = m_line[1];
        linesId[2] = m_line[2];
        return true;
      }

      //....Get the line at an edge of the triangle.
      //....Input:
      //......edge: edge number (0, 1 or 2).
      //....Return:
      //......Line at edge number.
      int32_t LineAtEdge(unsigned short edge)
      {
        if (edge < 3)
          return m_line[edge];
        else
          return -1;
      }
    protected:
      int32_t  m_line[3];// line numbers vector
    };


    class TinLine
    {
    public:
      TinLine() : m_nodefrom(-1), m_nodeto(-1), m_leftpoly(-1), m_rightpoly(-1), m_type(Normalline) {}

      TinLine(int32_t& nodefrom, int32_t &nodeto, int32_t &leftpoly, int32_t &rightpoly, Ltype type) 
        : m_nodefrom(nodefrom), m_nodeto(nodeto), m_leftpoly(leftpoly), m_rightpoly(rightpoly), m_type(type) {}

      bool operator==(const TinLine &rhs) const;

      bool operator>(const TinLine &rhs) const;

      bool operator<(const TinLine &rhs) const;

      /*!  Set initial node number.*/
      void setNodeFrom(int32_t nodeid) { m_nodefrom = nodeid; }

      /*!  Get initial node number.*/
      int32_t getNodeFrom() { return m_nodefrom; }

      /*!  Set last node number.*/
      void setNodeTo(int32_t nodeid) { m_nodeto = nodeid; }

      /*!  Get last node number.*/
      int32_t getNodeTo() { return m_nodeto; }

      /*!  Set initial and last node numbers.*/
      void Nodes(int32_t nodefrom, int32_t nodeto)
      {
        m_nodefrom = nodefrom;
        m_nodeto = nodeto;
      }

      /*!  Set left polygon number.*/
      void setLeftPolygon(int32_t polyid) { m_leftpoly = polyid; }

      /*!  Get left polygon number.*/
      int32_t getLeftPolygon() { return m_leftpoly; }

      /*!  Set right polygon number.*/
      void setRightPolygon(int32_t polyid) { m_rightpoly = polyid; }

      /*!  Get right polygon number.*/
      int32_t getRightPolygon() { return m_rightpoly; }

      /*!  Set left and right polygon numbers*/
      void Polygons(int32_t leftpoly, int32_t rightpoly)
      {
        m_leftpoly = leftpoly;
        m_rightpoly = rightpoly;
      }

      /*!
      \brief Method for exchange polygons in the triangulation
      \param oldPolyId is the identification number of the old polygon
      \param newPolyId is the identification number of the new polygon
      \return TRUE if the polygons are the polygons are exchanged or FALSE otherwise
      */
      bool ExchangePolygon(int32_t oldPolyId, int32_t newPolyId);

      /*!
      \brief Method for exchange nodes in the triangulation
      \param oldNodeId is the identification number of the old node
      \param newNodeId is the identification number of the new node
      \return TRUE if the nodes are the polygons are exchanged or FALSE otherwise
      */
      bool ExchangeNode(int32_t oldNodeId, int32_t newNodeId);

      /*!  Set line type.*/
      void setType(Ltype ltype) { m_type = ltype; }

      /*!  Get line type.*/
      Ltype getType() { return m_type; }

      /*!
      \brief Method for swap two nodes
      \return TRUE always
      */
      bool SwapNode();

      /*!
      \brief Method for swap two adjacent polygons
      \return TRUE always
      */
      bool SwapPolygon();

      /*!
      \brief Method for swap two nodes and two adjacent polygons
      \return TRUE always
      */
      bool SwapNodePolygon();

    protected:
      int32_t m_nodefrom; //!<  initial node number
      int32_t m_nodeto; //!<  last node number
      int32_t m_leftpoly; //!<  left polygon number
      int32_t m_rightpoly; //!<  right polygon number
      Ltype m_type; //!<  line type

    };


    /*!
    \class TinNode
    Class that defines a node for triangular irregular network
    */

    class TinNode
    {

    public:
      TinNode() : m_type(Deletednode), m_point(0, 0, 0) {}

      TinNode(const TinNode &rhs){ m_type = rhs.m_type; m_edge = rhs.m_edge; m_point = rhs.m_point; }

      bool operator== (const TinNode &rhs) const;

      bool operator> (const TinNode &rhs) const;

      bool operator< (const TinNode &rhs) const;

      bool setEdge(int32_t edge);

      bool removeEdge(int32_t edge);

      std::vector<int32_t> &getEdge() { return m_edge; }

      /*! Set node height value.*/
      void setZ(double zvalue) { m_point.setZ(zvalue); }

      /*! Get node height value.*/
      double getZ() { return m_point.getZ(); }

      /*! Set node X axis coordinate. */
      void setX(double xvalue) { m_point.setX(xvalue); }

      /*! Get node X axis coordinate.*/
      double getX() { return m_point.getX(); }

      /*! Set node Y axis coordinate.*/
      void setY(double yvalue) { m_point.setY(yvalue); }

      /*! Get node Y axis coordinate.*/
      double getY() { return  m_point.getY(); }

      /*! Set node coordinates. */
      void setNPoint(te::gm::PointZ npoint) { m_point = npoint; }

      /*! Get node coordinates.*/
      te::gm::PointZ& getNPoint() { return m_point; }

      /*!Set node type. */
      void setType(Ntype ntype) { m_type = ntype; }

      /*!Get node type. */
      Ntype getType() { return m_type; }

      //!\brief Set node coordinates and height.
      //!\param npoint: Point with coordinates.
      //!\param ntype: Type (default value = NORMAL).
      void Init(te::gm::PointZ& npoint, Ntype ntype = Normalnode)
      {
        m_point = npoint;
        m_type = ntype;
      }

      //!\brief Set node coordinates, height and type.
      //!\param xvalue: X axis coordinate.
      //!\param yvalue: Y axis coordinate.
      //!\param zvalue: Height value.
      //!\param ntype: Type (default value = NORMAL).
      void Init(double xvalue, double yvalue, double zvalue, Ntype ntype = Normalnode)
      {
        m_point.setX(xvalue);
        m_point.setY(yvalue);
        m_point.setZ(zvalue);
        m_type = ntype;
      }
    protected:
      Ntype m_type; //!< node type
      te::gm::PointZ m_point; //!< Node point

    public:
      std::vector<int32_t> m_edge;
    };

    /*!
    \class Tin

    \brief Class to define TIN strutures.

    \ingroup mnt

    */
    class TEMNTEXPORT Tin
    {
    public:
      Tin() : m_nodatavalue(std::numeric_limits<double>::max()), m_min(std::numeric_limits<double>::max()), m_max(std::numeric_limits<double>::min()) {}

      /*! Function used to set the Spatial Reference System ID  */
      void setSRID(int srid);

      /*! Function used to set the envelope parameter */
      void setEnvelope(te::gm::Envelope &env);

      /*!
      \brief Method used to load a triangular network (TIN)
      \return true if the TIN is loaded with no errors or false otherwise
      */
      bool LoadTin(te::da::DataSourcePtr &inDsrc, std::string &inDsetName, double zmin = std::numeric_limits<double>::min(), double zmax = std::numeric_limits<double>::max());

      /*!
      \brief Method used to load a triangular network (TIN) generated by QGIS
      \return true if the TIN is loaded with no errors or false otherwise
      */
      bool LoadTinQGIS(te::da::DataSourcePtr &inDsrc, std::string &inDsetName, double zmin, double zmax);

      bool SaveTin(te::da::DataSourcePtr &outDsrc, std::string &outDsetName);

      bool BuildTriangle(int32_t id, te::gm::LinearRing* lr, double *val, int32_t *right, int32_t *left, te::mnt::Ntype *type, \
        double zmin, double zmax, bool &first, KD_TREE &nodetree, te::sam::rtree::Index<std::size_t>& linetree);

   protected:

      te::da::DataSetType* GetDataSetType(std::string &outDsetName);

      /*!
      \brief Method that finds a triangle containing a given point
      \param ptr1 is a pointer to a Point object
      \return true if the triangle was found or false (-1L)otherwise
      */
      int32_t FindTriangle(te::gm::PointZ &ptr1);

      /*!
      \brief Method that reads the vertex (points) of a given triangle
      \param triangId is the triangle identification number
      \param vertex is a pointer to a list of Point object, the triangle vertices
      \return true always
      */
      bool TrianglePoints(int32_t triangId, te::gm::PointZ *vertex);

      bool NeighborsId(int32_t triangId, int32_t *neighsId);

      /*!
      \brief Method that reads the identification number of the opposite node of a given edge
      \param triangId is the triangle identification number
      \param linId is the line identification number
      \return the opposite node identification considering the linId
      */
      int32_t OppositeNode(int32_t triangId, int32_t linId);

      /*!
      \brief Method that reads the identification number of the opposite edge of a given node
      \param triangId is the triangle identification number
      \param nodeId is the node identification number
      \return the opposite edge identification considering the nodeId or -1 if error
      */
      int32_t OppositeEdge(int32_t triangId, int32_t nodeId);

      /*!
      \brief Method that find the oposite lines of a specific node
      \param v is the node identification number
      \param linids is pointer to a list of line identificators
      \return true if the line is found with no errors or false otherwise
      */
      bool NodeOppositeLines(int32_t v, std::vector<int32_t> &linids);

      /*!
      \brief Method that finds out which is the previous node
      \param nodeId is the node identification number
      \return the number of the previous node
      */
      int32_t PreviousNode(int32_t nodeId);

      /*!
      \brief Method that finds out which is the next node
      \param nodeId is the node identification number
      \return the number of the next node
      */
      int32_t NextNode(int32_t nodeId);

      /*!
      \brief Method that verifies if a triangle contains a given point
      \param triangId is the triangle identification number
      \param pt is a pointer to a te::gm::PointZ object
      \return TRUE if the point is in the triangle or FALSE (-1L)otherwise
      */
      bool ContainsPoint(int32_t triangId, te::gm::PointZ &pt);

      /*!
      \brief Method that find a line containing a specific node
      \param nid is the node identification number
      \return the number of the line if it was found with no errors or FALSE (-1L) otherwise
      */
      std::vector<int32_t> FindLine(int32_t nid);

      /*!
      \brief Method that find a line given two nodes identification
      \param fnid is the first node identification number
      \param snid is the second node identification number
      \return the number of the line if it was found with no errors or FALSE (-1L) otherwise
      */
      int32_t FindLine(int32_t fnid, int32_t snid);

      /*!
      \brief Method that includes a node in the tin line list
      \param v is the node identificator number
      \param linids is a pointer to a list of line identificators
      \return TRUE if the node is included with no errors or FALSE otherwise
      */
      bool NodeLines(int32_t v, std::vector<int32_t> &linids);

      /*!
      \brief Method that reads the identification number of the nodes of a given triangle
      \param triangId is the triangle identification number
      \param vertex is the number of the vertex to be considered
      \return the node identification related to the vertex
      */
      int32_t NodeId(int32_t triangId, short vertex);

      /*!
      \brief Method that reads the identification number of the nodes of a given triangle
      \param triangId is the triangle identification number
      \param nodesIds is a pointer to a list of node identification numbers
      \return TRUE always
      */
      bool NodesId(int32_t triangId, int32_t *nodeIds);


      /*!
      \brief Method that includes a node in the tin node list
      \param v is the node identification number
      \param nodids is a pointer to a list of node identificators
      \return TRUE if the node is included with no errors or FALSE otherwise
      */
      bool NodeNodes(int32_t v, std::vector<int32_t>& nodids);

      /*!
      \brief Method that search a node in a triangulation
      \param v is the node identification number
      \return the right or left polygon of a line containing v or FALSE (-1L) otherwise
      */
     int32_t NodeTriangle(int32_t v);

      /*!
      \brief Method that includes a node in a triangle list
      \param v is the node identification number
      \param triangles is pointer to a vetor of triangles
      \return TRUE if the node is included with no errors or FALSE otherwise
      */
      bool NodeTriangles(int32_t v, std::vector<int32_t>& triangles);

      /*!
      \brief Method that reallocates Vectors
      \param nSize is the vector size
      \return true always
      */
      bool ReallocateVectors(size_t nSize);

      /*!
      \brief Method that calculates the first and second derivatives in the nodes of a given triangle
      \return TRUE if the derivatives are calculate with no errors or FALSE otherwise
      */
      bool NodeDerivatives();

      /*!
      \brief Method that calculates the first derivatives in the nodes of a given triangle
      \return TRUE if the derivatives are calculate with no errors or FALSE otherwise
      */
      bool TriangleFirstDeriv();

      /*!
      \brief Method that calculates the second derivatives in the nodes of a given triangle
      \return TRUE if the derivatives are calculate with no errors or FALSE otherwise
      */
      bool TriangleSecondDeriv();

      /*!
      \brief Method that calculates the first derivatives in the nodes of a given triangle
      \return TRUE if the derivatives are calculate with no errors or FALSE otherwise
      */
      bool NodeFirstDeriv();

      /*!
      \brief Method that calculates the second derivative at all triangulation nodes
      \return TRUE if the derivatives are calculated with no errors or False otherwise
      */
      bool NodeSecondDeriv();

      /*!
      \brief Method that find the triangle list sharing a given node
      \param nodeid is the node identification number
      \param rightri is a pointer to the triangles at right side
      \param lefttri is a pointer to the triangles at left side
      \return TRUE if the lists are find with no errors or FALSE otherwise
      */
      bool NodeTriangles(int32_t nodeid, std::vector<int32_t> &rightri, std::vector<int32_t> &leftri);

      /*!
      \brief Method that searches the closest points of a specific node
      \param nid is the node identification number
      \param clstNids is a pointer to a list of closest node identificators
      \param useBrNode is the break nodeidentification
      \return TRUE if the points are found with no errors or FALSE otherwise
      */
      bool NodeClosestPoints(int32_t nid, int32_t *clstNids, bool useBrNode = true);

      /*!
      \brief Method that calculates the first derivative in a given node
      \param nodeid is the node identification number
      \param clstNodes is the vector of a list of nodes)
      \return a Point object containing the first derivative in x and y directions
      */
      te::gm::PointZ CalcNodeFirstDeriv(int32_t nodeId, int32_t clstNodes[CLNODES]);

      /*!
      \brief Method that calculates the second derivative in a given node
      \param nodeid is the node identification number
      \param clstNIds is the vector of a list of nodes identification
      \return a Point object containing the first derivative in x and y directions
      */
      TinNode CalcNodeSecondDeriv(int32_t nodeId, int32_t clstNIds[CLNODES]);
        
        /*!
      \brief Method that calculates the first derivatives in the nodes of a given break triangle
      \return TRUE if the derivatives are calculate with no errors or FALSE otherwise
      */
      bool BreakNodeFirstDeriv();

      /*!
      \brief Method that calculates the second derivatives in the nodes of a given break triangle
      \return TRUE if the derivatives are calculate with no errors or FALSE otherwise
      */
      bool BreakTriangleSecondDeriv();

      /*!
      \brief Method that searches the Break node closest points
      \param nid is the node identification number
      \param rClstNids is a pointer to a list of right node identificators
      \param lClstNids is a pointer to a list of left node identificators
      \return TRUE if the points are found with no errors or FALSE otherwise
      */
      bool BreakNodeClosestPoints(int32_t nid, int32_t *rClstNids, int32_t *lClstNids);

      /*!
      \brief Method that calculates the second derivatives in a node of a given triangle
      \param triangles is a pointer to a list of triangle identificators (SIDList object)
      \param fderiv is a pointer to a Point object representing the first derivative in x and y directions
      \return TRUE if the derivatives are calculate with no errors or FALSE otherwise
      */
      bool CalcTriangleSecondDeriv(std::vector<int32_t> &triangles, std::vector<te::gm::PointZ> &fderiv);

      /*!
      \brief Method that calculates the second derivative at all triangulation break nodes
      \return TRUE if the derivatives are calculated with no errors or False otherwise
      */
      bool BreakNodeSecondDeriv();

      /*!
      \brief Method that check the topology in a triangulation
      \return TRUE if the topology was checked with no errors or FALSE otherwise
      */
      bool CheckTopology();

      /*!
      \brief Method that check the lines in a triangulation
      \return TRUE if the lines was checked with no errors or FALSE otherwise
      */
      bool CheckLines(int32_t trid);

      /*!
      \brief Method that evaluates Z values for pt1 and pt2 using the Akima polynomium fitted in a triangle
      \param triid is the triangle identificator number
      \param pt1 is a pointer to a Point3d object
      \param pt2 is a pointer to a Point3d object
      \return TRUE always
      */
      bool CalcZvalueAkima(int32_t triid, te::gm::PointZ &pt1, te::gm::PointZ &pt2);

      /*!
      \brief Method that defines the coefficients of the Akima polynomium fitted in a given triangle
      \param triid is the triangle identification number
      \param coef is a pointer to a double vector containing the polynomium coefficients
      \return TRUE if the coefficients are determined with no errors or FALSE otherwise
      */
      bool DefineAkimaCoeficients(int32_t triid, double *coef);

      /*!
      \brief Method that defines the coefficients of the Akima polynomium fitted in a given triangle
      \param triid is the triangle identification number
      \param nodesid is the list of triangle nodes identification
      \param p3d is a pointer to a Point3d object
      \param coef is a pointer to a double vector containing the polynomium coefficients
      \return TRUE if the coefficients are determined with no errors or FALSE otherwise
      */
      bool DefineAkimaCoeficients(int32_t triid, int32_t *nodesid, te::gm::PointZ *p3d, double *coef);

      bool FillGridValue(int32_t triid, int32_t flin, int32_t llin, int32_t fcol, int32_t lcol, double zvalue);

      /*!
      \brief Method that calculates the lines and the columns intercepted by a triangle
      \param grid is a pointer to a grid object that will be created
      \param nodesid is a vector with nodes identification of the current triangle
      \param flin and llin are the first and the last lines (rows) of the grid
      \param fcol and lcol are the first and the last columns of the grid
      \return TRUE if the gradient grid is filled or FALSE otherwise
      */
      bool DefineInterLinesColumns(int32_t *nodesid, int32_t &flin, int32_t &llin, int32_t &fcol, int32_t &lcol);

      int m_srid;                                  //!< Attribute with spatial reference information

      te::gm::Envelope m_env;                      //!< Attribute used to restrict the area to generate the samples.

      size_t m_linesize; //!< Triangulation lines vector size.
      size_t m_triangsize; //!< Triangulation triangles vector size.
      size_t m_nodesize; //!< Triangulation nodes vector size.

      std::vector<TinLine> m_line; //!< Triangulation lines vector.
      std::vector<TinTriang> m_triang; //!< Triangulation triangles vector.
      std::vector<TinNode> m_node; //!< Triangulation nodes vector.

      std::vector<te::gm::PointZ> m_tfderiv; //Pointer to triangles first derivatives vector.
      std::vector<te::gm::PointZ> m_nfderiv; //Pointer to nodes first derivatives vector.
      std::vector<te::gm::PointZ> m_nbrfderiv; //Pointer to right side nodes first derivatives vector.
      std::vector<te::gm::PointZ> m_nblfderiv; //Pointer to left side nodes first derivatives vector.
      std::vector<TinNode> m_tsderiv; //Pointer to triangles second derivatives vector.
      std::vector<TinNode> m_nsderiv; //Pointer to nodes second derivatives vector.
      std::vector<TinNode> m_nbrsderiv; //Pointer to right side nodes second derivatives vector.
      std::vector<TinNode> m_nblsderiv; //Pointer to left side nodes second derivatives vector.

      int32_t m_fbnode; //!<First break node number.
      int32_t m_lnode; //!<Triangulation last node number.
      int32_t m_ltriang; //!<Triangulation last triangle number.
      int32_t m_lline; //!<Triangulation last line number.

      double m_nodatavalue;
      double m_min;
      double m_max;

      te::rst::Raster* m_rst;
      double m_resx, m_resy;

    };

  } // end namespace mnt
} // end namespace te

#endif
