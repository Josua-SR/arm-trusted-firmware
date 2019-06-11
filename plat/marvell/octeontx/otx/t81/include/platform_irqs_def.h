/*
 * Copyright (C) 2018 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#ifndef __PLATFORM_IRQS_DEF_H__
#define __PLATFORM_IRQS_DEF_H__

/*******************************************************************************
 * Definitions of IRQs
 ******************************************************************************/

/* Definitions of IRQ SGI (Software Generated Interrupt) IDs (range 0x0 - 0xf) */
#define SGI_IRQ_OFFSET			0x0

/* Definitions of IRQ PPI (Per-Processor Interrupt) IDs (range 0x10 - 0x1f) */
#define PPI_IRQ_OFFSET			0x10

/* Secure physical generic timer IRQ (interrupt 0xd)*/
#define SEC_TIMER_PPI_IRQ_BASE	(0xd + PPI_IRQ_OFFSET)
#define SEC_TIMER_PPI_IRQ		(SEC_TIMER_PPI_IRQ_BASE)


/* Definitions of IRQ SPI (Shared Peripheral Interrupt) IDs (range 0x20-0x3FC) */
#define SPI_IRQ_OFFSET			0x20

/* UAA definitons (range 0x20 - 0x23) */
#define UAA_SPI_IRQ_DEVS		0x4
#define UAA_SPI_IRQ_BASE		(0x0 + SPI_IRQ_OFFSET)
#define UAA_SPI_IRQ(dev)		((dev) + UAA_SPI_IRQ_BASE)

/* Watchdog interrupt (interrupt 0x24)*/
#define GTI_WATCHDOG_SPI_IRQ_BASE		(UAA_SPI_IRQ_BASE + UAA_SPI_IRQ_DEVS)
#define GTI_WATCHDOG_SPI_IRQ_DEVS		0x1
#define GTI_WATCHDOG_SPI_IRQ(dev)		(GTI_WATCHDOG_SPI_IRQ_BASE)

/* PEM definitions (range 0x25 - 0x30) */
/*
 * T81 has 3 PEMs
 */
#define PEM_SPI_IRQ_DEVS		0x3

/*
 * Each of PEM has 4 interrupts
 * (INTA, INTB, INTC, INTD)
 */
#define PEM_SPI_IRQS_PER_DEV			0x4

#define PEM_SPI_IRQ_BASE		(GTI_WATCHDOG_SPI_IRQ_BASE + GTI_WATCHDOG_SPI_IRQ_DEVS)
#define PEM_SPI_IRQ(dev, irq)	((dev) * PEM_SPI_IRQS_PER_DEV + (irq) + PEM_SPI_IRQ_BASE)

/* SMMU definitions (range 0x31 - 0x32)*/
/*
 * Two interrupts per SMMU instance (Global and Context).
 * T81 has 1 SMMU instances
 */
#define SMMU_SPI_IRQ_DEVS		0x1
#define SMMU_SPI_IRQS_PER_DEV	0x2
#define SMMU_SPI_IRQ_BASE		(PEM_SPI_IRQ_BASE + (PEM_SPI_IRQ_DEVS*PEM_SPI_IRQS_PER_DEV))
#define SMMU_SPI_IRQ(dev, irq)	((dev) * SMMU_SPI_IRQS_PER_DEV + (irq) + SMMU_SPI_IRQ_BASE)

#define MCC_SPI_IRQS		0x0
#define MCC_SPI_IRQ_BASE	(SMMU_SPI_IRQ_BASE + \
				 SMMU_SPI_IRQS_PER_DEV * SMMU_SPI_IRQ_DEVS)
#define MCC_SPI_IRQ(irq)	(-1)

#define MDC_SPI_IRQS		0x0
#define MDC_SPI_IRQ_BASE	(MCC_SPI_IRQ_BASE + MCC_SPI_IRQS)
#define MDC_SPI_IRQ()		(-1)

#endif /* __PLATFORM_IRQS_DEF_H__ */
