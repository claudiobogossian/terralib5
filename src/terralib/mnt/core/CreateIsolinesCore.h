/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file terralib/mnt/core/CreateIsolines.h

  \brief This file contains a class that represents the create isolines.

  \reference Adapted from TerraLib4.
*/

#ifndef __TERRALIB_MNT_INTERNAL_CREATEISOLINES_H
#define __TERRALIB_MNT_INTERNAL_CREATEISOLINES_H

// Terralib Includes
#include "Config.h"
#include "CreateIsolinesParams.h"

#include "../../../../src/terralib/dataaccess.h"
#include "../../../../src/terralib/dataaccess/datasource/DataSourceFactory.h"
#include "../../../../src/terralib/memory/DataSet.h"
#include "../../../../src/terralib/common/Holder.h"
#include "../../../../src/terralib/statistics/core/Enums.h"

// STL Includes
#include <map>
#include <memory>


namespace te
{
  //forward declarations
  namespace da  { class DataSetType; }
  namespace gm  { class Geometry; }
  namespace mem { class DataSet; }
}

struct  RasterBlockSize
{
  int m_idThread = 0;
  int m_numRows = 0;
  int m_initalRow = 0;
  int m_finalRow = 0;
};

class ConnectLinesParams
{
  public:

  /*! \brief Default constructor. */
    ConnectLinesParams();

  /*! \brief Virtual destructor. */
    ~ConnectLinesParams();

  public:

    int m_quota = 0, m_srid = 0;
    std::vector<te::gm::LineString*>  m_vecSegments;
    std::vector<te::gm::LineString*> m_lsOut;
};

class GenerateSegmentsParams
{
  public:

  /*! \brief Default constructor. */
  GenerateSegmentsParams();

  /*! \brief Virtual destructor. */
  ~GenerateSegmentsParams();

  public:

  std::auto_ptr<te::rst::Raster> m_rasterPtr;
  std::vector< std::vector<te::gm::LineString*> > m_vecSegments;
  std::vector<double> m_nvals;
};

namespace te
{
  namespace mnt
  {
    class TEMNTEXPORT CreateIsolines
    {
      /*!
      \class CreateIsolines

      \brief Class used to execute the create isolines

      */

    public:

      /*! \brief Default constructor. */
      CreateIsolines();

      /*! \brief Virtual destructor. */
      ~CreateIsolines();

    public:

      void setInput(te::da::DataSourcePtr inRasterDsrc, 
        std::string inRasterName, 
        std::auto_ptr<te::da::DataSetType> inDsetType);

      std::auto_ptr<te::rst::Raster> getPrepareRaster();

      void setParams(std::vector<double> &nval, std::vector<double> &gval, double vmax, double vmin, double dummy, bool hasDummy);
  
      void setOutput(te::da::DataSourcePtr outDsrc, std::string dsname);

      bool run(std::auto_ptr<te::rst::Raster> raster);
      
      static bool generateSegments(std::auto_ptr<te::rst::Raster> raster, std::vector<double> nvals, std::vector< std::vector<te::gm::LineString*> >& vecSegments);

      static bool connectLines(std::vector<te::gm::LineString*>  vec, int srid, std::vector<te::gm::LineString*>& lsOut);

      static void interpolacao(int direction, te::gm::LineString* line, double quota, double coord, double c_inf, double c_sup, double z_inf, double z_sup);

      static bool connectLinesThreaded(ConnectLinesParams* params);

      static bool equal(te::gm::PointZ &p1, te::gm::PointZ &p2, double &tol);

      static void firstCaseSpecial(double quota, double& lineSupLeft, double& lineSupRigth, double& lineInfLeft, double& lineInfRigth);

      static void segments(int idQuota,  te::gm::LineString* line, double quota, double ylg_sup, double xlg_ant, double xlg_pos, double ylg_inf,
        double& lineSupLeft, double& lineSupRigth, double& lineInfLeft, double& lineInfRigth, std::vector< std::vector<te::gm::LineString*> >& vecSegments);

      static bool generateSegmentsThreaded(GenerateSegmentsParams* params);

      bool SaveIso(std::vector<te::gm::LineString*> lsOut);

      void Save(te::da::DataSource* source, te::da::DataSet* result, te::da::DataSetType* outDsType);

      std::vector<RasterBlockSize> calculateBlocks(int numRows, int numThreads);

      void rstMemoryBlock(std::auto_ptr<te::rst::Raster> raster, std::vector<RasterBlockSize> vecBlocks, std::vector<GenerateSegmentsParams>& vecGenerateParams);


      void createFileShp(std::auto_ptr<te::mem::DataSet> dataSet, te::da::DataSetType* dataSetType);
      bool connectLines1(std::vector<te::gm::LineString*>  vecSegments, double scale, std::vector<te::gm::LineString*>& lsOut);
      bool connectLines(std::vector<te::gm::LineString*>  vecSegments, std::vector<te::gm::LineString*>& lsOut);
     
    protected:

      /*! Function used to create the output dataset */
      std::auto_ptr<te::mem::DataSet> createDataSet(te::da::DataSet* inputDataSet, te::da::DataSetType* dsType);

      te::da::DataSourcePtr m_inRasterDsrc;
      std::string m_inRasterName;
      std::auto_ptr<te::da::DataSetType> m_inRasterDsType;

      te::da::DataSourcePtr m_outDsrc;
      std::string m_outDsetName;
   
      std::vector<double> m_values;
      std::vector<double> m_guidevalues;
      int m_srid;
     
      static double m_vmax;
      static double m_vmin;
      static double m_dummy;
      static bool m_hasDummy;
    };
  }
}

#endif //__TERRALIB_MNT_INTERNAL_CREATEISOLINES_H