module("luci.controller.modbusbridge.configure", package.seeall)

function index()
--    entry({"modbusbridge", "configure"}, cbi("modbusbridge-configure/configure"), "Modbus Bridge Configuration", 30).dependent=false
    entry({"admin", "services", "modbusbridge"}, cbi("modbusbridge-configure/configure"), "Modbus Bridge", 30).dependent=false
    entry({"admin", "services", "modbusbridge", "reload"}, call("reload_modbusbridge")).dependent=false
end


function reload_modbusbridge()
    local conn = ubus.connect()

    if not conn then
	error("Failed to connect to ubusd")
    end

    conn:call("modbusbridge", "reload", { })

    conn:close()
end
