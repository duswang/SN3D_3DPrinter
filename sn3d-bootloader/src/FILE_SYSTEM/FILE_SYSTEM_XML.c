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
#include <libxml/xmlwriter.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/encoding.h>
#endif

#include "SN_BOOTLOADER.h"

#include "FILE_SYSTEM_XML.h"

#if LITTLE_ENDIAN
#    define Swap32LE(x) (x)
#    define Swap32BE(x) ((((x) & 0xff000000) >> 24) | (((x) & 0xff0000) >> 8) | (((x) & 0xff00) << 8) | (((x) & 0xff) << 24))
     static float sReverseFloat( const float inFloat );
#else
#    define Swap32LE(x) ((((x) & 0xff000000) >> 24) | (((x) & 0xff0000) >> 8) | (((x) & 0xff00) << 8) | (((x) & 0xff) << 24))
#    define Swap32BE(x) (x)
#endif

/* ******* STATIC DEFINE ******* */

/* ******* GLOBAL VARIABLE ******* */

/* ******* STATIC FUNCTIONS ******* */

/* MAHCINEINFO XML */
static SN_STATUS sParseXML_machineInfoFile(machineInfo_t* machineInfo, xmlDocPtr doc, xmlNodePtr cur);

static SN_STATUS sParseXML_VersionFile(versionInfo_t* versionInfo, xmlDocPtr doc, xmlNodePtr cur);

/* Hash File */
static unsigned long sGet_size_by_fd(int fd);

/* Binary Option Config */
static printOption_t* sReadOptionBinary(const char* srcPath);
static SN_STATUS      sCreateOptionFile(const char* srcPath, const printOption_t optionParam);

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


SN_STATUS FileSystem_optionBinaryToXML(const char *srcPath, const char*desPath)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    printOption_t* binaryOptionConfig = NULL;


    if(srcPath == NULL || desPath == NULL)
    {
        return SN_STATUS_INVALID_PARAM;
    }

    binaryOptionConfig = sReadOptionBinary(srcPath);
    if(binaryOptionConfig == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "Read Binary Option Config Failed.");
    }

    retStatus = sCreateOptionFile(desPath , *binaryOptionConfig);

    SN_SYS_ERROR_StatusCheck(retStatus, "Create SN3d Option Config File Faield.");


    return retStatus;
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
        retStatus = SN_STATUS_NOT_INITIALIZED;
    }

    cur = xmlDocGetRootElement(doc);
    if(cur == NULL)
    {
        retStatus = SN_STATUS_NOT_INITIALIZED;
    }

    if(retStatus != SN_STATUS_OK)
    {
        retStatus = FileSystem_fctl_CreateDircetoryTree(FIRMWARE_FOLDER_PATH);
        SN_SYS_ERROR_StatusCheck(retStatus, "Directory Create Failed.");

        FileSystem_fctl_CopyFile(DEFAULT_VERSION_PATH, srcPath);

        return FileSystem_versionInfoXMLLoad(srcPath);
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
        retStatus = FileSystem_fctl_CreateDircetoryTree(FIRMWARE_FOLDER_PATH);
        SN_SYS_ERROR_StatusCheck(retStatus, "Directory Create Failed.");

        FileSystem_fctl_CopyFile(DEFAULT_MACHINE_PATH, srcPath);

        return FileSystem_machineInfoXMLLoad(srcPath);
    }


    if (xmlStrcmp(cur->name, (const xmlChar *)"machine"))
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
        if((!xmlStrcmp(cur->name, (const xmlChar *)"binaryName")))
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

