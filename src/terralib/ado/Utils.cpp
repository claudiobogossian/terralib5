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
  \file terralib/ado/Utils.cpp
   
  \brief Utility functions for ADO.  
*/

// TerraLib
#include "../common/Translator.h"
#include "../datatype.h"
#include "../dataaccess.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/Enums.h"
#include "Config.h"
#include "Utils.h"
#include "Exception.h"
#include "Globals.h"

inline void TESTHR( HRESULT hr )
{
	if( FAILED(hr) ) _com_issue_error( hr );
}

namespace te
{
  namespace ado
  {
    int ado2Terralib(ADOX::DataTypeEnum adoType)
    {

      int res;

      switch(adoType)
      {
        case ADOX::adBoolean:
          res = te::dt::BOOLEAN_TYPE;
          break;

        case ADOX::adEmpty:
          te::dt::VOID_TYPE;
          break;

        case ADOX::adBinary:
        case ADOX::adVarBinary:
        case ADOX::adLongVarBinary:
          res = te::dt::BYTE_ARRAY_TYPE;
          break;

        case ADOX::adVarWChar:
        case ADOX::adWChar:
        case ADOX::adVarChar:
        case ADOX::adLongVarChar:
        case ADOX::adLongVarWChar:
        case ADOX::adBSTR:
        case ADOX::adChar:
          res = te::dt::StringType::VAR_STRING;
          break;

        case ADOX::adBigInt:
          res = te::dt::INT16_TYPE;
          break;

        case ADOX::adDouble:
          res = te::dt::DOUBLE_TYPE;
          break;

        case ADOX::adInteger:
          res = te::dt::INT32_TYPE;
        break;

        case ADOX::adTinyInt:
        case ADOX::adSmallInt:
          res = te::dt::INT16_TYPE;
          break;

        case ADOX::adUnsignedBigInt:
          res = te::dt::UINT64_TYPE;
          break;

        case ADOX::adUnsignedInt:
          res = te::dt::UINT32_TYPE;
          break;

        case ADOX::adUnsignedSmallInt:
        case ADOX::adUnsignedTinyInt:
          res = te::dt::UINT16_TYPE;
          break;

        //case ADOX::adDate:
        //case ADOX::adDBDate:
        //case ADOX::adDBTime:
        //case ADOX::adDBTimeStamp:

        //case ADOX::adGUID:
        //case ADOX::adError:
        //case ADOX::adSingle:
        //case ADOX::adDecimal:
        //case ADOX::adNumeric:
        //case ADOX::adChapter:
        //case ADOX::adVarNumeric:
        //case ADOX::adCurrency:
        //case ADOX::adFileTime:
        //case ADOX::adPropVariant:
        //case ADOX::adUserDefined:

        default:
          throw te::ado::Exception(TR_ADO("The informed type could not be mapped to TerraLib type system!"));
        break;
      }

      return res;
    }

    ADOX::DataTypeEnum terralib2Ado(int terralib)
    {

      ADOX::DataTypeEnum res;

      switch(terralib)
      {
        case te::dt::CHAR_TYPE:
        case te::dt::UCHAR_TYPE:
          res = ADOX::adWChar;
          break;

        case te::dt::INT16_TYPE:
          res = ADOX::adInteger;
          break;

        case te::dt::INT32_TYPE:
          res = ADOX::adInteger;
          break;

        case te::dt::INT64_TYPE:
          res = ADOX::adInteger;
          break;

        //case te::dt::NUMERIC_TYPE:
        //case te::dt::DATETIME_TYPE:

        case te::dt::FLOAT_TYPE:
        case te::dt::DOUBLE_TYPE:
          res = ADOX::adDouble;
          break;

        case te::dt::STRING_TYPE:
          res = ADOX::adLongVarWChar;
          break;

        case te::dt::BOOLEAN_TYPE:
          res = ADOX::adBoolean;
          break;

        case te::dt::BYTE_ARRAY_TYPE:
          res = ADOX::adLongVarBinary;
          break;

        case te::dt::GEOMETRY_TYPE:
        case te::dt::ARRAY_TYPE:
          res = ADOX::adLongVarBinary;
          break;

        default:
          throw te::ado::Exception(TR_ADO("The informed type could not be mapped to ADO type system!"));
        break;
      }

      return res;
    }

