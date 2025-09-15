/* *********************************************************************************************************************************
888b     d888 88888888888 .d8888b.   .d8888b.  888888888   .d8888b.  888b    888  .d8888b.  888      8888888b.   
8888b   d8888     888    d88P  Y88b d88P  Y88b 888        d88P  Y88b 8888b   888 d88P  Y88b 888      888   Y88b  
88888b.d88888     888    Y88b. d88P 888    888 888               888 88888b  888 Y88b.      888      888    888  
888Y88888P888     888     "Y88888"  Y88b. d888 8888888b.       .d88P 888Y88b 888  "Y888b.   888      888   d88P  
888 Y888P 888     888    .d8P""Y8b.  "Y888P888      "Y88b  .od888P"  888 Y88b888     "Y88b. 888      8888888P"   
888  Y8P  888     888    888    888        888        888 d88P"      888  Y88888       "888 888      888 T88b    
888   "   888     888    Y88b  d88P Y88b  d88P Y88b  d88P 888"       888   Y8888 Y88b  d88P 888      888  T88b   
888       888     888     "Y8888P"   "Y8888P"   "Y8888P"  888888888  888    Y888  "Y8888P"  88888888 888   T88b  
   ********************************************************************************************************************************* */
#include "stdafx.h"
#include "math.h"
#define SITENUM	4
#define MT3552NSLR
//#define MT8952NSLR

short DebugConsole[SITENUM] = { 1, 0, 0, 0 }, site = 0, flag[SITENUM] = { 0, 0, 0, 0 };
double adresult[SITENUM] = { 0.0f };
double adresult1[SITENUM][1000] = { 0.0f };

BYTE sitesta[SITENUM] = { 0 };
/* ************************************** LaserTrimming, VersionTrim ********************************** */
int	TBit1[SITENUM]  = { 0, 0, 0, 0 };		// 0: 720Khz
																				// 1: 1.5Mhz
int TBit2[SITENUM]  = { 0, 0, 0, 0 };   // 0: PFM
																				// 1: FPWM
int TBit3[SITENUM]  = { 0, 0, 0, 0 };   // 0: Vref=0.8v
																				// 1: Vref=0.6v
double IEN1H[SITENUM] = { 999.0 }, IEN2L[SITENUM] ={ 999.0 };
/* **************************************************************************************************** */
// ******************************************************************* VI SOURCE DEFINITION *******************************************************************
// FPVI
FPVI10 VinFPVI0			(0,	 "VIN");				// VIN, PIN1
FPVI10 SwFPVI1			(1,  "SW");					// SW,	PIN2
// FOVI
FOVI EnFOVI					(0,  "EN");					// EN,  PIN5
FOVI BST_FOVI				(2,  "BST");				// BST, PIN4
FOVI FB_FOVI				(3,  "FB");					// FB,  PIN6
FOVI SiteChk				(6,  "SITECHECK");	// SITE No.
FOVI BST2_FOVI			(7,  "BST-SW");			// BST-SW
// OTHER
QTMU_PLUS qtmu0			(0);
CBIT128 rlyC;

DUT_API void HardWareCfg()	{
	/*For example: four channels dvi to config two sites*/
	/*StsSetModuleToSite(MD_DVI400, SITE_1, 0, 1, -1);
		StsSetModuleToSite(MD_DVI400, SITE_2, 2, 3, -1);*/

	// FPVI
	StsSetModuleToSite(MD_FPVI10, SITE_1, 0, 1, -1);	// SITE1, IDC1
	StsSetModuleToSite(MD_FPVI10, SITE_2, 2, 3, -1);	// SITE2, IDC2
	StsSetModuleToSite(MD_FPVI10, SITE_3, 4, 5, -1);	// SITE3, IDC3
	StsSetModuleToSite(MD_FPVI10, SITE_4, 6, 7, -1);	// SITE4, IDC4
	// FOVI
	StsSetModuleToSite(MD_FOVI, SITE_1,  0,		 1,		2,	 3,		4,	 5,   6,   7, -1);
	StsSetModuleToSite(MD_FOVI, SITE_2,  8,		 9,  10,	11,  12,  13,  14,  15, -1);
	StsSetModuleToSite(MD_FOVI, SITE_3,  16,  17,  18,	19,  20,  21,  22,  23, -1);
	StsSetModuleToSite(MD_FOVI, SITE_4,  24,  25,  26,	27,  28,  29,  30,  31, -1);
	
	// QTMU
	StsSetModuleToSite(MD_QTMUPLUS, SITE_1, 0, -1);
	StsSetModuleToSite(MD_QTMUPLUS, SITE_2, 1, -1);
	StsSetModuleToSite(MD_QTMUPLUS, SITE_3, 2, -1);
	StsSetModuleToSite(MD_QTMUPLUS, SITE_4, 3, -1);
}

