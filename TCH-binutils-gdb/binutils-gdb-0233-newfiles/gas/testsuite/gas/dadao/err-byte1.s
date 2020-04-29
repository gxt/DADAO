% { dg-do assemble { target dadao-*-* } }
% { dg-error "unterminated string|missing closing" "" { target dadao-*-* } 11 }
% { dg-error "unterminated string|missing closing" "" { target dadao-*-* } 13 }
% { dg-warning "end of file" "" { target dadao-*-* } 0 }

# Note that the error is detected in the preformatter, before the text
# gets to the assembler.  It also gets confused about the unterminated
# string.  Well, at least we get error messages for it, so no worries.

Main SWYM 0,0,0
 BYTE 2,"no end
 BYTE 0
