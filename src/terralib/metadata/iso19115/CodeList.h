/*  Copyright (C) 2001-2013 National Institute For Space Research (INPE) - Brazil.
 
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
 \file terralib/md/CodeList.h
 
 \brief This file contains the list of codes used in the ISO19115 standard.
 */

#ifndef __TERRALIB_MD_INTERNAL_CODELIST_H
#define __TERRALIB_MD_INTERNAL_CODELIST_H


namespace te
{
  namespace md
  {
    /** @name Citation Code List 
     *  Citation Codes
    */
    //@{      
    
    /*!
     \enum CI_DateTypeCode
     
     \brief Identification of when a given event occurred.
    */
    enum CI_DateTypeCode
    {
      CI_creation,    /*!< Date identifies when the resource was brought into CI_existence. */
      CI_publication, /*!< Date identifies when the resource was issued. */
      CI_revision     /*!< Identifies when the resource was examined or re-examined and improved or amended. */
    };
    
    /*!
     \enum CI_RoleCode.
     
     \brief Function performed by the responsible party.
    */
    enum CI_RoleCode
    {
      CI_resourceProvider, /*!< Party that supplies the resource	 */
      CI_custodian,        /*!< Party that accepts accountability and responsability for the data and ensures appropriate care and maintenance of the resource	 */
      CI_owner,         /*!< Party that owns the resource	 */
      CI_sponsor,          /*!< Party that sponsors the resource	 */
      CI_user,             /*!< Party who uses the resource	 */
      CI_distributor,      /*!< Party who distributes the resource	 */
      CI_originator,       /*!< Party who created the resource	 */
      CI_pointOfContact,   /*!< Party who can be contacted for acquiring knowledge about or acquisition of the resource	 */
      CI_principalInvestigator,	/*!< Key party responsible for gathering information and conducting research	 */
      CI_processor,        /*!< Party who has processed the data in a manner such that the resource has been modified	 */
      CI_publisher,        /*!< Party who published the resource	 */
      CI_author,           /*!< Party who authored the resource	 */
      CI_collaborator      /*!< Party who conducted or contributed to the research */
    };
    
    /*!
     \enum CI_OnLineFunctionCode
     
     \brief Function performed by the resource.
    */
    enum CI_OnLineFunctionCode
    {
      CI_download,       /*!< Online instructions for transferring data from one storage device or system to another. */
      CI_information,    /*!< Online information about the resource. */
      CI_offlineAccess,  /*!< Online instructions for requestiong the resource from the provider. */
      CI_order,          /*!< Online order process for obtaining the resource. */
      CI_search          /*!< Online search interface for seeking out information about the resource. */
    };
    
    /*!
     \enum CI_PresentationFormCode
     
     \brief Mode in which the data is represented.
    */
    enum CI_PresentationFormCode
    {
      CI_documentDigital,	 /*!< Digital representation of a primarily textual item (can contain illustrations also) */
      CI_documentHardcopy, /*!< Representation of a primarily textual item (can contain illustrations also) on paper, photograhic material, or other media */	 
      CI_imageDigital,     /*!< Likeness of natural or man-made features, objects, and activities acquired through the sensing of visual or any other segment 
                            of the electromagnetic spectrum by sensors, such as thermal infrared, and high resolution radar and stored in digital format. */
      CI_imageHardcopy,	   /*!< Likeness of natural or man-made features, objects, and activities acquired through the sensing of visual or any other segment 
                            of the electromagnetic spectrum by sensors, such as thermal infrared, and high resolution radar and reproduced on paper, 
                            photographic material, or other media for use directly by the human user. */
      CI_mapDigital,       /*!< Map represented in raster or vector form	 */
      CI_mapHardcopy,      /*!< Map printed on paper, photographic material, or other media for use directly by the human user*/
      CI_modelDigital,     /*!< Multi-dimensional digital representation of a feature, process, etc.	 */
      CI_podelHardcopy,	   /*!< 3-dimensional, physical model */
      CI_profileDigital,	 /*!< Vertical cross-section in digital form */
      CI_profileHardcopy,  /*!< Vertical cross-section printed on paper, etc.	 */
      CI_tableDigital,     /*!< Digital representation of facts or figures systematically displayed, especially in columns	 */
      CI_tableHardcopy,    /*!< Representation of facts or figures systematically displayed, especially in columns, printed onpapers, photographic material, or other media */
      CI_videoDigital,     /*!< Digital video recording	 */
      CI_videoHardcopy     /*!< Video recording on film	 */     
    };   
    
    //@} 
    
