#!/usr/bin/python3

## INCLUDE ##
# SYSTEM #
import sys
import os
from enum import Enum

# TIME #
import datetime
from time import sleep

# HASH #
import hashlib

# XML #
import xml.etree.cElementTree as ET

import struct

## DEFINE ##
# F/W #
FW_PATH     = "FirmwareFiles"
FW_NAME     = "sn3d_fw"
FW_RESOURCE = FW_PATH + "/" + "res"

# BINARY #
BIN_PATH    = "/SN3D/sn3d-project/bin/build"
BIN_NAME    = "sn3d"

# HASH #
HASH_MAGIC_NUMBER = "0xDEADBEEF"

# PAGE #
NONE_PAGE   = 0
INPUT_PAGE  = 1
EXIT_PAGE   = 5

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

def cls():
    os.system('cls' if os.name=='nt' else 'clear')


## FILE CONTROL ##
def mkdir(path):
    # Create target directory & all intermediate directories if don't exists
    try:
        os.makedirs(path)
        print("Directory " , path ,  " Created ")
    except FileExistsError:
        print("Directory " , path ,  " already exists")

## XML ##
def mkVersionInfo():
    global versionInfo
    
    mkdir(FW_RESOURCE)
    
    root = ET.Element(FW_RESOURCE)

    ET.SubElement(root, "project").text       = str(versionInfo['project'])
    ET.SubElement(root, "releaseNumber").text = str(versionInfo['releaseNumber'])
    ET.SubElement(root, "majorNumber").text   = str(versionInfo['majorNumber'])
    ET.SubElement(root, "minorNumber").text   = str(versionInfo['minorNumber'])
    ET.SubElement(root, "timestamp").text     = str(versionInfo['timestamp'])
    ET.SubElement(root, "binaryName").text    = str(versionInfo['binaryName'])
    ET.SubElement(root, "hash").text          = str(versionInfo['hash'])

    tree = ET.ElementTree(root)
    tree.write(FW_RESOURCE + "/" + "versionInfo.xml")

## HASH FUNCTION ##
def md5(fname):
    hash_md5 = hashlib.md5()
    with open(fname, "rb") as f:
        for chunk in iter(lambda: f.read(4096), b""):
            hash_md5.update(chunk)
    return hash_md5.hexdigest()


## PAGE FUNCTIONS ##
def mainPage(command):
    cls()
    
    print(ANSI_CYAN + "SN3D F/W Builder" + ANSI_OFF)
    print("=========================================")
    
    if command != NONE_PAGE :
        # Get input values
        print(ANSI_GREEN + "Loaded Version Info" + ANSI_OFF)
        print("Project   : " + versionInfo['project'])
        print("Version   : ",versionInfo['releaseNumber'],".",versionInfo['majorNumber'],".",versionInfo['minorNumber'])
        print("Timestamp : ", versionInfo['timestamp'])
        print("Hash      : ", versionInfo['hash'])
        print("=========================================")


    # ref DEF_PAGE
    print(INPUT_PAGE, "Generate F/W File")
    print(EXIT_PAGE, "Quit")
    print("")
    try:
        command = int(input("Choose an item: "))
    except ValueError:
        print(ANSI_RED + "Invalid Input Value." + ANSI_OFF)
        sleep(2)
    return command

def inputPage():
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
        versionInfo['hash']          = md5(BIN_PATH + "/" + BIN_NAME)
    
    except ValueError:
        print(ANSI_RED + "Invalid Input Value." + ANSI_OFF)
        
        sleep(3)
        cls()
        return STATUS_IS_NOT_OK

    mkVersionInfo()
    return STATUS_IS_OK

def exitPage():
    cls()
    sys.exit(0)




## MAIN PAGE ##
def main():
    status = STATUS_IS_OK
    
    # Create F/W Directory #
    mkdir(FW_PATH)
    
    command = NONE_PAGE
    
    while True:
        command = mainPage(command)
        
        if command == INPUT_PAGE:
            status = inputPage()
        
        elif command == EXIT_PAGE:
            exitPage()


if __name__== "__main__":
    main()


