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

int te::ado::ado2Terralib(ADOX::DataTypeEnum adoType)
{
  switch(adoType)
  {
    case ADOX::adBoolean:
      return te::dt::BOOLEAN_TYPE;
      break;

    case ADOX::adEmpty:
      return te::dt::VOID_TYPE;
      break;

    case ADOX::adBinary:
    case ADOX::adVarBinary:
    case ADOX::adLongVarBinary:
      return te::dt::BYTE_ARRAY_TYPE;
      break;

    case ADOX::adVarWChar:
    case ADOX::adWChar:
    case ADOX::adVarChar:
    case ADOX::adLongVarChar:
    case ADOX::adLongVarWChar:
    case ADOX::adBSTR:
    case ADOX::adChar:
      return te::dt::VAR_STRING;
      break;

    case ADOX::adBigInt:
      return te::dt::INT16_TYPE;
      break;

    case ADOX::adDouble:
      return te::dt::DOUBLE_TYPE;
      break;

    case ADOX::adInteger:
      return te::dt::INT32_TYPE;
    break;

    case ADOX::adTinyInt:
    case ADOX::adSmallInt:
      return te::dt::INT16_TYPE;
      break;

    case ADOX::adUnsignedBigInt:
      return te::dt::UINT64_TYPE;
      break;

    case ADOX::adUnsignedInt:
      return te::dt::UINT32_TYPE;
      break;

    case ADOX::adUnsignedSmallInt:
    case ADOX::adUnsignedTinyInt:
      return te::dt::UINT16_TYPE;
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
}

ADOX::DataTypeEnum te::ado::terralib2Ado(int terralib)
{
  switch(terralib)
  {
    case te::dt::CHAR_TYPE:
    case te::dt::UCHAR_TYPE:
      return ADOX::adWChar;
      break;

    case te::dt::INT16_TYPE:
      return ADOX::adInteger;
      break;

    case te::dt::INT32_TYPE:
      return ADOX::adInteger;
      break;

    case te::dt::INT64_TYPE:
      return ADOX::adInteger;
      break;

    //case te::dt::NUMERIC_TYPE:
    //case te::dt::DATETIME_TYPE:

    case te::dt::FLOAT_TYPE:
    case te::dt::DOUBLE_TYPE:
      return ADOX::adDouble;
      break;

    case te::dt::STRING_TYPE:
      return ADOX::adLongVarWChar;
      break;

    case te::dt::BOOLEAN_TYPE:
      return ADOX::adBoolean;
      break;

    case te::dt::BYTE_ARRAY_TYPE:
      return ADOX::adLongVarBinary;
      break;

    case te::dt::GEOMETRY_TYPE:
    case te::dt::ARRAY_TYPE:
      return ADOX::adLongVarBinary;
      break;

    default:
      throw te::ado::Exception(TR_ADO("The informed type could not be mapped to ADO type system!"));
    break;
  }
}

void te::ado::addAdoPropertyFromTerralib(ADOX::_TablePtr table, te::dt::Property* prop)
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

te::dt::Property* te::ado::getPropertyFromADO(ADOX::_ColumnPtr column)
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
        resProp = new te::dt::StringProperty(std::string(cName), te::dt::VAR_STRING, size);
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

te::dt::Property* te::ado::Convert2Terralib(ADOX::_ColumnPtr column)
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
      prop = new te::dt::DateTimeProperty(std::string(cName), te::dt::TIME_INSTANT);
      break;
          
    default:
      throw te::ado::Exception(TR_ADO("The informed column could not be mapped to TerraLib Data Set Type!"));
  }

  return prop;
}

std::vector<te::dt::Property*> te::ado::Convert2Terralib(ADOX::ColumnsPtr columns)
{
  std::vector<te::dt::Property*> properties;

  for(long i = 0; i < columns->GetCount(); i++)
    properties.push_back(Convert2Terralib(columns->GetItem(i)));

  return properties;
}

te::da::Constraint* te::ado::Convert2Terralib(ADOX::_KeyPtr key)
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

bool te::ado::isZProperty(te::gm::GeomType type)
{
  if( (type >= 1000 && type < 2000) || (type >= 3000 && type < 4000) )
    return true;
  return false;
}

