/**
 * @file SN_VERSION_CONTROOL.c
 * @author Bato
 * @date 18 Sep 2018
 * @brief
 *
 * @see https://bitbucket.org/xengiennering/sn3d-project/src/master/
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */
#include "SN_BOOTLOADER.h"
#include "SN_VERSION_CONTROL.h"

#define FIRMWARE_PATH                    "/SN3D/sn3d-project/sn3d-bootloader/res/firmware"
#define TEMP_FIRMWARE_PATH               "/SN3D/sn3d-project/sn3d-bootloader/res/tempFirmware"

#define FIRMWARE_VERSION_PATH            "/SN3D/sn3d-project/sn3d-bootloader/res/firmware/version.xml"
#define TEMP_FIRMWARE_VERSION_PATH       "/SN3D/sn3d-project/sn3d-bootloader/res/tempFirmware/version.xml"

#define FIRMWARE_MACHINE_INFO_PATH       "/SN3D/sn3d-project/sn3d-bootloader/res/firmware/machineInfo.xml"
#define TEMP_FIRMWARE_MACHINE_INFO_PATH  "/SN3D/sn3d-project/sn3d-bootloader/res/tempFirmware/machineInfo.xml"

#define FIRMWARE_BINARY_PATH             "/SN3D/sn3d-project/sn3d-bootloader/res/firmware/exe"
#define TEMP_FIRMWARE_BINARY_PATH        "/SN3D/sn3d-project/sn3d-bootloader/res/tempFirmware/exe"

#define SN3D_BINARY_FOLDER_PATH          "/SN3D/sn3d-project/bin/build"
#define SN3D_BINARY_PATH                 "/SN3D/sn3d-project/bin/build/exe"

#define SN3D_OPTION_FOLDER_PATH          "/SN3D/sn3d-project/res/optionConfig"

#define SN3D_MACHINE_CONFIG_FOLDER_PATH  "/SN3D/sn3d-project/res/machineConfig"
#define SN3D_MACHINE_CONFIG_PATH         "/SN3D/sn3d-project/res/machineConfig/machineInfo.xml"

#define MACHINE_5_5_INCH_PATH            "/SN3D/sn3d-project/sn3d-bootloader/res/bootConfig/5_5.txt"
#define MACHINE_8_9_INCH_PATH            "/SN3D/sn3d-project/sn3d-bootloader/res/bootConfig/8_9.txt"
#define MACHINE_15_6_INCH_PATH           "/SN3D/sn3d-project/sn3d-bootloader/res/bootConfig/4k.txt"
#define MACHINE_23_8_INCH_PATH           MACHINE_15_6_INCH_PATH
#define MACHINE_DEFAULT_INCH_PATH        MACHINE_5_5_INCH_PATH
#define MACHINE_BOOTCONFIG_PATH          "/boot/config.txt"


void sFirmwareFileLoad(const char* path);
void sFirmwareFileCompare(void);
void sMachineInfoCompare(void);
void sUpdateBootConfig(float inch);

bool isNeedReboot = false;

const char* SN_VERSION_CONTROL_VersionCheck(void)
{
    struct dirent **nameList;
    int numberOfnameList = 0;
    int i = 0;

    bool isFirmwareFileExit = false;

    char nameBuffer[MAX_PATH_LENGTH];
    char path[MAX_PATH_LENGTH];

    char* firmwareFilename = NULL;

    numberOfnameList = scandir(USB_PATH, &nameList, 0, alphasort);
    if(numberOfnameList < 0)
    {
        perror("scandir");
    }
    else
    {
        for(i = 0; i < numberOfnameList; i++)
        {
            sprintf(nameBuffer, "%s", nameList[i]->d_name);

            if((strstr(nameBuffer, SN3D_FW_STR) != NULL)&& \
              (strstr(nameBuffer, HIDDEN_FILE_STR) == NULL) && \
              (nameBuffer[0] != '.'))
            {
                if(isFirmwareFileExit != true)
                {
                    if ((firmwareFilename = malloc (strlen(nameBuffer) + 1)) == NULL)
                    {
                        return NULL;
                    }

                    strcpy (firmwareFilename, nameBuffer);

                    printf("Firmware File Name : [ %s ]\n", firmwareFilename);

                    isFirmwareFileExit = true;
                }
            }

            if(strstr(nameBuffer, SN3D_OPTION_STR) && !strstr(nameBuffer, HIDDEN_FILE_STR))
            {

                printf("Option   File Name : [ %s ]\n", nameBuffer);

                sprintf(path, "%s/%s", USB_PATH, nameBuffer);

                printf("Option   File Path : [ %s ]\n", path);

                FileSystem_fctl_RemoveFiles(SN3D_OPTION_FOLDER_PATH);

                FileSystem_fctl_ExtractFile(path, SN3D_OPTION_FOLDER_PATH);

                isFirmwareFileExit = true;
            }


            free(nameList[i]);
            nameList[i] = NULL;
        }
        free(nameList);
        nameList = NULL;
    }

    if(!isFirmwareFileExit)
    {
        printf(" Can't not found Firmware Update File...\n");
    }

    return firmwareFilename;
}

