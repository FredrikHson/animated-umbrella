#!/bin/bash

version=$(blender --version | grep hash | sed 's/.*: //')

getfile()
{
	path=$1
	filename=$(basename $1)
	mkdir -p src/blender/$(dirname $path)
	curl https://git.blender.org/gitweb/gitweb.cgi/blender.git/blob_plain/${version}:/$path > src/blender/$path
}

getfile "source/blender/makesdna/DNA_key_types.h"
getfile "source/blender/makesdna/DNA_listBase.h"
getfile "source/blender/makesdna/DNA_ID.h"
getfile "source/blender/makesdna/DNA_defs.h"
getfile "source/blender/blenlib/BLI_sys_types.h"

