# Summary 

## Wheatstone Bridges summary
Componente electrónica de condicionamento de sinal, usada quando se quer passar de uma diferença de impedâncias, capacidade ou carga, para tensão.
Podem ser balanceadas ou não balanceadas,no último caso mede-se a tensão à saída, ou a tensão fica a zero à mesma mas resistência muda. Balenceadas não dependem da tensão de entrada.

## Controller Area Network (CAN)

CAN is mafe of 3 layers. Physical layer, Medium Access Control (MAC), Application layer.

### Physical Layer
Here is where bit transfer between the nodes occurs, including bit-timing, synchronization and enconding. 

For this a 2-wire differential bus is used (one wire CAN_H, one wire CAN_l). The bus capacity is one bit. The longest the wires, less bit rate they have.

The serial bus allows dominant or recessive bits. The bit enconding is NRZ (Non-Return-to-Zero).

Using only a single-wire provides fault tolerance against one wirebreaking or being short-cut or a 2-wire short-cut.

### Medium Access Control (MAC)

In CAN the method used to carry the messages is CSMA/CR (Carrier Sense Multiple-Access/Collision Resolution). Here, nodes delay transmissions until the bus is available for them. The conflicts are solved through the bitwise arbitration.

#### Bitwise arbitration
Packets are encapsulated with a unique identifier. Two different frames can be circulating, a data frame or a request frame.

Data frame packets have the following structure

```
|Start-of-Frame | Arbitration Field | Control Field | Data Field | CRC Field | ACK Field | EOF |

```

where CRC - Cyclic Redundancy Check. The request frame contains:

```
|Start-of-Frame | Arbitration Field | Control Field | CRC Field | ACK Field | EOF |

```

The arbitration field contains the unique identifier and the RTR (Remote Transmission Request). For request frames, RTR = 1, and domintant, RTR = 0.

Each node compares the transmitted bit level with the level monitored on the bus. If a recessive bit was sent and a dominat bit was monitored, it stops and becomes a receiver.

A lower identifier means a higher priority. Thus, for the same identifier, data frames take precedence over request frames.

In CAN 2.0A, the identifiers are 11-bit, in CAN2.0B is 29-bit, and contains more fields beyond the 2 said above.

### Frame Coding

To allow an accurate syncronization of receivers sampling clocks, sometimes bit-stuffing occurs, where an additional complementary bit is inserted in order to mantain the bit rate. Here it happens upon the detection of five identical consecutive bits.

### Errors

There are a few ways to detect errors: 
- Bit errors, wrong sumcheck.
- Bit-stuffing errors, more than five consecutive bits with the same polarity.
- CRC errors, mismatch between sent and received CRC.
- ACK errors, lack of acknowledgment.
- Form errors, violation of frame form.
When one of these occurs, an error frame is bradcasted.

Each node has 2 counters, Transmit-Error-Counter (TEC) and Receive-Error-Counter (REC). Nodes are a failure to the world when they have an increment bigger than correct nodes. To confine the errors, the nodes can make use of 3 different states, error active (operating normally), error peassive (wait 8-bit idle period before doing anything) and Off-bus (does absolutely nothing).

If a frame is failed, an extra frame validity condition is issued. Ofr example in the cases where there is no error until the last bit of the EOF.


## PHY and MAC

### Difficulties with WSN communications
- CSMA/CD is not possible. On this method, nodes send information at the same time and when it collides, it gets destroyed and is sent randomly later. On ethernet this collisions are defined as a cable energy pike, on wireless this is not possible to detect.
- Receivers frequency interferece is what determines the success, and sometimes the sender can't observe this before hand.

### Requirements for energy-efficiency
- Minimize collisions
- Minimize overheading (packets destined for other nodes)
- Minimize idle listening (trying to receive when nobody is sending)
- Minimize communication overhead
- Low complexity is also very nice :)

To accomplish this various aspects are taken into account:

