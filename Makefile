MIX = mix
CFLAGS = -fPIC -std=c99 -g -O2 -Wall -Wextra -Wno-unused-parameter

ERLANG_PATH = $(shell erl -eval 'io:format("~s", [lists:concat([code:root_dir(), "/erts-", erlang:system_info(version), "/include"])])' -s init stop -noshell)
CFLAGS += -I$(ERLANG_PATH)

.PHONY: all bzip2 clean priv/bzip2_nif.so

all: bzip2 priv/bzip2_nif.so

bzip2: priv/bzip2_nif.so
	$(MIX) compile

priv/bzip2_nif.so: priv/bzip2_nif.c
	$(CC) $(CFLAGS) -shared $(LDFLAGS) -o $@ priv/bzip2_nif.c -lbz2

clean:
	$(MIX) clean
	$(RM) priv/bzip2_nif.so