    /** @name Data Quality Code List 
     *  Data Quality Codes
    */
    //@{      
    
    /*!
     \enum DQ_EvaluationMethodTypeCode
     
     \brief Type or method for evaluating an identified data quality measure.
    */
    enum DQ_EvaluationMethodTypeCode
    {
      DQ_directInternal,    /*!< method of evaluating the quality of a dataset based on inspection of items within the dataset, where all data required is internal to the 
                             dataset being evaluated */
      DQ_directExternal,    /*!< method of evaluating the quality of a dataset based on inspection of items within the dataset, where reference data external to the dataset 
                             being evaluated is required */
      DQ_indirect           /*!< method of evaluating the quality of a dataset based on external knowledge */
    };    
    
    //@}
    
    /** @name DS Code List 
     *  
    */
    //@{      
    
    /*!
     \enum DS_AssociationTypeCode
     
     \brief Justification for the correlation of two datasets
    */
    enum DS_AssociationTypeCode
    {
      DS_crossReference,        /*!< reference from one dataset to another */
      DS_largeWorkCitation,     /*!< reference to a master dataset of which this one is a part */
      DS_partOfSeamlessDatabase,/*!< part of the same structured set of data held in a computer */
      DS_source,                 /*!< mapping and charting information from which the dataset content originates */
      DS_stereoMate             /*!< part of a set of imagery that when used together, provides three-dimensional images */
    };  
    
    /*!
     \enum DS_InitiativeTypeCode
     
     \brief Ttype of aggregation activity in which datasets are related
    */
    enum DS_InitiativeTypeCode
    {
      DS_campain,     /*!< series of organized planned actions */
      DS_collection,  /*!< accumulation of datasets assembled for a specific purpose */
      DS_exercise,    /*!< element and entity definitions */
      DS_experiment,  /*!< specific performance of a function of group of functions */
      DS_investigation, /*!< search or systematic inquiry */
      DS_mission,     /*!< specific operation of a data collection system */
      DS_sensor,    /*!< device or piece of equipment which detects or records */
      DS_operation, /*!< action that is part of a series of actions */
      DS_platform,  /*!< vehicle or other support base that holds a sensor */
      DS_process,   /*!< method of doing something involving a number of steps */
      DS_program,   /*!< specific planned activity */
      DS_project,   /*!< organized undertaking, research, or development */
      DS_sciencePaper, 	/*!< document based on an experiment or research */
      DS_study, /*!< examination or investigation */ 
      DS_task,  /*!< piece of work */ 
      DS_trial,  /*!< process of testing to discover or demonstrate something */
      DS_userGuide	/*!< operating manual for users */
    };
    
    //@} 
    
    /** @name Metadata Code List 
     *  
    */
    //@{ 
    
    /*!
     \enum MD_CellGeometryCode
     
     \brief Code indicating whether grid data is point or area
    */
    enum MD_CellGeometryCode
    {
      MD_cpoint,	/*!< each cell represents a point	*/
      MD_area   /*!< each cell represents an area	 */
    };
    
    /*!
     \enum MD_CharacterSetCode
     
     \brief name of the character coding standard used in the resource
    */
    enum MD_CharacterSetCode
    {
      MD_ucs2,    /*!< Universal fixed length 16 bits */
      MD_ucs4,    /*!< Universal fixed length 32 bits */
      MD_utf7,    /*!< Universal variable length 7 bits */
      MD_utf8,    /*!< Universal variable length 8 bits */
      MD_utf16,    /*!< Universal variable length 16 bits */
      MD_8859part1, /*!< Eastern Europe, latin-1 */
      MD_8859part2, /*!< Central Europe, latin-2 */
      MD_8859part3, /*!< Southern Europe, latin-3 */
      MD_8859part4, /*!< Northern Europe, latin-4 */
      MD_8859part5, /*!< Cirilic */
      MD_8859part6, /*!< Arabic */
      MD_8859part7, /*!< Greek */
      MD_8859part8, /*!< Hebraic */
      MD_8859part9, /*!< Turkish */
      MD_8859part11, /*!< Thai */
      MD_8859part14, /*!< Latin-8 */
      MD_8859part15, /*!< Latin-9 */
      MD_jis,      /*!< Japanese for eletronic transmission */
      MD_shiftJIS, /*!< Japanese MS-DOS */
      MD_eucJP,    /*!< Japaneses UNIX */
      MD_usAscii,  /*!< ASCII EUA (ISO 646 US) */
      MD_ebcdic,   /*!< IBM mainframes */
      MD_eucKR,    /*!< Korean */
      MD_big5,     /*!< Taiwan */
      MD_GB2312   /*!< Simplified Chinese code set */
    }; 
    
    
    /*!
     \enum  MD_ClassificationCode
     
     \brief CodeList Definition: name of the handling restrictions on the dataset
    */
    enum MD_ClassificationCode
    {
      MD_cunclassified,	/*!< available for general disclosure	*/ 
      MD_restricted,    /*!< not for general disclosure	*/
      MD_confidential,	/*!< available for someone who can be entrusted with information	*/
      MD_secret,      /*!< kept or meant to be kept private, unknown, or hidden from all but a select group of people	*/
      MD_topSecret		/*!< of the highest secrecy	*/
    };
    
