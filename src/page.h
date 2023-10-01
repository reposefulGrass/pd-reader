
#include <stdlib.h>
#include <string.h>

#include "pd_api.h"


#define ALPHANUM_CHARS ("abcdefghijklmnopqrstuvqxyz0123456789")


struct Page {
	// Holds the text to be rendered per line
	char** lines;

	uint8_t width;
	uint8_t char_width;

	uint8_t height;
	uint8_t char_height;

	uint8_t ver_border_size;
	uint8_t hor_border_size;
};
typedef struct Page page_t;


void
pageInit (page_t* page, PlaydateAPI* pd, LCDFont* font) {
	int display_height = pd->display->getHeight();
	int display_width = pd->display->getWidth();

	page->char_height = pd->graphics->getFontHeight(font);
	page->char_width = pd->graphics->getTextWidth(font, ALPHANUM_CHARS, sizeof(ALPHANUM_CHARS), kASCIIEncoding, 0);	
	page->height = display_height / page->char_height;
	page->width = display_width / page->char_width;

	page->lines = (page_t*) pd->system->realloc(NULL, sizeof(char*) * page->height);
	if (page->lines == NULL) {
		pd->system->logToConsole("%s:%s - Heap ran out of space!", __FILE__, __LINE__);
	}
}


uint8_t
pageAppend (page_t* page, PlaydateAPI* pd, char* str) {
	size_t len = strlen(str);
	if (len > page->width) {
		return -1;
	}

	for (int i = 0; i < page->height; i++) {
		if (page->lines[i] == NULL) {
			page->lines[i] = pd->system->realloc(NULL, len + 1);
			memcpy(page->lines[i], str, len);
			return i;
		}			
	}

	return -1;
}


void
pageDisplay (page_t* page, PlaydateAPI* pd) {
	pd->graphics->clear(kColorWhite);
	for (int i = 0; (i < page->height) && (page->lines[i] != NULL); i++) {
		pd->graphics->drawText(page->lines[i], strlen(page->lines[i]), kASCIIEncoding, 18, 18 * (i + 1));
	}
}



