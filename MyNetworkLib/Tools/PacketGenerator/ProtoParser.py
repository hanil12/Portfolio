



class ProtoParser():
    def __init__(self, start_id, recv_prevfix, send_prefix):
        self.recv_pkt = [] # 수신 패킷 목록
        self.send_pkt = [] # 송신 패킷 목록
        self.total_pkt = [] # 모든 패킷 목록
        self.start_id = start_id
        self.id = start_id
        self.recv_prevfix = recv_prevfix
        self.send_prevfix = send_prefix


    def parse_proto(self, path):
        f = open(path, 'r')
        lines = f.readlines()

        for line in lines:
            if line.startswith('message') == False:
                continue

            pkt_name = line.split()[1].upper()
            if pkt_name.startswith(self.recv_prevfix):
                self.recv_pkt.append(Packet(pkt_name, self.id)) # vector의 push_back
            elif pkt_name.startswith(self.send_prevfix):
                self.send_pkt.append(Packet(pkt_name, self.id))
            else :
                continue

            self.total_pkt.append(Packet(pkt_name, self.id))
            self.id += 1

        f.close()



class Packet:
        def __init__(self, name, id):
            self.name = name
            self.id = id