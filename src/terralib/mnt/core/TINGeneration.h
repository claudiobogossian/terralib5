
/*!
\file terralib/mnt/core/TINGeneration.h

\brief This file contains a class to generate TIN.
 Adapted from SPRING

 */

#ifndef __TERRALIB_MNT_INTERNAL_TINGENERATION_H
#define __TERRALIB_MNT_INTERNAL_TINGENERATION_H

// Terralib Includes
#include "Config.h"
#include "Tin.h"

#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/datasource/DataSource.h"

#include "../../geometry/MultiLineString.h"
#include "../../geometry/MultiPoint.h"
#include "../../geometry/PointZ.h"

namespace te
{
  namespace mnt
  {
    
    /*!
    \enum InputType
    \brief Input types.
    */
    enum InputType
    {
      Isolines,
      Samples
    };
    /*!
    \class TINGeneration

    \brief Class to generate TIN.

    \ingroup mnt

    */
    class TEMNTEXPORT TINGeneration : public Tin
    {
    public:
      /*! \brief Default constructor. */
      TINGeneration();

      ~TINGeneration();

      /*!
      \brief Generate TIN
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
        std::auto_ptr<te::da::DataSetType> inDsetType,
        InputType type);

      /*!
      \brief It sets the  BreakLine Datasource that is being used to generate TIN.
      \param inDsrc The datasource contains breaklines.
      \param inDsetName
      \param inDsetType
      \param tol breaklines simplification tolerance in meters.
      */
      void setBreakLine(te::da::DataSourcePtr inDsrc, std::string inDsetName, std::auto_ptr<te::da::DataSetType> inDsetType, double tol);

      /*!
      \brief It sets the Datasource that is being used to save TIN.
      \param inDsrc The output datasource.
      \param dsname
      */
      void setOutput(te::da::DataSourcePtr outDsrc, std::string dsname);

      /*!
      \brief It sets the parameters to generate TIN.
      \param tolerance lines simplification tolerance in meters.
      \param maxdist lines simplification maximum distance in meters.
      \param minedgesize edges minimum size in meters.
      \param atrz_iso isolines attribute contains Z value.
      \param atrz_pt samples  attribute contains Z value.
      */
      void setParams(const double& tolerance,
        const double &maxdist,
        const double &minedgesize,
        const std::string &atrz_iso, const std::string &atrz_pt);

      /*! Function used to set Triangulation lines simplification tolerance */
      void setTolerance(double tolerance) { m_tolerance = tolerance; };

      /*! Function used to set Triangulation lines simplification maximum distance */
      void setMaxdist(double maxdist) { m_maxdist = maxdist; };

      /*! Function used to set Triangulation edges minimum size */
      void setMinedgesize(double minedgesize) { m_minedgesize = minedgesize; };

      /*! Function used to set Triangulation method Delanay or Smaller Angle */
      void setMethod(int method) { m_method = method; }

    protected:
      size_t ReadPoints(te::gm::MultiPoint &mpt, std::string &geostype);

      size_t ReadSamples(te::gm::MultiPoint &mpt, te::gm::MultiLineString &isolines, std::string &geostype);

      size_t ReadBreakLines(te::gm::MultiPoint &mpt, te::gm::MultiLineString &isolines, std::string &geostype);

      /*! Create the two initial triangles, based on box.*/
      bool CreateInitialTriangles(size_t nsamples);

      /*! Convert samples to nodes and insert them in Tin.
      If farther than a tolerance from the closest point.*/
      bool InsertNodes(const te::gm::MultiPoint &mpt, const te::gm::MultiLineString &mls);

      bool SaveTin();

      /*!
      \brief Method used to insert a node in a TIN (Triangular Irregular Network)
      \param nodeId is the node identification number
      \param type is the node type (0 - if Delaunay)
      \return true if the node is inserted with no errors or false otherwise
      */
      bool InsertNode(int32_t nodeid, int type);

      /*!
      \brief Method used to delete a node in a TIN (Triangular Irregular Network)
      \param node is the node identification number
      \return true if the node is deleted with no errors or false otherwise
      */
      bool DeleteNode(int32_t node);

