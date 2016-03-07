/*
 * File: AttributeFill.i
 *
 * AttributeFill module specific to Lua
 */
 
%extend te::attributefill::VectorToVectorOp {
  void setParams(std::map<std::string, std::vector<int> > options, const std::vector<std::string>& toLayerProps)
  {
    std::map<std::string, std::vector<te::attributefill::OperationType> > opts;
	
	std::map<std::string, std::vector<int> >::iterator it = options.begin();
	
	while(it != options.end())
    {
      std::vector<int> operations = it->second;
	  std::vector<te::attributefill::OperationType> ops;
      for(std::size_t i = 0; i < operations.size(); ++i)
      {
        ops.push_back((te::attributefill::OperationType)operations[i]);
      }
	  
	  opts.insert(std::make_pair(it->first, ops));
	  
      ++it;
    }		
	
    $self->setParams(opts, toLayerProps);
  }
  
  void setParams(const std::string& fromAttribute, te::attributefill::OperationType operation, te::map::AbstractLayer* toLayer)
  {
    std::map<std::string, std::vector<te::attributefill::OperationType> > options;	
	std::vector<te::attributefill::OperationType> ops;
	std::vector<std::string> toLayerProps;
	
	std::auto_ptr<te::da::DataSetType> toSchema = toLayer->getSchema();
	std::vector<te::dt::Property*> props = toSchema->getProperties();
	
    for(std::size_t i = 0; i < props.size(); ++i)
    {
	  toLayerProps.push_back(props[i]->getName());
    }	
	
	ops.push_back(operation);
	options.insert(std::make_pair(fromAttribute, ops));
	
    $self->setParams(options, toLayerProps);
  }  

  void setParams(te::map::AbstractLayer* fromLayer, te::attributefill::OperationType operation, te::map::AbstractLayer* toLayer)
  {
    std::map<std::string, std::vector<te::attributefill::OperationType> > options;
	
	std::vector<te::attributefill::OperationType> ops;
	std::vector<std::string> toLayerProps;
	
	std::auto_ptr<te::da::DataSetType> toSchema = toLayer->getSchema();
	std::vector<te::dt::Property*> toProps = toSchema->getProperties();
	
    for(std::size_t i = 0; i < toProps.size(); ++i)
    {
	  std::cout << toProps[i]->getName() << std::endl;
      toLayerProps.push_back(toProps[i]->getName());
    }	
	
	ops.push_back(operation);

	std::auto_ptr<te::da::DataSetType> fromSchema = fromLayer->getSchema();
	std::vector<te::dt::Property*> fromProps = fromSchema->getProperties();
	
    for(std::size_t i = 0; i < fromProps.size(); ++i)
    {
      options.insert(std::make_pair(fromProps[i]->getName(), ops));
    }	
	
    $self->setParams(options, toLayerProps);
  }     
  
  void setInput(te::map::AbstractLayer* fromLayer, te::map::AbstractLayer* toLayer)
  {
    te::map::AbstractLayerPtr fl(fromLayer);
    te::map::AbstractLayerPtr tl(toLayer);
	
	$self->setInput(fl, tl);	
  }
  
  te::da::DataSource* createAndSetOutput(const std::string& dsName, const std::string& dsType, 
										const std::map<std::string, std::string>& connInfo)
  {
    te::da::DataSourcePtr outSource(te::da::DataSourceFactory::make(dsType).release());
    outSource->setConnectionInfo(connInfo);
    outSource->open();
	
	$self->setOutput(outSource, dsName);
	
	return outSource.get();
  }
  
  void setOutput(te::da::DataSource* outDs, std::string dsName)
  {
	te::da::DataSourcePtr ds(outDs);
    $self->setOutput(ds, dsName);
  }   
};

%extend te::attributefill::VectorToVectorMemory {
  std::string prun()
  {
	try
	{
      $self->run();
	}
	catch (const te::common::Exception& e)
	{
	  return e.what();
	}
	catch (const std::exception& e)
	{
	  return e.what();
	}
	
	return "";
  }   
};


%extend te::attributefill::RasterToVector {
  void setInput(te::rst::Raster* raster, te::map::DataSetLayer* toLayer)
  {
	te::da::DataSourcePtr vDs = te::da::GetDataSource(toLayer->getDataSourceId(), true);
	std::string fileNameVector(toLayer->getDataSetName());
    std::auto_ptr<te::da::DataSetType>dsTypeVector = vDs->getDataSetType(fileNameVector);
    std::auto_ptr<te::da::DataSetTypeConverter> converterVector(new te::da::DataSetTypeConverter(
									dsTypeVector.get(), vDs->getCapabilities(), vDs->getEncoding()));	
	
	$self->setInput(raster, vDs, fileNameVector, converterVector, toLayer->getSelected());		
  }  
  
  void setParams(int band, te::attributefill::OperationType operation, bool texture)
  {
	std::vector<unsigned int> bands;
	bands.push_back(band);
	
	std::vector<te::stat::StatisticalSummary> stats;
	
    switch(operation)
    {
      case te::attributefill::MIN_VALUE:
        stats.push_back(te::stat::MIN_VALUE);
        break;
      case te::attributefill::MAX_VALUE:
        stats.push_back(te::stat::MAX_VALUE);
        break;
      case te::attributefill::MEAN:
        stats.push_back(te::stat::MEAN);
        break;
      case te::attributefill::SUM:
        stats.push_back(te::stat::SUM);
        break;
      case te::attributefill::COUNT:
        stats.push_back(te::stat::COUNT);
        break;
        case te::attributefill::VALID_COUNT:
        stats.push_back(te::stat::VALID_COUNT);
        break;
      case te::attributefill::STANDARD_DEVIATION:
        stats.push_back(te::stat::STANDARD_DEVIATION);
        break;
        case te::attributefill::VARIANCE:
        stats.push_back(te::stat::VARIANCE);
        break;
      case te::attributefill::SKEWNESS:
        stats.push_back(te::stat::SKEWNESS);
        break;
      case te::attributefill::KURTOSIS:
        stats.push_back(te::stat::KURTOSIS);
        break;
      case te::attributefill::AMPLITUDE:
        stats.push_back(te::stat::AMPLITUDE);
        break;
      case te::attributefill::MEDIAN:
        stats.push_back(te::stat::MEDIAN);
        break;
      case te::attributefill::VAR_COEFF:
        stats.push_back(te::stat::VAR_COEFF);
        break;
      case te::attributefill::MODE:
        stats.push_back(te::stat::MODE);
        break;
      case te::attributefill::PERCENT_EACH_CLASS:
        stats.push_back(te::stat::PERCENT_EACH_CLASS_BY_AREA);
        break;
    }
	
	$self->setParams(bands, stats, texture);
  }
  
  te::da::DataSource* createAndSetOutput(const std::string& dsName, const std::string& dsType, 
										const std::map<std::string, std::string>& connInfo)
  {
    te::da::DataSourcePtr outSource(te::da::DataSourceFactory::make(dsType).release());
    outSource->setConnectionInfo(connInfo);
    outSource->open();
	
	$self->setOutput(outSource, dsName);
	
	return outSource.get();
  }  
  
  void setOutput(te::da::DataSource* ds, const std::string& dseName)
  {
    te::da::DataSourcePtr dsptr(ds);
	$self->setOutput(dsptr, dseName);
  }
  
  std::string prun()
  {
	try
	{
      $self->run();
	}
	catch (const te::common::Exception& e)
	{
	  return e.what();
	}
	catch (const std::exception& e)
	{
	  return e.what();
	}
	
	return "";
  }   
};