    /*!
     \enum MD_CoverageContentTypeCode
     
     \brief specific type of information represented in the cell
    */
    enum MD_CoverageContentTypeCode
    {
      MD_image,    /*!< meaningful numerical representation of a physical parameter that is not the actual value of the physical parameter */
      MD_thematic, /*!< code value with no quantitative meaning, used to represent a physical quantity */
      MD_physicalMeasure,     /*!< value in physical units of the quantity being measured */
      MD_referenceInformation,/*!< value of parameters used to locate physical measurements	*/
      MD_auxilliaryData,      /*!< values of extraneous data that are not part of the physical measurement	*/
      MD_qualityInformation,  /*!< values based on data quality	*/
      MD_modelResult          /*!< values of data from a model */
    };
    
    /*!
     \enum MD_DatatypeCode
     
     \brief datatype of element or entity
    */
    enum MD_DatatypeCode
    {    
      MD_class,     /*!< descriptor of a set of objects that share the same attributes, operations, methods, relationships, and behavior	*/
      MD_codelist,	/*!< descriptor of a set of objects that share the same attributes, operations, methods, relationships, and behavior	*/
      MD_enumeration,	/*!< data type whose instances form a list of named literal values, not extendable	*/
      MD_codelistElement,	/*!< permissible value for a codelist or enumeration	*/
      MD_abstractClass,   /*!< class that cannot be directly instantiated	*/
      MD_aggregateClass,	/*!< class that is composed of classes it is connected to by an aggregate relationship	*/
      MD_specifiedClass,	/*!< subclass that may be substituted for its superclass	*/
      MD_datatypeClass,	/*!< class with few or no operations whose primary purpose is to hold the abstract state of another class for transmittal, storage, encoding or persistent storage	*/
      MD_interfaceClass,/*!< named set of operations that characterize the behavior of an element	*/
      MD_unionClass,/*!< class describing a selection of one of the specified types	*/
      MD_metaClass,	/*!< class whose instances are classes	*/
      MD_typeClass,	/*!< class used for specification of a domain of instances (objects), together with the operations applicable to the objects. A type may have attributes and associations	*/
      MD_characterString,	/*!< free text field	*/
      MD_integer,	/*!< numerical field	*/
      MD_association	/*!< semantic relationship between two classes that involves connections among their instances */
    };
    
    /*!
     \enum MD_DimensionNameTypeCode
     
     \brief name of the dimensiony
    */
    enum MD_DimensionNameTypeCode
    {
      MD_row,   /*!< ordinate (y) axis		*/
      MD_column,/*!< abscissa (x) axis	*/
      MD_vertical,/*!< vertical (z) axis	*/
      MD_track,   /*!< along the direction of motion of the scan point	*/
      MD_crossTrack,/*!< perpendicular to the direction of motion of the scan point	*/
      MD_line,  /*!< scan line of a sensor	*/
      MD_sample,/*!< element along a scan line	*/
      MD_time  /*!< duration */
    };
    
    /*!
     \enum MD_GeometricObjectTypeCode
     
     \brief name of point or vector objects used to locate zero-, one-, two-, or three-dimensional spatial locations in the dataset
    */
    enum MD_GeometricObjectTypeCode
    {
      MD_complex,    /*!< set of geometric primitives such that their boundaries can be represented as a union of other primitives*/
      MD_composite,    /*!< connected set of curves, solids or surfaces */
      MD_curve,    /*!< bounded, 1-dimensional geometric primitive, representing the continuous image of a line */
      MD_point,    /*!< zero-dimensional geometric primitive, representing a position but not having an extent */
      MD_solid,    /*!< bounded, connected 3-dimensional geometric primitive, representing the continuous image of a region of space */
      MD_surface    /*!< bounded, connected 2-dimensional geometric primitive, representing the continuous image of a region of a plane */
    }; 
    
