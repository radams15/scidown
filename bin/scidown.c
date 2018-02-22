#include <stdio.h>
#include "DynString/dynstring.h"

int main(int    argc,
         char **argv)
{
	size_t n;
	size_t i;

	/**Initial string**/
	dynstr* str = dynstr_from("Hi, how are you?\nFine thanks, and you?\nI'm good!");
	printf("Original size %lu\n", str->size);

	/**Look for a string**/
	dyniter* pos = dynstr_match_all(str, "\n", &n);
	for (i = 0; i < n; i++)
	{
		printf("- %s\n", dyniter_print(pos[i]));
	}
	                                  /**Split in lines**/
	dynstr** lines = dynstr_splitc(str, '\n', &n);
	printf("%lu lines\n", n);
	for (i = 0; i < n; i++)
	{
		printf("> %s\n", lines[i]->data);
	}
	/**Strip \n **/
	dynstr_strip(str, '\n');
	printf("%s (%lu)\n", str->data, str->size);
	return 0;
}
