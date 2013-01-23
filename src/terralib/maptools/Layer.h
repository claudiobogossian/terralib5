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

  \brief A Layer is a reference to a virtual dataset with/without geometric attributes.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_LAYER_H
#define __TERRALIB_MAPTOOLS_INTERNAL_LAYER_H

// TerraLib
#include "AbstractLayer.h"

// STL
#include <memory>

namespace te
{
// Forward declarations
  namespace color { class ColorBar; }
  namespace da
  {
    class DataSource;
    class Select;
  }

  namespace se { class Style; }

  namespace map
  {
// Forward declarations
    class Grouping;
    class LayerRenderer;
    class LegendItem;

    /*!
      \class Layer

      \brief A Layer is a reference to a virtual dataset with/without geometric attributes.

      A Layer is the result of a query in a given DataSource.
    
      \sa AbstractLayer
    */
    class TEMAPEXPORT Layer : public AbstractLayer
    {
      public:

        /*!
          \brief It initializes a new Layer.

          \param id     The layer id.
          \param title  The title is a brief description about the layer, that can be used by an application to show a meaninfull label to the user.
          \param parent The parent Layer (or NULL if it has no parent).

          \note If parent is informed, it will take the ownership of the new layer.
        */
        Layer(const std::string& id, const std::string& title, AbstractLayer* parent = 0);

        /*! \brief Destructor. */
        ~Layer();

        const std::string& getType() const;

        bool isValid() const;

        bool isQueryable() const;

        void draw(Canvas* canvas, const te::gm::Envelope& bbox, int srid);

        te::se::Style* getStyle() const;

        void setStyle(te::se::Style* style);

        const te::gm::Envelope* getExtent() const;

        void setExtent(te::gm::Envelope* mbr);

        int getSRID() const;

        void setSRID(int srid);
       
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
        LayerRenderer* getRenderer() const;

        /*!
          \brief It sets the renderer used to paint this layer.

          \param renderer The renderer to be used to paint this layer.

          \note The Layer will take the ownership of the given renderer.
        */
        void setRenderer(LayerRenderer* renderer);

        /*!
          \brief It gets the grouping parameters used to generate the layer legend.

          \output It returns the grouping parameters of the legend layerr.
        */
        Grouping* getGrouping() const;

        /*!
          \brief It sets the grouping parameters to be used to generate the layer legend.

          \param grouping The grouping to be used to generate the layer legend.

          \note This class will take the ownership of the pointer to the grouping argument.
        */
        void setGrouping(Grouping* grouping);

        /*!
          \brief It checks if this layer has an associated legend.

          \output It returns true if the layer has an legend associated to this layer.
        */
        bool hasLegend();

        /*!
          \brief It gets the legend associated to this layer.

          \output The legend associated to this layer.
        */
        std::vector<te::map::LegendItem*> getLegend() const;

        /*!
          \brief It removes the legend associated to this layer.
        */
        void removeLegend();

        /*!
          \brief It inserts the given legend to this layer.

          \param The legend to be associated to this layer.
        */
        void insertLegend(const std::vector<LegendItem*>& legend);

        /*!
          \brief It gets the color bar associated to this layer.

          \output The color bar associated to this layer.
        */
        te::color::ColorBar* getColorBar() const;

        /*!
          \brief It sets the color bar to be associated to this layer.

          \param colorBar The color bar to be associated to this layer.

          \note This class will take the ownership of the pointer to the color bar argument.
        */
        void setColorBar(te::color::ColorBar* colorBar);

      public:
         
        static const std::string sm_type;  //!< A static data member used in the implementation of getType method.

      private:

        int m_srid;                                     //!< The SRS of the layer.
        std::string m_datasetName;                      //!< The dataset name where we will retrieve the layer objects.
        te::da::DataSource* m_ds;                       //!< The DataSource associated to this Layer.
        std::auto_ptr<te::gm::Envelope> m_mbr;          //!< The Layer bounding box.
        std::auto_ptr<te::se::Style> m_style;           //!< The style to be applied to the geographic objects in the layer.
        std::auto_ptr<LayerRenderer> m_renderer;        //!< A pointer to the internal renderer used to paint this layer.
        std::auto_ptr<Grouping> m_grouping;             //!< The grouping used to generate the layer legend.
        std::vector<LegendItem*> m_legend;              //!< The legend associated to this layer.
        std::auto_ptr<te::color::ColorBar> m_colorBar;  //!< The color bar associated to this layer.
    };

    typedef boost::intrusive_ptr<Layer> LayerPtr;

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_LAYER_H

