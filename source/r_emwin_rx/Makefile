MODULE_NAME = 	r_emwin_rx
VERSION = 		6.34.g.1.20
VERSION_STR = 	6.34g_120
MDF = V1

CONFIG_FOLDER = r_config
TARGET_FOLDER = ../../FITModules

$(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION_STR).zip: $(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION_STR).xml $(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION_STR).mdf
	cd ./$(MODULE_NAME)_vx.xx_xxx; \
	zip ../$(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION_STR).zip -r ./$(MODULE_NAME) -r ./$(CONFIG_FOLDER); \
	cd ../;	\

$(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION_STR).xml:
	cp ./$(MODULE_NAME)_vx.xx_xxx.xml tmp1.xml
	sed s/x.xx.x.x.xx/$(VERSION)/ tmp1.xml > tmp2.xml
	sed s/x.xx/$(VERSION_STR)/ tmp2.xml > $(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION_STR).xml
	rm -f tmp1.xml tmp2.xml

$(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION_STR).mdf:
	cp ./$(MODULE_NAME)_vx.xx_xxx.mdf $(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION_STR).mdf

clean:
	rm -rf $(TARGET_FOLDER)/$(MODULE_NAME)*.*
