#ifndef __ISX_MAX9295_MODE_TABLES__
#define __ISX_MAX9295_MODE_TABLES__

#include <media/camera_common.h>

enum {
	isx_MAX9295_MODE_1920X1536_CROP_30FPS,
	isx_MAX9295_MODE_START_STREAM,
	isx_MAX9295_MODE_STOP_STREAM,
};

static const int isx_30fps[] = {
	30,
};
 
static const struct camera_common_frmfmt isx_max9295_frmfmt[] = {
	{{1920, 1536}, isx_30fps, 1, 0, isx_MAX9295_MODE_1920X1536_CROP_30FPS},
};

#endif