      /*!
      \brief Method used to exchange two nodes in a TIN (Triangular Irregular Network)
      \param oldNnode is the old node identification number
      \param newNnode is the new node identification number
      \return true if the nodes are exchanged with no errors or false otherwise
      */
      bool NodeExchange(int32_t oldNode, int32_t newNode);

      /*!
      \brief Method used to create two new triangles in a TIN (Triangular Irregular Network)
      \param t is a triangle identification number
      \param nodeId is a node identification number
      \param testLines is a pointer to a vector with number of lines to be tested
      \return true if the triangles are created with no errors or false otherwise
      */
      bool TwoNewTriangles(int32_t t, int32_t nodeId, int32_t* testLines);

      /*!
      \brief Method used to duplicate a triangle in a TIN (Triangular Irregular Network)
      \param t is a triangle identification number
      \param n is the node identification number
      \param v is a vertex identification number
      \param testLines is a pointer to a vector with number of lines to be tested
      \return id of new edge the duplicated triangles with no errors or -1 otherwise
      */
      int32_t DuplicateTriangle(int32_t t, short n, int32_t v, int32_t* testLines);

      /*!
      \brief Method used to duplicate a neighbour triangle in a TIN (Triangular Irregular Network)
      \param tv is a triangle identification number
      \param an0 is an edge identification number
      \param v is a vertex identification number
      \param testLines is a pointer to a vector with number of lines to be tested
      \return true if the neighbor triangle is duplicated with no errors or false otherwise
      */
      bool DupNeighTriangle(int32_t tv, int32_t an0, short, int32_t v, int32_t *testLines);

      /*!
      \brief Method used to test if the triangulation follows the Delaunay rule
      \param tri1Id is the identification number of the first triangle
      \param tri2Id is the identification number of the second triangle
      \param tri3Id is the identification number of the third triangle
      \return true if the triangulation is Delaunay or FALSE otherwise
      */

      bool TestDelaunay(int32_t tri1Id, int32_t tri2Id, int32_t tri3Id);
      /*!
      \brief Method used to test if the triangulation follows the Delaunay rule
      \param tri1Id is the identification number of the first triangle
      \param tri2Id is the identification number of the second triangle
      \param tri3Id is the identification number of the third triangle
      \param tri4Id is the identification number of the fourth triangle
      \return true if the triangulation is Delaunay or false otherwise
      */
      bool TestDelaunay(int32_t tri1Id, int32_t tri2Id, int32_t tri3Id, int32_t tri4Id);

      /*!
      \brief Method used to test if the triangulation follows the Delaunay rule
      \param linId is the identification number of a line
      \return true if the triangulation is Delaunay or false otherwise
      */
      bool TestDelaunay(int32_t linId);

      /*!
      \brief Method used to test if the triangulation follows the Delaunay rule
      \param triId is a triangle identification number
      \param nviz is the number of the neighbor of the triangle triId
      \return true if the triangulation is Delaunay or false otherwise
      */
      bool TestDelaunay(int32_t triId, short nviz);

      /*!
      \brief Method used to Update Triangles of a given triangulation
      \param t is a triangle identification number
      \param tv is a triangle identification number
      \param ai is a side number of a triangle
      \return true if the triangles are updated or false otherwise
      */
      bool UpdateTriangles(int32_t t, int32_t tv, int32_t ai);

      
      /*!
      \brief Method that exchanges polygon
      \param triangId is the triangle identification number
      \param newPolyId is the new Polygon identification number
      \param edge is the edge number of the triangle
      \return the line identification
      */
      int32_t ExchangePolygon(int32_t triangId, int32_t newPolyId, short edge);

      /*!
      \brief Method that veryfies if the Isoline is a segment
      \param linid is the line identification number
      \return TRUE if the isoline is a segment or False otherwise
      */
      bool IsIsolineSegment(int32_t linid);

      /*!
      \brief Method that veryfies if the Breakline is a segment
      \param linid is the line identification number
      \return TRUE if the breakline is a segment or False otherwise
      */
      bool IsBreaklineSegment(int32_t linid);

      bool IsNeighborOnIsoOrBreakline(int32_t triId, short nviz);

      /*!
      \brief Method that creates a Delaunay triangulation
      \return TRUE if the triangulation is created with no errors or False otherwise
      */
      bool CreateDelaunay();

