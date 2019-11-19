/* Target Definitions for DADAO. */

#ifndef GCC_DADAO_H
#define GCC_DADAO_H

#define BITS_BIG_ENDIAN		1
#define BYTES_BIG_ENDIAN	1
#define WORDS_BIG_ENDIAN	1

#define UNITS_PER_WORD		8

/* register desciption */
#define FIRST_PSEUDO_REGISTER (64 * 2)

#define REGISTER_NAMES {						\
  /* reg-data */							\
  "zero", "rd01", "rd02", "rd03", "rd04", "rd05", "rd06", "rd07",	\
  "rd08", "rd09", "rd10", "rd11", "rd12", "rd13", "rd14", "rd15",	\
  "rd16", "rd17", "rd18", "rd19", "rd20", "rd21", "rd22", "rd23",	\
  "rd24", "rd25", "rd26", "rd27", "rd28", "rd29", "rd30", "rd31",	\
  "rd32", "rd33", "rd34", "rd35", "rd36", "rd37", "rd38", "rd39",	\
  "rd40", "rd41", "rd42", "rd43", "rd44", "rd45", "rd46", "rd47",	\
  "rd48", "rd49", "rd50", "rd51", "rd52", "rd53", "rd54", "rd55",	\
  "rd56", "rd57", "rd58", "rd59", "rd60", "rd61", "rd62", "rd63",	\
  /* reg-pointer */							\
  "null", "rp01", "rp02", "rp03", "rp04", "rp05", "rp06", "rp07",	\
  "rp08", "rp09", "rp10", "rp11", "rp12", "rp13", "rp14", "rp15",	\
  "rp16", "rp17", "rp18", "rp19", "rp20", "rp21", "rp22", "rp23",	\
  "rp24", "rp25", "rp26", "rp27", "rp28", "rp29", "rp30", "rp31",	\
  "rp32", "rp33", "rp34", "rp35", "rp36", "rp37", "rp38", "rp39",	\
  "rp40", "rp41", "rp42", "rp43", "rp44", "rp45", "rp46", "rp47",	\
  "rp48", "rp49", "rp50", "rp51", "rp52", "rp53", "rp54", "rp55",	\
  "rp56", "rp57", "rp58", "rp59", "rp60", "rp61", "rp62", "rp63",	\
  /* pseudo registers */						\
  "ra00" /* SHOULD NEVER BE USED DIRECTLY */				\
}

enum reg_class
{
  NO_REGS,				/* no registers in set */
  DATA_REGS,				/* data registers */
  POINTER_REGS,				/* pointer registers */
  ALL_REGS,				/* all registers */
  LIM_REG_CLASSES			/* max value + 1 */
}
 
#define N_REG_CLASSES		(int)LIM_REG_CLASSES

#endif /* GCC_DADAO_H */
