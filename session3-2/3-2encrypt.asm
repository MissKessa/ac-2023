;
; Converts a null-terminated ASCII string to upper-case
;
		.data
string: byte 'a_simple_string', 0
mask:   dword 1,0,1,1,1,1,1,1,0,1,1,1,1,1,1 ; array of numbers 

		.code
main:
		xor   r10, r10, r10    ; index of next character
		xor   r11, r11, r11    ; index of next mask value

loop: 
		lb    r20, string(r10) ; next character
		beqz  r20, final       ; end if 0 character
		lb    r21, mask(r11)   ; next mask character
		dadd  r22, r20, r21    ; add mask to crypt
		sb    r22, string(r10) ; store the converted character
		daddi r10, r10, 1      ; next index
		daddi r11, r11, 8      ; next mask value
		j     loop
final:
		nop
		nop
		and r10, r10, r0
		and r11, r11, r0
decrypt:
		lb    r20, string(r10) ; next character
		beqz  r20, end         ; end if 0 character
		lb    r21, mask(r11)   ; next mask character
		dsub  r22, r20, r21    ; decrypt mask
		sb    r22, string(r10) ; store the converted character
		daddi r10, r10, 1      ; next index
		daddi r11, r11, 8      ; next mask value
		j     decrypt
end:
		nop