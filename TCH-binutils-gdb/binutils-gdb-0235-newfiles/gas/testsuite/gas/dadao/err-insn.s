# { dg-do assemble { target dadao-*-* } }
_start:
 ldmbs rd2, rb2, rd1, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 ldmws rd2, rb2, rd1, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 ldmts rd2, rb2, rd1, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 ldmo rd2, rb2, rd1, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 ldmbu rd2, rb2, rd1, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 ldmwu rd2, rb2, rd1, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 ldmtu rd2, rb2, rd1, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 stmb rd2, rb2, rd1, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 stmw rd2, rb2, rd1, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 stmt rd2, rb2, rd1, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 stmo rd2, rb2, rd1, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 ldmrb rb2, rb1, rd2, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 stmrb rb2, rb1, rd2, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 ldmft rf2, rb2, rd2, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 ldmfo rf2, rb2, rd2, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 stmft rf2, rb2, rd2, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 stmfo rf2, rb2, rd2, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 ldmra ra2, rb2, rd2, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 stmra ra2, rb2, rd2, 63;	# { dg-error "the number of registers exceeds the limit" "" }

 rd2rd rd2, rd0, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rd2rd rd0, rd2, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rd2rb rb2, rd0, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rd2rb rb0, rd2, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rb2rd rd2, rb0, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rb2rd rd0, rb2, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rb2rb rb2, rb0, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rb2rb rb0, rb2, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rd2rf rf2, rd0, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rd2rf rf0, rd2, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rf2rd rd2, rf0, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rf2rd rd0, rf2, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rf2rf rf2, rf0, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rf2rf rf0, rf2, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rd2ra ra2, rd0, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rd2ra ra0, rd2, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 ra2rd rd2, ra0, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 ra2rd rd0, ra2, 63;		# { dg-error "the number of registers exceeds the limit" "" }
