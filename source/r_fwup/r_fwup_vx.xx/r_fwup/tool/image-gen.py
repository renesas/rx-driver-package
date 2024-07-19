#**********************************************************************************************************************
#* DISCLAIMER
#* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
#* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
#* applicable laws, including copyright laws.
#* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
#* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
#* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
#* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
#* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
#* THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
#* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
#* this software. By using this software, you agree to the additional terms and conditions found by accessing the
#* following link:
#* http://www.renesas.com/disclaimer
#*
#* Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
#**********************************************************************************************************************
#**********************************************************************************************************************
#* File Name    : image-gen.py
#* Device(s)    : RL/RX
#* Tool-Chain   : python 3
#* OS           : None
#* H/W Platform : PC
#* Description  : Generate RX/RL78 Update image
#**********************************************************************************************************************
#**********************************************************************************************************************
# History : DD.MM.YYYY Version Description
#           31.03.2023 3.00 Initial release.
#           18.04.2023 3.01 Changed signature data generation using ECDSA library to signature data generation using 
#                           PyCryptodome library.
#                           Fixed bugs in inputPrmDataSet and mot_data_address_area_check functions.
#           25.04.2023 3.02 Added a function to generate an RSU file with all user program data added in the format 
#                           of RSU header information (address information of 100h bytes), which is a function 
#                           for FW Update V2.00.
#                           Fixed a bug in processing when not including the data flash area in the RSU file.
#           09.11.2023 3.03 Added "BAREMETAL_FWUP_V2_V1_DATA" to RSU format type.
#**********************************************************************************************************************
import sys
import os
import fileinput
from pathlib import Path
import binascii
import hashlib
import csv
import argparse
from Crypto.Hash import SHA256
from Crypto.PublicKey import ECC
from Crypto.Signature import DSS
class Person:
    pass

# Value when the option is omitted
OPTION_OMIT = 'None'

def set_cmdline_argparser():
    """Command line argument parsing settings.
    
    Set valid commands and options on the command line.
    
    Args:
        None
    
    Returns:
        ArgumentParser object
    
    """
    
    # Local constant value
    SIGNATURE_TYPE = ['sha256', 'ecdsa']
    RSU_FORMAT  = ['BareMetal', 'RTOS', 'BareMetal_FWUP_V1', 'RTOS_FWUP_V1', 'RTOS_FWUP_V2_V1_DATA', 'BAREMETAL_FWUP_V2_V1_DATA']
    
    
    METAVER_FILE    = '<File>'
    METAVER_VERIFICATION_TYPE = '<VerificationType>'
    METAVER_RSU_FORMAT_TYPE = '<FileFormat>'
    
    MSG_HELP_TOOL                   = 'Renesas ImageGenerator ' + IMAGE_GENERATOR_VERSION
    MSG_HELP_I_USER_PROGRAM         = 'Specifies the mot file name of the user program.'
    MSG_HELP_I_BOOT_LOADER_PROGRAM  = 'Specifies the mot file name of the bootloader program.'
    MSG_HELP_I_PARAMER_FILE_NAME    = 'Specifies the input parameter file name.'
    MSG_HELP_O                      = 'Specifies the RSU file name to be output. (A mot file is generated when you specify a bootloader program)'
    MSG_HELP_ENCKEY                 = 'Specifies the key file name for signing the image. (must be specified when generating ECDSA signature data)'
    MSG_HELP_SIGNATURE_VERIFICATION = 'Specifies the signature verification method. If not specified, sha256 is chosen (choose from sha256 or ecdsa)'
    MSG_HELP_RSU_FORMAT_TYPE        = 'Specifies the RSU format type. BareMetal is selected if not specified (choose from BareMetal or RTOS)'
    # Set command line parser
    parser = argparse.ArgumentParser(description=MSG_HELP_TOOL)
    
    # Required command
    parser.add_argument('-iup', metavar=METAVER_FILE, required=True, help=MSG_HELP_I_USER_PROGRAM)
    parser.add_argument('-ip',  metavar=METAVER_FILE, required=True, help=MSG_HELP_I_PARAMER_FILE_NAME)
    parser.add_argument('-o',  metavar=METAVER_FILE, required=True, help=MSG_HELP_O)
    
    # Optional command
    parser.add_argument('-ibp', metavar=METAVER_FILE, required=False, default=OPTION_OMIT, help=MSG_HELP_I_BOOT_LOADER_PROGRAM)
    parser.add_argument('-key', metavar=METAVER_FILE, required=False, default='secp256r1.privatekey', help=MSG_HELP_ENCKEY)
    parser.add_argument('-vt',  choices=SIGNATURE_TYPE, metavar=METAVER_VERIFICATION_TYPE, default='sha256', help=MSG_HELP_SIGNATURE_VERIFICATION)
    parser.add_argument('-ff',  choices=RSU_FORMAT, metavar=METAVER_RSU_FORMAT_TYPE, default='BareMetal', help=MSG_HELP_RSU_FORMAT_TYPE)
    return parser

def CalculateMotorolaChecksum(str_checksum):
    str_checksum_index = 0
    str_checksum_index_len = len(str_checksum)
    check_sum = 0
    while str_checksum_index < str_checksum_index_len:
        index_data = (str_checksum[str_checksum_index:str_checksum_index+2])
        int_index_data = int(index_data, 16)
        check_sum = check_sum + int_index_data
        str_checksum_index = str_checksum_index + 2
    check_sum = ~check_sum & 0xFF;
    check_sum_str = format(check_sum, 'X')
    return check_sum_str.zfill(2)

# Generate signature.
def ecdsa_signature(key_pem,message):
    key = ECC.import_key(key_pem)
    h = SHA256.new(message)                 # bytes
    signer = DSS.new(key, 'fips-186-3')
    signature = signer.sign(h)
    return signature                        # bytes

# Debug Mode
DEBUG_MODE = False

# Image Generator Version
IMAGE_GENERATOR_VERSION = "Ver3.03"

