m = Map("modbusbridge", "Modbusbridge Configuration", "Basic modbusbridge configuration")

rtu = m:section(NamedSection, "rtu", "modbus_context", "RTU configuration")

device = rtu:option(ListValue, "device", "Device")
device:value("/dev/ttyS0", "/dev/ttyS0")
device:value("/dev/ttyS1", "/dev/ttyS1")
device.default = "/dev/ttyS0"

baud_rate = rtu:option(ListValue, "baud_rate", "Baud rate") 
baud_rate:value("9600", "9600")
baud_rate:value("19200", "19200")
baud_rate:value("57600", "57600")
baud_rate:value("115200", "115200")
baud_rate.default = "115200"

parity = rtu:option(ListValue, "parity", "Parity")
parity:value("N", "None")
parity:value("E", "Even")
parity:value("O", "Odd")
parity.default = "None"


data_bits = rtu:option(ListValue, "data_bits", "Data bits")
data_bits:value("5", "5")
data_bits:value("6", "6")
data_bits:value("7", "7")
data_bits:value("8", "8")
data_bits.default = "8"

stop_bits = rtu:option(ListValue, "stop_bits", "Stop bits")
stop_bits:value("1", "1")
stop_bits:value("2", "2")
stop_bits.default = "1"

-- End of the RTU section


tcp = m:section(NamedSection, "tcp", "modbus_context", "TCP configuration")
ip = tcp:option(Value, "ipaddr", "IP address")
ip.maxlength = 15
function ip:validate(value)
    return value:match("[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+")
end

port = tcp:option(Value, "port", "Port")
port.maxlength = 5
port.default = "502"

function port:validate(value)
    if tonumber(value) ~= nil and tonumber(value) >= 1 and tonumber(value) <= 65535 then
	return value
    else
	return nil
    end
end

-- End of the TCP section

-- Reload modbusbridge service
m.on_after_commit = function(self)                         
    require("luci.sys").call("/etc/init.d/modbusbridge reload &>/dev/null")
end

return m