    /*!
     \enum MD_ImagingConditionCode
     
     \brief code which indicates conditions which may affect the image
    */
    enum MD_ImagingConditionCode
    {
      MD_blurredImage,    /*!< portion of the image is blurred */
      MD_cloud,           /*!< portion of the image is partially obscured by cloud cover */
      MD_degradingObliquity,/*!< acute angle between the plane of the ecliptic (the plane of the Earth s orbit) and the plane of the celestial equator */
      MD_fog, /*!< portion of the image is partially obscured by fog */
      MD_heavySmokeOrDust,    /*!< portion of the image is partially obscured by heavy smoke or dust */
      MD_night,   /*!< image was taken at night */
      MD_rain,    /*!< image was taken during rainfall*/
      MD_semiDarkness, /*!< image was taken during semi-dark conditions -- twilight conditions */
      MD_shadow,    /*!< portion of the image is obscured by shadow */
      MD_snow,      /*!< portion of the image is obscured by snow */
      MD_TerrainMasking    /*!< the absence of collection data of a given point or area caused by the relative location of 
                            topographic features which obstruct the collection path between the collector(s) and the 
                            subject(s) of interest */
    };
    
    /*!
     \enum MD_KeywordTypeCode
     
     \brief Methods used to group similar keywords
    */
    enum MD_KeywordTypeCode
    {
      MD_dataCenter,	  /*!< identifies a data center related to the resource	*/
      MD_dataResolution,  /*!< keyword identifies a temporal or spatial resolution	*/
      MD_discipline,  /*!< keyword identifies a branch of instruction or specialized learning	*/
      MD_instrument,  /*!< keyword identifies the instrument used to collect the resource	*/
      MD_place,  /*!< keyword identifies a location	*/
      MD_platform,  /*!< keyword identifies the platform used to collect the resource	*/
      MD_project,  /*!< keyword identifies a project related to the resource	*/
      MD_stratum,  /*!< keyword identifies the layer(s) of any deposited substance	*/
      MD_temporal,  /*!< keyword identifies a time period related to the dataset	*/
      MD_theme,  /*!< keyword identifies a particular subject or topic	*/
      MD_taxon  /*!< keyword identifies a taxonomy of the dataset */
    };
    
    /*!
     \enum MD_MaintenanceFrequencyCode
     
     \brief Frequency with which modifications and deletions are made to the data after it is first produced
    */
    enum MD_MaintenanceFrequencyCode
    {
      MD_continual, /*!< data is repeatedly and frequently updated	*/
      MD_daily, /*!< data is updated each day	*/
      MD_weekly, /*!< data is updated on a weekly basis	*/
      MD_fortnightly, /*!< data is updated every two weeks	*/
      MD_monthly, /*!< data is updated each month	*/
      MD_quarterly, /*!< data is updated every three months	*/
      MD_biannually, /*!< data is updated twice each year	*/
      MD_annually, /*!< data is updated every year	*/
      MD_asNeeded, /*!< data is updated as deemed necessary	*/
      MD_irregular, /*!< data is updated in intervals that are uneven in duration	*/
      MD_notPlanned, /*!< there are no plans to update the data	*/
      MD_unknown, /*!< frequency of maintenance for the data is not known */
      MD_twoYears,/*!< data is updated every two years	(required by MGB) */
      MD_fiveYears /*!< data is updated every five years (required by MGB)	*/
    };
    
    /*!
     \enum MD_MediumFormatCode
     
     \brief method used to write to the medium
    */
    enum MD_MediumFormatCode
    {
      MD_cpio,    /*!< CoPy In / Out (UNIX file format and command)	*/
      MD_tar,    /*!< 	Tape ARchive	*/
      MD_highSierra,    /*!< 	high sierra file system	*/
      MD_iso9660,    /*!< 	information processing volume and file structure of CD-ROM	*/
      MD_iso9660RockRidge,    /*!< 	rock ridge interchange protocol (UNIX)	*/
      MD_iso9660AppleHFS,    /*!< 	hierarchical file system (Macintosh) */
    };
    
    
    /*!
     \enum MD_MediumNameCode
     
     \brief name of the medium
    */
    enum MD_MediumNameCode
    {
      MD_cdRom, /*!< read-only optical disk	*/
      MD_dvd, /*!< digital versatile disk	*/
      MD_dvdRom, /*!< digital versatile disk, read only	*/
      MD_3halfInchFloppy, /*!< 3,5 inch magnetic disk	*/
      MD_5quarterInchFloppy, /*!< 	5,25 inch magnetic disk	*/
      MD_7trackTape, /*!< 7 track magnetic tape	*/
      MD_9trackType, /*!< 9 track magnetic tape	*/
      MD_3480Cartridge, /*!< 3480 cartridge tape drive	*/
      MD_3490Cartridge, /*!< 3490 cartridge tape drive	*/
      MD_3580Cartridge, /*!< 3580 cartridge tape drive	*/
      MD_4mmCartridgeTape, /*!< 4 millimetre magnetic tape	*/
      MD_8mmCartridgeTape, /*!< 	8 millimetre magnetic tape	*/
      MD_1quarterInchCartridgeTape, /*!< 0,25 inch magnetic tape	*/
      MD_digitalLinearTape, /*!< half inch cartridge streaming tape drive	*/
      MD_onLine,	/*!<direct  linkage	*/
      MD_satellite, /*!< linkage through a satellite communication system	*/
      MD_telephoneLink, /*!< communication through a telephone network	*/
      MD_hardcopy, /*!< pamphlet or leaflet giving descriptive information */  
    };
    
