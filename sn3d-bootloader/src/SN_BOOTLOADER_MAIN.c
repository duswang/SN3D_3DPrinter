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

/* ******* UTIL :: FILE CONTROL FUNCTIONS ******* */

/* ******* MAIN FUNCTIONS ******* */

const char* firmwareVersionCheck(void)
{
    struct dirent **nameList;
    int numberOfnameList = 0;
    int i = 0;
    char* firmwareFilename = NULL;

    char* nameBuffer = NULL;

    numberOfnameList = scandir(USB_PATH, &nameList, 0, alphasort);
    if(numberOfnameList < 0)
    {
        perror("scandir");
    }
    else
    {
        for(i = 0; i < numberOfnameList; i++)
        {
            if(strstr(nameList[i]->d_name, SN3D_FW_STR))
            {
                firmwareFilename = (char*)malloc(sizeof(MAX_PATH_LENGTH * sizeof(char)));
                if(firmwareFilename == NULL)
                {
                    exit(-1);
                }

                nameBuffer = FileSystem_fctl_Extarct_FileName(nameList[i]->d_name);

                strcpy(firmwareFilename, nameList[i]->d_name);
                printf("SCAN FILE NAME [ %d ]: %s \n", i, firmwareFilename);

                free(nameBuffer);
                nameBuffer = NULL;
            }

            free(nameList[i]);
            nameList[i] = NULL;
        }
        free(nameList);
        nameList = NULL;
    }

    return firmwareFilename;
}

void machineInfoCheck(void)
{

}

/**
*  @brief main
*/
int main(void)
{
    printf("FIRMWARE USB PATH :: %s\n", USB_PATH);

    firmwareVersionCheck();
}


/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
