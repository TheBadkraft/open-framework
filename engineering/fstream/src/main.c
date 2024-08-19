#include <stdio.h>

#include "file.h"

int main(int argc, char *argv[])
{
	printf("OpenPlatform FStream Development\n");

	char *pPath = "/.target/100/main.C";

	bool exists = File.exists(pPath);
	printf("'%s' exists [%s]\n", pPath, exists ? "true" : "false");

	file *pFile = File.new(pPath);
	printf("'%s' size: %ld\n", pFile->name, pFile->size);
	printf("************************************************\n\n");

	pFile = File.new("/.target/001/main.C");
	pFile->mode = READ | WRITE;

	printf("'%s' exists [%s]\n", pFile->name, pFile->exists ? "true" : "false");
	printf("'%s' size: %ld\n", pFile->name, pFile->size);

	stream *fStream = File.open(pFile);
	printf("'%s' stream open [%s]\n", fStream->source, fStream->is_open ? "true" : "false");
	File.close(fStream);
	printf("'%s' stream open [%s]\n", fStream->source, fStream->is_open ? "true" : "false");

	fStream = File.open(pFile);
	printf("'%s' content:\n", fStream->source);
	char c;
	while (Stream.read(fStream, &c))
	{
		printf("%c", c);
	}
	printf("\n");

	File.close(fStream);
}