    /*!
     \enum MD_ObligationCode
     
     \brief 
    */
    enum MD_ObligationCode
    {
      MD_mandatory,	/*!< relement is always required	*/
      MD_optional,	/*!< relement is not required	*/
      MD_conditional	/*!< relement is required when a specific condition is met */
    };
    
    /*!
     \enum MD_PixelOrientationCode
     
     \brief point in a pixel corresponding to the Earth location of the pixel
    */
    enum MD_PixelOrientationCode
    {
      MD_center,    /*!< point halfway between the lower left and the upper right of the pixel */
      MD_lowerLeftCorner, /*!< the corner in the pixel closest to the origin of the SRS; if two are at the same distance from the origin, the one with the smallest x-value */
      MD_lowerRightCorner,  /*!< next corner counterclockwise from the lower left */
      MD_upperRightCorner,  /*!< next corner counterclockwise from the lower right */
      MD_upperLeftCorner    /*!< next corner counterclockwise from the upper right */
    };
    
    /*!
     \enum MD_ProgressCode
     
     \brief status of the dataset or progress of a review
    */
    enum MD_ProgressCode
    {
      MD_completed, /*!< production of the data has been completed */
      MD_hystoricalArchieve, /*!< data has been stored in an offline storage facility */
      MD_obsolete,  /*!< data is no longer relevant */
      MD_onGoing, /*!< data is continually being updated */
      MD_planned, /*!< fixed date has been established upon or by which the data will be created or updated */
      MD_required,  /*!< data needs to be generated or updated */
      MD_nderDevelopment  /*!< data needs to be generated or updated */
    };
    
    /*!
     \enum MD_RestrictionCode
     
     \brief 
    */
    enum MD_RestrictionCode
    {
      MD_copyright,  /*!< exclusive right to the publication, production, or sale of the rights to a literary, dramatic, 
                     musical, or artistic work, or to the use of a commercial print or label, granted by law for a 
                     specified period of time to an author, composer, artist, distributor */
      MD_patent,  /*!< government has granted exclusive right to make, sell, use or license an invention or discovery */
      MD_patentPending,   /*!< produced or sold information awaiting a patent */
      MD_trademark, /*!< a name, symbol, or other device identifying a product, officially registered and legally restricted 
                     to the use of the owner or manufacturer */
      MD_licence, /*!< formal permission to do something */
      MD_iIntelectualPropertyRights,  /*!< rights to financial benefit from and control of distribution of non-tangible property 
                                       that is a result of creativity */
      MD_restrict,  /*!< withheld from general circulation or disclosure */
      MD_other  /*!< limitation not listed */
    };
    
    /*!
     \enum MD_ScopeCode
     
     \brief class of information to which the referencing entity applies
    */
    enum MD_ScopeCode
    {
      MD_attribute, /*!< information applies to the attribute class	*/
      MD_attributeType, /*!< information applies to the characteristic of a feature	*/
      MD_collectionHardware, /*!< information applies to the collection hardware class	*/
      MD_collectionSession, /*!< information applies to the collection session	*/
      MD_dataset, /*!< information applies to the dataset	*/
      MD_series, /*!< information applies to the series	*/
      MD_nonGeographicDataset, /*!< information applies to non-geographic data	*/
      MD_dimensionGroup, /*!< information applies to a dimension group	*/
      MD_feature, /*!< information applies to a feature	*/
      MD_featureType, /*!< information applies to a feature type	*/
      MD_propertyType, /*!< information applies to a property type	*/
      MD_fieldSession, /*!< information applies to a field session	*/
      MD_software, /*!< information applies to a computer program or routine	*/
      MD_service, /*!< information applies to a capability which a service provider entity makes available to a service user 
                   entity through a set of interfaces that define a behaviour, such as a use case	*/
      MD_model, /*!< information applies to a copy or imitation of an existing or hypothetical object	*/
      MD_tile /*!< information applies to a tile, a spatial subset of geographic data */  
    };
    
    
    /*!
     \enum MD_SpatialRepresentationTypeCode
     
     \brief 
    */
    enum MD_SpatialRepresentationTypeCode
    {
      MD_vector,  /*!< vector data is used to represent geographic data */
      MD_grid,  /*!< grid data is used to represent geographic data */
      MD_tableText,  /*!< textual or tabular data is used to represent geographic data */
      MD_tin,   /*!< triangulated irregular network */
      MD_stereoModel,   /*!< three-dimensional view formed by the intersecting homologous rays of an overlapping pair of images	 */
      MD_video  /*!< scene from a video recording */
    };
    
