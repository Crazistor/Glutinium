#if !defined(__SC1145_CMOS_H_)
#define __SC1145_CMOS_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hi_comm_sns.h"
#include "hi_comm_video.h"
#include "hi_sns_ctrl.h"
#include "mpi_isp.h"
#include "mpi_ae.h"
#include "mpi_awb.h"
#include "mpi_af.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


#define SC1145_ID 1145
#define CMOS_SC1145_ISP_WRITE_SENSOR_ENABLE 1


#ifdef INIFILE_CONFIG_MODE

extern AE_SENSOR_DEFAULT_S  g_AeDft[];
extern AWB_SENSOR_DEFAULT_S g_AwbDft[];
extern ISP_CMOS_DEFAULT_S   g_IspDft[];
extern HI_S32 Cmos_LoadINIPara(const HI_CHAR *pcName);
#else

#endif


/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/

extern const unsigned int sensor_i2c_addr;
extern unsigned int sensor_addr_byte;
extern unsigned int sensor_data_byte;

#define SENSOR_720P_30FPS_MODE (1)

#define INCREASE_LINES (0) /* make real fps less than stand fps because NVR require*/
#define VMAX_720P30_LINEAR     (750+INCREASE_LINES)
#define VMAX_SC1145_720P30_LINEAR VMAX_720P30_LINEAR
#define CMOS_OV1145_SLOW_FRAMERATE_MODE (0)

#define FULL_LINES_MAX  (0xFFFF)


HI_U8 gu8SensorImageMode = SENSOR_720P_30FPS_MODE;
WDR_MODE_E genSensorMode = WDR_MODE_NONE;

static HI_U32 gu32FullLinesStd = VMAX_SC1145_720P30_LINEAR;
static HI_U32 gu32FullLines = VMAX_SC1145_720P30_LINEAR;

static HI_BOOL bInit = HI_FALSE;
HI_BOOL bSensorInit = HI_FALSE;
ISP_SNS_REGS_INFO_S g_stSnsRegsInfo = {0};
ISP_SNS_REGS_INFO_S g_stPreSnsRegsInfo = {0};
static HI_U8 gu8Fps = 30;

#if 0
/* Piris attr */
static ISP_PIRIS_ATTR_S gstPirisAttr=
{
	0,      // bStepFNOTableChange
	1,      // bZeroIsMax
	93,     // u16TotalStep
	62,     // u16StepCount
	/* Step-F number mapping table. Must be from small to large. F1.0 is 1024 and F32.0 is 1 */
	{30,35,40,45,50,56,61,67,73,79,85,92,98,105,112,120,127,135,143,150,158,166,174,183,191,200,208,217,225,234,243,252,261,270,279,289,298,307,316,325,335,344,353,362,372,381,390,399,408,417,426,435,444,453,462,470,478,486,493,500,506,512},
	ISP_IRIS_F_NO_1_4, // enMaxIrisFNOTarget
	ISP_IRIS_F_NO_5_6  // enMinIrisFNOTarget
};


#define PATHLEN_MAX 256
#define CMOS_CFG_INI "sc1145_cfg.ini"
static char pcName[PATHLEN_MAX] = "configs/sc1145_cfg.ini";
#endif

/* AE default parameter and function */
static HI_S32 cmos_get_ae_default(AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	if (HI_NULL == pstAeSnsDft)
	{
		printf("null pointer when get ae default value!\n");
		return -1;
	}

	pstAeSnsDft->u32LinesPer500ms = VMAX_SC1145_720P30_LINEAR * 30 / 2;
	pstAeSnsDft->u32FullLinesStd = gu32FullLinesStd;
	pstAeSnsDft->u32FlickerFreq = 0;

	pstAeSnsDft->au8HistThresh[0] = 0xd;
	pstAeSnsDft->au8HistThresh[1] = 0x28;
	pstAeSnsDft->au8HistThresh[2] = 0x60;
	pstAeSnsDft->au8HistThresh[3] = 0x80;

	pstAeSnsDft->u8AeCompensation = 0x38;

	pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
	pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
	pstAeSnsDft->stIntTimeAccu.f32Offset = 0;
	pstAeSnsDft->u32MaxIntTime = gu32FullLinesStd - 2;
	pstAeSnsDft->u32MinIntTime = 2;
	pstAeSnsDft->u32MaxIntTimeTarget = 65535;
	pstAeSnsDft->u32MinIntTimeTarget = 2;


	pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
	pstAeSnsDft->stAgainAccu.f32Accuracy = 1;
	pstAeSnsDft->u32MaxAgain = 16229;
	pstAeSnsDft->u32MinAgain = 1024;
	pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
	pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;



	pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_TABLE;
	pstAeSnsDft->stDgainAccu.f32Accuracy = 1;
	pstAeSnsDft->u32MaxDgain = 4096;
	pstAeSnsDft->u32MinDgain = 1024;
	pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
	pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;



	pstAeSnsDft->u32ISPDgainShift = 8;
	pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
	pstAeSnsDft->u32MaxISPDgainTarget = 4 << pstAeSnsDft->u32ISPDgainShift;



	pstAeSnsDft->u32LinesPer500ms = gu32FullLinesStd*30/2;

	return 0;
}

