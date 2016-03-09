/*
 * File: CellSpace.i
 *
 * Cellular Space module specific to Lua
 */

%extend te::cellspace::CellularSpacesOperations {
  void createCellSpace(te::da::DataSourceInfo outputSource,
					   const std::string& name,
		  		       double resX,
					   double resY,
			           const te::gm::Envelope& env,
	                   int srid,
					   te::cellspace::CellularSpacesOperations::CellSpaceType type,
					   te::map::DataSetLayer* layerBase)
  {
	te::da::DataSourceInfoPtr ds;  
    ds.reset(new te::da::DataSourceInfo);
	*ds = outputSource;
    te::map::AbstractLayerPtr layer(layerBase);
	
    $self->createCellSpace(ds, name, resX, resY, env, srid, type, layer);
	
	ds.reset();
  }
  
  void createCellSpace(te::da::DataSourceInfo outputSource,
					   const std::string& name,
		  		       double resX,
					   double resY,
			           const te::gm::Envelope& env,
	                   int srid,
					   te::cellspace::CellularSpacesOperations::CellSpaceType type,
					   te::map::AbstractLayer* layerBase)
  {
	te::da::DataSourceInfoPtr ds;  
    ds.reset(new te::da::DataSourceInfo);
	*ds = outputSource;
    te::map::AbstractLayerPtr layer(layerBase);
	
    $self->createCellSpace(ds, name, resX, resY, env, srid, type, layer);
	
	ds.reset();
  }  
};
