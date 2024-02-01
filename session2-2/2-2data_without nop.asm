	.data

var: dword 25
res: dword 0

	.code

main:
	daddi r10, r0, 5   ; r10 <- 5
	ld    r20, var(r0) ; load var in r20
	dadd  r1, r20, r10 ; r1 <- r20 + r10 
	sd    r1, res(r0)  ; store r1 in res