#** Macro ***
RSU_BAREMETAL = "BareMetal"                                  # Generate RSU file for FW Update 2.00 (bare metal) environment.
RSU_RTOS = "RTOS"                                            # Generate RSU file for FW Update 2.00 (FreeRTOS OTA) environment.
RSU_BAREMETAL_FWUP_V1 = "BareMetal_FWUP_V1"                  # Generate RSU file for FW Update 1.xx (bare metal) environment.
RSU_RTOS_FWUP_V1 = "RTOS_FWUP_V1"                           # Generate RSU file for FW Update 1.xx (FreeRTOS OTA) environment.
RSU_RTOS_FWUP_V2_V1_DATA = "RTOS_FWUP_V2_V1_DATA"
RSU_BAREMETAL_FWUP_V2_V1_DATA = "BAREMETAL_FWUP_V2_V1_DATA"

ADDRESS_AREA_TYPE_NON = 0
ADDRESS_AREA_TYPE_BANK0_BOOT_LOADER = 1
ADDRESS_AREA_TYPE_USER_PROGRAM = 2
ADDRESS_AREA_TYPE_DATA_FLASH = 3
ADDRESS_AREA_TYPE_OFSM = 4
ADDRESS_AREA_TYPE_NEAR_BOOT_LOADER = 5

HASH_SHA256 = "sha256"                                        # hash-sha256
SIG_SHA256_ECDSA = "ecdsa"                                    # sig-sha256-ecdsa

DEVICE_TYPE_DUAL_MODE = 1
DEVICE_TYPE_LINEAR_MODE = 0

BL_INITIAL_FW_MOT_FILE_SUPPORT = 4
UPDATE_FW_RSU_FILE_SUPPORT = 3

# Parameter Name string
DEVICE_TYPE_STR = "device Type"
CODE_FLASH_SIZE_STR = "Code Flash Size(Dual Mode Only)"
BOOT_LOADER_START_ADDRESS_STR = "Bootloader Start Address"
BOOT_LOADER_END_ADDRESS_STR = "Bootloader End Address"
USER_PROGRAM_START_ADDRESS_STR = "User Program Start Address"
USER_PROGRAM_END_ADDRESS_STR = "User Program End Address"
OFS_DATA_START_ADDRESS_STR = "OFS Data Start Address"
OFS_DATA_END_ADDRESS_STR = "OFS Data End Address"
DATA_FLASH_START_ADDRESS_STR = "Data Flash Start Address"
DATA_FLASH_END_ADDRESS_STR = "Data Flash End Address"
NEAR_DATA_START_ADDRESS_STR = "Near Data Start Address(RL78 Only)"
NEAR_DATA_END_ADDRESS_STR = "Near Data End Address(RL78 Only)"
FLASH_WRITE_SIZE_STR = "Flash Write Size"

# Parameter data String
# Device type String
DUAL_MODE_STR = "Dual Mode"
LINEAR_MODE_STR = "Linear Mode"
# Neaer Data Address String or Code Flash Size(RL78) or OFS Data
NO_USED_STR = "No Used."

# Reads all data from a file specified as a command line argument.
output_mot_file_name_str = ""
output_rsu_file_name_str = ""
input_useProgramFileName_Str = ""
input_bootLoaderFileName_Str = ""
input_prm_fileName_Str = ""

print("\n")

# Command line argument parse
parser = set_cmdline_argparser()
args = parser.parse_args()
input_useProgramFileName_Str = args.iup                                             # Set UserProgrameFileName
input_prm_fileName_Str = args.ip                                                    # Set InputParameterFileName.
input_key_file = args.key                                                           # Set key for signature data generation.
verification_type = args.vt                                                         # Signature verification method
rsu_format = args.ff                                                                # Set RSU format.

# Check the existence of the input mot file of the user program.
if os.path.isfile(input_useProgramFileName_Str) == False:
    print("The" ,input_useProgramFileName_Str, "file does not exist.")
    exit()
# Check the existence of the input parameter file.
if os.path.isfile(input_prm_fileName_Str) == False:
    print("The" ,input_prm_fileName_Str, "file does not exist.")
    exit()
# Check the existence of the input mot file for the bootloader program.
if args.ibp != "None":
    inputBootLoaderProgram = args.ibp
    if os.path.isfile(inputBootLoaderProgram) == False:
        print("The" ,inputBootLoaderProgram, "file does not exist.")
        exit()
# Check the existence of the key file for signature data generation.
if verification_type == "ecdsa":
    if os.path.isfile(input_key_file) == False:
        print("The" ,input_key_file, "file does not exist.")
        exit()
Input_UserProgram_MotFile = open(input_useProgramFileName_Str, 'r')
if args.ibp == 'None':
    input_bootLoaderFileName_Str = ""
    rsu_str = ".rsu"
    output_rsu_file_name_str = args.o + rsu_str                                     # Set output RSU file name.
    output_mot_file_name_str = ""
else:
    input_bootLoaderFileName_Str = args.ibp
    Input_BL_MotFile = open(input_bootLoaderFileName_Str, 'r')
    mot_str = ".mot"
    output_mot_file_name_str = args.o + mot_str                                     # Set output mot file name.
    output_rsu_file_name_str = ""

def input_file_close():
    if input_useProgramFileName_Str != "":
        Input_UserProgram_MotFile.close()
    if input_bootLoaderFileName_Str != "":
        Input_BL_MotFile.close()

# Output File
block_flag_file = open('block_flag_file.txt', 'w')                # A reference file that shows the relationship between addresses and block numbers.

# default Input Parameter
device_type = None
code_flash_size = None
bank0_boot_loader_area_start_address = None
bank0_boot_loader_area_end_address = None
user_program_area_start_address = None
user_program_area_end_address = None
ofs_data_start_address = None
ofs_data_end_address = None
data_flash_area_start_address = None
data_flash_area_end_address = None
boot_loader_near_start_address = None
boot_loader_near_end_address = None
flash_write_size = None

# Checks numeric validity.
def check_numeric_validity(in_data):
    data_chk_result = None
    
    if in_data < 0:
        data_chk_result =  False
        print("The numeric value of the input parameter is negative.")
    elif in_data > 0xFFFFFFFF:
        data_chk_result = False
        print("Input parameter data exceeds 8 digits.")
    else:
        data_chk_result = True
    return data_chk_result

