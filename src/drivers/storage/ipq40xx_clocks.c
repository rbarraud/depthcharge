/*
 * Copyright (C) 2014 The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <libpayload.h>
#include "drivers/storage/ipq40xx_mmc.h"
#include "drivers/storage/ipq40xx_clocks.h"

void clock_config_mmc(MmcCtrlr *ctrlr, int mode)
{
	printf("%s : %d\n",__func__,mode);
	/* Select SDCC clock source as DDR_PLL_SDCC1_CLK  192MHz */
	writel(0x100, GCC_SDCC1_APPS_RCGR);
	/* Update APPS_CMD_RCGR to reflect source selection */
	writel(0x1, GCC_SDCC1_APPS_CMD_RCGR);
	udelay(10);

	if (mode == MMC_IDENTIFY_MODE) {
		/* Set root clock generator to bypass mode */
		writel(0x0, GCC_SDCC1_APPS_CBCR);
		udelay(10);
		/* Choose divider for 400KHz */
		writel(0x1e4 , GCC_SDCC1_MISC);
		/* Enable root clock generator */
		writel(0x1, GCC_SDCC1_APPS_CBCR);
		udelay(10);
	}
	if (mode == MMC_DATA_TRANSFER_MODE) {
		/* Set root clock generator to bypass mode */
		writel(0x0, GCC_SDCC1_APPS_CBCR);
		udelay(10);
		/* Choose divider for 48MHz */
		writel(0x3, GCC_SDCC1_MISC);
		/* Enable root clock generator */
		writel(0x1, GCC_SDCC1_APPS_CBCR);
		udelay(10);
	}
	//  mmc_boot_mci_clk_enable(ctrlr);
}

void clock_disable_mmc(void)
{
	/* Clear divider */
	writel(0x0, GCC_SDCC1_MISC);
}
