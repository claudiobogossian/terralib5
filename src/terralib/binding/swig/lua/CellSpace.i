%extend te::cellspace::CellularSpacesOperations {
  void createCellSpace(te::da::DataSourceInfo* outputSource,
					   const std::string& name,
		  		       const double& resX,
					   const double& resY,
			           const te::gm::Envelope& env,
	                   const int srid,
					   te::cellspace::CellularSpacesOperations::CellSpaceType type,
					   te::map::DataSetLayer* layerBase)
  {
    te::da::DataSourceInfoPtr ds(outputSource);
    te::map::AbstractLayerPtr layer(layerBase);
    $self->createCellSpace(ds, name, resX, resY, env, srid, type, layer);
  }
};