void SN_VERSION_CONTROL_VersionUpdate(const char* fileName)
{
    char path[MAX_PATH_LENGTH];

    printf("%s", fileName);
    sprintf(path, "%s/%s", USB_PATH, fileName); fflush(stdout);
    printf("Firmware File Path : [ %s ]\n", path); fflush(stdout);

    sFirmwareFileLoad(path);

    sFirmwareFileCompare();

    SN_SYS_Log("\n\nFirmware Everything up-to-date.");
}

void sFirmwareFileLoad(const char* newFirmwarPath)
{
    /* Make Temp Directory */
    FileSystem_fctl_MakeDirectory(TEMP_FIRMWARE_PATH);

    /* Remove Old Firmware Temp File */
    FileSystem_fctl_RemoveFiles(TEMP_FIRMWARE_PATH);

    /* Copy to new Firmware File */
    FileSystem_fctl_ExtractFile(newFirmwarPath, TEMP_FIRMWARE_PATH);

}

void sFirmwareFileCompare(void)
{
    bool isNeedUpdate = false;
    versionInfo_t* currentVersion = NULL;
    versionInfo_t* newVersion = NULL;

    currentVersion = FileSystem_versionInfoXMLLoad(FIRMWARE_VERSION_PATH);

    newVersion = FileSystem_versionInfoXMLLoad(TEMP_FIRMWARE_VERSION_PATH);

    printf("\n\n * Current Version * \n");
    printf(" Project   : [ %s ] \n", currentVersion->name);
    printf(" Version   : [ v%ld.%ld.%ld ] \n", currentVersion->releaseNumber, currentVersion->majorNumber, currentVersion->minorNumber);
    printf(" Timestamp : [ %s ] \n", currentVersion->timestamp);
    printf(" binray    : [ %s ] \n\n", currentVersion->binaryName);

    printf("\n\n * New Version * \n");
    printf(" Project   : [ %s ] \n", newVersion->name);
    printf(" Version   : [ v%ld.%ld.%ld ] \n", newVersion->releaseNumber, newVersion->majorNumber, newVersion->minorNumber);
    printf(" Timestamp : [ %s ] \n", newVersion->timestamp);
    printf(" binray    : [ %s ] \n\n", newVersion->binaryName);

    if(newVersion->releaseNumber > currentVersion->releaseNumber)
    {
        isNeedUpdate = true;
    }
    else if(newVersion->majorNumber > currentVersion->majorNumber)
    {
        isNeedUpdate = true;
    }
    else if(newVersion->minorNumber > currentVersion->minorNumber)
    {
        isNeedUpdate = true;
    }

    if(isNeedUpdate)
    {
        printf("\n\n * Remove Old Firmware Verision... * \n");
        /* Remove Old Firmware Temp File */
        FileSystem_fctl_RemoveFiles(FIRMWARE_PATH);
        printf("\n\n * Updateing... * \n");

        FileSystem_fctl_CopyFile(TEMP_FIRMWARE_VERSION_PATH, FIRMWARE_VERSION_PATH);
        FileSystem_fctl_CopyFile(TEMP_FIRMWARE_BINARY_PATH, FIRMWARE_BINARY_PATH);

        /* Replace Old Firmware Binary File */
        printf("\n\n * Update Binary to SN3D Service... * \n");
        FileSystem_fctl_RemoveFiles(SN3D_BINARY_FOLDER_PATH);
        FileSystem_fctl_CopyFile(FIRMWARE_BINARY_PATH, SN3D_BINARY_PATH);

        printf("\n * Update Completed. * \n");

        isNeedReboot = true;
    }
    else
    {
        printf("\n * Current firmware version is newest version. * \n\n");
    }

    /* Always Update */
    FileSystem_fctl_CopyFile(TEMP_FIRMWARE_MACHINE_INFO_PATH, FIRMWARE_MACHINE_INFO_PATH);

    /* Update to SN3D Service */
    FileSystem_fctl_RemoveFiles(SN3D_MACHINE_CONFIG_FOLDER_PATH);
    FileSystem_fctl_CopyFile(FIRMWARE_MACHINE_INFO_PATH, SN3D_MACHINE_CONFIG_PATH);

    sMachineInfoCompare();

    FileSystem_fctl_RemoveFiles(TEMP_FIRMWARE_PATH);

    if(isNeedReboot == true)
    {
        SN_SYS_Log("System Reboot. \n\n");

        sync();
        reboot(RB_AUTOBOOT);
    }
}

