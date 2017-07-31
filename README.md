# modbus-bridge

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
