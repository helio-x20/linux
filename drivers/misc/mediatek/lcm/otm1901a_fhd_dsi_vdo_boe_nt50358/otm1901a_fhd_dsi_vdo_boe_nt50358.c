/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

#ifndef BUILD_LK
#include <linux/string.h>
#include <linux/kernel.h>
#endif
#include "lcm_drv.h"

#ifdef BUILD_LK
#include <platform/mt_gpio.h>
#include <platform/mt_i2c.h>

#if 0
#include <platform/upmu_common.h>
#include <platform/mt_pmic.h>
#endif

#include <string.h>
#elif defined(BUILD_UBOOT)
#include <asm/arch/mt_gpio.h>
#else

#if 0
#include <mach/mt_pm_ldo.h>
#endif

#ifdef CONFIG_MTK_LEGACY
#include <mach/mt_gpio.h>
#include <mach/gpio_const.h>
#include <cust_gpio_usage.h>
#endif

#endif
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  (1080)
#define FRAME_HEIGHT (1920)

//#define LCM_ID_OMT9608A ()

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

#define REGFLAG_DELAY             							0XFE
#define REGFLAG_END_OF_TABLE      							0XFD   // END OF REGISTERS MARKER

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	        lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)											lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)   				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)   


#define   LCM_DSI_CMD_MODE							0


static unsigned int lcm_compare_id(void);


struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};





static struct LCM_setting_table lcm_initialization_setting[] = {

	/*

	Note :

	Data ID will depends on the following rule.

		count of parameters > 1	=> Data ID = 0x39
		count of parameters = 1	=> Data ID = 0x15
		count of parameters = 0	=> Data ID = 0x05
		
	Structure Format :

	{DCS command, count of parameters, {parameter list}}
	{REGFLAG_DELAY, milliseconds of time, {}},
	...
	Setting ending by predefined flag
	{REGFLAG_END_OF_TABLE, 0x00, {}}

	*/

    {0x00, 1, {0x00} },
	{0xFF, 3, {0x19, 0x01, 0x01} },

	{0x00, 1, {0x80} },
	{0xFF, 2, {0x19, 0x01} },

	{0x00, 1, {0x00} },
	{0x1C, 1, {0x33} },

	{0x00, 1, {0xA0} },
	{0xC1, 1, {0xE8} },

	{0x00, 1, {0xA7} },
	{0xC1, 1, {0x00} },

	{0x00, 1, {0x90} },
	{0xC0, 6, {0x00, 0x2f, 0x00, 0x00, 0x00, 0x01} },

	{0x00, 1, {0xc0} },
	{0xC0, 6, {0x00, 0x2f, 0x00, 0x00, 0x00, 0x01} },

	{0x00, 1, {0x9A} },
	{0xC0, 1, {0x1E} },

	{0x00, 1, {0xAC} },
	{0xC0, 1, {0x06} },

	{0x00, 1, {0xDC} },
	{0xC0, 1, {0x06} },

	{0x00, 1, {0x81} },
	{0xA5, 1, {0x04} },

	{0x00, 1, {0x84} },
	{0xC4, 1, {0x20} },

	{0x00, 1, {0xa5} },
	{0xB3, 1, {0x1d} },
	
	{0x00, 1, {0x92} },
	{0xE9, 1, {0x00} },

	{0x00, 1, {0x90} },
	{0xF3, 1, {0x01} },

	{0x00, 1, {0xF7} },
	{0xC3, 4, {0x04, 0x18, 0x04, 0x04} },

	{0x00, 1, {0xB4} },
	{0xC0, 1, {0x80} },

	{0x00, 1, {0x93} },
	{0xC5, 1, {0x19} },
	
	{0x00, 1, {0x95} },
	{0xC5, 1, {0x2D} },
	
	{0x00, 1, {0x97} },
	{0xC5, 1, {0x14} },
	
	{0x00, 1, {0x99} },
	{0xC5, 1, {0x28} },
	
	{0x00, 1, {0x00} },
	{0xD8, 2, {0x23, 0x23} },
	
