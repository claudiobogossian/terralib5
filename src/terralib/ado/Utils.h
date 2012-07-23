/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/ado/Utils.h
   
  \brief Utility functions for ADO.  
*/

#ifndef __TERRALIB_ADO_INTERNAL_UTILS_H
#define __TERRALIB_ADO_INTERNAL_UTILS_H

// ADO
#import "msado15.dll" \
    no_namespace rename("EOF", "EndOfFile")
#import "msadox.dll"

namespace te
{
  namespace da
  {
    class DataSetType;
    class PrimaryKey;
    class ForeignKey;
    class UniqueKey;
    class Constraint;
  }

  namespace dt
  {
    class Property;
  }

  namespace ado
  {

    int ado2Terralib(ADOX::DataTypeEnum adoType);
    ADOX::DataTypeEnum terralib2Ado(int terralib);
    te::dt::Property* getPropertyFromADO(ADOX::_ColumnPtr column);
    te::da::PrimaryKey* getPrimaryKeyFromADO(ADOX::_KeyPtr key);
    te::da::ForeignKey* getForeignKeyFromADO(ADOX::_KeyPtr key);
    te::da::UniqueKey* getUniqueKeyFromADO(ADOX::_KeyPtr key);
    void setPrimaryKey(te::da::DataSetType* dt, ADOX::_KeyPtr key);

    void addAdoPropertyFromTerralib(ADOX::_TablePtr table, te::dt::Property* prop);

    int getAdoColumnId(ADOX::_TablePtr table, std::string& name);

    te::dt::Property* Convert2Terralib(ADOX::_ColumnPtr column);
    te::da::Constraint* Convert2Terralib(ADOX::_KeyPtr key);
    std::vector<te::dt::Property*> Convert2Terralib(ADOX::ColumnsPtr columns);

  } // end namespace ado
}   // end namespace te

#endif  // __TERRALIB_ADO_INTERNAL_UTILS_H
