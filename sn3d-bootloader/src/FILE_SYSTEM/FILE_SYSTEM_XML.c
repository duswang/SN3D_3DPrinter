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

#include "SN_BOOTLOADER.h"

#include "FILE_SYSTEM_XML.h"

/* ******* STATIC DEFINE ******* */

/* ******* GLOBAL VARIABLE ******* */

/* ******* STATIC FUNCTIONS ******* */

/* MAHCINEINFO XML */
static SN_STATUS sParseXML_machineInfoFile(machineInfo_t* machineInfo, xmlDocPtr doc, xmlNodePtr cur);

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
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "hash memory allocate falid.");
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
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "hash memory allocate falid.");
    }

    for(i = 0; i < MD5_DIGEST_LENGTH; ++i)
    {
        sprintf((char*)&hashStr[i * 2], "%02x", (unsigned int)hash[i]);
    }

    printf("\n\n HASH STR :: %s \n\n", hashStr);

    return hashStr;
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
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "XMl File Open Failed.");
    }

    cur = xmlDocGetRootElement(doc);
    if(cur == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "XMl File Open Failed.");
    }

    if (xmlStrcmp(cur->name, (const xmlChar *)"machine"))
    {
        cur = cur->xmlChildrenNode;
        xmlFreeDoc(doc);

        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "XMl File is Invlaid.");

        return NULL;
    }

    mahcineInfo = (machineInfo_t *)malloc(sizeof(machineInfo_t));
    if(mahcineInfo == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "printOption memory allocate failed.");
    }

    retStatus = sParseXML_machineInfoFile(mahcineInfo, doc, cur);
    SN_SYS_ERROR_CHECK(retStatus, "machine Info XML File Load Failed.");

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
            sscanf((const char *)key, "%ld", &machineInfo->screenHeight);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"resolution_w")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%ld", &machineInfo->screenWidth);
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
            sscanf((const char *)key, "%ld", &machineInfo->machineHeight);
            xmlFree(key);
        }
        cur = cur->next;
    }

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
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "XMl File Open Failed.");
    }

    cur = xmlDocGetRootElement(doc);
    if(cur == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "XMl File Open Failed.");
    }

    if (xmlStrcmp(cur->name, (const xmlChar *)"version"))
    {
        cur = cur->xmlChildrenNode;
        xmlFreeDoc(doc);

        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "XMl File is Invlaid.");

        return NULL;
    }

    versionInfo = (versionInfo_t *)malloc(sizeof(versionInfo_t));
    if(versionInfo == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "printOption memory allocate failed.");
    }

    retStatus = sParseXML_VersionFile(versionInfo, doc, cur);
    SN_SYS_ERROR_CHECK(retStatus, "machine Info XML File Load Failed.");

    xmlFreeDoc(doc);

    return versionInfo;
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
            sscanf((const char *)key, "%ld", &versionInfo->releaseNumber);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"majorNumber")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%ld", &versionInfo->majorNumber);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"minorNumber")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%ld", &versionInfo->minorNumber);
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

static unsigned long sGet_size_by_fd(int fd)
{
    struct stat statbuf;

    if(fstat(fd, &statbuf) < 0)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "file can't open.");
    }

    return statbuf.st_size;
}
