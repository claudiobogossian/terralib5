#define TEMAPEXPORT

%warnfilter(401) AbstractLayer;

%rename (Layer) AbstractLayer;

%{
#include "terralib/maptools/FolderLayer.h"
#include "terralib/qt/widgets/se/StyleDockWidget.h"
#include "terralib/binding/swig/Styler.h"
#include "terralib/memory/DataSet.h"
#include "terralib/memory/DataSetItem.h"
%}

#ifdef SWIGJAVA
// ---------------------------------
// Java Documentation
// ---------------------------------
%typemap(javaimports) te::map::AbstractLayer "
/**
 * This is the base class for layers.
 */"

%javamethodmodifiers te::map::AbstractLayer::getId() const "
/**
 * It returns the layer id.
 *
 * @return The layer id.
 */
 public";

%javamethodmodifiers te::map::AbstractLayer::getTitle() const "
/**
 * It returns the layer title.
 *
 * @return The layer title.
 */
 public";

%javamethodmodifiers te::map::AbstractLayer::getDescendants() "
/**
 * It returns a list with the descendants of this layer.
 *
 * @return A list containing the descendants of this layer.
 */
 public";

%javamethodmodifiers te::map::AbstractLayer::getVisibility() const "
/**
 * It returns the layer visibility.
 * <p>
 * This information can be used, for instance, to know if the layer must be drawn.
 *
 * @return The layer visibility.
 */
 public";

%javamethodmodifiers te::map::AbstractLayer::setVisibility(Visibility v) "
/**
 * It sets the layer visibility.
 * <p>
 * This information can be used, for instance, to know if the layer must be drawn.
 *
 * @param v The layer visibility.
 */
 public";

%javamethodmodifiers te::map::AbstractLayer::getExtent() const "
/**
 * It returns the Layer extent (or minimum bounding box).
 *
 * @return The Layer extent (or minimum bounding box) with coordinates in the same SRS as the layer.
 */
 public";

%javamethodmodifiers te::map::AbstractLayer::getSRID() const "
/**
 * It returns the Spatial Reference System ID associated to the Layer.
 *
 * @return The Spatial Reference System ID associated to the Layer.
 */
 public";

%javamethodmodifiers te::map::AbstractLayer::getGeomPropertyName() const "
/**
 * Returns the name of the geometric property.
 *
 * @return The name of the geometric property.
 */
 public";

%javamethodmodifiers te::map::AbstractLayer::setGeomPropertytName(const std::string& name) "
/**
 * Updates the name of the geometric property.
 *
 * @param name The new name.
 */
 public";

%javamethodmodifiers te::map::AbstractLayer::isValid() const "
/**
 * It returns true if the layer can be used for instance to draw, otherwise, it returns false.
 * <p>
 * This method can be used to check if the data referenced by the layer is available (accessible), or not.
 *
 * @return True, if the layer is valid, otherwise, it returns false.
 */
 public"; 
 
%javamethodmodifiers OpenLayers() throw (te::common::Exception) "
/**
 * Opens a window for the user to select, from available datasources, some list of layers.
 *
 * @return A list with the selected layers.
 *
 * @throws java.io.IOException An exception will be raised if the operation fail.
 */
  public";

%javamethodmodifiers GetFolderLayer() throw (te::common::Exception) "
/**
 * Returns the selected layers as children of a single layer called a folder layer.
 *
 * @return A single layer containing all selected layers.
 *
 * @throws java.io.IOException An exception will be raised if the operation fail.
 */
 public";

%javamethodmodifiers te::map::AbstractLayer::toString() "
/**
 * Returns the title of the layer.
 *
 * @return The title of the layer.
 */
 public";

%javamethodmodifiers te::map::AbstractLayer::changeStyle() "
/**
 * Presents a dialog for the user to update the style of rendering of the layer.
 * <p>
 * After the update, the layer will always be rendered with the new style.
 */
 public";

%javamethodmodifiers te::map::AbstractLayer::getGeometries(const te::gm::Envelope& bbox) "
/**
 * Returns a br.inpe.terralib5.DataSet bounded by <i>bbox</i>.
 *
 * @param bbox The bounding rectangle of the desired dataset.
 *
 * @return A DataSet or <i>null</i> if no data intersects the desired bounding rectangle.
 */
 public";
	
%javamethodmodifiers te::map::AbstractLayer::getObjectIdColsNames() "
/**
 * Returns the names of columns that compose the identifier of each feature.
 *
 * @return A set of names of the columns that compose the identifier of features.
 */
 public";
	
