#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>


LEAF(_my_putchar)
	li	t1,0xB0000000
	sb	a0,0(t1)
        
	jr	ra
	nop	
END(_my_putchar)

LEAF(_my_exit)
        li      t1,0xB0000000
	sb	t0,0x10(t1)

        jr      ra
        nop
END(_my_exit)

LEAF(_my_getchar)
        li      t1,0xB0000000
lp1:
        lb      v0,0(t1)
	beq	v0,0,lp1
		
	sb	v0,0(t1)
	#bne	v0,13,end
	
	#li	t0,10
	#sb	t0,0(t1)
	
end:
        jr      ra
        nop
END(_my_getchar)
