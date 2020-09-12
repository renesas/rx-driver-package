MODULE_NAME = 	r_uid_rx
VERSION = 		v1.13

TARGET_FOLDER = ../../FITModules

$(TARGET_FOLDER)/$(MODULE_NAME)_$(VERSION).zip: $(TARGET_FOLDER)/$(MODULE_NAME)_$(VERSION).xml
	cd ./$(MODULE_NAME)_vx.xx; \
	zip ../$(TARGET_FOLDER)/$(MODULE_NAME)_$(VERSION).zip -r ./$(MODULE_NAME); \
	cd ../;	\

$(TARGET_FOLDER)/$(MODULE_NAME)_$(VERSION).xml:
	cp ./$(MODULE_NAME)_vx.xx.xml $(TARGET_FOLDER)/$(MODULE_NAME)_$(VERSION).xml

clean:
	rm -rf $(TARGET_FOLDER)/$(MODULE_NAME)*.*
