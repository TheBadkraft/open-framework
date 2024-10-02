#include <stdio.h>

#include "../open/core/core.h"
#include "../open/io/indexer.h"
#include "../open/io/nonspace_tokenizer.h"

#include "../open/testing/test.h"

void _init_document(void);
void _init_indexer(void);
void _indexer_tokenize_default(void);
void _indexer_tokenize_nonspace(void);

void __output_doc_info(document);
void __output_token_info(token);

int main(int argc, string *argv)
{
	writeln("OpenPlatform: IO.Text namespace\n");

	BEGIN_SET(IO.Text.Token, true)
	{
		TEST(_init_document);
		TEST(_init_indexer);
		TEST(_indexer_tokenize_default);
		TEST(_indexer_tokenize_nonspace);
	}
	END_SET(IO.Text.Token);

	TEST_STATS();

	return 0;
}
void __output_doc_info(document doc)
{
	writefln("doc: source  -> %s", doc->source == NULL ? "NULL" : doc->source);
	writefln("     content -> %s", doc->content);
}
void __output_token_info(token tkn)
{
	writeln("tokens:");
	token current = tkn;
	string word = NULL;

	while (current)
	{
		Token.word(current, &word);
		writefln(":> %s", word);

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
void _indexer_tokenize_default(void)
{
	write_header("Indexer: using the default index tokenizer");

	document doc = Document.new();
	doc->content = "this is a test";

	indexer ndxr;
	token tkn;

	writefln("%s", Indexer.init("test", NULL, &ndxr) ? "SUCCESS" : "FAILURE");
	ndxr->tokenize(doc, &tkn);
	assert(tkn != NULL);

	string word = NULL;
	Token.word(tkn, &word);
	assert(strcmp(doc->content, word) == 0);

	__output_token_info(tkn);
	Document.dispose(doc);
	Indexer.dispose(ndxr);
	Token.free(tkn);
}
void _indexer_tokenize_nonspace(void)
{
	write_header("Indexer: using the nonspace index tokenizer");

	document doc = Document.new();
	doc->content = "letter := \"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\":|\n";

	indexer ndxr;
	token tkn;

	writefln("%s", Indexer.init("test", index_nonspace, &ndxr) ? "SUCCESS" : "FAILURE");
	ndxr->tokenize(doc, &tkn);
	assert(tkn != NULL);

	__output_token_info(tkn);
	Document.dispose(doc);
	Indexer.dispose(ndxr);
	Token.free(tkn);
}
