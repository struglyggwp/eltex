CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic
POSIX_LDLIBS = -lrt

SYSTEMV_TARGETS = systemv_server systemv_client
POSIX_TARGETS = posix_server posix_client
TARGETS = $(SYSTEMV_TARGETS) $(POSIX_TARGETS)

.PHONY: all systemv posix run_systemv run_posix clean

all: $(TARGETS)

systemv: $(SYSTEMV_TARGETS)

posix: $(POSIX_TARGETS)

systemv_server: systemv_server.c
	$(CC) $(CFLAGS) $< -o $@

systemv_client: systemv_client.c
	$(CC) $(CFLAGS) $< -o $@

posix_server: posix_server.c
	$(CC) $(CFLAGS) $< -o $@ $(POSIX_LDLIBS)

posix_client: posix_client.c
	$(CC) $(CFLAGS) $< -o $@ $(POSIX_LDLIBS)

run_systemv: systemv
	./systemv_server & \
	sleep 0.2; \
	./systemv_client; \
	wait

run_posix: posix
	rm -f /dev/mqueue/posix_server_queue /dev/mqueue/posix_client_queue
	./posix_server & \
	sleep 0.2; \
	./posix_client; \
	wait

clean:
	rm -f $(TARGETS)
