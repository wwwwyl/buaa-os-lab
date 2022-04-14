#ifndef _PMAP_H_
#define _PMAP_H_

#include "types.h"
#include "queue.h"
#include "mmu.h"
#include "printf.h"


LIST_HEAD(Page_list, Page);
typedef LIST_ENTRY(Page) Page_LIST_entry_t;

struct Page {
	Page_LIST_entry_t pp_link;	/* free list link */

	// Ref is the count of pointers (usually in page table entries)
	// to this page.  This only holds for pages allocated using
	// page_alloc.  Pages allocated at boot time using pmap.c's "alloc"
	// do not have valid reference count fields.

	u_short pp_ref;
};

extern struct Page *pages;
// Buddy
LIST_HEAD(Buddy_list, Buddy);
typedef LIST_ENTRY(Buddy) Buddy_LIST_entry_t;

struct Buddy {
        Buddy_LIST_entry_t bb_link;      /* free list link */

        // Ref is the count of pointers (usually in page table entries)
        // to this page.  This only holds for pages allocated using
        // page_alloc.  Pages allocated at boot time using pmap.c's "alloc"
        // do not have valid reference count fields.
	u_long bb_start;
	u_char bb_size;
	u_long bb_parent;
        u_short bb_ref;
};

extern struct Buddy *buddys;

static inline u_long
page2ppn(struct Page *pp)
{
	return pp - pages;
}

/* Get the physical address of Page 'pp'.
 */
static inline u_long
page2pa(struct Page *pp)
{
	return page2ppn(pp) << PGSHIFT;
}

/* Get the Page struct whose physical address is 'pa'.
 */
static inline struct Page *
pa2page(u_long pa)
{
	if (PPN(pa) >= npage) {
		panic("pa2page called with invalid pa: %x", pa);
	}

	return &pages[PPN(pa)];
}

/* Get the kernel virtual address of Page 'pp'.
 */
static inline u_long
page2kva(struct Page *pp)
{
	return KADDR(page2pa(pp));
}

/* Transform the virtual address 'va' to physical address.
 */
static inline u_long
va2pa(Pde *pgdir, u_long va)
{
	Pte *p;

	pgdir = &pgdir[PDX(va)];

	if (!(*pgdir & PTE_V)) {
		return ~0;
	}

	p = (Pte *)KADDR(PTE_ADDR(*pgdir));

	if (!(p[PTX(va)]&PTE_V)) {
		return ~0;
	}

	return PTE_ADDR(p[PTX(va)]);
}

/********** functions for memory management(see implementation in mm/pmap.c). ***********/

void mips_detect_memory();

void mips_vm_init();

void mips_init();
void page_init(void);
void page_check();
void physical_memory_manage_check();
int page_alloc(struct Page **pp);
void page_free(struct Page *pp);
void page_decref(struct Page *pp);
int pgdir_walk(Pde *pgdir, u_long va, int create, Pte **ppte);
int page_insert(Pde *pgdir, struct Page *pp, u_long va, u_int perm);
struct Page *page_lookup(Pde *pgdir, u_long va, Pte **ppte);
void page_remove(Pde *pgdir, u_long va) ;
void tlb_invalidate(Pde *pgdir, u_long va);

void boot_map_segment(Pde *pgdir, u_long va, u_long size, u_long pa, int perm);

extern struct Page *pages;

void buddy_init(void);
int buddy_alloc(u_int size, u_int *pa, u_char *pi);
void buddy_free(u_int pa);


#endif /* _PMAP_H_ */
