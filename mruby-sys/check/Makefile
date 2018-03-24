INCLUDE = ../mruby/include
OUT_DIR = $(PWD)/out/

load: load.c
	env \
	MRUBY_BUILD_DIR=$(OUT_DIR) \
	MRUBY_ROOT=../mruby \
	MRUBY_CONFIG=$(PWD)/config.rb \
	ruby ../mruby/minirake --rakefile ../mruby/Rakefile
	clang -g -I $(INCLUDE) load.c -L $(OUT_DIR)/host/lib -lmruby -lm -o load.out
