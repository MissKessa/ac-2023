
.microarch
out_of_order_retirement = true
early_branch_evaluation = true
branch_prediction = always_not_taken
exb_no_pipelined_cycles = 4
exb_instructions = [add.d, sub.d]
exc_pipelined_cycles = 3
exc_instructions = [mul.d, div.d]

.data
new1:	double 3.5, 9.5
new2:	double 7.2
result:	double 5.0
count:	hword 8

.code
	lh r10, count(r0)
	l.d f1, new1(r0)
	l.d f3, new2(r0)
repeat:
	add.d f4, f3, f3
	sub.d f4, f9, f1
	daddi r10, r10, -4
	nop
	ori r1, r0, 1
	nop
	dmul r9, r10, r1
	daddi r9, r10, 2
	daddi r10, r10, 2
	beqz r10, end
	j repeat
end:
	ori r5, r0, -100
	s.d f4, result(r0)
	
