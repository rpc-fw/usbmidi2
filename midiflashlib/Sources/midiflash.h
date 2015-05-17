#ifndef MIDIFLASH_H
#define MIDIFLASH_H

void sysex_start();
void sysex_stop();

int sysex_queue_push(uint8_t b);
int sysex_process_byte(uint8_t byte);

#endif /* MIDIFLASH_H */
