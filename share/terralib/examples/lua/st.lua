-------------------------------------------------------
-- import TerraLib module and do some initialization --
-------------------------------------------------------
require ( "terralib_mod_binding_lua" )

---------------------------
-- using Spatial Temporal module --
---------------------------

-- Functions
-- ---------

-- Returns the DataSetInfo description for a KML file
function GetDSInfoForKMLFile(fileLocation)
	dsConn = { URI=fileLocation }
	dsInfo = terralib_mod_binding_lua.te.da.DataSourceInfo()

	dsInfo:setId(terralib_mod_binding_lua.GetRandomicId())
	dsInfo:setConnInfo(dsConn)
	dsInfo:setType("OGR")

	return dsInfo
end

-- Open a desired file using OGR driver
function OpenDataSource(fileLocation)
	dsInfo = GetDSInfoForKMLFile(fileLocation)

	terralib_mod_binding_lua.OpenDataSource(dsInfo)

	return dsInfo
end

-- Getting the data sets
function GetTrajectoryDataSets(fileLocation)
	dsInfo = OpenDataSource(fileLocation)

    local phTimeIdx = 3
    local geomIdx = 12

    tjinfo40 = terralib_mod_binding_lua.te.st.TrajectoryDataSetInfo (dsInfo, "40: locations", phTimeIdx, geomIdx, -1, "40")
    tjDS40 = terralib_mod_binding_lua.te.st.STDataLoader.getDataSet(tjinfo40)
		
    tjinfo41 = terralib_mod_binding_lua.te.st.TrajectoryDataSetInfo(dsInfo, "41: locations", phTimeIdx, geomIdx, -1, "41")
    tjDS41 = terralib_mod_binding_lua.te.st.STDataLoader.getDataSet(tjinfo41);
		
	tjDS40:moveBeforeFirst();
	tjDS41:moveBeforeFirst();

	dI = terralib_mod_binding_lua.te.dt.Date(terralib_mod_binding_lua.boost.gregorian.greg_year(2008), terralib_mod_binding_lua.boost.gregorian.greg_month(01), terralib_mod_binding_lua.boost.gregorian.greg_day(01))
	dF = terralib_mod_binding_lua.te.dt.Date(terralib_mod_binding_lua.boost.gregorian.greg_year(2008), terralib_mod_binding_lua.boost.gregorian.greg_month(03), terralib_mod_binding_lua.boost.gregorian.greg_day(31))
	
    time1 = terralib_mod_binding_lua.te.dt.TimeInstant(dI, terralib_mod_binding_lua.te.dt.TimeDuration(0,0,0))
    time2 = terralib_mod_binding_lua.te.dt.TimeInstant(dF, terralib_mod_binding_lua.te.dt.TimeDuration(23,59,59))
    period = terralib_mod_binding_lua.te.dt.TimePeriod(time1, time2) 

    tjDS40period = terralib_mod_binding_lua.te.st.STDataLoader.getDataSet(tjinfo40, period, terralib_mod_binding_lua.te.dt.DURING)
	tjDS41period = terralib_mod_binding_lua.te.st.STDataLoader.getDataSet(tjinfo41, period, terralib_mod_binding_lua.te.dt.DURING)

	tjDS40period:moveBeforeFirst()
	tjDS41period:moveBeforeFirst()

	res = {}

	res["tjDS40"] = tjDS40
	res["tjDS41"] = tjDS41
	res["tjDS40period"] = tjDS40period
	res["tjDS41period"] = tjDS41period

	return res
end

-- Print Trajectory data sets
function PrintTrajectoryDataSetInfos(dss)
	local i = 0

	for k, v in pairs(dss) do
		print("Printing information about the trajectory data set " .. i .. " :\n")
		PrintTrajectoryDataSetInfo(v)

		i = i+1
	end
end

-- Print Trajectory data set
function PrintTrajectoryDataSetInfo(ds)
	textent = ds:getTemporalExtent()
	print("Temporal Extent: " .. textent:toString() .. "\n");

	sextent = ds:getSpatialExtent();
	print("Spatial Extent: " .. sextent:getLowerLeft().x .. "\n")
end

-- Printing time series infos
function PrintTimeSeriesInfo(ts)
	if(ts == nil) then
		print("Time series is NULL!\n")
		return
	end  

	print("Time series id: " .. ts:getId() .. "\n")
	print("Time series temporal extent: " .. ts:getTemporalExtent():toString() .. "\n")

	minValue = 0.
	maxValue = 0.

	ts:getValueExtent(minValue, maxValue)

	print("Time series value extent: (" .. minValue .. "," .. maxValue .. ")\n")

	gm = ts:getLocation();

	if(gm ~= nil) then
		print("Time series location: " .. gm:toString() .. "\n")
	end
end

-- Printing time series
function PrintTimeSeries(ts)
  if(ts == nil) then
    print("Time series is NULL!\n")
    return
  end

  PrintTimeSeriesInfo(ts)

  -- Print time series observations
  -- todo
end

--[[
  std::cout << "Time series observations: " << std::endl;
  
  te::st::TimeSeriesIterator it = ts->begin();  
  while(it != ts->end())
  {
    std::cout << "Date and time: " <<  it.getTime()->toString() << std::endl;
    std::cout << "Value: " <<  it.getValue()->toString()  << std::endl << std::endl;     
    ++it;
  }
}
--]]


-- Printing trajectory distance
function TrajectoryDistance(tj1, tj2)
	if(tj1 == nil) then
		print("The first trajectory is NULL!\n")
		return
	end

	if(tj2 == nill) then
	    print("The second trajectory is NULL!\n")
		return
	end

	-- Second: calculate the distance
	distance = tj1:getDistance(tj2)

	print("Printing the distance between two trajectories \n")
	print("============================================================\n")
    
	PrintTimeSeries(distance.get());
end


-- Init TerraLib
local t = terralib_mod_binding_lua.TeSingleton.getInstance()
t:initialize()

-- Load plugins
local pmger = terralib_mod_binding_lua.te.plugin.PluginManager.getInstance()
pmger:loadAll()

-- Init ST data loader
terralib_mod_binding_lua.te.st.STDataLoader.initialize();

-- Get data sets from KML file
local dss = GetTrajectoryDataSets("D:/funcate/vs2010/terralib5/data/kml/t_40_41.kml") 

-- Print the informations about each data set
PrintTrajectoryDataSetInfos(dss)

-- Does not working
-------------------
dss = nil

collectgarbage("collect")

-- Finalize ST data loader
terralib_mod_binding_lua.te.st.STDataLoader.finalize();

-- Finalize plugins and TerraLib
pmger:shutdownAll()
pmger:unloadAll()
pmger:clear()

-- Finalize TerraLib
t:finalize()    
