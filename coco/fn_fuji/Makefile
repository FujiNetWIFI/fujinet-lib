AR=lwar
CC=cmoc

OBJS=fuji_reset.o \
	fuji_get_ssid.o \
	fuji_scan_networks.o

libfuji.a: $(OBJS)
	$(AR) --create libfuji.a $(OBJS)

fuji_reset.o: fuji_reset.c fuji.h
	$(CC) -o fuji_reset.o -c fuji_reset.c

fuji_get_ssid.o: fuji_get_ssid.c fuji.h
	$(CC) -o fuji_get_ssid.o -c fuji_get_ssid.c

fuji_scan_networks.o: fuji_scan_networks.c fuji.h
	$(CC) -o fuji_scan_networks.o -c fuji_scan_networks.c

clean:
	$(RM) *.o *.a *~
