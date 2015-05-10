#ifndef MIDIFLASH_H
#define MIDIFLASH_H

void sysex_start();
void sysex_stop();

void sysex_queue_push(uint8_t b);

#endif /* MIDIFLASH_H */
