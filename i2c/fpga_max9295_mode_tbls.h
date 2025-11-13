#ifndef __FPGA_MAX9295_MODE_TABLES__
#define __FPGA_MAX9295_MODE_TABLES__

#include <media/camera_common.h>

enum {
	FPGA_MAX9295_MODE_3840X1080_CROP_60FPS,
	FPGA_MAX9295_MODE_START_STREAM,
	FPGA_MAX9295_MODE_STOP_STREAM,
};

static const int fpga_60fps[] = {
	60,
};
 
static const struct camera_common_frmfmt fpga_max9295_frmfmt[] = {
	{{3840, 1080}, fpga_60fps, 1, 0, FPGA_MAX9295_MODE_3840X1080_CROP_60FPS},
};

#endif