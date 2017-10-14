#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char *keyPrefix = "chapterCatAuto";
const char *versionString = "1.0";

int isFragmentInvalid(const char * const fragment) {
	return strchr(fragment, ';') != NULL || strchr(fragment, '\n') != NULL;
}

int main(int argc, char* argv[]) {
	if(argc < 2 || !strcmp(argv[1], "--help")) {
		printf("Usage: %s [specification file name]\n", argv[0]);
		return argc < 2;
	}
	if(!strcmp(argv[1], "--version")) {
		puts(versionString);
		return 0;
	}
	FILE *specFile = fopen(argv[1], "r");
	if(specFile) {
		fseek(specFile, 0L, SEEK_END);
		const size_t specFileSize = ftell(specFile);
		fseek(specFile, 0L, SEEK_SET);
		const size_t specBufferSize = specFileSize + 1;
		void *specBuffer = malloc(specBufferSize);
		memset(specBuffer, '\0', specBufferSize);
		fread(specBuffer, 1, specFileSize, specFile);
		fclose(specFile);
		char *specBody = specBuffer;
		const size_t bufferSize = 16384;
		char *buffer = malloc(bufferSize);
		memset(buffer, 0, bufferSize);
		size_t documentBodySize = 1;
		char *documentBody = malloc(1);
		documentBody[0] = '\0';
		size_t tocBodySize = 1;
		char *tocBody = malloc(1);
		tocBody[0] = '\0';
		int chapterNumber = 0;
		do {
			char *chapterFilename = strsep(&specBody, ";");
			char *chapterName = strsep(&specBody, "\n");
			if(chapterFilename == NULL || chapterName == NULL)
				break;
			else if(isFragmentInvalid(chapterFilename)) {
				fprintf(stderr, "Invalid fragment: %s", chapterFilename);
				break;
			}
			else if(isFragmentInvalid(chapterName)) {
				fprintf(stderr, "Invalid fragment: %s", chapterName);
				break;
			}
			FILE *chapterFile = fopen(chapterFilename, "r");
			if(chapterFile != NULL) {
				chapterNumber++;
				fseek(chapterFile, 0L, SEEK_END);
				const size_t chapterFileSize = ftell(chapterFile);
				fseek(chapterFile, 0L, SEEK_SET);
				const size_t chapterBufferSize = chapterFileSize + 1;
				char *chapterBuffer = malloc(chapterBufferSize);
				memset(chapterBuffer, 0, chapterBufferSize);
				fread(chapterBuffer, 1, chapterFileSize, chapterFile);
				fclose(chapterFile);
				snprintf(buffer, bufferSize, "# %s<span id='%s%i' />\n\n", chapterName, keyPrefix, chapterNumber);
				documentBodySize += strlen(buffer) + chapterFileSize + 2L;
				documentBody = realloc(documentBody, documentBodySize);
				strcat(documentBody, buffer);
				strncat(documentBody, chapterBuffer, chapterFileSize);
				strcat(documentBody, "\n\n");
				snprintf(buffer, bufferSize, "- [%s](#%s%i)\n\n", chapterName, keyPrefix, chapterNumber);
				tocBodySize += strlen(buffer);
				tocBody = realloc(tocBody, tocBodySize);
				strcat(tocBody, buffer);
			}
			else {
				fprintf(stderr, "Cannot open file: %s", chapterFilename);
			}
		} while(1);

		printf("%s%s", tocBody, documentBody);

		free(tocBody);		
		free(buffer);
		free(documentBody);
		free(specBuffer);
		return 0;
	}
	else {
		fprintf(stderr, "Error: Failed to open specification file.\n");
		return 1;
	}
}
