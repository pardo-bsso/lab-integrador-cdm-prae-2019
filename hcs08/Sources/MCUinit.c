/*
** ###################################################################
**     This code is generated by the Device Initialization Tool.
**     It is overwritten during code generation.
**     USER MODIFICATION ARE PRESERVED ONLY INSIDE INTERRUPT SERVICE ROUTINES
**     OR EXPLICITLY MARKED SECTIONS
**
**     Project   : lab_integrador
**     Processor : MC9S08JM32CLHE
**     Version   : Component 01.003, Driver 01.06, CPU db: 3.00.046
**     Datasheet : MC9S08JM60 Rev. 1 11/2007
**     Date/Time : 12/3/2019, 3:17 PM
**     Abstract  :
**         This module contains device initialization code 
**         for selected on-chip peripherals.
**     Contents  :
**         Function "MCU_init" initializes selected peripherals
**
**     Copyright : 1997 - 2009 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################
*/

/* MODULE MCUinit */

#include <MC9S08JM60.h>                /* I/O map for MC9S08JM32CLHE */
#include "MCUinit.h"

/* User declarations and definitions */
/*   Code, declarations and definitions here will be preserved during code generation */
#include "timeutils.h"
/* End of user declarations and definitions */


/*
** ===================================================================
**     Method      :  MCU_init (component MC9S08JM60_64)
**
**     Description :
**         Device initialization code for selected peripherals.
** ===================================================================
*/
void MCU_init(void)
{
  /* ### MC9S08JM60_64 "Cpu" init code ... */
  /*  PE initialization code after reset */

  /* Common initialization of the write once registers */
  /* SOPT1: COPT=0,STOPE=0 */
  SOPT1 = 0x13;                                      
  /* SOPT2: COPCLKS=0,COPW=0,SPI1FE=1,SPI2FE=1,ACIC=0 */
  SOPT2 = 0x06;                                      
  /* SPMSC1: LVWF=0,LVWACK=0,LVWIE=0,LVDRE=1,LVDSE=1,LVDE=1,BGBE=0 */
  SPMSC1 = 0x1C;                                      
  /* SPMSC2: LVDV=0,LVWV=0,PPDF=0,PPDACK=0,PPDC=0 */
  SPMSC2 = 0x00;                                      
  /*  System clock initialization */
  if (*(unsigned char*far)0xFFAF != 0xFF) { /* Test if the device trim value is stored on the specified address */
    MCGTRM = *(unsigned char*far)0xFFAF; /* Initialize MCGTRM register from a non volatile memory */
    MCGSC = *(unsigned char*far)0xFFAE; /* Initialize MCGSC register from a non volatile memory */
  }
  /* MCGC2: BDIV=1,RANGE=0,HGO=0,LP=0,EREFS=0,ERCLKEN=0,EREFSTEN=0 */
  MCGC2 = 0x40;                        /* Set MCGC2 register */
  /* MCGC1: CLKS=0,RDIV=0,IREFS=1,IRCLKEN=1,IREFSTEN=0 */
  MCGC1 = 0x06;                        /* Set MCGC1 register */
  /* MCGC3: LOLIE=0,PLLS=0,CME=0,VDIV=1 */
  MCGC3 = 0x01;                        /* Set MCGC3 register */
  while(!MCGSC_LOCK) {                 /* Wait until FLL is locked */
   SRS = 0x55;                         /* Reset watchdog counter write 55, AA */
   SRS = 0xAA;
  }
  
  /* Common initialization of the CPU registers */
  /* PTADS: PTADS5=1,PTADS4=1,PTADS3=1,PTADS2=1,PTADS1=1,PTADS0=1 */
  PTADS = 0x3F;                                      
  /* ### Init_GPIO init code */
  /* PTADD: PTADD5=1,PTADD4=1,PTADD3=1,PTADD2=1,PTADD1=1,PTADD0=1 */
  PTADD |= (unsigned char)0x3F;                      
  /* ### Init_RTC init code */
  /* RTCSC: RTIF=0,RTCLKS=2,RTIE=1,RTCPS=8 */
  RTCSC = 0x58;                        /* Configure RTC */
  /* ### Init_GPIO init code */
  /* PTEDD: PTEDD7=1,PTEDD6=1,PTEDD5=1,PTEDD4=1,PTEDD3=1,PTEDD2=1,PTEDD1=1,PTEDD0=1 */
  PTEDD = 0xFF;                                      
  /* ### Init_GPIO init code */
  /* PTGDD: PTGDD5=1,PTGDD4=1,PTGDD3=1,PTGDD2=1,PTGDD1=1,PTGDD0=1 */
  PTGDD |= (unsigned char)0x3F;                      
  /* ### Init_COP init code */
  SRS = 0x55;                          /* Clear WatchDog counter - first part */
  SRS = 0xAA;                          /* Clear WatchDog counter - second part */
  /* ### Init_GPIO init code */
  /* PTDDD: PTDDD7=1,PTDDD6=1,PTDDD5=1,PTDDD4=1,PTDDD3=1,PTDDD2=1,PTDDD1=1,PTDDD0=1 */
  PTDDD = 0xFF;                                      
  /* ### */
  asm CLI;                             /* Enable interrupts */
} /*MCU_init*/


