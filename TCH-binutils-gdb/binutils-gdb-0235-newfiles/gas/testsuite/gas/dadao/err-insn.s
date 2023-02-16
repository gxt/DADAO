# { dg-do assemble { target dadao-*-* } }
_start:
 ldmbs rd1, rb2, rd2, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 ldmws rd1, rb2, rd2, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 ldmts rd1, rb2, rd2, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 ldmo rd1, rb2, rd2, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 ldmbu rd1, rb2, rd2, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 ldmwu rd1, rb2, rd2, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 ldmtu rd1, rb2, rd2, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 stmb rd1, rb2, rd2, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 stmw rd1, rb2, rd2, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 stmt rd1, rb2, rd2, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 stmo rd1, rb2, rd2, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 ldmrb rb1, rb2, rd2, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 stmrb rb1, rb2, rd2, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 ldmft rf1, rb2, rd2, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 ldmfo rf1, rb2, rd2, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 stmft rf1, rb2, rd2, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 stmfo rf1, rb2, rd2, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 ldmra ra1, rb2, rd2, 63;	# { dg-error "the number of registers exceeds the limit" "" }
 stmra ra1, rb2, rd2, 63;	# { dg-error "the number of registers exceeds the limit" "" }

 rd2rd rd1, rd0, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rd2rd rd0, rd1, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rd2rb rb1, rd0, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rd2rb rb0, rd1, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rb2rd rd1, rb0, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rb2rd rd0, rb1, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rb2rb rb1, rb0, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rb2rb rb0, rb1, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rd2rf rf1, rd0, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rd2rf rf0, rd1, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rf2rd rd1, rf0, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rf2rd rd0, rf1, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rf2rf rf1, rf0, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rf2rf rf0, rf1, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rd2ra ra1, rd0, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 rd2ra ra0, rd1, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 ra2rd rd1, ra0, 63;		# { dg-error "the number of registers exceeds the limit" "" }
 ra2rd rd0, ra1, 63;		# { dg-error "the number of registers exceeds the limit" "" }
