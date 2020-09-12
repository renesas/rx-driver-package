MODULE_NAME_LIST = 	r_ble_qe_utility r_ble_rx23w r_bsp r_byteq r_can_rx r_cmt_rx r_cmtw_rx\
					r_ctsu_qe r_dac_rx r_datfrx_rx r_dmaca_rx r_drw2d_rx r_dsmif_rx r_dtc_rx\
					r_eeprom_spi r_elc_rx r_emwin_rx r_ether_rx r_flash_rx r_flash_spi\
					r_glcdc_rx r_gpio_rx r_irda_sci_rx r_irq_rx r_iwdt_rx\
					r_jpegd_rx r_jpege_rx r_lcdc_rx r_longq r_lpc_rx r_lpt_rx r_lvd_rx\
					r_mesh_rx23w r_memdrv_rx r_mmcif_rx r_mpc_rx r_pdc_rx r_ptp_light_rx r_ptp_rx\
					r_qspi_smstr_rx r_riic_rx r_rscan_rx r_rspi_rx r_rtc_rx r_s2_rx\
					r_s12ad_rx r_sci_iic_rx r_sci_rx r_scif_rx r_sdc_sdmem_rx r_sdhi_rx r_sdc_sdio_rx\
					r_sdsi_rx r_simple_glcdc_config_rx r_simple_graphic_rx r_src_api_rx\
					r_ssi_api_rx r_sys_time_rx r_t4_dns_client_rx r_t4_driver_rx r_t4_file_driver_rx\
					r_t4_ftp_server_rx r_t4_http_server_rx r_t4_rx r_t4_sntp_client_rx\
					r_tfat_driver_rx r_tfat_rx r_touch_qe r_uid_rx\
					r_usb_basic r_usb_basic_mini r_usb_hcdc r_usb_hcdc_mini\
					r_usb_hhid r_usb_hhid_mini r_usb_hmsc r_usb_hmsc_mini\
					r_usb_pcdc r_usb_pcdc_mini r_usb_phid r_usb_phid_mini\
					r_usb_pmsc r_usb_pmsc_mini r_vbatt_rx r_wdt_rx r_wifi_esp32 r_wifi_esp8266 r_wifi_sx_ulpgn\
					r_st7735_lcd_driver\

all:
	for i in $(MODULE_NAME_LIST); do \
		cd ./source/$$i/; \
		make; \
		cd ../../; \
	done
		
clean:
	for i in $(MODULE_NAME_LIST); do \
		cd ./source/$$i/; \
		make clean; \
		cd ../../; \
	done
