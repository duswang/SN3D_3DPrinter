/**
 * @file FILE_SYSTEM_FCTL.c
 * @author Bato
 * @date 7 Nov 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */

#ifdef __APPLE__

#include <zip.h>
#endif

#ifdef linux

#include <zip.h>
#endif

#include "SN_BOOTLOADER.h"

#include "FILE_SYSTEM_FCTL.h"

/* ******* STATIC DEFINE ******* */

/* ******* GLOBAL VARIABLE ******* */

/* ******* STATIC FUNCTIONS ******* */
static int do_mkdir(const char *path, mode_t mode);

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
SN_STATUS FileSystem_fctl_CreateDircetoryTree(const char* path)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    int   status = 0;
    char* pathPointer = NULL;
    char *directoryPointer = NULL;
    char  pathBuffer[MAX_PATH_LENGTH];

    strcpy(pathBuffer, path);
    printf("DIRECTORY TREE :: %s \n\n", pathBuffer);

    pathPointer = pathBuffer;
    while (status == 0 && (directoryPointer = strchr(pathPointer, '/')) != 0)
    {
        if (directoryPointer != pathPointer)
        {
            *directoryPointer = '\0';
            status = do_mkdir(pathBuffer, 0777);
            printf("CREATE DIRECTORY :: %s \n\n", pathBuffer);
            *directoryPointer = '/';
        }
        pathPointer = directoryPointer + 1;
    }

    if(status == 0)
    {
        do_mkdir(path, 0777);
        printf("CREATE DIRECTORY :: %s \n\n", pathBuffer);
    }
    else
    {
        retStatus = SN_STATUS_NOT_OK;
    }

    return retStatus;
}


SN_STATUS FileSystem_fctl_CopyFile(const char* srcPath, const char* desPath)
{
    FILE* src = fopen(srcPath, "rb");
    FILE* des = fopen(desPath, "wb");
    char buf[1000];
    int readCnt = 0;

    if(src == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_INVALID_PARAM, "src path invalid.");
    }

    if(des == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_INVALID_PARAM, "des path invalid.");
    }

    while(true)
    {

        readCnt = fread((void*)buf, 1, sizeof(buf), src);

        if(readCnt < sizeof(buf))
        {
            if(feof(src) != 0)
            {
                fwrite((void*)buf, 1, readCnt, des);
                SN_SYS_ERROR_SystemLog("Module => File System => COPY FILE.");
                break;
            }
            else
            {
                break;
            }
        }

        fwrite((void*)buf,1, sizeof(buf), des);
    }

    fclose(src);
    fclose(des);

    chmod(desPath, 0777);

    return SN_STATUS_OK;
}

SN_STATUS FileSystem_fctl_RemoveFiles(const char* dir)
{
    DIR *d = opendir(dir);
    size_t path_len = strlen(dir);
    int r = -1;

    if(d)
    {
        struct dirent *p;

        r = 0;

        while(!r &&(p=readdir(d)))
        {
            int r2 = -1;
            char *buf;
            size_t len;

            if(!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
            {
                continue;
            }

            len = path_len + strlen(p->d_name) + 2;
            buf = malloc(len);

            if(buf)
            {
                struct stat statbuf;

                snprintf(buf, len,"%s/%s", dir, p->d_name);

                if(!stat(buf, &statbuf))
                {
                    if(S_ISDIR(statbuf.st_mode))
                    {
                        r2 = FileSystem_fctl_RemoveFiles(buf);
                    }
                    else
                    {
                        r2 = unlink(buf);
                    }
                }

                free(buf);

                buf = NULL;
            }
            r = r2;
        }
        closedir(d);
    }
    else
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_INVALID_PARAM, "Folder Open Failed.");
    }

    if(!r)
    {
        SN_SYS_ERROR_SystemLog("Module => File System => REMOVE FOLDER FILES.");
    }

    return SN_STATUS_OK;
}