    /*!
     \enum MD_TopicCategoryCode
     
     \brief high-level geographic data thematic classification to assist in the grouping and search of available geographic data sets. 
     
     Can be used to group keywords as well. Listed examples are not exhaustive.
    */
    enum MD_TopicCategoryCode
    {
      MD_farming, /*!< rearing of animals and/or cultivation of plants. Examples: agriculture, irrigation, aquaculture, plantations, 
                   herding, pests and diseases affecting crops and livestock	*/
      MD_biota, /*!< flora and/or fauna in natural environment. Examples: wildlife, vegetation, biological sciences, ecology, wilderness, sealife, wetlands, habitat	*/
      MD_boundaries, /*!< legal land descriptions. Examples: political and administrative boundaries	*/
      MD_climatologyMeteorologyAtmosphere, /*!< processes and phenomena of the atmosphere. Examples: cloud cover, weather, climate, atmospheric conditions, climate change, precipitation	*/
      MD_economy, /*!< economic activities, conditions and employment. Examples: production, labour, revenue, commerce, industry, tourism and ecotourism, forestry, fisheries, commercial 
                   or subsistence hunting, exploration and exploitation of resources such as minerals, oil and gas	*/
      MD_elevation, /*!< height above or below sea level. Examples: altitude, bathymetry, digital elevation models, slope, derived products	*/
      MD_environment, /*!< environmental resources, protection and conservation. Examples: environmental pollution, waste storage and treatment, environmental impact assessment, 
                       monitoring environmental risk, nature reserves, landscape */ 
      MD_geoscientificInformation, /*!< information pertaining to earth sciences. Examples: geophysical features and processes, geology, minerals, sciences dealing with the composition, 
                                    structure and origin of the earth s rocks, risks of earthquakes, volcanic activity, landslides, gravity information, soils, permafrost, hydrogeology, erosion */
      MD_health, /*!< health, health services, human ecology, and safety. Examples: disease and illness, factors affecting health, hygiene, substance abuse, mental and physical health, 
                  health services	*/
      MD_imageryBaseMapsEarthCover, /*!< base maps. Examples: land cover, topographic maps, imagery, unclassified images, annotations	*/
      MD_intelligenceMilitary, /*!< military bases, structures, activities. Examples: barracks, training grounds, military transportation, information collection	*/
      MD_inlandWaters, /*!< inland water features, drainage systems and their characteristics. Examples: rivers and glaciers, salt lakes, water utilization plans, dams, currents, 
                        floods, water quality, hydrographic charts	*/
      MD_location, /*!< positional information and services. Examples: addresses, geodetic networks, control points, postal zones and services, place names	*/
      MD_oceans, /*!< features and characteristics of salt water bodies (excluding inland waters). Examples: tides, tidal waves, coastal information, reefs	*/
      MD_planningCadastre, /*!< information used for appropriate actions for future use of the land. Examples: land use maps, zoning maps, cadastral surveys, land ownership	*/
      MD_society, /*!< characteristics of society and cultures. Examples: settlements, anthropology, archaeology, education, traditional beliefs, manners and customs, demographic data,
                   recreational areas and activities, social impact assessments, crime and justice, census information	*/
      MD_structure, /*!< man-made construction. Examples: buildings, museums, churches, factories, housing, monuments, shops, towers	*/
      MD_transportation, /*!< means and aids for conveying persons and/or goods. Examples: roads, airports/airstrips, shipping routes, tunnels, nautical charts, vehicle or vessel location,
                          aeronautical charts, railways	*/
      MD_utilitiesCommunication, /*!< energy, water and waste systems and communications infrastructure and services. Examples: hydroelectricity, geothermal, solar and nuclear sources of 
                                  energy, water purification and distribution, sewage collection and disposal, electricity and gas distribution, data communication, telecommunication, radio,
                                  communication networks */
    };
    
