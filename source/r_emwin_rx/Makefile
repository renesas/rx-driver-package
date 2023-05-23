MODULE_NAME = 	r_emwin_rx
VERSION = 		6.32a_100
MDF = V1

CONFIG_FOLDER = r_config
TARGET_FOLDER = ../../FITModules

$(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).zip: $(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).xml $(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).mdf
	cd ./$(MODULE_NAME)_vx.xx_xxx; \
	zip ../$(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).zip -r ./$(MODULE_NAME) -r ./$(CONFIG_FOLDER); \
	cd ../;	\

$(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).xml:
	cp ./$(MODULE_NAME)_vx.xx_xxx.xml tmp.xml
	sed s/x.xx/$(VERSION)/ tmp.xml > $(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).xml
	rm -f tmp.xml

$(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).mdf:
	cp ./$(MODULE_NAME)_vx.xx_xxx.mdf $(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).mdf

clean:
	rm -rf $(TARGET_FOLDER)/$(MODULE_NAME)*.*
