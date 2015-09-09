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
  \file TextController.h
   
  \brief Class that represents text controller.
  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_MAP_CONTROLLER_H
#define __TERRALIB_LAYOUT_INTERNAL_MAP_CONTROLLER_H

// TerraLib
#include "../../core/Config.h"
#include "../../core/pattern/mvc/AbstractItemController.h"

#include "../../../maptools/AbstractLayer.h"

namespace te
{
  namespace layout
  {
    class AbstractItemModel;
    /*!
    \brief Class that represents text controller.
    
    \ingroup layout
    \sa te::layout::AbstractItemController
    */
    class TELAYOUTEXPORT MapController : public AbstractItemController
    {
      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */ 
        MapController(AbstractItemModel* model);

        /*!
          \brief Destructor
        */ 
        virtual ~MapController();

        /*!
          \brief Method called by the subject to inform changes in the model
        */
        virtual void update(const Subject* subject);

        /*!
          \brief Method called by the view to inform that new layers have been added
        */
        virtual void addLayers(const std::list<te::map::AbstractLayerPtr>& layerList);

        /*!
          \brief Method called by the view to inform that the extent and scale have been changed
        */
        virtual void extentChanged(const te::gm::Envelope& envelope, double scale);

        virtual void setZoom(const int& zoom);

        virtual void setProperty(const Property& property);

        virtual void setProperties(const Properties& properties);

        virtual Properties getExtentChangedProperties(const te::gm::Envelope& envelope, double scale);

        /*!
          \brief Syncs the given scale to the item (view). If the values are the same, returns FALSE. If the values had to be sync, return TRUE.
        */
        virtual bool syncScaleToItem(double scale);

        /*!
          \brief Syncs the given extent to the item (view). If the values are the same, returns FALSE. If the values had to be sync, return TRUE.
        */
        virtual bool syncExtentToItem(const te::gm::Envelope& envelope);

    protected:

        int m_zoom;
        bool m_ignoreExtentChangedEvent;
    };
  }
}
#endif //__TERRALIB_LAYOUT_INTERNAL_TEXT_CONTROLLER_H