	{0x00, 1, {0x00} },
	{0xE1, 24, {0x00, 0x06, 0x0b, 0x15, 0x1e, 0x26, 0x33, 0x47, 0x53, 0x68, 0x74, 0x7c, 0x7d, 0x75, 0x6e, 0x5b, 0x48, 0x36, 0x2a, 0x23, 0x1a, 0x10, 0x0b, 0x03} },
	
	{0x00, 1, {0xBC} },
	{0xC0, 1, {0x00} },
	
	{0x00, 1, {0x80} },
	{0xC0, 14, {0x00, 0x87, 0x00, 0x0a, 0x0a, 0x00, 0x87, 0x0a, 0x0a, 0x00, 0x87, 0x00, 0x0a, 0x0a} },
	
	{0x00, 1, {0xF0} },
	{0xC3, 6, {0x22, 0x02, 0x00, 0x00, 0x00, 0x0c} },
	
	{0x00, 1, {0xa0} },
	{0xC0, 7, {0x00, 0x00, 0x00, 0x00, 0x03, 0x22, 0x03} },
	
	{0x00, 1, {0xd0} },
	{0xC0, 7, {0x00, 0x00, 0x00, 0x00, 0x03, 0x22, 0x03} },
	
	{0x00, 1, {0x90}},
	{0xC2, 8, {0x83, 0x01, 0x00, 0x00, 0x82, 0x01, 0x00, 0x00}},
	
	{0x00, 1, {0x80}},
	{0xc3, 12, {0x82, 0x02, 0x03, 0x00, 0x03, 0x84, 0x81, 0x03, 0x03, 0x00, 0x03, 0x84}},
	
	{0x00, 1, {0x90}},
	{0xc3, 12, {0x00, 0x01, 0x03, 0x00, 0x03, 0x84, 0x01, 0x02, 0x03, 0x00, 0x03, 0x84}},
	
	{0x00, 1, {0x80}},
	{0xcc, 15, {0x09, 0x0a, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x28, 0x28, 0x28, 0x28, 0x28}},
	
	{0x00, 1, {0x90}},
	{0xcc, 15, {0x0a, 0x09, 0x14, 0x13, 0x12, 0x11, 0x15, 0x16, 0x17, 0x18, 0x28, 0x28, 0x28, 0x28, 0x28}},
	
	{0x00, 1, {0xa0}},
	{0xcc, 15, {0x1d, 0x1e, 0x1f, 0x19, 0x1a, 0x1b, 0x1c, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27}},
	
	{0x00, 1, {0x00}},
	{0xe2, 24, {0x00, 0x06, 0x0b, 0x15, 0x1e, 0x26, 0x33, 0x47, 0x53, 0x68, 0x74, 0x7c, 0x7d, 0x75, 0x6e, 0x5b, 0x48, 0x36, 0x2a, 0x23, 0x1a, 0x10, 0x0b, 0x03}},
	
	{0x00, 1, {0x00}},
	{0xe3, 24, {0x00, 0x06, 0x0b, 0x15, 0x1e, 0x26, 0x33, 0x47, 0x53, 0x68, 0x74, 0x7c, 0x7d, 0x75, 0x6e, 0x5b, 0x48, 0x36, 0x2a, 0x23, 0x1a, 0x10, 0x0b, 0x03}},
	
	{0x00, 1, {0x00}},
	{0xe4, 24, {0x00, 0x06, 0x0b, 0x15, 0x1e, 0x26, 0x33, 0x47, 0x53, 0x68, 0x74, 0x7c, 0x7d, 0x75, 0x6e, 0x5b, 0x48, 0x36, 0x2a, 0x23, 0x1a, 0x10, 0x0b, 0x03}},
	
	{0x00, 1, {0x00}},
	{0xe5, 24, {0x00, 0x06, 0x0b, 0x15, 0x1e, 0x26, 0x33, 0x47, 0x53, 0x68, 0x74, 0x7c, 0x7d, 0x75, 0x6e, 0x5b, 0x48, 0x36, 0x2a, 0x23, 0x1a, 0x10, 0x0b, 0x03}},
	
