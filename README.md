# wire-shrimp

## Building

To run the project:

`$ make && ./build/wire-shrimp`

Then executable will subesquently be found in `/build/wire-shrimp`

## Diagrams

### Class diagram

```mermaid
classDiagram
    class Config {
        +string interface
        +string filter
        +int count
    }

    class EthernetHeader {
        +string dstMac
        +string srcMac
        +uint16_t etherType
    }

    class IPv4Header {
        +string srcIp
        +string dstIp
        +uint8_t protocol
        +uint8_t ttl
        +uint8_t ihl
    }

    class TCPHeader {
        +uint16_t srcPort
        +uint16_t dstPort
        +uint8_t flags
    }

    class UDPHeader {
        +uint16_t srcPort
        +uint16_t dstPort
    }

    class ParsedPacket {
        +EthernetHeader eth
        +optional~IPv4Header~ ip
        +optional~TCPHeader~ tcp
        +optional~UDPHeader~ udp
        +string protocol
        +bool valid
    }

    class Parser {
        +parse(const uint8_t* data, size_t len)$ ParsedPacket
        -parseEthernet(const uint8_t* data)$ EthernetHeader
        -parseIPv4(const uint8_t* data)$ IPv4Header
        -parseTCP(const uint8_t* data)$ TCPHeader
        -parseUDP(const uint8_t* data)$ UDPHeader
    }

    class Printer {
        +printPacket(ParsedPacket packet)$ void
        +printInterface(PcapLiveDevice* interface)$ void
    }

    class Receiver {
        -Config config
        -PcapLiveDevice* device
        -int currentPacketCount
        -bool active
        -onPacketArrives(RawPacket*, PcapLiveDevice*, void*)$ void
        +Receiver(Config cfg)
        +~Receiver()
        +start() void
        +stop() void
        +onPacket(RawPacket* packet) void
    }

    ParsedPacket *-- EthernetHeader
    ParsedPacket o-- IPv4Header
    ParsedPacket o-- TCPHeader
    ParsedPacket o-- UDPHeader

    Parser ..> ParsedPacket : creates
    Parser ..> EthernetHeader : creates
    Parser ..> IPv4Header : creates
    Parser ..> TCPHeader : creates
    Parser ..> UDPHeader : creates

    Printer ..> ParsedPacket : reads

    Receiver --> Config : holds
    Receiver ..> Parser : uses
    Receiver ..> Printer : uses
```

### Data flow

```mermaid
flowchart TD
    %% External Entities (Rectangles)
    User[User / CLI]
    Network[Network / Selected Interface]

    %% Processes (Circles/Rounded)
    P1((Parse Arguments))
    P2((Retrieve Interface Info))
    P3((Capture Packets))
    P4((Analyze Packets))

    %% Data Flows (Arrows)
    User -- "Command: wire-shrimp -n 100 -i <interface>" --> P1
    
    P1 -- "Interface Target" --> P2
    P2 -- "Interface Details" --> User
    
    P1 -- "Parameters: Count (100) & Interface" --> P3
    P3 -- "Listen/Sniff Request" --> Network
    Network -- "Raw Packets" --> P3
    
    P3 -- "Collected Raw Packets" --> P4
    P4 -- "Parsed Packet Info\n(Protocol, IPv4, etc.)" --> User
```
