# D-Router
Information-Centric Networking (ICN) enables efficient content distribution through in-network caching and native multicast. However, the additional states of network intermediary nodes make ICN forwarding more burdensome, and the unpredictability of cache hits during forwarding leads to uncertain content retrieval latency.
To overcome performance bottlenecks at the router's data plane and enhance network determinism, we propose the decoupled content router with a remote content store (D-Router). This novel architecture decouples the local content store (CS) from routers and introduces the remote CS device for centralized management of important contents. When interest packets arrive at a router whose CS is overloaded, we ensure certainty by forwarding them to the remote CS for processing if the requested content is cached there, preventing blocking before the router's local CS and potential random cache hits along the forwarding path. The dual-path forwarding is supported through the design of routers and a dual-path routing protocol.
Experiments show significant improvements in data plane performance compared to classic architecture: a 30\% reduction in round-trip time (RTT) and a 25\% increase in throughput. Additionally, the determinism and jitter of the network are improved, as the RTT for every content request managed by the remote CS is kept within an acceptable range for users. Furthermore, the decoupling of CS makes it easier to deploy network upgrades for network administrators.