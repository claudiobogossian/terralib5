/*
 * File: Xml.i
 *
 * XML module specific to Lua
 */

%extend te::xml::AbstractWriter {
  void writeDataSourceList()
  {
    $self->writeStartElement("te_da:DataSourceList");

    $self->writeAttribute("xmlns:te_common", "http://www.terralib.org/schemas/common");

    te::da::DataSourceInfoManager::iterator itBegin = te::da::DataSourceInfoManager::getInstance().begin();
    te::da::DataSourceInfoManager::iterator itEnd = te::da::DataSourceInfoManager::getInstance().end();
    te::da::DataSourceInfoManager::iterator it;

    //std::vector<std::string> dsIdVec;
    //for (std::list<te::map::AbstractLayerPtr>::const_iterator itL = layers.begin(); itL != layers.end(); ++itL)
    //  dsIdVec.push_back(itL->get()->getDataSourceId());

    for(it=itBegin; it!=itEnd; ++it)
    {
	  // Verify why use this
      //if (std::find(dsIdVec.begin(), dsIdVec.end(), it->second->getId()) == dsIdVec.end())
      //  continue;

      bool ogrDsrc = it->second->getAccessDriver() == "OGR";

      $self->writeStartElement("te_da:DataSource");

      $self->writeAttribute("id", it->second->getId());
      $self->writeAttribute("type", it->second->getType());
      $self->writeAttribute("access_driver", it->second->getAccessDriver());

      $self->writeStartElement("te_da:Title");
      $self->writeValue((!ogrDsrc) ? it->second->getTitle() : te::common::ConvertLatin1UTFString(it->second->getTitle()));
      $self->writeEndElement("te_da:Title");

      $self->writeStartElement("te_da:Description");
      $self->writeValue((!ogrDsrc) ? it->second->getDescription() : te::common::ConvertLatin1UTFString(it->second->getDescription()));
      $self->writeEndElement("te_da:Description");

      $self->writeStartElement("te_da:ConnectionInfo");
      std::map<std::string, std::string> info = it->second->getConnInfo();
      std::map<std::string, std::string>::iterator conIt;

      for(conIt=info.begin(); conIt!=info.end(); ++conIt)
      {
        $self->writeStartElement("te_common:Parameter");

        $self->writeStartElement("te_common:Name");
        $self->writeValue(conIt->first);
        $self->writeEndElement("te_common:Name");

        $self->writeStartElement("te_common:Value");
        $self->writeValue((ogrDsrc && (conIt->first == "URI" || conIt->first == "SOURCE")) ? te::common::ConvertLatin1UTFString(conIt->second) : conIt->second);
        $self->writeEndElement("te_common:Value");

        $self->writeEndElement("te_common:Parameter");
      }
      $self->writeEndElement("te_da:ConnectionInfo");

      $self->writeEndElement("te_da:DataSource");
    }

    $self->writeEndElement("te_da:DataSourceList");
  }  
};
