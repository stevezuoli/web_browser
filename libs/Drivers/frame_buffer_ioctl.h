#ifndef _FB_IOCTL_H
#define _FB_IOCTL_H

#define FBIO_MAGIC_NUMBER                   'F'

// Implemented in the eInk HAL.
#define FBIO_EINK_UPDATE_DISPLAY            _IO(FBIO_MAGIC_NUMBER, 0xdb) // 0x46db (fx_type: fx_update_full || fx_update_partial)
#define FBIO_EINK_UPDATE_DISPLAY_AREA       _IO(FBIO_MAGIC_NUMBER, 0xdd) // 0x46dd (update_area_t *)
#define FBIO_EINK_RESTORE_DISPLAY           _IO(FBIO_MAGIC_NUMBER, 0xef) // 0x46ef (fx_type: fx_update_full || fx_update_partial)
#define FBIO_EINK_SET_REBOOT_BEHAVIOR       _IO(FBIO_MAGIC_NUMBER, 0xe9) // 0x46e9 (reboot_behavior_t)
#define FBIO_EINK_GET_REBOOT_BEHAVIOR       _IO(FBIO_MAGIC_NUMBER, 0xed) // 0x46ed (reboot_behavior_t *)
#define FBIO_EINK_SET_DISPLAY_ORIENTATION   _IO(FBIO_MAGIC_NUMBER, 0xf0) // 0x46f0 (orientation_t)
#define FBIO_EINK_GET_DISPLAY_ORIENTATION   _IO(FBIO_MAGIC_NUMBER, 0xf1) // 0x46f1 (orientation_t *)

typedef unsigned char uchar;

typedef struct mxcfb_rect {
	quint32 top;
	quint32 left;
	quint32 width;
	quint32 height;
};

typedef struct mxcfb_alt_buffer_data {
	quint32 phys_addr;
	quint32 width;	/* width of entire buffer */
	quint32 height;	/* height of entire buffer */
	struct mxcfb_rect alt_update_region;	/* region within buffer to update */
};


typedef struct mxcfb_update_data { 
    struct mxcfb_rect update_region; 
    
    quint32 waveform_mode; // 0x0002 = WAVEFORM_MODE_GC16 
    quint32 update_mode; // 0x0000 = UPDATE_MODE_PARTIAL 
    quint32 update_marker; // 0x002a 
    int temp; // 0x1001 = TEMP_USE_PAPYRUS 
    quint32 flags; // 0x0000 
    struct mxcfb_alt_buffer_data alt_buffer_data; // must not used when flags is 0
};


typedef struct mxcfb_update_data_v510 { 
	struct mxcfb_rect update_region; 

	quint32 waveform_mode; // 0x0002 = WAVEFORM_MODE_GC16 
	quint32 update_mode; // 0x0000 = UPDATE_MODE_PARTIAL 
	quint32 update_marker; // 0x002a 
	quint32 hist_bw_waveform_mode;
	quint32 hist_gray_waveform_mode;
	int temp; // 0x1001 = TEMP_USE_PAPYRUS 
	quint32 flags; // 0x0000 
	struct mxcfb_alt_buffer_data alt_buffer_data; // must not used when flags is 0
};

//Kindle Touch 参数
#define MXCFB_WAIT_FOR_VSYNC	_IOW('F', 0x20, u_int32_t)
#define MXCFB_SET_GBL_ALPHA     _IOW('F', 0x21, struct mxcfb_gbl_alpha)
#define MXCFB_SET_CLR_KEY       _IOW('F', 0x22, struct mxcfb_color_key)
#define MXCFB_SET_OVERLAY_POS   _IOWR('F', 0x24, struct mxcfb_pos)
#define MXCFB_GET_FB_IPU_CHAN 	_IOR('F', 0x25, u_int32_t)
#define MXCFB_SET_LOC_ALPHA     _IOWR('F', 0x26, struct mxcfb_loc_alpha)
#define MXCFB_SET_LOC_ALP_BUF    _IOW('F', 0x27, unsigned long)
#define MXCFB_SET_GAMMA	       _IOW('F', 0x28, struct mxcfb_gamma)
#define MXCFB_GET_FB_IPU_DI 	_IOR('F', 0x29, u_int32_t)
#define MXCFB_GET_DIFMT	       _IOR('F', 0x2A, u_int32_t)
#define MXCFB_GET_FB_BLANK     _IOR('F', 0x2B, u_int32_t)
#define MXCFB_SET_DIFMT		_IOW('F', 0x2C, u_int32_t)

