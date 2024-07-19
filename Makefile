RELEASE_TAG_COMMIT_NUMBER_LATEST = 5c921fc40bd76f7c34968b1a7e2fa2e113ca0a2e
RELEASE_TAG_COMMIT_VERSION_LATEST = 1.43
VERSIONS_FILE_NAME = versions.xml

MODULE_NAME_LIST_LATEST = 	r_aeropoint_rx \
					r_ble_qe_utility r_ble_rx23w r_ble_da14531_rx r_bsp r_byteq \
					r_can_rx r_canfd_rx r_cellular r_cmt_rx r_cmtw_rx r_comms_i2c_rx r_ctsu_qe \
					r_dac_rx r_datfrx_rx r_dmaca_rx r_drw2d_rx r_dsmif_rx r_dtc_rx \
					r_eeprom_spi r_elc_rx r_emwin_rx r_ether_rx \
					r_flash_rx r_flash_spi r_fs1015_rx r_fs2012_rx r_fs3000_rx r_fwup \
					r_glcdc_rx r_gpio_rx \
					r_hs300x_rx r_hs400x_rx \
					r_irda_sci_rx r_irq_rx r_iwdt_rx \
					r_jpegd_rx r_jpege_rx \
					r_lcdc_rx r_longq r_lpc_rx r_lpt_rx r_lvd_rx \
					r_memdrv_rx r_mesh_rx23w r_mmcif_rx r_mpc_rx \
					r_ob1203_rx r_pdc_rx r_ptp_light_rx r_ptp_rx \
					r_qspi_smstr_rx r_qspix_rx \
					r_ri3c_rx r_riic_rx r_riichs_rx r_rscan_rx r_rsci_rx r_rspi_rx r_rspia_rx r_rtc_rx r_ryz012_rx \
					r_s12ad_rx r_s2_rx r_sci_iic_rx r_sci_rx r_scif_rx r_sdc_sdmem_rx r_sdc_spi_rx r_sdhi_rx r_sdsi_rx \
					r_src_api_rx r_ssi_api_rx r_sys_time_rx \
					r_t4_driver_rx r_t4_rx r_tfat_driver_rx r_tfat_rx r_tsip_rx_lib \
					r_uid_rx r_usb_basic r_usb_basic_mini \
					r_usb_hcdc r_usb_hcdc_mini r_usb_hhid r_usb_hhid_mini r_usb_hmsc r_usb_hmsc_mini \
					r_usb_pcdc r_usb_pcdc_mini r_usb_phid r_usb_phid_mini r_usb_pmsc r_usb_pmsc_mini \
					r_vbatt_rx \
					r_wdt_rx r_wifi_sx_ulpgn r_wifi_da16xxx \
					r_zmod4xxx_rx \
					rm_comms_uart_rx rm_rai_data_collector_rx rm_rai_data_shipper_rx \
					rm_touch_qe \

all:
	for i in $(MODULE_NAME_LIST_LATEST); do \
		cd ./source/$$i/; \
		make; \
		cd ../../; \
	done

latest:
	for i in $(MODULE_NAME_LIST_LATEST); do \
		cd ./source/$$i/; \
		make; \
		cd ../../; \
	done

clean:
	rm -rf ./FITModules
	mkdir ./FITModules

fix_xml:
	sed -i '/  <!-- Latest Package -->/q' ./$(VERSIONS_FILE_NAME)
	for i in $(MODULE_NAME_LIST_LATEST); do \
		cd ./source/$$i/; \
		sh ../../tools/version_xml_generator.sh ./Makefile release_tag_commit_number=$(RELEASE_TAG_COMMIT_NUMBER_LATEST) release_tag_commit_version=$(RELEASE_TAG_COMMIT_VERSION_LATEST) >> ../../$(VERSIONS_FILE_NAME);\
		cd ../..; \
	done
	echo "</modules>" >> ./$(VERSIONS_FILE_NAME)
	sed -i 's/\r//g' ./$(VERSIONS_FILE_NAME)

update_xml:
	sed -i '/  <!-- Latest Package -->/d' ./$(VERSIONS_FILE_NAME)
	sed -i '/<\/modules>/d' ./$(VERSIONS_FILE_NAME)
	echo "  <!-- Latest Package -->" >> ./$(VERSIONS_FILE_NAME)
	for i in $(MODULE_NAME_LIST_LATEST); do \
		cd ./source/$$i/; \
		sh ../../tools/version_xml_generator.sh ./Makefile release_tag_commit_number=$(RELEASE_TAG_COMMIT_NUMBER_LATEST) release_tag_commit_version=$(RELEASE_TAG_COMMIT_VERSION_LATEST) >> ../../$(VERSIONS_FILE_NAME);\
		cd ../..; \
	done
	echo "</modules>" >> ./$(VERSIONS_FILE_NAME)
	sed -i 's/\r//g' ./$(VERSIONS_FILE_NAME)

%:
	rm -rf ./repo
	mkdir ./repo
	git clone https://github.com/renesas/rx-driver-package.git -b $@ ./repo
	mv ./repo/FITModules/* ./FITModules
	rm -rf ./repo
