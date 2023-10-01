
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "pd_api.h"


static int displayFilename (const char* filename, void* userdata);
static int update (void* userdata);

void copyString (char* dst, size_t dst_len, char* src, size_t src_len);


bool files_found = false;

char* text_rows[20];
int text_rows_index = 0;


int
eventHandler (PlaydateAPI* pd, PDSystemEvent event, uint32_t arg) {
	pd->system->resetElapsedTime();

	if (event == kEventInit) {
		// Create /Data/com.sodachips.pdreader directory if it doesn't exist
		pd->file->mkdir("Texts");

		int file_err = pd->file->listfiles("Texts", displayFilename, pd, 0);
		if (file_err == -1) {
			char *err_text = pd->file->geterr();
			pd->graphics->clear(kColorWhite);
			pd->graphics->drawText(err_text, strlen(err_text), kASCIIEncoding, 10, 10);
		}
		if (! files_found) {
			char *err_text = "Please load a folder with your texts into /Data/<GameID>/Texts/\n";
			pd->graphics->clear(kColorWhite);
			pd->graphics->drawText(err_text, strlen(err_text), kASCIIEncoding, 18, 18);
		}

		pd->system->setUpdateCallback(update, pd);
	}
	
	return 0;
}


static int
displayFilename (const char *filename, void* userdata) {
	PlaydateAPI* pd = userdata;

	files_found = false;
	pd->system->logToConsole("Iterated over %s\n", filename);

	size_t filename_len = strlen(filename);
	text_rows[text_rows_index] = pd->system->realloc(NULL, filename_len);
	copyString(text_rows[text_rows_index], filename_len, filename, filename_len);
	text_rows_index++;

	pd->graphics->clear(kColorWhite);
	for (int i = 0; i < text_rows_index; i++) {
		pd->graphics->drawText(text_rows[i], strlen(text_rows[i]), kASCIIEncoding, 18, 18 * (i + 1));
	}

	return 1;
}


static int
update (void* userdata) {
	PlaydateAPI* pd = userdata;

	return 1;
}


void
copyString (char* dst, size_t dst_len, char* src, size_t src_len) {
	assert(src_len >= dst_len);

	for (int i = 0; i < src_len; i++) {
		dst[i] = src[i];
	}	
}

