### Modbusbridge usage:
```
Usage: modbusbridge DEVICE BAUD_RATE PARITY DATA_BITS STOP_BITS IP_ADDRESS PORT

Example usage:
modbusbridge /dev/ttyS0 115200 'N' 8 1 192.168.1.1 1502
```

### Package structure
```
/
├── etc/
│   ├── config/
│   │   └── modbusbridge
│   └── init.d/
│       └── modbusbridge
└── usr/
    ├── bin/
    │   └── modbusbridge
    └── lib/
        └── lua/
            └── luci/
                ├── controller/
                │   └── modbusbridge/
                |       └── configure.lua
                └── model/
                    └── cbi/
                        └── modbusbridge-configure/
                            └── configure.lua
```
