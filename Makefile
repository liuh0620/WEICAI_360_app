OBJS = main.o imx6_com.o tsh_comm.o upgrade.o
TARGET = t6a_app
LIBS +=   -lpthread -L.  -lcommif   
LDFLAGS += -Wl,-rpath,.:/home/tsh/lib
$(TARGET): $(OBJS)
	$(CC)  $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBS)
	cp t6a_app ./app_update.bin
	sync
clean:
	rm -f *.o *.out t6a_app