    void addAdoPropertyFromTerralib(ADOX::_TablePtr table, te::dt::Property* prop)
    {
      int pType = prop->getType();

      switch(pType)
      {
        case te::dt::CHAR_TYPE:
        case te::dt::UCHAR_TYPE:
        case te::dt::INT16_TYPE:
        case te::dt::INT32_TYPE:
        case te::dt::INT64_TYPE:
        case te::dt::FLOAT_TYPE:
        case te::dt::DOUBLE_TYPE:
        case te::dt::BOOLEAN_TYPE:
        case te::dt::BYTE_ARRAY_TYPE:
          table->Columns->Append(prop->getName().c_str(), te::ado::terralib2Ado(pType), 0);
          break;

        case te::dt::STRING_TYPE:
        {
          te::dt::StringProperty* p = (te::dt::StringProperty*)prop;
          table->Columns->Append(prop->getName().c_str(), te::ado::terralib2Ado(pType), p->size());
          break;
        }

        //case te::dt::NUMERIC_TYPE:
        //case te::dt::DATETIME_TYPE:
        
        case te::dt::GEOMETRY_TYPE:
          table->Columns->Append(prop->getName().c_str(), te::ado::terralib2Ado(pType), 0);
          break;
          
        case te::dt::ARRAY_TYPE:
          table->Columns->Append(prop->getName().c_str(), te::ado::terralib2Ado(pType), 0);
          break;

        default:
          throw te::ado::Exception(TR_ADO("The informed type could not be mapped to ADO type system!"));
        break;
      }
    }

    te::dt::Property* getPropertyFromADO(ADOX::_ColumnPtr column)
    {
      te::dt::Property* resProp;

      _bstr_t cName = column->GetName();
      ADOX::DataTypeEnum cType = column->GetType();
      int size = column->GetDefinedSize();

      if(cType == ADOX::adDate)
      {
        //ADOX::Properties** porps;
        //column->get_Properties(porps);
      }

      switch(cType)
      {
        case ADOX::adVarWChar:
            resProp = new te::dt::StringProperty(std::string(cName), te::dt::StringType::VAR_STRING, size);
          break;

        case ADOX::adInteger:
        case ADOX::adDouble:
          resProp = new te::dt::SimpleProperty(std::string(cName), te::dt::INT64_TYPE);
          break;

        case ADOX::adLongVarBinary:
          resProp = new te::dt::ArrayProperty(std::string(cName), new te::dt::SimpleProperty(std::string(cName), te::dt::BYTE_ARRAY_TYPE));
          break;
          
        default:
          throw te::ado::Exception(TR_ADO("The informed column could not be mapped to TerraLib Data Set Type!"));
      }

      return resProp;

    }

    te::da::PrimaryKey* getPrimaryKeyFromADO(ADOX::_KeyPtr key)
    {
      te::da::PrimaryKey* pk = new te::da::PrimaryKey();

      _bstr_t cName = key->GetName();
      ADOX::ColumnsPtr cols =  key->GetColumns();
  
      pk->setName(std::string(cName));

      for(long i = 0; i < cols->GetCount(); i++)
      {
        ADOX::_ColumnPtr c = cols->GetItem(i);

        pk->add(getPropertyFromADO(c));
      }

      return pk;
    }

    te::da::ForeignKey* getForeignKeyFromADO(ADOX::_KeyPtr key)
    {
      te::da::ForeignKey* fk = 0;

      _bstr_t cName = key->GetName();
      ADOX::ColumnsPtr cols = key->GetColumns();

      for(long i = 0; i < cols->GetCount(); i++)
      {
        ADOX::_ColumnPtr c = cols->GetItem(i);

        //te::dt::Property* refP = getPropertyFromADO(refCol);

      }

      return fk;
    }

    void setPrimaryKey(te::da::DataSetType* dt, ADOX::_KeyPtr key)
    {
      te::da::PrimaryKey* pk = new te::da::PrimaryKey(std::string(key->GetName()), dt);
      ADOX::ColumnsPtr cols = key->GetColumns();
      
     for(long i = 0; i < cols->GetCount(); i++)
      {
        ADOX::_ColumnPtr c = cols->GetItem(i);

        pk->add(dt->getProperty(std::string(c->GetName())));
      }
    }

