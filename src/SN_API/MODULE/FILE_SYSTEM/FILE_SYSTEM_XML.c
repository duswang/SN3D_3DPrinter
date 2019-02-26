/**
 * @file SN_MODULE_FILE_SYSTEM_XML.c
 * @author Bato
 * @date 7 Nov 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */

#ifdef __APPLE__
#include <libxml2/libxml/xmlreader.h>
#include <libxml2/libxml/xmlmemory.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>
#endif

#ifdef linux
#include <libxml/xmlreader.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#endif

#include "SN_API.h"
#include "SN_MODULE_FILE_SYSTEM.h"

#include "FILE_SYSTEM_XML.h"
#include "FILE_SYSTEM_FCTL.h"

/* ******* STATIC DEFINE ******* */
/** @name Z config *////@{
#define Z_DELAY_OFFSET (2000) // 2 Second.

/** @def SPEED_MM_MIN_TO_MM_MSEC(speed_mm_min)
 *  @brief mm/m to mm/s
 *
 *  @param speed_mm_min
 *
 *  @return mm/s
 */
#define SPEED_MM_MIN_TO_MM_MSEC(speed_mm_min) \
        ((speed_mm_min) / (60 * 1000))

/** @def Z_DELAY_MSEC_CAL(distnace, speed)
 *  @brief calculate z delay
 *
 *  @param distance
 *  @param speed
 *
 *  @return z_delay (msec)
 */
#define Z_DELAY_MSEC_CAL(distnace, speed) \
    ((((distnace) * 2) / (SPEED_MM_MIN_TO_MM_MSEC(speed))) + Z_DELAY_OFFSET)
///@}
///
/* ******* GLOBAL VARIABLE ******* */

/* ******* STATIC FUNCTIONS ******* */
/* Target XML */
static SN_STATUS sParseXML_targetFile(printTarget_t* printTarget, xmlDocPtr doc, xmlNodePtr cur);

/* MAHCINEINFO XML */
static SN_STATUS sParseXML_machineInfoFile(machineInfo_t* machineInfo, xmlDocPtr doc, xmlNodePtr cur);

/* DEVICE XML */
static SN_STATUS sParseXML_deviceInfoFile(deviceInfo_t* deviceInfo, xmlDocPtr doc, xmlNodePtr cur);

static SN_STATUS sSetXML_deviceInfoFile(const deviceInfo_t deviceInfo, xmlDocPtr doc, xmlNodePtr cur);

/* OPTION XML */
static SN_STATUS sParseXML_optionFile(printOption_t* printOption, xmlDocPtr doc, xmlNodePtr cur);

/* VERSION XML */
static SN_STATUS sParseXML_VersionFile(versionInfo_t* versionInfo, xmlDocPtr doc, xmlNodePtr cur);

/* Hash File */
static unsigned long sGet_size_by_fd(int fd);

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

unsigned char* FileSysetm_MD5_Hash_WithFile(char* path, char* salt)
{
    unsigned char* hash = NULL;

    int file_descript = 0;
    unsigned long file_size = 0;
    char* file_buffer = NULL;
    MD5_CTX mdContext;

    hash = (unsigned char*)malloc(sizeof(unsigned char) * MD5_DIGEST_LENGTH);
    if(hash == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_OK, "hash memory allocate falid.");
    }

    file_descript = open(path, O_RDONLY);
    if(file_descript < 0) exit(-1);

    file_size = sGet_size_by_fd(file_descript);
    printf("file size:\t%lu\n", file_size);

    file_buffer = mmap(0, file_size, PROT_READ, MAP_SHARED, file_descript, 0);

    MD5_Init (&mdContext);

    MD5_Update(&mdContext, (unsigned char*) file_buffer, file_size);

    if(salt != NULL)
    {
        MD5_Update(&mdContext, (unsigned char*)salt, strlen(salt));
    }

    munmap(file_buffer, file_size);
    MD5_Final(hash, &mdContext);

    munmap(file_buffer, file_size);

    return hash;
}

unsigned char* FileSystem_MD5_HashToString(unsigned char* hash)
{
    int i = 0;
    unsigned char* hashStr = NULL;

    if(hash == NULL)
    {
        return NULL;
    }

    hashStr = (unsigned char*)malloc(sizeof(unsigned char) * ((MD5_DIGEST_LENGTH * 2) + 1));
    if(hashStr == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_OK, "hash memory allocate falid.");
    }

    for(i = 0; i < MD5_DIGEST_LENGTH; ++i)
    {
        sprintf((char*)&hashStr[i * 2], "%02x", (unsigned int)hash[i]);
    }

    printf("\n\n HASH STR :: %s \n\n", hashStr);

    return hashStr;
}

