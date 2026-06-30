/////////////////////////////////////////////////////////////////////////////////////
// RCU Reset and Clock Unit Subroutine package                         0.91 AC
/////////////////////////////////////////////////////////////////////////////////////
.equ RCU,    0x40021000
.equ APB1EN, 0x1C
.equ T5EN,   0x10
.equ APB2EN, 0x18
.equ PAEN,   0x4
.equ PBEN,   0x8
// RCU1EN Enabel APB1 bus devices //////////////////////////////////// 1.0 / AC ///
rcu1en: li t0,RCU           // t0 points to RCU...
        lw t1,APB1EN(t0)    // ...retrieve current configuration...
        or t1,t1,a0         // ...and update the configuration...
        sw t1,APB1EN(t0)    // ...then save back the updated config...
        ret                 // and go home!
// RCU2EN Enabel APB2 bus devices //////////////////////////////////// 1.0 / AC ///
rcu2en: li t0,RCU           // t0 points to RCU...
        lw t1,APB2EN(t0)    // ...retrieve current configuration...
        or t1,t1,a0         // ...and update the configuration...
        sw t1,APB2EN(t0)    // ...then save back the updated config...
        ret                 // and go home!

/////////////////////////////////////////////////////////////////////////////////////
// Timer 5 Subroutine package                                          0.91 AC
/////////////////////////////////////////////////////////////////////////////////////
.equ TIMER5, 0x40001000
.equ T5CTR0, 0x00
.equ UPIF,   0x10
.equ PSC,    0x28
.equ CAR,    0x2C
.equ CNT,    0x24
// T5OMSI Initialize T5 to overflow each millisecond /////////////////// 1.0 / AC ///
.global t5omsi 
t5omsi: addi sp, sp, -4
        sw ra, 0(sp)

        li a0, T5EN         // Prepare to turn on Timer 5...
        call rcu1en         // ..do the set-up! 

        li t0, TIMER5       // Prepare to configur Timer 5...
        li t1, 0x06B        // ...set prescaler to
        sw t1,PSC(t0)       //    divide by 108, thus count 0..107...
        li t1, 0x3E7        // ...and then counter auto reload to
        sw t1, CAR(t0)      //    reload at 999 for a 1ms counter..
        sw zero,UPIF(t0)    // ...then clear the Interrupt Flag...
        li t1,0x01          // ...and start the counting!
        sw t1,T5CTR0(t0)

        lw ra, 0(sp)        // Restore return address...
        addi sp, sp, 4      // ...then the stack pointer...
        ret                 // ...and return to caller!   
.global t5expq
t5expq: li t0, TIMER5       // Prepare to, and then read, Timer 5... 
        lw a0, UPIF(t0)     // ...counter overflow flag...
        beqz a0, 1f         // ...done if not overflow...
        sw zero,UPIF(t0)    // ...else reset flag...
1:      ret                 // ...and return to caller!


//a0 = start/new value
//a1 = old min
//a2 = new min
//a3 = old max
//a4 = new max
.global map
map:
   
   sub t0, a3, a1 //old range = old max - old min
   beqz t0, 1f //if t0 = 0
   
   sub t1, a4, a2 //new range = new max - new min
   
   //NewValue = (((OldValue - OldMin) * NewRange) / OldRange) + NewMin
   sub t2, a0, a1 //OldValue - OldMin
   mul t3, t2, t1 //(OldValue - OldMin) * NewRange
   div t4, t3, t0 //((OldValue - OldMin) * NewRange) / OldRange
   add t5, t4, a2 //(((OldValue - OldMin) * NewRange) / OldRange) + NewMin

   mv a0, t5 //Move new value into a0 to return
   j 2f
1:
   mv a0, a2 //return value = new min
2: ret