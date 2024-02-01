.microarch
	exb_no_pipelined_cycles = 3
	exb_instructions = [daddi, dsub]

.data
message: byte 'Uryyb Jbeyq!!!', 0

.code
main:
		xor r10, r10, r10	    ; Initialize index
		daddi r11, r0, 65		; 'A'
		daddi r12, r0, 78		; 'N'
		daddi r13, r0, 91		; '['		
		daddi r14, r0, 97		; 'a'
		daddi r15, r0, 110		; 'n'		
		daddi r16, r0, 122		; 'z'
		daddi r17, r0, 123		; '{'		
loop:
        lb r20, message(r10)  	; Load character
        beqz r20, exit   	    ; End if 0 character
noalpha:
		slt r21, r20, r11		; r20 < 'A'?
		bnez r21, next		
		slt r21, r16, r20		; 'z' < r20?
		bnez r21, next
		slt r21, r20, r13		; r20 =< 'Z'?
		bnez r21, alpha
		slt r21, r20, r14		; r20 < 'a'?
		bnez r21, next
alpha:	
        slt r21, r20, r12		; r20 =< 'M'?
		bnez r21, lower
		slt r21, r20, r13		; r20 =< 'Z'?
		bnez r21, over
		slt r21, r20, r15		; r20 =< 'm'?
		bnez r21, lower
		slt r21, r20, r17		; r20 =< 'z'?
		bnez r21, over
lower:                          ; ('A' =< r20 =< 'M') || ('a' =< r20 =< 'M'):
        daddi r20, r20, 13      
		j next
over:                           ; ('M' < r20 =< 'Z') || ('m' < r20 =< 'z'):
        daddi r20, r20, -13
		j next
next:
        sb r20, message(r10)    ; Store character         
        daddi r10, r10, 1       ; Move to next character
        j loop
exit:
        nop                     ; Exit

