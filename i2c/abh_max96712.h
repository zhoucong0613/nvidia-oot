/*
 * Copyright (c) 2018-2020, NVIDIA Corporation.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file
 * <b>MAX96712 API: For Maxim Integrated MAX96712 deserializer</b>
 *
 * @b Description: Defines elements used to set up and use a
 *  Maxim Integrated MAX96712 deserializer.
 */

#ifndef __MAX96712_H__
#define __MAX96712_H__

#include "abh_gmsl_link.h"

int abh_max96712_start_streaming(struct device *dev, struct gmsl_link_ctx *g_ctx);
int abh_max96712_start_streaming_deepth(struct device *dev, struct gmsl_link_ctx *g_ctx);
int abh_max96712_stop_streaming(struct device *dev, struct gmsl_link_ctx *g_ctx);

int abh_max96712_check_link_status(struct device *dev, int link);
int abh_max96712_monopolize_link(struct device *dev, int link);
int abh_max96712_enable_link(struct device *dev, int link);
int abh_max96712_restore_link(struct device *dev);
int abh_max96712_check_link_status(struct device *dev, int link);
int abh_max96712_set_link_bandwidth(struct device *dev, int link, int gbps);
int abh_max96712_get_ox5b_config(struct device *dev, int link);
int abh_max96712_lock_link(struct device *dev);
int abh_max96712_unlock_link(struct device *dev);

/** @} */

#endif  /* __MAX96712_H__ */