/* the function of sensor set fps */
static HI_VOID cmos_fps_set(HI_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{

	HI_U32 u32VblankingLines = 0xFFFF;
	if ((f32Fps <= 30) && (f32Fps >= 0.5))
	{
		if(SENSOR_720P_30FPS_MODE == gu8SensorImageMode)
		{
			u32VblankingLines = VMAX_720P30_LINEAR * 30 / f32Fps;
		}
	}
	else
	{
		printf("Not support Fps: %f\n", f32Fps);
		return;
	}


#if CMOS_SC1145_ISP_WRITE_SENSOR_ENABLE
	g_stSnsRegsInfo.astI2cData[4].u32Data = (u32VblankingLines >> 8)&0xff;
	g_stSnsRegsInfo.astI2cData[5].u32Data = u32VblankingLines & 0xFF;
#else
	sensor_write_register(0x320e, (u32VblankingLines >> 8)&0xff);
	sensor_write_register(0x320f, u32VblankingLines & 0xff);
#endif

	pstAeSnsDft->f32Fps = f32Fps;
	pstAeSnsDft->u32MaxIntTime = u32VblankingLines - 4;
	gu32FullLinesStd = u32VblankingLines;
	gu8Fps = f32Fps;
	pstAeSnsDft->u32LinesPer500ms = u32VblankingLines * f32Fps / 2;
	pstAeSnsDft->u32FullLinesStd = gu32FullLinesStd;

	return;
}

static HI_VOID cmos_slow_framerate_set(HI_U32 u32FullLines,
		AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{

	u32FullLines = (u32FullLines > 0xFFFF) ? 0xFFFF : u32FullLines;
	gu32FullLines = u32FullLines;


#if CMOS_SC1145_ISP_WRITE_SENSOR_ENABLE
	g_stSnsRegsInfo.astI2cData[4].u32Data = ((u32FullLines >> 8) & 0xFF);
	g_stSnsRegsInfo.astI2cData[5].u32Data = u32FullLines & 0xFF;
#else
	sensor_write_register(0x320e, ((u32FullLines >> 8) & 0xFF));
	sensor_write_register(0x320f, u32FullLines & 0xFF);
#endif
	pstAeSnsDft->u32MaxIntTime = gu32FullLines - 4;
	pstAeSnsDft->u32MaxIntTimeTarget= gu32FullLines - 4;

	return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static HI_VOID cmos_inttime_update(HI_U32 u32IntTime)
{

#if CMOS_SC1145_ISP_WRITE_SENSOR_ENABLE
	g_stSnsRegsInfo.astI2cData[0].u32Data = (u32IntTime >> 4) & 0xFF;
	g_stSnsRegsInfo.astI2cData[1].u32Data = (u32IntTime<<4) & 0xF0;
#else
	sensor_write_register(0x3e01, ((u32IntTime >> 4) & 0xFF));
	sensor_write_register(0x3e02, (u32IntTime<<4) & 0xF0);
#endif

	return;
}

static  HI_U32   analog_gain_table[64] =
{
	1024, 1084, 1145, 1205, 1265,  1325,  1385,   1446,  1506,  1566,  1626,  1687,  1747,  1807,  1868,  1927,
	2048, 2169, 2290, 2410, 2592,  2650,  2771,   2891,  3013,  3131,  3252,  3373,  3494,  3615,  3736,  3854,
	4096, 4338, 4579, 4821, 5059,  5300,  5542,   5784,  6025,  6263,  6504,  6746,  6988,  7229,  7471,  7709,
	8192, 8675, 9159, 9642, 10117, 10600, 11083,  11567, 12050, 12526, 13009, 13492, 13976, 14456, 14942, 15417
};

static  HI_U32   digital_gain_table[5] =
{
	1024,2048,4096,8192,16384
};



static HI_VOID cmos_again_calc_table(HI_U32 *pu32AgainLin, HI_U32 *pu32AgainDb)
{
	int i;

	if((HI_NULL == pu32AgainLin) ||(HI_NULL == pu32AgainDb))
	{
		printf("null pointer when get ae sensor gain info  value!\n");
		return;
	}


	if (*pu32AgainLin >= analog_gain_table[63])
	{
		*pu32AgainLin = analog_gain_table[63];
		*pu32AgainDb = 63;
		return ;
	}

	for (i = 1; i < 64; i++)
	{
		if (*pu32AgainLin < analog_gain_table[i])
		{
			*pu32AgainLin = analog_gain_table[i - 1];
			*pu32AgainDb = i - 1;
			break;
		}
	}

	return;

}


static HI_VOID cmos_dgain_calc_table(HI_U32 *pu32DgainLin, HI_U32 *pu32DgainDb)
{
	int i;

	if((HI_NULL == pu32DgainLin) ||(HI_NULL == pu32DgainDb))
	{
		printf("null pointer when get ae sensor gain info  value!\n");
		return;
	}


	if (*pu32DgainLin >= digital_gain_table[4])
	{
		*pu32DgainLin = digital_gain_table[4];
		*pu32DgainDb = 4;
		return ;
	}

	for(i = 1; i < 5; i++)
	{
		if(*pu32DgainLin < digital_gain_table[i])
		{
			*pu32DgainLin = digital_gain_table[i - 1];
			*pu32DgainDb = i - 1;
			return;
		}

	}
	return;

}

static HI_VOID cmos_gains_update(HI_U32 u32Again, HI_U32 u32Dgain)
{
	HI_U8 u8AgainHigh, u8AgainLow;
	HI_U8 u8DgainReg;


	if (u32Again >= 1 && u32Again < 0x10)
		u8AgainHigh = 0x0;
	else if (u32Again >= 0x10 && u32Again < 0x20)
		u8AgainHigh = 0x1;
	else if (u32Again >= 0x20 && u32Again < 0x30)
		u8AgainHigh = 0x3;
	else if (u32Again >= 0x30 && u32Again < 0x40)
		u8AgainHigh = 0x7;
	else
		u8AgainHigh = 0x0;

	u8AgainLow = u32Again & 0xf;

	switch(u32Dgain)
	{
	case 0:
		u8DgainReg = 0x84;
		break;

	case 1:
		u8DgainReg = 0xa4;
		break;

	case 2:
		u8DgainReg = 0xe4;
		break;

	case 3:
		u8DgainReg = 0xe5;
		break;

	case 4:
		u8DgainReg = 0xe7;
		break;

	default:
		u8DgainReg = 0x84;
		break;
	}


#if CMOS_SC1145_ISP_WRITE_SENSOR_ENABLE
	g_stSnsRegsInfo.astI2cData[2].u32Data = (u8AgainHigh << 4 | u8AgainLow);
	g_stSnsRegsInfo.astI2cData[3].u32Data = u8DgainReg;
#else
	sensor_write_register(0x3e09, (u8AgainHigh << 4 | u8AgainLow));
	sensor_write_register(0x3e0f, u8DgainReg);
#endif

	return;
}

HI_S32 cmos_init_ae_exp_function(AE_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
	memset(pstExpFuncs, 0, sizeof(AE_SENSOR_EXP_FUNC_S));

	pstExpFuncs->pfn_cmos_get_ae_default    = cmos_get_ae_default;
	pstExpFuncs->pfn_cmos_fps_set           = cmos_fps_set;
	pstExpFuncs->pfn_cmos_slow_framerate_set= cmos_slow_framerate_set;
	pstExpFuncs->pfn_cmos_inttime_update    = cmos_inttime_update;
	pstExpFuncs->pfn_cmos_gains_update      = cmos_gains_update;
	pstExpFuncs->pfn_cmos_again_calc_table  = cmos_again_calc_table;
	pstExpFuncs->pfn_cmos_dgain_calc_table  = cmos_dgain_calc_table;

	return 0;
}


/* AWB default parameter and function */
static AWB_CCM_S g_stAwbCcm =
{
	4850,
	{
		0x0222,  0x80e5,  0x803d,
		0x8050,  0x01cd,  0x807d,
		0x0018,  0x8183,  0x026b
	},

	3160,
	{
		0x021c,  0x806d,  0x80af,
		0x8085,  0x0209,  0x8084,
		0x8004,  0x81de,  0x02e2
	},

	2470,
	{
		0x026a,  0x8112,  0x8058,
		0x8080,  0x01b9,  0x8039,
		0x803e,  0x8409,  0x0547
	}


};

static AWB_AGC_TABLE_S g_stAwbAgcTable =
{
	/* bvalid */
	1,

	/*1,  2,  4,  8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768*/
	/* saturation */
	//{0x80,0x80,0x7a,0x78,0x70,0x68,0x60,0x58,0x50,0x48,0x40,0x38,0x38,0x38,0x38,0x38}
	{0x80,0x78,0x70,0x68,0x60,0x55,0x4b,0x40,0x38,0x38,0x38,0x38,0x38,0x38,0x38,0x38}

};

static HI_S32 cmos_get_awb_default(AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
	if (HI_NULL == pstAwbSnsDft)
	{
		printf("null pointer when get awb default value!\n");
		return -1;
	}

	memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));

	pstAwbSnsDft->u16WbRefTemp = 4850;
	pstAwbSnsDft->au16GainOffset[0] = 0x16b;
	pstAwbSnsDft->au16GainOffset[1] = 0x100;
	pstAwbSnsDft->au16GainOffset[2] = 0x100;
	pstAwbSnsDft->au16GainOffset[3] = 0x15c;
	pstAwbSnsDft->as32WbPara[0] = 134;
	pstAwbSnsDft->as32WbPara[1] = -51;
	pstAwbSnsDft->as32WbPara[2] = -172;
	pstAwbSnsDft->as32WbPara[3] = 175014;
	pstAwbSnsDft->as32WbPara[4] = 128;
	pstAwbSnsDft->as32WbPara[5] = -124140;

	memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcm, sizeof(AWB_CCM_S));
	memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTable, sizeof(AWB_AGC_TABLE_S));

	return 0;
}

