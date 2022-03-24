/* This is a simplefied ELF reader.
 * You can contact me if you find any bugs.
 *
 * Luming Wang<wlm199558@126.com>
 */

#include "kerelf.h"
#include <stdio.h>
/* Overview:
 *   Check whether it is a ELF file.
 *
 * Pre-Condition:
 *   binary must longer than 4 byte.
 *
 * Post-Condition:
 *   Return 0 if `binary` isn't an elf. Otherwise
 * return 1.
 */
int is_elf_format(u_char *binary)
{
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;
        if (ehdr->e_ident[EI_MAG0] == ELFMAG0 &&
                ehdr->e_ident[EI_MAG1] == ELFMAG1 &&
                ehdr->e_ident[EI_MAG2] == ELFMAG2 &&
                ehdr->e_ident[EI_MAG3] == ELFMAG3) {
                return 1;
        }

        return 0;
}

Elf32_Addr roundup(Elf32_Addr addr){
        Elf32_Word pg = 0x800;
        if(addr % pg == 0) return addr;
        else return (addr / pg) * 0x800;
}

Elf32_Addr rounddown(Elf32_Addr addr){
	Elf32_Word pg = 0x800;
	if(addr % pg == 0) return addr;
	else return (addr / pg + 1) * 0x800; 
}

/* Overview:
 *   read an elf format binary file. get ELF's information
 *
 * Pre-Condition:
 *   `binary` can't be NULL and `size` is the size of binary.
 *
 * Post-Condition:
 *   Return 0 if success. Otherwise return < 0.
 *   If success, output address of every section in ELF.
 */

/*
    Exercise 1.2. Please complete func "readelf". 
*/
int readelf(u_char *binary, int size)
{
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;

        int Nr;

        Elf32_Shdr *shdr = NULL;
	Elf32_Phdr *phdr = NULL;

        u_char *ptr_sh_table = NULL;
        Elf32_Half sh_entry_count;
        Elf32_Half sh_entry_size;


        // check whether `binary` is a ELF file.
        if (size < 4 || !is_elf_format(binary)) {
                printf("not a standard elf format\n");
                return 0;
        }
	
	phdr = (Elf32_Phdr *) (binary + ehdr->e_phoff);
	for(int i=0; i<ehdr->e_phnum-1; i++){
		for(int j=i+1; j<ehdr->e_phnum; j++){
			Elf32_Addr l_1,r_1,l_2,r_2;
			if((phdr+i)->p_vaddr < (phdr+j)->p_vaddr){
				l_1 = (phdr+i)->p_vaddr;
				r_1 = l_1 + (phdr+i)->p_memsz;
				l_2 = (phdr+j)->p_vaddr;
                        	r_2 = l_2 + (phdr+j)->p_memsz;
			}else{
				l_2 = (phdr+i)->p_vaddr;
                                r_2 = l_2 + (phdr+i)->p_memsz;
                                l_1 = (phdr+j)->p_vaddr;
                                r_1 = l_1 + (phdr+j)->p_memsz;
			}
			if(l_2<r_1){
				//printf("Conflict at page va : 0x%x\n", roundup(l_2));
				return 0;
			}
			if(rounddown(r_1) == rounddown(l_2)){
				printf("Overlay at page va : 0x%x\n", roundup(l_2));
				return 0;
			}
		}
	}	
	//printf("0x%x\n", rounddown(0x804));
	for(int i=0; i<ehdr->e_phnum; i++){
		printf("%d:0x%x,0x%x\n", i, (phdr+i)->p_filesz, (phdr+i)->p_memsz);
	}

        // get section table addr, section header number and section header size.
        //shdr = (Elf32_Shdr *) (binary + ehdr->e_shoff);
        // for each section header, output section number and section addr.
	//printf("Read : %d:0x%x,0x%x\n", 2, (shdr+2)->sh_offset, (shdr+2)->sh_addr);
	//printf("Read : %d:0x%x,0x%x\n", 3, (shdr+3)->sh_offset, (shdr+3)->sh_addr);
	//for(int i=0; i<ehdr->e_shnum;i++){
	//       printf("%d:0x%x\n",i,(shdr+i)->sh_addr);
	//}	       
        // hint: section number starts at 0.


        return 0;
}

