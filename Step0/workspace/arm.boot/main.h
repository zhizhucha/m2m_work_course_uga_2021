/*
 * test.h
 *
 *  Created on: Jan 12, 2021
 *      Author: ogruber
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stddef.h>

#define VIC_IRQ_STATUS

extern unsigned char CC;

void irq_handler();

#endif /* MAIN_H_ */
