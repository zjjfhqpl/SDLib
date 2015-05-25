/*
 * scale_arm_neon.s
 * libsepparse


.text
.align 4
.globl _my_arm_test
.globl _my_thumb_test
.globl _bilinear_interpolation_arm_neon_shit

.arm

_bilinear_interpolation_arm_neon_shit:
					 vldr.32	d0,[r1]			@ d0 =  tl		
					 vldr.32	d1,[r1,#4]		@ d1 =  tr		
					 vldr.32	d2,[r1,#8]		@ d2 =  bl		
					 vldr.32	d3,[r1,#12]		@ d3 =  br		
					 vldr.32	d4,[r1,#16]		@ r4 = distx		
					 vldr.32	d5,[r1,#20]		@ r5 = distx		
					 vmul.i32	d6, d4, d5		@ d6 = d4*d5		
					 vshl.i32	d7, d4, #8		@ d7 = d4<<8		
					 vsub.I32  d8, d7,d6  
					 vshl.i32	d9, d5, #8		@ d7=d4<<8		
					 vsub.I32  d10, d9,d6 
					 mov r2, #256		
					 vdup.32     d11, r2 	
					 vmul.i32	d11, d11,d11			
					 vsub.i32  d11, d11,d7			
					 vsub.i32  d11, d11,d9			
					 vadd.i32  d11, d11,d6		@ d11 distixiy	
					 
					 mov r2, #0x000000ff		
					 vdup.32     d12, r2 	
					 vand.i32  d13, d0, d12	
					 vmul.i32	d13, d13, d11		@ distixiy * (tl & 0x000000ff)	
					 vand.i32  d14, d1, d12	
					 vmul.i32	d14, d14, d8		@ distixiy * (tl & 0x000000ff)	
					 vand.i32  d15, d2, d12	
					 vmul.i32	d15, d15, d10		@ distixy * (tl & 0x000000ff)	
					 vand.i32  d16, d3, d12	
					 vmul.i32	d16, d16, d6		@ distixy * (tl & 0x000000ff)	
					 vadd.i32  d17, d13, d14	
					 vadd.i32  d17, d17, d15	
					 vadd.i32  d17, d17, d16	

					 mov r2, #0x0000ff00		
					 vdup.32     d12, r2 	
					 vand.i32  d13, d0, d12	
					 vmul.i32	d13, d13, d11		
					 vand.i32  d14, d1, d12	
					 vmul.i32	d14, d14, d8		
					 vand.i32  d15, d2, d12	
					 vmul.i32	d15, d15, d10		
					 vand.i32  d16, d3, d12	
					 vmul.i32	d16, d16, d6	
					 vadd.i32  d18, d13, d14	
					 vadd.i32  d18, d18, d15	
					 vadd.i32  d18, d18, d16	 
					 
					 mov r2, #0xff000000		
					 vdup.32     d12, r2 	
					 vand.i32  d18, d18, d12	
					 vorr.i32  d17, d17, d18	

					 vshr.U32	d0, d0, #16		@ d17 >>= 16		
					 vshr.U32	d1, d1, #16		@ d17 >>= 16		
					 vshr.U32	d2, d2, #16		@ d17 >>= 16		
					 vshr.U32	d3, d3, #16		@ d17 >>= 16		
					 vshr.U32	d17, d17, #16	@ d17 >>= 16		
					 
					 mov r2, #0x000000ff		
					 vdup.32     d12, r2 	
					 vand.i32  d13, d0, d12	
					 vmul.i32	d13, d13, d11		
					 vand.i32  d14, d1, d12	
					 vmul.i32	d14, d14, d8		
					 vand.i32  d15, d2, d12	
					 vmul.i32	d15, d15, d10		
					 vand.i32  d16, d3, d12
					 vmul.i32	d16, d16, d6		
					 vadd.i32  d18, d13, d14	
					 vadd.i32  d18, d18, d15	
					 vadd.i32  d18, d18, d16	
					 
					 mov r2, #0x00ff0000		
					 vdup.32     d12, r2 	
					 vand.i32  d18, d18, d12	
					 vorr.i32  d17, d17, d18	

					 mov r2, #0x0000ff00		
					 vdup.32     d12, r2 	
					 vand.i32  d13, d0, d12	
					 vmul.i32	d13, d13, d11		
					 vand.i32  d14, d1, d12	
					 vmul.i32	d14, d14, d8		
					 vand.i32  d15, d2, d12	
					 vmul.i32	d15, d15, d10		
					 vand.i32  d16, d3, d12	
					 vmul.i32	d16, d16, d6		
					 vadd.i32  d18, d13, d14	
					 vadd.i32  d18, d18, d15	
					 vadd.i32  d18, d18, d16	
					 
					 mov r2, #0xff000000		
					 vdup.32     d12, r2 
					 vand.i32  d18, d18, d12
					 vorr.i32  d17, d17, d18
					 vstr.32	d17, [r0]
					 bx          lr
	
_my_arm_test:


 /*   vdup.32     q0, r0 */
 vldr.32	d0,[r1]
 vldr.32	d1,[r2]
 /*vdup.u8     d0, r0 */
 /*vdup.u8     d1, r1*/ 

 	vmul.u8  d1, d0, d1

    /*qadd8       r0, r1, r2
    add         r0, r2, lsl #2*/
	vstr.32	d1, [r0]
    bx          lr

.thumb
.thumb_func _my_thumb_test

_my_thumb_test:

    movw       r1, #1001
    rev16      r0, r1
    bx         lr
