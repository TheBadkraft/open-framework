#include <stdio.h>
#include <stdbool.h>

#include "../include/person.h"

void write_stats()
{
	bool has_collection = People.is();
	printf("has people [%s] cnt:%ld cap:%ld\n", has_collection ? "true" : "false", People.count(), People.capacity());
}

int main(int argc, char *argv[])
{
	person *p[6];
	p[0] = Person.new("John", "Smith");
	p[1] = Person.new("Amos", "Carter");
	p[2] = Person.new("Sarah", "Miller");
	p[3] = Person.new("Max", "Carter");
	p[4] = Person.new("John", "Miller");

	for (int i = 0; i < 5; i++)
	{
		printf("%s, %s\n", p[i]->lname->buffer, p[i]->fname->buffer);
	}
	write_stats();

	size_t capacity = People.init();
	write_stats();

	People.append(p[0]);
	People.append(p[1]);
	write_stats();

	// People.destroy(&free_person);
	People.destroy(&free_person);
}
