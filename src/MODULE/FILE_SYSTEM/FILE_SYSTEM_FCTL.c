/**
 * @file FILE_SYSTEM_FCTL.c
 * @author Bato
 * @date 7 Nov 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */

#include "SN_API.h"
#include "SN_MODULE_FILE_SYSTEM.h"

#include "FILE_SYSTEM_FCTL.h"

/* ******* STATIC DEFINE ******* */

/* ******* GLOBAL VARIABLE ******* */

/* ******* STATIC FUNCTIONS ******* */

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

SN_STATUS FileSystem_fctl_CopyFile(const char* srcPath, const char* desPath)
{
    FILE* src = fopen(srcPath, "rb");
    FILE* des = fopen(desPath, "wb");
    char buf[1000];
    int readCnt = 0;

    if(src == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_INVALID_PARAM, "src path invalid.");
    }

    if(des == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_INVALID_PARAM, "des path invalid.");
    }

    while(true)
    {

        readCnt = fread((void*)buf, 1, sizeof(buf), src);

        if(readCnt < sizeof(buf))
        {
            if(feof(src) != 0)
            {
                fwrite((void*)buf, 1, readCnt, des);
                SN_SYS_Log("Module => File System => MAKE TEMP FILE.");
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

    return SN_STATUS_OK;
}

SN_STATUS FileSystem_fctl_RemoveFiles(const char* folderPath)
{
    DIR *d = opendir(folderPath);
    size_t path_len = strlen(folderPath);
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

                snprintf(buf, len,"%s/%s", folderPath, p->d_name);

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
            }
            r = r2;
        }
        closedir(d);
    }
    else
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_INVALID_PARAM, "Temp File Folder Open Failed.");
    }

    if(!r)
    {
        SN_SYS_Log("Module => File System => REMOVE TEMP FILE.");
    }

    return SN_STATUS_OK;
}

SN_STATUS FileSystem_fctl_MakeDirectory(const char* dir)
{
    if(mkdir(dir, 0755) < 0)
    {
        if (errno != EEXIST)
        {
            SN_SYS_ERROR_CHECK(SN_STATUS_INVALID_PARAM, "Temp File Path Invalid.");
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
        SN_SYS_ERROR_CHECK(SN_STATUS_INVALID_PARAM, "Temp File Folder Open Failed.");
    }

    for (i = 0; i < zip_get_num_entries(za, 0); i++)
    {
        if (zip_stat_index(za, i, 0, &sb) == 0)
        {
            len = strlen(sb.name);

            if (sb.name[len - 1] == '/')
            {
                FileSystem_fctl_MakeDirectory(sb.name);
            }
            else
            {
                zf = zip_fopen_index(za, i, 0);

                if (!zf) {
                    SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "Temp File Extract Failed.");
                }

                len = strlen(desPath) + strlen(sb.name);

                snprintf(fullFilePath, len + 2, "%s/%s", desPath, sb.name);

                fd = open(fullFilePath, O_RDWR | O_TRUNC | O_CREAT, 0644);

                if (fd < 0) {
                    SN_SYS_ERROR_CHECK(SN_STATUS_INVALID_PARAM, "Temp File Path Invalid.");
                }

                sum = 0;

                while (sum != sb.size)
                {
                    len = zip_fread(zf, buf, 1000);
                    if (len < 0)
                    {
                        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "Temp File Extract Failed.");
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
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "Temp File Extract Failed.");
    }

    SN_SYS_Log("Module => File System => EXTRACT TEMP FILE.");

    return retStatus;
}


uint32_t FileSystem_CountFileNumWithExtetion(const char* srcPath, const char* fileExtention)
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

            if(strstr(p->d_name, fileExtention) != NULL)
            {
                slice++;
            }
        }
        closedir(d);
    }

    else
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_INVALID_PARAM, "Temp File Path Invalid.");
    }

    if(!r)
    {
        printf("Module => File System => Slice File Number : [ %04d ]\n", slice); fflush(stdout);
    }

    return slice;
}

const char* FileSystem_fctl_ExtractFileExtention(const char *filename)
{
    const char *dot = strrchr(filename, '.');

    if(filename[0] == '.') return "";
    if(!dot || dot == filename) return "";
    return dot + 1;
}

const char* FileSystem_fctl_Extarct_FileName(const char *filename)
{
    char *retStr;
    char *lastDot;

    if (filename == NULL)
    {
         return NULL;
    }

    if ((retStr = malloc (strlen (filename) + 1)) == NULL)
    {
        return NULL;
    }

    strcpy (retStr, filename);

    lastDot = strrchr (retStr, '.');
    if (lastDot != NULL)
        *lastDot = '\0';

    return retStr;
}

