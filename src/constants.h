
#ifndef __SCIDOWN_CONSTANT_H__
#define __SCIDOWN_CONSTANT_H__


#define A0_STRING    "a0paper"
#define A1_STRING    "a1paper"
#define A2_STRING    "a2paper"
#define A3_STRING    "a3paper"
#define A4_STRING    "a4paper"
#define A5_STRING    "a5paper"
#define A6_STRING    "a6paper"

#define B0_STRING    "b0paper"
#define B1_STRING    "b1paper"
#define B2_STRING    "b2paper"
#define B3_STRING    "b3paper"
#define B4_STRING    "b4paper"
#define B5_STRING    "b5paper"
#define B6_STRING    "b6paper"

#define C0_STRING    "c0paper"
#define C1_STRING    "c1paper"
#define C2_STRING    "c2paper"
#define C3_STRING    "c3paper"
#define C4_STRING    "c4paper"
#define C5_STRING    "c5paper"
#define C6_STRING    "c6paper"

#define B0J_STRING   "b0j"
#define B1J_STRING   "b1j"
#define B2J_STRING   "b2j"
#define B3J_STRING   "b3j"
#define B4J_STRING   "b4j"
#define B5J_STRING   "b5j"
#define B6J_STRING   "b6j"


#define ANSIA_STRING "ansiapaper"
#define ANSIB_STRING "ansibpaper"
#define ANSIC_STRING "ansicpaper"
#define ANSID_STRING "ansidpaper"
#define ANSIE_STRING "ansiepaper"

#define ANSIA_STRING "ansiapaper"
#define ANSIB_STRING "ansibpaper"
#define ANSIC_STRING "ansicpaper"
#define ANSID_STRING "ansidpaper"
#define ANSIE_STRING "ansiepaper"

#define LETTE_STRING "letterpaper"
#define EXECU_STRING "executivepaper"
#define LEGAL_STRING "legalpaper"

#define C_PRO_STRING "proc"
#define C_ART_STRING "article"
#define C_BOO_STRING "book"
#define C_MIN_STRING "minimal"
#define C_REP_STRING "report"
#define C_SLI_STRING "slides"
#define C_MEM_STRING "memoir"
#define C_BEA_STRING "beamer"


typedef enum scidown_paper_size {
	A0PAPER,
	A1PAPER,
	A2PAPER,
	A3PAPER,
	A4PAPER,
	A5PAPER,
	A6PAPER,

	B0PAPER,
	B1PAPER,
	B2PAPER,
	B3PAPER,
	B4PAPER,
	B5PAPER,
	B6PAPER,

	C0PAPER,
	C1PAPER,
	C2PAPER,
	C3PAPER,
	C4PAPER,
	C5PAPER,
	C6PAPER,

	B0J,
	B1J,
	B2J,
	B3J,
	B4J,
	B5J,
	B6J,

	ANSIAPAPER,
	ANSIBPAPER,
	ANSICPAPER,
	ANSIDPAPER,
	ANSIEPAPER,

	LETTERPAPER,
	EXECUTIVEPAPER,
	LEGALPAPER
} scidow_paper_size;

typedef enum scidown_doc_class {
    CLASS_PROC,
    CLASS_ARTICLE,
    CLASS_BOOK,
    CLASS_MINIMAL,
    CLASS_REPORT,
    CLASS_SLIDES,
    CLASS_MEMOIR,
    CLASS_BEAMER
} scidown_doc_class;

scidow_paper_size string_to_paper (char *            str);
char*             paper_to_string (scidow_paper_size size);

scidown_doc_class string_to_class (char *            str);
char*             class_to_string (scidown_doc_class cls);

#endif
