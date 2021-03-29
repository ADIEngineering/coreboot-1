/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef SOC_INTEL_COMMON_BLOCK_XEON_SA_DEF_H
#define SOC_INTEL_COMMON_BLOCK_XEON_SA_DEF_H

#define PAM0	0x80
#define PAM1	0x81
#define PAM2	0x82
#define PAM3	0x83
#define PAM4	0x84
#define PAM5	0x85
#define PAM6	0x86

/*
 * System Memory Map Registers
 * - top_of_ram -> TSEG - DPR: uncacheable
 * - TESG - DPR -> BGSM: cacheable with standard MTRRs and reserved
 * - BGSM -> TOLUD: not cacheable with standard MTRRs and reserved
 * - 4GiB -> TOUUD: cacheable
 */
enum {
	SA_TOHM_REG,
	SA_TOLM_REG,
	SA_TSEG_REG,
	/* Must be last. */
	MAX_MAP_ENTRIES
};

/*
 * Set Fixed MMIO range
 *   REG = Either PCI configuration space registers.
 *   IS_64_BIT = If registers/offset is 64 bit.
 *   DESCRIPTION = Name of the register/offset.
 */
struct sa_mem_map_descriptor {
	unsigned int reg;
	bool is_64_bit;
	const char *description;
};

#endif	/* SOC_INTEL_COMMON_BLOCK_XEON_SA_DEF_H */
