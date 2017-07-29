module("luci.controller.modbusbridge.configure", package.seeall)

function index()
    entry({"admin", "services", "modbusbridge"}, cbi("modbusbridge-configure/configure"), "Modbusbridge", 30).dependent=false
end
