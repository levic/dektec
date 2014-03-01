//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Helpers.h *#*#*#*#*#*#*#*#*# (C) 2000-2008 DekTec

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This program is free software; You can freely use/modify this source code in any way
// you desire, including for commercial applications.
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//	MG	21-10-2002	Created	

#ifndef __HELPERS_H
#define __HELPERS_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <linux/version.h>

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ General helpers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// PCI write/read helpers
#define WRITE_BYTE(val, base, offset)	writeb(val, (char *)base + offset);
#define WRITE_WORD(val, base, offset)	writew(val, (char *)base + offset);
#define WRITE_LONG(val, base, offset)	writel(val, (char *)base + offset);
#define READ_BYTE(val, base, offset)	val = readb((char *)base + offset);
#define READ_WORD(val, base, offset)	val = readw((char *)base + offset);
#define READ_LONG(val, base, offset)	val = readl((char *)base + offset);


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Kernel version dependancies +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,1)
enum dma_data_direction {
	DMA_BIDIRECTIONAL = 0,
	DMA_TO_DEVICE = 1,
	DMA_FROM_DEVICE = 2,
	DMA_NONE = 3,
};
#endif
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,5)
	#define pci_dma_sync_sg_for_device pci_dma_sync_sg
	#define pci_dma_sync_sg_for_cpu pci_dma_sync_sg
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,71)
	#define pgprot_noncached(prot)	((boot_cpu_data.x86 > 3)					  \
				 ? (__pgprot(pgprot_val(prot) | _PAGE_PCD | _PAGE_PWT)) : (prot))
#endif

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Scatter/Gather helpers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Kernel 2.6.24 provides an updated struct scatterlist API. We use inline functions as
// wrapper for kernel dependant scatter-gather code

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSgInitTable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static inline void Dta1xxSgInitTable(
	struct scatterlist* sg,
	Int NumEntries)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,24)
	// Kernel 2.6.24 or later
	sg_init_table(sg, NumEntries);
#else
	// Pre kernel 2.6.24 or later
	return; // NOTHING TODO
#endif
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSgSetPage -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static inline void Dta1xxSgSetPage(
	struct scatterlist* sg,
	struct page* page,
	Int size,
	Int offset)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,24)
	// Kernel 2.6.24 or later
	sg_set_page(sg, page, size, offset);
#else
	// Pre kernel 2.6.24 or later
	sg->page = page;
	sg->length = size;
	sg->offset = offset;
#endif
}

#endif //__HELPERS_H
