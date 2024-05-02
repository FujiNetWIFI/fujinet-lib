# Generating stub functions for new platform

When adding a new platform, the following can be run to generate stub functions for the various devices.

This is a very simple parser of the include files to find function definitions and convert them to stub files that can then be implemented.

## How the scripts work

There are various assumptions in order to make the script very simple.

- Read from the library device include files "fujinet-fuji.h", "fujinet-network.h"
- Look for lines that start with one of the letters "b", "u", "i", or "v" (for bool, uint, int, void)
- And have an open bracket on the line
- Assume the line is the full function definition (i.e. all parameters on 1 line)
- Remove the semi-colon from the line
- Generate a stub file that returns "true" for bool, nothing for void, or 0 for number types.
- Saves stub to "function name".cpp in appropriate subdir of src

## setup

Create the initial directories for the new platform

```shell
NEW_PLATFORM=commodore
mkdir ${NEW_PLATFORM}/src/{bus,fn_fuji,fn_network}
```

## fuji

```shell
grep '^[buiv].*(' fujinet-fuji.h | while read f; do FILE_NAME=$(echo $f | cut -d\( -f1 | awk '{print $2}').cpp;  echo $f | awk '{
  LINE=gsub(/;/, "")
  printf("#include <stdbool.h>\n#include <stdint.h>\n#include \"fujinet-fuji.h\"\n\n%s\n{\n", $0)
  if ($1 == "bool") {
    printf("\treturn true;\n}\n")
  } else if ($1 == "void") {
    printf("}\n")
  } else {
    printf("\treturn 0;\n}\n")
  }
}' > $NEW_PLATFORM/src/fn_fuji/${FILE_NAME}; done
```

## network

```shell
grep '^[buiv].*(' fujinet-network.h | while read f; do FILE_NAME=$(echo $f | cut -d\( -f1 | awk '{print $2}').cpp;  echo $f | awk '{
  LINE=gsub(/;/, "")
  printf("#include <stdbool.h>\n#include <stdint.h>\n#include \"fujinet-network.h\"\n\n%s\n{\n", $0)
  if ($1 == "bool") {
    printf("\treturn true;\n}\n")
  } else if ($1 == "void") {
    printf("}\n")
  } else {
    printf("\treturn 0;\n}\n")
  }
}' > $NEW_PLATFORM/src/fn_network/${FILE_NAME}; done
```
