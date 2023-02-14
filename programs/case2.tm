#Q = {q0,qs,q1,q2,q3,q4,accept,accept_t,accept_r,accept_u,accept_e,reject,reject_f,reject_a,reject_l,reject_s,reject_e}

#S = {1}

#G = {1,_,t,r,u,e,f,a,l,s}

#q0 = q0

#B = _

#F = {accept_e,reject_e}

#N = 2

q0 __ __ ** accept

q0 1_ _1 r* qs

qs _1 _1 ** accept

qs 11 11 ** q1

q1 11 _1 rr q1

q1 __ __ ** reject

q1 _1 _1 ** reject

q1 1_ 1_ ** q2

q2 1_ _1 rr q3

q3 1_ _1 r* q4

q3 __ __ ** reject

q4 11 11 *l q4

q4 _1 _1 ** accept

q4 1_ 1_ *r q1

accept 1_ __ l* accept

accept 11 _1 l* accept

accept _1 t1 r* accept_t

accept __ t_ r* accept_t

accept_t _1 r1 r* accept_r

accept_t __ r_ r* accept_r

accept_r _1 u1 r* accept_u

accept_r __ u_ r* accept_u

accept_u _1 e1 r* accept_e

accept_u __ e_ r* accept_e



reject 1_ __ l* reject

reject 11 _1 l* reject

reject _1 f1 r* reject_f

reject __ f_ r* reject_f

reject_f _1 a1 r* reject_a

reject_f __ a_ r* reject_a

reject_a _1 l1 r* reject_l

reject_a __ l_ r* reject_l

reject_l _1 s1 r* reject_s

reject_l __ s_ r* reject_s

reject_s _1 e1 r* reject_e

reject_s __ e_ r* reject_e