# Input Paramerter Set
def inputPrmDataSet(prmFile_data):
    param_chk_result = True
    device_type = None
    code_flash_size = None
    bank0_boot_loader_area_start_address = None
    bank0_boot_loader_area_end_address = None
    user_program_area_start_address = None
    user_program_area_end_address = None
    ofs_data_start_address = None
    ofs_data_end_address = None
    data_flash_area_start_address = None
    data_flash_area_end_address = None
    boot_loader_near_start_address = None
    boot_loader_near_end_address = None
    flash_write_size = None
    
    for prmFile_line_data in prmFile_data:
        lineData_lsit = list(prmFile_line_data)
        if len(lineData_lsit) == 2:
            prm_name_str = lineData_lsit[0]
            data_str = lineData_lsit[1]
            # Device type
            if prm_name_str == DEVICE_TYPE_STR:
                if data_str == DUAL_MODE_STR:
                    device_type = DEVICE_TYPE_DUAL_MODE
                elif data_str == LINEAR_MODE_STR:
                    device_type = DEVICE_TYPE_LINEAR_MODE
                else:
                    print("Device type Input Parameter Error!!")
                    param_chk_result = False
            # Code Flash Size
            elif prm_name_str == CODE_FLASH_SIZE_STR:
                if data_str == NO_USED_STR:
                    code_flash_size = 0
                else:
                    code_flash_size = int(data_str, 16)
                    param_chk_result = check_numeric_validity(code_flash_size)
                    if param_chk_result == False:
                        break
            # Boot loader Start address
            elif prm_name_str == BOOT_LOADER_START_ADDRESS_STR:
                bank0_boot_loader_area_start_address = int(data_str, 16)
                param_chk_result = check_numeric_validity(bank0_boot_loader_area_start_address)
                if param_chk_result == False:
                    break
            # Boot loader End address
            elif prm_name_str == BOOT_LOADER_END_ADDRESS_STR:
                bank0_boot_loader_area_end_address = int(data_str, 16)
                param_chk_result = check_numeric_validity(bank0_boot_loader_area_end_address)
                if param_chk_result == False:
                    break
            # User Program Start address
            elif prm_name_str == USER_PROGRAM_START_ADDRESS_STR:
                user_program_area_start_address = int(data_str, 16)
                param_chk_result = check_numeric_validity(user_program_area_start_address)
                if param_chk_result == False:
                    break
            # User Program End address
            elif prm_name_str == USER_PROGRAM_END_ADDRESS_STR:
                user_program_area_end_address = int(data_str, 16)
                param_chk_result = check_numeric_validity(user_program_area_end_address)
                if param_chk_result == False:
                    break
            # OFSM Data Start address
            elif prm_name_str == OFS_DATA_START_ADDRESS_STR:
                if data_str == NO_USED_STR:
                    ofs_data_start_address = -1
                else:
                    ofs_data_start_address = int(data_str, 16)
                    param_chk_result = check_numeric_validity(ofs_data_start_address)
                    if param_chk_result == False:
                        break
            # OFSM Data End address
            elif prm_name_str == OFS_DATA_END_ADDRESS_STR:
                if data_str == NO_USED_STR:
                    ofs_data_end_address = -1
                else:
                    ofs_data_end_address = int(data_str, 16)
                    param_chk_result = check_numeric_validity(ofs_data_end_address)
                    if param_chk_result == False:
                        break
            # Data Flash Start address
            elif prm_name_str == DATA_FLASH_START_ADDRESS_STR:
                if data_str == NO_USED_STR:
                    data_flash_area_start_address = -1
                else:
                    data_flash_area_start_address = int(data_str, 16)
                    param_chk_result = check_numeric_validity(data_flash_area_start_address)
                    if param_chk_result == False:
                        break
            # Data Flash End address
            elif prm_name_str == DATA_FLASH_END_ADDRESS_STR:
                if data_str == NO_USED_STR:
                    data_flash_area_end_address = -1
                else:
                    data_flash_area_end_address = int(data_str, 16)
                    param_chk_result = check_numeric_validity(data_flash_area_end_address)
                    if param_chk_result == False:
                        break
            # Near Data Start address
            elif prm_name_str == NEAR_DATA_START_ADDRESS_STR:
                if data_str == NO_USED_STR:
                    boot_loader_near_start_address = -1
                else:
                    boot_loader_near_start_address = int(data_str, 16)
                    param_chk_result = check_numeric_validity(boot_loader_near_start_address)
                    if param_chk_result == False:
                        break
            # Near Data End address
            elif prm_name_str == NEAR_DATA_END_ADDRESS_STR:
                if data_str == NO_USED_STR:
                    boot_loader_near_end_address = -1
                else:
                    boot_loader_near_end_address = int(data_str, 16)
                    param_chk_result = check_numeric_validity(boot_loader_near_end_address)
                    if param_chk_result == False:
                        break
            # Flash Write Size
            elif prm_name_str == FLASH_WRITE_SIZE_STR:
                flash_write_size = int(data_str)
                param_chk_result = check_numeric_validity(flash_write_size)
                if param_chk_result == False:
                    break
                    
    return param_chk_result, device_type, code_flash_size, bank0_boot_loader_area_start_address, bank0_boot_loader_area_end_address, user_program_area_start_address, user_program_area_end_address, ofs_data_start_address, ofs_data_end_address, data_flash_area_start_address, data_flash_area_end_address, boot_loader_near_start_address, boot_loader_near_end_address, flash_write_size
with open(input_prm_fileName_Str) as f:
    prmFile_data = csv.reader(f)
    param_chk_result, device_type, code_flash_size, bank0_boot_loader_area_start_address, bank0_boot_loader_area_end_address, user_program_area_start_address, user_program_area_end_address, ofs_data_start_address, ofs_data_end_address, data_flash_area_start_address, data_flash_area_end_address, boot_loader_near_start_address, boot_loader_near_end_address, flash_write_size = inputPrmDataSet(prmFile_data)
f.close()
# Exit if there is an error in the input parameters.
if param_chk_result == False:
    input_file_close()
    exit()
