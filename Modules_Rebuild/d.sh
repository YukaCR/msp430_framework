iptables -t mangle -N wan0
iptables -t mangle -A wan0 -j MARK --set-mark 0x10
iptables -t mangle -A wan0 -j CONNMARK --save-mark  # copy packet-mark to connect-mark

# 新建 wan1 链
iptables -t mangle -N wan1
iptables -t mangle -A wan1 -j MARK --set-mark 0x20
iptables -t mangle -A wan1 -j CONNMARK --save-mark  # copy packet-mark to connect-mark

# 应用至 OUTPUT 链
iptables -t mangle -A OUTPUT -o ppp+ -p  tcp -m state --state NEW -m statistic --mode nth --every 2 --packet 0 -j wan0
iptables -t mangle -A OUTPUT -o ppp+ -p  tcp -m state --state NEW -m statistic --mode nth --every 2 --packet 1 -j wan1
iptables -t mangle -A OUTPUT -o ppp+ -p  tcp -m state --state ESTABLISHED,RELATED -j CONNMARK --restore-mark # copy connect-mark to packet-mark
iptables -t mangle -A OUTPUT -o ppp+ -p  udp -m state --state NEW -m statistic --mode nth --every 2 --packet 0 -j wan0
iptables -t mangle -A OUTPUT -o ppp+ -p  udp -m state --state NEW -m statistic --mode nth --every 2 --packet 1 -j wan1
iptables -t mangle -A OUTPUT -o ppp+ -p  udp -m state --state ESTABLISHED,RELATED -j CONNMARK --restore-mark # copy connect-mark to packet-mark
iptables -t mangle -A OUTPUT -o ppp+ -p icmp -m state --state NEW -m statistic --mode nth --every 2 --packet 0 -j wan0
iptables -t mangle -A OUTPUT -o ppp+ -p icmp -m state --state NEW -m statistic --mode nth --every 2 --packet 1 -j wan1
iptables -t mangle -A OUTPUT -o ppp+ -p icmp -m state --state ESTABLISHED,RELATED -j CONNMARK --restore-mark # copy connect-mark to packet-mark

# 应用至 PREROUTING 链
iptables -t mangle -A PREROUTING -s 222.24.12.0/24 ! -d 222.24.12.0/24 -p  tcp -m state --state NEW -m statistic --mode nth --every 2 --packet 0 -j wan0
iptables -t mangle -A PREROUTING -s 222.24.12.0/24 ! -d 222.24.12.0/24 -p  tcp -m state --state NEW -m statistic --mode nth --every 2 --packet 1 -j wan1
iptables -t mangle -A PREROUTING -s 222.24.12.0/24 ! -d 222.24.12.0/24 -p  tcp -m state --state ESTABLISHED,RELATED -j CONNMARK --restore-mark # copy connect-mark to packet-mark
iptables -t mangle -A PREROUTING -s 222.24.12.0/24 ! -d 222.24.12.0/24 -p  udp -m state --state NEW -m statistic --mode nth --every 2 --packet 0 -j wan0
iptables -t mangle -A PREROUTING -s 222.24.12.0/24 ! -d 222.24.12.0/24 -p  udp -m state --state NEW -m statistic --mode nth --every 2 --packet 1 -j wan1
iptables -t mangle -A PREROUTING -s 222.24.12.0/24 ! -d 222.24.12.0/24 -p  udp -m state --state ESTABLISHED,RELATED -j CONNMARK --restore-mark # copy connect-mark to packet-mark
iptables -t mangle -A PREROUTING -s 222.24.12.0/24 ! -d 222.24.12.0/24 -p icmp -m state --state NEW -m statistic --mode nth --every 2 --packet 0 -j wan0
iptables -t mangle -A PREROUTING -s 222.24.12.0/24 ! -d 222.24.12.0/24 -p icmp -m state --state NEW -m statistic --mode nth --every 2 --packet 1 -j wan1
iptables -t mangle -A PREROUTING -s 222.24.12.0/24 ! -d 222.24.12.0/24 -p icmp -m state --state ESTABLISHED,RELATED -j CONNMARK --restore-mark # copy connect-mark to packet-mark

# 对内网流量进行 SNAT
iptables -t nat -A POSTROUTING -o ppp+ -j MASQUERADE