/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/geometry/GeometryFile.h

  \brief A class for read a wkb encoded geometry file.
*/

#ifndef __TERRALIB_PLUGINEDIT_INTERNAL_GEOMETRYFILE_H
#define __TERRALIB_PLUGINEDIT_INTERNAL_GEOMETRYFILE_H

// TerraLib
#include "../../../common/Exception.h"

// STL
#include <fstream>
#include <map>
#include <memory>
#include <vector>

namespace te
{
  // Forward declarations
  namespace gm
  {
    class Geometry;
  }
}

/*!
     * \class GeometryFile
     *
     * \brief A class that can parses a file with wkb encoded geometries.
     *
     * \ingroup geometry
     */
class GeometryFile
{
public:

  /*!
       * \name Initializer methods.
       * Methods for initializing and destructing object.
       */
  //@{

  /*!
       * \brief Constructor.
       */
  GeometryFile();

  /*!
       * \brief Destructor.
       */
  ~GeometryFile();
  //@}

  /*!
       * \brief Opens the file \a fileName.
       *
       * \param fileName The name of geometry file. (File extension is .geom)
       *
       * \exception If the file could not be opened an te::common::Exception will be raised.
       *
       * \note There's no check about the file contents, just for its existence. It means that if an unknown type of file was passed,
       * it will try to open it. The behavior of the class then is unknown.
       */
  void openFile(const char* fileName) throw (te::common::Exception);

  /*!
       * \brief Returns the next geometry.
       *
       * If the setGeometries was used, then the geometries to be returned are the geometries identified by ids given to the function,
       * otherwise it will traverse the entire file.
       *
       * \return The next geometry on the file or NULL if there's no more geometries to get.
       *
       * \note The ownership of the returned geometries are given to the caller.
       */
  te::gm::Geometry* next();

  /*!
       * \brief Sets the desired geometries to get from file.
       *
       * This is a filter of the traversing file. If this is used before next was called, only geometries indentified by \a gids
       * will be returned by \a next function. If not defined the file will be entirely traversed.
       *
       * \param gids Ids of the geometries to be filtered. These values comes from 'FID' column.
       */
  void setGeometries(const std::vector<unsigned int>& gids);

  static void writeGeometries(const char* fileName, const std::map<int, te::gm::Geometry*>& geoms);

  static void writeGeometries(const char* fileName, const std::vector<te::gm::Geometry*>& geoms);

protected:

  std::auto_ptr<std::ifstream> m_file;      //!< Pointer to the geometry file.
  std::auto_ptr<std::ifstream> m_posFile;   //!< Pointer to the geometry pos file.
  std::vector<unsigned int> m_gids;         //!< Selected ids to use on traverse operation.
  unsigned int m_i;                         //!< Current position on traversing.
};


#endif  // __TERRALIB_PLUGINEDIT_INTERNAL_GEOMETRYFILE_H



