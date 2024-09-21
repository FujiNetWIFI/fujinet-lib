#!/bin/bash

if [ $# -ne 2 ]; then
  echo "Usage: $(basename $0) SOURCE_DIR DESTINATION_DIR"
  echo "  This script flattens every file in any directory under SOURCE_DIR into DESTINATION_DIR and then applies 0xb1 filetype for orca makefile"
  echo "  It will fail if the SOURCE_DIR does not exist"
  exit 1
fi

SOURCE_DIR=$1
DEST_DIR=$2

if [ ! -d "${SOURCE_DIR}" ]; then
  echo "ERROR: SOURCE_DIR '${SOURCE_DIR}' does not exist."
  exit 1
fi

# Create destination directory if it does not exist
if [ ! -d "$DEST_DIR" ]; then
  mkdir -p "$DEST_DIR"
fi

TEMP_ZIP=$(mktemp -u --tmpdir source_archive_XXXXX.zip)

# Ensure the temporary file is removed on script exit or interruption
trap 'rm -f "$TEMP_ZIP"' EXIT

# Create a zip file of the entire source directory (including subdirectories)
zip -r "$TEMP_ZIP" "$SOURCE_DIR"/* >/dev/null 2>&1

# Check if destination directory exists, if not create it
if [ ! -d "$DEST_DIR" ]; then
  mkdir -p "$DEST_DIR"
fi

# Extract the zip file without preserving the directory structure
unzip -j "$TEMP_ZIP" -d "$DEST_DIR" > /dev/null 2>&1

cd $DEST_DIR
find . -type f -exec iix chtyp -t 0xb1 {} \;
FILE_LIST=$(find . -type f -exec basename {} \; | xargs -I {} echo -n "+{} " | sed 's/ $$//')
echo ${FILE_LIST}
