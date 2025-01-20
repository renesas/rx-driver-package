MODULE_NAME = 	r_aeropoint_rx
VERSION = 		1.00
MDF = V1

CONFIG_FOLDER = r_config
TARGET_FOLDER = ../../FITModules
FILE_PATH = $(MODULE_NAME)_vx.xx/$(MODULE_NAME)/ref

$(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).zip: $(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).xml $(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).mdf
ifneq ("$(wildcard $(FILE_PATH))","")
	cd ./$(MODULE_NAME)_vx.xx; \
	mkdir $(CONFIG_FOLDER); \
	cp $(MODULE_NAME)/ref/cri_aero_config_ais_reference.h $(CONFIG_FOLDER)/cri_aero_config_ais.h; \
	zip ../$(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).zip -r ./$(MODULE_NAME) -r ./$(CONFIG_FOLDER); \
	rm -rf $(CONFIG_FOLDER); \
	cd ../;
else
	cd ./$(MODULE_NAME)_vx.xx; \
	zip ../$(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).zip -r ./$(MODULE_NAME) -r ./$(CONFIG_FOLDER); \
	cd ../;
endif


$(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).xml:
	cp ./$(MODULE_NAME)_vx.xx.xml tmp.xml
	sed s/x.xx/$(VERSION)/ tmp.xml > $(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).xml
	rm -f tmp.xml

$(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).mdf:
	cp ./$(MODULE_NAME)_vx.xx.mdf $(TARGET_FOLDER)/$(MODULE_NAME)_v$(VERSION).mdf

clean:
	rm -rf $(TARGET_FOLDER)/$(MODULE_NAME)*.*
