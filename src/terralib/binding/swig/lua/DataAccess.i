%extend te::da::DataSourceManager {
  void insert(te::da::DataSource* source)
  {
    te::da::DataSourcePtr src(source);
    $self->insert(src);
  }
};