// ******************************************* USER ROUTINE *******************************************
// ******************************************* USER ROUTINE *******************************************
// ******************************************* USER ROUTINE *******************************************
void VIRelay(void)	{
	rlyC.SetOn(VinFPVI, SwFPVI, ENFOVI, BstFOVI, FbFOVI, -1);
	delay_ms(1);
}
void VIRelayWCAP(void)	{
	rlyC.SetOn(VinFPVI, SwFPVI, ENFOVI, BstFOVI, FbFOVI,
						 CapVIN, -1);
	delay_ms(1);
}
void PWR0(void)	{
	VinFPVI0.Set		(FV,	float(0), FPVI10_5V, FPVI10_10MA, RELAY_ON);
	SwFPVI1.Set			(FV,	float(0), FPVI10_5V, FPVI10_10MA, RELAY_ON);
	EnFOVI.Set			(FV,  float(0), FOVI_5V,	 FOVI_10MA,		RELAY_ON);
	BST_FOVI.Set		(FV,  float(0), FOVI_5V,	 FOVI_10MA,		RELAY_ON);
	FB_FOVI.Set			(FV,  float(0), FOVI_5V,	 FOVI_10MA,		RELAY_ON);
}
void PWROFF(void)	{
	VinFPVI0.Set		(FV,	float(0), FPVI10_5V, FPVI10_10MA, RELAY_OFF);
	SwFPVI1.Set			(FV,	float(0), FPVI10_5V, FPVI10_10MA, RELAY_OFF);
	EnFOVI.Set			(FV,  float(0), FOVI_5V,	 FOVI_10MA,		RELAY_OFF);
	BST_FOVI.Set		(FV,  float(0), FOVI_5V,	FOVI_10MA,		RELAY_OFF);
	FB_FOVI.Set			(FV,  float(0), FOVI_5V,	FOVI_10MA,		RELAY_OFF);
}
void FreshSiteFlagInit(void)	{
	for (site = 0; site < SITENUM; site++) 
		flag[site] = 1;
  StsGetSiteStatus(sitesta, SITENUM);
  for (site = 0; site < SITENUM; site++) {
		if (sitesta[site]) 
			flag[site] = 0;
  }
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//initialize function will be called before all the test functions.
DUT_API void InitBeforeTestFlow()	{
 	qtmu0.Init();
	fprintf(stderr, "-----------------------------------------------------\n");
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//initializefunction will be called after all the test functions.
DUT_API void InitAfterTestFlow()	{		
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//Fail site hardware set function will be called after failed params, it can be called for serveral times. 
DUT_API void SetupFailSite(const unsigned char*byFailSite)	{			
}
// test0, SiteNoChk
// ****************************************** SiteNoChk ******************************************
// ****************************************** SiteNoChk ******************************************
// ****************************************** SiteNoChk ******************************************
DUT_API int SiteNoChk(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
    CParam *SiteCheck = StsGetParam(funcindex,"SiteCheck");
  //}}AFX_STS_PARAM_PROTOTYPES

  // TODO: Add your function code here
  SiteChk.Set(FI, 1.0e-3f, FOVI_5V, FOVI_1MA, RELAY_ON);
	delay_ms(3);
	SiteChk.MeasureVI(20 , 10);
	
	for(site = 0; site < SITENUM; site++ )	{
		adresult[site]=SiteChk.GetMeasResult(site, MVRET);
		SiteCheck->SetTestResult(site, 0, adresult[site] / (site + 1));
	}

	SiteChk.Set(FI, 0, FOVI_5V,FOVI_1MA,RELAY_ON);
	delay_ms(1);
	SiteChk.Set(FV, 0, FOVI_5V,FOVI_1MA,RELAY_ON, 5);
	SiteChk.Set(FV, 0, FOVI_5V,FOVI_1MA,RELAY_OFF);
	delay_ms(1);

  return 0;
}

// test1, ContactChk
// ****************************************** ContactChk ******************************************
// ****************************************** ContactChk ******************************************
// ****************************************** ContactChk ******************************************
DUT_API int ContactChk(short funcindex, LPCTSTR funclabel)	{
  //{{AFX_STS_PARAM_PROTOTYPES
    CParam *RCont = StsGetParam(funcindex,"RCont");
  //}}AFX_STS_PARAM_PROTOTYPES

  // TODO: Add your function code here
  return 0;
}
// test2, Continuity
// ****************************************** Continuity ******************************************
// ****************************************** Continuity ******************************************
// ****************************************** Continuity ******************************************
DUT_API int Continuity(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
    CParam *VIN = StsGetParam(funcindex,"VIN");
    CParam *SW = StsGetParam(funcindex,"SW");
    CParam *BST = StsGetParam(funcindex,"BST");
    CParam *EN = StsGetParam(funcindex,"EN");
    CParam *FB = StsGetParam(funcindex,"FB");
    CParam *SWVIN = StsGetParam(funcindex,"SWVIN");
	//}}AFX_STS_PARAM_PROTOTYPES

	// TODO: Add your function code here
  rlyC.SetOn(-1);
 	delay_ms(1);

	PWR0();
	// RELAY SETTING
	rlyC.SetOn(VinFPVI, SwFPVI, ENFOVI, BstFOVI, FbFOVI, -1);
	delay_ms(1);
	
 	// VIN
 	VinFPVI0.Set(FI, -1000.0e-6, FPVI10_2V, FPVI10_1MA, RELAY_ON);
 	delay_ms(1);
 	VinFPVI0.MeasureVI(10, 10);
 	for	(site = 0; site < SITENUM; site++)
		VIN->SetTestResult(site, 0, VinFPVI0.GetMeasResult(site, MVRET));
 	VinFPVI0.Set(FV, float(0), FPVI10_2V, FPVI10_1MA, RELAY_ON);
 	// SW
 	SwFPVI1.Set(FI, -1000.0e-6, FPVI10_2V, FPVI10_1MA, RELAY_ON);
 	delay_ms(1);
 	SwFPVI1.MeasureVI(10, 10);
 	for	(site = 0; site < SITENUM; site++)
		SW->SetTestResult(site, 0, SwFPVI1.GetMeasResult(site, MVRET));
 	SwFPVI1.Set(FV, float(0), FPVI10_2V, FPVI10_1MA, RELAY_ON);
 	// BST
 	BST_FOVI.Set(FI, -1000.0e-6, FOVI_2V, FOVI_1MA, RELAY_ON);
 	delay_ms(1);
 	BST_FOVI.MeasureVI(10, 10);
 	for	(site = 0; site < SITENUM; site++)
		BST->SetTestResult(site, 0, BST_FOVI.GetMeasResult(site, MVRET));
 	BST_FOVI.Set(FV, float(0), FOVI_2V, FOVI_1MA, RELAY_ON);
 	// EN
 	EnFOVI.Set(FI, -200.0e-6, FOVI_2V, FOVI_1MA, RELAY_ON);
 	delay_ms(1);
 	EnFOVI.MeasureVI(10, 10);
 	for	(site = 0; site < SITENUM; site++)
		EN->SetTestResult(site, 0, EnFOVI.GetMeasResult(site, MVRET));
 	EnFOVI.Set(FV, float(0), FOVI_2V, FOVI_1MA, RELAY_ON);
 	// FB
 	FB_FOVI.Set(FI, -1000.0e-6, FOVI_2V, FOVI_1MA, RELAY_ON);
 	delay_ms(1);
 	FB_FOVI.MeasureVI(10, 10);
 	for	(site = 0; site < SITENUM; site++)
		FB->SetTestResult(site, 0, FB_FOVI.GetMeasResult(site, MVRET));
 	FB_FOVI.Set(FV, float(0), FOVI_2V, FOVI_1MA, RELAY_ON);

	// HIGH SIDE MOS
	rlyC.SetOn(VinFPVI, SwFPVI, -1);
	delay_ms(1);
	// VIN-SW(SW FORCE VOLT)
	VinFPVI0.Set(FV, float(0), FPVI10_2V, FPVI10_10MA, RELAY_ON);
	SwFPVI1.Set(FV, float(2), FPVI10_2V, FPVI10_1MA, RELAY_ON);
 	delay_ms(1);
 	SwFPVI1.MeasureVI(10, 10);
 	for	(site = 0; site < SITENUM; site++)
		SWVIN->SetTestResult(site, 0, SwFPVI1.GetMeasResult(site, MVRET));
 	SwFPVI1.Set(FV, float(0), FPVI10_2V, FPVI10_10MA, RELAY_ON);

  rlyC.SetOn(-1);
 	delay_ms(1);

	return 0;
}
// test3, SupplyCurrent
// ****************************************** SupplyCurrent ******************************************
// ****************************************** SupplyCurrent ******************************************
// ****************************************** SupplyCurrent ******************************************
DUT_API int SupplyCurrent(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
    CParam *Shutdown = StsGetParam(funcindex,"Shutdown");
    CParam *Quiescent = StsGetParam(funcindex,"Quiescent");
	//}}AFX_STS_PARAM_PROTOTYPES
	double IINSD[SITENUM] = { 999.0f }, IINQ[SITENUM] = { 999.0f };
	double ii = 0;

	// TODO: Add your function code here
  rlyC.SetOn(-1);
 	delay_ms(1);
	FreshSiteFlagInit();

	rlyC.SetOn(VinFPVI, ENFOVI, -1);
	delay_ms(2);
	
	// VIN = 12
	for(ii = float(0); ii <= float(12); ii += 1000e-3)
		VinFPVI0.Set(FV, float(ii), FPVI10_20V, FPVI10_100MA, RELAY_ON);
	VinFPVI0.Set(FV, float(12.0), FPVI10_20V, FPVI10_100MA, RELAY_ON, 2);
	// EN
	EnFOVI.Set(FV, float(0.0), FOVI_5V,	FOVI_100MA, RELAY_ON);
	delay_ms(1);

	// ************************************** IINSD VIN=12V **************************************
	// ************************************** IINSD VIN=12V **************************************
	// ************************************** IINSD VIN=12V **************************************
	VinFPVI0.MeasureVI(500, 2);
	for(site = 0; site < SITENUM; site++)
		IINSD[site] = fabs(VinFPVI0.GetMeasResult(site, MIRET));
	// ************************************** IQ VIN=12V **************************************
	// ************************************** IQ VIN=12V **************************************
	// ************************************** IQ VIN=12V **************************************
	rlyC.SetOn(VinFPVI, ENFOVI, FbFOVI, SwFPVI, -1);
	delay_ms(2);
	// EN = 5
	EnFOVI.Set(FV, float(5.0), FOVI_5V,	FOVI_100MA, RELAY_ON);
	// FB
	FB_FOVI.Set(FV, float(0.85), FOVI_5V,	FOVI_10MA, RELAY_ON);
	delay_ms(1);
	SwFPVI1.Set(FI, float(-0.1e-3), FPVI10_20V, FPVI10_100MA, RELAY_ON);
	delay_ms(1);

	VinFPVI0.MeasureVI(500, 2);
	for(site = 0; site < SITENUM; site++)
		IINQ[site] = VinFPVI0.GetMeasResult(site, MIRET);

	// OFFSET
	for(site = 0; site < SITENUM; site++)	{
		IINSD[site] += float(0);
		IINQ[site] += float(0);
	}

	for(site = 0; site < SITENUM; site++)	{
		Shutdown	->SetTestResult(site, 0, fabs(IINSD[site])*1e6);
		Quiescent	->SetTestResult(site, 0, fabs(IINQ[site])*1e6);
		if(DebugConsole[site])	{	
			fprintf(stderr, "ISD_S%d		: %3.4f\n", site + 1, IINSD[site]*1e6);
			fprintf(stderr, "IQ_S%d		: %3.4f\n", site + 1, IINQ[site]*1e6);
		}
	}
	// PowerDownReset
	PWR0();
	PWROFF();
	rlyC.SetOn(-1);
 	delay_ms(1);

	return 0;
}
// test4, HSide
// ****************************************** HSMOS ******************************************
// ****************************************** HSMOS ******************************************
// ****************************************** HSMOS ******************************************
DUT_API int HSide(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
    CParam *ILK1 = StsGetParam(funcindex,"ILK1");
    CParam *ILK2 = StsGetParam(funcindex,"ILK2");
	//}}AFX_STS_PARAM_PROTOTYPES
	double HSLK[SITENUM] = { 0.0f }, LSLK[SITENUM] = { 0.0f }, vtemp[SITENUM] = { 0.0f };

	rlyC.SetOn(-1);
	FreshSiteFlagInit();
	// VIN
	VinFPVI0.Set(FV, 0.0f, FPVI10_5V, FPVI10_10MA, RELAY_ON);
	// EN
	EnFOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);
	// SW
	SwFPVI1.Set(FI, 1e-3f, FPVI10_5V, FPVI10_10MA, RELAY_ON);
	delay_ms(2);
	
	// RELAY SETTINGS
	rlyC.SetOn(VinFPVI, ENFOVI, SwFPVI, -1);
	delay_ms(1);
	// VIN
	VinFPVI0.Set(FV, 0.0f, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	delay_ms(2);
	// *********************************** HighSideLeakage, VIN=18, VSW=0 ***********************************
	// *********************************** HighSideLeakage, VIN=18, VSW=0 ***********************************
	// *********************************** HighSideLeakage, VIN=18, VSW=0 ***********************************
	// VIN=5 -> 18V
	VinFPVI0.Set(FV, 5.0f, FPVI10_20V, FPVI10_10MA, RELAY_ON, 2);
	delay_ms(1);
	VinFPVI0.Set(FV, 10.0f, FPVI10_20V, FPVI10_10MA, RELAY_ON, 2);
	delay_ms(1);
	VinFPVI0.Set(FV, 15.0f, FPVI10_20V, FPVI10_10MA, RELAY_ON, 2);
	delay_ms(1);
	VinFPVI0.Set(FV, 16.0f, FPVI10_20V, FPVI10_10MA, RELAY_ON, 2);
	delay_ms(1);
	VinFPVI0.Set(FV, 17.0f, FPVI10_20V, FPVI10_10MA, RELAY_ON, 2);
	delay_ms(1);
	VinFPVI0.Set(FV, 18.0f, FPVI10_20V, FPVI10_10MA, RELAY_ON, 2);
	delay_ms(1);
	// SW = 0
	SwFPVI1.Set(FV, float(0.0), FPVI10_5V, FPVI10_100UA, RELAY_ON);
	delay_ms(2);
  
	VinFPVI0.MeasureVI(20, 10);
  for (site = 0; site < SITENUM; site++)	{
		vtemp[site] = VinFPVI0.GetMeasResult(site, MVRET);
		if(vtemp[site] > float(17.8))	{
			SwFPVI1.MeasureVI(60, 10);
			HSLK[site] = SwFPVI1.GetMeasResult(site, MIRET);
		}
		ILK1->SetTestResult(site, 0, fabs(HSLK[site] *1e9));
	}
	// *********************************** LowSideLeakage, VIN=VSW=18 ***********************************
	// *********************************** LowSideLeakage, VIN=VSW=18 ***********************************
	// *********************************** LowSideLeakage, VIN=VSW=18 ***********************************
	// SW = 18
	SwFPVI1.Set(FV, float(5.0), FPVI10_20V, FPVI10_1MA, RELAY_ON, 2);
	delay_ms(2);
	SwFPVI1.Set(FV, float(10.0), FPVI10_20V, FPVI10_1MA, RELAY_ON, 2);
	delay_ms(2);
	SwFPVI1.Set(FV, float(15.0), FPVI10_20V, FPVI10_1MA, RELAY_ON, 2);
	delay_ms(2);
	SwFPVI1.Set(FV, float(16.0), FPVI10_20V, FPVI10_1MA, RELAY_ON, 2);
	delay_ms(2);
	SwFPVI1.Set(FV, float(17.0), FPVI10_20V, FPVI10_1MA, RELAY_ON, 2);
	delay_ms(2);
	SwFPVI1.Set(FV, float(18.0), FPVI10_20V, FPVI10_1MA, RELAY_ON, 2);
	delay_ms(2);
  
	SwFPVI1.MeasureVI(60, 10);
  for (site = 0; site < SITENUM; site++)	{
		vtemp[site] = SwFPVI1.GetMeasResult(site, MVRET);
		if(vtemp[site] > float(17.8))	{
			SwFPVI1.MeasureVI(60, 10);
			LSLK[site] = SwFPVI1.GetMeasResult(site, MIRET);
		}
		ILK2->SetTestResult(site, 0, fabs(LSLK[site] *1e9));
	}
	// PowerDownReset
	PWR0();
	PWROFF();
	rlyC.SetOn(-1);
 	delay_ms(1);

	return 0;
}

// test5, LSide
// ****************************************** LSMOS ******************************************
// ****************************************** LSMOS ******************************************
// ****************************************** LSMOS ******************************************
DUT_API int LSide(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
    CParam *RDSON = StsGetParam(funcindex,"RDSON");
    CParam *I_TEST = StsGetParam(funcindex,"I_TEST");
    CParam *IVALLY0 = StsGetParam(funcindex,"IVALLY0");
    CParam *RATIO = StsGetParam(funcindex,"RATIO");
    CParam *CurrentLimit = StsGetParam(funcindex,"CurrentLimit");
    CParam *IZX = StsGetParam(funcindex,"IZX");
    CParam *INEG = StsGetParam(funcindex,"INEG");
	//}}AFX_STS_PARAM_PROTOTYPES
 
	return 0;
}
// test6
// ****************************************** Switching1 ******************************************
// ****************************************** Switching1 ******************************************
// ****************************************** Switching1 ******************************************
DUT_API int Switching1(short funcindex, LPCTSTR funclabel)	{
  //{{AFX_STS_PARAM_PROTOTYPES
  CParam *SwitchingFrquency = StsGetParam(funcindex,"SwitchingFrquency");
  CParam *HSOnTime = StsGetParam(funcindex,"HSOnTime");
  CParam *MinOffTime = StsGetParam(funcindex,"MinOffTime");
  CParam *MaxDuty = StsGetParam(funcindex,"MaxDuty");
  //}}AFX_STS_PARAM_PROTOTYPES
	double Ton[SITENUM] = { 0.0 }, Toff[SITENUM] = { 0.0 }, Duty[SITENUM] = { 0.0 }, Fsw[SITENUM] = { 0.0 };

  rlyC.SetOn(-1);
 	delay_ms(1);
	FreshSiteFlagInit();
	// *** FOR MT8952NSLR IS AVAILABLE ***********************************************************
#ifdef MT8952NSLR
	// SW
	SwFPVI1.Set(FI, -1e-9f, FPVI10_20V, FPVI10_1A, RELAY_ON);
	delay_ms(3);
	// RELAY SETTINGS, FB-QTMUA w/o SWAP
	rlyC.SetOn(VinFPVI, FbFOVI, ENFOVI, SwFPVI, ENQTMUa, /*CBSTSW,*/ /*GNDs, VBSTSW,*/ -1);
	delay_ms(1);
	// SW
	SwFPVI1.Set(FI, 0.0f, FPVI10_20V, FPVI10_1A, RELAY_ON);
	delay_ms(1);
	// VIN = 6
	VinFPVI0.Set(FV, 6.0f, FPVI10_10V, FPVI10_1A, RELAY_ON);
	delay_ms(1);
	// EN = 5
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	//// BST-SW = 5
	//BST2_FOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	//delay_ms(1);
	
	// FB
	// TM0
	FB_FOVI.Set(FV, 6.5f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(1);
	FB_FOVI.Set(FI, 1e-6f, FOVI_10V, FOVI_100MA, RELAY_ON);	// It's necessary
	delay_ms(1);

	// EN
	// TM1
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	// TM2
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	// FB RELEASE
	FB_FOVI.Set(FI, 1e-6f, FOVI_10V, FOVI_100MA, RELAY_OFF);
	delay_ms(3);
	// TM3
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	// TM4
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	// TM5
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	// EN = 5
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	// FB = 5.5
	FB_FOVI.Set(FV, 5.5f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(3);

	// SW = 2.5
	////SwFPVI1.Set(FV, 2.5f, FPVI10_20V, FPVI10_1A, RELAY_ON);
	SwFPVI1.Set(FV, 5.0f, FPVI10_20V, FPVI10_1A, RELAY_ON);
	delay_ms(3);
	// VIN = 12
	VinFPVI0.Set(FV, 12.0f, FPVI10_20V, FPVI10_1A, RELAY_ON);
	delay_ms(1);

	// QTMu Settings
  qtmu0.Connect();
	qtmu0.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_10MHz);
  qtmu0.SetStartTrigger(float(5.0), QTMU_PLUS_POS_SLOPE);
  qtmu0.SetStopTrigger(float(5.0), QTMU_PLUS_NEG_SLOPE);
  qtmu0.SetInSource(QTMU_PLUS_SINGLE_SOURCE);
	delay_ms(1);

	qtmu0.Meas(QTMU_PLUS_COARSE,QTMU_PLUS_TRNG_US,0);
	for(site = 0; site < SITENUM; site++)	{
		Ton[site] = qtmu0.GetMeasureResult(site);
		////Fsw[site] = 2.5f / ((Ton[site] + 1.0e-15f)*5.0f);
		Fsw[site] = 5 / ((Ton[site] + 1.0e-15f)*5.0f);
		// List
		HSOnTime->SetTestResult(site, 0, Ton[site] * 1e0f);						// usec
		SwitchingFrquency->SetTestResult(site, 0, Fsw[site] * 1e3f);	// Khz
	}
	// Offtime Settings
	qtmu0.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_10MHz);
  qtmu0.SetStartTrigger(float(5.0), QTMU_PLUS_NEG_SLOPE);
  qtmu0.SetStopTrigger(float(5.0), QTMU_PLUS_POS_SLOPE);
  qtmu0.SetInSource(QTMU_PLUS_SINGLE_SOURCE);
	delay_ms(1);

	qtmu0.Meas(QTMU_PLUS_COARSE,QTMU_PLUS_TRNG_US,0);
	for(site = 0; site < SITENUM; site++ )	{
		Toff[site]=qtmu0.GetMeasureResult(site);
		//Duty[site]=(1-(Ton[site] * Fsw[site])) * 100;
		Duty[site]=(1-(Toff[site] * Fsw[site])) * 100;	// usec*Mhz
		
		MinOffTime->SetTestResult(site, 0, Toff[site]*1e3f);					// nsec
		MaxDuty->SetTestResult(site, 0, Duty[site]);
	}
#endif
	// *** FOR MT8952NSLR IS AVAILABLE ***********************************************************
	// *** FOR MT3552NSLR IS AVAILABLE ***********************************************************
#ifdef MT3552NSLR
	// SW
	SwFPVI1.Set(FI, -1e-9f, FPVI10_20V, FPVI10_1A, RELAY_ON);
	delay_ms(3);
	// RELAY SETTINGS, FB-QTMUA w/o SWAP
	rlyC.SetOn(VinFPVI, FbFOVI, ENFOVI, SwFPVI, ENQTMUa, /*CBSTSW,*/ /*GNDs, VBSTSW,*/ -1);
	delay_ms(1);
	// SW
	SwFPVI1.Set(FI, 0.0f, FPVI10_20V, FPVI10_1A, RELAY_ON);
	delay_ms(1);
	// VIN = 6
	VinFPVI0.Set(FV, 6.0f, FPVI10_10V, FPVI10_1A, RELAY_ON);
	delay_ms(1);
	// EN = 5
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	//// BST-SW = 5
	//BST2_FOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	//delay_ms(1);
	
	// FB
	// TM0
	FB_FOVI.Set(FV, 6.5f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(1);
	FB_FOVI.Set(FI, 1e-6f, FOVI_10V, FOVI_100MA, RELAY_ON);	// It's necessary
	delay_ms(1);

	// EN
	// TM1
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	// TM2
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	// FB RELEASE
	FB_FOVI.Set(FI, 1e-6f, FOVI_10V, FOVI_100MA, RELAY_OFF);
	delay_ms(3);
	// TM3
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	// TM4
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	// TM5
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	// EN = 5
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	// FB = 5.5
	FB_FOVI.Set(FV, 5.5f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(3);

	// SW = 2.5
	////SwFPVI1.Set(FV, 2.5f, FPVI10_20V, FPVI10_1A, RELAY_ON);
	SwFPVI1.Set(FV, 5.0f, FPVI10_20V, FPVI10_1A, RELAY_ON);
	delay_ms(3);
	// VIN = 12
	VinFPVI0.Set(FV, 12.0f, FPVI10_20V, FPVI10_1A, RELAY_ON);
	delay_ms(1);

	// QTMu Settings
  qtmu0.Connect();
	qtmu0.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_10MHz);
  qtmu0.SetStartTrigger(float(5.0), QTMU_PLUS_POS_SLOPE);
  qtmu0.SetStopTrigger(float(5.0), QTMU_PLUS_NEG_SLOPE);
  qtmu0.SetInSource(QTMU_PLUS_SINGLE_SOURCE);
	delay_ms(1);

	qtmu0.Meas(QTMU_PLUS_COARSE,QTMU_PLUS_TRNG_US,0);
	for(site = 0; site < SITENUM; site++)	{
		Ton[site] = qtmu0.GetMeasureResult(site);
		////Fsw[site] = 2.5f / ((Ton[site] + 1.0e-15f)*5.0f);
		Fsw[site] = 5 / ((Ton[site] + 1.0e-15f)*5.0f);
		// List
		HSOnTime->SetTestResult(site, 0, Ton[site] * 1e0f);						// usec
		SwitchingFrquency->SetTestResult(site, 0, Fsw[site] * 1e3f);	// Khz
	}
	// Offtime Settings
	qtmu0.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_10MHz);
  qtmu0.SetStartTrigger(float(5.0), QTMU_PLUS_NEG_SLOPE);
  qtmu0.SetStopTrigger(float(5.0), QTMU_PLUS_POS_SLOPE);
  qtmu0.SetInSource(QTMU_PLUS_SINGLE_SOURCE);
	delay_ms(1);

	qtmu0.Meas(QTMU_PLUS_COARSE,QTMU_PLUS_TRNG_US,0);
	for(site = 0; site < SITENUM; site++ )	{
		Toff[site]=qtmu0.GetMeasureResult(site);
		//Duty[site]=(1-(Ton[site] * Fsw[site])) * 100;
		Duty[site]=(1-(Toff[site] * Fsw[site])) * 100;	// usec*Mhz
		
		MinOffTime->SetTestResult(site, 0, Toff[site]*1e3f);					// nsec
		MaxDuty->SetTestResult(site, 0, Duty[site]);
	}
#endif
	// *** FOR MT3552NSLR IS AVAILABLE ***********************************************************
  // TODO: Add your function code here
	PWR0();
	PWROFF();
  rlyC.SetOn(-1);
 	delay_ms(1);

  return 0;
}

// test8, RegulatorV
// ****************************************** RegulatorV ******************************************
// ****************************************** RegulatorV ******************************************
// ****************************************** RegulatorV ******************************************
DUT_API int RegulatorV(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
  CParam *ReferenceVoltage = StsGetParam(funcindex,"ReferenceVoltage");
	//}}AFX_STS_PARAM_PROTOTYPES

	double vref[SITENUM] = { 0.0 };

  rlyC.SetOn(-1);
 	delay_ms(1);
  FreshSiteFlagInit();

	// RELAY SETTING
	rlyC.SetOn(VinFPVI, FbFOVI, ENFOVI, CBSTSW, -1);
	delay_ms(1);
	// LASER BIT3
	// '0' REF=800mV, '1' REF=600mV
	// ****************************************** VFB ******************************************
	// ****************************************** VFB ******************************************
	// ****************************************** VFB ******************************************
	// VCC = 6
	VinFPVI0.Set(FV, 6.0f, FPVI10_20V, FPVI10_100MA, RELAY_ON);
	// EN = 5
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	// TM0
	FB_FOVI.Set(FV, 6.5f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	// FB = 0
	//FB_FOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON); 
	//delay_ms(1);
	
	// TM1
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_us(300);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_us(300);
	FB_FOVI.Set(FI, -1e-12f, FOVI_1V, FOVI_10UA, RELAY_ON); 
	delay_ms(1);

	FB_FOVI.MeasureVI(30, 10);
	for(site=0; site<SITENUM; site++)	{
		vref[site] = FB_FOVI.GetMeasResult(site, MVRET) *1e3f;
		if(DebugConsole[site])	fprintf(stderr, "VREF_S%d		: %3.4f\n", site + 1, vref[site]);
		ReferenceVoltage->SetTestResult(site, 0, vref[site]);
	}

  // TODO: Add your function code here
	VinFPVI0.Set(FV, 0.0f, FPVI10_5V, FPVI10_10MA, RELAY_ON);	// VIN
	EnFOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);				// EN
	FB_FOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);			// FB

  rlyC.SetOn(-1);
 	delay_ms(1);
  
	return 0;
}
// test9
// ****************************************** CurrentFB ******************************************
// ****************************************** CurrentFB ******************************************
// ****************************************** CurrentFB ******************************************
DUT_API int CurrentFB(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
    CParam *FBCurrent = StsGetParam(funcindex,"FBCurrent");
	//}}AFX_STS_PARAM_PROTOTYPES

	// TODO: Add your function code here
	double ifb[SITENUM] = { 0.0 };

  rlyC.SetOn(-1);
 	delay_ms(1);
  FreshSiteFlagInit();

	rlyC.SetOn(VinFPVI, FbFOVI, ENFOVI, CBSTSW, -1);
	delay_ms(1);
	// ****************************************** IFB ******************************************
	// ****************************************** IFB ******************************************
	// ****************************************** IFB ******************************************
	// VCC = 6
	VinFPVI0.Set(FV, 6.0f, FPVI10_10V, FPVI10_1MA, RELAY_ON);
	// EN = 5
	EnFOVI.Set(FV, 5.0f, FOVI_2V, FOVI_10MA, RELAY_ON);
	// FB = 0.76
	FB_FOVI.Set(FV, 0.7f, FOVI_2V, FOVI_100UA, RELAY_ON);
	delay_ms(1);
	FB_FOVI.Set(FV, 0.71f, FOVI_2V, FOVI_100UA, RELAY_ON);
	delay_ms(1);
	FB_FOVI.Set(FV, 0.72f, FOVI_2V, FOVI_100UA, RELAY_ON);
	delay_ms(1);
	FB_FOVI.Set(FV, 0.73f, FOVI_2V, FOVI_100UA, RELAY_ON);
	delay_ms(1);
	FB_FOVI.Set(FV, 0.74f, FOVI_2V, FOVI_100UA, RELAY_ON);
	delay_ms(1);
	FB_FOVI.Set(FV, 0.75f, FOVI_2V, FOVI_100UA, RELAY_ON);
	delay_ms(1);
	FB_FOVI.Set(FV, 0.76f, FOVI_2V, FOVI_100UA, RELAY_ON);
	delay_ms(1);

	FB_FOVI.MeasureVI(900, 5);
	for(site=0; site<SITENUM; site++)	{
		ifb[site] = FB_FOVI.GetMeasResult(site, MIRET) * 1e9f;	// nA
		//if(DebugConsole[site])	fprintf(stderr, "IFB_S%d		: %3.4f\n", site + 1, fabs(ifb[site]));
		FBCurrent->SetTestResult(site, 0, fabs(ifb[site]));
	}
  // TODO: Add your function code here
	VinFPVI0.Set(FV, 0.0f, FPVI10_5V, FPVI10_10MA, RELAY_ON);	// VIN
	EnFOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);				// EN
	FB_FOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);			// FB

  rlyC.SetOn(-1);
 	delay_ms(1);

	return 0;
}

