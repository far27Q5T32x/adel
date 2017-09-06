// :PATCH:

#include <cstddef>
#include <iostream>
#include <string.h>
#include <stdio.h>

#include "encryption.h"

char *readText(const char *path, size_t &size)
{
	printf("Reading file : %s\n", path);

	FILE *file = fopen(path, "rb");
	if (!file)
		return NULL;

	if (fseek(file, 0, SEEK_END)) {
		fclose(file);
		return NULL;
	}

	size = ftell(file);
	char *text = new char[size + 1];
	text[size] = 0;

	if (fseek(file, 0, SEEK_SET)) {
		fclose(file);
		delete [] text;
		return NULL;
	}

	size_t read_size = fread(text, 1, size, file);
	fclose(file);

	if (read_size != size) {
		delete [] text;
		return NULL;
	}

	return text;
}


bool writeText(const char *text, const size_t size, const char *path)
{
	printf("Writing file : %s\n", path);

	FILE *file = fopen(path, "wb");
	if (!file)
		return false;

	size_t written_size = fwrite(text, 1, size, file);
	fclose(file);

	if (written_size != size)
		return false;

	return true;
}


bool processFile(const char *path, const char option)
{
	bool success = false;
	char *text;
	size_t size;

	if (option == 'e') {
		printf("Encrypting file : %s\n", path);

		text = readText(path, size);

		if (text == NULL)
			return false;

		char *encrypted_text = encryptText(text, size, path);

		if (encrypted_text != text)
			success = writeText(encrypted_text, size, path);

		delete[] encrypted_text;
	}
	else if (option == 'd') {
		printf( "Decrypting file : %s\n", path);

		text = readText(path, size);

		if (text == NULL)
			return false;

		char *decrypted_text = decryptText(text, size, path);

		if (decrypted_text != text)
			success = writeText(decrypted_text, size, path);

		delete[] decrypted_text;
	}

	return success;
}


int main(int argc, char** argv)
{
	char option = 0;

	--argc;
	++argv;

	if (argc > 0 && (!strcmp(argv[0], "-e") || !strcmp(argv[0], "-d"))) {
		option = argv[0][1];
		--argc;
		++argv;
	}

	if (option) {
		for (int i = 0; i < argc; ++i) {
			if (!processFile(argv[i], option)) {
				printf("*** ERROR : can't process file : %s\n", argv[i]);
			}
		}
	}
	else {
		puts("*** ERROR : missing option");
		puts("Usage :");
		puts("        crypt option file1.lua file2.lua ...");
		puts("Options :");
		puts("        -e : encrypt files");
		puts("        -d : decrypt files");
	}
}


