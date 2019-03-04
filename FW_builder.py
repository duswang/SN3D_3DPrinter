#!/usr/bin/python3
import sys
import os
import datetime
import hashlib

from time import sleep
import struct

if os.getenv('C', '1') == '0':
    ANSI_RED = ''
    ANSI_GREEN = ''
    ANSI_YELLOW = ''
    ANSI_CYAN = ''
    ANSI_WHITE = ''
    ANSI_OFF = ''
else:
    ANSI_CSI = "\033["
    ANSI_RED = ANSI_CSI + '31m'
    ANSI_GREEN = ANSI_CSI + '32m'
    ANSI_YELLOW = ANSI_CSI + '33m'
    ANSI_CYAN = ANSI_CSI + '36m'
    ANSI_WHITE = ANSI_CSI + '37m'
    ANSI_OFF = ANSI_CSI + '0m'

strFileName = None
isNewConf = False
cmd1 = 0
packedByteArray = None
fileConf = None

def md5(fname):
    hash_md5 = hashlib.md5()
    with open(fname, "rb") as f:
        for chunk in iter(lambda: f.read(4096), b""):
            hash_md5.update(chunk)
    return hash_md5.hexdigest()


def cls():
    os.system('cls' if os.name=='nt' else 'clear')

def main():
    global strFileName
    global cmd1
    global packedByteArray
    
    project = None
    releaseNumber = 0
    majorNumber = 0
    minorNumber = 0
    timestamp = 0
    binaryName = 0
    hash = 0
    
    while True:
        # 1. Generate F/W File
        # 4. Quit
        cls()
        print(ANSI_CYAN + "SN3D F/W Builder" + ANSI_OFF)
        if cmd1 == 1 or cmd1 == 2 or cmd1 == 3:
            # Get input values
            print(ANSI_GREEN + "Loaded Configuration" + ANSI_OFF)
            print("Project: " + project)
            print("Version : ",releaseNumber,".",majorNumber,".",minorNumber)
            print("Timestamp : ", timestamp)
            print("Hash : ", hash)
            
            print("=========================================\n\n\n")
        print("1. Generate F/W File")
        print("2. Quit")
        print("")
        try:
            cmd1 = int(input("Choose an item: "))
        except ValueError:
            print(ANSI_RED + "Invalid Input Value." + ANSI_OFF)
            sleep(2)
        
        if cmd1 == 1:
            isNewConf = True
            cls()
            try:
                project = input("Enter project name: ")
                if (len(project) >= 8):
                    project = project[0:7]
                
                releaseNumber = int(input("release number : "))
                majorNumber =   int(input("major number : "))
                minorNumber =   int(input("minor number : "))
                timestamp =     datetime.date.today().strftime("%B %d, %Y")
                binaryName =    "sn3d"
                hash =          md5("/SN3D/sn3d-project/bin/build/sn3d")
    
            except ValueError:
                print(ANSI_RED + "Invalid Input Value." + ANSI_OFF)
                
                sleep(2)
                cls()
                continue
        elif cmd1 == 2:
            cls()
            sys.exit(0)

if __name__== "__main__":
    main()


