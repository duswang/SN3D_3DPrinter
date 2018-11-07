/**
 * @file SN_MODULE_FILE_SYSTEM_XML.c
 * @author Bato
 * @date 7 Nov 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */


#include "SN_API.h"
#include "SN_MODULE_FILE_SYSTEM.h"

#include "FILE_SYSTEM_XML.h"

/* ******* STATIC DEFINE ******* */

/* ******* GLOBAL VARIABLE ******* */

/* ******* STATIC FUNCTIONS ******* */

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

static const char* parse_ProjectName(xmlDocPtr doc, xmlNodePtr cur)
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
            strcpy(retStr, (const char *)sGetFilename((const char *)key));

            xmlFree(key);
        }
        cur = cur->next;
    }

    return retStr;
}

static const char* sParseXML_TargetName(const char *srcPath)
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
            retStr = parse_ProjectName(doc, cur);

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

static SN_STATUS sParseXML_ConfigFile(const char* srcPath)
{
    return SN_STATUS_OK;
}
static SN_STATUS sParseXML_OptionFile(const char* srcPath)
{
    return SN_STATUS_OK;
}