// test10
// ****************************************** SoftStartTime ******************************************
// ****************************************** SoftStartTime ******************************************
// ****************************************** SoftStartTime ******************************************
DUT_API int SoftStartTime(short funcindex, LPCTSTR funclabel)	{
  //{{AFX_STS_PARAM_PROTOTYPES
    CParam *TSS = StsGetParam(funcindex,"TSS");
  //}}AFX_STS_PARAM_PROTOTYPES

	double Tss[SITENUM] = {0.0f};
  // TODO: Add your function code here
	rlyC.SetOn(-1);
	FreshSiteFlagInit();

	// VIN
	VinFPVI0.Set(FV, 0.0f, FPVI10_5V, FPVI10_10MA, RELAY_ON);
	// SW
	SwFPVI1.Set(FI, 1e-3f, FPVI10_20V, FPVI10_10MA, RELAY_ON);
	// EN
	EnFOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);
	delay_ms(10);

	// RELAY SETTING
	rlyC.SetOn(VinFPVI, FbFOVI, ENFOVI, SwFPVI, CapVIN, CBSTSW, GNDs, VBSTSW, TSSQTMU, QTMU_SWAP, -1);
	delay_ms(1);

	// VIN
	VinFPVI0.Set(FV, 0.0f, FPVI10_20V, FPVI10_10MA, RELAY_ON);
	// EN
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(10);
	// VIN=12
	VinFPVI0.Set(FV, 12.0f, FPVI10_20V, FPVI10_10MA, RELAY_ON);
	delay_ms(1);
	// FB=0.57
	FB_FOVI.Set(FV, 0.57f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	// SW 1mA loading
	SwFPVI1.Set(FI, -1e-3f, FPVI10_20V, FPVI10_10MA, RELAY_ON);
	// BST-SW 
	BST2_FOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(2);
	
	// TMU SETTING
	qtmu0.Init();
	delay_ms(2);
	qtmu0.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
	qtmu0.SetStopInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
	qtmu0.SetStartTrigger(4.8f, QTMU_PLUS_POS_SLOPE);
	qtmu0.SetStopTrigger(11.0f, QTMU_PLUS_POS_SLOPE);			// EN positive to SW positive(A-B)
	qtmu0.SetInSource(QTMU_PLUS_DUAL_SOURCE);
	qtmu0.Connect();
	delay_ms(1);
	// EN = 5(TRIGGER)
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
  // TSS *****************************************************************
	for(site = 0; site < SITENUM; site++ )  {
	  qtmu0.SetSinglePulseMeas(QTMU_PLUS_COARSE, QTMU_PLUS_TIME_US, site);
	  qtmu0.SetTimeOut(5);
	  qtmu0.SinglePlsMeas(site);
		Tss[site]=qtmu0.GetMeasureResult(site);							// us
		Tss[site] += float(0.0);
		TSS->SetTestResult(site, 0, fabs(Tss[site])*1e-3);	// ms
	}
	PWR0();
	PWROFF();
	rlyC.SetOn(-1);
	delay_ms(2);
	
	return 0;
}
// test11
// ****************************************** FuncEN1 ******************************************
// ****************************************** FuncEN1 ******************************************
// ****************************************** FuncEN1 ******************************************
DUT_API int FuncEN1(short funcindex, LPCTSTR funclabel)	{
  //{{AFX_STS_PARAM_PROTOTYPES
    CParam *VENH = StsGetParam(funcindex,"VENH");
    CParam *VNEL = StsGetParam(funcindex,"VNEL");
    CParam *VENHYS = StsGetParam(funcindex,"VENHYS");
  //}}AFX_STS_PARAM_PROTOTYPES
	double VEN_ON[SITENUM] = { 999.0f }, VEN_OFF[SITENUM] = { 999.0f }, ICC[SITENUM] = { 999.0f };
	double vf = 0.0f, StepV = 50e-3f, ii = 0.0f;

	// TODO: Add your function code here
  rlyC.SetOn(-1);
 	delay_ms(1);
	FreshSiteFlagInit();

	PWR0();
	PWROFF();
	delay_ms(1);
		
	rlyC.SetOn(VinFPVI, SwFPVI, ENFOVI, FbFOVI, CBSTSW, CapVIN, -1);
	delay_ms(2);
	// VIN
	for(ii = float(0); ii <= float(12); ii += 1000e-3)
		VinFPVI0.Set(FV, float(ii), FPVI10_20V, FPVI10_100MA, RELAY_ON, 5);
	// EN
	EnFOVI.Set(FV, float(0.6), FOVI_5V,	FOVI_100MA, RELAY_ON);
	// FB = 0.73
	FB_FOVI.Set(FV, float(0.73), FOVI_5V,	FOVI_10MA, RELAY_ON);
	delay_ms(1);
	// SW
	SwFPVI1.Set(FI, float(-1e-3), FPVI10_5V, FPVI10_100MA, RELAY_ON);
	delay_ms(1);

	// ************************************** VenUVLO ON **************************************
	// ************************************** VenUVLO ON **************************************
	// ************************************** VenUVLO ON **************************************
	for(vf = float(0.9); vf < float(1.5);)	{
		for(site = 0; site < SITENUM; site++)	{
			EnFOVI.Set(FV, float(vf), FOVI_5V,	FOVI_100MA, RELAY_ON);
 			delay_us(300);
		
			VinFPVI0.MeasureVI(200, 5);
			ICC[site] = VinFPVI0.GetMeasResult(site, MIRET, MAX_RESULT);
			//if(DebugConsole[site])	fprintf(stderr, "V_S%d	: %3.4f	I_S%d	:%3.4f\n", site+1, vf, site+1, ICC[site]*1e6);
			if( (ICC[site] > float(1200e-6f))&&flag[site]==0)	{			// 1200uA
				EnFOVI.MeasureVI(30, 10);
				VEN_ON[site] = EnFOVI.GetMeasResult(site, MVRET);
				flag[site] = 1;
			}
		}
		if(vf >= 1.15f) 
			StepV = float(10e-3);
		vf += float(StepV);
		if(flag[0] && flag[1] && flag[2] && flag[3])
			break;
	}
	// ************************************** VenUVLO OFF **************************************
	// ************************************** VenUVLO OFF **************************************
	// ************************************** VenUVLO OFF **************************************
	FreshSiteFlagInit();
	StepV = float(-50e-3);

	for(vf = float(1.3); vf > float(0.9);)	{
		for(site = 0; site < SITENUM; site++)	{
			EnFOVI.Set(FV, float(vf), FOVI_5V,	FOVI_100MA, RELAY_ON);
 			delay_us(300);
		
			VinFPVI0.MeasureVI(200, 5);
			ICC[site] = VinFPVI0.GetMeasResult(site, MIRET, MAX_RESULT);
			//if(DebugConsole[site])	fprintf(stderr, "V_S%d	: %3.4f	I_S%d	:%3.4f\n", site+1, vf, site+1, ICC[site]*1e6);
			//if( (ICC[site] < float(600e-6f))&&flag[site]==0)	{			// 600uA
			if( (ICC[site] < float(800e-6f))&&flag[site]==0)	{			// 800uA(#2)
				EnFOVI.MeasureVI(30, 10);
				VEN_OFF[site] = EnFOVI.GetMeasResult(site, MVRET);
				flag[site] = 1;
			}
		}
		if(vf <= 1.15f) 
			StepV = float(-10e-3);
		vf += float(StepV);
		if(flag[0] && flag[1] && flag[2] && flag[3])
			break;
	}
	// ************************************** IEN(2V) **************************************
	// ************************************** IEN(2V) **************************************
	// ************************************** IEN(2V) **************************************
	// EN = 2
	EnFOVI.Set(FV, float(2.0), FOVI_5V,	FOVI_10MA, RELAY_ON);
	delay_ms(1);
	EnFOVI.Set(FV, float(2.0), FOVI_5V,	FOVI_1MA, RELAY_ON);
	delay_ms(1);
	EnFOVI.Set(FV, float(2.0), FOVI_5V,	FOVI_10UA, RELAY_ON);
	delay_ms(1);
	EnFOVI.MeasureVI(200, 5);
	for(site = 0; site < SITENUM; site++)
		IEN1H[site] = EnFOVI.GetMeasResult(site, MIRET, MAX_RESULT);
	// ************************************** IEN(0V) **************************************
	// ************************************** IEN(0V) **************************************
	// ************************************** IEN(0V) **************************************
	// EN = 0
	EnFOVI.Set(FV, float(0.0), FOVI_5V,	FOVI_1MA, RELAY_ON);
	delay_ms(1);
	EnFOVI.Set(FV, float(0.0), FOVI_5V,	FOVI_10UA, RELAY_ON);
	delay_ms(1);
	EnFOVI.MeasureVI(200, 5);
	for(site = 0; site < SITENUM; site++)
		IEN2L[site] = EnFOVI.GetMeasResult(site, MIRET, MAX_RESULT);

	for(site = 0; site < SITENUM; site++)	{
		VENH	->SetTestResult(site, 0, VEN_ON[site]);
		VNEL	->SetTestResult(site, 0, VEN_OFF[site]);
		VENHYS->SetTestResult(site, 0, (VEN_ON[site] - VEN_OFF[site])*1e3);
		if(DebugConsole[site])	{	
			fprintf(stderr, "VenON_S%d	: %3.4f\n", site + 1, VEN_ON[site]);
			fprintf(stderr, "VenOff_S%d	: %3.4f\n", site + 1, VEN_OFF[site]);
			fprintf(stderr, "VenHys_S%d	: %3.4f\n", site + 1, (VEN_ON[site] - VEN_OFF[site])*1e3);
		}
	}
	// PowerDownReset
	PWR0();
	PWROFF();
	rlyC.SetOn(-1);
 	delay_ms(1);

  return 0;
}

