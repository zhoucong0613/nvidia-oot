#ifndef __X5_max96724_H__
#define __X5_max96724_H__

#include "abh_gmsl_link.h"

int x5_max96724_start_streaming(struct device *dev, struct gmsl_link_ctx *g_ctx);
int x5_max96724_stop_streaming(struct device *dev, struct gmsl_link_ctx *g_ctx);

int x5_max96724_check_link_status(struct device *dev, int link);
int x5_max96724_monopolize_link(struct device *dev, int link);
int x5_max96724_enable_link(struct device *dev, int link);
int x5_max96724_restore_link(struct device *dev);
int x5_max96724_check_link_status(struct device *dev, int link);
int x5_max96724_set_link_bandwidth(struct device *dev, int link, int gbps);
int x5_max96724_lock_link(struct device *dev);
int x5_max96724_unlock_link(struct device *dev);

#endif  /* __max96724_H__ */