%javamethodmodifiers te::map::AbstractLayer::updateGeometry(te::da::ObjectId* oId, te::gm::Geometry* geom) "
/**
 * Updates, in the data source, the geometry <i>geom</i> identified by <i>oId</i>.
 *
 * @param oId Idebtifier of the geometry.
 *
 * @param geom The geometry to be updated.
 */
 public";

%javamethodmodifiers te::map::AbstractLayer::updateGeometries(std::vector<te::da::ObjectId*> oIds, std::vector<te::gm::Geometry*> geoms) throw (te::common::Exception) "
/**
 * Updates a set o geometries in the data source.
 * <p>
 * Note that the <i>oIds</i> set and <i>geoms</i> set MUST have the same size. Each element in <i>oIds</i> identifies a geometry in <i>geoms</i> set.
 *
 * @param oIds Set of identifiers.
 *
 * @param geoms Set of geometries to be updated.
 *
 * @throws java.io.IOException If the data source could not update the geometries an exception will be raised.
 */
 public";
#endif

/*
 * Map tools enums
 */
%ignore te::map::ImageType;
%ignore te::map::LineDashStyle;
%ignore te::map::LineCapStyle;
%ignore te::map::LineJoinStyle;
%ignore te::map::PtMarkerType;
%ignore te::map::AlignType;
%ignore te::map::GroupingType;
%ignore te::map::ChartType;

%include "terralib/maptools/Enums.h"

namespace std {
 %template(LayerVector) vector<te::map::AbstractLayer*>;
}

%nodefaultctor te::map::AbstractLayer;

%ignore te::map::AbstractLayer::setId;
%ignore te::map::AbstractLayer::setTitle;
%ignore te::map::AbstractLayer::getAncestors;
%ignore te::map::AbstractLayer::hasVisibilityChanged;
%ignore te::map::AbstractLayer::setVisibilityAsChanged;
%ignore te::map::AbstractLayer::updateVisibilityOfAncestors;
%ignore te::map::AbstractLayer::updateVisibility;
%ignore te::map::AbstractLayer::setExtent;
%ignore te::map::AbstractLayer::setSRID;
%ignore te::map::AbstractLayer::select;
%ignore te::map::AbstractLayer::getSelected;
%ignore te::map::AbstractLayer::deselect;
%ignore te::map::AbstractLayer::clearSelected;
%ignore te::map::AbstractLayer::getStyle;
%ignore te::map::AbstractLayer::setStyle;
%ignore te::map::AbstractLayer::getGrouping;
%ignore te::map::AbstractLayer::setGrouping;
%ignore te::map::AbstractLayer::getChart;
%ignore te::map::AbstractLayer::setChart;
%ignore te::map::AbstractLayer::getSchema;
%ignore te::map::AbstractLayer::getData;
%ignore te::map::AbstractLayer::getType;
%ignore te::map::AbstractLayer::draw;

%newobject OpenLayers() throw (te::common::Exception);
%newobject GetFolderLayer() throw (te::common::Exception);

%{
	static std::vector<te::map::AbstractLayer*> OpenLayers() throw (te::common::Exception)
	{
		std::vector<te::map::AbstractLayer*> layers;
		
		// Get the layer(s) to be added
		std::auto_ptr<te::qt::widgets::DataSourceSelectorDialog> dselector(new te::qt::widgets::DataSourceSelectorDialog());

    dselector->activateWindow();
    dselector->raise();

		int retval = dselector->exec();

		if(retval == QDialog::Rejected)
		  return layers;

		std::list<te::da::DataSourceInfoPtr> selectedDatasources = dselector->getSelecteds();

		if(selectedDatasources.empty())
		  return layers;

		dselector.reset(0);

		const std::string& dsTypeId = selectedDatasources.front()->getType();

		const te::qt::widgets::DataSourceType* dsType = te::qt::widgets::DataSourceTypeManager::getInstance().get(dsTypeId);

		std::auto_ptr<QWidget> lselectorw(dsType->getWidget(te::qt::widgets::DataSourceType::WIDGET_LAYER_SELECTOR));

		if(lselectorw.get() == 0)
		  throw te::common::Exception("No layer selector widget found for this type of data source.");

		te::qt::widgets::AbstractLayerSelector* lselector = dynamic_cast<te::qt::widgets::AbstractLayerSelector*>(lselectorw.get());

		if(lselector == 0)
		  throw te::common::Exception("Wrong type of object for layer selection!");

		lselector->set(selectedDatasources);

		std::list<te::map::AbstractLayerPtr> lss = lselector->getLayers();

		lselectorw.reset(0);

		std::list<te::map::AbstractLayerPtr>::const_iterator it;
		
		for(it=lss.begin(); it!=lss.end(); ++it)
		{
			te::map::AbstractLayer* l = (*it).get();
			l->attach();
			layers.push_back(l);
		}
		
		return layers;
	}
	
	static te::map::AbstractLayer* GetFolderLayer() throw (te::common::Exception)
	{
		std::vector<te::map::AbstractLayer*> ls = OpenLayers();
    std::vector<te::map::AbstractLayer*>::iterator it;

    te::map::FolderLayer* l = new te::map::FolderLayer("xxx", "Layers");

    te::common::TreeItemPtr item_l(0);

    for(it=ls.begin(); it!=ls.end(); ++it)
    {
      item_l.reset(*it);
      l->add(item_l);
    }

    return l;
	}
%}