    /*!
     \enum MD_TopologyLevelCode
     
     \brief 
    */
    enum MD_TopologyLevelCode
    {
      MD_geometryOnly	, /*!< geometry objects without any additional structure which describes topology	*/
      MD_topology1D, /*!< 1-dimensional topological complex -- commonly called chain-node topology	*/
      MD_planarGraph, /*!< 1-dimensional topological complex that is planar. (A planar graph is a graph that can be drawn in a plane in such a way that no two edges intersect except at 
                       a vertex.)	*/
      MD_fullPlanarGraph, /*!< 2-dimensional topological complex that is planar. (A 2-dimensional topological complex is commonly called full topology in a cartographic 2D environment.)	*/
      MD_surfaceGraph, /*!< 1-dimensional topological complex that is isomorphic to a subset of a surface. (A geometric complex is isomorphic to a topological complex if their elements 
                        are in a one-to-one, dimensional-and boundry-preserving correspondence to one another.)	*/
      MD_fullSurfaceGraph, /*!< 2-dimensional topological complex that is isomorphic to a subset of a surface	*/
      MD_topology3D, /*!< 3-dimensional topological complex. (A topological complex is a collection of topological primitives that are closed under the boundary operations.)	*/
      MD_fullTopology3D, /*!< complete coverage of a 3D Euclidean coordinate space	*/
      MD_abstract /*!< topological complex without any specified geometric realisation */  
    };
    
    //@} 
    
    /** @name Metadata Image Code List 
     *  
    */
    //@{ 
    
    /*!
     \enum MI_BandDefinition
     
     \brief designation of criterion for defining maximum and minimum wavelengths for a spectral band
    */
    enum MI_BandDefinition
    {
      MI_3db, /*!< width of a distribution equal to the distance between the outer two points on the distribution having power level half of that at the peak	*/
      MI_halfMaximum, /*!< width of a distribution equal to the distance between the outer two points on the distribution having power level half of that at the peak	*/
      MI_fiftyPercent, /*!< full spectral width of a spectral power density measured at 50% of its peak height	*/
      MI_oneOverE, /*!< width of a distribution equal to the distance between the outer two points on the distribution having power level 1/e that of the peak	*/
      MI_equivalentWidth /*!< width of a band with full sensitivity or absorption at every wavelength that detects or absorbs the same amount of energy as the band described	*/
    };
    
    /*!
     \enum MI_ContextCode
     
     \brief designation of criterion for defining the context of the scanning process event
    */
    enum MI_ContextCode
    {
      MI_acqisition, /*!< event related to a specific domain	*/
      MI_pass, /*!< event related to a sequence of collections	*/
      MI_wayPoint /*!< event related to navigational manoeuvre	*/
    };
    
    /*!
     \enum MI_GeometryTypeCode
     
     \brief geometric description of the collection
    */
    enum MI_GeometryTypeCode
    {
      MI_point, /*!< single geographic point of interest	*/
      MI_linear, /*!< extended collection in a single vector	*/
      MI_areal, /*!< collection of a geographic area defined by a polygon (coverage)	*/
      MI_strip /*!< series of linear collections grouped by way points	*/
    };
    
    /*!
     \enum MI_ObjectiveTypeCode
     
     \brief temporal persistence of collection objective
    */
    enum MI_ObjectiveTypeCode
    {
      MI_instantaneousCollection, /*!<	single instance of collection	*/
      MI_persistentView,	 /*!< multiples instances of collection	*/
      MI_survey /*!<	collection over specified domain	*/
    };
    
    /*!
     \enum MI_OperationTypeCode
     
     \brief code indicating whether the data contained in this packet is real, simulated, or synthesized
    */
    enum MI_OperationTypeCode 
    {
      MI_real, /*!< originates from live-fly or other non-simulated operational source	*/
      MI_simulated, /*!< originates from target simulator sources	*/
      MI_synthesized /*!< mix of real and simulated data	*/
    };
    