// test12
// ****************************************** FuncEN2 ******************************************
// ****************************************** FuncEN2 ******************************************
// ****************************************** FuncEN2 ******************************************
DUT_API int FuncEN2(short funcindex, LPCTSTR funclabel)	{
  //{{AFX_STS_PARAM_PROTOTYPES
    CParam *IEN1 = StsGetParam(funcindex,"IEN1");
    CParam *IEN2 = StsGetParam(funcindex,"IEN2");
  //}}AFX_STS_PARAM_PROTOTYPES

  // TODO: Add your function code here
	for(site = 0; site < SITENUM; site++)	{
		IEN1	->SetTestResult(site, 0, IEN1H[site]*1e6);
		IEN2	->SetTestResult(site, 0, IEN2L[site]*1e9);
		
		if(DebugConsole[site])	{	
			fprintf(stderr, "IEN1H_S%d	: %3.4f\n", site + 1, IEN1H[site]*1e6);
			fprintf(stderr, "IEN2L_S%d	: %3.4f\n", site + 1, IEN2L[site]*1e9);
		}
	}
  return 0;
}
// test13, UVLO
// ****************************************** UVLO ******************************************
// ****************************************** UVLO ******************************************
// ****************************************** UVLO ******************************************
DUT_API int UVLO(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
    CParam *UVLO_Rising = StsGetParam(funcindex,"UVLO_Rising");
    CParam *UVLO_Falling = StsGetParam(funcindex,"UVLO_Falling");
    CParam *UVLO_VHYS = StsGetParam(funcindex,"UVLO_VHYS");
	//}}AFX_STS_PARAM_PROTOTYPES
	double UVLO_ON[SITENUM] = { 999.0f }, UVLO_OFF[SITENUM] = { 999.0f }, ICC[SITENUM] = { 999.0f };
	double vf = 0.0f, StepV = 50e-3f, ii = 0.0f;

	// TODO: Add your function code here
  rlyC.SetOn(-1);
 	delay_ms(1);
	FreshSiteFlagInit();

	// Initialize
	rlyC.SetOn(VinFPVI, SwFPVI, ENFOVI, BstFOVI, FbFOVI, CapVIN, -1);
	delay_ms(1);
	PWR0();
	PWROFF();
	delay_ms(1);
	
	// Setting
	rlyC.SetOn(VinFPVI, SwFPVI, ENFOVI, FbFOVI, CBSTSW, CapVIN, -1);
	// VIN RAMP UP 3V
	for(ii = float(0); ii <= float(3); ii += 100e-3)
		VinFPVI0.Set(FV, float(ii), FPVI10_5V, FPVI10_100MA, RELAY_ON);
	// EN=5
	EnFOVI.Set(FV, float(5.0), FOVI_5V,	FOVI_100MA, RELAY_ON);
	// FB=0.85
	FB_FOVI.Set(FV, float(0.85), FOVI_5V,	FOVI_10MA, RELAY_ON);
	delay_ms(1);
	// SW
	SwFPVI1.Set(FI, float(-0.1e-3), FPVI10_5V, FPVI10_10MA, RELAY_ON);
	delay_ms(1);
	// ************************************** VinUVLO ON **************************************
	// ************************************** VinUVLO ON **************************************
	// ************************************** VinUVLO ON **************************************
	for(vf = float(3.55); vf < float(4.55);)	{
		for(site = 0; site < SITENUM; site++)	{
			VinFPVI0.Set(FV, vf, FPVI10_5V, FPVI10_100MA, RELAY_ON, 2);
 			delay_us(300);
		
			VinFPVI0.MeasureVI(200, 5);
			ICC[site] = VinFPVI0.GetMeasResult(site, MIRET);

			//if(DebugConsole[site])	fprintf(stderr, "V_S%d	: %3.4f	I_S%d	:%3.4f\n", site+1, vf, site+1, ICC[site]*1e6);
			//if( (ICC[site] > float(800e-6f))&&flag[site]==0)	{			// 800uA
			if( (ICC[site] > float(600e-6f))&&flag[site]==0)	{			// 600uA(#2)
				UVLO_ON[site] = VinFPVI0.GetMeasResult(site, MVRET);
				flag[site] = 1;
			}
		}
		if(vf >= 3.5f) 
			StepV = float(10e-3);
		vf += float(StepV);
		if(flag[0] && flag[1] && flag[2] && flag[3])
			break;
	}
	// ************************************** VinUVLO OFF **************************************
	// ************************************** VinUVLO OFF **************************************
	// ************************************** VinUVLO OFF **************************************
	FreshSiteFlagInit();
	StepV = float(-50e-3);
	VinFPVI0.Set(FV, float(4.0), FPVI10_5V, FPVI10_100MA, RELAY_ON, 2);
	delay_ms(1);
	for(vf = float(4.0); vf > float(3.0);)	{
		for(site = 0; site < SITENUM; site++)	{
			VinFPVI0.Set(FV, vf, FPVI10_5V, FPVI10_100MA, RELAY_ON);
 			delay_us(300);
		
			VinFPVI0.MeasureVI(200, 5);
			ICC[site] = VinFPVI0.GetMeasResult(site, MIRET);

			//if(DebugConsole[site])	fprintf(stderr, "V_S%d	: %3.4f	I_S%d	:%3.4f\n", site+1, vf, site+1, ICC[site]*1e6);
			if( (ICC[site] < float(400e-6f))&&flag[site]==0)	{			// 400uA
				UVLO_OFF[site] = VinFPVI0.GetMeasResult(site, MVRET);
				flag[site] = 1;
			}
		}
		if(vf <= 3.7f) 
			StepV = float(-10e-3);
		vf += float(StepV);
		if(flag[0] && flag[1] && flag[2] && flag[3])
			break;
	}

	for(site = 0; site < SITENUM; site++)	{
		UVLO_Rising	->SetTestResult(site, 0, UVLO_ON[site]);
		UVLO_Falling->SetTestResult(site, 0, UVLO_OFF[site]);
		UVLO_VHYS		->SetTestResult(site, 0, (UVLO_ON[site] - UVLO_OFF[site])*1e3);
		if(DebugConsole[site])	{	
			fprintf(stderr, "VinON_S%d	: %3.4f\n", site + 1, UVLO_ON[site]);
			fprintf(stderr, "VinOff_S%d	: %3.4f\n", site + 1, UVLO_OFF[site]);
			fprintf(stderr, "VinHys_S%d	: %3.4f\n", site + 1, (UVLO_ON[site] - UVLO_OFF[site])*1e3);
		}
	}
	// PowerDownReset
	PWR0();
	PWROFF();
	rlyC.SetOn(-1);
 	delay_ms(1);

	return 0;
}
// test14
// ****************************************** OVP ******************************************
// ****************************************** OVP ******************************************
// ****************************************** OVP ******************************************
DUT_API int OVP(short funcindex, LPCTSTR funclabel)	{
  //{{AFX_STS_PARAM_PROTOTYPES
    CParam *OVP_Rising = StsGetParam(funcindex,"OVP_Rising");
    CParam *OVP_Falling = StsGetParam(funcindex,"OVP_Falling");
    CParam *OVP_VHYS = StsGetParam(funcindex,"OVP_VHYS");
  //}}AFX_STS_PARAM_PROTOTYPES
	double OVP_ON[SITENUM] = { 999.0f }, OVP_OFF[SITENUM] = { 999.0f }, ICC[SITENUM] = { 999.0f };
	double vf = 0.0f, StepV = 100e-3f, ii = 0.0f;

	// TODO: Add your function code here
  rlyC.SetOn(-1);
 	delay_ms(1);
	FreshSiteFlagInit();

	rlyC.SetOn(VinFPVI, ENFOVI, FbFOVI, CBSTSW, CapVIN, -1);
	delay_ms(2);
	
	// VIN RAMP UP 12V
	for(ii = float(0); ii <= float(12); ii += 1000e-3)
		VinFPVI0.Set(FV, float(ii), FPVI10_50V, FPVI10_100MA, RELAY_ON, 5);
	// EN=5
	EnFOVI.Set(FV, float(5.0), FOVI_5V,	FOVI_100MA, RELAY_ON);
	// FB=0.85
	FB_FOVI.Set(FV, float(0.8), FOVI_2V,	FOVI_10MA, RELAY_ON);//0.85
	delay_ms(1);

	// VIN
	VinFPVI0.Set(FV, float(12.0), FPVI10_50V, FPVI10_100MA, RELAY_ON, 2);
	delay_us(300);
	VinFPVI0.Set(FV, float(15.0), FPVI10_50V, FPVI10_100MA, RELAY_ON, 2);
	delay_us(300);
	VinFPVI0.Set(FV, float(19.0), FPVI10_50V, FPVI10_100MA, RELAY_ON, 2);
	delay_us(300);

	// ************************************** VinOVP ON **************************************
	// ************************************** VinOVP ON **************************************
	// ************************************** VinOVP ON **************************************
	for(vf = float(19.0); vf < float(25.0);)	{
		for(site = 0; site < SITENUM; site++)	{
			VinFPVI0.Set(FV, vf, FPVI10_50V, FPVI10_100MA, RELAY_ON);
 			delay_us(300);
		
			VinFPVI0.MeasureVI(100, 20);// 100, 5
			ICC[site] = VinFPVI0.GetMeasResult(site, MIRET);

			//if(DebugConsole[site])	fprintf(stderr, "V_S%d	: %3.4f	I_S%d	:%3.4f\n", site+1, vf, site+1, ICC[site]*1e6);
			//if( (ICC[site] < float(500e-6f))&&flag[site]==0)	{			// 500uA
			if( (ICC[site] < float(600e-6f))&&flag[site]==0)	{			// 600uA(#2)
				OVP_ON[site] = VinFPVI0.GetMeasResult(site, MVRET);
				flag[site] = 1;
			}
		}
		if(vf >= 20.0f) 
			StepV = float(10e-3);
		vf += float(StepV);
		if(flag[0] && flag[1] && flag[2] && flag[3])
			break;
	}
	// ************************************** VinOVP OFF **************************************
	// ************************************** VinOVP OFF **************************************
	// ************************************** VinOVP OFF **************************************
	FreshSiteFlagInit();
	StepV = float(-100e-3);
	for(vf = float(21.0); vf > float(18.0);)	{
		for(site = 0; site < SITENUM; site++)	{
			VinFPVI0.Set(FV, vf, FPVI10_50V, FPVI10_100MA, RELAY_ON);
 			delay_us(300);
		
			VinFPVI0.MeasureVI(200, 5);
			ICC[site] = VinFPVI0.GetMeasResult(site, MIRET);

			//if(DebugConsole[site])	fprintf(stderr, "V_S%d	: %3.4f	I_S%d	:%3.4f\n", site+1, vf, site+1, ICC[site]*1e6);
			if( (ICC[site] > float(1000e-6f))&&flag[site]==0)	{			// 1000uA
				OVP_OFF[site] = VinFPVI0.GetMeasResult(site, MVRET);
				flag[site] = 1;
			}
		}
		if(vf <= 19.0f) 
			StepV = float(-10e-3);
		vf += float(StepV);
		if(flag[0] && flag[1] && flag[2] && flag[3])
			break;
	}
	for(site = 0; site < SITENUM; site++)	{
		OVP_Rising	->SetTestResult(site, 0, OVP_ON[site]);
		OVP_Falling	->SetTestResult(site, 0, OVP_OFF[site]);
		OVP_VHYS		->SetTestResult(site, 0, (OVP_ON[site] - OVP_OFF[site])*1e3);
		if(DebugConsole[site])	{	
			fprintf(stderr, "VinOVPT_S%d	: %3.4f\n", site + 1, OVP_ON[site]);
			fprintf(stderr, "VinOVPR_S%d	: %3.4f\n", site + 1, OVP_OFF[site]);
			fprintf(stderr, "VinOVPHys_S%d	: %3.4f\n", site + 1, (OVP_ON[site] - OVP_OFF[site])*1e3);
		}
	}
	// PowerDownReset
	PWR0();
	PWROFF();
	rlyC.SetOn(-1);
 	delay_ms(1);
  
	// TODO: Add your function code here
	return 0;

}
// test15
// ****************************************** BOOST ******************************************
// ****************************************** BOOST ******************************************
// ****************************************** BOOST ******************************************
DUT_API int BOOST(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
    CParam *BSTOK_HIGH = StsGetParam(funcindex,"BSTOK_HIGH");
    CParam *BSTOK_LOW = StsGetParam(funcindex,"BSTOK_LOW");
    CParam *BSTOK_HYS = StsGetParam(funcindex,"BSTOK_HYS");
	//}}AFX_STS_PARAM_PROTOTYPES
	double TMPV[SITENUM] = { 0.0f }, i = 0.0f, step_v = 10e-3f;
  double BSTOK_ON[SITENUM] = { 0.f }, BSTOK_OFF[SITENUM] = { 0.0f }, BSTOK_HYSV[SITENUM] = { 0.0f };
	
	FreshSiteFlagInit();
	rlyC.SetOn(CapVIN, CBSTSW, VinFPVI, ENFOVI, FbFOVI, SwFPVI, 
						 VBSTSW, GNDs, -1);
 	delay_ms(2);
	// VIN = 6V
	VinFPVI0.Set(FV, 6.0f, FPVI10_20V, FPVI10_100MA, RELAY_ON);
 	delay_us(300);
	
	// ************************************************************************************************************
	// EN = 5.0V(TM0)
 	EnFOVI.Set(FV, 5.0f, FOVI_5V, FOVI_10MA, RELAY_ON, 5);
 	delay_us(100);
	// FB = 6.5V
	FB_FOVI.Set(FV, 6.5f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	
	// VIN = 24v
 	VinFPVI0.Set(FV, 8.0f, FPVI10_20V, FPVI10_100MA, RELAY_ON);
 	delay_us(500);
	VinFPVI0.Set(FV, 9.0f, FPVI10_20V, FPVI10_100MA, RELAY_ON);
 	delay_us(500);
	VinFPVI0.Set(FV, 10.0f, FPVI10_20V, FPVI10_100MA, RELAY_ON);
 	delay_us(500);
	VinFPVI0.Set(FV, 11.0f, FPVI10_20V, FPVI10_100MA, RELAY_ON);
 	delay_us(500);
	VinFPVI0.Set(FV, 12.0f, FPVI10_20V, FPVI10_100MA, RELAY_ON);
 	delay_us(500);
	
	// SW = 3V/100mA
	SwFPVI1.Set(FV, 3.0f, FPVI10_10V, FPVI10_100MA, RELAY_ON);
 	delay_ms(1);
	// FB = 5.0V/10uA
 	FB_FOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10UA, RELAY_ON);
 	delay_ms(1);

	// BST (BST-SW Vdiff = 1.5 to 3.5, real 4.5v - 6.5v)
	BST2_FOVI.Set(FV, 1.5f, FOVI_5V, FOVI_10MA, RELAY_ON);
 	delay_ms(1);
	// ************************************** BSTOK_ON **************************************
	// ************************************** BSTOK_ON **************************************
	// ************************************** BSTOK_ON **************************************
	for(site = 0; site < SITENUM; site++)	{
		for(i = 1.5f; i <= 3.5f;)	{
			// BST (BST-SW Vdiff = 1.5 to 3.5, real 4.5v - 6.5v)
			BST2_FOVI.Set(FV, i, FOVI_5V, FOVI_10MA, RELAY_ON);
			delay_us(300);
			FB_FOVI.MeasureVI(10, 5);
			delay_us(300);
			TMPV[site] = FB_FOVI.GetMeasResult(site, MVRET);
			//if(DebugConsole[site])	fprintf(stderr, "V_S%d	: %3.4f\n", site+1, TMPV[site]);
			if(TMPV[site] > float(4.8))	{
				flag[site] = 1;
				BST2_FOVI.MeasureVI(10, 5);
				BSTOK_ON[site] = BST2_FOVI.GetMeasResult(site, MVRET);
			}
			i += step_v;
			if(flag[0] && flag[1] && flag[2] && flag[3])
				break;
		}
	}
	FreshSiteFlagInit();
	// ************************************** BSTOK_OFF **************************************
	// ************************************** BSTOK_OFF **************************************
	// ************************************** BSTOK_OFF **************************************
		for(site = 0; site < SITENUM; site++)	{
			for(i = 3.5f; i >= 1.5f;)	{
				BST2_FOVI.Set(FV, i, FOVI_10V, FOVI_10MA, RELAY_ON);
 				delay_us(300);
				FB_FOVI.MeasureVI(20, 10);
				TMPV[site] = FB_FOVI.GetMeasResult(site, MVRET);
				//if(DebugConsole[site])	fprintf(stderr, "V_S%d	: %3.4f\n", site+1, TMPV[site]);
				if(TMPV[site] < float(0.3))	{
					flag[site] = 1;
					BST2_FOVI.MeasureVI(20, 10);
					BSTOK_OFF[site] = BST2_FOVI.GetMeasResult(site, MVRET);
				}
				i += -step_v;
				if(flag[0] && flag[1] && flag[2] && flag[3])
					break;
			}
		}

  for(site = 0; site < SITENUM; site++) {
		BSTOK_HYSV[site] = BSTOK_ON[site] - BSTOK_OFF[site];
		BSTOK_HIGH  ->SetTestResult(site, 0, BSTOK_ON[site]);
    BSTOK_LOW   ->SetTestResult(site, 0, BSTOK_OFF[site]);
    BSTOK_HYS   ->SetTestResult(site, 0, BSTOK_HYSV[site] * 1e3);
		
		if(DebugConsole[site])	{	
			fprintf(stderr, "VbstON_S%d	: %3.4f\n", site + 1, BSTOK_ON[site]);
			fprintf(stderr, "VbstOff_S%d	: %3.4f\n", site + 1, BSTOK_OFF[site]);
			fprintf(stderr, "VbstHys_S%d	: %3.4f\n", site + 1, (BSTOK_ON[site] - BSTOK_OFF[site])*1e3);
		}
  }

	PWR0();
	PWROFF();
	delay_ms(1);
	rlyC.SetOn(-1);

  // TODO: Add your function code here
  return 0;
}
// test16
// ****************************************** pContinuity ******************************************
// ****************************************** pContinuity ******************************************
// ****************************************** pContinuity ******************************************
DUT_API int pContinuity(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
    CParam *VIN = StsGetParam(funcindex,"VIN");
    CParam *SW = StsGetParam(funcindex,"SW");
    CParam *BST = StsGetParam(funcindex,"BST");
    CParam *EN = StsGetParam(funcindex,"EN");
    CParam *FB = StsGetParam(funcindex,"FB");
    CParam *SWVIN = StsGetParam(funcindex,"SWVIN");
	//}}AFX_STS_PARAM_PROTOTYPES

	// TODO: Add your function code here
  rlyC.SetOn(-1);
 	delay_ms(1);

	PWR0();
	rlyC.SetOn(VinFPVI, SwFPVI, ENFOVI, BstFOVI, FbFOVI, -1);
	delay_ms(1);

 	// VIN
 	VinFPVI0.Set(FI, -1000.0e-6, FPVI10_2V, FPVI10_1MA, RELAY_ON);
 	delay_ms(1);
 	VinFPVI0.MeasureVI(10, 10);
 	for	(site = 0; site < SITENUM; site++)
		VIN->SetTestResult(site, 0, VinFPVI0.GetMeasResult(site, MVRET));
 	VinFPVI0.Set(FV, float(0), FPVI10_2V, FPVI10_1MA, RELAY_ON);
 	// SW
 	SwFPVI1.Set(FI, -1000.0e-6, FPVI10_2V, FPVI10_1MA, RELAY_ON);
 	delay_ms(1);
 	SwFPVI1.MeasureVI(10, 10);
 	for	(site = 0; site < SITENUM; site++)
		SW->SetTestResult(site, 0, SwFPVI1.GetMeasResult(site, MVRET));
 	SwFPVI1.Set(FV, float(0), FPVI10_2V, FPVI10_1MA, RELAY_ON);
 	// BST
 	BST_FOVI.Set(FI, -1000.0e-6, FOVI_2V, FOVI_1MA, RELAY_ON);
 	delay_ms(1);
 	BST_FOVI.MeasureVI(10, 10);
 	for	(site = 0; site < SITENUM; site++)
		BST->SetTestResult(site, 0, BST_FOVI.GetMeasResult(site, MVRET));
 	BST_FOVI.Set(FV, float(0), FOVI_2V, FOVI_1MA, RELAY_ON);
 	// EN
	EnFOVI.Set(FI, -200.0e-6, FOVI_2V, FOVI_1MA, RELAY_ON);
	////EnFOVI.Set(FV, float(-2), FOVI_2V, FOVI_1MA, RELAY_ON);
 	delay_ms(1);
 	EnFOVI.MeasureVI(10, 10);
 	for	(site = 0; site < SITENUM; site++)
		EN->SetTestResult(site, 0, EnFOVI.GetMeasResult(site, MVRET));
 	EnFOVI.Set(FV, float(0), FOVI_2V, FOVI_1MA, RELAY_ON);
 	// FB
 	FB_FOVI.Set(FI, -1000.0e-6, FOVI_2V, FOVI_1MA, RELAY_ON);
 	delay_ms(1);
 	FB_FOVI.MeasureVI(10, 10);
 	for	(site = 0; site < SITENUM; site++)
		FB->SetTestResult(site, 0, FB_FOVI.GetMeasResult(site, MVRET));
 	FB_FOVI.Set(FV, float(0), FOVI_2V, FOVI_1MA, RELAY_ON);
 	
	rlyC.SetOn(VinFPVI, SwFPVI, -1);
	delay_ms(1);
	// VIN-SW(SW FORCE VOLT)
	VinFPVI0.Set(FV, float(0), FPVI10_2V, FPVI10_10MA, RELAY_ON);
	SwFPVI1.Set(FV, float(2), FPVI10_2V, FPVI10_1MA, RELAY_ON);
 	delay_ms(1);
 	SwFPVI1.MeasureVI(10, 10);
 	for	(site = 0; site < SITENUM; site++)
		SWVIN->SetTestResult(site, 0, SwFPVI1.GetMeasResult(site, MVRET));
 	SwFPVI1.Set(FV, float(0), FPVI10_2V, FPVI10_10MA, RELAY_ON);

  rlyC.SetOn(-1);
 	delay_ms(1);

	return 0;
}