# RSU header address Information
rsu_addr_info = Person()
rsu_addr_info.program_num = 0x0000
rsu_addr_info.program_start_address = []
rsu_addr_info.program_data_size = []
rsu_addr_info.reserved = 0xFFFFFFFF

bank1_boot_loader_area_start_address = 0
bank1_boot_loader_area_end_address = 0

user_program_start_address = 0
user_program_end_address = 0
bank1_boot_loader_mot_data_list = []                                                                                             # Bank1 Area Boot Loader mot Data
user_program_block_flg = []                                                                                                      # Boot Loader Buffer
boot_loader_buf = []
user_program_buf = []                                                                                                            # User Program Buffer
user_program_str = ""
data_flash_block_flg = []
data_flash_buf = []                                                                                                              # Data Flash Buffer
data_flash_str = ""
rsu_addr_info_buf_lst = [0xFFFFFFFF] * 64                                                                                        # RSU hedaer(200h - 2FFh) Initialize
sig_verification_bin = b''

def mot_data_adress_area_check(int_data):
    adr_type = ADDRESS_AREA_TYPE_NON
    adr_result = bool((int_data >= bank0_boot_loader_area_start_address) and (int_data <= bank0_boot_loader_area_end_address))
    if adr_result == True:
        adr_type = ADDRESS_AREA_TYPE_BANK0_BOOT_LOADER                                                                           # Boot loader adress Area
        return adr_type
    adr_result = bool((int_data >= user_program_area_start_address) and (int_data <= user_program_area_end_address))
    if adr_result == True:
        adr_type = ADDRESS_AREA_TYPE_USER_PROGRAM                                                                                # User Program address Area
        return adr_type
    if (data_flash_area_start_address != -1) and (data_flash_area_end_address != -1):
        adr_result = bool((int_data >= data_flash_area_start_address) and (int_data <= data_flash_area_end_address))
        if adr_result == True:
            adr_type = ADDRESS_AREA_TYPE_DATA_FLASH                                                                              # Data Flash address Area
            return adr_type
    adr_result = bool((int_data >= ofs_data_start_address) and (int_data <= ofs_data_end_address))
    if adr_result == True:
        adr_type = ADDRESS_AREA_TYPE_OFSM                                                                                        # OFSM address Area
        return adr_type
    adr_result = bool((boot_loader_near_start_address == -1) and (boot_loader_near_end_address == -1))
    if adr_result == False:
        adr_result = bool((int_data >= boot_loader_near_start_address) and (int_data <= boot_loader_near_end_address))
        if adr_result == True:
            adr_type = ADDRESS_AREA_TYPE_NEAR_BOOT_LOADER                                                                        # Boot Loader near address Area
    return adr_type

def get_s_record_type_str(int_address):
    if (int_address >= 0) and (int_address <= 0xFFFF):                                                                           # 2 Byte address
        s_record_type_str = "S1"
        s_record_len_str = '13'
        s_record_adr_format = '04X'
    elif (int_address >= 0x10000) and (int_address <= 0xFFFFFF):                                                                 # 3 Byte address
        s_record_type_str = "S2"
        s_record_len_str = '14'
        s_record_adr_format = '06X'
    elif (int_address >= 0x1000000) and (int_address <= 0xFFFFFFFF):                                                             # 4 Byte address
        s_record_type_str = "S3"
        s_record_len_str = '15'
        s_record_adr_format = '08X'
    return s_record_type_str, s_record_len_str, s_record_adr_format

# User Program Buffer Initialize
user_program_area_size = user_program_area_end_address - user_program_area_start_address + 1
result = divmod(user_program_area_size, flash_write_size)
if result[1] == 0: # remainder
    user_program_area_block_num = result[0]
    block_cnt = 0
    user_program_buf_cnt = 0
    while True:
        user_program_buf.append('FF')
        user_program_buf_cnt += 1
        if user_program_buf_cnt >= user_program_area_size:
            break
    while True:
        user_program_block_flg.append(0)
        block_cnt += 1
        if block_cnt >= user_program_area_block_num:
            user_program_block_flg.append(0)                  # EOF flag
            break
else:
    print("Input parameter error!!")
    print("User program area range is not in unit of flash write size.")
    exit()
# Data Flash Buffer Initialize
if (data_flash_area_start_address != -1) and (data_flash_area_end_address != -1):
    data_flash_area_size = data_flash_area_end_address - data_flash_area_start_address + 1
    result = divmod(data_flash_area_size, flash_write_size)
    if result[1] == 0:                                        # remainder
        data_flash_area_block_num = result[0]
        block_cnt = 0
        data_flash_buf_cnt = 0
        while True:
            data_flash_buf.append('FF')
            data_flash_buf_cnt += 1
            if data_flash_buf_cnt >= data_flash_area_size:
                break
        while True:
            data_flash_block_flg.append(0)
            block_cnt += 1
            if block_cnt >= data_flash_area_block_num:
                data_flash_block_flg.append(0)                # EOF flag
                break
    else:
        print("Input parameter error!!")
        print("Data Flash area range is not in unit of flash write size.")
        exit()
# boot_loader buffer initialize
boot_loader_area_size = bank0_boot_loader_area_end_address - bank0_boot_loader_area_start_address + 1
boot_loader_str = ""
result = divmod(boot_loader_area_size, flash_write_size)
if result[1] == 0:                                            # remainder
    if device_type == DEVICE_TYPE_DUAL_MODE:                  # dual mode
        code_flash_result = divmod(code_flash_size, 2)
        if code_flash_result[1] == 0:                         # remainder
            code_flash_half_size  = code_flash_result[0]
            bank1_boot_loader_area_start_address = bank0_boot_loader_area_start_address - code_flash_half_size
            bank1_boot_loader_area_end_address = bank0_boot_loader_area_end_address - code_flash_half_size
    boot_loader_buf_cnt = 0
    while True:
        boot_loader_buf.append('FF')
        boot_loader_buf_cnt += 1
        if boot_loader_buf_cnt >= boot_loader_area_size:
            break
else:
    print("Input parameter error!!")
    print("Boot Loader area range is not in unit of flash write size.")
    exit()
    
user_program_mot_data_list = []
data_flash_mot_data_list = []