static SN_STATUS sCreateOptionFile(const char* srcPath, const printOption_t optionParam)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    xmlTextWriterPtr writer = NULL;
    char buffer[MAX_FILENAME_LENGTH];

    if(srcPath == NULL)
    {
        return SN_STATUS_INVALID_PARAM;
    }

    writer = xmlNewTextWriterFilename(srcPath, 0755);
    if(writer == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_OK, "Option Config XML Write initialized failed.");
    }

    xmlTextWriterStartDocument(writer, NULL, "UTF-8", NULL);
    xmlTextWriterStartElement(writer, (const xmlChar *)"option");

    sprintf(buffer, "%s",optionParam.name);
    xmlTextWriterWriteElement(writer, (const xmlChar *)"option_name",                (const xmlChar *)buffer); xmlTextWriterEndElement(writer);
    sprintf(buffer, "%f",optionParam.layerThickness);
    xmlTextWriterWriteElement(writer, (const xmlChar *)"layerThickness",             (const xmlChar *)buffer); xmlTextWriterEndElement(writer);

    sprintf(buffer, "%d",optionParam.bottomLayerExposureTime);
    xmlTextWriterWriteElement(writer, (const xmlChar *)"bottomLayerExposureTime",   (const xmlChar *)buffer); xmlTextWriterEndElement(writer);
    sprintf(buffer, "%d",optionParam.bottomLayerNumber);
    xmlTextWriterWriteElement(writer, (const xmlChar *)"bottomLayerNumber",          (const xmlChar *)buffer); xmlTextWriterEndElement(writer);

    sprintf(buffer, "%f",optionParam.bottomLiftFeedRate);
    xmlTextWriterWriteElement(writer, (const xmlChar *)"bottomLiftFeedRate",         (const xmlChar *)buffer); xmlTextWriterEndElement(writer);
    sprintf(buffer, "%d",optionParam.layerExposureTime);
    xmlTextWriterWriteElement(writer, (const xmlChar *)"layerExposureTime",          (const xmlChar *)buffer); xmlTextWriterEndElement(writer);
    sprintf(buffer, "%d",optionParam.liftDistance);
    xmlTextWriterWriteElement(writer, (const xmlChar *)"liftDistance",               (const xmlChar *)buffer); xmlTextWriterEndElement(writer);
    sprintf(buffer, "%f",optionParam.liftFeedRate);
    xmlTextWriterWriteElement(writer, (const xmlChar *)"liftFeedRate",               (const xmlChar *)buffer); xmlTextWriterEndElement(writer);

    sprintf(buffer, "%d",optionParam.bright);
    xmlTextWriterWriteElement(writer, (const xmlChar *)"bright",                     (const xmlChar *)buffer); xmlTextWriterEndElement(writer);

    xmlTextWriterEndElement(writer);
    xmlTextWriterEndDocument(writer);
    xmlFreeTextWriter(writer);

    return retStatus;
}

static printOption_t* sReadOptionBinary(const char* srcPath)
{
    printOption_t* binaryOption = NULL;
    FILE *binaryFile = NULL;

    binaryFile = fopen(srcPath,"rb");
    if(binaryFile == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_OK, "Option Binary File Open Failed.");
    }

    binaryOption = (printOption_t*)malloc(sizeof(printOption_t));
    if(binaryOption == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_OK, "Memory Allocate Failed.");
    }

    fread(&binaryOption->name, sizeof(uint32_t), 2, binaryFile);

    fread(&binaryOption->layerThickness, sizeof(uint32_t), 1, binaryFile);
    binaryOption->layerThickness = sReverseFloat(binaryOption->layerThickness);

    fread(&binaryOption->bottomLayerExposureTime, sizeof(uint32_t), 1, binaryFile);
    binaryOption->bottomLayerExposureTime = Swap32BE(binaryOption->bottomLayerExposureTime);

    fread(&binaryOption->bottomLayerNumber, sizeof(uint32_t), 1, binaryFile);
    binaryOption->bottomLayerNumber = Swap32BE(binaryOption->bottomLayerNumber);

    fread(&binaryOption->bottomLiftFeedRate, sizeof(uint32_t), 1, binaryFile);
    binaryOption->bottomLiftFeedRate = sReverseFloat(binaryOption->bottomLiftFeedRate);

    fread(&binaryOption->layerExposureTime, sizeof(uint32_t), 1, binaryFile);
    binaryOption->layerExposureTime = Swap32BE(binaryOption->layerExposureTime);

    fread(&binaryOption->liftFeedRate, sizeof(uint32_t), 1, binaryFile);
    binaryOption->liftFeedRate = sReverseFloat(binaryOption->liftFeedRate);

    fread(&binaryOption->liftDistance, sizeof(uint32_t), 1, binaryFile);
    binaryOption->liftDistance = Swap32BE(binaryOption->liftDistance);

    fread(&binaryOption->bright, sizeof(uint32_t), 1, binaryFile);
    binaryOption->bright = Swap32BE(binaryOption->bright);

    fclose(binaryFile);

    printf("\n\n%s %f %d %d %f %d %f %d %d\n\n", binaryOption->name, \
                                   binaryOption->layerThickness, \
                                   binaryOption->bottomLayerExposureTime, \
                                   binaryOption->bottomLayerNumber, \
                                   binaryOption->bottomLiftFeedRate, \
                                   binaryOption->layerExposureTime, \
                                   binaryOption->liftFeedRate, \
                                   binaryOption->liftDistance, \
                                   binaryOption->bright
    );
    return binaryOption;
}

static float sReverseFloat( const float inFloat )
{
#if LITTLE_ENDIAN
    float retVal;
    char *floatToConvert = ( char* ) & inFloat;
    char *returnFloat = ( char* ) & retVal;

    // swap the bytes into a temporary buffer
    returnFloat[0] = floatToConvert[3];
    returnFloat[1] = floatToConvert[2];
    returnFloat[2] = floatToConvert[1];
    returnFloat[3] = floatToConvert[0];

    return retVal;
#else
    return inFloat;
#endif

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