DUT_API int RON(short funcindex, LPCTSTR funclabel)	{
  //{{AFX_STS_PARAM_PROTOTYPES
    CParam *HSRON = StsGetParam(funcindex,"HSRON");
    CParam *HRON_VIN = StsGetParam(funcindex,"HRON_VIN");
    CParam *HRON_SW = StsGetParam(funcindex,"HRON_SW");
    CParam *LSRON = StsGetParam(funcindex,"LSRON");
    CParam *LSRON_VIN = StsGetParam(funcindex,"LSRON_VIN");
    CParam *LSRON_SW = StsGetParam(funcindex,"LSRON_SW");
  //}}AFX_STS_PARAM_PROTOTYPES

  double Load1mAVSW[SITENUM]		= {0.0f, 0.0f, 0.0f, 0.0f},	Load1mAISW[SITENUM]		= {0.0f, 0.0f, 0.0f, 0.0f};
	double Load200mAVSW[SITENUM]	= {0.0f, 0.0f, 0.0f, 0.0f}, Load200mAISW[SITENUM] = {0.0f, 0.0f, 0.0f, 0.0f};
	double Load200mAVIN[SITENUM]	= {0.0f, 0.0f, 0.0f, 0.0f};
	double HSRon[SITENUM]					= {0.0f, 0.0f, 0.0f, 0.0f}, LSRon[SITENUM]				= {0.0f, 0.0f, 0.0f, 0.0f};
  double CheckSWV[SITENUM]			= {0.0f, 0.0f, 0.0f, 0.0f};

	rlyC.SetOn(-1);
	delay_ms(1);
	FreshSiteFlagInit();
	// VIN
	VinFPVI0.Set(FV, 0.0f, FPVI10_5V, FPVI10_10MA, RELAY_ON);
	// SW
	SwFPVI1.Set(FI, 1e-3f, FPVI10_20V, FPVI10_10MA, RELAY_ON);
	// EN
	EnFOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);
	delay_ms(10);

	// RELAY SETTINGS
	rlyC.SetOn(VinFPVI, FbFOVI, ENFOVI, SwFPVI, CapVIN, CBSTSW, -1);
	delay_ms(1);

	// VIN
	VinFPVI0.Set(FV, 0.0f, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	// EN
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	// VIN
	VinFPVI0.Set(FV, 6.0f, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	delay_ms(5);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(5);
	
	// TM0
	FB_FOVI.Set(FV, 6.5f, FOVI_10V, FOVI_1MA, RELAY_ON);
	delay_ms(5);
	FB_FOVI.Set(FV, 0.63f, FOVI_1V, FOVI_1MA, RELAY_ON);
	delay_ms(1);

	// TM1
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	// TM2
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	// TM3
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	// RELAY SETTINGS
	rlyC.SetOn(VinFPVI, FbFOVI, ENFOVI, SwFPVI, CapVIN, CBSTSW, VBSTSW, GNDs, -1);
	delay_ms(1);
	// BST-SW, 100%
	BST2_FOVI.Set(FV, 5.0f, FOVI_20V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	VinFPVI0.Set(FV, 12.0f, FPVI10_20V, FPVI10_1A, RELAY_ON);
	delay_ms(1);
  // ****************************************** HSRON ******************************************
	// ****************************************** HSRON ******************************************
	// ****************************************** HSRON ******************************************
	// 1mA loading
	SwFPVI1.Set(FI, -1.0e-3f, FPVI10_20V, FPVI10_1A, RELAY_ON);
	delay_us(100);
	SwFPVI1.MeasureVI(10, 10);
	for(site = 0; site < SITENUM; site++)	{
		Load1mAVSW[site] = SwFPVI1.GetMeasResult(site, MVRET);
    Load1mAISW[site] = SwFPVI1.GetMeasResult(site, MIRET);
	}
	// 200mA loading
	SwFPVI1.Set(FI, -200.0e-3f, FPVI10_20V, FPVI10_1A, RELAY_ON);
	delay_ms(5);
	SwFPVI1.MeasureVI(10, 10);
	VinFPVI0.MeasureVI(10, 10);
	for(site = 0; site < SITENUM; site++)	{
    Load200mAVSW[site] = SwFPVI1.GetMeasResult(site,MVRET);
    Load200mAISW[site] = SwFPVI1.GetMeasResult(site,MIRET);
    Load200mAVIN[site] = VinFPVI0.GetMeasResult(site,MVRET);
	}
	SwFPVI1.Set(FI, 0.5e-3f, FPVI10_20V, FPVI10_1A, RELAY_ON);
	delay_us(100);
	SwFPVI1.Set(FI, 0.5e-3f, FPVI10_20V, FPVI10_1A, RELAY_OFF);
	delay_us(100);

	for(site = 0; site < SITENUM; site++)	{
		BEGIN_SINGLE_SITE(site);
			HSRon[site] = ( fabs( Load1mAVSW[site] - Load200mAVSW[site] ) / (fabs(Load200mAISW[site] - Load1mAISW[site]) + 0.1e-9f));
		END_SINGLE_SITE();
	}

	for(site = 0; site < SITENUM; site++)	{
		HSRON->SetTestResult(site, 0, HSRon[site]*1e3f);
		HRON_VIN->SetTestResult(site, 0, Load200mAVIN[site]);
		HRON_SW->SetTestResult(site, 0, Load200mAVSW[site]);
	}
	PWR0();
	PWROFF();
	delay_ms(2);

  rlyC.SetOn(-1);
  delay_ms(1);

  // ****************************************** LSRON ******************************************
	// ****************************************** LSRON ******************************************
	// ****************************************** LSRON ******************************************
	VinFPVI0.Set(FV, 0.0f, FPVI10_5V, FPVI10_100MA, RELAY_ON);	// POR, RESET TEST MODE
	SwFPVI1.Set(FI, 1e-3f, FPVI10_20V, FPVI10_10MA, RELAY_ON);
	EnFOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);
	delay_ms(10);
	// RELAY SETTINGS
	rlyC.SetOn(VinFPVI, FbFOVI, ENFOVI, SwFPVI, CapVIN, CBSTSW, -1);
	delay_ms(1);
	// VIN
	VinFPVI0.Set(FV, 0.0f, FPVI10_10V, FPVI10_100MA, RELAY_ON);
	// EN
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	// VIN
	VinFPVI0.Set(FV, 6.0f, FPVI10_20V, FPVI10_100MA, RELAY_ON);
	// EN
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	// TM0
	FB_FOVI.Set(FV, 6.5f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	FB_FOVI.Set(FV, 0.63f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	// TM1
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_us(100);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_us(100);
	// TM2
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_us(100);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_us(100);
	// TM3
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_us(100);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_us(100);
	// TM4
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_us(100);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_us(100);
	// VIN=12
  VinFPVI0.Set(FV, 12.0f, FPVI10_20V, FPVI10_1A, RELAY_ON, 5);
	delay_ms(1);

	SwFPVI1.MeasureVI(10, 20);
	for(site = 0; site < SITENUM; site++)
	  CheckSWV[site] =  SwFPVI1.GetMeasResult(site,MVRET);

	SwFPVI1.Set(FI, 1.0e-3f, FPVI10_20V, FPVI10_1A, RELAY_ON);
	delay_ms(1);
	SwFPVI1.MeasureVI(10, 20);
	for(site = 0; site < SITENUM; site++)	{
		Load1mAVSW[site] = SwFPVI1.GetMeasResult(site, MVRET);
    Load1mAISW[site] = SwFPVI1.GetMeasResult(site, MIRET);
	}

	SwFPVI1.Set(FI, 200.0e-3f, FPVI10_20V, FPVI10_1A, RELAY_ON);
	delay_us(100);
	VinFPVI0.MeasureVI(10, 20);
	SwFPVI1.MeasureVI(10, 20);
	for(site = 0; site < SITENUM; site++)	{
		Load200mAVSW[site] = SwFPVI1.GetMeasResult(site, MVRET);
    Load200mAISW[site] = SwFPVI1.GetMeasResult(site, MIRET);
    Load200mAVIN[site] = VinFPVI0.GetMeasResult(site, MVRET);
	}
	SwFPVI1.Set(FI, 1.0e-12f, FPVI10_20V, FPVI10_1A, RELAY_ON);
	delay_ms(1);

	for(site = 0; site < SITENUM; site++)	{
		BEGIN_SINGLE_SITE(site);
      LSRon[site] = ( fabs( Load1mAVSW[site] - Load200mAVSW[site] ) / (fabs(Load200mAISW[site] - Load1mAISW[site]) + 0.1e-9f));
		END_SINGLE_SITE();
	}

	for(site = 0; site < SITENUM; site++)	{
		LSRON->SetTestResult(site, 0, LSRon[site]*1e3f);
    LSRON_VIN->SetTestResult(site, 0, Load200mAVIN[site]);
    LSRON_SW->SetTestResult(site, 0,  Load200mAVSW[site]);
	}
	SwFPVI1.Set(FI, 0.0e-12f, FPVI10_20V, FPVI10_1A, RELAY_ON);
	delay_ms(1);
	EnFOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);
	delay_us(100);

	PWR0();
	PWROFF();
	delay_ms(2);

	rlyC.SetOn(-1);
 	delay_ms(1);

  // TODO: Add your function code here
  return 0;
}


DUT_API int IPK(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
    CParam *IPK1 = StsGetParam(funcindex,"IPK1");
    CParam *IPK2 = StsGetParam(funcindex,"IPK2");
    CParam *IPK_TESTMODE = StsGetParam(funcindex,"IPK_TESTMODE");
    CParam *IPKSW = StsGetParam(funcindex,"IPKSW");
    CParam *IPKVIN = StsGetParam(funcindex,"IPKVIN");
    CParam *IEN = StsGetParam(funcindex,"IEN");
    CParam *IEN0 = StsGetParam(funcindex,"IEN0");
    CParam *IEN1 = StsGetParam(funcindex,"IEN1");
    CParam *IPK0_TESTMODE = StsGetParam(funcindex,"IPK0_TESTMODE");
    CParam *IPK0VIN = StsGetParam(funcindex,"IPK0VIN");
    CParam *IPK0SW = StsGetParam(funcindex,"IPK0SW");
    CParam *RIPK = StsGetParam(funcindex,"RIPK");
  //}}AFX_STS_PARAM_PROTOTYPES
	short SetV = 0;
	double StepV = 0.0f;
	// IPK
	double RIpk[SITENUM]		= { 0.0f };
	double Ipk[SITENUM]			= { 0.0f };
	double Ipkvin[SITENUM]	= { 0.0f };
	double Ipksw[SITENUM]		= { 0.0f };
	// IPK0
	double Ipk0[SITENUM]		= { 0.0f };
	double Ipk0vin[SITENUM] = { 0.0f };
	double Ipk0sw[SITENUM]	= { 0.0f };
	// IEN
	double ien[SITENUM]			= { 0.0f };
	double ien0[SITENUM]		= { 0.0f };
	double ien1[SITENUM]		= { 0.0f };

	double IVIN_C[4]={0.0f};
	double VSW_V[4]={0.0f};

	rlyC.SetOn(-1);
	FreshSiteFlagInit();
	// VIN
	VinFPVI0.Set(FV, 0.0f, FPVI10_5V, FPVI10_10MA, RELAY_ON);
	// SW
	SwFPVI1.Set(FI, 1e-3f, FPVI10_20V, FPVI10_10MA, RELAY_ON);
	// EN
	EnFOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);
	delay_ms(10);

	// RELAY SETTINGS
	rlyC.SetOn(VinFPVI, FbFOVI, ENFOVI, SwFPVI, CapVIN, CBSTSW, -1);
	delay_ms(1);
	// VIN
	VinFPVI0.Set(FV, 0.0f, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	// EN
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(10);

	// VIN=5
	VinFPVI0.Set(FV, 5.0f, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	delay_ms(1);
	// EN=5
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	// TM0
	FB_FOVI.Set(FV, 6.5f, FOVI_10V, FOVI_1MA, RELAY_ON);
	delay_ms(1);
	FB_FOVI.Set(FV, 0.63f, FOVI_1V, FOVI_1MA, RELAY_ON);
	delay_ms(1);

	// TM1
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	// TM2
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	// TM3
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	// RELAY SETTINGS
	rlyC.SetOn(VinFPVI, FbFOVI, ENFOVI, SwFPVI, CapVIN, CBSTSW, VBSTSW, GNDs, -1);
	delay_ms(1);
	// BST-SW=5
	BST2_FOVI.Set(FV, 5.0f, FOVI_20V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	// VIN=12
	VinFPVI0.Set(FV, 12.0f, FPVI10_20V, FPVI10_10A, RELAY_ON);
	delay_ms(1);

  VinFPVI0.MeasureVI(20, 20);
  SwFPVI1.MeasureVI(20, 20);
  for (site = 0; site < SITENUM; site++)	{
		IVIN_C[site] = VinFPVI0.GetMeasResult(site, MIRET)*1.0e3f;
		VSW_V[site] = SwFPVI1.GetMeasResult(site, MVRET);
	}
	// 50mA LOADING
  SwFPVI1.Set(FI, -50.0e-3f, FPVI10_20V, FPVI10_10A, RELAY_ON);
  delay_ms(1);
  VinFPVI0.MeasureVI(20, 20);
  SwFPVI1.MeasureVI(20, 20);
  for (site = 0; site < SITENUM; site++)	{
		IVIN_C[site] = VinFPVI0.GetMeasResult(site, MIRET)*1.0e3f;
		VSW_V[site] = SwFPVI1.GetMeasResult(site, MVRET);
  }
  
	for (site = 0; site < SITENUM; site++)	{
		flag[site]		= 1;
    Ipk[site]			= 0.0f;
	  Ipkvin[site]	= 0.0f;
		Ipksw[site]		= 0.0f;
  }
	FreshSiteFlagInit();
	
	for (site = 0; site < SITENUM; site++)	{
		if (IVIN_C[site] >= 50.0f  &&  VSW_V[site] >= 11.5f)	{
			for (StepV = -0.01f; StepV >= -0.6f; StepV -= 0.1f)	{
				SwFPVI1.Set(FI, StepV, FPVI10_20V, FPVI10_10A, RELAY_ON);
				delay_ms(1);
			}
			// SW 600mA LOADING
			SwFPVI1.Set(FI, -0.6f, FPVI10_20V, FPVI10_10A, RELAY_ON);
			delay_ms(1);
			// VIN=12
			VinFPVI0.Set(FV, 12.0f, FPVI10_20V, FPVI10_10A, RELAY_ON, 5);
			delay_ms(1);
			// SINK 600mA TO 2000mA LOADING
			for (StepV = -0.6f; StepV >= -2.0f; StepV -= 0.01f)	{
				SetV = 0;
				for (site = 0; site < SITENUM; site++)	{
					BEGIN_SINGLE_SITE(site);
						if (flag[site] == 0)	{
							SetV = 1;
							SwFPVI1.Set(FI, StepV, FPVI10_20V, FPVI10_10A, RELAY_ON);
						}
					END_SINGLE_SITE();
				}
				delay_us(10);
				// MEASUREMENT
				FB_FOVI.MeasureVI(20, 20);
				VinFPVI0.MeasureVI(20, 20);
				SwFPVI1.MeasureVI(20, 20);

				for (site = 0; site < SITENUM; site++)	{
					if (flag[site] == 0)	{
						adresult[site] = FB_FOVI.GetMeasResult(site, MVRET);		// GET FB VOLTAGE
						Ipkvin[site] = VinFPVI0.GetMeasResult(site, MVRET);			// GET VIN VOLTAGE
						Ipksw[site] = SwFPVI1.GetMeasResult(site, MVRET);				// GET SW VOLTAGE
						delay_us(10);
						if (adresult[site] <= 0.35f && flag[site] == 0)	{
							Ipk[site] = -StepV;
							flag[site] = 1;
						}
						else
							Ipk[site] = 99.0f;
						}
					}
					if (SetV == 0)
						break;
				}
			}
		}
		
		for(site = 0; site < SITENUM; site++ )	{
			IPK_TESTMODE->SetTestResult(site, 0, Ipk[site]);
			IPKSW				->SetTestResult(site, 0, Ipksw[site]);
			IPKVIN			->SetTestResult(site, 0, Ipkvin[site]);
		}
		// SW, EN, BS-SW
		SwFPVI1.Set(FI, 1e-6f, FPVI10_20V, FPVI10_1A, RELAY_ON,5);
		delay_ms(2);
		EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
		BST2_FOVI.Set(FV, 0.0f, FOVI_20V, FOVI_10MA, RELAY_ON);
		delay_ms(1);
		// VIN, SW, EN, SW
		VinFPVI0.Set(FV, 0.0f, FPVI10_5V, FPVI10_1A, RELAY_ON);
		SwFPVI1.Set(FV, 0.0f, FPVI10_20V, FPVI10_1A, RELAY_ON);
		EnFOVI.Set(FV, 0.0f, FOVI_5V, FOVI_100MA, RELAY_ON);
		SwFPVI1.Set(FI, 0.0f, FPVI10_20V, FPVI10_1A, RELAY_ON);
		delay_ms(10);

		rlyC.SetOn(-1);
		delay_ms(1);
		// VIN=0
		VinFPVI0.Set(FV, 0.0f, FPVI10_5V, FPVI10_10MA, RELAY_ON);
		SwFPVI1.Set(FI, 1e-3f, FPVI10_20V, FPVI10_10MA, RELAY_ON);
		EnFOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);
		delay_ms(10);

		// RELAY SETTINGS
		rlyC.SetOn(VinFPVI, FbFOVI, ENFOVI, SwFPVI, CapVIN, CBSTSW, VBSTSW, GNDs, -1);
		delay_ms(1);

		// VIN, EN
		VinFPVI0.Set(FV, 0.0f, FPVI10_10V, FPVI10_10MA, RELAY_ON);
		EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
		delay_ms(1);
		// TM0
		VinFPVI0.Set(FV, 5.0f, FPVI10_10V, FPVI10_10MA, RELAY_ON);
		delay_ms(1);
		EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
		delay_ms(1);
		FB_FOVI.Set(FV, 6.5f, FOVI_10V, FOVI_1MA, RELAY_ON);
		delay_ms(3);
		FB_FOVI.Set(FV, 0.63f, FOVI_10V, FOVI_1MA, RELAY_ON); 
		delay_ms(1);

		// TM1
		EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
		delay_ms(1);
		EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
		delay_ms(1);
		// TM2
		EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
		delay_ms(1);
		EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
		delay_ms(1);
		// TM3
		EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
		delay_ms(1);

		// VIN=12
		VinFPVI0.Set(FV, 12.0f, FPVI10_20V, FPVI10_1A, RELAY_ON);
		delay_ms(1);
		// BST-SW=5
		BST2_FOVI.Set(FV, 5.0f, FOVI_20V, FOVI_10MA, RELAY_ON);
		delay_ms(1);
	
   VinFPVI0.MeasureVI(20, 20);
   SwFPVI1.MeasureVI(20, 20);
   for (site = 0; site < SITENUM; site++)	{
		 IVIN_C[site] = VinFPVI0.GetMeasResult(site, MIRET)*1.0e3f;
		 VSW_V[site] = SwFPVI1.GetMeasResult(site, MVRET);
   }
   // 50mA LOADING
	 SwFPVI1.Set(FI, -50.0e-3f, FPVI10_20V, FPVI10_1A, RELAY_ON);
   delay_ms(1);
   VinFPVI0.MeasureVI(20, 20);
   SwFPVI1.MeasureVI(20, 20);
   for (site = 0; site<SITENUM; site++)	{
		 IVIN_C[site] = VinFPVI0.GetMeasResult(site, MIRET)*1.0e3f;
		 VSW_V[site] = SwFPVI1.GetMeasResult(site, MVRET);
   }

	 for (site = 0; site < SITENUM; site++)	{
		 flag[site] = 1;
     Ipk0[site] = 0.0f;
   }
	 FreshSiteFlagInit();

	 for (site = 0; site < SITENUM; site++)	{
		 if (IVIN_C[site] >= 50.0f  &&  VSW_V[site] >= 11.5f)	{
			 for (StepV = 0.1f; StepV <= 0.5f; StepV += 0.1f)	{
				 SwFPVI1.Set(FI, StepV, FPVI10_20V, FPVI10_1A, RELAY_ON);
				 delay_ms(1);
			 }

			 for (StepV = 0.5f; StepV >= -0.5f; StepV -= 0.01f)	{
				 SetV = 0;
				 for (site = 0; site < SITENUM; site++)	{
					 BEGIN_SINGLE_SITE(site);
						if (flag[site] == 0)	{
							SetV = 1;
							SwFPVI1.Set(FI, StepV, FPVI10_20V, FPVI10_1A, RELAY_ON);
						}
					 END_SINGLE_SITE();
				 }
				 delay_us(10);
				 FB_FOVI.MeasureVI(20, 20);
				 VinFPVI0.MeasureVI(20, 20);
				 SwFPVI1.MeasureVI(20, 20);
				 for (site = 0; site<SITENUM; site++)	{
					 if (flag[site] == 0)	{
						 adresult[site] = FB_FOVI.GetMeasResult(site, MVRET);		// GET FB VOLT
						 Ipk0vin[site] = VinFPVI0.GetMeasResult(site, MVRET);		// GET VIN VOLT
						 Ipk0sw[site] = SwFPVI1.GetMeasResult(site, MVRET);			// GET SW VOLT
						 delay_us(10);
					
						 if (adresult[site] <= 0.35f && flag[site] == 0)	{
							 Ipk0[site] = -StepV;
							 flag[site] = 1;
						 }
						 else Ipk0[site] = 99.0f;
					 }
			   }
				 if (SetV == 0)
					 break;
			}
		}
	}
	for(site=0; site<SITENUM; site++ )	{
		IPK0_TESTMODE->SetTestResult(site, 0, Ipk0[site]);
		IPK0SW->SetTestResult(site, 0, Ipk0sw[site]);
		IPK0VIN->SetTestResult(site, 0, Ipk0vin[site]);
	}
	for(StepV = -0.5f; StepV <= 0.0f; StepV = StepV + 0.1f )	{
		SwFPVI1.Set(FI, StepV, FPVI10_20V, FPVI10_1A, RELAY_ON);
		delay_ms(2);
	}
	delay_ms(1);

	PWR0();
	PWROFF();

	rlyC.SetOn(-1);
	delay_ms(1);
	// RELAY SETTINGS
	rlyC.SetOn(VinFPVI,FbFOVI,ENFOVI,CBSTSW,SwFPVI,-1);
	delay_ms(2);

	// VIN=12
	VinFPVI0.Set(FV, 12.0f, FPVI10_20V, FPVI10_10MA, RELAY_ON);
	delay_ms(1);
	// SW
	SwFPVI1.Set(FI, 1.0e-3f, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	delay_ms(2);
	// VIN=12
	VinFPVI0.Set(FV, 12.0f, FPVI10_20V, FPVI10_100MA, RELAY_ON);
	delay_ms(10);
	// FB
	FB_FOVI.Set(FV, 0.0f, FOVI_1V, FOVI_1MA, RELAY_ON);
	
	// EN
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_1MA, RELAY_ON);
	delay_ms(1);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_100UA, RELAY_ON);
	delay_ms(1);

	for(site = 0; site < SITENUM; site++)	{
		flag[site]=1;
		if(sitesta[site])
			flag[site]=0;
	}
	EnFOVI.MeasureVI(10, 20);
	for(site=0; site<SITENUM; site++ )	{
		adresult[site] = EnFOVI.GetMeasResult(site,MIRET);
		ien[site]=adresult[site]*1e6f;
		IEN->SetTestResult(site, 0, ien[site]);
	}
	// RELAY SETTINGS
	rlyC.SetOn(VinFPVI, FbFOVI, ENFOVI, SwFPVI, CBSTSW, VBSTSW, GNDs , -1);
	delay_ms(1);
	// BST-SW=5, 
	BST2_FOVI.Set(FV, 5.0f, FOVI_20V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	FB_FOVI.Set(FV, 6.5f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	EnFOVI.MeasureVI(30, 10);
	for(site=0; site<SITENUM; site++ )	{
		adresult[site] = EnFOVI.GetMeasResult(site,MIRET,MIN_RESULT);
		ien0[site]=adresult[site]*1e6f;
		IEN0->SetTestResult(site, 0, ien0[site]);
	}
	delay_ms(1);
	// FB=0
	FB_FOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	EnFOVI.MeasureVI(30, 10);
	for(site = 0; site < SITENUM; site++ )	{
		adresult[site] = EnFOVI.GetMeasResult(site, MIRET);
		ien1[site]=adresult[site]*1e6f;
		IEN1->SetTestResult(site, 0, ien1[site]);
	}
	// *** ALIAS ************
	// IEN	= IEN5V
	// IEN0 = IPKREF0
	// IEN1 = IPKREF
	// CURRENT RATIO FORMULA: RIPK = (IPKREF - IEN5V)/(IPKREF0 - IEN5V)
	// IPK = I
	// IPK0 = IPK0
	// PeakCurrent = {IPK0 + RIPK * (I - IPK0)} + OFFSET
	for(site=0; site<SITENUM; site++ )	{
		RIpk[site] = (ien1[site] - ien[site]) / (fabs(ien0[site] - ien[site] + 0.1e-15f));
		IPK1->SetTestResult(site, 0, (RIpk[site] * (Ipk[site] - Ipk0[site]) + Ipk0[site] + 0.45f) );
		RIPK->SetTestResult(site, 0, RIpk[site]);
	}
	PWR0();
	PWROFF();
	delay_ms(1);

  rlyC.SetOn(-1);
 	delay_ms(1);

  // TODO: Add your function code here
  return 0;
}


DUT_API int CurrentLimit(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
    CParam *Ivalley = StsGetParam(funcindex,"Ivalley");
    CParam *Izero = StsGetParam(funcindex,"Izero");
    CParam *IVALLEY_TESTMODE = StsGetParam(funcindex,"IVALLEY_TESTMODE");
    CParam *IVALLEY0_TESTMODE = StsGetParam(funcindex,"IVALLEY0_TESTMODE");
    CParam *SW_LEAK = StsGetParam(funcindex,"SW_LEAK");
    CParam *INEG = StsGetParam(funcindex,"INEG");
  //}}AFX_STS_PARAM_PROTOTYPES


 int SetV=0;

 double StepV = 0.0f;
 double Izero1[4] = {0.0f}, Climit0[4] = {0.0f};

 int flag[SITENUM] = {0};
 BYTE sitesta[SITENUM] = {0};


	double Climit[4]={0.0f};

  rlyC.SetOn(-1);
	delay_ms(1);
	FreshSiteFlagInit();

	VinFPVI0.Set(FV, 0.0f, FPVI10_5V, FPVI10_100MA, RELAY_ON);	//VIN
	SwFPVI1.Set(FI, 1e-3f, FPVI10_20V, FPVI10_100MA, RELAY_ON);	//SW
	EnFOVI.Set(FV, 0.0f, FOVI_5V, FOVI_100MA, RELAY_ON);		//EN
	delay_ms(10);


	rlyC.SetOn(VinFPVI, FbFOVI, ENFOVI, SwFPVI, CapVIN, CBSTSW, -1);//CHANNEL1//BATT_Fpvi,NTC_Fovi,NTC_Fovi,
	delay_ms(10);//

	VinFPVI0.Set(FV, 0.0f, FPVI10_10V, FPVI10_100MA, RELAY_ON);
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(10);


	VinFPVI0.Set(FV, 0.0f, FPVI10_10V, FPVI10_100MA, RELAY_ON);
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);



//===== VFB =====

	VinFPVI0.Set(FV, 5.0f, FPVI10_10V, FPVI10_100MA, RELAY_ON);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	FB_FOVI.Set(FV, 6.5f, FOVI_10V, FOVI_1MA, RELAY_ON); //TM0
	delay_ms(1);
	FB_FOVI.Set(FV, 0.63f, FOVI_10V, FOVI_1MA, RELAY_ON); 
	delay_ms(1);
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);	//TM1
	delay_ms(1);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);	//TM2
	delay_ms(1);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);	//TM3
	delay_ms(1);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);	//TM4
	delay_ms(1);

	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	FB_FOVI.Set(FI, 10e-6f, FOVI_10V, FOVI_100UA, RELAY_ON); 
	delay_ms(1);
	rlyC.SetOn(VinFPVI, FbFOVI, ENFOVI, SwFPVI, CapVIN, CBSTSW, VBSTSW, GNDs, -1);//CHANNEL1//BATT_Fpvi,NTC_Fovi,NTC_Fovi,
	delay_ms(1);//
	BST2_FOVI.Set(FV, 5.0f, FOVI_20V, FOVI_10MA, RELAY_ON);		//BST
	delay_ms(1);//

    VinFPVI0.Set(FV, 12.0f, FPVI10_20V, FPVI10_1A, RELAY_ON);
    delay_ms(1);