# Boot Loader File data Read
if input_bootLoaderFileName_Str != "":
    s_record_adr_str = ""
    bank0_boot_loader_mot_list = []
    ofsm_data_mot_list = []
    near_boot_loader_mot_list = []
    InMotFileData = Input_BL_MotFile.read()
    InMotLineData = InMotFileData.splitlines()
    MotLineCount = 0
    MotLineMax = len(InMotLineData)
    for MotLineCount in range(MotLineMax):
        motLineData = InMotLineData[MotLineCount]
        record = motLineData[0:2]
        byteNum = motLineData[2:4]
        motLineDataLen = len(motLineData)
        if (record == 'S7') or (record == 'S8') or (record == 'S9'):
           s_record_adr_str += motLineData
           s_record_adr_str += '\n'
        elif (record == 'S1') or (record == 'S2') or (record == 'S3'):
           if record == 'S1':
               adr_offset = 8
           elif record == 'S2':
               adr_offset = 10
           else:
               adr_offset = 12
           byteNum = motLineData[2:4]
           count = int(byteNum, 16)
           adr = motLineData[4:adr_offset]
           adr_hex_str = "0x" + adr
           adr_hex_int = int(adr_hex_str, 16)
           adr_type = mot_data_adress_area_check(adr_hex_int)
           if adr_type == ADDRESS_AREA_TYPE_BANK0_BOOT_LOADER:
               motData = motLineData[adr_offset:motLineDataLen-2]
               motData_Len = len(motData) // 2
               motDataLength = len(motData)
               bank0_boot_loader_mot_list.append(motLineData)
               if device_type == DEVICE_TYPE_DUAL_MODE:                                                                           # dual mode
                   bank1_boot_loader_address = adr_hex_int - code_flash_half_size
                   s_record_str, s_record_len_str, s_record_adr_format = get_s_record_type_str(bank1_boot_loader_address)
                   bank1_mot_sum_work_data = byteNum + format(bank1_boot_loader_address, s_record_adr_format).upper() + motData
                   bank1_mot_sum_str = CalculateMotorolaChecksum(bank1_mot_sum_work_data)
                   bank1_mot_line_data = s_record_str + bank1_mot_sum_work_data + bank1_mot_sum_str
                   bank1_mot_line_data += '\n'
                   bank1_boot_loader_mot_data_list.append(bank1_mot_line_data)
           elif adr_type == ADDRESS_AREA_TYPE_OFSM:
               ofsm_data_mot_list.append(motLineData)
           elif adr_type == ADDRESS_AREA_TYPE_NEAR_BOOT_LOADER:
               near_boot_loader_mot_list.append(motLineData)
    block_flag_file_cnt = 0
    # Boot Loader bank0 mot data
    bank0_boot_loader_mot_str = ""
    for boot_loader_list_index in range(len(bank0_boot_loader_mot_list)):
        bank0_boot_loader_mot_str += bank0_boot_loader_mot_list[boot_loader_list_index]
        bank0_boot_loader_mot_str += '\n'
    # near Boot Loader mot data
    near_boot_loader_mot_str = ""
    for near_boot_loader_list_index in range(len(near_boot_loader_mot_list)):
        near_boot_loader_mot_str += near_boot_loader_mot_list[near_boot_loader_list_index]
        near_boot_loader_mot_str += '\n'
    # Boot Loader ofsm mot data
    boot_loader_ofsm_mot_str = ""
    for boot_loader_list_index in range(len(ofsm_data_mot_list)):
        boot_loader_ofsm_mot_str += ofsm_data_mot_list[boot_loader_list_index]
        boot_loader_ofsm_mot_str += '\n'
    if device_type == DEVICE_TYPE_DUAL_MODE:                                                                                     # dual mode
        # Boot Loader buf bin
        boot_loader_str = ""
        for boot_loader_buf_index in range(len(boot_loader_buf)):
            boot_loader_str += boot_loader_buf[boot_loader_buf_index]
        boot_loader_bin = binascii.unhexlify(boot_loader_str)
        # Boot Loader bank1 mot data
        bank1_boot_loader_mot_str = ""
        for boot_loader_list_index in range(len(bank1_boot_loader_mot_data_list)):
            bank1_boot_loader_mot_str += bank1_boot_loader_mot_data_list[boot_loader_list_index]

# User Program mot file data Read
InMotFileData = Input_UserProgram_MotFile.read()
InMotLineData = InMotFileData.splitlines()
MotLineCount = 0
code_flash_data_bin = ""
code_flash_data = []

MotLineMax = len(InMotLineData)

for MotLineCount in range(MotLineMax):
    motLineData = InMotLineData[MotLineCount]
    record = motLineData[0:2]
    byteNum = motLineData[2:4]
    motLineDataLen = len(motLineData)
    if (record == 'S1') or (record == 'S2') or (record == 'S3'):
       if record == 'S1':
           adr_offset = 8
       elif record == 'S2':
           adr_offset = 10
       else:
           adr_offset = 12
       byteNum = motLineData[2:4]
       count = int(byteNum, 16)
       adr = motLineData[4:adr_offset]
       adr_hex_str = "0x" + adr
       adr_hex_int = int(adr_hex_str, 16)
       adr_type = mot_data_adress_area_check(adr_hex_int)
       if adr_type == ADDRESS_AREA_TYPE_USER_PROGRAM:
           motData = motLineData[adr_offset:motLineDataLen-2]
           motData_Len = len(motData) // 2
           motDataLength = len(motData)
           user_program_area_offset = adr_hex_int - user_program_area_start_address
           mot_data_index = 0
           user_program_buf_index = 0
           user_program_start_block_index = user_program_area_offset // flash_write_size
           user_program_end_block_index = (user_program_area_offset + motData_Len -1) // flash_write_size
           user_program_block_flg[user_program_start_block_index] = 1
           if user_program_start_block_index != user_program_end_block_index:
               user_program_block_flg[user_program_end_block_index] = 1
           while True:
               user_program_buf[user_program_area_offset + user_program_buf_index] = motData[mot_data_index:mot_data_index+2]
               mot_data_index += 2
               user_program_buf_index += 1
               if mot_data_index >= motDataLength:
                   break
       elif adr_type == ADDRESS_AREA_TYPE_DATA_FLASH:
           motData = motLineData[adr_offset:motLineDataLen-2]
           motData_Len = len(motData) // 2
           motDataLength = len(motData)
           data_flash_area_offset = adr_hex_int - data_flash_area_start_address
           mot_data_index = 0
           data_flash_buf_index = 0
           data_flash_start_block_index = data_flash_area_offset // flash_write_size
           data_flash_end_block_index = (data_flash_area_offset + motData_Len -1) // flash_write_size
           data_flash_block_flg[data_flash_start_block_index] = 1
           if data_flash_start_block_index != data_flash_end_block_index:
               data_flash_block_flg[data_flash_end_block_index] = 1
           while True:
               data_flash_buf[data_flash_area_offset + data_flash_buf_index] = motData[mot_data_index:mot_data_index+2]
               mot_data_index += 2
               data_flash_buf_index += 1
               if mot_data_index >= motDataLength:
                   break
