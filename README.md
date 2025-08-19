# ip-macToipv6Example
basic example of the process of 6to4

(This program uses very very weird techniques to perform the following operations, while following principles such as "clean code" or functional can make the structure more visibly pleasing, this code is written procedurally and also obfuscated just for the fun of it!

Note: no useless functions were added to confuse the reader more, comments are also left IN so you can understand what each section does in as little amount of time as possible without having to understand the actual code)


process: bitflip first byte of mac with mask 0x02, add ff:fe, convert ipv4 to hex, combine with boilerplate 2002,subnet and \64 for completion sake

example:

F1:F2:F3:F4:F5:F6, F1 becomes f3, split in half of 2 pairs of 3
F3,F2,F3
FF,FE
F4,F5,F6, 
then group into twos with FF and FE,
such as f3f2:f3ff:fef4:f5f6, 
for the ip convert to hex, i.e 192.168.0.1 becomes c0 a8 0 1,
combine the first two sections (192.168)
== C0A8.0.1

add boilerplate:
2002:combinedIP:subnet:newMac\64

this is a brief overview of how an ipv6 is created, for the purposes of security, it not only tells you the IPv4 but also the hardware mac address.

however this system does indeed have workarounds such as spoofing your mac and ip, it's quite fun to implement the system.

