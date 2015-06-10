/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file terralib/dataaccess/dataset_fw.h
   
  \brief Forward declarations for the dataset module.
 */

#ifndef __TERRALIB_DATAACCESS_INTERNAL_DATASET_FW_H
#define __TERRALIB_DATAACCESS_INTERNAL_DATASET_FW_H

// Forward declarations
namespace te
{
  namespace da
  {
    class CheckConstraint;
    class Constraint;
    class DataSet;
    class DataSetAdapter;
    class DataSetPersistence;
    class DataSetType;
    class DataSetTypePersistence;
    class ForeignKey;
    class Index;    
    class PrimaryKey;
    class Sequence;
    class UniqueKey;

  } // end namespace da
}   // end namespace te

#endif // __TERRALIB_DATAACCESS_INTERNAL_DATASET_FW_H

