cmd_Release/binding.node := c++ -bundle -stdlib=libc++ -undefined dynamic_lookup -Wl,-no_pie -Wl,-search_paths_first -mmacosx-version-min=10.7 -arch x86_64 -L./Release  -o Release/binding.node Release/obj.target/binding/syntax.o 