HI_S32 cmos_init_awb_exp_function(AWB_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
	memset(pstExpFuncs, 0, sizeof(AWB_SENSOR_EXP_FUNC_S));

	pstExpFuncs->pfn_cmos_get_awb_default = cmos_get_awb_default;

	return 0;
}

#define DMNR_CALIB_CARVE_NUM_SC1145 7

float g_coef_calib_sc1145[DMNR_CALIB_CARVE_NUM_SC1145][4] =
{
	{100.000000f, 2.000000f, 0.036796f, 0.000000f, },
	{206.000000f, 2.313867f, 0.038276f, 0.000000f, },
	{440.000000f, 2.643453f, 0.041375f, 0.000000f, },
	{880.000000f, 2.944483f, 0.047171f, 0.000000f, },
	{918.000000f, 2.962843f, 0.047373f, 0.000000f, },
	{1322.000000f, 3.121231f, 0.051051f, 0.000000f, },
	{1599.000000f, 3.203848f, 0.055288f, 0.000000f, },
};



static ISP_NR_ISO_PARA_TABLE_S g_stNrIsoParaTab[HI_ISP_NR_ISO_LEVEL_MAX] =
{
	//u16Threshold//u8varStrength//u8fixStrength//u8LowFreqSlope
	{1500,       80,             256-256,            0 },  //100    //                      //
	{1500,       80,             256-256,            0 },  //200    // ISO                  // ISO //u8LowFreqSlope
	{1500,       80,             256-256,            0 },  //400    //{400,  1200, 96,256}, //{400 , 0  }
	{1750,       80,              256-256,           0 },  //800    //{800,  1400, 80,256}, //{600 , 2  }
	{1500,       80,             256-256,            0 },  //1600   //{1600, 1200, 72,256}, //{800 , 8  }
	{1500,       80,             256-256,            0 },  //3200   //{3200, 1200, 64,256}, //{1000, 12 }
	{1375,       65,             256-256,            0 },  //6400   //{6400, 1100, 56,256}, //{1600, 6  }
	{1375,       70,             256-256,            0 },  //12800  //{12000,1100, 48,256}, //{2400, 0  }
	{1375,       65,             256-256,            0 },  //25600  //{36000,1100, 48,256}, //
	{1375,       70,             256-256,            0 },  //51200  //{64000,1100, 96,256}, //
	{1250,       70,             256-256,            0 },  //102400 //{82000,1000,240,256}, //
	{1250,       70,             256-256,            0 },  //204800 //                           //
	{1250,       70,             256-256,            0 },  //409600 //                           //
	{1250,       70,             256-256,            0 },  //819200 //                           //
	{1250,       70,             256-256,            0 },  //1638400//                           //
	{1250,       70,             256-256,            0 },  //3276800//                           //
};