    /*!
     \enum MI_PolarizationOrientationCode
     
     \brief polarization of the antenna relative to the waveform
    */
    enum MI_PolarizationOrientationCode
    {
      MI_horizontal, /*!< polarization of the sensor oriented in the horizontal plane in realtion to the swath direction	*/
      MI_vertical, /*!< polarization of the sensor oriented in the vertical plane in realtion to the swath direction	*/
      MI_leftCircular, /*!< polarization of the sensor oriented in the left circular plane in realtion to the swath direction	*/
      MI_rightCircular, /*!< polarization of the sensor oriented in the right circular plane in realtion to the swath direction	*/
      MI_theta, /*!< polarization of the sensor oriented in the angle between +90 deg and 0 deg parallel to swath direction	*/
      MI_phi /*!< polarization of the sensor oriented in the angle between +90 deg and 0 deg perpendicular to swath direction	*/
    };
    /*!
     \enum MI_PriorityCode
     
     \brief ordered list of priorities
    */
    enum MI_PriorityCode
    {
      MI_critical, /*!<	decisive importance	*/
      MI_highImportance, /*!<	requires resources to be made available	*/
      MI_mediumImportance, /*!<	normal operation priority	*/
      MI_lowImportance /*!<	to be completed when resources are available	*/
    };
    /*!
     \enum MI_SequenceCode
     
     \brief temporal relation of activation
    */
    enum MI_SequenceCode
    {
      MI_start, /*!<	beginning of a collection	*/
      MI_end, /*!<	end of a collection	*/
      MI_instantaneous /*!<	collection without a significant duration	*/
    };
    /*!
     \enum MI_TransferFunctionTypeCode
     
     \brief transform function to be used when scaling a physical value for a given element
    */
    enum MI_TransferFunctionTypeCode
    {
      MI_tlinear, /*!< beginning of a collection	*/
      MI_logarithmic, /*!<	end of a collection	*/
      MI_exponential /*!<	collection without a significant duration	*/
    };
    /*!
     \enum MI_TriggerCode
     
     \brief mechanism of activation
    */
    enum MI_TriggerCode
    {
      MI_automatic, /*!<	event due to external stimuli	*/
      MI_manual, /*!<	event manually instigated	*/
      MI_preProgrammed /*!<	event instigated by planned internal stimuli	*/
    };
    //@} 
    
    /** @name MX Code List 
     *  
    */
    //@{ 
    
    
    /*!
     \enum MX_ScopeCode
     
     \brief Extension of MD_ScopeCode for the needs of GMX application schemas and in the context of a transfer
    */
    enum MX_ScopeCode
    {
      MX_attribute, /*!<	information applies to the attribute class	*/
      MX_attributeType, /*!<	information applies to the characteristic of a feature	*/
      MX_collectionHardware, /*!<	information applies to the collection hardware class	*/
      MX_collectionSession, /*!<	information applies to the collection session	*/
      MX_dataset, /*!<	information applies to the dataset	*/
      MX_series, /*!<	information applies to the series	*/
      MX_nonGeographicDataset, /*!<	information applies to non-geographic data	*/
      MX_dimensionGroup, /*!<	information applies to a dimension group	*/
      MX_feature, /*!<	information applies to a feature	*/
      MX_featureType, /*!<	information applies to a feature type	*/
      MX_propertyType, /*!<	information applies to a property type	*/
      MX_fieldSession, /*!<	information applies to a field session	*/
      MX_software, /*!< information applies to a computer program or routine	*/
      MX_service, /*!<	information applies to a capability which a service provider entity makes available to a service user entity through a set of interfaces that 
                   define a behaviour, such as a use case	*/
      MX_model, /*!<	information applies to a copy or imitation of an existing or hypothetical object	*/
      MX_tile, /*!<	information applies to a tile, a spatial subset of geographic data	*/
      MX_initiative, /*!<	The referencing entity applies to a transfer aggregate which was originally identified as an initiative (DS_Initiative)	*/
      MX_stereomate, /*!<	The referencing entity applies to a transfer aggregate which was originally identified as a stereo mate (DS_StereoMate)	*/
      MX_sensor, /*!<	The referencing entity applies to a transfer aggregate which was originally identified as a sensor (DS_Sensor)	*/
      MX_platformSeries, /*!<	The referencing entity applies to a transfer aggregate which was originally identified as a platform series (DS_PlatformSeries)	*/
      MX_sensorSeries, /*!<	The referencing entity applies to a transfer aggregate which was originally identified as a sensor series (DS_SensorSeries)	*/
      MX_productionSeries, /*!<	The referencing entity applies to a transfer aggregate which was originally identified as a production series (DS_ProductionSeries)	*/
      MX_transferAggregate, /*!<	The referencing entity applies to a transfer aggregate which has no existence outside of the transfer context	*/
      MX_otherAggregate /*!<	The referencing entity applies to a transfer aggregate which has an existence outside of the transfer context, but which does not pertains 
                         to a specific aggregate type.   */
    };
    //@} 
    
  } // end namespace md
}   // end namespace te

#endif  // __TERRALIB_MD_INTERNAL_CODELIST_H