//RONLS


 //===== ICLHS =====
    for (site = 0; site<SITENUM; site++)
    {
     flag[site] = 1;;
     Climit[site] = 0.0f;
     if (sitesta[site])
      flag[site] = 0;;
    }

   for (StepV = -0.0f; StepV >= -0.5f; StepV -= 0.1f)
    {
       SwFPVI1.Set(FI, StepV, FPVI10_5V, FPVI10_1A, RELAY_ON);
    }
     VinFPVI0.Set(FV, 12.0f, FPVI10_20V, FPVI10_10A, RELAY_ON);
    delay_ms(1);
     SwFPVI1.Set(FI, -0.5, FPVI10_5V, FPVI10_10A, RELAY_ON);
	delay_ms(1);
 
	for (StepV = -0.5f; StepV >= -2.0f; StepV -= 0.01f)////1.0375//1.55
    {
     SetV = 0;
     for (site = 0; site<SITENUM; site++)
     {
      BEGIN_SINGLE_SITE(site);
      if (flag[site]<0.5f)
      {
       SetV = 1;
       SwFPVI1.Set(FI, StepV, FPVI10_20V, FPVI10_10A, RELAY_ON);
      }
      END_SINGLE_SITE();
 
     }
     delay_us(100);//500
     //SW.MeasureVI(10, 10);//20,20
   //  FB_FOVI.MeasureVI(20, 20);//50,20
	 FB_FOVI.MeasureVI(20, 20);//50,20
     for (site = 0; site<SITENUM; site++)
     {
      if (flag[site] == 0)
      {
       // adresult[i] = SW.GetMeasResult(i,MVRET);
    //   adresult[i] = FB_FOVI.GetMeasResult(i, MVRET);
       adresult[site] = FB_FOVI.GetMeasResult(site, MVRET);

       if (adresult[site] <= 4.0f && flag[site]<0.5f)
       {
        Climit[site] = -StepV;
        flag[site] = 1;;
       }
   else    Climit[site] = 99.0f;
      }
     }
     if (SetV == 0) break;
    }


	for(site=0; site<SITENUM; site++ )
	{
	 //Ivalley->SetTestResult(i, 0, Climit[i]);	//FT & EVB OFFSET
	 IVALLEY_TESTMODE->SetTestResult(site, 0, Climit[site]);
	}


	for(StepV=-1.0f; StepV<=0.0f; StepV=StepV+0.1f )
	{
		SwFPVI1.Set(FI, StepV, FPVI10_10V, FPVI10_10A, RELAY_ON);
		delay_us(100);
	}
	delay_ms(1);