static ISP_CMOS_DEMOSAIC_S g_stIspDemosaic =
{
	/*For Demosaic*/
	1, /*bEnable*/
	12,/*u16VhLimit*/
	8,/*u16VhOffset*/
	48,   /*u16VhSlope*/
	/*False Color*/
	1,    /*bFcrEnable*/
	{ 8, 8, 8, 8, 8, 8, 8, 8, 3, 0, 0, 0, 0, 0, 0, 0},    /*au8FcrStrength[ISP_AUTO_ISO_STENGTH_NUM]*/
	{24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24},    /*au8FcrThreshold[ISP_AUTO_ISO_STENGTH_NUM]*/
	/*For Ahd*/
	400, /*u16UuSlope*/
	{512,512,512,512,512,512,512,  400,  0,0,0,0,0,0,0,0}    /*au16NpOffset[ISP_AUTO_ISO_STENGTH_NUM]*/
};

static ISP_CMOS_GE_S g_stIspGe =
{
	/*For GE*/
	0,    /*bEnable*/
	7,    /*u8Slope*/
	7,    /*u8Sensitivity*/
	4096, /*u16Threshold*/
	4096, /*u16SensiThreshold*/
	{1024,1024,1024,2048,2048,2048,2048,  2048,  2048,2048,2048,2048,2048,2048,2048,2048}    /*au16Strength[ISP_AUTO_ISO_STENGTH_NUM]*/
};

