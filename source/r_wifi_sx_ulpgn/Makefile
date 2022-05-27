MAKE = 			make
MODULE_NAME = 	r_wifi_sx_ulpgn
VERSION = 		1.16
# VERSION_MAJOR and VERSION_MINOR shall be updated when VERSION is changed.
VERSION_MAJOR =	1
VERSION_MINOR =	16
MDF = V2

CONFIG_FOLDER = r_config
TARGET_FOLDER = ../../FITModules

.PHONY: all
all: clean $(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).zip

$(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).zip: $(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).xml $(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION)_extend.mdf
	$(MAKE) update_version
	cd ./$(MODULE_NAME)_vx.xx && \
	zip ../$@ -r ./$(MODULE_NAME) -r ./$(CONFIG_FOLDER)
	$(MAKE) update_version_post

$(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).xml:
	cp ./$(MODULE_NAME)_vx.xx.xml tmp.xml
	sed 's/x.xx/$(VERSION)/' tmp.xml > $@
	rm -f tmp.xml

$(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION)_extend.mdf:
	cp ./$(MODULE_NAME)_vx.xx_extend.mdf $@

.PHONY: update_version
update_version:
	cp $(MODULE_NAME)_vx.xx/$(MODULE_NAME)/$(MODULE_NAME)_if.h tmp.h
	sed -e 's/WIFI_SX_ULPGN_CFG_VERSION_MAJOR_VALUE/$(VERSION_MAJOR)/' -e 's/WIFI_SX_ULPGN_CFG_VERSION_MINOR_VALUE/$(VERSION_MINOR)/' tmp.h > $(MODULE_NAME)_vx.xx/$(MODULE_NAME)/$(MODULE_NAME)_if.h

.PHONY: update_version_post
update_version_post:
	cp tmp.h $(MODULE_NAME)_vx.xx/$(MODULE_NAME)/$(MODULE_NAME)_if.h
	rm -f tmp.h

.PHONY: clean
clean:
	rm -rf $(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION)*.*