///////////
 	VinFPVI0.Set(FV, 0.0f, FPVI10_5V, FPVI10_1A, RELAY_ON);	//VIN
	SwFPVI1.Set(FV, 0.0f, FPVI10_5V, FPVI10_1A, RELAY_ON);	//SW
	EnFOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);		//EN

	FB_FOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);		//FB

 	BST_FOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);		//BST
	BST2_FOVI.Set(FV, 0.0f, FOVI_20V, FOVI_10MA, RELAY_ON);		//BST
	delay_ms(1);//

    rlyC.SetOn(-1);
 	delay_ms(1);


/////IVALLEY0_TESTMODE


	for (site = 0; site<SITENUM; site++)
     {
      BEGIN_SINGLE_SITE(site);	

	rlyC.SetOn(VinFPVI,FbFOVI,ENFOVI,SwFPVI,CapVIN,CBSTSW,BstFOVI,-1);//CHANNEL1//BATT_Fpvi,NTC_Fovi,NTC_Fovi,MODE_Ovi
	delay_ms(1);//

 	VinFPVI0.Set(FV, 0.0f, FPVI10_5V, FPVI10_100MA, RELAY_ON);	//VIN
	EnFOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);		//EN
	delay_ms(1);
	FB_FOVI.Set(FV, 0.0f, FOVI_5V, FOVI_1MA, RELAY_ON);		//FB
	delay_ms(1);

	VinFPVI0.Set(FV, 0.0f, FPVI10_10V, FPVI10_100MA, RELAY_ON);
	SwFPVI1.Set(FI, 1e-3f, FPVI10_5V, FPVI10_100MA, RELAY_ON);	//SW
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

