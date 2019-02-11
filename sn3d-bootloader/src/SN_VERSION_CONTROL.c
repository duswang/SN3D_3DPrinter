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

/* ******* STATIC DEFINE ******* */

/* ******* GLOBAL VARIABLE ******* */

/* ******* STATIC FUNCTIONS ******* */
static bool sFirmwareFileCompare(void);

static void sFirmwareFileLoad(const char* path);

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
bool SN_VERSION_CONTROL_VersionUpdate(const char* fileName)
{
    char path[MAX_PATH_LENGTH];

    printf("%s", fileName);
    sprintf(path, "%s/%s", USB_PATH, fileName); fflush(stdout);
    printf("Firmware File Path : [ %s ]\n", path); fflush(stdout);

    sFirmwareFileLoad(path);

    return sFirmwareFileCompare();
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Static Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
void sFirmwareFileLoad(const char* newFirmwarPath)
{
    /* Make Temp Directory */
    FileSystem_fctl_MakeDirectory(TEMP_FIRMWARE_PATH);

    /* Remove Old Firmware Temp File */
    FileSystem_fctl_RemoveFiles(TEMP_FIRMWARE_PATH);

    /* Copy to new Firmware File */
    FileSystem_fctl_ExtractFile(newFirmwarPath, TEMP_FIRMWARE_PATH);

}

static bool sFirmwareFileCompare(void)
{
    bool isNeedUpdate = false;
    char path[MAX_PATH_LENGTH];

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

        sprintf(path, "%s/%s", TEMP_FIRMWARE_PATH, newVersion->binaryName);

        FileSystem_fctl_CopyFile(TEMP_FIRMWARE_VERSION_PATH, FIRMWARE_VERSION_PATH);
        FileSystem_fctl_CopyFile(path, FIRMWARE_BINARY_PATH);

        /* Replace Old Firmware Binary File */
        printf("\n\n * Update Binary to SN3D Service... * \n");
        FileSystem_fctl_RemoveFiles(SN3D_BINARY_FOLDER_PATH);
        FileSystem_fctl_CopyFile(FIRMWARE_BINARY_PATH, SN3D_BINARY_PATH);

        printf("\n * Update Completed. * \n");
    }
    else
    {
        printf("\n => Current firmware version is newest version. <= \n\n\n");
    }

    return isNeedUpdate;
}