    te::dt::Property* Convert2Terralib(ADOX::_ColumnPtr column)
    {
      te::dt::Property* prop = 0;

      _bstr_t cName = column->GetName();
      ADOX::DataTypeEnum cType = column->GetType();
      long cSize = column->GetDefinedSize();
      
      switch(cType)
      {
        case ::adBoolean:
          prop = new te::dt::SimpleProperty(std::string(cName), ado2Terralib(cType));
          break;

        case ::adVarWChar:
        case ::adWChar:
        case ::adVarChar:
        case ::adLongVarChar:
        case ::adLongVarWChar:
        case ::adBSTR:
        case ::adChar:
          prop = new te::dt::StringProperty(std::string(cName), (te::dt::StringType)ado2Terralib(cType), cSize);
          break;

        case ADOX::adTinyInt:
        case ADOX::adSmallInt:
          prop = new te::dt::SimpleProperty(std::string(cName), ado2Terralib(cType));
          break;

        case ADOX::adInteger:
          prop = new te::dt::SimpleProperty(std::string(cName), ado2Terralib(cType));
          break;

        case ADOX::adBigInt:
          prop = new te::dt::SimpleProperty(std::string(cName), ado2Terralib(cType));
          break;

        case ADOX::adDouble:
          prop = new te::dt::SimpleProperty(std::string(cName), ado2Terralib(cType));
          break;

        case ::adUnsignedBigInt:
          prop = new te::dt::SimpleProperty(std::string(cName), ado2Terralib(cType));
          break;

        case ::adUnsignedInt:
          prop = new te::dt::SimpleProperty(std::string(cName), ado2Terralib(cType));
          break;

        case ::adUnsignedSmallInt:
        case ::adUnsignedTinyInt:
          prop = new te::dt::SimpleProperty(std::string(cName),ado2Terralib(cType));
          break;

        case ADOX::adLongVarBinary:
          prop = new te::dt::ArrayProperty(std::string(cName), new te::dt::SimpleProperty(std::string(cName), ado2Terralib(cType)));
          break;

        case ADOX::adDate:
        case ADOX::adDBDate:
        case ADOX::adDBTime:
        case ADOX::adDBTimeStamp:
          prop = new te::dt::DateTimeProperty(std::string(cName), te::dt::DateTimeType::TIME_INSTANT);
          break;
          
        default:
          throw te::ado::Exception(TR_ADO("The informed column could not be mapped to TerraLib Data Set Type!"));
      }

      return prop;
    }

    std::vector<te::dt::Property*> Convert2Terralib(ADOX::ColumnsPtr columns)
    {
      std::vector<te::dt::Property*> properties;

      for(long i = 0; i < columns->GetCount(); i++)
        properties.push_back(Convert2Terralib(columns->GetItem(i)));

      return properties;
    }

    te::da::Constraint* Convert2Terralib(ADOX::_KeyPtr key)
    {

      if(key->GetType() == ADOX::adKeyPrimary)
      {
        te::da::PrimaryKey* con = new te::da::PrimaryKey(std::string(key->GetName()));
        con->setProperties(Convert2Terralib(key->GetColumns()));

        return con;
      }
      else if(key->GetType() == ADOX::adKeyForeign)
      {
        te::da::ForeignKey* con = new te::da::ForeignKey(std::string(key->GetName()));
        con->setProperties(Convert2Terralib(key->GetColumns()));

        return con;
      }
      else if(key->GetType() == ADOX::adKeyUnique)
      {
        te::da::UniqueKey* con = new te::da::UniqueKey(std::string(key->GetName()));
        con->setProperties(Convert2Terralib(key->GetColumns()));

        return con;
      }
      else
        throw te::ado::Exception(TR_ADO("Unknown type!"));


    }

    bool isZProperty(te::gm::GeomType type)
    {
      if( (type >= 1000 && type < 2000) || (type >= 3000 && type < 4000) )
        return true;
      return false;
    }

