MODULE_NAME = 	r_lwip_rx
VERSION = 		1.00
MDF = V2

CONFIG_FOLDER = r_config
TARGET_FOLDER = ../../FITModules

.PHONY: all
all: clean $(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).zip

$(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).zip: $(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).xml $(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION)_extend.mdf
	cd ./$(MODULE_NAME)_vx.xx; \
	zip ../$(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).zip -r ./$(MODULE_NAME) -r ./$(CONFIG_FOLDER) -i '*.c' '*.h' '*.pdf'; \
	cd ../;

$(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).xml:
	cp ./$(MODULE_NAME)_vx.xx.xml tmp.xml
	sed s/x.xx/$(VERSION)/ tmp.xml > $@
	rm -f tmp.xml

$(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION)_extend.mdf:
	cp ./$(MODULE_NAME)_vx.xx_extend.mdf tmp.mdf
	sed s/x.xx/$(VERSION)/ tmp.mdf > $@
	rm -f tmp.mdf

.PHONY: clean
clean:
	rm -rf $(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION)*.*
