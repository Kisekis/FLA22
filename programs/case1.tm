#Q = {q0,dup,first1,first0,first0_reverse,first0_reverse_start,first1_reverse,first1_reverse_start,accept}

#S = {0,1}

#G = {0,1,_}

#q0 = q0

#B = _

#F = {accept}

#N = 2

q0 0_ 00 rr dup
q0 1_ 11 rr dup
q0 __ __ ** accept ;empty input

dup 0_ 00 rr dup
dup 1_ 11 rr dup

dup __ __ ll reverse_start
reverse_start 00 1_ *l first0_reverse
reverse_start 11 1_ *l first1_reverse


first0_reverse *0 0_ ll first0_reverse
first0_reverse *1 1_ ll first0_reverse
first0_reverse *_ 0_ l* accept


first1_reverse *0 0_ ll first1_reverse
first1_reverse *1 1_ ll first1_reverse
first1_reverse *_ 1_ l* accept
; 100   
; 100
