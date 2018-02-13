/* latex.h - LaTeX renderer and utilities */

#ifndef SCIDOWN_LATEX_H
#define SCIDOWN_LATEX_H

#include "document.h"
#include "buffer.h"

#ifdef __cplusplus
extern "C" {
#endif


/*************
 * CONSTANTS *
 *************/

typedef enum scidown_latex_flags {
	/* -- experimental flags -- */
	SCIDOWN_LATEX_CHARTER = (1 << 5),
	SCIDOWN_LATEX_GNUPLOT = (1 << 6),
	SCIDOWN_LATEX_PGFPLOT = (1 << 7),
} scidown_latex_flags;

typedef enum scidown_latex_tag {
	SCIDOWN_LATEX_TAG_NONE = 0,
	SCIDOWN_LATEX_TAG_OPEN,
	SCIDOWN_LATEX_TAG_CLOSE
} scidown_latex_tag;


/*********
 * TYPES *
 *********/
struct {
	char* figure;
	char* listing;
	char* table;
}typedef localization;


struct scidown_latex_renderer_state {
	void *opaque;

	struct {
		int header_count;
		int current_level;
		int level_offset;
		int nesting_level;
	} toc_data;

	scidown_latex_flags flags;
	html_counter counter;
	localization localization;

	/* extra callbacks */
	void (*link_attributes)(hoedown_buffer *ob, const hoedown_buffer *url, const hoedown_renderer_data *data);
};
typedef struct scidown_latex_renderer_state scidown_latex_renderer_state;


/*************
 * FUNCTIONS *
 *************/

/* hoedown_html_smartypants: process an HTML snippet using SmartyPants for smart punctuation */
void scidown_latex_smartypants(hoedown_buffer *ob, const uint8_t *data, size_t size);

/* hoedown_html_is_tag: checks if data starts with a specific tag, returns the tag type or NONE */
scidown_latex_tag scidown_latex_is_tag(const uint8_t *data, size_t size, const char *tagname);


/* hoedown_html_renderer_new: allocates a regular HTML renderer */
hoedown_renderer *scidown_latex_renderer_new(
	scidown_latex_flags render_flags,
	int nesting_level,
	localization local
) __attribute__ ((malloc));

/* hoedown_html_renderer_free: deallocate an HTML renderer */
void scidown_latex_renderer_free(hoedown_renderer *renderer);


#ifdef __cplusplus
}
#endif

#endif /** SCIDOWN_LATEX_H **/
