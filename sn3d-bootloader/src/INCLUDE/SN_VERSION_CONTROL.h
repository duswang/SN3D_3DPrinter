/*
 * SN_VERSION_CONTROL.c
 *
 *  Created on: 2018. 12. 13.
 *      Author: BartKim
 */

#ifndef SN_VERSION_CONTROL_H_
#define SN_VERSION_CONTROL_H_

#define SN3D_FW_STR         "sn3d_fw"
#define SN3D_FW_EXTENTION   "zip"

#define SN3D_OPTION_STR         "sn3d_option"
#define SN3D_OPTION_EXTENTION   "zip"

#define HIDDEN_FILE_STR         "._"

extern const char* SN_VERSION_CONTROL_VersionCheck(void);

extern void SN_VERSION_CONTROL_VersionUpdate(const char* fileName);

#endif /* SN_VERSION_CONTROL_H_ */
