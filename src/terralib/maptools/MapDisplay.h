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
  \file terralib/maptools/MapDisplay.h

  \brief The map display controls how a set of layers are displayed.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_MAPDISPLAY_H
#define __TERRALIB_MAPTOOLS_INTERNAL_MAPDISPLAY_H

// TerraLib
#include "AbstractMapDisplay.h"

namespace te
{
  namespace map
  {
    /*!
      \class MapDisplay

      \brief This class controls how a set of layers are displayed.

      \ingroup map

      \sa AbstractMapDisplay, AbstractLayer
     */
    class TEMAPEXPORT MapDisplay : public AbstractMapDisplay
    {
      public:

        /*! \brief It initializes a new MapDisplay. */
        MapDisplay();

        /*! \brief Virtual destructor. */
        virtual ~MapDisplay();

        virtual void setLayerList(const std::list<te::map::AbstractLayerPtr>& layers);

        virtual te::map::AlignType getHAlign() const;

        virtual te::map::AlignType getVAlign() const;

        virtual void setAlign(te::map::AlignType h, te::map::AlignType v);

        virtual const te::gm::Envelope& getExtent() const;

        virtual void setExtent(te::gm::Envelope& e, bool doRefresh = true);

        virtual int getSRID() const;

        virtual void setSRID(const int& srid, bool doRefresh = true);

      protected:

        te::gm::Envelope m_extent;                          //!< The display extent.
        std::list<te::map::AbstractLayerPtr> m_layerList;   //!< The layer list to be displayed.
        int m_srid;                                         //!< The display SRS.
        te::map::AlignType m_hAlign;                        //!< The display horizontal align.
        te::map::AlignType m_vAlign;                        //!< The display vertical align.
   };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_MAPDISPLAY_H
