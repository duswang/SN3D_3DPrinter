#!/usr/bin/python3

## INCLUDE ##
# SYSTEM #
import sys
import os


# TYPE #
from enum import Enum
import struct

# TIME #
import datetime
from time import sleep

# HASH #
import hashlib

# XML #
import xml.etree.cElementTree as ET

# FILE #
from shutil import copyfile
import zipfile
import shutil

## DEFINE ##
# F/W #
FW_PATH     = "FirmwareFiles"
FW_NAME     = "sn3d_fw"
FW_RESOURCE = FW_PATH + "/" + "res"

# BINARY #
BIN_PATH    = "bin/build"
BIN_NAME    = "sn3d"

# MACHINE CONFIG #
MACHINE_INFO_PATH         = "sn3d-bootloader/lib/machineConfig"
MACHINE_INFO_NAME_5_5     = "machineInfo_5_5.xml"
MACHINE_INFO_NAME_5_5_2K  = "machineInfo_5_5_2k.xml"
MACHINE_INFO_NAME_8_9     = "machineInfo_8_9.xml"
MACHINE_INFO_NAME_15_6    = "machineInfo_15_6.xml"
MACHINE_INFO_NAME_23_8    = "machineInfo_23_8.xml"
MACHINE_INFO_NAME_DEFAULT = MACHINE_INFO_NAME_5_5

MACHINE_INFO_5_5     = 1
MACHINE_INFO_5_5_2K  = 2
MACHINE_INFO_8_9     = 3
MACHINE_INFO_15_6    = 4
MACHINE_INFO_23_8    = 5
MACHINE_INFO_DEFAULT = MACHINE_INFO_5_5

# HASH #
HASH_MAGIC_NUMBER = "0xDEADBEEF"

# PAGE #
NONE_PAGE          = 0
VERSION_INFO_PAGE  = 1
MACHINE_INFO_PAGE  = 2
SAVE_FW_PAGE       = 3
EXIT_PAGE          = 5

# ERROR #
STATUS_IS_OK        = 0x01
STATUS_IS_NOT_OK    = 0xFF

# SCREEN #
if os.getenv('C', '1') == '0':
    ANSI_RED = ''
    ANSI_GREEN = ''
    ANSI_YELLOW = ''
    ANSI_CYAN = ''
    ANSI_WHITE = ''
    ANSI_OFF = ''
else:
    ANSI_CSI = "\033["
    ANSI_dRED = ANSI_CSI + '31m'
    ANSI_GREEN = ANSI_CSI + '32m'
    ANSI_YELLOW = ANSI_CSI + '33m'
    ANSI_CYAN = ANSI_CSI + '36m'
    ANSI_WHITE = ANSI_CSI + '37m'
    ANSI_OFF = ANSI_CSI + '0m'


## CONSOLE ##

versionInfo = {
    'project':       "project",
    'releaseNumber':         0,
    'majorNumber':           0,
    'minorNumber':           0,
    'timestamp':        "NOPE",
    'binaryName':       "sn3d",
    'hash':                0x00
}

machineInfo = MACHINE_INFO_DEFAULT;

def cls():
    os.system('cls' if os.name=='nt' else 'clear')


## FILE CONTROL ##
def mkdir(path):
    # Create target directory & all intermediate directories if don't exists
    try:
        os.makedirs(path)
        print("Directory " , path ,  " Created ")
    except FileExistsError:
        print("Directory " , path ,  " Already exists")

def rmdir(path):
    try:
        shutil.rmtree(path)
        print("Directory " , path ,  " Remove")
    except FileExistsError:
        print("Directory " , path ,  " No such directory")

## XML ##
def mkVersionInfo():
    global versionInfo
    
    mkdir(FW_RESOURCE)
    
    root = ET.Element("version")

    ET.SubElement(root, "project").text       = str(versionInfo['project'])
    ET.SubElement(root, "releaseNumber").text = str(versionInfo['releaseNumber'])
    ET.SubElement(root, "majorNumber").text   = str(versionInfo['majorNumber'])
    ET.SubElement(root, "minorNumber").text   = str(versionInfo['minorNumber'])
    ET.SubElement(root, "timestamp").text     = str(versionInfo['timestamp'])
    ET.SubElement(root, "binaryName").text    = str(versionInfo['binaryName'])
    ET.SubElement(root, "hash").text          = str(versionInfo['hash'])

    tree = ET.ElementTree(root)
    tree.write(FW_RESOURCE + "/" + "version.xml")

## HASH FUNCTION ##
def md5(fname):
    hash_md5 = hashlib.md5()
    with open(fname, "rb") as f:
        for chunk in iter(lambda: f.read(4096), b""):
            hash_md5.update(chunk)
    return hash_md5.hexdigest()


## PAGE DISPLAY FUNCTIONS ##
def infoScreen():
    # Get Version values
    print(ANSI_GREEN + "Saved Version Info" + ANSI_OFF)
    print("Project   :  " + versionInfo['project'])
    print("Version   : ", versionInfo['releaseNumber'],".",versionInfo['majorNumber'],".",versionInfo['minorNumber'])
    print("Timestamp : ", versionInfo['timestamp'])
    print("Hash      : ", versionInfo['hash'])
    print("=========================================")
    
    # Get Machine Info values
    print(ANSI_GREEN + "Saved Machine Info" + ANSI_OFF)
    if machineInfo == MACHINE_INFO_5_5:
        print("Machine Inch   : " + " 5.5 Inch")
    elif machineInfo == MACHINE_INFO_5_5_2K:
        print("Machine Inch   : " + " 5.5 Inch 2K")
    elif machineInfo == MACHINE_INFO_8_9:
        print("Machine Inch   : " + " 8.9 Inch")
    elif machineInfo == MACHINE_INFO_15_6:
        print("Machine Inch   : " + "15.6 Inch")
    elif machineInfo == MACHINE_INFO_23_8:
        print("Machine Inch   : " + "23.8 Inch")
    else:
        print("Machine Inch   : " + "N/A")
    print("=========================================")

