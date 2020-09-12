MODULE_NAME = 	r_bsp
VERSION = 		v5.52

CONFIG_FOLDER = r_config
TARGET_FOLDER = ../../FITModules

$(TARGET_FOLDER)/$(MODULE_NAME)_$(VERSION).zip: $(TARGET_FOLDER)/$(MODULE_NAME)_$(VERSION).xml
	cd ./$(MODULE_NAME)_vx.xx;	\
	zip ../$(TARGET_FOLDER)/$(MODULE_NAME)_$(VERSION).zip -r ./$(MODULE_NAME) -r ./$(CONFIG_FOLDER);	\
	cd ../;	\

$(TARGET_FOLDER)/$(MODULE_NAME)_$(VERSION).xml:
	cp ./$(MODULE_NAME)_vx.xx.xml $(TARGET_FOLDER)/$(MODULE_NAME)_$(VERSION).xml

clean:
	rm -rf $(TARGET_FOLDER)/$(MODULE_NAME)*.*
