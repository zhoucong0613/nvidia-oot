
#ifndef __GMSL_LINK_H__
#define __GMSL_LINK_H__

#define GMSL_CSI_1X4_MODE 0x1
#define GMSL_CSI_2X4_MODE 0x2
#define GMSL_CSI_2X2_MODE 0x3
#define GMSL_CSI_4X2_MODE 0x4

#define GMSL_CSI_PORT_A 0x0
#define GMSL_CSI_PORT_B 0x1
#define GMSL_CSI_PORT_C 0x2
#define GMSL_CSI_PORT_D 0x3
#define GMSL_CSI_PORT_E 0x4
#define GMSL_CSI_PORT_F 0x5

#define GMSL_SERDES_CSI_LINK_A 0x1
#define GMSL_SERDES_CSI_LINK_B 0x2

/* Didn't find kernel defintions, for now adding here */
#define GMSL_CSI_DT_RAW_8 0x2A
#define GMSL_CSI_DT_RAW_10 0x2B
#define GMSL_CSI_DT_RAW_12 0x2C
#define GMSL_CSI_DT_UED_U1 0x30
#define GMSL_CSI_DT_EMBED 0x12
#define GMSL_CSI_DT_YUV16 0x1E
#define GMSL_CSI_DT_YUV10 0x1F

#define GMSL_ST_ID_UNUSED 0xFF

/**
 * Maximum number of data streams (\ref gmsl_stream elements) in a GMSL link
 * (\ref gmsl_link_ctx).
 */
#define GMSL_DEV_MAX_NUM_DATA_STREAMS 4

/**
 * Holds the configuration of the GMSL links from a sensor to its serializer to
 * its deserializer.
 */
struct gmsl_link_ctx {
	// __u32 st_vc;            /**< Default sensor virtual channel. */
	// __u32 dst_vc;           /**< Destination virtual channel (user-defined). */
	// __u32 src_csi_port;     /**< Sensor to serializer CSI port connection. */
	// __u32 dst_csi_port;     /**< Deserializer to Jetson CSI port connection. */
	// __u32 serdes_csi_link;  /**< GMSL link between serializer and deserializer
    //                             devices. */
	__u32 num_streams;      /**< Number of active streams to be mapped
                                from sensor. */
	__u32 num_ser_csi_lanes;    /**< Sensor's CSI lane configuration. */
//	__u32 csi_mode;         /**< Deserializer CSI mode. */
	__u32 ser_reg;          /**< Serializer slave address. */
	__u32 sdev_reg;         /**< Sensor proxy slave address. */
	__u32 sdev_def;         /**< Sensor default slave address. */
	// bool serdev_found;      /**< Indicates whether the serializer device for
    //                          the specified sensor source was found. Set by
    //                          the serializer driver during setup; used by
    //                          the deserializer driver to choose certain
    //                          configuration settings during setup. */
	__u32 streams[GMSL_DEV_MAX_NUM_DATA_STREAMS];
        /*< An array of information about the data streams in the link. */
	struct device *sensor_dev;   /**< Sensor device handle. */
	struct device *des_dev;
	struct device *ser_dev;  
	__u32 ser_inv_polarity;
	__u8 multi_vc;
	__u8 des_link;
};

/** @} */

#endif  /* __GMSL_LINK_H__ */