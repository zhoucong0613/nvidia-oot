#ifndef __ABH_OV9282_MAX9295D_MODE_TABLES__
#define __ABH_OV9282_MAX9295D_MODE_TABLES__

#include <media/camera_common.h>

enum {
	ABH_MAX9295D_MODE_1280X400_CROP_30FPS,
	ABH_MAX9295D_START_STREAM,
	ABH_MAX9295D_STOP_STREAM,
};

static const int abh_30fps[] = {
	30,
};


static const struct camera_common_frmfmt abh_max9295d_frmfmt_deepth[] = {
	{{1280, 400}, abh_30fps, 1, 0, ABH_MAX9295D_MODE_1280X400_CROP_30FPS},
};

#endif