//===== VFB =====

	VinFPVI0.Set(FV, 4.5f, FPVI10_10V, FPVI10_100MA, RELAY_ON);
	EnFOVI.Set(FV, 4.5f, FOVI_10V, FOVI_10MA, RELAY_ON);
	FB_FOVI.Set(FV, 6.5f, FOVI_10V, FOVI_1MA, RELAY_ON); //TM0
	delay_ms(3);
	delay_ms(1);
	FB_FOVI.Set(FV, 0.63f, FOVI_10V, FOVI_1MA, RELAY_ON); //TM0
	delay_ms(3);
	


	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);	//TM1
	delay_ms(1);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);	//TM2
	delay_ms(1);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);	//TM3
	delay_ms(1);

	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
		

	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);	//TM4
	delay_ms(1);

	FB_FOVI.Set(FI, 10e-6f, FOVI_10V, FOVI_100UA, RELAY_ON); 
	delay_ms(1);

	rlyC.SetOn(VinFPVI, FbFOVI, ENFOVI, SwFPVI, CapVIN, CBSTSW, VBSTSW, GNDs, -1);//CHANNEL1//BATT_Fpvi,NTC_Fovi,NTC_Fovi,
	delay_ms(1);//
	BST2_FOVI.Set(FV, 5.0f, FOVI_20V, FOVI_10MA, RELAY_ON);		//BST

    VinFPVI0.Set(FV, 12.0f, FPVI10_20V, FPVI10_1A, RELAY_ON);
    delay_ms(1);




//RONLS


 //===== ICLHS =====

     flag[site] = 1;;
     Climit0[site] = 0.0f;
     if (sitesta[site])
      flag[site] = 0;;
   

		
   for (StepV = 0.0f; StepV <= 0.5f; StepV += 0.1f)
    {
       SwFPVI1.Set(FI, StepV, FPVI10_5V, FPVI10_1A, RELAY_ON);
			 delay_us(100);
    }
	
 
	for (StepV = 0.5f; StepV >= -0.5f; StepV -= 0.01f)////1.0375//1.55
    {
     SetV = 0;
      if (flag[site]<0.5f)
      {
       SetV = 1;
       SwFPVI1.Set(FI, StepV, FPVI10_20V, FPVI10_1A, RELAY_ON);
      }

     delay_us(100);//500

	 FB_FOVI.MeasureVI(20, 20);//50,20

      if (flag[site] == 0)
      {
       adresult[site] = FB_FOVI.GetMeasResult(site, MVRET);
       if (adresult[site] <= 2.0f && flag[site]<0.5f)
       {
        Climit0[site] = -StepV;
        flag[site] = 1;;
       }
				 else    Climit0[site] = 99.0f;
      }
     if (SetV == 0) break;
    }


	 Ivalley->SetTestResult(site, 0, (2.56*(Climit[site]-Climit0[site]))+Climit0[site]);	//FT & EVB OFFSET
	 IVALLEY0_TESTMODE->SetTestResult(site, 0, Climit0[site]);

	for(StepV=-0.5f; StepV<=0.0f; StepV=StepV+0.1f )
	{
		SwFPVI1.Set(FI, StepV, FPVI10_10V, FPVI10_1A, RELAY_ON);
		delay_us(100);
	}



///////////
 	VinFPVI0.Set(FV, 0.0f, FPVI10_5V, FPVI10_1A, RELAY_ON);	//VIN
	SwFPVI1.Set(FV, 0.0f, FPVI10_5V, FPVI10_1A, RELAY_ON);	//SW
	EnFOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);		//EN

	FB_FOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);		//FB

 	BST_FOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);		//BST
	BST2_FOVI.Set(FV, 0.0f, FOVI_20V, FOVI_10MA, RELAY_ON);		//BST
	delay_ms(1);//

      rlyC.SetOn(-1);
 	delay_ms(1);

	END_SINGLE_SITE(); 
    }

///////////Izero

        rlyC.SetOn(-1);
     delay_ms(1);
     StsGetSiteStatus(sitesta,SITENUM);

 	VinFPVI0.Set(FV, 0.0f, FPVI10_5V, FPVI10_10MA, RELAY_ON);	//VIN
	SwFPVI1.Set(FI, 1e-3f, FPVI10_5V, FPVI10_1A, RELAY_ON);	//SW
	EnFOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);		//EN

	FB_FOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);		//FB


	rlyC.SetOn(VinFPVI,FbFOVI,ENFOVI,SwFPVI,CapVIN,-1);//CHANNEL1//BATT_Fpvi,NTC_Fovi,NTC_Fovi,MODE_Ovi
	delay_ms(1);//

//	SwFPVI1.Set(FI, -1e-3f, FPVI10_5V, FPVI10_1A, RELAY_ON);	//SW
//	FOVI_NTC.Set(FV, 3.0f, FOVI_10V, FOVI_1MA, RELAY_ON);
 	VinFPVI0.Set(FV, 0.0f, FPVI10_5V, FPVI10_10MA, RELAY_ON);	//VIN
	EnFOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);		//EN

	FB_FOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);		//FB
//	FOVI_VCC.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);		//VCC
 //	BST_FOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);		//BST


////VFB1
//	FB_FOVI.Set(FV, 0.7f, FOVI_10V, FOVI_10MA, RELAY_ON); 
	VinFPVI0.Set(FV, 0.0f, FPVI10_10V, FPVI10_1A, RELAY_ON);
	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

//===== VFB =====

	VinFPVI0.Set(FV, 5.0f, FPVI10_10V, FPVI10_1A, RELAY_ON);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	FB_FOVI.Set(FV, 6.5f, FOVI_10V, FOVI_1MA, RELAY_ON); //TM0
	delay_ms(3);

	FB_FOVI.Set(FV, 0.63f, FOVI_10V, FOVI_1MA, RELAY_ON); 
	delay_ms(1);



	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);	//TM1
	delay_ms(1);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);	//TM2
	delay_ms(1);
	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
//	rlyC.SetOn(VinFPVI,FbFOVI,ENFOVI,SwFPVI,IOLIM_Fovi,CapVIN,VCC_Cap,BstFOVI,-1);//CHANNEL1//BATT_Fpvi,NTC_Fovi,NTC_Fovi,
	delay_ms(1);//,CBSTSW

	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);	//TM3
	delay_ms(1);

	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);




	EnFOVI.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);	//TM4
	delay_ms(1);

	EnFOVI.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	FB_FOVI.Set(FI, 1e-6f, FOVI_10V, FOVI_10UA, RELAY_ON); 
	delay_ms(1);


    VinFPVI0.Set(FV, 12.0f, FPVI10_20V, FPVI10_1A, RELAY_ON);
    delay_ms(1);
//	BST_FOVI.Set(FV, 5.0f, FOVI_20V, FOVI_10MA, RELAY_ON);		//BST
	delay_ms(1);


//Izero1
 //===== Izero1 =====
    for (site = 0; site<SITENUM; site++)
    {
     flag[site] = 1;;
     Izero1[site] = 0.0f;
     if (sitesta[site])
      flag[site] = 0;;
    }

    for (StepV = -0.3f; StepV <= 0.3f; StepV += 0.01f)
    {
     SetV = 0;
     for (site = 0; site<SITENUM; site++)
     {
      BEGIN_SINGLE_SITE(site);
      if (flag[site]<0.5f)
      {
       SetV = 1;
       SwFPVI1.Set(FI, StepV, FPVI10_20V, FPVI10_1A, RELAY_ON);
      }
      END_SINGLE_SITE();
 
     }
     delay_ms(1);//500
     //SW.MeasureVI(10, 10);//20,20
     FB_FOVI.MeasureVI(20, 20);//50,20
//	 FOVI_IOLIM.MeasureVI(20, 20);//50,20
     for (site = 0; site<SITENUM; site++)
     {
      if (flag[site] == 0)
      {
       // adresult[i] = SW.GetMeasResult(i,MVRET);
       adresult[site] = FB_FOVI.GetMeasResult(site, MVRET);
    //   adresult[i] = FOVI_IOLIM.GetMeasResult(i, MVRET);

       if (adresult[site] <= 1.0f && flag[site]<0.5f)
       {
        Izero1[site] = -StepV;
        flag[site] = 1;;
       }
			 else    Izero1[site] = 99.0f;
      }
     }
     if (SetV == 0) break;
    }


	for(site=0; site<SITENUM; site++ )
	{
	 Izero->SetTestResult(site, 0, Izero1[site]);	//FT & EVB OFFSET
	}


	for(StepV=-0.5f; StepV<=0.0f; StepV=StepV+0.1f )
	{
		SwFPVI1.Set(FI, StepV, FPVI10_10V, FPVI10_1A, RELAY_ON);
		delay_us(100);
	}
	delay_ms(1);



///////////INEG


/////////
	EnFOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);		//EN
 	VinFPVI0.Set(FV, 0.0f, FPVI10_5V, FPVI10_10MA, RELAY_ON);	//VIN
	SwFPVI1.Set(FV, 0.0f, FPVI10_5V, FPVI10_10MA, RELAY_ON);	//SW
	FB_FOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);		//FB
 	BST_FOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);		//BST
	BST2_FOVI.Set(FV, 0.0f, FOVI_20V, FOVI_10MA, RELAY_ON);		//BST
	delay_ms(1);//


	EnFOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_OFF);		//EN
 	VinFPVI0.Set(FV, 0.0f, FPVI10_5V, FPVI10_10MA, RELAY_OFF);	//VIN
	SwFPVI1.Set(FV, 0.0f, FPVI10_5V, FPVI10_10MA, RELAY_OFF);	//SW
	FB_FOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_OFF);		//FB
 	BST_FOVI.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_OFF);		//BST
	BST2_FOVI.Set(FV, 0.0f, FOVI_20V, FOVI_10MA, RELAY_OFF);		//BST
	delay_ms(1);//

      rlyC.SetOn(-1);
 	delay_ms(1);




    // TODO: Add your function code here
    return 0;
}