      /*!
      \brief Method that generates a Delaunay triangulation
      \param nt is the triangle number
      \param ntbase is the base triangle number
      \param contr is a counter
      \return TRUE if the triangulation is generated with no errors or False otherwise
      */
      bool GenerateDelaunay(int32_t nt, int32_t ntbase, int32_t contr);

      /*!
      \brief Method that modifies the bounds of Triangles
      \return TRUE if the triangulation is generated with no errors or False otherwise
      */
      bool ModifyBoundTriangles();

      /*!
      \brief Method that define the Constrained Isolines
      \return TRUE if the Isolines are defined or FALSE otherwise
      */
      bool IsolinesConstrained();

      /*!
      \brief Method that test the Isolines
      \return true if the isolines are modified or false otherwise
      */
      bool TestIsolines();

      /*!
      \brief Method used to load a triangular network (TIN)
      \return true if the TIN is loaded with no errors or false otherwise
      */
      bool LoadTin();


      /*!
      \brief Method used to create a Triangulation using the Minimum Angle Method
      \return TRUE if the triangulation is created with no errors or FALSE otherwise
      */
      bool CreateMinAngleTriangulation();

      /*!
      \brief Method used to test the angle between two normals
      \param triId is the triangle identification number
      \param nviz  is the number of the neighbour triangle
      \return TRUE if the angle is smaller than old triangles or FALSE otherwise
      */
      bool TestAngleBetweenNormals(int32_t triId, short nviz);
      bool InsertBreakNodes(te::gm::MultiLineString &breaklines);
      bool InsertBreakLines();


      /*!
      \brief Method fint the point that intersects two triangles containing points pf and pn
      \param pf is a pointer to the first point
      \param pn is a pointer to the last point
      \param p3d is a pointer to a list of Point3d objects
      \return TRUE if the point is found with no errors or FALSE otherwise
      */
      bool FindInterPoints(te::gm::PointZ &pf, te::gm::PointZ &pn, std::vector<te::gm::PointZ> &p3d, std::vector<bool> &fixed);

      /*!
      \brief Method that checks if a point 3D is on the isoline segment
      \param linid is the line identification number
      \param pt3d is a pointer to a list of Point3D objects
      \return TRUE if the point is on the isoline segment or FALSE otherwise
      */
      bool OnIsolineSegment(int32_t linid, te::gm::PointZ &pt3d, bool &fixed);

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

      /*!
      \brief Method that order lines
      \return TRUE if the lines were ordered with no errors or FALSE otherwise
      */
      bool OrderLines();

      /*!
      \brief Method that recreates a Delaunay triangulation
      \return TRUE if the triangulation is recreated with no errors or False otherwise
      */
      bool ReCreateDelaunay();


      /*!
      \brief Method that regenerates a Delaunay triangulation
      \param nt is the triangle number
      \param ntbase is the base triangle number
      \param contr is a counter
      \return TRUE if the triangulation is regenerated with no errors or False otherwise
      */
      bool ReGenerateDelaunay(int32_t nt, int32_t ntbase, int32_t contr);

    protected:

      te::da::DataSourcePtr m_inDsrc_sample;
      std::string m_inDsetName_sample;
      std::auto_ptr<te::da::DataSetType> m_inDsetType_sample;

      te::da::DataSourcePtr m_inDsrc_point;
      std::string m_inDsetName_point;
      std::auto_ptr<te::da::DataSetType> m_inDsetType_point;

      te::da::DataSourcePtr m_inDsrc_break;
      std::string m_inDsetName_break;
      std::auto_ptr<te::da::DataSetType> m_inDsetType_break;

      te::da::DataSourcePtr m_outDsrc;
      std::string m_outDsetName;

      std::string m_atrZ_sample;
      std::string m_atrZ_point;

      //te::gm::MultiPoint m_Points;
      //te::gm::MultiLineString m_Lines;

      double m_tolerance; //!< Triangulation lines simplification tolerance.
      double m_maxdist; //!< Triangulation lines simplification maximum distance.
      double m_minedgesize; //!< Triangulation edges minimum size.
   
      double m_tolerance_break; //!< Triangulation breaklines simplification tolerance.

      int m_method; //!< Triangulation method Delanay or Smaller Angle
    };
  } // end namespace mnt
} // end namespace te

#endif