## PAGE FUNCTIONS ##
def mainPage(command):
    cls()
    
    print(ANSI_CYAN + "SN3D F/W Builder" + ANSI_OFF)
    print("=========================================")
    
    infoScreen()

    # ref DEF_PAGE
    print(VERSION_INFO_PAGE, "Setup  Version Info File")
    print(MACHINE_INFO_PAGE, "Choose Machine Info File")
    print(SAVE_FW_PAGE,      "Export F/W File")
    print(EXIT_PAGE, "Quit")
    print("")
    try:
        command = int(input("Choose an item: "))
    except ValueError:
        print(ANSI_RED + "Invalid Input Value." + ANSI_OFF)
        sleep(2)
    return command

def versionInfoPage():
    global versionInfo
    
    cls()
    
    try:
        versionInfo['project']       =     input("Enter project name : ")
        if (len(versionInfo['project']) >= 8):
            versionInfo['project'] = project[0:7]
    
        versionInfo['releaseNumber'] = int(input("release number     : "))
        versionInfo['majorNumber']   = int(input("major number       : "))
        versionInfo['minorNumber']   = int(input("minor number       : "))
        versionInfo['timestamp']     = datetime.date.today().strftime("%B %d, %Y")
        versionInfo['binaryName']    = BIN_NAME
        
        hash_anwser = input("\nUse Magic Number on Hash?[Y/n] : ")
        if  hash_anwser == "Y" or hash_anwser == "y" or hash_anwser == "yes":
            versionInfo['hash']      = HASH_MAGIC_NUMBER
        else:
            versionInfo['hash']      = md5(BIN_PATH + "/" + BIN_NAME)
    
    except ValueError:
        print(ANSI_RED + "Invalid Input Value." + ANSI_OFF)
        
        sleep(3)
        cls()
        return STATUS_IS_NOT_OK

    mkVersionInfo()

    return STATUS_IS_OK

def machineInfoPage():
    global machineInfo
    
    cls()
    
    # ref DEF_PAGE
    print(1, "5.5  Inch")
    print(2, "5.5  Inch 2K")
    print(3, "8.9  Inch")
    print(4, "15.6 Inch")
    print(5, "23.8 Inch")
    print(6, "Quit")
    print("")
    try:
        machine_item = int(input("Choose an item: "))
    except ValueError:
        print(ANSI_RED + "Invalid Input Value." + ANSI_OFF)
        sleep(2)
        cls()
        return STATUS_IS_NOT_OK


    machineInfo = machine_item

    if machine_item == MACHINE_INFO_5_5:
        copyfile(MACHINE_INFO_PATH + "/" + MACHINE_INFO_NAME_5_5, FW_RESOURCE + "/" + "machineInfo.xml")
    elif machine_item == MACHINE_INFO_5_5_2K:
        copyfile(MACHINE_INFO_PATH + "/" + MACHINE_INFO_NAME_5_5_2K, FW_RESOURCE + "/" + "machineInfo.xml")
    elif machine_item == MACHINE_INFO_8_9:
        copyfile(MACHINE_INFO_PATH + "/" + MACHINE_INFO_NAME_8_9, FW_RESOURCE + "/" + "machineInfo.xml")
    elif machine_item == MACHINE_INFO_15_6:
        copyfile(MACHINE_INFO_PATH + "/" + MACHINE_INFO_NAME_15_6, FW_RESOURCE + "/" + "machineInfo.xml")
    elif machine_item == MACHINE_INFO_23_8:
        copyfile(MACHINE_INFO_PATH + "/" + MACHINE_INFO_NAME_23_8, FW_RESOURCE + "/" + "machineInfo.xml")
    else:
        return STATUS_IS_OK

    return STATUS_IS_OK

def generateFWPage():
    ret = 0
    
    global versionInfo
    global machineInfo
    
    firmwareName = input("Enter project name : ")
    
    FW_ZIP = zipfile.ZipFile(FW_PATH + "/" + FW_NAME + "_" + firmwareName + ".zip", 'w')

    for folder, subfolders, files in os.walk(FW_RESOURCE):
        for file in files:
            #if folder.find("AppleDouble") == -1:
                print(folder + "/" + file)
                FW_ZIP.write(os.path.join(folder, file), file, compress_type = zipfile.ZIP_DEFLATED)

    FW_ZIP.close()

    sleep(3)

    return ret

def exitPage():
    cls()
    sys.exit(0)




## MAIN PAGE ##
def main():
    status = STATUS_IS_OK
    

    # Create F/W Directory #
    mkdir(FW_PATH)
    rmdir(FW_PATH)
    
    mkdir(FW_RESOURCE)
    
    sleep(2)
    
    copyfile(BIN_PATH + "/" + BIN_NAME, FW_RESOURCE + "/" + BIN_NAME)
    copyfile(MACHINE_INFO_PATH + "/" + MACHINE_INFO_NAME_5_5, FW_RESOURCE + "/" + "machineInfo.xml")
    
    command = NONE_PAGE
    
    while True:
        command = mainPage(command)
        
        if command == VERSION_INFO_PAGE:
            status = versionInfoPage()

        elif command == MACHINE_INFO_PAGE:
            status = machineInfoPage()
        
        elif command == SAVE_FW_PAGE:
            status = generateFWPage()
        
        elif command == EXIT_PAGE:
            exitPage()


if __name__== "__main__":
    main()


