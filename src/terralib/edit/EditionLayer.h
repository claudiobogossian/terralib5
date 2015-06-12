/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/edit/EditionLayer.h

  \brief This class represents a layer being created from user-edition.
*/

#ifndef __TERRALIB_EDIT_INTERNAL_EDITIONLAYER_H
#define __TERRALIB_EDIT_INTERNAL_EDITIONLAYER_H

// TerraLib
#include "../maptools/AbstractLayer.h"
#include "Config.h"

namespace te
{
  namespace edit
  {
    /*!
      \class EditionLayer

      \brief This class represents a layer being created from user-edition.
    */
    class TEEDITEXPORT EditionLayer : public te::map::AbstractLayer
    {
      public:

        /*!
          \brief It initializes a new layer.

          \param parent The parent layer (NULL if it has no parent).
        */
        EditionLayer(te::map::AbstractLayer* parent = 0);

        /*!
          \brief It initializes a new layer.

          \param id     The layer id.
          \param parent The parent layer (NULL if it has no parent).
        */
        EditionLayer(const std::string& id, te::map::AbstractLayer* parent = 0);

        /*!
          \brief It initializes a new Layer.

          \param id     The layer id.
          \param title  The title is a brief description about the layer.
          \param parent The parent layer (NULL if it has no parent).
        */
        EditionLayer(const std::string& id, const std::string& title, te::map::AbstractLayer* parent = 0);

        /*! \brief Destructor. */
        ~EditionLayer();

        std::auto_ptr<te::map::LayerSchema> getSchema() const;

        void setSchema(te::map::LayerSchema* schema);

        std::auto_ptr<te::da::DataSet> getData(te::common::TraverseType travType = te::common::FORWARDONLY,
                                               const te::common::AccessPolicy accessPolicy = te::common::RAccess) const;

        std::auto_ptr<te::da::DataSet> getData(const std::string& propertyName,
                                               const te::gm::Envelope* e,
                                               te::gm::SpatialRelation r = te::gm::INTERSECTS,
                                               te::common::TraverseType travType = te::common::FORWARDONLY,
                                               const te::common::AccessPolicy accessPolicy = te::common::RAccess) const;

        std::auto_ptr<te::da::DataSet> getData(const std::string& propertyName,
                                               const te::gm::Geometry* g,
                                               te::gm::SpatialRelation r,
                                               te::common::TraverseType travType = te::common::FORWARDONLY,
                                               const te::common::AccessPolicy accessPolicy = te::common::RAccess) const;

        std::auto_ptr<te::da::DataSet> getData(te::da::Expression* restriction,
                                               te::common::TraverseType travType = te::common::FORWARDONLY,
                                               const te::common::AccessPolicy accessPolicy = te::common::RAccess) const;

        std::auto_ptr<te::da::DataSet> getData(const te::da::ObjectIdSet* oids,
                                               te::common::TraverseType travType = te::common::FORWARDONLY,
                                               const te::common::AccessPolicy accessPolicy = te::common::RAccess) const;

        bool isValid() const;

        void draw(te::map::Canvas* canvas, const te::gm::Envelope& bbox, int srid);

        /*!
          \brief It returns the layer type: EDITIONLAYER.

          \return The layer type: EDITIONLAYER.
        */
        const std::string& getType() const;

        const std::string& getRendererType() const;

        void setRendererType(const std::string& t);

      private:

        te::map::LayerSchema* m_schema;    //!< The layer schema.
        std::string m_rendererType;        //!< A pointer to the internal renderer used to paint this layer.
        static const std::string sm_type;  //!< A static data member used in the implementation of getType method.
    };

    typedef boost::intrusive_ptr<EditionLayer> EditionLayerPtr;

  } // end namespace edit
}   // end namespace te

#endif  // __TERRALIB_EDIT_INTERNAL_EDITIONLAYER_H
