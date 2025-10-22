#ifndef __X5_MAX9295_MODE_TABLES__
#define __X5_MAX9295_MODE_TABLES__

#include <media/camera_common.h>

enum {
	X5_MAX9295_MODE_1280X1216_CROP_30FPS,
	X5_MAX9295_MODE_START_STREAM,
	X5_MAX9295_MODE_STOP_STREAM,
};

static const int x5_30fps[] = {
	30,
};
 
static const struct camera_common_frmfmt x5_max9295_frmfmt[] = {
	{{1280, 1216}, x5_30fps, 1, 0, X5_MAX9295_MODE_1280X1216_CROP_30FPS},
};

#endif