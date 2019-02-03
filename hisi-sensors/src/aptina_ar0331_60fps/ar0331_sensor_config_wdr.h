#ifndef SENSOR_CONFIG_WDR_H_                    
#define SENSOR_CONFIG_WDR_H_                    
                                                
static int sensor_rom_hdr[] = {                 
//[HiSPi HDR 1080p60]

	0xFFFE00C8, //DELAY 200ms
	0x301A0001, //RESET_REGISTER, 0x0001
	0x301A10D8, //RESET_REGISTER, 0x10D8
	0xFFFE00C8, //DELAY 200ms
//LOAD = AR0331 HDR sequencer load - 3.0
	0x30888000,
	0x30864540,
	0x30866134,
	0x30864A31,
	0x30864342,
	0x30864560,
	0x30862714,
	0x30863DFF,
	0x30863DFF,
	0x30863DEA,
	0x30862704,
	0x30863D10,
	0x30862705,
	0x30863D10,
	0x30862715,
	0x30863527,
	0x3086053D,
	0x30861045,
	0x30864027,
	0x30860427,
	0x3086143D,
	0x3086FF3D,
	0x3086FF3D,
	0x3086EA62,
	0x30862728,
	0x30863627,
	0x3086083D,
	0x30866444,
	0x30862C2C,
	0x30862C2C,
	0x30864B00,
	0x3086432D,
	0x30866343,
	0x30861664,
	0x3086435F,
	0x30864F50,
	0x30862604,
	0x30862694,
	0x308627FC,
	0x3086530D,
	0x30865C0D,
	0x30865754,
	0x30861709,
	0x30865556,
	0x30864953,
	0x30860753,
	0x3086034D,
	0x3086286C,
	0x30864C09,
	0x3086282C,
	0x30862828,
	0x3086261C,
	0x30864E5C,
	0x30860960,
	0x30864500,
	0x30864580,
	0x308626BE,
	0x308627F8,
	0x30861702,
	0x308627FA,
	0x30865C0B,
	0x30861712,
	0x308626BA,
	0x30865C03,
	0x30861713,
	0x308627F2,
	0x3086171C,
	0x30865F28,
	0x30860817,
	0x30860360,
	0x3086173C,
	0x308626B2,
	0x30861616,
	0x30865F4D,
	0x30861926,
	0x30869316,
	0x30861627,
	0x3086FA45,
	0x3086A017,
	0x30860527,
	0x3086FB17,
	0x30861F45,
	0x3086801F,
	0x30861705,
	0x308627FA,
	0x3086171E,
	0x30865D17,
	0x30860C26,
	0x30869248,
	0x30864D4E,
	0x3086269A,
	0x30862808,
	0x30864C0B,
	0x30866017,
	0x30860327,
	0x3086F217,
	0x30862626,
	0x30869216,
	0x3086165F,
	0x30864D19,
	0x30862693,
	0x30861616,
	0x308627FA,
	0x30862643,
	0x30865C01,
	0x30864540,
	0x30862798,
	0x30861720,
	0x30864A65,
	0x30864316,
	0x30866643,
	0x30865A43,
	0x3086165B,
	0x30864327,
	0x30869C45,
	0x30866017,
	0x30860627,
	0x30869D17,
	0x30861C45,
	0x30864023,
	0x30861705,
	0x30862798,
	0x30865D26,
	0x30864417,
	0x30860E28,
	0x30860053,
	0x3086014B,
	0x30865251,
	0x30861244,
	0x30864B01,
	0x3086432D,
	0x30864643,
	0x30861647,
	0x3086434F,
	0x30865026,
	0x30860426,
	0x30868427,
	0x3086FC53,
	0x30860D5C,
	0x30860D57,
	0x30865417,
	0x30860955,
	0x30865649,
	0x30865307,
	0x30865303,
	0x30864D28,
	0x30866C4C,
	0x30860928,
	0x30862C28,
	0x30862826,
	0x30860C4E,
	0x30865C09,
	0x30866045,
	0x30860045,
	0x30868026,
	0x3086AE27,
	0x3086F817,
	0x30860227,
	0x3086FA5C,
	0x30860B17,
	0x30861226,
	0x3086AA5C,
	0x30860317,
	0x30860B27,
	0x3086F217,
	0x3086265F,
	0x30862808,
	0x30861703,
	0x30866017,
	0x30860326,
	0x3086A216,
	0x3086165F,
	0x30864D1A,
	0x30862683,
	0x30861616,
	0x308627FA,
	0x308645A0,
	0x30861705,
	0x308627FB,
	0x3086171F,
	0x30864580,
	0x30862017,
	0x30860527,
	0x3086FA17,
	0x30861E5D,
	0x3086170C,
	0x30862682,
	0x3086484D,
	0x30864E26,
	0x30868A28,
	0x3086084C,
	0x30860B60,
	0x30861707,
	0x308627F2,
	0x30861738,
	0x30862682,
	0x30861616,
	0x30865F4D,
	0x30861A26,
	0x30868316,
	0x30861627,
	0x3086FA26,
	0x3086435C,
	0x30860145,
	0x30864027,
	0x30869817,
	0x30861F4A,
	0x30861244,
	0x30860343,
	0x30861604,
	0x30864316,
	0x30865843,
	0x30861659,
	0x30864316,
	0x3086279C,
	0x30864560,
	0x30861705,
	0x3086279D,
	0x3086171D,
	0x30864540,
	0x30862217,
	0x30860527,
	0x3086985D,
	0x30862645,
	0x3086170E,
	0x30862800,
	0x30865301,
	0x30864B52,
	0x30865112,
	0x30864460,
	0x30862C2C,
	0x30862C2C,
//	
	0x301A0058, 	//RESET_REGISTER, 0x0058	
	0x30B00000, 	//DIGITAL_TEST, 0x0000		
	0x30BA07EC, 	//DIGITAL_CTRL, 0x07EC		
	0x31AC100C, 	//DATA_FORMAT_BITS, 0x100C	
//LOAD= PLL_settings - 4 Lane 12-bit HiSPi	
	0x302A0006, 	//VT_PIX_CLK_DIV, 0x0006 
	0x302C0001, 	//VT_SYS_CLK_DIV, 0x0001 
	0x302E0004, 	//PRE_PLL_CLK_DIV, 0x0004
	0x30300042, 	//PLL_MULTIPLIER, 0x0042 
	0x3036000C, 	//OP_PIX_CLK_DIV, 0x000C 
	0x30380001, 	//OP_SYS_CLK_DIV, 0x0001 
//
	0x300200E4, 	//Y_ADDR_START, 0x00E4				
	0x30040042, 	//X_ADDR_START, 0x0042				
	0x30060523, 	//Y_ADDR_END, 0x0523				
	0x300807C9, 	//X_ADDR_END, 0x07C9				
	0x300A0465, 	//FRAME_LENGTH_LINES, 0x0465		
	0x300C044C, 	//LINE_LENGTH_PCK, 0x044C			
	0x30120409, 	//COARSE_INTEGRATION_TIME, 0x0409	
	0x30A20001, 	//X_ODD_INC, 0x0001 				
	0x30A60001, 	//Y_ODD_INC, 0x0001 				
	0x30400000, 	//READ_MODE, 0x0000 				
//LOAD= HDR Mode 16x
	0x30820008, 	//OPERATION_MODE_CTRL, 0x0008
	0x305E0080, 	//GLOBAL_GAIN, 0x0080
	//LOAD= HDR Mode Setup
	0xFFFE003C, //DELAY 60ms	
	//LOAD= 2D motion compensation ON
	0x318CC001, 	//HDR_MC_CTRL2, 0xC001	
	0x31982050, 	//HDR_MC_CTRL8, 0x061E	
	0x30FE0000, 	//NOISE_PEDESTAL, 0x0000
	0x320A0000, 	//ADACD_PEDESTAL, 0x0000
	//LOAD= ALTM Bypassed
	0x301A0058, 	// Stop Streaming
	0x24000003,	//ALTM_CONTROL, 0x0003
	0x24500000,  //ALTM_OUT_PEDESTAL, 0x0000
	0x301E00A8,  //DATA_PEDESTAL, 0x00A8	
	0x301A005C, 	// Enable Streaming
	//LOAD= ADACD Enabled
	0x320200CF, 	// REG= 0x3202, 0x00CF		  
	0x32060A06, 	// REG= 0x3206, 0x0A06		  
	0x32081A12, 	// REG= 0x3208, 0x1A12		  
	0x32000002, 	//ADACD_CONTROL, 0x0002
	//
	0x31E00200, 	//PIX_DEF_ID, 0x200 	
	0x30600006, 	//ANALOG_GAIN, 0x0006	
	0x318A0E10, 	//HDR_MC_CTRL1, 0x0E10	
	//LOAD= Disable Embedded Data and Stats
	0x30641802, 	//SMIA_TEST, EMBEDDED_STATS_EN, 0x0000
	0x30641802, 	//SMIA_TEST, EMBEDDED_DATA, 0x0000 
	//
	0x301A005E, 	
	//LOAD= ADACD Enabled
	0x320200CF, 	// ADACD_NOISE_MODEL1
	0x32060A06,	// ADACD_NOISE_FLOOR1
	0x32081A12, 	// ADACD_NOISE_FLOOR2
	0x32000002, 	// ADACD_CONTROL
//
	0x31AE0304, 	//SERIAL_FORMAT, 0x0304 	
	0x31C60400,   // HISPI_CONTROL_STATUS  PACKTIZED SP
	0x306E9210, 	//DATAPATH_SELECT VDD_SLVS = VDDIO = 1.8V
//LOAD= Analog Settings
	0x31808089, 	//DELTA_DK_CONTROL, 0x8089
	0x30F44000,
	0x3ED48F6C,
	0x3ED66666,
	0x3EDA8899,
	0x3EE600F0,
//
	0x3ED29F46, 	//DAC_LD_6_7, 0x9F46	

//adjust lvds timing 
	0x31C00603,
	0xFFFE00C8,	//DELAY 200ms
	//the register 0x31C0 need to be modified depending on hardware or logic.
	//the value is 0x8603 in hisilicon REF board.
	//the value is 0x80D8 in hisilicon DEMO board.
	0x31C08603,	//

//tempsens_ctrl
	0x30B40051,

	0x301A005C, 	//RESET_REGISTER, 0x005C

	0xFFFFFFFF		// END

};                                                   
                                                     
#endif /*SENSOR_CONFIG_WDR_H_*/ 