// IOCTLs for Kindle Touch E-ink panel updates 
// IOCTL 命令
#define MXCFB_SET_WAVEFORM_MODES	_IOW('F', 0x2B, struct mxcfb_waveform_modes)
#define MXCFB_SET_TEMPERATURE		_IOW('F', 0x2C, int32_t)
#define MXCFB_SET_AUTO_UPDATE_MODE	_IOW('F', 0x2D, quint32)
#define MXCFB_SEND_UPDATE		_IOW('F', 0x2E, struct mxcfb_update_data)
#define MXCFB_WAIT_FOR_UPDATE_COMPLETE	_IOW('F', 0x2F, quint32)
#define MXCFB_SET_PWRDOWN_DELAY		_IOW('F', 0x30, int32_t)
#define MXCFB_GET_PWRDOWN_DELAY		_IOR('F', 0x31, int32_t)
#define MXCFB_SET_UPDATE_SCHEME		_IOW('F', 0x32, quint32)
#define MXCFB_SET_PAUSE			_IOW('F', 0x33, quint32)
#define MXCFB_GET_PAUSE			_IOR('F', 0x34, quint32)
#define MXCFB_SET_RESUME		_IOW('F', 0x35, quint32)
#define MXCFB_CLEAR_UPDATE_QUEUE	_IOW('F', 0x36, quint32)
#define MXCFB_SEND_UPDATE_V510		_IOW('F', 0x2E, struct mxcfb_update_data_v510)

// Kindle Touch update 参数
#define GRAYSCALE_8BIT				0x1
#define GRAYSCALE_8BIT_INVERTED			0x2

#define AUTO_UPDATE_MODE_REGION_MODE		0
#define AUTO_UPDATE_MODE_AUTOMATIC_MODE		1
#define AUTO_UPDATE_MODE_AUTOMATIC_MODE_FULL	AUTO_UPDATE_MODE_AUTOMATIC_MODE // Lab126 
#define AUTO_UPDATE_MODE_AUTOMATIC_MODE_PART	2 // Lab126 

#define UPDATE_SCHEME_SNAPSHOT			0
#define UPDATE_SCHEME_QUEUE			1
#define UPDATE_SCHEME_QUEUE_AND_MERGE		2

#define UPDATE_MODE_PARTIAL			0x0
#define UPDATE_MODE_FULL			0x1

#define WAVEFORM_MODE_INIT	0x0	// Screen goes to white (clears) 
#define WAVEFORM_MODE_DU	0x1	// Grey->white/grey->black 
#define WAVEFORM_MODE_GC16	0x2	// High fidelity (flashing)
#define WAVEFORM_MODE_GC4	0x3	// Lower fidelity 
#define WAVEFORM_MODE_A2	0x4	// Faster but even lower fidelity 
#define WAVEFORM_MODE_GL16	0x5	// High fidelity from white transition 

#define WAVEFORM_MODE_AUTO			257

#define TEMP_USE_AMBIENT			0x1000
#define TEMP_USE_PAPYRUS			0X1001

#define EPDC_FLAG_ENABLE_INVERSION		0x01
#define EPDC_FLAG_FORCE_MONOCHROME		0x02
#define EPDC_FLAG_USE_CMAP				0x04
#define EPDC_FLAG_USE_ALT_BUFFER		0x100

#define FB_POWERDOWN_DISABLE			-1




enum einkfb_events_t
{
    einkfb_event_update_display = 0,        // FBIO_EINK_UPDATE_DISPLAY
    einkfb_event_update_display_area,       // FBIO_EINK_UPDATE_DISPLAY_AREA

    einkfb_event_blank_display,             // FBIOBLANK (fb.h)
    einkfb_event_rotate_display,            // FBIO_EINK_SET_DISPLAY_ORIENTATION

    einkfb_event_null = -1
};
typedef enum einkfb_events_t einkfb_events_t;

enum fx_type
{
    // Deprecated from HAL, but still supported by Shim.
    //
    fx_stipple_posterize_dark = 5,          // Row 0: WPWP...; row 1: PBPB... (W=white, B=black, P=posterize).
    fx_stipple_posterize_lite = 6,          // Row 0: WPWP...; row 1: PWPW... (W=white, P=posterize).

    fx_stipple_lighten_dark = 7,            // Row 0: WXWX...; row 1: XBXB... (W=white, B=black, X=keep).
    fx_stipple_lighten_lite = 8,            // Row 0: WXWX...; row 1: XWXW... (W=white, X=keep).

    fx_mask = 11,                           // Only for use with update_area_t's non-NULL buffer which_fx.
    fx_buf_is_mask = 14,                    // Same as fx_mask, but doesn't require a doubling (i.e., the buffer & mask are the same).

    fx_flash = 20,                          // Only for use with update_area_t (for faking a flashing update).

    fx_none = -1,                           // No legacy-FX to apply.

    // Screen-update FX, supported by HAL.
    //
    fx_update_partial = 0,                  // eInk GU/PU/MU-style (non-flashing) update.
    fx_update_full = 1                      // eInk GC-style (slower, flashing) update.
};

typedef enum fx_type fx_type;

struct update_area_t
{
    // Note:  The bottom-right (x2, y2) coordinate is actually such that (x2 - x1) and (y2 - y1)
    //        are xres and yres, respectively, when normally xres and yres would be
    //        (x2 - x1) + 1 and (y2 - y1) + 1, respectively.
    //
    int         x1, y1,                     // Top-left...
                x2, y2;                     // ...bottom-right.

    fx_type     which_fx;                   // FX to use.

    qint8       *buffer;                    // If NULL, extract from framebuffer, top-left to bottom-right, by rowbytes.
};

typedef struct update_area_t update_area_t;