# Generation of a reference file showing the relationship between addresses and block numbers.
block_flag_file = open('block_flag_file.txt', 'w')                                                      # A reference file that shows the relationship between addresses and block numbers.
user_program_block_flg_num = len(user_program_block_flg)
data_flash_block_flg_num = len(data_flash_block_flg)
block_flag_file_cnt = 0
user_program_block_flg_num = len(user_program_block_flg)
# Generation of a reference file showing the relationship between addresses and block numbers.
while True:
    user_program_block_flg_start_adr = user_program_area_start_address + (flash_write_size * block_flag_file_cnt)
    user_program_block_flg_end_adr = user_program_area_start_address + (flash_write_size * block_flag_file_cnt + (flash_write_size - 1))
    block_flag_file.write(str(block_flag_file_cnt))
    block_flag_file.write(" = ")
    block_flag_file.write(str(user_program_block_flg[block_flag_file_cnt]))
    block_flag_file.write(", ")
    block_flag_file.write(str(format(user_program_block_flg_start_adr, 'X')))
    block_flag_file.write("-")
    block_flag_file.write(str(format(user_program_block_flg_end_adr, 'X')))
    block_flag_file.write("\n")
    block_flag_file_cnt += 1
    if block_flag_file_cnt >= user_program_block_flg_num:
        break
block_flag_file.close()
if DEBUG_MODE == False:
    os.remove('block_flag_file.txt')
if data_flash_block_flg_num != 0:
    data_flash_block_flag_file = open('data_flash_block_flag_file.txt', 'w')
    data_flash_block_flag_file_cnt = 0
    while True:
        data_flash_block_flg_start_adr = data_flash_area_start_address + (flash_write_size * data_flash_block_flag_file_cnt)
        data_flash_block_flg_end_adr = data_flash_area_start_address + (flash_write_size * data_flash_block_flag_file_cnt + (flash_write_size - 1))
        data_flash_block_flag_file.write(str(data_flash_block_flag_file_cnt))
        data_flash_block_flag_file.write(" = ")
        data_flash_block_flag_file.write(str(data_flash_block_flg[data_flash_block_flag_file_cnt]))
        data_flash_block_flag_file.write(", ")
        data_flash_block_flag_file.write(str(format(data_flash_block_flg_start_adr, 'X')))
        data_flash_block_flag_file.write("-")
        data_flash_block_flag_file.write(str(format(data_flash_block_flg_end_adr, 'X')))
        data_flash_block_flag_file.write("\n")
        data_flash_block_flag_file_cnt += 1
        if data_flash_block_flag_file_cnt >= data_flash_block_flg_num:
            break
    data_flash_block_flag_file.close()
    if DEBUG_MODE == False:
        os.remove('data_flash_block_flag_file.txt')
# Generate address information for RSU header.
if (rsu_format == RSU_BAREMETAL_FWUP_V1) or (rsu_format == RSU_RTOS_FWUP_V1) or (rsu_format == RSU_RTOS_FWUP_V2_V1_DATA) or(rsu_format == RSU_BAREMETAL_FWUP_V2_V1_DATA):             # Generate RSU file for FW Update 1.xx environment.
    user_program_all_buf_str = ""
    for cnt in range(len(user_program_buf)):
        user_program_all_buf_str += user_program_buf[cnt]
    user_program_all_buf_bin = binascii.unhexlify(user_program_all_buf_str)
    user_program_all_buf_rsu_header_cut_bin = user_program_all_buf_bin[0x300:]
# User Program data Create
block_flag_index_cnt = 0
block_flag_start_index = -1
block_flag_data_size = 0
while True:
    if rsu_addr_info.program_num > 31:
        print("The number of program data exceeds 31.")
        input_file_close()
        exit()
    if user_program_block_flg[block_flag_index_cnt] == 1 and block_flag_start_index == -1:
        block_flag_start_index = block_flag_index_cnt
    elif user_program_block_flg[block_flag_index_cnt] == 0 and block_flag_start_index >= 0:
        block_flag_data_size = (block_flag_index_cnt - block_flag_start_index) * flash_write_size
        user_program_block_flg_start_adr = user_program_area_start_address + (block_flag_start_index * flash_write_size)
        rsu_addr_info.program_start_address.append(user_program_block_flg_start_adr)
        rsu_addr_info.program_data_size.append(block_flag_data_size)
        rsu_addr_info.program_num += 1
        user_program_index = block_flag_index_cnt * flash_write_size
        user_program_start_index = block_flag_start_index * flash_write_size
        # Add User Program data
        for user_program_index in range(block_flag_data_size):
            user_program_str += user_program_buf[user_program_start_index + user_program_index]
        block_flag_start_index = -1
        block_flag_data_size = 0
    block_flag_index_cnt += 1
    if block_flag_index_cnt >= user_program_block_flg_num:
        break
