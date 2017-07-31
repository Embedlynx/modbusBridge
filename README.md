# Modbusbridge openwrt/lede feed
___

## Build process:

### First install needed dependencies:
```
apt-get install subversion g++ zlib1g-dev build-essential git python
apt-get install libncurses5-dev gawk gettext unzip file libssl-dev wget
```

### Download lede:
```
git clone https://github.com/lede-project/source.git lede
cd lede
git checkout v17.01.2
```

### Add modbusbrige feed in your feeds.conf local file(add also luci):
```
echo "src-git modbusbridge https://github.com/Embedlynx/modbusBridge.git" >> feeds.conf
echo "src-git luci https://git.lede-project.org/project/luci.git" >> feeds.conf
```

### Update your build environment and install the packages:
```
./scripts/feeds update -a
./scripts/feeds install -a
```

### Configure your build:
```
make defconfig
make menuconfig
```

Now select the right "Target System" and "Target Profile" for your device. For example, for VoCore2 select:
```
    Target System   --->
                        MediaTek Ralink MIPS

    Subtarget       --->
                        MT7628 based boards

    Target Profile  --->
                        VoCore VoCore2
```

Then build-in modbusbridge package, select:
```
    Utilities       --->
                        <*> modbusbridge
```

### And build:
```
    make -j4
```

### Upgrade your device:
The firmware images are now in ```bin/targets/____/____/*.bin```. In our example ```bin/targets/ramips/mt7628/lede-ramips-mt7628-vocore2-squashfs-sysupgrade.bin```.

Upload ```*-sysupgrade.bin``` image to your device. You can use ssh, for example:

```scp bin/targets/ramips/mt7628/lede-ramips-mt7628-vocore2-squashfs-sysupgrade.bin root@192.168.1.1:/tmp/```

And upgrade your device firmware:

```root@LEDE:/# sysupgrade -v /tmp/lede-ramips-mt7628-vocore2-squashfs-sysupgrade.bin```

___

## Modbusbridge usage:

### Enable modbusbridge service:

```/etc/init.d/modbusbridge enable```


### Configure modbusbridge:
There are two ways to configure the modbusbridge - in the web configuration or in the console.

#### Web configuration:
Open your device web configuration:

```sensible-browser 192.168.1.1```

And go to:
```
    Services
            |-> Modbusbridge
```
Edit the configuration and click "Save and Apply". This will automatically reload the modbusbridge service.

#### Console configuration:
Open ```/etc/config/modbusbridge``` file.

```vi /etc/config/modbusbridge```

And edit the configuration. Then, you should manual restart the modbusbridge service:

```/etc/init.d/modbusbridge reload```
