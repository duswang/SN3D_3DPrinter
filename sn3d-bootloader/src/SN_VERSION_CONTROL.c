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


static unsigned char* sGenerateHash(char* path, char* salt);
static void print_md5_sum(const unsigned char* md, const unsigned char* path);

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
bool SN_VERSION_CONTROL_VersionUpdate(const char* fileName)
{
    unsigned char* hash = NULL;
    char path[MAX_PATH_LENGTH];

    hash = sGenerateHash(FIRMWARE_BINARY_PATH, "test");
    if(hash == NULL)
    {

    }
    else
    {
        free((void*)hash);
    }

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

static void print_md5_sum(const unsigned char* md, const unsigned char* path)
{
    int i;

    printf("\n\n * HASH *\nHASH : ");

    for(i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
            printf("%02x", md[i]);
    }

    printf("\n\nPATH : %s\n\n", path);
}

static unsigned long get_size_by_fd(int fd)
{
    struct stat statbuf;

    if(fstat(fd, &statbuf) < 0)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "file can't open.");
    }

    return statbuf.st_size;
}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

static unsigned char* sGenerateHash(char* path, char* salt)
{
    unsigned char* hash = NULL;
    int file_descript = 0;
    unsigned long file_size = 0;
    char* file_buffer = NULL;
    char* temp_buffer = NULL;

    hash = (unsigned char*)malloc(sizeof(unsigned char) * MD5_DIGEST_LENGTH);
    if(hash == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "hash memory allocate falid.");
    }

    file_descript = open(path, O_RDONLY);
    if(file_descript < 0) exit(-1);

    file_size = get_size_by_fd(file_descript);
    printf("file size:\t%lu\n", file_size);

    file_buffer = mmap(0, file_size, PROT_READ, MAP_SHARED, file_descript, 0);

    if(salt != NULL)
    {
        MD5((unsigned char*)concat(file_buffer, salt), file_size + strlen(salt), hash);
    }
    else
    {
        MD5((unsigned char*) file_buffer, file_size, hash);
    }
    munmap(file_buffer, file_size);

    print_md5_sum(hash, (const unsigned char*)path);

    return hash;
}
