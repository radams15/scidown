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
	                          "Donec `fermentum *placerat* lectus`.\n"
	                          "This is a [link](www.google.com).\n"
	                          "## SECTION II\n"
	                          "\tA Quote block\n"
	                          "\tSecond line\n"
	                          "\tThird line\n"
	                          "### SUBSECTION I\n"
	                          "Maecenas ut _dui_ nisi."
	                          "Nullam tincidunt aliquet augue, a consectetur ***justo*** varius ut.\n"
	                          "```python\n"
	                          "def test():\n"
	                          "\tprint(\"hello world\")\n"
	                          "```\n"
	                          "A simple python code.\n"
	                          "this is a block equation: $$x = 3$$ where is $x$ is a variable.\n"
	                          "an image ![Alt text](http://url/ title) with alt text and url\n");
	printf("%s\n\n===============\n\n", doc->data);
	parser standard = default_parser();
	document* res = parse(doc, standard);
	ast_print(res, "");
	return 0;
}