%newobject te::map::AbstractLayer::getGeometries(const te::gm::Envelope& bbox);

%extend te::map::AbstractLayer {
	std::string toString()
	{
		return self->getTitle();
	}
	
	void changeStyle()
	{
		Styler st(self);

    st.setWindowTitle(QObject::tr("Change layer style"));
		
		st.activateWindow();
		
		st.raise();
		
		st.exec();
	}
	
	te::da::DataSet* getGeometries(const te::gm::Envelope& bbox)
	{
		std::auto_ptr<te::map::LayerSchema> schema = self->getSchema();

		te::gm::GeometryProperty* prp = te::da::GetFirstGeomProperty(schema.get());

		if(prp != 0)
			return self->getData(prp->getName(), &bbox).release();

		return 0;
	}
	
	std::vector<std::string> getObjectIdColsNames()
	{
		std::vector<std::string> res;

		std::auto_ptr<te::map::LayerSchema> schema = self->getSchema();
		
		te::da::GetOIDPropertyNames(schema.get(), res);
		
		return res;
	}
	
	void updateGeometry(te::da::ObjectId* oId, te::gm::Geometry* geom)
	{
	}

	void updateGeometries(std::vector<te::da::ObjectId*> oIds, std::vector<te::gm::Geometry*> geoms) throw (te::common::Exception)
	{
    te::map::DataSetLayer* lDSet = dynamic_cast<te::map::DataSetLayer*>(self);

		if(lDSet == 0)
			return;

		std::auto_ptr<te::map::LayerSchema> schema = self->getSchema();
		std::vector<std::string> pNames;

		te::da::DataSetType type(schema->getName());

		// Adding Oids properties
		te::da::GetOIDPropertyNames(schema.get(), pNames);

    std::vector< std::set<int> > allCols;
    std::vector< size_t > ids;

		for(size_t i=0; i<pNames.size(); i++)
		{
			te::dt::Property* prpI = schema->getProperty(pNames[i]);

			if(prpI != 0)
				type.add(prpI->clone());

      ids.push_back(i);
		}

		// Adding geometry property
		te::dt::Property* prpS = te::da::GetFirstSpatialProperty(schema.get());
		type.add(prpS->clone());

		// Creating memory data set
		te::mem::DataSet dset(&type);

		//Adding values.
		for(size_t i=0; i<oIds.size(); i++)
		{
			te::mem::DataSetItem* item = new te::mem::DataSetItem(&dset);
			boost::ptr_vector<te::dt::AbstractData> data = oIds[i]->getValue();

			for(size_t j=0; j<data.size(); j++)
				item->setValue(j, data[j].clone());

			geoms[i]->setSRID(self->getSRID());

			item->setValue(data.size(), geoms[i]);

			dset.add(item);

      std::set<int> cols;
      cols.insert(data.size());
  
      allCols.push_back(cols);
		}

		te::da::DataSourcePtr dSrc = te::da::GetDataSource(lDSet->getDataSourceId());

		if(dSrc.get() != 0)
			dSrc->update(schema->getName(), &dset, allCols, ids);
  }
}

%include "terralib/maptools/AbstractLayer.h"

// Wrap function
std::vector<te::map::AbstractLayer*> OpenLayers() throw (te::common::Exception);
te::map::AbstractLayer* GetFolderLayer() throw (te::common::Exception);