SN_STATUS FileSystem_fctl_MakeDirectory(const char* dir)
{
    if(mkdir(dir, 0755) < 0)
    {
        if (errno != EEXIST)
        {
            SN_SYS_ERROR_StatusCheck(SN_STATUS_INVALID_PARAM, "File Path Invalid.");
        }
    }

    return SN_STATUS_OK;
}

SN_STATUS FileSystem_fctl_ExtractFile(const char* srcPath, const char* desPath)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    struct zip *za;
    struct zip_file *zf;
    struct zip_stat sb;

    char buf[1000];

    int err;
    int i, len;
    int fd;
    long long sum;
    char fullFilePath[MAX_PATH_LENGTH];

    if ((za = zip_open(srcPath, 0, &err)) == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_INVALID_PARAM, "xFile Folder Open Failed.");
    }

    for (i = 0; i < zip_get_num_entries(za, 0); i++)
    {
        if (zip_stat_index(za, i, 0, &sb) == 0)
        {
            len = strlen(desPath) + strlen(sb.name);

            snprintf(fullFilePath, len + 2, "%s/%s", desPath, sb.name);

            if (sb.name[strlen(sb.name) - 1] == '/')
            {
                // Not Extract.
                /*
                FileSystem_fctl_MakeDirectory(fullFilePath);
                chmod(fullFilePath, 0777);

                printf("%s\n", fullFilePath);
                */
            }
            else if(sb.name[0] == '_')
            {
                // Not Extract.
            }
            else
            {
                zf = zip_fopen_index(za, i, 0);

                if (!zf) {
                    SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_OK, "File Extract Failed.");
                }
                printf("%s\n", fullFilePath);
                fd = open(fullFilePath, O_RDWR | O_TRUNC | O_CREAT, 0777);

                if (fd < 0) {
                    SN_SYS_ERROR_StatusCheck(SN_STATUS_INVALID_PARAM, "File Path Invalid.");
                }

                sum = 0;

                while (sum != sb.size)
                {
                    len = zip_fread(zf, buf, 1000);
                    if (len < 0)
                    {
                        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_OK, "File Extract Failed.");
                    }
                    write(fd, buf, len);
                    sum += len;
                }

                close(fd);
                zip_fclose(zf);
            }
        }
    }

    if (zip_close(za) == -1)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_OK, "File Extract Failed.");
    }

    SN_SYS_ERROR_SystemLog("Module => File System => EXTRACT FILE.");

    return retStatus;
}

uint32_t FileSystem_CountFileWithStr(const char* srcPath, const char* condStr)
{
    DIR *d = opendir(srcPath);
    uint32_t slice = 0;

    int r = -1;

    if(d)
    {
        struct dirent *p;

        r = 0;

        while(!r &&( p=readdir(d)))
        {
            if(!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
            {
                continue;
            }

            if(strstr(p->d_name, condStr) != NULL)
            {
                slice++;
            }
        }
        closedir(d);
    }

    else
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_INVALID_PARAM, "File Path Invalid.");
    }

    if(!r)
    {

    }

    return slice;
}

const char* FileSystem_fctl_ExtractFileExtention(const char* filename)
{
    const char *dot = strrchr(filename, '.');

    if(filename[0] == '.') return "";
    if(!dot || dot == filename) return "";
    return dot + 1;
}

char* FileSystem_fctl_Extarct_FileName(const char *filename)
{
    char *retStr;
    char *lastDot;

    if (filename == NULL)
    {
         return NULL;
    }

    if ((retStr = malloc (strlen(filename) + 1)) == NULL)
    {
        return NULL;
    }

    strcpy (retStr, filename);

    lastDot = strrchr (retStr, '.');
    if (lastDot != NULL)
        *lastDot = '\0';

    return retStr;
}

static int do_mkdir(const char *path, mode_t mode)
{
    struct stat            st;
    int             status = 0;

    if (stat(path, &st) != 0)
    {
        /* Directory does not exist. EEXIST for race condition */
        if (mkdir(path, mode) != 0 && errno != EEXIST)
            status = -1;
    }
    else if (!S_ISDIR(st.st_mode))
    {
        errno = ENOTDIR;
        status = -1;
    }

    return(status);
}
