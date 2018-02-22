#include <stdio.h>
#include "DynString/dynstring.h"

int main(int    argc,
         char **argv)
{
	/**Initial string**/
	dynstr* str = dynstr_from("Hi, how are you?\nFine thanks, and you?\nI'm good!");
	printf("Original size %lu\n", str->size);

    /**Split in lines**/
	size_t n;
	dynstr** lines = dynstr_splits(str, "\n", &n);
	printf("%lu lines\n", n);
	size_t i;
	for (i = 0; i < n; i++)
	{
		printf("> %s\n", lines[i]->data);
	}
	/**Strip \n **/
	dynstr_strip(str, '\n');
	printf("%s (%lu)\n", str->data, str->size);
	return 0;
}
