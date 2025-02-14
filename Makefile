.DEFAULT_GOAL := all


all: web_page compile


web_page:
	python ./scripts/gen_web_page.py ./assets/index.html ./include/web_page.h 
 
compile:
	pio run