# Data Flash Create
if len(data_flash_buf) != 0:
    block_flag_index_cnt = 0
    block_flag_start_index = -1
    block_flag_data_size = 0
    while True:
        if rsu_addr_info.program_num > 31:
            print("The number of program data exceeds 31.")
            input_file_close()
            exit()
        if data_flash_block_flg[block_flag_index_cnt] == 1 and block_flag_start_index == -1:
            block_flag_start_index = block_flag_index_cnt
        elif data_flash_block_flg[block_flag_index_cnt] == 0 and block_flag_start_index >= 0:
            block_flag_data_size = (block_flag_index_cnt - block_flag_start_index) * flash_write_size
            data_flash_block_flg_start_adr = data_flash_area_start_address + (block_flag_start_index * flash_write_size)
            rsu_addr_info.program_start_address.append(data_flash_block_flg_start_adr)
            rsu_addr_info.program_data_size.append(block_flag_data_size)
            rsu_addr_info.program_num += 1
            data_flash_index = block_flag_index_cnt * flash_write_size
            data_flash_start_index = block_flag_start_index * flash_write_size
            ### Add Data Flash data
            for data_flash_index in range(block_flag_data_size):
                data_flash_str += data_flash_buf[data_flash_start_index + data_flash_index]
            block_flag_start_index = -1
            block_flag_data_size = 0
        block_flag_index_cnt += 1
        if block_flag_index_cnt >= data_flash_block_flg_num:
            break
# User Program Data Add Data Flash
user_program_data_flash_str = user_program_str + data_flash_str
# Set FFh to the unused data in the address information of the RSU header.
for data_cnt in range(rsu_addr_info.program_num, 31):
    rsu_addr_info.program_start_address.append(0xFFFFFFFF)
    rsu_addr_info.program_data_size.append(0xFFFFFFFF)
# RSU address Infomation Create (200h - 2FFh)
if (rsu_format == RSU_BAREMETAL) or (rsu_format == RSU_RTOS):                                               # Generate RSU file for FW Update 2.00 (bare metal) environment.
    rsu_addr_info_bin = bytearray(rsu_addr_info.program_num.to_bytes(4, 'little'))
    for program_cnt in range(31):
        rsu_addr_info_bin += bytearray(rsu_addr_info.program_start_address[program_cnt].to_bytes(4, 'little'))
        rsu_addr_info_bin += bytearray(rsu_addr_info.program_data_size[program_cnt].to_bytes(4, 'little'))
    rsu_addr_info_bin += bytearray(rsu_addr_info.reserved.to_bytes(4, 'little'))
elif (rsu_format == RSU_BAREMETAL_FWUP_V1) or (rsu_format == RSU_RTOS_FWUP_V1):                                             # Generate RSU file for FW Update 2.00 (FreeRTOS OTA) environment.
    rsu_addr_info_bin = (b'\xFF')*256                                                                            # Set FF to the RSU header information (100h bytes).
elif (rsu_format == RSU_RTOS_FWUP_V2_V1_DATA) or (rsu_format == RSU_BAREMETAL_FWUP_V2_V1_DATA):
    user_program_num = 1
    user_program_area_start_address_offset = user_program_area_start_address + 0x300
    user_program_area_max_size = user_program_area_end_address - user_program_area_start_address_offset + 1
    rsu_format_type = 1
    rsu_addr_info_bin = bytearray(user_program_num.to_bytes(4, 'little'))
    rsu_addr_info_bin += bytearray(user_program_area_start_address_offset.to_bytes(4, 'little'))
    rsu_addr_info_bin += bytearray(user_program_area_max_size.to_bytes(4, 'little'))
    rsu_addr_info_bin += (b'\xFF')*240
    rsu_addr_info_bin += bytearray(rsu_format_type.to_bytes(4, 'little'))
# Add RSU address information to the signature verification data.

# Convert user program string to binary.
rsu_str = ''.join(s for s in user_program_data_flash_str)
user_program_data_bin = binascii.unhexlify(rsu_str)

# Add user program to signature verification data.
if (rsu_format == RSU_BAREMETAL) or (rsu_format == RSU_RTOS):                                                                 # Generate RSU file for FW Update 2.00 environment.
    sig_verification_bin = rsu_addr_info_bin + user_program_data_bin                                                          # Added user program to RSU header information for FW Update 2.00 (bare metal) environment.
elif (rsu_format == RSU_BAREMETAL_FWUP_V1) or (rsu_format == RSU_RTOS_FWUP_V1) or (rsu_format == RSU_RTOS_FWUP_V2_V1_DATA) or (rsu_format == RSU_BAREMETAL_FWUP_V2_V1_DATA):   # Generate RSU file for FW Update 1.xx environment.
    sig_verification_bin = rsu_addr_info_bin + user_program_all_buf_rsu_header_cut_bin                                        # Added user program to RSU header information for FW Update 1.xx (bare metal) environment.
hs_data_bin = hashlib.sha256(sig_verification_bin).digest()

if verification_type == SIG_SHA256_ECDSA:                            # sig-sha256-ecdsa
    signature_int = (bytearray(b'\x00')*64)
    # read key
    rf = open(input_key_file,'r')
    private_key_pem_str = rf.read()
    rf.close()
    signature = ecdsa_signature(private_key_pem_str,sig_verification_bin)
    # generate Signiture
    for num in range(len(signature)):
        signature_int[num] = signature[num]
# RSU Header Information Create (0h - 1FFh)
# if (rsu_format == RSU_BAREMETAL) or (rsu_format == RSU_RTOS) or (rsu_format == RSU_BAREMETAL_FWUP_V1) or (rsu_format == RSU_RTOS_FWUP_V1):
if (rsu_format == RSU_BAREMETAL) or (rsu_format == RSU_RTOS):           # Generate RSU file for FW Update 2.00 (bare metal) environment.
    rsu_header_bin = b'RELFWV2'                                              # Magic Code
elif (rsu_format == RSU_BAREMETAL_FWUP_V1) or (rsu_format == RSU_RTOS_FWUP_V1) or (rsu_format == RSU_RTOS_FWUP_V2_V1_DATA) or (rsu_format == RSU_BAREMETAL_FWUP_V2_V1_DATA):  # Generate RSU file for FW Update 1.xx (bare metal) environment.
    rsu_header_bin = b'Renesas'                                              # Magic Code
rsu_header_bin += b'\xFE'                                                    # Image Flag
if verification_type == SIG_SHA256_ECDSA:
    rsu_header_bin += b'sig-sha256-ecdsa' + (b'\x00'*16)                     # Firmware Verification Type
    rsu_header_bin += (64).to_bytes(4, byteorder='little')                   # Signature size
    rsu_header_bin += signature_int                                          # Signature(64byte)