/*
** ===================================================================
**     Interrupt handler : isrVrtc
**
**     Description :
**         User interrupt service routine. 
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
__interrupt void isrVrtc(void)
{
  /* Write your interrupt code here ... */
  tick();

}
/* end of isrVrtc */



/* Initialization of the CPU registers in FLASH */

/* NVPROT: FPS7=1,FPS6=1,FPS5=1,FPS4=1,FPS3=1,FPS2=1,FPS1=1,FPDIS=1 */
const unsigned char NVPROT_INIT @0x0000FFBD = 0xFF;

/* NVOPT: KEYEN=0,FNORED=1,SEC01=1,SEC00=0 */
const unsigned char NVOPT_INIT @0x0000FFBF = 0x7E;



extern near void _Startup(void);

/* Interrupt vector table */
#ifndef UNASSIGNED_ISR
  #define UNASSIGNED_ISR ((void(*near const)(void)) 0xFFFF) /* unassigned interrupt service routine */
#endif

void (* near const _vect[])(void) @0xFFC4 = { /* Interrupt vector table */
         isrVrtc,                      /* Int.no. 29 Vrtc (at FFC4)                  Used */
         UNASSIGNED_ISR,               /* Int.no. 28 Viic (at FFC6)                  Unassigned */
         UNASSIGNED_ISR,               /* Int.no. 27 Vacmp (at FFC8)                 Unassigned */
         UNASSIGNED_ISR,               /* Int.no. 26 Vadc (at FFCA)                  Unassigned */
         UNASSIGNED_ISR,               /* Int.no. 25 Vkeyboard (at FFCC)             Unassigned */
         UNASSIGNED_ISR,               /* Int.no. 24 Vsci2tx (at FFCE)               Unassigned */
         UNASSIGNED_ISR,               /* Int.no. 23 Vsci2rx (at FFD0)               Unassigned */
         UNASSIGNED_ISR,               /* Int.no. 22 Vsci2err (at FFD2)              Unassigned */
         UNASSIGNED_ISR,               /* Int.no. 21 Vsci1tx (at FFD4)               Unassigned */
         UNASSIGNED_ISR,               /* Int.no. 20 Vsci1rx (at FFD6)               Unassigned */
         UNASSIGNED_ISR,               /* Int.no. 19 Vsci1err (at FFD8)              Unassigned */
         UNASSIGNED_ISR,               /* Int.no. 18 Vtpm2ovf (at FFDA)              Unassigned */
         UNASSIGNED_ISR,               /* Int.no. 17 Vtpm2ch1 (at FFDC)              Unassigned */
         UNASSIGNED_ISR,               /* Int.no. 16 Vtpm2ch0 (at FFDE)              Unassigned */
         UNASSIGNED_ISR,               /* Int.no. 15 Vtpm1ovf (at FFE0)              Unassigned */
         UNASSIGNED_ISR,               /* Int.no. 14 Vtpm1ch5 (at FFE2)              Unassigned */
         UNASSIGNED_ISR,               /* Int.no. 13 Vtpm1ch4 (at FFE4)              Unassigned */
         UNASSIGNED_ISR,               /* Int.no. 12 Vtpm1ch3 (at FFE6)              Unassigned */
         UNASSIGNED_ISR,               /* Int.no. 11 Vtpm1ch2 (at FFE8)              Unassigned */
         UNASSIGNED_ISR,               /* Int.no. 10 Vtpm1ch1 (at FFEA)              Unassigned */
         UNASSIGNED_ISR,               /* Int.no.  9 Vtpm1ch0 (at FFEC)              Unassigned */
         UNASSIGNED_ISR,               /* Int.no.  8 VReserved (at FFEE)             Unassigned */
         UNASSIGNED_ISR,               /* Int.no.  7 Vusb (at FFF0)                  Unassigned */
         UNASSIGNED_ISR,               /* Int.no.  6 Vspi2 (at FFF2)                 Unassigned */
         UNASSIGNED_ISR,               /* Int.no.  5 Vspi1 (at FFF4)                 Unassigned */
         UNASSIGNED_ISR,               /* Int.no.  4 Vlol (at FFF6)                  Unassigned */
         UNASSIGNED_ISR,               /* Int.no.  3 Vlvd (at FFF8)                  Unassigned */
         UNASSIGNED_ISR,               /* Int.no.  2 Virq (at FFFA)                  Unassigned */
         UNASSIGNED_ISR,               /* Int.no.  1 Vswi (at FFFC)                  Unassigned */
         _Startup                      /* Int.no.  0 Vreset (at FFFE)                Reset vector */
};




/* END MCUinit */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.07 [04.34]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/