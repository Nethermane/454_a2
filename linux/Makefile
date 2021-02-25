# If you want text output in the window, you need to install the open
# source freetype package.  On Linux, do "sudo apt-get install libfreetype6-dev".

# If you don't have freetype, use this:

LDFLAGS  = -L. -lglfw -lGL -ldl
CXXFLAGS = -g -Wall -Wno-write-strings -Wno-parentheses -Wno-unused-result -DLINUX

# If you have installed the freetype package, use this:

#LDFLAGS  = -L. -Llib32 -lglfw -lGL -ldl -lfreetype
#CXXFLAGS = -g -I/usr/include/freetype2 -Wall -Wno-write-strings -Wno-parentheses -Wno-unused-result -DLINUX -DHAVE_FREETYPE

vpath %.cpp ../src
vpath %.c   ../src/glad/src

OBJS = font.o gbuffer.o renderer.o toon.o wavefront.o linalg.o  gpuProgram.o glad.o pixelZoom.o

EXEC = toon

all:    $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJS) $(LDFLAGS) 

# glad.o:	glad.c
# 	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *~ $(EXEC) $(OBJS) Makefile.bak

depend:	
	makedepend -Y ../src/*.h ../src/*.cpp 2> /dev/null
	@echo ""
	@echo "Now edit the Makefile to remove the ../src/ prefix of all of the dependences for the .o files"
	@echo ""

# DO NOT DELETE

font.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
font.o: ../src/headers.h ../src/glad/include/glad/glad.h
gbuffer.o: ../src/gbuffer.h
gbuffer.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
gbuffer.o: ../src/headers.h ../src/glad/include/glad/glad.h
gpuProgram.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
gpuProgram.o: ../src/glad/include/glad/glad.h
gpuProgram.o: ../src/gpuProgram.h ../src/headers.h
gpuProgram.o: ../src/headers.h ../src/glad/include/glad/glad.h
headers.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
headers.o: ../src/glad/include/glad/glad.h
linalg.o: ../src/linalg.h
pixelZoom.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
pixelZoom.o: ../src/glad/include/glad/glad.h
pixelZoom.o: ../src/glad/include/glad/glad.h
pixelZoom.o: ../src/gpuProgram.h ../src/headers.h
pixelZoom.o: ../src/pixelZoom.h ../src/gpuProgram.h ../src/headers.h
renderer.o: ../src/gbuffer.h
renderer.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
renderer.o: ../src/glad/include/glad/glad.h
renderer.o: ../src/headers.h ../src/glad/include/glad/glad.h
renderer.o: ../src/renderer.h ../src/wavefront.h ../src/seq.h
renderer.o: ../src/seq.h ../src/shadeMode.h ../src/gpuProgram.h
renderer.o: ../src/shadeMode.h ../src/gpuProgram.h ../src/gbuffer.h
renderer.o: ../src/toon.h
renderer.o: ../src/wavefront.h ../src/headers.h
seq.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
seq.o: ../src/headers.h ../src/glad/include/glad/glad.h
toon.o: ../src/font.h ../src/pixelZoom.h
toon.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
toon.o: ../src/gpuProgram.h ../src/renderer.h ../src/gbuffer.h
toon.o: ../src/headers.h ../src/glad/include/glad/glad.h
toon.o: ../src/wavefront.h ../src/seq.h ../src/shadeMode.h
wavefront.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
wavefront.o: ../src/gpuProgram.h ../src/wavefront.h ../src/seq.h
wavefront.o: ../src/headers.h ../src/glad/include/glad/glad.h
wavefront.o: ../src/seq.h ../src/shadeMode.h ../src/gpuProgram.h
wavefront.o: ../src/shadeMode.h
