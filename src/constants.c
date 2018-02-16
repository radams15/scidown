#include "constants.h"

#include <stdlib.h>
#include <string.h>

scidow_paper_size
string_to_paper(char *str) {
  if(!str)
    return A4PAPER;

  if(!strcmp(str, A0_STRING)) {
    return A0PAPER;
  }
  if(!strcmp(str, A1_STRING)) {
    return A1PAPER;
  }
  if(!strcmp(str, A2_STRING)) {
    return A2PAPER;
  }
  if(!strcmp(str, A3_STRING)) {
    return A3PAPER;
  }
  if(!strcmp(str, A4_STRING)) {
    return A4PAPER;
  }
  if(!strcmp(str, A5_STRING)) {
    return A5PAPER;
  }
  if(!strcmp(str, A6_STRING)) {
    return A6PAPER;
  }

  if(!strcmp(str, B0_STRING)) {
    return B0PAPER;
  }
  if(!strcmp(str, B1_STRING)) {
    return B1PAPER;
  }
  if(!strcmp(str, B2_STRING)) {
    return B2PAPER;
  }
  if(!strcmp(str, B3_STRING)) {
    return B3PAPER;
  }
  if(!strcmp(str, B4_STRING)) {
    return B4PAPER;
  }
  if(!strcmp(str, B5_STRING)) {
    return B5PAPER;
  }
  if(!strcmp(str, B6_STRING)) {
    return B6PAPER;
  }


  if(!strcmp(str, C0_STRING)) {
    return C0PAPER;
  }
  if(!strcmp(str, C1_STRING)) {
    return C1PAPER;
  }
  if(!strcmp(str, C2_STRING)) {
    return C2PAPER;
  }
  if(!strcmp(str, C3_STRING)) {
    return C3PAPER;
  }
  if(!strcmp(str, C4_STRING)) {
    return C4PAPER;
  }
  if(!strcmp(str, C5_STRING)) {
    return C5PAPER;
  }
  if(!strcmp(str, C6_STRING)) {
    return C6PAPER;
  }


  if(!strcmp(str, B0J_STRING)) {
    return B0J;
  }
  if(!strcmp(str, B1J_STRING)) {
    return B1J;
  }
  if(!strcmp(str, B2J_STRING)) {
    return B2J;
  }
  if(!strcmp(str, B3J_STRING)) {
    return B3J;
  }
  if(!strcmp(str, B4J_STRING)) {
    return B4J;
  }
  if(!strcmp(str, B5J_STRING)) {
    return B5J;
  }
  if(!strcmp(str, B6J_STRING)) {
    return B6J;
  }

  if(!strcmp(str, ANSIA_STRING)) {
    return ANSIAPAPER;
  }
  if(!strcmp(str, ANSIB_STRING)) {
    return ANSIBPAPER;
  }
  if(!strcmp(str, ANSIC_STRING)) {
    return ANSICPAPER;
  }
  if(!strcmp(str, ANSID_STRING)) {
    return ANSIDPAPER;
  }
  if(!strcmp(str, ANSIE_STRING)) {
    return ANSIEPAPER;
  }

  if(!strcmp(str, LETTE_STRING)) {
    return LETTERPAPER;
  }
  if(!strcmp(str, EXECU_STRING)) {
    return EXECUTIVEPAPER;
  }
  if(!strcmp(str, LEGAL_STRING)) {
    return LEGALPAPER;
  }

  return A4PAPER;
}


char *
paper_to_string(scidow_paper_size size) {
  switch(size) {
  case A0PAPER:
    return A0_STRING;
  case A1PAPER:
    return A1_STRING;
  case A2PAPER:
    return A2_STRING;
  case A3PAPER:
    return A3_STRING;
  case A4PAPER:
    return A4_STRING;
  case A5PAPER:
    return A5_STRING;
  case A6PAPER:
    return A6_STRING;

  case B0PAPER:
    return B0_STRING;
  case B1PAPER:
    return B1_STRING;
  case B2PAPER:
    return B2_STRING;
  case B3PAPER:
    return B3_STRING;
  case B4PAPER:
    return B4_STRING;
  case B5PAPER:
    return B5_STRING;
  case B6PAPER:
    return B6_STRING;

  case C0PAPER:
    return C0_STRING;
  case C1PAPER:
    return C1_STRING;
  case C2PAPER:
    return C2_STRING;
  case C3PAPER:
    return C3_STRING;
  case C4PAPER:
    return C4_STRING;
  case C5PAPER:
    return C5_STRING;
  case C6PAPER:
    return C6_STRING;

  case B0J:
    return B0J_STRING;
  case B1J:
    return B1J_STRING;
  case B2J:
    return B2J_STRING;
  case B3J:
    return B3J_STRING;
  case B4J:
    return B4J_STRING;
  case B5J:
    return B5J_STRING;
  case B6J:
    return B6J_STRING;

  case ANSIAPAPER:
    return ANSIA_STRING;
  case ANSIBPAPER:
    return ANSIB_STRING;
  case ANSICPAPER:
    return ANSIC_STRING;
  case ANSIDPAPER:
    return ANSID_STRING;
  case ANSIEPAPER:
    return ANSIE_STRING;

  case LETTERPAPER:
    return LETTE_STRING;
  case EXECUTIVEPAPER:
    return EXECU_STRING;
  case LEGALPAPER:
    return LEGAL_STRING;
  }
}


scidown_doc_class
string_to_class(char *str) {
  if(!str)
    return CLASS_ARTICLE;

  if(!strcmp(str, C_PRO_STRING))
    return CLASS_PROC;
  if(!strcmp(str, C_ART_STRING))
    return CLASS_ARTICLE;
  if(!strcmp(str, C_BOO_STRING))
    return CLASS_BOOK;
  if(!strcmp(str, C_MIN_STRING))
    return CLASS_MINIMAL;
  if(!strcmp(str, C_REP_STRING))
    return CLASS_REPORT;
  if(!strcmp(str, C_SLI_STRING))
    return CLASS_SLIDES;
  if(!strcmp(str, C_MEM_STRING))
    return CLASS_MEMOIR;
  if(!strcmp(str, C_BEA_STRING))
    return CLASS_BEAMER;

  return CLASS_ARTICLE;
}

char *
class_to_string(scidown_doc_class cls) {
  switch(cls) {
  case CLASS_PROC:
    return C_PRO_STRING;
  case CLASS_ARTICLE:
    return C_ART_STRING;
  case CLASS_BOOK:
    return C_BOO_STRING;
  case CLASS_MINIMAL:
    return C_MIN_STRING;
  case CLASS_REPORT:
    return C_REP_STRING;
  case CLASS_SLIDES:
    return C_SLI_STRING;
  case CLASS_MEMOIR:
    return C_MEM_STRING;
  case CLASS_BEAMER:
    return C_BEA_STRING;
  }
}
