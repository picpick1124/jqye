src_main=main.c

src_lib=$(wildcard ./src/*.c)
sub_lib=$(patsubst %.c, %.o, $(src_lib))
target_lib=./lib/libexpr.so

target=main

$(target): $(src_main) $(target_lib)
	$(CC) $(src_main) -l expr -lm -L lib -I include -o $(target)

$(target_lib): $(sub_lib)
	$(CC) -shared -o $(target_lib)  $(sub_lib)

%.o:%.c
	$(CC) -c -fpic $< -o $@ -I include -lm