printTarget_t* FileSystem_targetXMLLoad(const char *srcPath)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    printTarget_t* printTarget = NULL;

    const char         *docname;
    xmlDocPtr           doc;
    xmlNodePtr          cur;


    docname = srcPath;
    doc = xmlParseFile(docname);
    if(doc == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "XMl File Open Failed.");
    }

    cur = xmlDocGetRootElement(doc);
    if(cur == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "XMl File Open Failed.");
    }

    cur = cur->xmlChildrenNode;

    while (cur != NULL)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"GCode")))
        {
            retStatus = sParseXML_targetFile(printTarget, doc, cur);
            SN_SYS_ERROR_StatusCheck(retStatus, "target XML File Load Failed.");
        }
        cur = cur->next;
    }

    xmlFreeDoc(doc);

    return printTarget;
}

printOption_t* FileSystem_optionXMLLoad(const char *srcPath)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    printOption_t* printOption = NULL;

    const char         *docname;
    xmlDocPtr           doc;
    xmlNodePtr          cur;

    docname = srcPath;
    doc = xmlParseFile(docname);
    if(doc == NULL)
    {
        retStatus = SN_STATUS_NOT_INITIALIZED;
    }

    cur = xmlDocGetRootElement(doc);
    if(cur == NULL)
    {
        retStatus = SN_STATUS_NOT_INITIALIZED;
    }

    if(retStatus != SN_STATUS_OK)
    {
        retStatus = FileSystem_fctl_CreateDircetoryTree(OPTION_FOLDER_PATH);
        SN_SYS_ERROR_StatusCheck(retStatus, "Directory Create Failed.");

        FileSystem_fctl_CopyFile(DEFAULT_OPTION_FILE_PATH, srcPath);

        return FileSystem_optionXMLLoad(srcPath);
    }

    if (xmlStrcmp(cur->name, (const xmlChar *)"option"))
    {
        cur = cur->xmlChildrenNode;
        xmlFreeDoc(doc);

        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_OK, "XMl File is Invlaid.");

        return NULL;
    }

    printOption = (printOption_t *)malloc(sizeof(printOption_t));
    if(printOption == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "printOption memory allocate failed.");
    }

    retStatus = sParseXML_optionFile(printOption, doc, cur);
    SN_SYS_ERROR_StatusCheck(retStatus, "option XML File Load Failed.");

    xmlFreeDoc(doc);

    return printOption;
}

SN_STATUS FileSystem_deviceInfoXMLUpdate(const char *srcPath, const deviceInfo_t deviceInfo)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    const char         *docname;
    xmlDocPtr           doc;
    xmlNodePtr          cur;

    FILE* xmlFile = NULL;


    docname = srcPath;
    doc = xmlParseFile(docname);
    if(doc == NULL)
    {
        retStatus = SN_STATUS_NOT_INITIALIZED;
    }

    cur = xmlDocGetRootElement(doc);
    if(cur == NULL)
    {
        retStatus = SN_STATUS_NOT_INITIALIZED;
    }

    if(retStatus != SN_STATUS_OK)
    {
        retStatus = FileSystem_fctl_CreateDircetoryTree(DEVICE_FOLDER_PATH);
        SN_SYS_ERROR_StatusCheck(retStatus, "Directory Create Failed.");

        FileSystem_fctl_CopyFile(DEFAULT_DEVICE_FILE_PATH, srcPath);

        return FileSystem_deviceInfoXMLUpdate(srcPath, deviceInfo);
    }

    if (xmlStrcmp(cur->name, (const xmlChar *)"device"))
    {
        cur = cur->xmlChildrenNode;
        xmlFreeDoc(doc);

        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_OK, "XMl File is Invlaid.");

        return SN_STATUS_NOT_OK;
    }

    retStatus = sSetXML_deviceInfoFile(deviceInfo, doc, cur);
    SN_SYS_ERROR_StatusCheck(retStatus, "device Info XML File Load Failed.");

    xmlFile = fopen(srcPath, "wb");
    if(xmlFile == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_INVALID_PARAM, "src path invalid.");
    }

    xmlDocDump(xmlFile, doc);

    fclose(xmlFile);

    xmlFreeDoc(doc);

    return retStatus;
}