static ISP_CMOS_RGBSHARPEN_S g_stIspRgbSharpen =
{
	{   0,   0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},/* bEnLowLumaShoot */
	{  40,  40, 40, 43, 45, 48, 50, 55, 58, 60, 60, 60, 60, 60, 60, 65},/*SharpenUD*/
	{  58,  58, 60, 60, 63, 63, 65, 65, 68, 70, 70, 70, 70, 70, 70, 70},/*SharpenD*/
	{   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*TextureNoiseThd*/
	{   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*EdgeNoiseThd*/
	{ 100,  85, 80, 75, 70, 60, 55, 50, 48, 45, 45, 45, 45, 45, 45, 45},/*overshoot*/
	{ 100, 100, 95, 95, 85, 80, 75, 70, 65, 60, 60, 60, 60, 60, 60, 60},/*undershoot*/

};


static ISP_CMOS_UVNR_S g_stIspUVNR =
{
	//{100,	200,	400,	800,	1600,	3200,	6400,	12800,	25600,	51200,	102400,	204800,	409600,	819200,	1638400,	3276800};
	{1,	    2,       4,      5,      7,      48,     32,     16,     16,     16,      16,     16,     16,     16,     16,        16},      /*UVNRThreshold*/
	{0,		0,		0,		0,		0,		0,		0,		0,		0,		1,			1,		2,		2,		2,		2,		2},  /*Coring_lutLimit*/
	{0,		0,		0,		16,		34,		34,		34,		34,		34,		34,		34,		34,		34,		34,		34,			34}  /*UVNR_blendRatio*/
};

static ISP_CMOS_DPC_S g_stCmosDpc =
{
	//1,/*IR_channel*/
	//1,/*IR_position*/
	{70,150,240,248,250,252,252,252,252,252,252,252,252,252,252,252},/*au16Strength[16]*/
	{0,0,0,0,0,0,0,0,0x24,0x80,0x80,0x80,0xE5,0xE5,0xE5,0xE5},/*au16BlendRatio[16]*/
};


static ISP_CMOS_DRC_S g_stIspDrc =
{
	0,
	10,
	0,
	2,
	192,
	60,
	0,
	0,
	0,
	{1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024}
};


HI_U32 cmos_get_isp_default(ISP_CMOS_DEFAULT_S *pstDef)
{
	if (HI_NULL == pstDef)
	{
		printf("null pointer when get isp default value!\n");
		return -1;
	}

	memset(pstDef, 0, sizeof(ISP_CMOS_DEFAULT_S));

	memcpy(&pstDef->stDrc, &g_stIspDrc, sizeof(ISP_CMOS_DRC_S));
	memcpy(&pstDef->stDemosaic, &g_stIspDemosaic, sizeof(ISP_CMOS_DEMOSAIC_S));
	memcpy(&pstDef->stRgbSharpen, &g_stIspRgbSharpen, sizeof(ISP_CMOS_RGBSHARPEN_S));
	memcpy(&pstDef->stGe, &g_stIspGe, sizeof(ISP_CMOS_GE_S));
	//  pstDef->stNoiseTbl.u8SensorIndex = HI_ISP_NR_SENSOR_INDEX_OV9712;
	pstDef->stNoiseTbl.stNrCaliPara.u8CalicoefRow = DMNR_CALIB_CARVE_NUM_SC1145;
	pstDef->stNoiseTbl.stNrCaliPara.pCalibcoef    = (HI_FLOAT (*)[4])g_coef_calib_sc1145;

	memcpy(&pstDef->stNoiseTbl.stIsoParaTable[0], &g_stNrIsoParaTab[0],sizeof(ISP_NR_ISO_PARA_TABLE_S)*HI_ISP_NR_ISO_LEVEL_MAX);

	memcpy(&pstDef->stUvnr,       &g_stIspUVNR,       sizeof(ISP_CMOS_UVNR_S));
	memcpy(&pstDef->stDpc,       &g_stCmosDpc,       sizeof(ISP_CMOS_DPC_S));

	pstDef->stSensorMaxResolution.u32MaxWidth  = 1280;
	pstDef->stSensorMaxResolution.u32MaxHeight = 720;

	return 0;
}

HI_U32 cmos_get_isp_black_level(ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
	if (HI_NULL == pstBlackLevel)
	{
		printf("null pointer when get isp black level value!\n");
		return -1;
	}

	/* Don't need to update black level when iso change */
	pstBlackLevel->bUpdate = HI_FALSE;

	pstBlackLevel->au16BlackLevel[0] = 258;
	pstBlackLevel->au16BlackLevel[1] = 258;
	pstBlackLevel->au16BlackLevel[2] = 258;
	pstBlackLevel->au16BlackLevel[3] = 258;


	return 0;

}

HI_VOID cmos_set_pixel_detect(HI_BOOL bEnable)
{
	HI_U32 u32Lines = VMAX_720P30_LINEAR * 30 /5;

	return;

#if CMOS_SC1145_ISP_WRITE_SENSOR_ENABLE
	if (bEnable) /* setup for ISP pixel calibration mode */
	{
		/* 5 fps */
		sensor_write_register(0x320e, (u32Lines >> 4) && 0xFF);
		sensor_write_register(0x320f, ((u32Lines<<4)&&0xF0));

		/* max exposure time*/


	}
	else /* setup for ISP 'normal mode' */
	{
		sensor_write_register(0x320e, (gu32FullLinesStd >> 8) && 0XFF);
		sensor_write_register(0x320f, gu32FullLinesStd && 0xFF);

		bInit = HI_FALSE;
	}
#else
	if (bEnable) /* setup for ISP pixel calibration mode */
	{

		sensor_write_register(0x3e01, (u32Lines >> 8) && 0xFF);
		sensor_write_register(0x3e02, (u32Lines - 4) && 0xFF);

		/* min gain */
		sensor_write_register(0x3e0e, 0x00);
		sensor_write_register(0x3e0f, 0x00);

		/* 5 fps */
		sensor_write_register(0x320e, (u32Lines >> 8) && 0xFF);
		sensor_write_register(0x320f, u32Lines && 0xFF);
	}
	else /* setup for ISP 'normal mode' */
	{
		sensor_write_register(0x320e, (gu32FullLinesStd >> 8) && 0XFF);
		sensor_write_register(0x320f, gu32FullLinesStd && 0xFF);

		bInit = HI_FALSE;
	}
#endif

	return;
}

HI_VOID cmos_set_wdr_mode(HI_U8 u8Mode)
{
	bInit = HI_FALSE;

	switch(u8Mode)
	{
	case WDR_MODE_NONE:
		if (SENSOR_720P_30FPS_MODE == gu8SensorImageMode)
		{
			gu32FullLinesStd = VMAX_SC1145_720P30_LINEAR;
		}
		genSensorMode = WDR_MODE_NONE;
		printf("linear mode\n");
		break;
	default:
		printf("NOT support this mode!\n");
		return;
		break;
	}

	return;
}

HI_U32 cmos_get_sns_regs_info(ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
#if CMOS_SC1145_ISP_WRITE_SENSOR_ENABLE

	HI_S32 i;

	if (HI_FALSE == bInit)
	{
		g_stSnsRegsInfo.enSnsType = ISP_SNS_I2C_TYPE;
		g_stSnsRegsInfo.u8Cfg2ValidDelayMax = 2;
		g_stSnsRegsInfo.u32RegNum = 6;

		for (i=0; i<g_stSnsRegsInfo.u32RegNum; i++)
		{
			g_stSnsRegsInfo.astI2cData[i].bUpdate = HI_TRUE;
			g_stSnsRegsInfo.astI2cData[i].u8DevAddr = sensor_i2c_addr;
			g_stSnsRegsInfo.astI2cData[i].u32AddrByteNum = sensor_addr_byte;
			g_stSnsRegsInfo.astI2cData[i].u32DataByteNum = sensor_data_byte;
		}
		g_stSnsRegsInfo.astI2cData[0].u8DelayFrmNum = 0;
		g_stSnsRegsInfo.astI2cData[0].u32RegAddr = 0x3e01;     //exp high  bit[7:0]
		g_stSnsRegsInfo.astI2cData[1].u8DelayFrmNum = 0;
		g_stSnsRegsInfo.astI2cData[1].u32RegAddr = 0x3e02;     //exp low  bit[7:4]
		g_stSnsRegsInfo.astI2cData[2].u8DelayFrmNum = 0;
		g_stSnsRegsInfo.astI2cData[2].u32RegAddr = 0x3e09;     //analog agin
		g_stSnsRegsInfo.astI2cData[3].u8DelayFrmNum = 2;
		g_stSnsRegsInfo.astI2cData[3].u32RegAddr = 0x3e0f;     //digita agin

		g_stSnsRegsInfo.astI2cData[4].u8DelayFrmNum = 0;
		g_stSnsRegsInfo.astI2cData[4].u32RegAddr = 0x320e;     //TIMING_VTS  high bit[3:0]
		g_stSnsRegsInfo.astI2cData[5].u8DelayFrmNum = 0;
		g_stSnsRegsInfo.astI2cData[5].u32RegAddr = 0x320f;     //TIMING_VTS  low bit[7:0]


		bInit = HI_TRUE;
	}
	else
	{
		for (i=0; i<g_stSnsRegsInfo.u32RegNum; i++)
		{
			if (g_stSnsRegsInfo.astI2cData[i].u32Data == g_stPreSnsRegsInfo.astI2cData[i].u32Data)
			{
				g_stSnsRegsInfo.astI2cData[i].bUpdate = HI_FALSE;
			}
			else
			{
				g_stSnsRegsInfo.astI2cData[i].bUpdate = HI_TRUE;
			}
		}
	}

	if (HI_NULL == pstSnsRegsInfo)
	{
		printf("null pointer when get sns reg info!\n");
		return -1;
	}

	memcpy(pstSnsRegsInfo, &g_stSnsRegsInfo, sizeof(ISP_SNS_REGS_INFO_S));
	memcpy(&g_stPreSnsRegsInfo, &g_stSnsRegsInfo, sizeof(ISP_SNS_REGS_INFO_S));

#endif

	return 0;
}



static HI_S32 cmos_set_image_mode(ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode)
{
	HI_U8 u8SensorImageMode = gu8SensorImageMode;

	bInit = HI_FALSE;

	if (HI_NULL == pstSensorImageMode )
	{
		printf("null pointer when set image mode\n");
		return -1;
	}

	if ((pstSensorImageMode->u16Width <= 1280) && (pstSensorImageMode->u16Height <= 720))
	{
		if (WDR_MODE_NONE == genSensorMode)
		{
			if (pstSensorImageMode->f32Fps <= 30)
			{
				u8SensorImageMode = SENSOR_720P_30FPS_MODE;
			}
			else
			{
				printf("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
						pstSensorImageMode->u16Width,
						pstSensorImageMode->u16Height,
						pstSensorImageMode->f32Fps,
						genSensorMode);

				return -1;
			}
		}
		else
		{
			printf("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
					pstSensorImageMode->u16Width,
					pstSensorImageMode->u16Height,
					pstSensorImageMode->f32Fps,
					genSensorMode);

			return -1;
		}
	}
	else
	{
		printf("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
				pstSensorImageMode->u16Width,
				pstSensorImageMode->u16Height,
				pstSensorImageMode->f32Fps,
				genSensorMode);

		return -1;
	}

	/* Sensor first init */
	if (HI_FALSE == bSensorInit)
	{
		gu8SensorImageMode = u8SensorImageMode;

		return 0;
	}

	/* Switch SensorImageMode */
	if (u8SensorImageMode == gu8SensorImageMode)
	{
		/* Don't need to switch SensorImageMode */
		return -1;
	}

	gu8SensorImageMode = u8SensorImageMode;

	return 0;
}

HI_VOID sensor_global_init()
{
	gu8SensorImageMode = SENSOR_720P_30FPS_MODE;
	genSensorMode = WDR_MODE_NONE;
	gu32FullLinesStd = VMAX_SC1145_720P30_LINEAR;
	gu32FullLines = VMAX_SC1145_720P30_LINEAR;
	bInit = HI_FALSE;
	bSensorInit = HI_FALSE;

	memset(&g_stSnsRegsInfo, 0, sizeof(ISP_SNS_REGS_INFO_S));
	memset(&g_stPreSnsRegsInfo, 0, sizeof(ISP_SNS_REGS_INFO_S));
}

HI_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
	memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

	pstSensorExpFunc->pfn_cmos_sensor_init = sensor_init;
	//pstSensorExpFunc->pfn_cmos_sensor_exit = sensor_exit;
	pstSensorExpFunc->pfn_cmos_sensor_global_init = sensor_global_init;
	pstSensorExpFunc->pfn_cmos_set_image_mode = cmos_set_image_mode;
	pstSensorExpFunc->pfn_cmos_set_wdr_mode = cmos_set_wdr_mode;

	pstSensorExpFunc->pfn_cmos_get_isp_default = cmos_get_isp_default;
	pstSensorExpFunc->pfn_cmos_get_isp_black_level = cmos_get_isp_black_level;
	pstSensorExpFunc->pfn_cmos_set_pixel_detect = cmos_set_pixel_detect;
	pstSensorExpFunc->pfn_cmos_get_sns_reg_info = cmos_get_sns_regs_info;

	return 0;
}

/****************************************************************************
 * callback structure                                                       *
 ****************************************************************************/

int sensor_register_callback(void)
{
	ISP_DEV IspDev = 0;
	HI_S32 s32Ret;
	ALG_LIB_S stLib;
	ISP_SENSOR_REGISTER_S stIspRegister;
	AE_SENSOR_REGISTER_S  stAeRegister;
	AWB_SENSOR_REGISTER_S stAwbRegister;

	cmos_init_sensor_exp_function(&stIspRegister.stSnsExp);
	s32Ret = HI_MPI_ISP_SensorRegCallBack(IspDev, SC1145_ID, &stIspRegister);
	if (s32Ret)
	{
		printf("sensor register callback function failed!\n");
		return s32Ret;
	}

	stLib.s32Id = 0;
	strncpy(stLib.acLibName, HI_AE_LIB_NAME, sizeof(HI_AE_LIB_NAME));
	cmos_init_ae_exp_function(&stAeRegister.stSnsExp);
	s32Ret = HI_MPI_AE_SensorRegCallBack(IspDev, &stLib, SC1145_ID, &stAeRegister);
	if (s32Ret)
	{
		printf("sensor register callback function to ae lib failed!\n");
		return s32Ret;
	}

	stLib.s32Id = 0;
	strncpy(stLib.acLibName, HI_AWB_LIB_NAME, sizeof(HI_AWB_LIB_NAME));
	cmos_init_awb_exp_function(&stAwbRegister.stSnsExp);
	s32Ret = HI_MPI_AWB_SensorRegCallBack(IspDev, &stLib, SC1145_ID, &stAwbRegister);
	if (s32Ret)
	{
		printf("sensor register callback function to ae lib failed!\n");
		return s32Ret;
	}

	return 0;
}

int sensor_unregister_callback(void)
{
	ISP_DEV IspDev = 0;
	HI_S32 s32Ret;
	ALG_LIB_S stLib;

	s32Ret = HI_MPI_ISP_SensorUnRegCallBack(IspDev, SC1145_ID);
	if (s32Ret)
	{
		printf("sensor unregister callback function failed!\n");
		return s32Ret;
	}

	stLib.s32Id = 0;
	strncpy(stLib.acLibName, HI_AE_LIB_NAME, sizeof(HI_AE_LIB_NAME));
	s32Ret = HI_MPI_AE_SensorUnRegCallBack(IspDev, &stLib, SC1145_ID);
	if (s32Ret)
	{
		printf("sensor unregister callback function to ae lib failed!\n");
		return s32Ret;
	}

	stLib.s32Id = 0;
	strncpy(stLib.acLibName, HI_AWB_LIB_NAME, sizeof(HI_AWB_LIB_NAME));
	s32Ret = HI_MPI_AWB_SensorUnRegCallBack(IspDev, &stLib, SC1145_ID);
	if (s32Ret)
	{
		printf("sensor unregister callback function to ae lib failed!\n");
		return s32Ret;
	}

	return 0;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __MN34220_CMOS_H_ */
