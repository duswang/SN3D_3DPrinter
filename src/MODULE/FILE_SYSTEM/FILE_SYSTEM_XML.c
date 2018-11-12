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

/* ******* GLOBAL VARIABLE ******* */

/* ******* STATIC FUNCTIONS ******* */
/* PROJECT XML */
static const char* sParse_ProjectName(xmlDocPtr doc, xmlNodePtr cur);

/* CONFIG XML */
static SN_STATUS sParseXML_ConfigFile(const char* srcPath);

/* OPTION XML */
static SN_STATUS sParseXML_OptionFile(const char* srcPath);

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
const char* FileSystem_GetProjectName(const char *srcPath)
{
    const char         *docname;
    xmlDocPtr           doc;
    xmlNodePtr          cur;
    const char *retStr = NULL;

    if(srcPath == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_INVALID_PARAM, "XMl File Open Failed.");
    }

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
            retStr = sParse_ProjectName(doc, cur);

            if(retStr == NULL)
            {
                SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "CWS File is Invalid File.");
            }
        }
        cur = cur->next;
    }

    xmlFreeDoc(doc);

    return retStr;
}

static const char* sParse_ProjectName(xmlDocPtr doc, xmlNodePtr cur)
{
    char *retStr = NULL;
    xmlChar *key;

    cur = cur->xmlChildrenNode;

    while(cur != NULL)
    {
        if((!xmlStrcmp(cur->name, (const xmlChar *)"name")))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

            if ((retStr = malloc (strlen((const char *)key) + 1)) == NULL)
            {
                return NULL;
            }
            strcpy(retStr, (const char *)FileSystem_fctl_Extarct_FileName((const char *)key));

            xmlFree(key);
        }
        cur = cur->next;
    }

    return retStr;
}

static SN_STATUS sParseXML_ConfigFile(const char* srcPath)
{
    return SN_STATUS_OK;
}
static SN_STATUS sParseXML_OptionFile(const char* srcPath)
{
    return SN_STATUS_OK;
}
