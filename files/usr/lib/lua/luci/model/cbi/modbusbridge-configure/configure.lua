m = Map("modbusbridge", "Modbus Bridge Configuration", "Basic modbus bridge configuration")

rtu = m:section(NamedSection, "rtu", "modbus_context", "RTU configuration")

serial_port = rtu:option(ListValue, "serial_port", "Serial Port")
serial_port:value("/dev/ttyS0", "/dev/ttyS0")
serial_port:value("/dev/ttyS1", "/dev/ttyS1")
serial_port.default = "/dev/ttyS0"

baudrate = rtu:option(ListValue, "baudrate", "Baudrate") 
baudrate:value("1200", "1200")
baudrate:value("2400", "2400")
baudrate:value("4800", "4800")
baudrate:value("9600", "9600")
baudrate:value("19200", "19200")
baudrate:value("38400", "38400")
baudrate:value("57600", "57600")
baudrate:value("115200", "115200")
baudrate.default = "9600"

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

slave_id = rtu:option(Value, "slave_id", "Slave id(1-255)")
slave_id.default = 1
slave_id.maxlength = 3

function slave_id:validate(value)
    if tonumber(value) ~= nil and tonumber(value) >= 1 and tonumber(value) <= 255 then
	return value
    else
	return nil
    end
end

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

return m
