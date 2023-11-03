options = -O2 -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations  \
-Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wconversion 		\
-Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers 		\
-Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel 		\
-Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE -fsanitize=address

DUMP = $(wildcard graph_dump/*.cpp)
Main_files = *.cpp

start: compile_dump compile_main exe run to_html

compile_dump:
	g++ $(DUMP) -c $(options) $(debug)

compile_main:
	g++ $(Main_files) -c $(options) $(debug)

exe:
	g++ *.o -o main.exe $(options) $(debug)

run:
	./main.exe

to_html:
	dot -Tsvg graph.dot -o browser.html

clean:
	rm -f *.o
	rm -f *.exe