else:
    rsu_header_bin += b'hash-sha256' + (b'\x00')*21                          # Firmware Verification Type
    rsu_header_bin += (32).to_bytes(4, byteorder='little')                   # Signature size
    rsu_header_bin += hs_data_bin                                            # Signature(32byte)
    rsu_header_bin += (b'\x00')*32                                           # Signature(32byte)
rsu_header_bin += (b'\xFF')*404                                              # Reserved(404byte)
rsu_output_bin = rsu_header_bin                                              # Add RSU header
rsu_output_bin += sig_verification_bin                                       # Add Verifivation data (RSU Address Information and User Program)
# RSU File size Set
rsu_file_size = len(rsu_output_bin)
rsu_file_bin = bytearray(rsu_output_bin)
rsu_file_size_bin = bytearray(rsu_file_size.to_bytes(4, 'little'))
rsu_file_bin[0x06C:0x06C+4] = rsu_file_size_bin
# RSU File Create
if output_rsu_file_name_str != "":
    if (rsu_format == RSU_RTOS) or (rsu_format == RSU_RTOS_FWUP_V1) or (rsu_format == RSU_RTOS_FWUP_V2_V1_DATA):
        rsu_file_data_bin = rsu_file_bin[0x200:]
    else:
        rsu_file_data_bin = rsu_file_bin
    with open(output_rsu_file_name_str,'wb') as f:
        f.write(rsu_file_data_bin)
    f.close()
    print("Successfully generated the",output_rsu_file_name_str,"file.")
# RSU header mot data create.
program_index = 0
data_index = 0
mot_adr = user_program_area_start_address
s_record_str, s_record_len_str, s_record_adr_format = get_s_record_type_str(mot_adr)
rsu_header_bin = rsu_output_bin[0:0x300]

while data_index < len(rsu_header_bin):
    mot_data_str = s_record_len_str
    mot_data_str += format(mot_adr+data_index, s_record_adr_format).upper()
    mot_data_str += rsu_header_bin[data_index:data_index+16].hex().upper()
    mot_sum_str = CalculateMotorolaChecksum(mot_data_str)
    mot_data_str += mot_sum_str
    mot_data_str += '\n'
    mot_data_str = mot_data_str[:0] + s_record_str + mot_data_str[0:]
    data_index += 16
    user_program_mot_data_list.append(mot_data_str)
# User Program mot data create.
program_adr_offset = 0x300
for user_program_count in range(rsu_addr_info.program_num):
    mot_adr = rsu_addr_info.program_start_address[user_program_count]
    data_size = rsu_addr_info.program_data_size[user_program_count]
    s_record_str, s_record_len_str, s_record_adr_format = get_s_record_type_str(mot_adr)
    user_program_bin = rsu_output_bin[program_adr_offset:program_adr_offset+data_size]
    data_index = 0
    while data_index < len(user_program_bin):
        mot_data_str = s_record_len_str
        mot_test_adr_str = format(mot_adr+data_index, s_record_adr_format).upper()
        mot_data_str += format(mot_adr+data_index, s_record_adr_format).upper()
        mot_data_str += user_program_bin[data_index:data_index+16].hex().upper()
        mot_sum_str = CalculateMotorolaChecksum(mot_data_str)
        mot_data_str += mot_sum_str
        mot_data_str += '\n'
        mot_data_str = mot_data_str[:0] + s_record_str + mot_data_str[0:]
        data_index += 16
        adr_type = mot_data_adress_area_check(mot_adr)
        if adr_type == ADDRESS_AREA_TYPE_USER_PROGRAM:
            user_program_mot_data_list.append(mot_data_str)
        elif adr_type == ADDRESS_AREA_TYPE_DATA_FLASH:
            data_flash_mot_data_list.append(mot_data_str)
    program_adr_offset += data_size
#********** output mot file data set **********
if input_bootLoaderFileName_Str != "":
    out_mot_file_data_str = ""                                                                # output mot file str data
    out_mot_file_data_str += "S00F00006177735F746573742E6D6F7408\n"                           # S0 Record aws_test.mot File Name
    # Add Near Boot Loader(RL78 Only)
    if near_boot_loader_mot_str != "":
        out_mot_file_data_str += near_boot_loader_mot_str                                     # Added near Boot Loader mot data to output mot file data.
    # Add Data Flash
    if len(data_flash_mot_data_list) != 0:
        for index in range(len(data_flash_mot_data_list)):
            out_mot_file_data_str += data_flash_mot_data_list[index]                          # Add the mot data of the user program to the data of the mot file for output.
    #** Add Boot Loader ofsm mot data ***
    if len(boot_loader_ofsm_mot_str) != 0:                                                    # dual mode and OFSM list OFSM data exist.
        out_mot_file_data_str += boot_loader_ofsm_mot_str                                     # Added OFSM mot data to output mot file data.
        #** Add Bank1 Boot Loader ***
        if (device_type == DEVICE_TYPE_DUAL_MODE) and (len(bank1_boot_loader_mot_str) != 0):  # dual mode and Bank1 list Bank1 data exist.
            out_mot_file_data_str += bank1_boot_loader_mot_str
    #** Add UserProgram mot data ***
    user_program_mot_str = ""
    for index in range(len(user_program_mot_data_list)):
        user_program_mot_str += user_program_mot_data_list[index]
    out_mot_file_data_str += user_program_mot_str                                             # Adds user program data to the mot file output buffer.
    #** Add Bank0 Boot Loader mot data ***
    if bank0_boot_loader_mot_str != "":
        out_mot_file_data_str += bank0_boot_loader_mot_str
    #** Add S Record data ***
    out_mot_file_data_str += s_record_adr_str
    if (rsu_format == RSU_BAREMETAL) or (rsu_format == RSU_RTOS):                        # Generate RSU file for FW Update 2.00 environment.
        with open(output_mot_file_name_str,'w') as f:
            f.write(out_mot_file_data_str)
        f.close()
        print("Successfully generated the",output_mot_file_name_str,"file.")
input_file_close()

