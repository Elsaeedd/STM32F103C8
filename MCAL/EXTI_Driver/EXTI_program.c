/*
 * EXTI_program.c
 *
 *  Created on: Feb 8, 2021
 *      Author: Boon
 */

#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "EXTI_interface.h"
#include "EXTI_private.h"
#include "EXTI_config.h"

#ifndef		NULL
#define 	NULL		(void *)0
#endif

static void (*EXTI0_pfCallBack)(void) = NULL;
static void (*EXTI1_pfCallBack)(void) = NULL;
static void (*EXTI5_pfCallBack)(void) = NULL;



void EXTI_voidInit(void)
{
	/*		Select all Edge Modes for All Lines		*/
#if			EXTI_LINE0 == EXTI_FALLING
			SET_BIT(EXTI -> FTSR , 0);
#elif		EXTI_LINE0 == EXTI_RISING
			SET_BIT(EXTI -> RTSR , 0);
#elif		EXTI_LINE0 == EXTI_IOC
			SET_BIT(EXTI -> RTSR , 0);
			SET_BIT(EXTI -> FTSR , 0);
#else
#error		"Wrong Choice of edge mode for line0 !"
#endif

#if			EXTI_LINE1 == EXTI_FALLING
			SET_BIT(EXTI -> FTSR , 1);
#elif		EXTI_LINE1 == EXTI_RISING
			SET_BIT(EXTI -> RTSR , 1);
#elif		EXTI_LINE1 == EXTI_IOC
			SET_BIT(EXTI -> RTSR , 1);
			SET_BIT(EXTI -> FTSR , 1);
#else
#error		"Wrong Choice of edge mode for line1 !"
#endif

#if			EXTI_LINE2 == EXTI_FALLING
			SET_BIT(EXTI -> FTSR , 2);
#elif		EXTI_LINE2 == EXTI_RISING
			SET_BIT(EXTI -> RTSR , 2);
#elif		EXTI_LINE2 == EXTI_IOC
			SET_BIT(EXTI -> RTSR , 2);
			SET_BIT(EXTI -> FTSR , 2);
#else
#error		"Wrong Choice of edge mode for line2 !"
#endif
	/*		Disable all EXTI lines		 			*/
			EXTI -> IMR = 0;
	/*		Clear All Pending BITs					*/
			EXTI -> PR 	= 0xFFFFFFFF;
}

void EXTI_voidEnableLine(u8 Copy_u8LineNum)
{
	if(Copy_u8LineNum < 16)
	{
		SET_BIT(EXTI -> IMR , Copy_u8LineNum);
	}
}
void EXTI_voidSetSignalLatch(u8 Copy_u8LineNum, u8 Copy_u8EdgeMode)
{
	if(Copy_u8LineNum < 16)
	{
		CLR_BIT(EXTI -> FTSR , Copy_u8LineNum );
		CLR_BIT(EXTI -> RTSR , Copy_u8LineNum );
		switch(Copy_u8EdgeMode)
		{
			case EXTI_RISING 	: 	SET_BIT(EXTI -> RTSR , Copy_u8LineNum ); 	break;
			case EXTI_FALLING 	:	SET_BIT(EXTI -> FTSR , Copy_u8LineNum ); 	break;
			case EXTI_IOC 		: 	SET_BIT(EXTI -> RTSR , Copy_u8LineNum );
									SET_BIT(EXTI -> FTSR , Copy_u8LineNum );	break;
		}
	}
}
void EXTI_voidDisableLine(u8 Copy_u8LineNum )
{
	if(Copy_u8LineNum < 16)
	{
		CLR_BIT(EXTI -> IMR , Copy_u8LineNum);
	}
}
void EXTI0_voidSetCallBack(void (*pf) (void)) // address ahmed Function
{
	if(pf != NULL)
	{
		/*EXTI0_pfCallBack = pf = address ahmed Function*/
		EXTI0_pfCallBack = pf ;
	}
}
void EXTI1_voidSetCallBack(void (*pf) (void)) // address ahmed Function
{
	if(pf != NULL)
	{
		EXTI1_pfCallBack = pf ;
	}
}


void EXTI9_5_voidSetCallBack(u8 EXTI_u8Number,void (*pf) (void) ){

	if(pf != NULL)
	{

		switch(EXTI_u8Number){

		case 5: 	EXTI5_pfCallBack = pf ;	break;


		default : /* return error */;

		}
	}
}


void EXTI_voidSetSoftwareTrigger(u8 Copy_u8LineNum )
{
	/*		Task								*/
	if(Copy_u8LineNum < 16)
	{
		SET_BIT(EXTI -> SWIER, Copy_u8LineNum);
	}
}
/* 			External Interrupt									*/
void EXTI0_IRQHandler(void)
{
	EXTI0_pfCallBack(); //ahmed();
	/*		Pending Bit Clear = Execute interrupt once			 */
	SET_BIT(EXTI -> PR , 0);
}

void EXTI1_IRQHandler(void)
{
	EXTI1_pfCallBack(); //Elsaeed();
	/*		Pending Bit Clear = Execute interrupt once			 */
	SET_BIT(EXTI -> PR , 1);
}

void EXTI9_5_IRQHandler(void){

	if(GET_BIT(EXTI -> PR ,5) == 1){
		EXTI5_pfCallBack();
		SET_BIT(EXTI -> PR , 5);
	}
/*	if(GET_BIT(EXTI -> PR ,6) == 1){
		EXTI6_pfCallBack();
		SET_BIT(EXTI -> PR , 6);
	}
	if(GET_BIT(EXTI -> PR ,7) == 1){
		EXTI7_pfCallBack();
		SET_BIT(EXTI -> PR , 7);
	}
	if(GET_BIT(EXTI -> PR ,8) == 1){
		EXTI8_pfCallBack();
		SET_BIT(EXTI -> PR , 8);
	}
	if(GET_BIT(EXTI -> PR ,9) == 1){
		EXTI9_pfCallBack();
		SET_BIT(EXTI -> PR , 9);
	}*/

}




/* 		internal interrupt	Core Peripheral Interrupt			*/
