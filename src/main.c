
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "pd_api.h"
#include "page.h"


static int iterateFiles (const char* filename, void* userdata);
static int update (void* userdata);


bool 	g_files_found = false;
page_t 	g_page;


int
eventHandler (PlaydateAPI* pd, PDSystemEvent event, uint32_t arg) {
	int res;

	if (event == kEventInit) {
		// This will create /Data/com.sodachips.pdreader directory if it doesn't exist
		res = pd->file->mkdir("Books");

		pageInit(&g_page, pd);		

		res = pd->file->listfiles("Books", iterateFiles, pd, 0);
		if (res == -1) {
			char *err_text = pd->file->geterr();
			pd->graphics->clear(kColorWhite);
			pd->graphics->drawText(err_text, strlen(err_text), kASCIIEncoding, 10, 10);
		}
		if (! g_files_found) {
			char *err_text = "Please load a folder with your texts into /Data/<GameID>/Books/\n";
			pd->graphics->clear(kColorWhite);
			pd->graphics->drawText(err_text, strlen(err_text), kASCIIEncoding, 18, 18);
		}

event_init_end:
		pd->system->setUpdateCallback(update, pd);
	}
	
	return 0;
}


static int
iterateFiles (const char *filename, void* userdata) {
	PlaydateAPI* pd = userdata;

	g_files_found = true;
	pd->system->logToConsole("Iterated over %s\n", filename);

	uint8_t res = pageAppend(&g_page, pd, filename);
	if (res == -1) {
		pd->system->logToConsole("Page ran out of lines!");
	}

	return 1;
}


static int
update (void* userdata) {
	PlaydateAPI* pd = userdata;

	pageDisplay(&g_page, pd);

	return 1;
}


