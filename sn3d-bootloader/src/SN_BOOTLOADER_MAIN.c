/**
 * @file SN_BOOTLOADER_MAIN.c
 * @author Bato
 * @date 18 Sep 2018
 * @brief
 *
 * @see https://bitbucket.org/xengiennering/sn3d-project/src/master/
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */

#include "SN_BOOTLOADER.h"


/* ******* STATIC DEFINE ******* */

/* ******* GLOBAL VARIABLE ******* */

/* ******* STATIC FUNCTIONS ******* */
const char* sBootloader_FW_FileSearching(void);
static void sBootloader_Terminate(bool isNeedReboot);
static bool sBootloader_Start(const char* firmwareFile);

/**
*  @brief main
*/
int main(void)
{
    const char* firmwareFile = NULL;

    bool isNeedReboot = false;

    firmwareFile = sBootloader_FW_FileSearching();

    if(firmwareFile != NULL)
    {
        isNeedReboot = sBootloader_Start(firmwareFile);

        sBootloader_Terminate(isNeedReboot);
    }
    else
    {
        // Start SN3D.service.
    }

    return 0;
}

const char* sBootloader_FW_FileSearching(void)
{
    struct dirent **nameList;
    int numberOfnameList = 0;
    int i = 0;

    bool isFirmwareFileExit = false;

    char nameBuffer[MAX_PATH_LENGTH];
    char path[MAX_PATH_LENGTH];

    char* firmwareFilename = NULL;

    numberOfnameList = scandir(USB_FOLDER_PATH, &nameList, 0, alphasort);
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
              (strstr(nameBuffer, SN3D_FW_EXTENTION))  && \
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

            if(strstr(nameBuffer, SN3D_OPTION_STR) && !strstr(nameBuffer, HIDDEN_FILE_STR) && strstr(nameBuffer, SN3D_OPTION_EXTENTION))
            {
                printf("Option   File Name : [ %s ]\n", nameBuffer);

                sprintf(path, "%s/%s", USB_FOLDER_PATH, nameBuffer);

                printf("Option   File Path : [ %s ]\n", path);

                FileSystem_fctl_CreateDircetoryTree(SN3D_OPTION_FOLDER_PATH);
                FileSystem_fctl_RemoveFiles(SN3D_OPTION_FOLDER_PATH);
                FileSystem_fctl_ExtractFile(path, SN3D_OPTION_FOLDER_PATH);
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
static bool sBootloader_Start(const char* firmwareFile)
{
    bool isNeedReboot = false;


    isNeedReboot = SN_VERSION_CONTROL_VersionUpdate(firmwareFile);
    isNeedReboot = SN_VERSION_CONTROL_MachineInfoUpdate(firmwareFile);

    return isNeedReboot;
}

static void sBootloader_Terminate(bool isNeedReboot)
{
    FileSystem_fctl_RemoveFiles(TEMP_FIRMWARE_FOLDER_PATH);

    if(isNeedReboot == true)
    {
        SN_SYS_Log("System Reboot. \n\n");

        sync();
        reboot(RB_AUTOBOOT);
    }
}
