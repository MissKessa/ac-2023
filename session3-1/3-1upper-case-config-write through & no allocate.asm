.mem
	type = cache-split
	address_bits = 64
	line_size = 16
	nlines = 32
	associativity = 1
	write_policy = write-through
	allocation_policy = no-write-allocate
	miss_time = 16
	mma_time = 8

.microarch
	;Default microarchitecture

		.data
string: byte 'A_simple_string', 0

		.code
main:
		xor   r10, r10, r10    ; index of next character

loop: 
		lb    r20, string(r10) ; next character
		beqz  r20, final       ; end if 0 character
		andi  r20, r20, DFh    ; mask to convert to upper-case
		sb    r20, string(r10) ; store the converted character
		daddi r10, r10, 1      ; next index
		j     loop
final:
		nop

