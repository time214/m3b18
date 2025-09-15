// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__3811CD50_B7B0_42B9_9E73_805A91708537__INCLUDED_)
#define AFX_STDAFX_H__3811CD50_B7B0_42B9_9E73_805A91708537__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

#define DUT_API extern "C" __declspec(dllexport)
#include <string>
using namespace std;
#include "usertype.h"
#include "userres.h"

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__3811CD50_B7B0_42B9_9E73_805A91708537__INCLUDED_)
// XB18_MT8952NSLR(FC-SOT563)
// TEST FLOW ***********************************************
// test0	: SiteNoChk
// test1	: ContactChk
// test2	: Continuity
// test3	: SupplyCurrent
// test4	: HSide
// test5	: LSide
// test6	: Switching1
// test7	: Switching2
// test8	: RegulatorV
// test9	: CurrentFB
// test10 : SoftStartTime
// test11 : FucnEN1
// test12 : FuncEN2
// test13	: UVLO
// test14	: OVP
// test15	: BOOST
// test16	: pContinuity
// ******************************************************************* PIN DESCTIPTION ST *******************************************************************
// PACKAGE TYPE: FC-SOT563
// PIN01:	VIN
// PIN02:	SW
// PIN03:	GND
// PIN04:	BST
// PIN05:	EN (Non-K)
// PIN06:	FB (Non-K)
// ******************************************************************* PIN DESCTIPTION END *******************************************************************
// ******************************************************************* RELAY CONTROL ******************************************************************
// IDC1, SITE1
// IDC2, SITE2
// IDC3, SITE3
// IDC4, SITE4
#define CapVIN					  0, ( 0 + 32), ( 0 + 64), ( 0 + 96)	// K1			CAP
#define ENQTMUa 	  		  2, ( 2 + 32), ( 2 + 64), ( 2 + 96)	// K3

#define CBSTSW						4, ( 4 + 32), ( 4 + 64), ( 4 + 96)	// K5			CAP
#define VinFPVI 	  			5, ( 5 + 32), ( 5 + 64), ( 5 + 96)	// K6
#define SwFPVI				    6, ( 6 + 32), ( 6 + 64), ( 6 + 96)	// K7
#define ENFOVI						7, ( 7 + 32), ( 7 + 64), ( 7 + 96)	// K8
#define BstFOVI				    9, ( 9 + 32), ( 9 + 64), ( 9 + 96)	// K10
#define FbFOVI	         10, (10 + 32), (10 + 64), (10 + 96)	// K11

#define GNDs				     11, (11 + 32), (11 + 64), (11 + 96)	// K12
#define VBSTSW			     12, (12 + 32), (12 + 64), (12 + 96)	// K13		V/I SERIES
#define QTMU_SWAP			   13, (13 + 32), (13 + 64), (13 + 96)	// K14
#define TSSQTMU				   15, (15 + 32), (15 + 64), (15 + 96)	// K16
// ****************************************************************************** S1, IDC1
#define CapVINsS1  				0 
#define CapSWS1  					1
#define ENQTMUaS1					2
#define VinFPVIS1				  5
#define SwFPVIS1					6
#define ENFOVIS1					7
#define ENOCS1  				  7
#define BstFOVIS1 				9
#define FbFOVIS1  			 10
#define GNDsS1		       11
#define QTMU_SWAPS1			 13
// ****************************************************************************** S2, IDC2
#define CapVINsS2  				(0  + 32)
#define CapSWS2  					(1  + 32)
#define ENQTMUaS2					(2  + 32)
#define VinFPVIS2				  (5  + 32)
#define SwFPVIS2					(6  + 32)
#define ENFOVIS2					(7  + 32)
#define ENOCS2  				  (7  + 32)
#define BstFOVIS2 				(9  + 32)
#define FbFOVIS2  			  (10 + 32)
#define GNDsS2		        (11 + 32)
#define QTMU_SWAPS2			  (13 + 32)
// ****************************************************************************** S3, IDC3
#define CapVINsS3  				(0  + 64)
#define CapSWS3  					(1  + 64)
#define ENQTMUaS3					(2  + 64)
#define VinFPVIS3				  (5  + 64)
#define SwFPVIS3					(6  + 64)
#define ENFOVIS3					(7  + 64)
#define ENOCS3  				  (7  + 64)
#define BstFOVIS3 				(9  + 64)
#define FbFOVIS3  			  (10 + 64)
#define GNDsS3		        (11 + 64)
#define QTMU_SWAPS3			  (13 + 64)
// ****************************************************************************** S4, IDC4
#define CapVINsS4  				(0  + 96)
#define CapSWS4  					(1  + 96)
#define ENQTMUaS4					(2  + 96)
#define VinFPVIS4				  (5  + 96)
#define SwFPVIS4					(6  + 96)
#define ENFOVIS4					(7  + 96)
#define ENOCS4  				  (7  + 96)
#define BstFOVIS4 				(9  + 96)
#define FbFOVIS4  			  (10 + 96)
#define GNDsS4		        (11 + 96)
#define QTMU_SWAPS4			  (13 + 96)
