This is the second version of AS's transmitter. This version has included some features like: 
- LoRa messages are sent spending more time.
- The message is PlantaPiloto/"device"#"data". The first part, PlantaPiloto, checks if our message is sents correctly.

With the library "base64.hpp" we can encode the message. 

We use a delay among 500-5000 ms because of to avoid interferences between transmitters. 