const std::string& te::ado::GetGeometryName(te::gm::GeomType t)
{
  switch(t)
  {
    case te::gm::GeometryType:
      return Globals::sm_geometryTypeName;
      break;

    case te::gm::PointType:
      return Globals::sm_pointTypeName;
      break;

    case te::gm::LineStringType:
      return Globals::sm_lineStringTypeName;
      break;

    case te::gm::PolygonType:
      return Globals::sm_polygonTypeName;
      break;

    case te::gm::MultiPointType:
      return Globals::sm_multiPointTypeName;
      break;

    case te::gm::MultiLineStringType:
      return Globals::sm_multiLineStringTypeName;
      break;

    case te::gm::MultiPolygonType:
      return Globals::sm_multiPolygonTypeName;
      break;

    case te::gm::CurvePolygonType:
      return Globals::sm_curvePolygonTypeName;
      break;

    case te::gm::PolyhedralSurfaceType:
      return Globals::sm_polyhedralSurfaceTypeName;
      break;

    case te::gm::GeometryZType:
      return Globals::sm_geometryZTypeName;
      break;

    case te::gm::PointZType:
      return Globals::sm_pointZTypeName;
      break;

    case te::gm::LineStringZType:
      return Globals::sm_lineStringZTypeName;
      break;

    case te::gm::PolygonZType:
      return Globals::sm_polygonZTypeName;
      break;

    case te::gm::MultiPointZType:
      return Globals::sm_multiPointZTypeName;
      break;

    case te::gm::MultiLineStringZType:
      return Globals::sm_multiLineStringZTypeName;
      break;

    case te::gm::MultiPolygonZType:
      return Globals::sm_multiPolygonZTypeName;
      break;

    case te::gm::CurvePolygonZType:
      return Globals::sm_curvePolygonZTypeName;
      break;

    case te::gm::PolyhedralSurfaceZType:
      return Globals::sm_polyhedralSurfaceZTypeName;
      break;

    case te::gm::GeometryMType:
      return Globals::sm_geometryMTypeName;
      break;

    case te::gm::PointMType:
      return Globals::sm_pointMTypeName;
      break;

    case te::gm::LineStringMType:
      return Globals::sm_lineStringMTypeName;
      break;

    case te::gm::PolygonMType:
      return Globals::sm_polygonMTypeName;
      break;

    case te::gm::MultiPointMType:
      return Globals::sm_multiPointMTypeName;
      break;

    case te::gm::MultiLineStringMType:
      return Globals::sm_multiLineStringMTypeName;
      break;

    case te::gm::MultiPolygonMType:
      return Globals::sm_multiPolygonMTypeName;
      break;

    case te::gm::CurvePolygonMType:
      return Globals::sm_curvePolygonMTypeName;
      break;

    case te::gm::PolyhedralSurfaceMType:
      return Globals::sm_polyhedralSurfaceMTypeName;
      break;

    case te::gm::GeometryZMType:
      return Globals::sm_geometryZMTypeName;
      break;

    case te::gm::PointZMType:
      return Globals::sm_pointZMTypeName;
      break;

    case te::gm::LineStringZMType:
      return Globals::sm_lineStringZMTypeName;
      break;

    case te::gm::PolygonZMType:
      return Globals::sm_polygonZMTypeName;
      break;

    case te::gm::MultiPointZMType:
      return Globals::sm_multiPointZMTypeName;
      break;

    case te::gm::MultiLineStringZMType:
      return Globals::sm_multiLineStringZMTypeName;
      break;

    case te::gm::MultiPolygonZMType:
      return Globals::sm_multiPolygonZMTypeName;
      break;

    case te::gm::CurvePolygonZMType:
      return Globals::sm_curvePolygonZMTypeName;
      break;

    case te::gm::PolyhedralSurfaceZMType:
      return Globals::sm_polyhedralSurfaceZMTypeName;
      break;

    default:
      return Globals::sm_geometryTypeName;
  }
}

bool te::ado::isGeomProperty(_ConnectionPtr adoConn, std::string tableName, std::string columnName)
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

void te::ado::Blob2Variant(const char* blob, int size, _variant_t & var)
{
  char *pByte;

  SAFEARRAY FAR* psa;
  SAFEARRAYBOUND rgsabound[1];
  rgsabound[0].lLbound = 0;
  rgsabound[0].cElements = size;

  psa = SafeArrayCreate(VT_I1, 1, rgsabound);

  if(SafeArrayAccessData(psa,(void **)&pByte) == NOERROR)
    memcpy(pByte, blob, size);
  SafeArrayUnaccessData(psa);

  var.vt = VT_ARRAY | VT_UI1;
  var.parray = psa;
}

void te::ado::Variant2Blob(const _variant_t var, int size, char* & blob)
{
  char *cdata = 0;
  if(var.vt == (VT_ARRAY | VT_UI1))
  {
    SafeArrayAccessData(var.parray,(void **)&cdata);
    blob = new char[size];
    memcpy(blob, cdata, size);
    SafeArrayUnaccessData(var.parray);
  }
}