void sMachineInfoCompare(void)
{
    machineInfo_t* currentMachineInfo = NULL;

    FB_Window_t* currentDisplay = NULL;

    currentMachineInfo = FileSystem_machineInfoXMLLoad(FIRMWARE_MACHINE_INFO_PATH);

    currentDisplay = SN_MACHINE_CONTROL_DisplayCheck();

    printf("\n\n * Current Machine Info * \n");
    printf(" Name       : [ %s ] \n", currentMachineInfo->name);
    printf(" Resolution : [ %ld x %ld px ] \n", currentMachineInfo->screenWidth, currentMachineInfo->screenHeight);
    printf(" Display    : [ %.1f inch ] \n", currentMachineInfo->inch);
    printf(" Z Limit    : [ %ld mm ] \n\n", currentMachineInfo->machineHeight);

    if((currentDisplay->w != currentMachineInfo->screenWidth) || (currentDisplay->h != currentMachineInfo->screenHeight))
    {
        sUpdateBootConfig(currentMachineInfo->inch);
        FileSystem_fctl_CopyFile(TEMP_FIRMWARE_MACHINE_INFO_PATH, FIRMWARE_MACHINE_INFO_PATH);

        isNeedReboot = true;
    }
    else
    {
        SN_SYS_Log("Don't Need to update mahcine.\n\n");
    }

    free(currentDisplay);
}

void sUpdateBootConfig(float inch)
{
    SN_SYS_Log(" *\n Updating Machine... *");

    if(inch == (float)5.5)
    {
        SN_SYS_Log("\n 5.5 Inch Updating...\n\n");
        FileSystem_fctl_CopyFile(MACHINE_5_5_INCH_PATH, MACHINE_BOOTCONFIG_PATH);
    }
    else if(inch == (float)8.9)
    {
        SN_SYS_Log("\n 8.9 Inch Updating...\n\n");
        FileSystem_fctl_CopyFile(MACHINE_8_9_INCH_PATH, MACHINE_BOOTCONFIG_PATH);
    }
    else if(inch == (float)15.6)
    {
        SN_SYS_Log("\n 15.6 Inch Updating...\n\n");
        FileSystem_fctl_CopyFile(MACHINE_15_6_INCH_PATH, MACHINE_BOOTCONFIG_PATH);
    }
    else if(inch == (float)23.8)
    {
        SN_SYS_Log("\n 23.8 Inch Updating...\n\n");
        FileSystem_fctl_CopyFile(MACHINE_23_8_INCH_PATH, MACHINE_BOOTCONFIG_PATH);
    }
    else
    {
        SN_SYS_Log("\n Default :: 5.5 Inch.\n\n");
        FileSystem_fctl_CopyFile(MACHINE_DEFAULT_INCH_PATH, MACHINE_BOOTCONFIG_PATH);
    }
}
