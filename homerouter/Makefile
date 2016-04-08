all: dhcpd

clean:
	-rm backup/*.bak

dhcpd:
	-sudo mkdir -p /etc/ltsp
	-sudo cp /etc/ltsp/dhcpd.conf /etc/ltsp/dhcpd.conf.`date +%Y%m%d-%H%M%S`.bak
	sudo cp dhcpd.conf /etc/ltsp/
