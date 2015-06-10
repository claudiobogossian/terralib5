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
  \file DataAccess.h
   
  \brief JavaScript exporting routine for the TerraLib Data Access module.
 */

#ifndef __TERRALIB_BINDING_V8_JSI_DATAACCESS_INTERNAL_DATAACCESS_H
#define __TERRALIB_BINDING_V8_JSI_DATAACCESS_INTERNAL_DATAACCESS_H

// TerraLib
#include "../Config.h"

// Google V8
#include <v8.h>

namespace te
{
// Forward declaration
  namespace da { class DataSource; }

  namespace v8
  {
    namespace jsi
    {
      /*!
        \brief It returns a reference to the persistent template of a AbstractConnectionPool object.

        \return A reference to the persistent template of a AbstractConnectionPool object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetAbstractConnectionPoolTemplate();

      /*!
        \brief It returns a reference to the persistent template of a BatchExecutor object.

        \return A reference to the persistent template of a BatchExecutor object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetBatchExecutorTemplate();

      /*!
        \brief It returns a reference to the persistent template of a PreparedQuery object.

        \return A reference to the persistent template of a PreparedQuery object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetPreparedQueryTemplate();

      /*!
        \brief It returns a reference to the persistent template of a DataSourceCatalogLoader object.

        \return A reference to the persistent template of a DataSourceCatalogLoader object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetDataSourceCatalogLoaderTemplate();

      /*!
        \brief It returns a reference to the persistent template of a DataSourceTransactor object.

        \return A reference to the persistent template of a DataSourceTransactor object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetDataSourceTransactorTemplate();

      /*!
        \brief It registers the DataSourceCatalog class.

        You can use the DataSourceCatalog class like:
        \code
        var c = new TeDataSourceCatalog();
        \endcode       

        \param global The global object that will be used to register the DataSourceCatalog class

        \exception Exception It may throws an exception if it is not possible to register the class.
       */
      void RegisterDataSourceCatalog(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a DataSourceCatalog object.

        \return A reference to the persistent template of a DataSourceCatalog object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetDataSourceCatalogTemplate();

      /*!
        \brief It registers the GeometryFactory class.

        You can use the GeometryFactory class as a singleton using the following syntax:
        \code
        var mygeom = TeGeometryFactory.make(geomtype_id, srid);
        \endcode       

        \param global The global object that will be used to register the GeometryFactory class

        \exception Exception It may throws an exception if it is not possible to register the class.
       */
      void RegisterDataSourceManager(::v8::Local<::v8::Object>& global);

      /*!
        \brief It registers the DataSourceFactory class.

        You can use the DataSourceFactory class as a singleton using the following syntax:
        \code
        var ds = DataSourceFactory.make("POSTGIS");
        \endcode       

        \param global The global object that will be used to register the DataSourceFactory class

        \exception Exception It may throws an exception, if it is not possible to register the class.
       */
      void RegisterDataSourceFactory(::v8::Local<::v8::Object>& global);

      /*!
        \brief It registers the DataSource class.

        You can use the DataSource class like:
        \code
        TeDataSource.open();
        \endcode       

        \param global The global object that will be used to register the DataSource class

        \exception Exception It may throw an exception, if it is not possible to register the class.
       */
      void RegisterDataSource(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a DataSource object.

        \return A reference to the persistent template of a Geometry object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetDataSourceTemplate();

      /*!
        \brief It registers the CheckConstraint class.

        You can use the CheckConstraint class like:
        \code
        var c = new TeCheckConstraint(dt, id);
        \endcode       

        \param global The global object that will be used to register the CheckConstraint class

        \exception Exception It may throw an exception if it is not possible to register the class.
       */
      void RegisterCheckConstraint(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a CheckConstraint object.

        \return A reference to the persistent template of a CheckConstraint object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetCheckConstraintTemplate();

      /*!
        \brief It returns a reference to the persistent template of a Constraint object.

        \return A reference to the persistent template of a Constraint object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetConstraintTemplate();

      /*!
        \brief It returns a reference to the persistent template of a DataSet object.

        \return A reference to the persistent template of a DataSet object.
       */

      ::v8::Persistent<::v8::FunctionTemplate>& GetDataSetTemplate();

      /*!
        \brief It returns a reference to the persistent template of a DataSetItem object.

        \return A reference to the persistent template of a DataSetItem object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetDataSetItemTemplate();

      /*!
        \brief It returns a reference to the persistent template of a DataSetPersistence object.

        \return A reference to the persistent template of a DataSetPersistence object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetDataSetPersistenceTemplate();

      /*!
        \brief It registers the DataSetType class.

        You can use the DataSetType class like:
        \code
        var type = new TeSetType(name, id);
        \endcode       

        \param global The global object that will be used to register the DataSetType class

        \exception Exception It may throw an exception, if it is not possible to register the class.
       */
      void RegisterDataSetType(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a DataSetType object.

        \return A reference to the persistent template of a DataSetType object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetDataSetTypeTemplate();

      /*!
        \brief It returns a reference to the persistent template of a DataSetTypePersistence object.

        \return A reference to the persistent template of a DataSetTypePersistence object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetDataSetTypePersistenceTemplate();

      /*!
        \brief It registers the ForeignKey class.

        You can use the ForeignKey class like:
        \code
        var c = new TeForeignKey(id);
        \endcode       

        \param global The global object that will be used to register the ForeignKey class

        \exception Exception It may throw an exception, if it is not possible to register the class.
       */
      void RegisterForeignKey(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a ForeignKey object.

        \return A reference to the persistent template of a ForeignKey object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetForeignKeyTemplate();

      /*!
        \brief It registers the Index class.

        You can use the Index class like:
        \code
        var c = new TeIndex(parent, id);
        \endcode       

        \param global The global object that will be used to register the Index class

        \exception Exception It may throw an exception, if it is not possible to register the class.
       */
      void RegisterIndex(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a Index object.

        \return A reference to the persistent template of a Index object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetIndexTemplate();

      /*!
        \brief It registers the PrimaryKey class.

        You can use the PrimaryKey class like:
        \code
        var c = new TePrimaryKey(datasettype, id);
        \endcode       

        \param global The global object that will be used to register the PrimaryKey class

        \exception Exception It may throw an exception, if it is not possible to register the class.
       */
      void RegisterPrimaryKey(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a PrimaryKey object.

        \return A reference to the persistent template of a PrimaryKey object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetPrimaryKeyTemplate();

      /*!
        \brief It registers the Sequence class.

        You can use the Sequence class like:
        \code
        var c = new TeSequence(catalog, id);
        \endcode       

        \param global The global object that will be used to register the Sequence class

        \exception Exception It may throw an exception, if it is not possible to register the class.
       */
      void RegisterSequence(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a Sequence object.

        \return A reference to the persistent template of a Sequence object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetSequenceTemplate();

      /*!
        \brief It registers the UniqueKey class.

        You can use the UniqueKey class like:
        \code
        var uk = new TeUniqueKey(datasettype, id);
        \endcode       

        \param global The global object that will be used to register the UniqueKey class

        \exception Exception It may throw an exception, if it is not possible to register the class.
       */
      void RegisterUniqueKey(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a UniqueKey object.

        \return A reference to the persistent template of a UniqueKey object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetUniqueKeyTemplate();

      /*!
        \brief It registers the ArrayProperty class.

        You can use the ArrayProperty class like:
        \code
        var p = new TeArrayProperty(name, elementtype);
        \endcode       

        \param global The global object that will be used to register the ArrayProperty class

        \exception Exception It may throw an exception, if it is not possible to register the class.
       */
      void RegisterArrayProperty(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a ArrayProperty object.

        \return A reference to the persistent template of a ArrayProperty object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetArrayPropertyTemplate();

      /*!
        \brief It registers the CompositeProperty class.

        You can use the CompositeProperty class like:
        \code
        var p = new TeCompositeProperty(cname, name, id, parent);
        \endcode       

        \param global The global object that will be used to register the CompositeProperty class

        \exception Exception It may throw an exception, if it is not possible to register the class.
       */
      void RegisterCompositeProperty(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a CompositeProperty object.

        \return A reference to the persistent template of a CompositeProperty object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetCompositePropertyTemplate();

      /*!
        \brief It registers the DateTimeProperty class.

        You can use the DateTimeProperty class like:
        \code
        var p = new TeDateTimeProperty(name, datetype);
        \endcode       

        \param global The global object that will be used to register the DateTimeProperty class

        \exception Exception It may throw an exception, if it is not possible to register the class.
       */
      void RegisterDateTimeProperty(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a DateTimeProperty object.

        \return A reference to the persistent template of a DateTimeProperty object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetDateTimePropertyTemplate();

       /*!
        \brief It registers the GeometryProperty class.

        You can use the GeometryProperty class like:
        \code
        var gp = new TeGeometryProperty(name, srid, subtype);
        \endcode       

        \param global The global object that will be used to register the GeometryProperty class

        \exception Exception It may throw an exception, if it is not possible to register the class.
       */
      void RegisterGeometryProperty(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a GeometryProperty object.

        \return A reference to the persistent template of a GeometryProperty object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetGeometryPropertyTemplate();

       /*!
        \brief It registers the NumericProperty class.

        You can use the NumericProperty class like:
        \code
        var gp = new TeNumericProperty(name, precision, scale);
        \endcode       

        \param global The global object that will be used to register the NumericProperty class

        \exception Exception It may throw an exception, if it is not possible to register the class.
       */
      void RegisterNumericProperty(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a NumericProperty object.

        \return A reference to the persistent template of a NumericProperty object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetNumericPropertyTemplate();     
     
      /*!
        \brief It registers the Property class.

        You can use the Property class like:
        \code
        var p = new TeProperty(other_property);
        \endcode       

        \param global The global object that will be used to register the Property class

        \exception Exception It may throw an exception if it is not possible to register the class.
       */
      void RegisterProperty(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a Property object.

        \return A reference to the persistent template of a Property object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetPropertyTemplate();

       /*!
        \brief It registers the PropertyVisitor class.

        You can use the PropertyVisitor class like:
        \code
        var p = new TePropertyVisitor();
        \endcode       

        \param global The global object that will be used to register the PropertyVisitor class

        \exception Exception It may throw an exception if it is not possible to register the class.
       */
      void RegisterPropertyVisitor(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a PropertyVisitor object.

        \return A reference to the persistent template of a PropertyVisitor object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetPropertyVisitorTemplate();

       /*!
        \brief It registers the RasterProperty class.

        You can use the RasterProperty class like:
        \code
        var p = new TeRasterProperty(name);
        \endcode       

        \param global The global object that will be used to register the RasterProperty class

        \exception Exception It may throw an exception if it is not possible to register the class.
       */
      void RegisterRasterProperty(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a RasterProperty object.

        \return A reference to the persistent template of a RasterProperty object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetRasterPropertyTemplate();

      /*!
        \brief It registers the SimpleProperty class.

        You can use the SimpleProperty class like:
        \code
        var p = new TeSimpleProperty(name, datatype);
        \endcode       

        \param global The global object that will be used to register the SimpleProperty class

        \exception Exception It may throw an exception, if it is not possible to register the class.
       */
      void RegisterSimpleProperty(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a SimpleProperty object.

        \return A reference to the persistent template of a SimpleProperty object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetSimplePropertyTemplate();

      /*!
        \brief It registers the StringProperty class.

        You can use the StringProperty class like:
        \code
        var p = new TeSimpleProperty(name, strtype);
        \endcode       

        \param global The global object that will be used to register the StringProperty class

        \exception Exception It may throw an exception, if it is not possible to register the class.
       */
      void RegisterStringProperty(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a StringProperty object.

        \return A reference to the persistent template of a StringProperty object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetStringPropertyTemplate();

    } // end namespace jsi
  }   // end namespace v8
}     // end namespace te

#endif  // __TERRALIB_BINDING_V8_JSI_DATAACCESS_INTERNAL_DATAACCESS_H

