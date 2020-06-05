

#include <erl_nif.h>

#include "bzlib.h"

/*
LIBRARY			LIBBZ2
DESCRIPTION		"libbzip2: library for data compression"
EXPORTS
	BZ2_bzCompressInit
	BZ2_bzCompress
	BZ2_bzCompressEnd
	BZ2_bzDecompressInit
	BZ2_bzDecompress
	BZ2_bzDecompressEnd
	BZ2_bzReadOpen
	BZ2_bzReadClose
	BZ2_bzReadGetUnused
	BZ2_bzRead
	BZ2_bzWriteOpen
	BZ2_bzWrite
	BZ2_bzWriteClose
	BZ2_bzWriteClose64
	BZ2_bzBuffToBuffCompress
	BZ2_bzBuffToBuffDecompress
	BZ2_bzlibVersion
	BZ2_bzopen
	BZ2_bzdopen
	BZ2_bzread
	BZ2_bzwrite
	BZ2_bzflush
	BZ2_bzclose
	BZ2_bzerror
*/

/* Get the version of the libbzip2 library. */
static ERL_NIF_TERM libVersion(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    return enif_make_string(env, BZ2_bzlibVersion(), ERL_NIF_LATIN1);
}

/* Declare functions to export (and corresponding arity). */
static ErlNifFunc nif_funcs[] = {
    {"libVersion", 0, libVersion, 0}
};

ERL_NIF_INIT(Elixir.Bzip2, nif_funcs, NULL, NULL, NULL, NULL)