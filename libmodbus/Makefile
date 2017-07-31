#
# Copyright (C) 2006-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=libmodbus
PKG_VERSION:=3.1.4
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/Embedlynx/libmodbus
PKG_SOURCE_VERSION:=1006e9e957e0796c7d5a2086dc595412496615bf


PKG_MAINTAINER:=Georgi Dimov <georgi.dimov@embedlynx.com>

PKG_LICENSE:=GPL-3.0+ LGPL-2.1+
PKG_LICENSE_FILES:=COPYING COPYING.LESSER

PKG_FIXUP:=autoreconf
PKG_INSTALL:=1


include $(INCLUDE_DIR)/package.mk


define Package/libmodbus
  SECTION:=libs
  CATEGORY:=Libraries
  URL:=http://www.libmodbus.org
  TITLE:=libmodbus-tcp-rtu-gateway
endef


define Package/libmodbus/description
  Modbus library with tcp rtu gateway added for Linux, Mac OS X, FreeBSD, QNX and Win32.
endef


CONFIGURE_ARGS += --without-documentation --disable-tests
TARGET_CFLAGS += $(FPIC)


define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/modbus $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libmodbus.{so*,la} $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/libmodbus.pc $(1)/usr/lib/pkgconfig/
endef


define Package/libmodbus/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libmodbus.so* $(1)/usr/lib/
endef


$(eval $(call BuildPackage,libmodbus))
