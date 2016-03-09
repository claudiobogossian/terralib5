%extend te::mem::DataSetItem {
  static te::mem::DataSetItem* create(te::mem::DataSet* ds)
  {
    return new te::mem::DataSetItem(ds);
  }

  void setInt16(int i, int value)
  {
    $self->setValue(i, new te::dt::Int16(value));
  }
  
  void setInt32(int i, int value)
  {
    $self->setValue(i, new te::dt::Int32(value));
  }  
  
  void setInt64(int i, int value)
  {
    $self->setValue(i, new te::dt::Int64(value));
  }  
  
  void setGeom(int i, te::gm::Geometry* geom)
  {	
	$self->setGeometry(i, geom->getEnvelope());
  }
};
