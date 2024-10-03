#include <stdio.h>

#include "../open/core/core.h"
#include "../open/io/indexer.h"
#include "../open/io/nonspace_indexer.h"

#include "../open/testing/test.h"

void _init_document(void);
void _init_indexer(void);
void _indexer_index_default(void);
void _indexer_index_nonspace(void);
void _cursor_equals(void);
void _cursor_not_equals(void);

void __output_doc_info(document);
void __output_token_info(cursor);

int main(int argc, string *argv)
{
	writeln("OpenPlatform: IO.Text namespace\n");

	BEGIN_SET(IO.Text.Cursor, true)
	{
		TEST(_init_document);
		TEST(_init_indexer);
		TEST(_indexer_index_default);
		TEST(_indexer_index_nonspace);
		TEST(_cursor_equals);
		TEST(_cursor_not_equals);
	}
	END_SET(IO.Text.Cursor);

	TEST_STATS();

	return 0;
}
void __output_doc_info(document doc)
{
	writefln("doc: source  -> %s", doc->source == NULL ? "NULL" : doc->source);
	writefln("     content -> %s", doc->content);
}
void __output_cursor_info(cursor tkn)
{
	writeln("cursors:");
	cursor current = tkn;
	string word = NULL;

	while (current)
	{
		Cursor.word(current, &word);
		writefln(":> %s", word[0] == '\n' ? "\\n" : word);

		current = current->next;
	}

	writeln("--------------------------------");
}

void _init_document(void)
{
	document doc = Document.new();
	assert(doc != NULL);

	doc->content = "this is a test";

	__output_doc_info(doc);
	Document.dispose(doc);
}
void _init_indexer(void)
{
	indexer ndxr;
	bool retOk = false;

	assert(retOk = Indexer.init("test", NULL, &ndxr));
	writefln("%s", retOk ? "SUCCESS" : "FAILURE");

	Indexer.dispose(ndxr);
}
void _indexer_index_default(void)
{
	write_header("Indexer: using the default index tokenizer");

	document doc = Document.new();
	doc->content = "this is a test";

	indexer ndxr;
	cursor tkn;

	writefln("%s", Indexer.init("test", NULL, &ndxr) ? "SUCCESS" : "FAILURE");
	ndxr->index(doc, &tkn);
	assert(tkn != NULL);

	string word = NULL;
	Cursor.word(tkn, &word);
	assert(strcmp(doc->content, word) == 0);

	__output_cursor_info(tkn);
	Document.dispose(doc);
	Indexer.dispose(ndxr);
	Cursor.dispose(tkn);
}
void _indexer_index_nonspace(void)
{
	write_header("Indexer: using the nonspace index tokenizer");

	document doc = Document.new();
	doc->content = "letter := \"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\"\n";

	indexer ndxr;
	cursor tkn;

	writefln("%s", Indexer.init("test", index_nonspace, &ndxr) ? "SUCCESS" : "FAILURE");
	ndxr->index(doc, &tkn);
	assert(tkn != NULL);

	__output_cursor_info(tkn);
	Document.dispose(doc);
	Indexer.dispose(ndxr);
	Cursor.dispose(tkn);
}
void _cursor_equals(void)
{
	string val = "hello";
	cursor curs = Cursor.new(val, 5);

	assert(Cursor.equals(curs, val));
}
void _cursor_not_equals(void)
{
	string val = "helLo";
	cursor curs = Cursor.new("hello", 5);

	assert(!Cursor.equals(curs, val));
}