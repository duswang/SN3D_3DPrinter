/**
 * @file SN_MODULE_IMAGE_VIEWER.h
 * @author Bato
 * @date 1 Sep 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */
/**
 * @defgroup MODULE_IMAGE_VIEWER Image Viewer
 * @ingroup MODULE
 * @brief Image Viewer Module Functions.
 * @{
 */
#ifndef MODUEL_INCLUDE_SN_MODUEL_IMAGE_VIEWER_H_
#define MODUEL_INCLUDE_SN_MODUEL_IMAGE_VIEWER_H_

/** @name Image Viewer Module
 *  @brief Description of Display Module Init and Uninit funtions.
 *////@{
/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_IMAGE_VIEWER_Init(void);

/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_IMAGE_VIEWER_Destroy(void);
///@}

/** @name Image Viewer Module :: Printing
 *  @brief
 *////@{
/** @brief
 *
 *  @param sliceIndex
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_IMAGE_VIEWER_UPDATE(uint32_t sliceIndex);
///@}

/** @name Image Viewer Module :: Screen Control
 *  @brief
 *////@{
/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_IMAGE_VIEWER_CLEAR(void);
///@}
#endif /* MODUEL_INCLUDE_SN_MODUEL_IMAGE_VIEWER_H_ */
/**@}*/
