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
		te::da::ObjectIdSet oidSet;

		te::da::DataSetType type(schema->getName());

		// Adding Oids properties
		te::da::GetOIDPropertyNames(schema.get(), pNames);

		for(size_t i=0; i<pNames.size(); i++)
		{
			te::dt::Property* prpI = schema->getProperty(pNames[i]);

			if(prpI != 0)
			{
				type.add(prpI->clone());
				oidSet.addProperty(prpI->getName(), oidSet.size(), prpI->getType());
			}
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

			oidSet.add(oIds[i]->clone());
		}

		te::da::DataSourcePtr dSrc = te::da::GetDataSource(lDSet->getDataSourceId());
		std::vector<size_t> pprps(1, pNames.size());

		std::map<std::string, std::string> opts;

		if(dSrc.get() != 0)
			dSrc->update(schema->getName(), &dset, pprps, &oidSet, opts);
	}
}

%include "terralib/maptools/AbstractLayer.h"

// Wrap function
std::vector<te::map::AbstractLayer*> OpenLayers() throw (te::common::Exception);
te::map::AbstractLayer* GetFolderLayer() throw (te::common::Exception);