- Central vs Distributed MAC, having a central station telling when nodes ay access the medium.
It's simple and efficient for collisions, but not scalable. May be useful for clustered networks.

- Scheduled vs Contention-based MAC
When scheduled, there are no collisions, no overheading, there is low idle listening and well defined sleep times. 
However, there is a need for signaling and synchronization, causing some overhead, 
there may be clock drifts, scalability problems and its har to adapt this kind of network to changes.
If contention-based, it's simpler, scalable, adapts well to changes and to traffic load and has no signalingsync overhead.
But, is prone to collisions, idle listening and overheading.
  
### Contention-base MAC

Regarding Contention-based MAC, basic ideas are ALOHA and its son, CSMA (Carrier Sense Multiple Access).
The latter takes into account the collisions, but it can suffer from the exposed and hidden terminal scenario.
  
There is CSMA/CR,CD and CA. 
CD detecs the collision, and destroys everything.
CA avoids the collision, by sending a warning.
  
#### Exposed Terminal Scenario

It happens when the sender, S1, knows there is a another sender, S2, close. But does not know where the receivers,
R1 and R2 are. S1 senses S2 is sending a message to R2, so it doesn't send afraid of interference on receiving.
But R1 and R2 are far away from each other and nothing would happen, but S1 didn't know that.

#### Hidden Terminal Scenario

This happens when A can communicate with B. 
C can also communicate with B. 
However, A and C cannot communicate with each other since 
they cannot sense each other on the network, because they are out of range of each other.
  
#### Shut up senders

To prevent this, receivers can send a shut up message while or beofre receiving. On the first option
the same channel on where the reception is occuring witll be busy. 
On the latter, the intereferers need to store the information, this is called RTS/CTS.

#### RTS/CTS - MACA Protocol

B ask C is he can receive the transmission (Request To Send - RTS).
C agrees, sending out a Clear To Send - CTS.
If interferers hear about this, they store the information and deal with it.
B sends, C acks.
And this, my friends, it's what is called a MACA protocol, and does not fucking solve the 
hidden/exposed terminal problems, neither reduces energy.
  
#### Other protocols

Other protocols are: 
- PAMAS (Power Access Multiaccess with Signaling), same as MACA, but receiver sends 
a busy tone when receiving data. Ongoing transmission nearby destroy RTS by busy tone.
- S-MAC, same as MACA but clustered nodes have sleeping times, and times where they can exchange information.
- Timeout-MAC same as S-MAC, but where there is no traffic, it goes to sleep. The problem 
here is that when a node is waiting for another node's trasmission it might go to sleep.
- D-MAC
- Preamble Sampling
- B-MAC I and II
- SCP-MAC

### Scheduled-based MAC

- WiDom
- TRAMA, based on priorities, the hidden nodes problem can happen here too. 
If a node doesn't node about other node's existance, thus priority.

### IEEE

Combines contention and scheduled MAC.

### Others

- LEACH (Low-Energy Adaptive Clustering HIerarchy), dense network of nodes, reporting to a central sink, each node can 
reach sink directly.
- SMACS, set up directional links between neighboring nodes 
- LEMMA
- Wakeup Radio MAC protocols
- RT-Link (TDMA - Time Division Multiple Access)+ALOHA)

## Energy Management

### Energy Scavenge
- Light
- Temperature gradients
- Vibrations
- Pressure variation
- Air/liquid flow

### CPU power consumption

Power vs Energy, heat depends on power consumption, battery life depends on energy consumption.

### CMOD power consumption
- Voltage drops (V^2)
- Toggling
- Leakage

### Power Saving strategies
- Reduce power supply voltage
- Run at lower clock frequency
- Disconnect parts from power supply if not needed
- Disable functions if not needed

Goinf into power-down modes can cost time and energy, it must be determined if it's worth.

### Power management styles
- Static, does not depend on CPU activity
- Dynamic

To reduce power, race-to-dark is used when the consumptions is dominated by static power. 
Basically, run as fast as possible and then remove power supply.













