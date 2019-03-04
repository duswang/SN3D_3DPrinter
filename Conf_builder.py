#!/usr/bin/python3
import sys
import os
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

def cls():
    os.system('cls' if os.name=='nt' else 'clear')

def main():
    global strFileName
    global isNewConf
    global cmd1
    global packedByteArray
    global fileConf

    optionName = None
    layerThickness = 0
    bottomLayerExposure = 0
    numBottomLayer = 0
    bottomLiftSpeed = 0
    normalLayerExposure = 0
    normalLiftSpeed = 0
    liftDistance = 0
    brightness = 0

    while True:
        # 1. Build a new configuration
        # 2. Open an Existing configuration
        # 3. Save
        # 4. Quit
        cls()
        print(ANSI_CYAN + "SN3D Configuration Builder" + ANSI_OFF)
        if cmd1 == 1 or cmd1 == 2 or cmd1 == 3:
            # Get input values
            # 1. Option name ()
            print(ANSI_GREEN + "Loaded Configuration" + ANSI_OFF)
            print("Name: " + optionName)
            print("Layer Thickness(mm): ", layerThickness)
            print("Bottom Exposure duration(ms): ", bottomLayerExposure)
            print("Number of Bottom layers: ", numBottomLayer)
            print("Bottom Layer Lift speed(mm/s): ", bottomLiftSpeed)
            print("Normal layer exposure duration(ms):", normalLayerExposure)
            print("Normal layer Lift Speed(mm/s): ", normalLiftSpeed)
            print("Lift Distance(mm): ", liftDistance)
            print("Backlight Brightness: ", brightness)

            print("=========================================\n\n\n")

        print("1. Build a new configuration")
        print("2. Open an exisiting configuration")
        print("3. Edit Configuration items")
        print("4. Save")
        print("5. Quit")
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
                optionName = input("Enter option name: ")
                if (len(optionName) >= 8):
                    optionName = optionName[0:7]
                layerThickness = float(input("Enter Layer Thickness(mm): "))
                layerThickness = round(layerThickness, 5)
                bottomLayerExposure = int(input("Enter Bottom Exposure(ms): "))
                numBottomLayer = int(input("Enter number of bottom layer: "))
                bottomLiftSpeed = float(input("Enter Bottom Layer Lift Speed(mm/s): "))
                normalLayerExposure = int(input("Enter Normal Layer Exposure(ms): "))
                normalLiftSpeed = float(input("Enter Normal Layer Lift Speed: "))
                liftDistance = int(input("Enter Lift distance(mm): "))
                brightness = int(input("Enter Brightness (0-255): "))

                packedByteArray = struct.pack(">8sfiififii", bytes(optionName, "utf-8"), layerThickness, bottomLayerExposure, numBottomLayer,
                                    bottomLiftSpeed, normalLayerExposure, normalLiftSpeed, liftDistance, brightness)
            except ValueError:
                print(ANSI_RED + "Invalid Input Value." + ANSI_OFF)

            sleep(2)
            cls()
            continue

        elif cmd1 == 2:
            isNewConf = False
            # Load file and unpack using struct.unpack_from
            cls()
            strFileName = input("Open Configuration File (ex: option_1.s3c): ")
            if (strFileName == ''):
                strFileName = 'option_1.s3c'

            print(ANSI_RED + "Opening " + strFileName + ANSI_OFF)
            try:
                fileConf = open(strFileName, 'rb')
                packedByteArray = fileConf.read()
                optionName, layerThickness, bottomLayerExposure, numBottomLayer, bottomLiftSpeed, \
                normalLayerExposure, normalLiftSpeed, liftDistance, brightness = struct.unpack(">8sfiififii", packedByteArray)
                optionName = str(optionName, "utf-8")
                layerThickness = round(layerThickness, 5)
            except IOError:
                print(ANSI_RED + 'Failed to open %s.'%strFileName + ANSI_OFF)
                cmd1 = 0

            sleep(2)
            cls()
            continue

        elif cmd1 == 3:
            if packedByteArray == None:
                print(ANSI_RED + "No Configuration Loaded")
                sleep(2)
                continue
            cls()
            print("1. Name: " + optionName)
            print("2. Layer Thickness(mm): ", layerThickness)
            print("3. Bottom Exposure duration(ms): ", bottomLayerExposure)
            print("4. Number of Bottom layers: ", numBottomLayer)
            print("5. Bottom Layer Lift speed(mm/s): ", bottomLiftSpeed)
            print("6. Normal layer exposure duration(ms):", normalLayerExposure)
            print("7. Normal layer Lift Speed(mm/s): ", normalLiftSpeed)
            print("8. Lift Distance(mm): ", liftDistance)
            print("9. Backlight Brightness: ", brightness)
            print()
            try:
                cmd2 = int(input("Choose an item: "))

                if cmd2 == 1:
                    print()
                    optionName = input("Enter option name: ")
                    if (len(optionName) >= 8):
                        optionName = optionName[0:7]
                elif cmd2 == 2:
                    print()
                    layerThickness = float(input("Enter Layer Thickness(mm): "))
                    layerThickness = round(layerThickness, 5)
                elif cmd2 == 3:
                    print()
                    bottomLayerExposure = int(input("Enter Bottom Exposure(ms): "))
                elif cmd2 == 4:
                    print()
                    numBottomLayer = int(input("Enter number of bottom layer: "))
                elif cmd2 == 5:
                    print()
                    bottomLiftSpeed = float(input("Enter Bottom Layer Lift Speed(mm/s): "))
                elif cmd2 == 6:
                    print()
                    normalLayerExposure = int(input("Enter Normal Layer Exposure(ms): "))
                elif cmd2 == 7:
                    print()
                    normalLiftSpeed = float(input("Enter Normal Layer Lift Speed: "))
                elif cmd2 == 8:
                    print()
                    liftDistance = int(input("Enter Lift distance(mm): "))
                elif cmd2 == 9:
                    print()
                    brightness = int(input("Enter Brightness (0-255): "))
                else:
                    continue

                    packedByteArray = struct.pack(">8sfiififii", bytes(optionName, "utf-8"), layerThickness, bottomLayerExposure, numBottomLayer,
                                        bottomLiftSpeed, normalLayerExposure, normalLiftSpeed, liftDistance, brightness)

            except ValueError:
                print(ANSI_RED + "Invalid Input Value." + ANSI_OFF)
                sleep(2)

            continue

        elif cmd1 == 4:
            if packedByteArray == None:
                print(ANSI_RED + "Configuration has not been completed!" + ANSI_OFF)
                sleep(3)
                continue

            if isNewConf:
                strFileName = input("Configuration File (ex: option_1.s3c): ")

            if strFileName == '':
                strFileName = 'option_1.s3c'

            # open new file to save
            try:
                fileConf = open(strFileName, 'wb')
                print('saving configuration to ' + strFileName)
                fileConf.write(packedByteArray)
            except IOError:
                print(ANSI_RED + "Failed to create %s."%strFileName + ANSI_OFF)
            finally:
                fileConf.close()

            sleep(2)
            cls()
            continue
        elif cmd1 == 5:
            cls()
            sys.exit(0)

        else:
            cls()
            continue



if __name__== "__main__":
    main()
