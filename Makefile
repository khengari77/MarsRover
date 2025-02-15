.DEFAULT_GOAL := all

# Default target (build)
all: web_page upload monitor

#	Generate web page
web_page:
	python ./scripts/gen_web_page.py ./assets/index.html ./include/SetupPage.h 

# Build the project
build:
	pio run 

# Clean build artifacts
clean:
	pio run -t clean

# Upload firmware to the device
upload:
	pio run -t upload

# Start serial monitor
monitor:
	pio device monitor

# Shortcut: clean + build + upload
deploy: clean build upload

.PHONY: all web_page build clean upload monitor deploy
