
#include <stdbool.h>
#include <string.h>

#include <erl_nif.h>
#include <bzlib.h>

/* Global data. */
ErlNifResourceType *g_bz_stream_res_type;

/* Custom alloc/free functions that use Erlang's memory allocators. */
static void *bzalloc(void *ctx, int n ,int m) {
    return enif_alloc(n * m);
}

static void bzfree(void *ctx,void *p) {
    enif_free(p);
}

/* Open a bzlib stream. */
static ERL_NIF_TERM open(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    bz_stream *stream = enif_alloc_resource(g_bz_stream_res_type, sizeof(bz_stream));
    stream->bzalloc = bzalloc;
    stream->bzfree = bzfree;

    ERL_NIF_TERM term = enif_make_resource(env, stream);
    enif_release_resource(stream);
    return term;
}

/* Initialise a bzlib stream resource ready for compression.

    The bzlib stream context is initialized using the default values used by the bzip command line
    program for compression.
    TODO:
        1. Add support for controlling these values by providing functions with higher arity.
 */
static ERL_NIF_TERM compressInit(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    bz_stream *stream = NULL;
    enif_get_resource(env, argv[0], g_bz_stream_res_type, (void *)  &stream);

    int res = BZ2_bzCompressInit(stream, 9, 0, 0);
    return enif_make_atom(env, res == BZ_OK ? "ok" : "error");
}

static ERL_NIF_TERM compress(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    bz_stream *stream = NULL;
    enif_get_resource(env, argv[0], g_bz_stream_res_type, (void *)  &stream);
    ErlNifBinary in;
    ErlNifBinary out = { 0 };
    if (enif_inspect_binary(env, argv[1], &in)) {
        stream->avail_in = in.size;
        stream->next_in = (char *) in.data;
        char *temp_out = enif_alloc(BZ_MAX_UNUSED);

        while (true) {
            stream->avail_out = BZ_MAX_UNUSED;
            stream->next_out = temp_out;
            int ret = BZ2_bzCompress(stream, BZ_FINISH);

            unsigned int bytes_compressed = BZ_MAX_UNUSED - stream->avail_out;
            if (bytes_compressed) {
                // Write some compressed data
                if (out.size == 0) {
                    enif_alloc_binary(bytes_compressed, &out);
                    memcpy(out.data, temp_out, bytes_compressed);
                }
                else {
                    enif_realloc_binary(&out, out.size + bytes_compressed);
                    memcpy(&out.data[out.size - bytes_compressed - 1], stream->next_out, bytes_compressed);
                }
            }
            if (ret == BZ_STREAM_END) {
                break;
            }
        }
        enif_free(temp_out);
    }
    else {
        // TODO handle the error case.
    }

    return enif_make_binary(env, &out);
}

static ERL_NIF_TERM compressEnd(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    bz_stream *stream = NULL;
    enif_get_resource(env, argv[0], g_bz_stream_res_type, (void *)  &stream);
    int ret = BZ2_bzCompressEnd(stream);
    return enif_make_atom(env, ret == BZ_OK ? "ok" : "error");
}

/* Initialise a bzlib stream resource ready for decompression.

    The bzlib stream context is initialized using the default values used by the bzip command line
    program for decompression.
    TODO:
        1. Add support for controlling these values by providing functions with higher arity.
 */
static ERL_NIF_TERM decompressInit(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    bz_stream *stream = NULL;
    enif_get_resource(env, argv[0], g_bz_stream_res_type, (void *)  &stream);

    int res = BZ2_bzDecompressInit(stream, 0, 0);
    return enif_make_atom(env, res == BZ_OK ? "ok" : "error");
}

/* Use a bzlib stream resource to decompress a binary.
 */
static ERL_NIF_TERM decompress(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    bz_stream *stream = NULL;
    enif_get_resource(env, argv[0], g_bz_stream_res_type, (void *)  &stream);
    ErlNifBinary in;
    ErlNifBinary out = { 0 };
    if (enif_inspect_binary(env, argv[1], &in)) {
        stream->avail_in = in.size;
        stream->next_in = (char *) in.data;
        char *temp_out = enif_alloc(BZ_MAX_UNUSED);

        while (true) {
            stream->avail_out = BZ_MAX_UNUSED;
            stream->next_out = temp_out;
            int ret = BZ2_bzDecompress(stream);

            unsigned int bytes_decompressed = BZ_MAX_UNUSED - stream->avail_out;
            if (bytes_decompressed) {
                // Write some decompressed data
                if (out.size == 0) {
                    enif_alloc_binary(bytes_decompressed, &out);
                    memcpy(out.data, temp_out, bytes_decompressed);
                }
                else {
                    enif_realloc_binary(&out, out.size + bytes_decompressed);
                    memcpy(&out.data[out.size - bytes_decompressed - 1], stream->next_out, bytes_decompressed);
                }
            }
            if (ret != BZ_OK) {
                break;
            }
        }
        enif_free(temp_out);
    }
    else {
        // TODO handle the error case.
    }

    return enif_make_binary(env, &out);
}

/* Releases all memory associated with a decompression stream. */
static ERL_NIF_TERM decompressEnd(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    bz_stream *stream = NULL;
    enif_get_resource(env, argv[0], g_bz_stream_res_type, (void *)  &stream);
    int ret = BZ2_bzDecompressEnd(stream);
    return enif_make_atom(env, ret == BZ_OK ? "ok" : "error");
}

/* Get the version of the libbzip2 library. */
static ERL_NIF_TERM libVersion(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    return enif_make_string(env, BZ2_bzlibVersion(), ERL_NIF_LATIN1);
}

/* Declare functions to export (and corresponding arity). */
static ErlNifFunc nif_funcs[] = {
    {"open", 0, open, 0},
    {"compressInit", 1, compressInit, 0},
    {"compress", 2, compress, 0},
    {"compressEnd", 1, compressEnd, 0},
    {"decompressInit", 1, decompressInit, 0},
    {"decompress", 2, decompress, 0},
    {"decompressEnd", 1, decompressEnd, 0},
    {"libVersion", 0, libVersion, 0}
};

int load(ErlNifEnv *env, void **priv_data, ERL_NIF_TERM load_info)
{
    g_bz_stream_res_type = enif_open_resource_type(env, NULL, "bzstream", NULL, ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER, NULL);
    return 0;
    return g_bz_stream_res_type == NULL ? 1 : 0;
}


ERL_NIF_INIT(Elixir.Bzip2, nif_funcs, load, NULL, NULL, NULL)