	{0x00, 1, {0x00}},
	{0xe6, 24, {0x00, 0x06, 0x0b, 0x15, 0x1e, 0x26, 0x33, 0x47, 0x53, 0x68, 0x74, 0x7c, 0x7c, 0x75, 0x6d, 0x5b, 0x48, 0x36, 0x2a, 0x23, 0x1b, 0x1c, 0x0c, 0x03}},
	
	{0x00, 1, {0x00}},
	{0xd9, 1, {0x00}},
	
	{0x00, 1, {0x01}},
	{0xd9, 1, {0xb4}},
	
	{0x00, 1, {0x81}},
	{0xa5, 1, {0x07}},
	
	{0x00, 1, {0x9d}},
	{0xc5, 1, {0x77}},
	
	{0x00, 1, {0xb3}},
	{0xc0, 1, {0xcc, 0xcb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
	
	{0x00, 1, {0x90}},
	{0xcb, 15, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
	
	{0x00, 1, {0xa0}},
	{0xcb, 15, {0x01, 0x01, 0x01,0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
	
	{0x00, 1, {0xb0}},
	{0xcb, 15, {0x00, 0x01, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
	
	{0x00, 1, {0xc0}},
	{0xcb, 8, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x77}},
	
	{0x00, 1, {0xd0}},
	{0xcb, 8, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x77}},
	
	{0x00, 1, {0xe0}},
	{0xcb, 8, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x77}},
	
	{0x00, 1, {0xf0}},
	{0xcb, 8, {0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x77, 0x77}},
	
	{0x00, 1, {0x80}},
	{0xcd, 15, {0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x02, 0x12, 0x11, 0x3f, 0x04, 0x3f}},
	
	{0x00, 1, {0x90}},
	{0xcd, 11, {0x06, 0x3f, 0x3f, 0x26, 0x26, 0x26, 0x21, 0x20, 0x1f, 0x26, 0x26}},
	
	{0x00, 1, {0xa0}},
	{0xcd, 15, {0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x01, 0x12, 0x11, 0x3f, 0x03, 0x3f}},
	
	{0x00, 1, {0xb0}},
	{0xcd, 11, {0x05, 0x3f, 0x3f, 0x26, 0x26, 0x26, 0x21, 0x20, 0x1f, 0x26, 0x26}},
	
	{0x00, 1, {0x9b}},
	{0xc5, 2, {0x55, 0x55}},
	
	{0x00, 1, {0x80}},
	{0xc4, 1, {0x15}},
	
	{0x00, 1, {0x00}},
	{0xff, 3, {0xff, 0xff, 0xff}},
/*
	{0x11, 0, {}},
	{0x10, 0, {}},
	{REGFLAG_DELAY, 12, {}},
	{0x29, 0, {}},
	{0x28, 0, {}},
	{REGFLAG_DELAY, 12, {}},
*/
	 // Sleep Out
	{0x11, 0, {}},
    {REGFLAG_DELAY, 120, {}},

     // Display ON
	{0x29, 0, {}},
	{REGFLAG_DELAY, 5, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {} }
};

static struct LCM_setting_table lcm_sleep_out_setting[] = {
        // Sleep Out
	{0x11, 0, {}},
    {REGFLAG_DELAY, 120, {}},

        // Display ON
	{0x29, 0, {}},
	{REGFLAG_DELAY, 5, {}},

	{REGFLAG_END_OF_TABLE, 0x00, {}}
};




static struct LCM_setting_table lcm_sleep_mode_in_setting[] = {

	// Display off sequence
	{0x28, 0, {}},
	//{REGFLAG_DELAY, 200, {}},

        // Sleep Mode On

	{0x10, 0, {}},
	//{REGFLAG_DELAY, 120, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}

};


void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)

{
	unsigned int i;

    for(i = 0; i < count; i++){ 
        unsigned cmd;
        cmd = table[i].cmd;

        switch (cmd) {
            case REGFLAG_DELAY :
                MDELAY(table[i].count);
                break;

            case REGFLAG_END_OF_TABLE :
                break;

            default:
		dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
		MDELAY(2);
       	}
    }
}

// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}



static void lcm_get_params(LCM_PARAMS *params)
{
		memset(params, 0, sizeof(LCM_PARAMS));
	
		params->type   = LCM_TYPE_DSI;

		params->width  = FRAME_WIDTH;
		params->height = FRAME_HEIGHT;

       //1 SSD2075 has no TE Pin 
		// enable tearing-free
		//params->dbi.te_mode 				= LCM_DBI_TE_MODE_DISABLED;//LCM_DBI_TE_MODE_DISABLED;
		//params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;

        #if (LCM_DSI_CMD_MODE)
		params->dsi.mode   = CMD_MODE;
        #else
		//params->dsi.mode   = SYNC_PULSE_VDO_MODE;
		params->dsi.mode   = BURST_VDO_MODE;
		//params->dsi.mode   = SYNC_EVENT_VDO_MODE; 
		
        #endif
	
		// DSI
		/* Command mode setting */
		//1 Three lane or Four lane
		params->dsi.LANE_NUM		    = LCM_FOUR_LANE;
		//The following defined the fomat for data coming from LCD engine.
		//params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
		//params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
		//params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
		params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

		// Highly depends on LCD driver capability.
		// Not support in MT6573
		//params->dsi.packet_size=256;

		// Video mode setting		
            //params->dsi.intermediat_buffer_num = 0;//because DSI/DPI HW design change, this parameters should be 0 when video mode in MT658X; or memory leakage

		params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
		//params->dsi.word_count=720*3;	

		
		params->dsi.vertical_sync_active				= 2;  //---3
		params->dsi.vertical_backporch					= 6; //---14
		params->dsi.vertical_frontporch					= 13;  //----8
		params->dsi.vertical_active_line				= FRAME_HEIGHT; //1920

		params->dsi.horizontal_sync_active				= 10;  //----2
		params->dsi.horizontal_backporch				= 20; //----28
		params->dsi.horizontal_frontporch				= 40; //----50
		params->dsi.horizontal_active_pixel				= FRAME_WIDTH;//1080


        //	params->dsi.HS_PRPR=6;
	    //params->dsi.LPX=8; 
		//params->dsi.HS_PRPR=5;
		//params->dsi.HS_TRAIL=13;

		// Bit rate calculation
		//1 Every lane speed
		//params->dsi.pll_div1=1;		// div1=0,1,2,3;div1_real=1,2,4,4 ----0: 546Mbps  1:273Mbps
		//params->dsi.pll_div2=1;		// div2=0,1,2,3;div1_real=1,2,4,4	
		//params->dsi.fbk_div =30;//30;    // fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)	

        params->dsi.PLL_CLOCK=401;
		
}

static void lcm_suspend(void)
{
		printk("[lin find] %s %d\n", __func__, __LINE__);
        push_table(lcm_sleep_mode_in_setting, sizeof(lcm_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
}


static void lcm_resume(void)
{
		printk("[lin find] %s %d\n", __func__, __LINE__);
       push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);

}

static void lcm_init(void)
{
	SET_RESET_PIN(1);
    MDELAY(2);          //>1ms
	SET_RESET_PIN(0);
	UDELAY(100);          //>10us
	
	SET_RESET_PIN(1);
	MDELAY(6);          //>5ms  
	
    push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
/*
	lcm_suspend();
	MDELAY(200);          //>5ms  
	lcm_resume();
*/
}


         
#if (LCM_DSI_CMD_MODE)
static void lcm_update(unsigned int x, unsigned int y,
                       unsigned int width, unsigned int height)
{
	unsigned int x0 = x;
	unsigned int y0 = y;
	unsigned int x1 = x0 + width - 1;
	unsigned int y1 = y0 + height - 1;

	unsigned char x0_MSB = ((x0>>8)&0xFF);
	unsigned char x0_LSB = (x0&0xFF);
	unsigned char x1_MSB = ((x1>>8)&0xFF);
	unsigned char x1_LSB = (x1&0xFF);
	unsigned char y0_MSB = ((y0>>8)&0xFF);
	unsigned char y0_LSB = (y0&0xFF);
	unsigned char y1_MSB = ((y1>>8)&0xFF);
	unsigned char y1_LSB = (y1&0xFF);

	unsigned int data_array[16];

	data_array[0]= 0x00053902;
	data_array[1]= (x1_MSB<<24)|(x0_LSB<<16)|(x0_MSB<<8)|0x2a;
	data_array[2]= (x1_LSB);
	dsi_set_cmdq(&data_array, 3, 1);

	data_array[0]= 0x00053902;
	data_array[1]= (y1_MSB<<24)|(y0_LSB<<16)|(y0_MSB<<8)|0x2b;
	data_array[2]= (y1_LSB);
	dsi_set_cmdq(&data_array, 3, 1);
	/*
	data_array[0] = 0x00290508;
	dsi_set_cmdq(&data_array, 1, 1);
	*/
	data_array[0]= 0x002c3909;
	dsi_set_cmdq(data_array, 1, 0);
}
#endif

static unsigned int lcm_compare_id(void)
{

    int   array[4];
    char  buffer[3];
    char  id0=0;
    char  id1=0;
    char  id2=0;
   
    SET_RESET_PIN(1);
    MDELAY(2);
    SET_RESET_PIN(0);
    UDELAY(11);
    SET_RESET_PIN(1);
    MDELAY(6);

	array[0] = 0x00033700;// read id 
	dsi_set_cmdq(array, 1, 1);
	read_reg_v2(0xDA,buffer, 1);
	
	array[0] = 0x00033700;// read id 
	dsi_set_cmdq(array, 1, 1);
	read_reg_v2(0xDB,buffer+1, 1);
	
	array[0] = 0x00033700;// read id 
	dsi_set_cmdq(array, 1, 1);
	read_reg_v2(0xDC,buffer+2, 1);
	
	id0 = buffer[0]; //should be 0x40
	id1 = buffer[1];//should be 0x00
	id2 = buffer[2];//should be 0x00

	#ifdef BUILD_LK
		printf("zhibin uboot %s\n", __func__);
		printf("%s, id0 = 0x%08x\n", __func__, id0);//should be 0x40
		printf("%s, id1 = 0x%08x\n", __func__, id1);//should be 0x00
		printf("%s, id2 = 0x%08x\n", __func__, id2);//should be 0x00
	#else
		printk("zhibin kernel %s\n", __func__);
                printk("%s, id0 = 0x%08x\n", __func__, id0);//should be 0x40
		printk("%s, id1 = 0x%08x\n", __func__, id1);//should be 0x00
		printk("%s, id2 = 0x%08x\n", __func__, id2);//should be 0x00	
	#endif

    if (id0 == 0x40)
        return 1;
    else
        return 0;
}

#if 0
static unsigned int lcm_esd_check(void)
{
#ifndef BUILD_LK
	char  buffer[3];
	int   array[4];

	/// please notice: the max return packet size is 1
	/// if you want to change it, you can refer to the following marked code
	/// but read_reg currently only support read no more than 4 bytes....
	/// if you need to read more, please let BinHan knows.
	/*
			unsigned int data_array[16];
			unsigned int max_return_size = 1;
			
			data_array[0]= 0x00003700 | (max_return_size << 16);	
			
			dsi_set_cmdq(&data_array, 1, 1);
	*/
	array[0] = 0x00023700;// read id return two byte,version and id
	dsi_set_cmdq(array, 1, 1);

	read_reg_v2(0xA1, buffer, 2);
	if(buffer[0]==0x20 && buffer[1] == 0x75)
	{
		return FALSE;
	}
	else
	{			 
		return TRUE;
	}
#endif

}

static unsigned int lcm_esd_recover(void)
{
	lcm_init();
	lcm_resume();

	return TRUE;
}
#endif

LCM_DRIVER otm1901a_fhd_dsi_vdo_boe_nt50358_lcm_drv = 
{
    .name		= "otm1901a_fhd_dsi_vdo_boe_nt50358_ok",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.compare_id    = lcm_compare_id,
//	.esd_check = lcm_esd_check,
//	.esd_recover = lcm_esd_recover,
    #if (LCM_DSI_CMD_MODE)
    .update         = lcm_update,
    #endif
    };
