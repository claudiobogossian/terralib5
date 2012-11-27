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
  \file Layer.h

  \brief A Raster Layer is a reference to a virtual dataset with raster geometry.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_RASTERLAYER_H
#define __TERRALIB_MAPTOOLS_INTERNAL_RASTERLAYER_H

// TerraLib
#include "AbstractLayer.h"

// STL
#include <memory>

namespace te
{
// Forward declarations
  namespace da
  {
    class DataSource;
  }

  namespace se 
  { 
    class RasterSymbolizer;
    class Style; 
  }
  
  namespace rst 
  { 
    class Raster;
    class RasterProperty;
  }

  namespace map
  {
// Forward declarations
    class RasterLayerRenderer;

    /*!
      \class RasterLayer

      \brief A Raster Layer is a reference to a virtual dataset with raster geometry.

      \sa AbstractLayer
    */
    class TEMAPEXPORT RasterLayer : public AbstractLayer
    {
      public:

        /*!
          \brief It initializes a new Layer.

          \param id     The layer id.
          \param title  The title is a brief description about the layer, that can be used by an application to show a meaninfull label to the user.
          \param parent The parent Layer (or NULL if it has no parent).

          \note If parent is informed, it will take the ownership of the new layer.
        */
        RasterLayer(const std::string& id, const std::string& title, AbstractLayer* parent = 0);

        /*! \brief Destructor. */
        ~RasterLayer();

         /*!
          \brief It returns the layer type.

          \return The layer type.
        */
        virtual const std::string& getType() const;

        /*!
          \brief It returns true if the layer can be drawn, otherwise, it returns false.

          This method can be used to check if the data referenced by the layer is available (accessible), or not.

          \return True, if the layer can be drawn, otherwise, it returns false.
        */
        virtual bool isValid() const;

        /*!
          \brief It returns true if the layer can be queried, otherwise, it returns false.

          Not all layers can be queried (ex: some WMS layers).

          \return True, if the layer can be queried, otherwise, it returns false.
        */
        virtual bool isQueryable() const;

        /*!
          \brief It draws the layer geographic objects in the given canvas using the informed SRS.

          The informed bounding box can be used to constraint the layer objects to be drawn.

          The bbox coordinates must be in the same Spatial Reference System given by srid.

          \param canvas The canvas were the layer objects will be drawn.
          \param bbox   The interest area to render the map.
          \param srid   The SRS to be used to draw the layer objects.
        */
        virtual void draw(Canvas* canvas, const te::gm::Envelope& bbox, int srid);

        /*!
          \brief It returns the style applied to the layer.

          \return The style applied to the layer.
        */
        virtual te::se::Style* getStyle() const;

        /*!
          \brief It sets the style to be applied to the layer.

          \param style The layer style.

          \note The layer will take the ownership of the given style.
        */
        virtual void setStyle(te::se::Style* style);

        /*!
          \brief It returns the Layer extent (or minimum bounding box). Its coordinates are in the same SRS as the layer.

          \return The Layer extent (or minimum bounding box).
        */
        virtual const te::gm::Envelope* getExtent() const;

        /*!
          \brief It sets the Layer extent (or minimum bounding box). Its coordinates must be in the same SRS as the layer.

          \param mbr The Layer extent (or minimum bounding box).

          \note The layer will take the ownership of the given MBR.
        */
        virtual void setExtent(te::gm::Envelope* mbr);

        /*!
          \brief It returns the Spatial Reference System ID associated to the Layer.

          \return The Spatial Reference System ID associated to the Layer.
        */
        virtual int getSRID() const;

        /*!
          \brief It sets the Spatial Reference System ID associated to the Layer.

          \param srid The Spatial Reference System ID to be associated to the Layer.
        */
        virtual void setSRID(int srid);
       
        /*!
          \brief It returns the datasource associated to the layer.

          \return the datasource associated to the layer.
        */
        te::da::DataSource* getDataSource() const;

        /*!
          \brief It sets the datasource associated to the layer.

          \param ds The datasource associated to the layer.
        */
        void setDataSource(te::da::DataSource* ds);

        /*!
          \brief It returns the dataset name where we retrieve the layer objects.

          \return The dataset name associated to the layer.
        */
        const std::string& getDataSetName() const;

        /*!
          \brief It sets the the dataset name where we will retrieve the layer objects.

          \param ds The dataset associated to the layer.
        */
        void setDataSetName(const std::string& datasetName);

        /*!
          \brief It returns the renderer used to paint this layer.

          \return The renderer used to paint this layer.
        */
        RasterLayerRenderer* getRenderer() const;

        /*!
          \brief It sets the renderer used to paint this layer.

          \param renderer The renderer to be used to paint this layer.

          \note The Layer will take the ownership of the given renderer.
        */
        void setRenderer(RasterLayerRenderer* renderer);


      public:

        /*!
          \brief Get the raster object from a data source

          \return  A pointer to a raster
        */
        te::rst::Raster* getRaster();
        
        /*!
          \brief Get the raster object from a data source

          \return  A pointer to a raster
        */
        te::rst::Raster const* getRaster() const;

        /*!
          \brief Get the raster property from a data source

          \return  A pointer to a raster property
        */
        const te::rst::RasterProperty* getRasterProperty();

        /*!
          \brief Get the raster symbolizer defined from a style

          \return  A pointer to a raster symbolizer
        */
        const te::se::RasterSymbolizer* getRasterSymbolizer();

      private:

        /*!
          \brief Try to get the raster objects (raster and raster property) from data source

          \note The data source and data set name must be already defined.
        */
        void loadRasterObjects();

      public:
         
        static const std::string sm_type;  //!< A static data member used in the implementation of getType method.

      private:

        int m_srid;                                        //!< The layer SRS.
        std::string m_datasetName;                         //!< The dataset name where we will retrieve the layer objects.
        te::da::DataSource* m_ds;                          //!< The DataSource associated to this Layer.
        std::auto_ptr<te::gm::Envelope> m_mbr;             //!< The Layer bounding box.
        std::auto_ptr<te::se::Style> m_style;              //!< The style to be applied to the geographic objects in the layer.
        std::auto_ptr<RasterLayerRenderer> m_renderer;     //!< A pointer to the internal renderer used to paint this layer.

        te::rst::Raster* m_raster;                          //!< A Pointer to the internal data sourcer raster object
        te::rst::RasterProperty* m_rasterProperty;          //!< A Pointer to the internal data sourcer raster property object
        const te::se::RasterSymbolizer* m_symbolizer;       //!< A pointer to the internal layer style
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_RASTERLAYER_H

