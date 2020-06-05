MIX = mix
CFLAGS = -fPIC -std=c99 -g -O2 -Wall -Wextra -Wno-unused-parameter

ERLANG_PATH = $(shell erl -eval 'io:format("~s", [lists:concat([code:root_dir(), "/erts-", erlang:system_info(version), "/include"])])' -s init stop -noshell)
CFLAGS += -I$(ERLANG_PATH)

BZIP2_PATH = deps/libbzip2

CFLAGS += -I$(BZIP2_PATH)

.PHONY: all bzip2 clean priv/bzip2_nif.so

all: bzip2 priv/bzip2_nif.so

bzip2: priv/bzip2_nif.so
	$(MIX) compile

priv/bzip2_nif.so: priv/bzip2_nif.c
	$(MAKE) CFLAGS="$(CFLAGS)" -C $(BZIP2_PATH) libbz2.a
	$(CC) $(CFLAGS) -shared $(LDFLAGS) -o $@ priv/bzip2_nif.c $(BZIP2_PATH)/libbz2.a

clean:
	$(MIX) clean
	$(MAKE) -C $(BZIP2_PATH) clean
	$(RM) priv/bzip2_nif.so
