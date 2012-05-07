.arm
.section .bss
.align 3 /* doubleword-aligned */
.global	__stack_start_addr

__stack_end_addr:
        .space  16384
__stack_start_addr:
