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

#include "SN_API.h"
#include "SN_MODULE_FILE_SYSTEM.h"

#include "FILE_SYSTEM_XML.h"
#include "FILE_SYSTEM_FCTL.h"

/* ******* STATIC DEFINE ******* */
/** @name Z config *////@{
#define Z_DELAY_OFFSET (1600)

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

/* OPTION XML */
static SN_STATUS sParseXML_optionFile(printOption_t* printOption, xmlDocPtr doc, xmlNodePtr cur);

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

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
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "XMl File Open Failed.");
    }

    cur = xmlDocGetRootElement(doc);
    if(cur == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "XMl File Open Failed.");
    }

    cur = cur->xmlChildrenNode;

    while (cur != NULL)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"GCode")))
        {
            retStatus = sParseXML_targetFile(printTarget, doc, cur);
            SN_SYS_ERROR_CHECK(retStatus, "target XML File Load Failed.");
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
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "XMl File Open Failed.");
    }

    cur = xmlDocGetRootElement(doc);
    if(cur == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "XMl File Open Failed.");
    }

    if (xmlStrcmp(cur->name, (const xmlChar *)"option"))
    {
        cur = cur->xmlChildrenNode;
        xmlFreeDoc(doc);

        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "XMl File is Invlaid.");

        return NULL;
    }

    printOption = (printOption_t *)malloc(sizeof(printOption_t));
    if(printOption == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "printOption memory allocate failed.");
    }

    retStatus = sParseXML_optionFile(printOption, doc, cur);
    SN_SYS_ERROR_CHECK(retStatus, "option XML File Load Failed.");

    xmlFreeDoc(doc);

    return printOption;
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
            sscanf((const char *)key, "%ld", &machineInfo->screenWdith);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"Z")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%ld", &machineInfo->machineHeight);
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
        if((!xmlStrcmp(cur->name, (const xmlChar *)"name")))
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
            sscanf((const char *)key, "%ld", &printOption->bottomLayerExposureTime);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"bottomLayerNumber")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%ld", &printOption->bottomLayerNumber);
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
            sscanf((const char *)key, "%ld", &printOption->layerExposureTime);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"liftDistance")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%ld", &printOption->liftDistance);
            xmlFree(key);
        }
        if((!xmlStrcmp(cur->name, (const xmlChar *)"liftFeedRate")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            sscanf((const char *)key, "%f", &printOption->liftFeedRate);
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
static SN_STATUS sParseXML_targetFile(printTarget_t* printTarget, xmlDocPtr doc, xmlNodePtr cur)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    return retStatus;
}