deviceInfo_t* FileSystem_deviceInfoXMLLoad(const char *srcPath)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    deviceInfo_t* deviceInfo = NULL;

    const char         *docname;
    xmlDocPtr           doc;
    xmlNodePtr          cur;

    docname = srcPath;
    doc = xmlParseFile(docname);
    if(doc == NULL)
    {
        retStatus = SN_STATUS_NOT_INITIALIZED;
    }

    cur = xmlDocGetRootElement(doc);
    if(cur == NULL)
    {
        retStatus = SN_STATUS_NOT_INITIALIZED;
    }

    if(retStatus != SN_STATUS_OK)
    {
        retStatus = FileSystem_fctl_CreateDircetoryTree(DEVICE_FOLDER_PATH);
        SN_SYS_ERROR_StatusCheck(retStatus, "Directory Create Failed.");

        FileSystem_fctl_CopyFile(DEFAULT_DEVICE_FILE_PATH, srcPath);

        return FileSystem_deviceInfoXMLLoad(srcPath);
    }

    if (xmlStrcmp(cur->name, (const xmlChar *)"device"))
    {
        cur = cur->xmlChildrenNode;
        xmlFreeDoc(doc);

        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_OK, "XMl File is Invlaid.");

        return NULL;
    }

    deviceInfo = (deviceInfo_t *)malloc(sizeof(deviceInfo_t));
    if(deviceInfo == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "printOption memory allocate failed.");
    }

    retStatus = sParseXML_deviceInfoFile(deviceInfo, doc, cur);
    SN_SYS_ERROR_StatusCheck(retStatus, "device Info XML File Load Failed.");

    xmlFreeDoc(doc);

    return deviceInfo;
}

machineInfo_t* FileSystem_machineInfoXMLLoad(const char *srcPath)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    machineInfo_t* mahcineInfo = NULL;

    const char         *docname;
    xmlDocPtr           doc;
    xmlNodePtr          cur;

    docname = srcPath;
    doc = xmlParseFile(docname);
    if(doc == NULL)
    {
        retStatus = SN_STATUS_NOT_INITIALIZED;
    }

    cur = xmlDocGetRootElement(doc);
    if(cur == NULL)
    {
        retStatus = SN_STATUS_NOT_INITIALIZED;
    }

    if(retStatus != SN_STATUS_OK)
    {
        retStatus = FileSystem_fctl_CreateDircetoryTree(MACHINE_FOLDER_PATH);
        SN_SYS_ERROR_StatusCheck(retStatus, "Directory Create Failed.");

        FileSystem_fctl_CopyFile(DEFAULT_MACHINE_FILE_PATH, srcPath);

        return FileSystem_machineInfoXMLLoad(srcPath);
    }

    if(xmlStrcmp(cur->name, (const xmlChar *)"machine"))
    {
        cur = cur->xmlChildrenNode;
        xmlFreeDoc(doc);

        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_OK, "XMl File is Invlaid.");

        return NULL;
    }

    mahcineInfo = (machineInfo_t *)malloc(sizeof(machineInfo_t));
    if(mahcineInfo == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "printOption memory allocate failed.");
    }

    retStatus = sParseXML_machineInfoFile(mahcineInfo, doc, cur);
    SN_SYS_ERROR_StatusCheck(retStatus, "machine Info XML File Load Failed.");

    xmlFreeDoc(doc);

    return mahcineInfo;
}

versionInfo_t* FileSystem_versionInfoXMLLoad(const char *srcPath)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    versionInfo_t* versionInfo = NULL;

    const char         *docname;
    xmlDocPtr           doc;
    xmlNodePtr          cur;

    docname = srcPath;
    doc = xmlParseFile(docname);
    if(doc == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "XMl File Open Failed.");
    }

    cur = xmlDocGetRootElement(doc);
    if(cur == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "XMl File Open Failed.");
    }

    if (xmlStrcmp(cur->name, (const xmlChar *)"version"))
    {
        cur = cur->xmlChildrenNode;
        xmlFreeDoc(doc);

        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_OK, "XMl File is Invlaid.");

        return NULL;
    }

    versionInfo = (versionInfo_t *)malloc(sizeof(versionInfo_t));
    if(versionInfo == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "printOption memory allocate failed.");
    }

    retStatus = sParseXML_VersionFile(versionInfo, doc, cur);
    SN_SYS_ERROR_StatusCheck(retStatus, "machine Info XML File Load Failed.");

    xmlFreeDoc(doc);

    return versionInfo;
}

