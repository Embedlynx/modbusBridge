include $(TOPDIR)/rules.mk

PKG_NAME:=modbusbridge
PKG_RELEASE:=1


KG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)


include $(INCLUDE_DIR)/package.mk


define Package/modbusbridge
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=Modbusbridge -- prints a snarky message
	DEPENDS:=+libmodbus
endef


# Specify what needs to be done to prepare for building the package.
define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef


define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		LIBS="-nodefaultlibs -lgcc -lc -lmodbus" \
		LDFLAGS="$(EXTRA_LDFLAGS)"\
		CFLAGS="$(TARGET_CFLAGS) $(EXTRA_CFLAGS)"\
		$(TARGET_CONFIGURE_OPTS) \
		CROSS="$(TARGET_CROSS)" \
		ARCH="$(ARCH)" \
		$(1);
endef


define Package/modbusbridge/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/modbusbridge $(1)/bin/
endef

$(eval $(call BuildPackage,modbusbridge))
