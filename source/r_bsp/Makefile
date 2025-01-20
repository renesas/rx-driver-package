MODULE_NAME = 	r_bsp
VERSION = 		7.52
MDF = NOT_SUPPORTED

CONFIG_FOLDER = r_config
TARGET_FOLDER = ../../FITModules

$(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).zip: $(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).xml
	cd ./$(MODULE_NAME)_vx.xx;	\
	zip ../$(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).zip -r ./$(MODULE_NAME) -r ./$(CONFIG_FOLDER);	\
	cd ../;	\
	convmv --notest -r -f shiftjis -t utf8 $(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).xml;	\
	convmv --notest -r -f shiftjis -t utf8 $(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).zip;	\

$(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).xml:
	cp ./$(MODULE_NAME)_vx.xx.xml tmp.xml
	sed s/x.xx/$(VERSION)/ tmp.xml > $(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).xml
	rm -f tmp.xml

clean:
	rm -rf $(TARGET_FOLDER)/$(MODULE_NAME)*.*