    std::string getOGCType(te::gm::GeomType type)
    {
      std::string res = "";

      switch(type)
      {
        case te::gm::GeomType::GeometryType:
          res = "GEOMETRY";
          break;

        case te::gm::GeomType::PointType:
          res = "POINT";
          break;

        case te::gm::GeomType::LineStringType:
          res = "LINESTRING";
          break;

        case te::gm::GeomType::PolygonType:
          res = "POLYGON";
          break;

        case te::gm::GeomType::MultiPointType:
          res = "MULTIPOINT";
          break;

        case te::gm::GeomType::MultiLineStringType:
          res = "MULTILINESTRING";
          break;

        case te::gm::GeomType::MultiPolygonType:
          res = "MULTIPOLYGON";
          break;

        case te::gm::GeomType::CurvePolygonType:
          res = "CURVE";
          break;

        case te::gm::GeomType::PolyhedralSurfaceType:
          res = "POLYHEDRALSURFACE";
          break;
        
        case te::gm::GeomType::GeometryZType:
          res = "GEOMETRYZ";
          break;

        case te::gm::GeomType::PointZType:
          res = "POINTZ";
          break;

        case te::gm::GeomType::LineStringZType:
          res = "LINESTRINGZ";
          break;

        case te::gm::GeomType::PolygonZType:
          res = "POLYGONZ";
          break;

        case te::gm::GeomType::MultiPointZType:
          res = "MULTIPOINTZ";
          break;

        case te::gm::GeomType::MultiLineStringZType:
          res = "MULTILINESTRINGZ";
          break;

        case te::gm::GeomType::MultiPolygonZType:
          res = "MULTIPOLYGONZ";
          break;

        case te::gm::GeomType::CurvePolygonZType:
          res = "CURVEZ";
          break;

        case te::gm::GeomType::PolyhedralSurfaceZType:
          res = "POLYHEDRALSURFACEZ";
          break;
        
        case te::gm::GeomType::GeometryMType:
          res = "GEOMETRYM";
          break;

        case te::gm::GeomType::PointMType:
          res = "POINTM";
          break;

        case te::gm::GeomType::LineStringMType:
          res = "LINESTRINGM";
          break;

        case te::gm::GeomType::PolygonMType:
          res = "POLYGONM";
          break;

        case te::gm::GeomType::MultiPointMType:
          res = "MULTIPOINTM";
          break;

        case te::gm::GeomType::MultiLineStringMType:
          res = "MULTILINESTRINGM";
          break;

        case te::gm::GeomType::MultiPolygonMType:
          res = "MULTIPOLYGONM";
          break;

        case te::gm::GeomType::CurvePolygonMType:
          res = "CURVEM";
          break;

        case te::gm::GeomType::PolyhedralSurfaceMType:
          res = "POLYHEDRALSURFACEM";
          break;

        case te::gm::GeomType::GeometryZMType:
          res = "GEOMETRYZM";
          break;

        case te::gm::GeomType::PointZMType:
          res = "POINTZM";
          break;

        case te::gm::GeomType::LineStringZMType:
          res = "LINESTRINGZM";
          break;

        case te::gm::GeomType::PolygonZMType:
          res = "POLYGONZM";
          break;

        case te::gm::GeomType::MultiPointZMType:
          res = "MULTIPOINTZM";
          break;

        case te::gm::GeomType::MultiLineStringZMType:
          res = "MULTILINESTRINGZM";
          break;

        case te::gm::GeomType::MultiPolygonZMType:
          res = "MultiPolygonZM";
          break;

        case te::gm::GeomType::CurvePolygonZMType:
          res = "CURVEZM";
          break;

        case te::gm::GeomType::PolyhedralSurfaceZMType:
          res = "POLYHEDRALSURFACEZM";
          break;

        default:
          throw te::ado::Exception(TR_ADO("The informed geometry type could not be mapped to OGC Type!"));

      }

      return res;
    }

    void createGeometryColumns(_ConnectionPtr adoConn)
    {

      try
      {
        ADOX::_CatalogPtr pCatalog;

        TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));
        pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));
            
        ADOX::_TablePtr pTable = 0;
      
        TESTHR(pTable.CreateInstance(__uuidof (ADOX::Table)));

        pTable->Name = "geometry_columns";

        pTable->Columns->Append("f_table_catalog", ADOX::adVarWChar, 256);
        pTable->Columns->Append("f_table_schema", ADOX::adVarWChar, 256);
        pTable->Columns->Append("f_table_name", ADOX::adVarWChar, 256);
        pTable->Columns->Append("f_geometry_column", ADOX::adVarWChar, 256);
        pTable->Columns->Append("coord_dimension", ADOX::adInteger, 0);
        pTable->Columns->Append("srid", ADOX::adInteger, 0);
        pTable->Columns->Append("type", ADOX::adVarWChar, 30);

        TESTHR(pCatalog->Tables->Append(_variant_t((IDispatch*)pTable)));

      }
      catch(_com_error &e)
      {
        throw Exception(TR_ADO(e.ErrorMessage()));
      }

    }

    bool isGeomProperty(_ConnectionPtr adoConn, std::string tableName, std::string columnName)
    {
      ADOX::_CatalogPtr pCatalog;

      TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));
      pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

      for(long i = 0; i < pCatalog->GetTables()->Count; i++)
      {
        if(std::string(pCatalog->GetTables()->GetItem(i)->GetName()) == "geometry_columns")
        {
          _RecordsetPtr recset;
          TESTHR(recset.CreateInstance(__uuidof(Recordset)));
  
          TESTHR(recset->Open(_bstr_t("geometry_columns"),
          _variant_t((IDispatch*)adoConn,true), adOpenKeyset, adLockOptimistic, adCmdTable));

          while(!recset->EndOfFile)
          {
            if(std::string((_bstr_t)recset->Fields->GetItem("f_table_name")->Value) == tableName
              && std::string((_bstr_t)recset->Fields->GetItem("f_geometry_column")->Value) == columnName)
              return true;

            recset->MoveNext();
          }
        }
      }

      return false;
    }
  }
}

