#include <stdio.h>
#include <string.h>

#include "open/io/io.h"
#include "open/io/indexer.h"

#include "open/internal/internal_io.h"

bool default_tokenizer(document, cursor *);

static bool doc_load(stream, document *);
static bool doc_dispose(document);

static index_delegate handler = default_tokenizer;

bool default_tokenizer(document pDoc, cursor *pToken)
{
    printf("[default indexer]\n");

    (*pToken) = Allocator.alloc(sizeof(cursor), INITIALIZED);
    if ((*pToken))
    {
        (*pToken)->pPos = pDoc->content;
        (*pToken)->length = String.length(pDoc->content);
    }

    return (*pToken) != NULL;
}
//  =========================================================

/// @brief Initialize the current indexer.
/// @param name
/// @param delegate
/// @param pIndexer
/// @return
bool indexer_init(string name, index_delegate delegate, indexer *pIndexer)
{
    printf("Sig.C Indexer [%s] ... initializing ", name);
    (*pIndexer) = Allocator.alloc(sizeof(struct io_indexer), UNINITIALIZED);

    if (!pIndexer)
    {
        //  throwErr()
        printf("\nERROR: Indexer could not be initialized ... OOPS!\n");
    }
    else
    { //  configure
        (*pIndexer)->name = name;
        if (delegate)
        {
            (*pIndexer)->index = delegate;
        }
        else
        {
            (*pIndexer)->index = handler;
        }

        printf("... ");
    }

    return (*pIndexer) != NULL;
}
/// @brief generates a cursor from the default lexer delegate
/// @param pDoc the source document
/// @param pToken the resulting cursor
/// @return true if successful; otherwise false
bool indexer_index(document pDoc, cursor *pToken)
{
    return handler(pDoc, pToken);
}
void indexer_dispose(indexer pIndexer)
{
    pIndexer->index = NULL;
    Allocator.dealloc(pIndexer);
}
//	======================================
static document doc_new(void)
{
    document doc = Allocator.alloc(sizeof(struct io_document), INITIALIZED);
    doc->source = NULL;

    return doc;
}
static bool doc_load(stream pStream, document *doc)
{
    if (pStream)
    {
        //  as long as stream is created in READ mode, this is unnecessary ...
        //      we do it here just in case to prevent an error
        if (!Stream.is_open(pStream))
        {
            if (!open_stream(pStream, READ))
            {
                return false;
            }
            // if (!Stream.open(pStream, READ))
            // {
            //     return false;
            // }
        }

        (*doc) = Allocator.alloc(sizeof(struct io_document), INITIALIZED);
        String.new(0, &(*doc)->source);
        String.copy((*doc)->source, pStream->source);
        String.new(pStream->length, &(*doc)->content);

        char *ndx = (*doc)->content;
        int pos = ndx - (*doc)->content;
        while ((*ndx = (char)fgetc(pStream->fstream)) != EOF)
        {
            ++ndx;
        }

        //  tack NULL at the end
        *ndx = '\0';
    }

    return (*doc) != NULL;
}
static bool doc_dispose(document doc)
{
    String.free(doc->source);
    String.free(doc->content);
    return Allocator.dealloc(doc);
}

const struct Document_T Document = {
    .new = &doc_new,
    .load = &doc_load,
    .dispose = &doc_dispose,
};

const struct IO_Indexer Indexer = {
    .init = &indexer_init,
    .index = &indexer_index,
    .dispose = &indexer_dispose};
