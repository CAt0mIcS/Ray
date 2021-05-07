set(SRC
	"include/ktx.h"
	"include/ktxvulkan.h"
	"other_include/KHR/khrplatform.h"

	"lib/checkheader.c"
	"lib/errstr.c"
	"lib/etcdec.cxx"
	# "lib/etcunpack.cxx"
	"lib/filestream.c"
	"lib/filestream.h"
	# "lib/gl_format.h"
	# "lib/gl_funcptrs.h"
	# "lib/gles1_funcptrs.h"
	# "lib/gles2_funcptrs.h"
	# "lib/gles3_funcptrs.h"
	# "lib/glloader.c"
	"lib/hashlist.c"
	"lib/hashtable.c"
	"lib/ktxgl.h"
	"lib/ktxint.h"
	"lib/memstream.c"
	"lib/memstream.h"
	"lib/stream.h"
	"lib/swap.c"
	"lib/texture.c"
	"lib/uthash.h"
	"lib/vk_format.h"
	"lib/vk_funclist.inl"
	"lib/vk_funcs.c"
	"lib/vk_funcs.h"
	"lib/vkloader.c"
	"lib/writer.c"
	# "lib/writer_v1.c"
)

find_package("Vulkan" REQUIRED FATAL_ERROR)
set(PUBLIC_INC
	"include"
)

set(PRIVATE_INC
	"other_include"
	"${Vulkan_INCLUDE_DIRS}"
)

add_library("ktx" STATIC "${SRC}")
set_target_properties("ktx" PROPERTIES FOLDER "Extern")

target_link_directories("ktx" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/")

target_include_directories("ktx" PUBLIC "${PUBLIC_INC}")
target_include_directories("ktx" PRIVATE "${PRIVATE_INC}")

AddRecursiveFilters("${SRC}")
SetOutputDirectory("ktx" "")

