require ("terralib_mod_binding_lua")

binding = terralib_mod_binding_lua

local function initTerraLib()
	local terralib = binding.TeSingleton.getInstance()
	terralib:initialize()
	
	return terralib
end

local function loadTerraLibPlugins()
	local pmger = binding.te.plugin.PluginManager.getInstance()
	pmger:loadAll()
	
	return pmger
end

function getConnInfo(name, dsType)
	local connInfo = {}
	
	if dsType == "ADO" then
		connInfo["PROVIDER"] = "Microsoft.Jet.OLEDB.4.0"
		connInfo["DB_NAME"] = name
		connInfo["CREATE_OGC_METADATA_TABLES"] = "TRUE"
		
	elseif dsType == "POSTGIS" then
		connInfo["PG_HOST"] = "localhost" 
		connInfo["PG_PORT"] = "5432" 
		connInfo["PG_USER"] = "postgres"
		connInfo["PG_PASSWORD"] = "postgres"
		connInfo["PG_DB_NAME"] = name
		connInfo["PG_CONNECT_TIMEOUT"] = "4" 
		connInfo["PG_CLIENT_ENCODING"] = "CP1252"     -- "LATIN1" --"WIN1252" 
		connInfo["CREATE_OGC_METADATA_TABLES"] = "TRUE"	
	elseif dsType == "OGR" then
		connInfo["URI"] = name
	end
	
	return connInfo
end

local function openDataSource(connInfo, dsType)
	local ds = binding.te.da.DataSourceFactory.make(dsType)
	ds:setConnectionInfo(connInfo) 
	ds:open()
	
	return ds
end

local function finalizeTerraLib(terralib, pmger)
	pmger:shutdownAll()
	pmger:unloadAll()
	pmger:clear()

	terralib:finalize()

	binding = nil
end

local function printTable(sometable)
	print("\n\n------------------------------ Table")
	for key, value in pairs(sometable) do
		print(key, value)
	end
end

local terralib = initTerraLib()
local pmger = loadTerraLibPlugins()
local dsType = "OGR"
local connInfo = getConnInfo("D:/terralib/data/shp/munic_2001.shp", dsType)
local ds = openDataSource(connInfo, dsType)

local idDs = binding.GetRandomicId()

ds:setId(idDs)

binding.DataSourceInsert(ds)

if ds then

	local dSetType = ds:getDataSetType("munic_2001")

	local dSet = ds:getDataSet("munic_2001")

	local gp = binding.GetFirstGeomProperty(dSetType)

	local idLayer = binding.GetRandomicId()

	local mbr = binding.te.gm.Envelope(binding.GetExtent(dSetType:getName(), gp:getName(), idDs))
	
	local layer = binding.te.map.DataSetLayer(idLayer)
	layer:setDataSetName(dSetType:getName())
	layer:setDataSourceId(idDs)
	layer:setVisibility(binding.NOT_VISIBLE)
	layer:setRendererType("ABSTRACT_LAYER_RENDERER")
	layer:setSRID(gp:getSRID())
	
	if mbr then
		layer:setExtent(mbr)
		local cellInfo = getConnInfo("brazilstates_from_shp", "OGR")
		
		local idSource = binding.GetRandomicId()
		
		local dsCell = binding.te.da.DataSourceInfo()
		dsCell:setConnInfo(cellInfo)
		dsCell:setAccessDriver("OGR")
		dsCell:setId(idSource)
		
		local csType = binding.te.cellspace.CellularSpacesOperations.CELLSPACE_POLYGONS
		
		binding.createCellSpace(dsCell, "brazil_cells_mask", 1, 1, mbr, gp:getSRID(), csType, layer)
	end
end

finalizeTerraLib(terralib, pmger)