static SN_STATUS sParseXML_deviceInfoFile(deviceInfo_t* deviceInfo, xmlDocPtr doc, xmlNodePtr cur)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    xmlChar *key;

    cur = cur->xmlChildrenNode;

    while(cur != NULL)
    {
        if((!xmlStrcmp(cur->name, (const xmlChar *)"language")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            strcpy(deviceInfo->language, (const char *)key);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"totalTime")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%d", &deviceInfo->totalTime);
            xmlFree(key);
        }
        cur = cur->next;
    }

    return retStatus;
}

static SN_STATUS sSetXML_deviceInfoFile(const deviceInfo_t deviceInfo, xmlDocPtr doc, xmlNodePtr cur)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    char buffer[MAX_PATH_LENGTH];

    cur = cur->xmlChildrenNode;

    while(cur != NULL)
    {
        if((!xmlStrcmp(cur->name, (const xmlChar *)"language")))
        {
            xmlNodeSetContent(cur, (const xmlChar*)deviceInfo.language);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"totalTime")))
        {
            sprintf(buffer, "%d", deviceInfo.totalTime);
            xmlNodeSetContent(cur, (const xmlChar*)buffer);
        }
        cur = cur->next;
    }

    return retStatus;
}

static SN_STATUS sParseXML_machineInfoFile(machineInfo_t* machineInfo, xmlDocPtr doc, xmlNodePtr cur)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    xmlChar *key;

    cur = cur->xmlChildrenNode;

    while(cur != NULL)
    {
        if((!xmlStrcmp(cur->name, (const xmlChar *)"machine_name")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            strcpy(machineInfo->name, (const char *)key);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"resolution_h")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%d", &machineInfo->screenHeight);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"resolution_w")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%d", &machineInfo->screenWidth);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"displayScreenSize")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%s", machineInfo->displayScreenSize);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"touchScreenSize")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%s", machineInfo->touchScreenSize);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"machine_z_limit")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%d", &machineInfo->machineHeight);
            xmlFree(key);
        }
        cur = cur->next;
    }

    return retStatus;
}

static SN_STATUS sParseXML_optionFile(printOption_t* printOption, xmlDocPtr doc, xmlNodePtr cur)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    xmlChar *key;

    cur = cur->xmlChildrenNode;

    while(cur != NULL)
    {
        if((!xmlStrcmp(cur->name, (const xmlChar *)"option_name")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            strcpy(printOption->name, (const char *)key);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"layerThickness")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%f", &printOption->layerThickness);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"bottomLayerExposureTime")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%d", &printOption->bottomLayerExposureTime);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"bottomLayerNumber")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%d", &printOption->bottomLayerNumber);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"bottomLiftFeedRate")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%f", &printOption->bottomLiftFeedRate);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"layerExposureTime")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%d", &printOption->layerExposureTime);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"liftDistance")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%d", &printOption->liftDistance);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"liftFeedRate")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%f", &printOption->liftFeedRate);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"bright")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%d", &printOption->bright);
            xmlFree(key);
        }
        cur = cur->next;
    }

    /* Calculate Time */
    printOption->liftTime = Z_DELAY_MSEC_CAL( \
                            printOption->liftDistance, \
                            printOption->liftFeedRate);

    return retStatus;
}

static SN_STATUS sParseXML_VersionFile(versionInfo_t* versionInfo, xmlDocPtr doc, xmlNodePtr cur)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    xmlChar *key;

    cur = cur->xmlChildrenNode;

    while(cur != NULL)
    {
        if((!xmlStrcmp(cur->name, (const xmlChar *)"project")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            strcpy(versionInfo->name, (const char *)key);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"releaseNumber")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%d", &versionInfo->releaseNumber);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"majorNumber")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%d", &versionInfo->majorNumber);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"minorNumber")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%d", &versionInfo->minorNumber);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"timestamp")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            strcpy(versionInfo->timestamp, (const char *)key);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"binraryName")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            strcpy(versionInfo->binaryName, (const char *)key);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"hash")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            strcpy(versionInfo->hash, (const char *)key);
            xmlFree(key);
        }
        cur = cur->next;
    }

    return retStatus;
}

static SN_STATUS sParseXML_targetFile(printTarget_t* printTarget, xmlDocPtr doc, xmlNodePtr cur)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    return retStatus;
}

static unsigned long sGet_size_by_fd(int fd)
{
    struct stat statbuf;

    if(fstat(fd, &statbuf) < 0)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_OK, "file can't open.");
    }

    return statbuf.st_size;
}
