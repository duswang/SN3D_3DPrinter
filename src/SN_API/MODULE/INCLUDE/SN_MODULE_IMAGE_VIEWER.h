/**
 * @file SN_MODULE_IMAGE_VIEWER.h
 * @author Bato
 * @date 1 Sep 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @defgroup MODULE_IMAGE_VIEWER Image Viewer
 * @ingroup MODULE
 * @brief Image Viewer Module Functions.
 * @{
 */

#ifndef MODUEL_INCLUDE_SN_MODUEL_IMAGE_VIEWER_H_
#define MODUEL_INCLUDE_SN_MODUEL_IMAGE_VIEWER_H_

/*************************************************************
 * @name Image Viewer Module
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

/*************************************************************@}*/


/*************************************************************
 * @name Image Viewer Module :: Window Control
 *  @brief
 *////@{
/** @brief Update Machine Screen and Thumbnail on Nextion Display
 *
 *  @param sliceIndex - Current slice index
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_IMAGE_VIEWER_WindowUpdate(uint32_t sliceIndex);

/** @brief Clean Machine Screen and Thumbnail on Nextion Display
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_IMAGE_VIEWER_WindowClean(void);

/** @brief Destory Window
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_IMAGE_VIEWER_WindowDestroy(void);

/*************************************************************@}*/

/*************************************************************
 * @name Image Viewer Module :: Nextion LCD Control
 *  @brief nextion display thumbnail control functions
 *  @warning thumbnail functions work with 'window' functions
 *  @note It take 0.8 msec task time on 120 width thumbnail image.
 *////@{

//extern  SN_STATUS SN_MODULE_IMAGE_VIEWER_ThumbnailUpdate(void);

extern SN_STATUS SN_MODULE_IMAGE_VIEWER_ThumbnailClean(void);

//extern  SN_STATUS SN_MODULE_IMAGE_VIEWER_ThumbnailDistroy(void);

/*************************************************************@}*/

#endif /* MODUEL_INCLUDE_SN_MODUEL_IMAGE_VIEWER_H_ */
/**@}*/
