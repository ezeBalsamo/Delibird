CC = gcc
CFLAGS = -g -Wall -Wextra
LIBS = -lcommons

SOURCE_DIR = src
TARGET_DIR = target

APPLICATIONS = config_updater

.PHONY : buildDirectoryCreation all clean

all : buildDirectoryCreation $(APPLICATIONS)

buildDirectoryCreation:
	@echo "About to create $(TARGET_DIR)/ (if it doesn't exist) ..."
	mkdir -p $(TARGET_DIR)

$(APPLICATIONS):
	@echo "About to compile and link with $(LIBS) and create $(APPLICATIONS) ..."
	$(CC) $(CFLAGS) $(SOURCE_DIR)/$(APPLICATIONS).c $(LIBS) -o $(TARGET_DIR)/$@

clean:
	@echo "About to delete objects and applications in $(TARGET_DIR)/ ..."
	rm -f $(TARGET_DIR)/*.o
	rm -f $(TARGET_DIR)/$(APPLICATIONS)