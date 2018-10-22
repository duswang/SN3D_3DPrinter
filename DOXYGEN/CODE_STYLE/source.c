/**
 * @file source.c
 * @author
 * @date
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */
#include "header.h"

/** @struct style_structure
 *  @brief
 */
typedef struct style_structure {
    int val;        /**< Value */
    int index;      /**< Index */
} styleStruct_t;

/** @enum style_enum
 *  @brief
 */
typedef enum style_enum {
    MSG_1,          /**< mseeage 1 */
    MSG_2,          /**< mseeage 2 */
    MSG_3,          /**< mseeage 3 */
    MSG_N           /**< number of mseeage */
} styleEnum_t;

int module_init(void)
{
    int retValue = 0;

    return retValue;
}

int module_uninit(void)
{
    int retValue = 0;

    return retValue;
}
