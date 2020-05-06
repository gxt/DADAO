# { dg-do assemble { target dadao-*-* } }
Main:
 FLOT $112,$223,$41 ;# { dg-error "invalid operands" "Y field of FLOT 1" }
 FLOT $112,$223,141 ;# { dg-error "invalid operands" "Y field of FLOT 2" }
