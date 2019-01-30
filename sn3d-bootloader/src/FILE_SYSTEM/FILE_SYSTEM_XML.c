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
#endif

#ifdef linux
#include <libxml/xmlreader.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#endif

#include "SN_BOOTLOADER.h"

#include "FILE_SYSTEM_XML.h"

/* ******* STATIC DEFINE ******* */

/* ******* GLOBAL VARIABLE ******* */

/* ******* STATIC FUNCTIONS ******* */

/* MAHCINEINFO XML */
static SN_STATUS sParseXML_machineInfoFile(machineInfo_t* machineInfo, xmlDocPtr doc, xmlNodePtr cur);

static SN_STATUS sParseXML_VersionFile(versionInfo_t* versionInfo, xmlDocPtr doc, xmlNodePtr cur);

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

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
        if((!xmlStrcmp(cur->name, (const xmlChar *)"name")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            strcpy(machineInfo->name, (const char *)key);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"RES_Y")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%ld", &machineInfo->screenHeight);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"RES_X")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%ld", &machineInfo->screenWidth);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"Z")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%ld", &machineInfo->machineHeight);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"Display")))
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
        if((!xmlStrcmp(cur->name, (const xmlChar *)"Project")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            strcpy(versionInfo->name, (const char *)key);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"ReleaseNumber")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%ld", &versionInfo->releaseNumber);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"MajorNumber")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%ld", &versionInfo->majorNumber);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"MinorNumber")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%ld", &versionInfo->minorNumber);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"Timestamp")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            strcpy(versionInfo->timestamp, (const char *)key);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"BinraryName")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            strcpy(versionInfo->binaryName, (const char *)key);
            xmlFree(key);
        }
        cur = cur->next;
    }

    return retStatus;
}


