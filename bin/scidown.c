#include <stdio.h>
#include "parser.h"


int main(int    argc,
         char **argv)
{
	/**Initial string**/
	dynstr* doc = dynstr_from("# TITLE\n"
	                          "Lorem ipsum *dolor* sit amet, consectetur adipiscing elit.\n"
	                          "Curabitur eleifend dui eu ~tellus~ elementum auctor.\n"
	                          "## SECTION I\n"
	                          "Donec egestas vitae **neque** ut molestie.\n"
	                          "Donec fermentum placerat lectus.\n"
	                          "## SECTION II\n"
	                          "\tA Quote block\n"
	                          "\tSecond line\n"
	                          "\tThird line\n"
	                          "### SUBSECTION I\n"
	                          "Maecenas ut _dui_ nisi."
	                          "Nullam tincidunt aliquet augue, a consectetur ***justo*** varius ut.\n");
	printf("%s\n\n===============\n\n", doc->data);
	parser standard = default_parser();
	document* res = parse(doc, standard);
	ast_print(res, "");
	return 0;
}
